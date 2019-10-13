import hashlib
import json
import logging
import os
import subprocess
import tarfile
import urllib.request
from collections import namedtuple

from . import patch_util
from .configuration import Configuration, resolve_configuration_variables, make_cmake_arguments
from .directories import Directories
from .tools import untargz, download_file, git_clone
from .quicktype import Patch, Copy, Requirement
from .copy_util import copy_files
from .cmake import CMake


def required_argument(name, dictlike):
    if name not in dictlike.keys():
        raise Exception(f'Missing required argument {name}')
    return dictlike[name]


def optional_argument(name, dictlike, default=None):
    if name not in dictlike.keys():
        return default
    return dictlike[name]


def value_or(value, default):
    return value if value is not None else default


def name_version(requirement):
    return f'{requirement.name}-{requirement.version}'


def get_requirement_hash(
        requirement: Requirement,
        hash_obj=None):
    m = hashlib.sha256() if hash_obj is None else hash_obj
    m.update(requirement.name.encode())
    m.update(requirement.url.encode())
    m.update(requirement.version.encode())
    return m


def install_requirement(
        requirement: Requirement,
        configuration: Configuration,
        directories: Directories,
        force_config=None,
        force_generator=None):
    requirement.configuration = value_or(requirement.configuration, {})
    resolve_configuration_variables(requirement.configuration, configuration)
    source_directory = download(requirement, directories)
    source_dir = override_source_directory(
        requirement, source_directory)
    if requirement.patch:
        patch_util.apply_patches(source_directory, requirement.patch)
    if value_or(requirement.cmake_build, True):
        build_with_cmake(requirement, source_dir, configuration, directories,
                         force_config, force_generator)
    if requirement.copy:
        copy_dependencies(source_directory, directories, requirement.copy)


def build_with_cmake(requirement: Requirement, source_dir, configuration: Configuration, directories: Directories, force_config, force_generator):
    build_dir = directories.make_build_directory(name_version(requirement))
    cmake = CMake(source_dir, build_dir)
    cmake.generator = force_generator
    cmake.build_type = force_config
    cmake.add_options(configuration)
    cmake.add_options(requirement.configuration)
    cmake.configure()
    cmake.install()
    dump_build_info(configuration, requirement,
                    cmake.build_directory, directories.install)


def copy_dependencies(source_directory, directories: Directories, copy_targets: list):
    for target_options in copy_targets:
        source_dir = source_directory
        rel_source_dir = target_options.source_directory
        if rel_source_dir is not None:
            source_dir = os.path.join(source_directory, rel_source_dir)
        # Assume destination is directory
        copy_files(
            source_dir,
            target_options.sources,
            os.path.join(directories.install, target_options.destination),
            value_or(target_options.keep_paths, True))


def override_source_directory(requirement: Requirement, source_directory: str):
    if requirement.cmake_directory is not None:
        return os.path.join(source_directory, requirement.cmake_directory)
    return source_directory


def download(requirement: Requirement, directories: Directories):
    logging.debug('Downloading requirement %s', requirement.url)
    # TODO - Detect if tar contains only one folder, or packs sources without it
    if requirement.url.endswith('.git'):
        source_dir = os.path.join(
            directories.source, name_version(requirement))
        git_clone(requirement.url, source_dir, requirement.version)
        return source_dir
    elif requirement.url.endswith('.tar.gz'):
        req_file = download_requirement(requirement, directories)
        return untargz(req_file, directories.source)[0]
    else:
        raise Exception(
            'Invalid url provided, must end with either .git, or .tar.gz')


def download_requirement(requirement: Requirement, directories: Directories):
    name = get_requirement_hash(requirement).hexdigest()
    download_file_path = os.path.join(directories.cache, name)
    download_file(requirement.url, download_file_path)
    return download_file_path


def dump_build_info(configuration: Configuration, requirement: Requirement, build_dir: str, install_directory_hash: str):
    from .json_encoder import JsonEncoder
    with open(os.path.join(build_dir, 'iwd-build-info.json'), 'w') as f:
        json.dump({
            'user-configuration': configuration,
            'requirement': requirement
        }, f, indent=4, cls=JsonEncoder)

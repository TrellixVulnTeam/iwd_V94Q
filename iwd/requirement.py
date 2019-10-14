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


class UserSettings:
    def __init__(self, configuration: Configuration, force_config: str = None, force_generator: str = None):
        self.configuration = configuration
        self.force_config = force_config
        self.force_generator = force_generator


class RequirementHandler:
    def __init__(self, requirement: Requirement, directories: Directories):
        self.__requirement_set_defaults(requirement)
        self.requirement = requirement
        self.directories = directories
        self.source_directory = download(self)

    def __requirement_set_defaults(self, requirement: Requirement):
        requirement.configuration = value_or(requirement.configuration, {})
        requirement.cmake_build = value_or(requirement.cmake_build, True)

    @property
    def cmake_directory(self):
        if self.requirement.cmake_directory is not None:
            return os.path.join(self.source_directory, self.requirement.cmake_directory)
        return self.source_directory

    @property
    def name_version(self):
        return name_version(self.requirement)


def install_requirement(
        requirement: Requirement,
        directories: Directories,
        user_settings: UserSettings):
    requirement_handler = RequirementHandler(requirement, directories)
    if requirement.patch:
        patch_util.apply_patches(
            requirement_handler.source_directory, requirement.patch)
    if requirement.cmake_build:
        build_with_cmake(requirement_handler, user_settings)
    if requirement.copy:
        copy_dependencies(requirement_handler, requirement.copy)


def build_with_cmake(requirement_handler: RequirementHandler, user_settings: UserSettings):
    requirement = requirement_handler.requirement
    directories = requirement_handler.directories
    resolve_configuration_variables(
        requirement.configuration, user_settings.configuration)
    build_dir = directories.make_build_directory(name_version(requirement))
    cmake = CMake(requirement_handler.cmake_directory, build_dir)
    cmake.generator = user_settings.force_generator
    cmake.build_type = user_settings.force_config
    cmake.add_options(user_settings.configuration)
    cmake.add_options(requirement.configuration)
    cmake.configure()
    cmake.install()
    dump_build_info(user_settings.configuration, requirement,
                    cmake.build_directory, directories.install)


def copy_dependencies(requirement_handler: RequirementHandler, copy_targets: list):
    source_directory = requirement_handler.source_directory
    directories = requirement_handler.directories
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


def download(requirement_handler: RequirementHandler):
    requirement = requirement_handler.requirement
    directories = requirement_handler.directories
    logging.debug('Downloading requirement %s', requirement.url)
    # TODO - Detect if tar contains only one folder, or packs sources without it
    if requirement.url.endswith('.git'):
        source_dir = os.path.join(
            directories.source, requirement_handler.name_version)
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

import argparse
import copy
from .configuration import Configuration
from .directories import Directories, makedirs
import hashlib
import json
import logging
import os
import re
from .requirement import Requirement
import subprocess
import tarfile
import tempfile
import urllib.request


EXPRESSION_RULE = re.compile(
    r'\$\((?P<key>\w+)\)')


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('cmake_args', nargs='*')
    args = parser.parse_args()
    return Configuration.from_arguments(args.cmake_args)


class Context:
    def __init__(self, directories, requirement, source_dir, configuration):
        self.directories = directories
        self.requirement = requirement
        self.source_dir = source_dir
        self.configuration = configuration


def join_dictionaries(a: dict, b: dict):
    result = copy.copy(a)
    result.update(b)
    return result


def parse_requirements(requirements_file_path: str):
    with open(requirements_file_path, 'r') as f:
        data = json.load(f)
        # TODO - Create and validate schema
        return [Requirement(**x) for x in data['requirements']]


def untargz(targzfile_path: str, output_directory: str):
    with tarfile.open(targzfile_path, 'r:gz') as tar:
        tar.extractall(path=output_directory)
        return [os.path.join(output_directory, x.name) for x in tar.getmembers()]


def create_cmake_args(configuration: Configuration, requirement: Requirement):
    return configuration.as_cmake_args() + Configuration(requirement.configuration).as_cmake_args()


def configuration_requirement_hash(requirement: Requirement, configuration: Configuration):
    return configuration.get_hash(requirement.get_hash()).hexdigest()


def download_requirement(requirement: Requirement, directories: Directories):
    name = requirement.get_hash().hexdigest()
    download_file_path = os.path.join(directories.cache, name)
    if not os.path.isfile(download_file_path):
        # TODO - When possible, this should print nice download status
        urllib.request.urlretrieve(
            requirement.url, filename=download_file_path)
    return download_file_path


def download_and_extract(requirement: Requirement, directories: Directories):
    req_file = download_requirement(requirement, directories)
    return untargz(req_file, directories.source)[0]


def subprocess_call(args):
    # TODO - Use logging
    print('-- Calling ' + ' '.join(args))
    subprocess.check_call(args)


def dump_build_info(configuration: Configuration, requirement: Requirement, build_dir: str, install_directory_hash: str):
    with open(os.path.join(build_dir, 'iwd-build-info.json'), 'w') as f:
        json.dump({
            'user-configuration': configuration.data,
            'requirement': requirement.__dict__
        }, f, indent=4)


def cmake(configuration: dict, requirement: Requirement, directories: Directories, source_dir: str):
    config_req_hash = configuration_requirement_hash(
        requirement, configuration)
    build_dir = makedirs(directories.build, config_req_hash)

    cmake_args = create_cmake_args(configuration, requirement)
    source_directory = requirement.override_source_directory(
        source_dir)
    # TODO - Handle windows case, when there is need to avoid multiconfig generator
    cmake_call_base = [
        'cmake', '-S', source_directory, '-B', build_dir
    ]
    subprocess_call(cmake_call_base + cmake_args)
    subprocess_call(['cmake', '--build', build_dir, '--target', 'install'])
    dump_build_info(configuration, requirement, build_dir, config_req_hash)


def main():
    configuration = parse_args()
    install_directory_name = configuration.get_hash().hexdigest()
    directories = Directories(os.getcwd())

    requirements = parse_requirements('requirements.txt')
    install_prefix = os.path.join(
        directories.install, install_directory_name)
    # TODO - Store the install_prefix, and configuration info in some nice readable text file
    # for future usage
    configuration['CMAKE_INSTALL_PREFIX'] = install_prefix

    for requirement in requirements:
        source_directory = download_and_extract(
            requirement, directories)
        cmake(configuration, requirement,
              directories, source_directory)

    print(
        f'--Done! Use -DCMAKE_PREFIX_PATH={install_prefix} while configuring your project')


if __name__ == '__main__':
    main()

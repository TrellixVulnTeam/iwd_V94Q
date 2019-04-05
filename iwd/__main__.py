import argparse
import copy
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
ARGUMENT_REGEX = re.compile(r'\w+=[\w+/]')


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('cmake_args', nargs='*')
    args = parser.parse_args()

    # TODO - This can be done more flexible with regex matching
    # Allow -D inputs and throw nice exception when invalid syntax
    def gen():
        for arg in args.cmake_args:
            x, y = arg.split('=')
            yield x, y
    return dict(gen())


class Directories:
    def __init__(self, working_directory):
        self.cache = makedirs(working_directory, '.cache')
        self.build = makedirs(working_directory, '.deps', 'build')
        self.source = makedirs(working_directory, '.deps', 'source')
        self.install = makedirs(
            working_directory, '.deps', 'install')


class Context:
    def __init__(self, directories, requirement, source_dir, configuration):
        self.directories = directories
        self.requirement = requirement
        self.source_dir = source_dir
        self.configuration = configuration


def makedirs(*args):
    path = os.path.join(*args)
    os.makedirs(path, exist_ok=True)
    return path


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


def requirement_hash(requirement: Requirement):
    m = hashlib.sha256()
    m.update(requirement.name.encode())
    m.update(requirement.url.encode())
    m.update(requirement.version.encode())
    return m.hexdigest()


def create_cmake_args(configuration: dict, requirement: Requirement):
    merged_args = join_dictionaries(
        requirement.configuration, configuration)
    # TODO - Use list comprehension

    def gen():
        for key, value in merged_args.items():
            match = EXPRESSION_RULE.match(value)
            # TODO - This should allow for text modification, i.e. usage of $(VALUE)/hardcoded
            if match is not None:
                value = configuration[match.groupdict()['key']]
            yield f'-D{key}={value}'

    return list(gen())


def configuration_requirement_hash(requirement: Requirement, configuration: dict):
    m = hashlib.sha256()
    m.update(requirement.name.encode())
    m.update(requirement.url.encode())
    m.update(requirement.version.encode())
    for k, v in configuration.items():
        m.update(f'-D{k}={v}'.encode())
    return m.hexdigest()


def configuration_hash(configuration: dict):
    m = hashlib.sha256()
    for k, v in configuration.items():
        m.update(f'-D{k}={v}'.encode())
    return m.hexdigest()


def download_requirement(requirement: Requirement, directories: Directories):
    name = requirement_hash(requirement)
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


def main():
    configuration = parse_args()
    install_directory_name = configuration_hash(configuration)
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

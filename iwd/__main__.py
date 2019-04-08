import argparse
import copy
from .configuration import Configuration
from .directories import Directories, makedirs
import hashlib
import json
from .json_encoder import JsonEncoder
import logging
import os
import re
from .requirement import Requirement
import subprocess
import tarfile
import tempfile
import urllib.request


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


def parse_requirements(requirements_file_path: str):
    with open(requirements_file_path, 'r') as f:
        data = json.load(f)
        # TODO - Create and validate schema
        return [Requirement(**x) for x in data['requirements']]


def make_install_id(requirements_file_path, configuration: Configuration):
    m = hashlib.sha256()
    with open(requirements_file_path, 'rb') as f:
        m.update(f.read())
    configuration.get_hash(m)
    return m.hexdigest()


def main():
    configuration = parse_args()
    install_directory_name = make_install_id(
        'requirements.txt', configuration)
    directories = Directories(os.getcwd())

    requirements = parse_requirements('requirements.txt')
    install_prefix = os.path.join(
        directories.install, install_directory_name)
    # TODO - Store the install_prefix, and configuration info in some nice readable text file
    # for future usage
    configuration['CMAKE_INSTALL_PREFIX'] = install_prefix

    for requirement in requirements:
        requirement.install(
            configuration, directories, install_prefix)

    print(
        f'--Done! Use -DCMAKE_PREFIX_PATH={install_prefix} while configuring your project')


if __name__ == '__main__':
    main()

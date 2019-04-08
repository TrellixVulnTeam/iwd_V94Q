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


def parse_requirements(requirements_file_path: str):
    with open(requirements_file_path, 'r') as f:
        data = json.load(f)
        # TODO - Create and validate schema
        return [Requirement(**x) for x in data['requirements']]


def main():
    configuration = parse_args()
    directories = Directories(os.getcwd())
    requirements = parse_requirements('requirements.txt')
    # TODO - Store the install_prefix, and configuration info in some nice readable text file
    # for future usage
    configuration['CMAKE_INSTALL_PREFIX'] = directories.install

    for requirement in requirements:
        requirement.install(
            configuration, directories)

    print(
        f'--Done! Use -DCMAKE_PREFIX_PATH={directories.install} while configuring your project')


if __name__ == '__main__':
    main()

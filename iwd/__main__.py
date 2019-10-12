import argparse
import copy
import hashlib
import json
import logging
import os
import re
import subprocess
import tarfile
import tempfile
import urllib.request

from jsonschema import validate

from .configuration import Configuration
from .directories import Directories, makedirs
from .json_encoder import JsonEncoder
from .requirement import Requirement, install_requirement

CMAKE_FILE_TEMPLATE = """\
set(IWD_INSTALL_PREFIX {INSTALL_PREFIX})
set(CMAKE_PREFIX_PATH {INSTALL_PREFIX})
"""


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('-B', '--build-dir',
                        default=os.path.join(os.getcwd(), 'build'))
    parser.add_argument(
        '--config',
        help='When using multi config cmake generator, force specific configuration to build',
        default=None)
    parser.add_argument(
        '-G',
        '--generator',
        help='Force specific cmake generator to be used',
        default=None)
    parser.add_argument('cmake_args', nargs='*')
    args = parser.parse_args()
    return args, Configuration.from_arguments(args.cmake_args)


def parse_json(file_path, schema=None):
    with open(file_path, 'r') as f:
        data = json.load(f)
        if schema is not None:
            validate(data, schema)
        return data


def parse_requirements(requirements_file_path: str):
    file_directory = os.path.dirname(__file__)
    schema = parse_json(os.path.join(file_directory, 'schema.json'))
    data = parse_json(requirements_file_path, schema=schema)
    return [Requirement(**x) for x in data['requirements']]


def write_cmake_file(directories):
    with open(os.path.join(directories.working_directory, 'iwd.cmake'), 'w') as f:
        f.write(CMAKE_FILE_TEMPLATE.format(INSTALL_PREFIX=directories.install))


def main():
    args, configuration = parse_args()
    directories = Directories(args.build_dir)
    requirements = parse_requirements('iwd.json')
    configuration['CMAKE_INSTALL_PREFIX'] = directories.install

    for requirement in requirements:
        install_requirement(
            requirement,
            configuration, directories, args.config, args.generator)
    write_cmake_file(directories)
    print(
        f'--Done! Use -DCMAKE_PREFIX_PATH={directories.install} while configuring your project')
    print('-- or add include(iwd.cmake) in your CMakeLists.txt file')


if __name__ == '__main__':
    main()

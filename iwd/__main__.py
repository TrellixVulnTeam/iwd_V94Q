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

CMAKE_FILE_TEMPLATE = """ \
set(IWD_INSTALL_PREFIX {INSTALL_PREFIX})
set(CMAKE_PREFIX_PATH {INSTALL_PREFIX})

"""


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('-B', '--build-dir',
                        default=os.path.join(os.getcwd(), 'build'))
    parser.add_argument('cmake_args', nargs='*')
    args = parser.parse_args()
    return args, Configuration.from_arguments(args.cmake_args)


def parse_requirements(requirements_file_path: str):
    with open(requirements_file_path, 'r') as f:
        data = json.load(f)
        # TODO - Create and validate schema
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
        requirement.install(
            configuration, directories)
    write_cmake_file(directories)
    print(
        f'--Done! Use -DCMAKE_PREFIX_PATH={directories.install} while configuring your project')
    print('-- or add include(iwd.cmake) in your CMakeLists.txt file')


if __name__ == '__main__':
    main()

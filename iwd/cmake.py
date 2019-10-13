
import os
import subprocess

from .configuration import Configuration, make_cmake_arg
from .directories import Directories
from .quicktype import Requirement


class CMake:
    def __init__(self, source_directory, build_directory):
        self.generator = None
        self.build_type = None
        self.configuration = {}
        self.source_directory = source_directory
        self.build_directory = build_directory

    def set_option(self, name, value):
        if isinstance(value, bool):
            self.configuration[name] = "ON" if value else "OFF"
        self.configuration[name] = str(value)

    def add_options(self, options):
        for k, v in options.items():
            self.set_option(k, v)

    def configure(self):
        cmake_call = [
            'cmake',
            '-S',
            self.source_directory,
            '-B', self.build_directory]

        if self.generator:
            cmake_call += ['-G', self.generator]

        cmake_call += [make_cmake_arg(k, v)
                       for k, v in self.configuration.items()]

        subprocess.check_call(cmake_call)

    def install(self):
        cmake_call = [
            'cmake',
            '--build',
            self.build_directory,
            '--target',
            'install']
        if self.build_type:
            cmake_call += ['--config', self.build_type]
        subprocess.check_call(cmake_call)

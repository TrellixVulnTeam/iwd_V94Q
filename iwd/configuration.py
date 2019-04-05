from collections import UserDict
import hashlib
import re


ARGUMENT_REGEX = re.compile(r'\w+=[\w+/]')


def make_cmake_arg(key, value):
    return f'-D{key}={value}'


class Configuration(UserDict):
    def __init__(self, initial_data):
        super().__init__(initial_data)

    def get_hash(self, hash_obj=None):
        m = hashlib.sha256() if hash_obj is None else hash_obj
        for k, v in self.data.items():
            m.update(make_cmake_arg(k, v).encode())
        return m

    def as_cmake_args(self):
        return [make_cmake_arg(key, value) for key, value in self.data.items()]

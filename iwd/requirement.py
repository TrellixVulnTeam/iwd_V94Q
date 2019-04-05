import os
import hashlib


class Requirement:
    def __init__(self, name, version, url, configuration={}, cmake_directory=None):
        self.name = name
        self.version = version
        self.url = url
        self.configuration = configuration
        self.cmake_directory = cmake_directory

    def override_source_directory(self, source_directory_org):
        if self.cmake_directory is not None:
            return os.path.join(source_directory_org, self.cmake_directory)
        return source_directory_org

    def get_hash(self, hash_obj=None):
        m = hashlib.sha256() if hash_obj is None else hash_obj
        m.update(self.name.encode())
        m.update(self.url.encode())
        m.update(self.version.encode())
        return m

import os


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

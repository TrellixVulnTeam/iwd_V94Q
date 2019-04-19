import os
import pathlib

def makedirs(*args):
    path = pathlib.Path(os.path.join(*args)).as_posix()
    os.makedirs(path, exist_ok=True)
    return path


class Directories:
    def __init__(self, working_directory):
        self.working_directory = working_directory
        self.cache = makedirs(working_directory, '.cache')
        self.build = makedirs(working_directory, '.deps', 'build')
        self.source = makedirs(working_directory, '.deps', 'source')
        self.install = makedirs(
            working_directory, '.deps', 'install')

    def make_build_directory(self, build_id: str):
        return makedirs(self.build, build_id)

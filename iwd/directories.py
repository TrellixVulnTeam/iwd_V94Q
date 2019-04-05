import os


def makedirs(*args):
    path = os.path.join(*args)
    os.makedirs(path, exist_ok=True)
    return path


class Directories:
    def __init__(self, working_directory):
        self.cache = makedirs(working_directory, '.cache')
        self.build = makedirs(working_directory, '.deps', 'build')
        self.source = makedirs(working_directory, '.deps', 'source')
        self.install = makedirs(
            working_directory, '.deps', 'install')

from .configuration import Configuration
import os
import subprocess
import tarfile
import urllib.request
import hashlib
from .directories import Directories


def required_argument(name, dictlike):
    if name not in dictlike.keys():
        raise Exception(f'Missing required argument {name}')
    return dictlike[name]


def optional_argument(name, dictlike, default=None):
    if name not in dictlike.keys():
        return default
    return dictlike[name]


class Requirement:
    def __init__(self, **kwargs):
        self.name = required_argument('name', kwargs)
        self.version = required_argument('version', kwargs)
        self.url = required_argument('url', kwargs)
        self.configuration = Configuration(
            optional_argument('configuration', kwargs, {}))
        self.cmake_directory = optional_argument(
            'cmake_directory', kwargs, None)

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

    def download(self, directories: Directories):
        # TODO - Detect if tar contains only one folder, or packs sources without it
        if self.url.endswith('.git'):
            source_dir = os.path.join(
                directories.source, f'{self.name}-{self.version}')
            git_clone(self.url, source_dir, self.version)
            return source_dir
        elif self.url.endswith('.tar.gz'):
            req_file = download_requirement(self, directories)
            return untargz(req_file, directories.source)[0]
        else:
            raise Exception(
                'Invalid url provided, must end with either .git, or .tar.gz')


def untargz(targzfile_path: str, output_directory: str):
    with tarfile.open(targzfile_path, 'r:gz') as tar:
        tar.extractall(path=output_directory)
        return [os.path.join(output_directory, x.name) for x in tar.getmembers()]


def download_requirement(requirement: Requirement, directories: Directories):
    name = requirement.get_hash().hexdigest()
    download_file_path = os.path.join(directories.cache, name)
    if not os.path.isfile(download_file_path):
        # TODO - When possible, this should print nice download status
        urllib.request.urlretrieve(
            requirement.url, filename=download_file_path)
    return download_file_path


def git_clone(url, target_directory, version):
    if not os.path.isdir(target_directory):
        subprocess.check_call([
            'git', 'clone', '--depth', '1', '--recursive', '--branch', version,  url, str(
                target_directory)
        ])

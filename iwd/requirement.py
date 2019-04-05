import os
import tarfile
import urllib.request
import hashlib
from .directories import Directories


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

    def download(self, directories: Directories):
        # TODO - Handle a case, where url points to git repository
        # TODO - Detect if tar contains only one folder, or packs sources without it
        req_file = download_requirement(self, directories)
        return untargz(req_file, directories.source)[0]


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

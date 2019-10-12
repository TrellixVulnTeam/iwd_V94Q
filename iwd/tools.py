import logging
import os
import tarfile
import urllib.request
import subprocess


def untargz(targzfile_path: str, output_directory: str):
    logging.debug('Extracting %s to %s', targzfile_path, output_directory)
    with tarfile.open(targzfile_path, 'r:gz') as tar:
        tar.extractall(path=output_directory)
        return [os.path.join(output_directory, x.name) for x in tar.getmembers()]


def download_file(url: str, download_file_path: str, override_existing=False):
    if not os.path.isfile(download_file_path) or override_existing:
        # TODO - When possible, this should print nice download status
        urllib.request.urlretrieve(
            url, filename=download_file_path)


def git_clone(url, target_directory, version):
    if not os.path.isdir(target_directory):
        subprocess.check_call([
            'git', 'clone', '--depth', '1', '--recursive', '--branch', version,  url, str(
                target_directory)
        ])

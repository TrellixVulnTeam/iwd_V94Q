import logging
import os
import tarfile
import urllib.request


def untargz(targzfile_path: str, output_directory: str):
    logging.debug('Extracting %s to %s', targzfile_path, output_directory)
    with tarfile.open(targzfile_path, 'r:gz') as tar:
        tar.extractall(path=output_directory)
        return [os.path.join(output_directory, x.name) for x in tar.getmembers()]


def download_file(url: str, download_file_path: str):
    if not os.path.isfile(download_file_path):
        # TODO - When possible, this should print nice download status
        urllib.request.urlretrieve(
            url, filename=download_file_path)

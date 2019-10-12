import glob
import shutil
import os


def copy_files(
        source_directory,
        search_pattern,
        destination_directory,
        keep_paths=False):
    search_expr = f'{source_directory}{os.sep}{search_pattern}'

    for target_file_path in glob.iglob(search_expr, recursive=True):
        if keep_paths:
            relpath = os.path.relpath(target_file_path, source_directory)
            dest_file_path = os.path.join(
                destination_directory,
                relpath)
        else:
            dest_file_path = os.path.join(
                destination_directory,
                os.path.basename(target_file_path))
        dest_directory = os.path.dirname(dest_file_path)
        os.makedirs(dest_directory, exist_ok=True)
        shutil.copy2(target_file_path, dest_file_path)

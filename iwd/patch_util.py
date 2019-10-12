import logging
import os

from .quicktype import Patch
from .quicktype import TypeEnum as PatchType


def load_text_file(file_path):
    with open(file_path, 'r') as f:
        return f.read()


def save_text_as(text, file_path):
    with open(file_path, 'w') as f:
        f.write(text)


def replace_in_file(file_path, search_string, replace_string):
    text = load_text_file(file_path)
    if text.find(search_string) == -1:
        raise Exception(
            f'No target string {search_string} in file {file_path} found')
    if isinstance(replace_string, list):
        replace_string = '\n'.join(replace_string)
    text = text.replace(search_string, replace_string)
    save_text_as(text, file_path)


def append_text(file_path, text):
    with open(file_path, 'a') as f:
        logging.debug('Appending text to file %s', file_path)
        if isinstance(text, list):
            text = '\n'.join(text)
        f.write(text)


def apply_replace_patch(source_directory, patch: Patch):
    target_file = os.path.join(
        source_directory, patch.file)
    apply_patch_check_file(target_file)
    string = patch.pattern
    if string is None:
        raise Exception(
            f'Missing required argument `pattern` for patch of type replace')
    replacement = patch.text
    replace_in_file(target_file, string, replacement)


def apply_append_patch(source_directory, patch: Patch):
    target_file = os.path.join(
        source_directory, patch.file)
    apply_patch_check_file(target_file)
    text = patch.text
    append_text(target_file, text)


def apply_create_patch(source_directory, patch: Patch):
    target_file = os.path.join(
        source_directory, patch.file)
    text = patch.text
    with open(target_file, 'w') as f:
        if isinstance(text, list):
            for line in text:
                f.write(line + '\n')
        else:
            f.write(text)


PATCH_COMMAND_MAPPINGS = {
    PatchType.REPLACE: apply_replace_patch,
    PatchType.APPEND: apply_append_patch,
    PatchType.CREATE: apply_create_patch
}


def apply_patch_check_file(file):
    if not os.path.isfile(file):
        raise Exception(
            'Failed to apply patch on file {} because it does not exist'.format(file))


def apply_patches(source_directory, patches_arr):
    logging.debug('Applying patches in %s', source_directory)
    for patch in patches_arr:
        patch_type = patch.type
        if patch_type in PATCH_COMMAND_MAPPINGS:
            PATCH_COMMAND_MAPPINGS[patch_type](source_directory, patch)
        else:
            raise Exception(f'Invalid patch type {patch_type}')

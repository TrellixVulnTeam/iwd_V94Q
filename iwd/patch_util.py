import logging


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

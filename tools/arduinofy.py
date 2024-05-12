from os import listdir, chdir, remove, getcwd, mkdir
from os.path import basename, isfile, isdir
from shutil import rmtree, move, copytree, copy
from json import loads
from traceback import print_exc

ARDUINOFY_FILE = 'arduinofy.json'
REPOSITORY_RELATIVE_KEY = 'repository_relative'
ACTIONS_KEY = 'actions'

def not_found_message(path: str) -> str:
    '''Creates message'''

    return f'Path {path} does not exist'

def move_action(src: str, dst: str):
    '''mv'''

    if isfile(src):
        copier = copy
    elif isdir(src):
        copier = copytree
    else:
        print(not_found_message(src))
        return

    try:
        move(src, dst, copier)
    except Exception:
        print_exc()

def copy_action(src: str, dst: str):
    '''cp'''

    if isfile(src):
        copier = copy
    elif isdir(src):
        copier = copytree
    else:
        print(not_found_message(src))
        return

    try:
        copier(src, dst)
    except Exception:
        print_exc()

def delete_action(src: str, _: str):
    '''rm'''

    if isfile(src):
        deleter = remove
        remove(src)
    elif isdir(src):
        deleter = rmtree
    else:
        print(not_found_message(src))
        return

    try:
        deleter(src)
    except Exception:
        print_exc()

def mkdir_action(src: str, _: str):
    '''mkdir'''

    if isfile(src):
        print(f'Path {src} already exists')
        return
    elif isdir(src):
        print(f'Path {src} already exists')
        return

    try:
        mkdir(src)
    except Exception:
        print_exc()

action_mappings = {
    "move": move_action,
    "copy": copy_action,
    "delete": delete_action,
    "mkdir": mkdir_action
}

def main() -> bool:
    '''Returns boolean true -> sucess'''

    file_dir = __file__.replace(basename(__file__), '')
    chdir(file_dir)

    if ARDUINOFY_FILE not in listdir():
        print('Error! no arduinofy.json found.')
        return False

    with open(ARDUINOFY_FILE, 'r', encoding='utf-8') as file:
        arduinofy = loads(file.read())

    if ACTIONS_KEY not in arduinofy:
        print(f'Error! no {ACTIONS_KEY} key')
        return False

    if REPOSITORY_RELATIVE_KEY in arduinofy:
        chdir(arduinofy[REPOSITORY_RELATIVE_KEY])

    print(f'Working in {getcwd()}')

    for action in arduinofy[ACTIONS_KEY]:
        src = action['src']
        dst = None

        if 'src' not in action:
            continue

        if 'action' not in action:
            continue

        if 'dst' in action:
            dst = action['dst']

        action_mappings[action['action']](src, dst)

    return False
    return True

if __name__ == '__main__':
    if main():
        remove(__file__)

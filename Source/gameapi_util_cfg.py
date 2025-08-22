import os

# Use this to exclude default options from the menu.
# this can be useful if you'd like to make your own
# implementations of them. True | False

# If you'd prefer to skip some options, but keep
# specific ones in, you can add them back, via the
# init function. For example:
# app.add_option("Project Update", lambda: app.project_update(0))
skipDefaultTools = False

# Use this to only show default options for one coding language.
# 0 = Show all, 1 = C, 2 = C++
language = 2

# Path configuration
GAME_PATH        = 'SonicNexus'
OBJECT_PATH_NAME = 'Objects'
ALL_CODE_NAME    = 'All.cpp'
ALL_HEADER_NAME  = 'All.hpp'
GAMEAPI_INC_PATH = 'SonicNexus.hpp'
PUB_FNS_PATH     = f'{GAME_PATH}/PublicFunctions.hpp'
OBJECT_NAMESPACE = 'GameLogic'

# cmake configuration
CMAKE_PATH = f'{GAME_PATH}/Objects.cmake'
GAME_NAME  = '${GAME_NAME}' # The game directory to look into

OBJECT_PATH = f'{GAME_PATH}/{OBJECT_PATH_NAME}'
ALL_CODE_PATH = f'{OBJECT_PATH}/{ALL_CODE_NAME}'
ALL_HEADER_PATH = f'{OBJECT_PATH}/{ALL_HEADER_NAME}'

# Function/Variable definitions
app = None

def init(app_in):
    global app
    app = app_in

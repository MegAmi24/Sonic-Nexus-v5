import os

# Use this to exclude default options from the menu.
# this can be useful if you'd like to make your own
# implementations of them. True | False

# If you'd prefer to skip some options, but keep
# specific ones in, you can add them back, via the
# init function. For example:
# app.add_option("Update All.cpp/hpp & CMake Project", app.project_update)
skipDefaultTools = True

# Path configurations. By default, these are used for
# generating All.cpp/hpp & Objects.cmake, generating
# public functions, creating new objects, etc
GAME_PATH        = 'SonicNexus'
OBJECT_PATH_NAME = 'Objects'
ALL_CODE_NAME    = 'All.cpp'
ALL_HEADER_NAME  = 'All.hpp'
GAMEAPI_INC_PATH = ALL_HEADER_NAME
PUB_FNS_PATH     = f'{GAME_PATH}/PublicFunctions.hpp'
OBJECT_NAMESPACE = 'GameLogic'

# cmake configuration
CMAKE_PATH = 'Objects.cmake'
GAME_NAME  = '${MOD_NAME}' # The game directory to look into

OBJECT_PATH = f'{GAME_PATH}/{OBJECT_PATH_NAME}'
ALL_CPP_PATH = f'{OBJECT_PATH}/{ALL_CODE_NAME}'
ALL_HPP_PATH = f'{OBJECT_PATH}/{ALL_HEADER_NAME}'

# Function/Variable definitions
app = None

def init(app_in):
    global app
    app = app_in
    app.add_option('Project Update', project_update)
    app.add_option('Generate Public Functions', app.gen_pub_fns)
    app.add_option('Create Object', app.create_object)
# init -> (app_in)

def project_update():
    filenames = []
    app.add_line(f'Generating {ALL_HEADER_NAME}')
    for dir_, _, files in os.walk(OBJECT_PATH):
        for file_name in files:
            rel_dir = os.path.relpath(dir_, OBJECT_PATH)
            filenames.append(f"{rel_dir}/{file_name}")

        obj_forward_decl = [f'typedef struct {os.path.splitext(os.path.basename(f))[0]} {os.path.splitext(os.path.basename(f))[0]};\n' for f in filenames if f.endswith(".hpp") and not f.endswith(ALL_HEADER_NAME)]
        obj_includes = [f'#include "{f}"\n' for f in filenames if f.endswith(".hpp") and not f.endswith(ALL_HEADER_NAME)]

        with open(ALL_HPP_PATH, "w") as f:
            f.write('#pragma once\n\n')
            f.write(f'namespace {OBJECT_NAMESPACE}\n{{\n\n')
            f.writelines(obj_forward_decl)
            f.write(f'\n}} // namespace {OBJECT_NAMESPACE}\n\n#include "{GAME_PATH}.hpp"\n#include "Math.hpp"\n')
            f.writelines(obj_includes)

    app.success_msg_generic()
# project_update -> (self)

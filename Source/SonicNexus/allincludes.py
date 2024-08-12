# allincludes.py
# Creates an All.hpp file in the Objects folder including SonicNexus.hpp file and every object .hpp file. If All.hpp already exists, it will be overwritten.

from pathlib import Path

objects_path = "./Objects"
game_hpp = "SonicNexus.hpp"

f = open(objects_path + "/All.hpp", "w")
f.write(f"#pragma once\n\n#include \"{game_hpp}\"\n")
for path in Path(objects_path).rglob("*.hpp"):
    if path.name == "All.hpp":
        continue
    f.write(f"#include \"{path.relative_to(objects_path)}\"\n")
f.close()

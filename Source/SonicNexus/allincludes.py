# allincludes.py
# Creates an All.hpp file in the Objects folder including SonicNexus.hpp file and every object .hpp file. If All.hpp already exists, it will be overwritten.

from pathlib import Path

f = open("./Objects/All.hpp", "w")
f.write(f"#pragma once\n\n#include \"SonicNexus.hpp\"\n")
for path in Path("./Objects").rglob("*.hpp"):
    if path.name == "All.hpp":
        continue
    f.write(f"#include \"{path.relative_to("./Objects")}\"\n")
f.close()

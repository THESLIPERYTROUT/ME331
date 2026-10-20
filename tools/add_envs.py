"""Append a [env:<name>] section to platformio.ini for every sketch folder that lacks one.

A sketch folder is any folder under src/ that directly contains source files.
Folders holding only subfolders (e.g. src/midterm/part1/, src/midterm/part2/)
are searched recursively, and the env name joins the path with underscores
(midterm_part1). Folders with no source files anywhere are skipped.

Usage (from the project root):
    python tools/add_envs.py

Existing envs are never modified. Add lib_deps to new envs by hand if needed.
"""
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
INI = ROOT / "platformio.ini"
SRC = ROOT / "src"
SOURCE_EXTS = {".cpp", ".c", ".ino"}


def find_sketches(folder):
    if any(f.is_file() and f.suffix in SOURCE_EXTS for f in folder.iterdir()):
        return [folder]
    sketches = []
    for sub in sorted(p for p in folder.iterdir() if p.is_dir()):
        sketches += find_sketches(sub)
    return sketches


text = INI.read_text(encoding="utf-8")
existing = set(re.findall(r"^\[env:([^\]]+)\]", text, flags=re.MULTILINE))
sketches = [s for d in sorted(p for p in SRC.iterdir() if p.is_dir()) for s in find_sketches(d)]

blocks = ""
for sketch in sketches:
    rel = sketch.relative_to(SRC).as_posix()
    name = rel.replace("/", "_")
    if name not in existing:
        blocks += f"\n[env:{name}]\nbuild_src_filter = +<{rel}/>\n"
        print(f"Added [env:{name}]")

if blocks:
    INI.write_text(text.rstrip("\n") + "\n" + blocks, encoding="utf-8")
else:
    print("All sketch folders already have an env.")

from pathlib import Path
import re
from collections import defaultdict, Counter
root = Path(r"C:\Users\AuditorFOB\Desktop\Code\LibraryDecomp")
text = (root / "make_lib_log.txt").read_text(encoding="utf-8-sig")
text = re.sub(r"\x1b\[[0-9;]*m", "", text)
lines = text.splitlines()
print("--- first errors ---")
n = 0
for i, l in enumerate(lines):
    if (": error:" in l or l.strip().startswith("cfe: Error")) and "CMake" not in l and "CategoryInfo" not in l:
        for j in range(i, min(i + 5, len(lines))):
            print(lines[j][:220])
        print("---")
        n += 1
        if n >= 15:
            break
failed = re.findall(r"make: \*\*\* \[Makefile:\d+: ([^\]]+)\] Error", text)
print("failed", failed)
text2 = re.sub(r"undefined reference to\s*\n\s*", "undefined reference to ", text)
quotes = "`'\"\u2018\u2019\u201c\u201d"
syms = []
for line in text2.splitlines():
    if "undefined reference to" not in line:
        continue
    part = line.split("undefined reference to", 1)[1].strip()
    while part and part[0] in quotes:
        part = part[1:]
    while part and part[-1] in quotes:
        part = part[:-1]
    if part.strip():
        syms.append(part.strip())
if syms:
    print("undef", Counter(syms))

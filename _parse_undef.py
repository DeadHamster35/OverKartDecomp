from pathlib import Path
import re
from collections import Counter, defaultdict

root = Path(r"C:\Users\AuditorFOB\Desktop\Code\LibraryDecomp")
raw = (root / "make_lib_log.txt").read_bytes()
if raw.startswith(b"\xef\xbb\xbf"):
    text = raw.decode("utf-8-sig")
elif raw.startswith(b"\xff\xfe"):
    text = raw.decode("utf-16")
else:
    text = raw.decode("utf-8", "replace")

idx = text.find("undefined reference")
print("first idx", idx)
print(repr(text[idx:idx + 90]))

text2 = text.replace("\r\n", "\n").replace("\r", "\n")
text2 = re.sub(r"\x1b\[[0-9;]*m", "", text2)
text2 = re.sub(r"undefined reference to\s*\n\s*", "undefined reference to ", text2)

syms = []
for line in text2.splitlines():
    if "undefined reference to" not in line:
        continue
    part = line.split("undefined reference to", 1)[1].strip()
    # strip wrapping quotes
    while part and part[0] in "`'\"\u2018\u2019\u201c\u201d":
        part = part[1:]
    while part and part[-1] in "`'\"\u2018\u2019\u201c\u201d":
        part = part[:-1]
    part = part.strip()
    if part:
        syms.append(part)

uniq = sorted(set(syms))
print("hits", len(syms), "unique", len(uniq))
c = Counter(syms)
print("top:")
for s, n in c.most_common(40):
    print(f"  {n:4d}  {s}")

file_syms = defaultdict(set)
cur_func_file = None
for line in text2.splitlines():
    m = re.search(r"(src/[^:\s]+\.c):(\d+):", line)
    if m and "undefined reference" in line:
        part = line.split("undefined reference to", 1)[1].strip()
        while part and part[0] in "`'\"\u2018\u2019\u201c\u201d":
            part = part[1:]
        while part and part[-1] in "`'\"\u2018\u2019\u201c\u201d":
            part = part[:-1]
        file_syms[m.group(1)].add(part.strip())
    elif m and "in function" in line:
        cur_func_file = m.group(1)

print("\nby source file:")
for f in sorted(file_syms, key=lambda x: (-len(file_syms[x]), x)):
    ss = sorted(file_syms[f])
    print(f"\n{f} ({len(ss)})")
    for s in ss:
        print(f"  {s}")

# LibraryVariables .data refs may not have .c:line on same line
libvars = [s for s in uniq if True]
# also catch from LibraryVariables.o lines
lv = set()
for line in text2.splitlines():
    if "LibraryVariables.o" in line and "undefined reference to" in line:
        part = line.split("undefined reference to", 1)[1].strip()
        while part and part[0] in "`'\"\u2018\u2019\u201c\u201d":
            part = part[1:]
        while part and part[-1] in "`'\"\u2018\u2019\u201c\u201d":
            part = part[:-1]
        if part.strip():
            lv.add(part.strip())
print("\nLibraryVariables.o unique", len(lv))
for s in sorted(lv):
    print(f"  {s}")

(root / "cursor_undef_syms.txt").write_text("\n".join(uniq) + "\n", encoding="utf-8")
print("\nwrote", len(uniq), "to cursor_undef_syms.txt")

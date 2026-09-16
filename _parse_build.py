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
text = re.sub(r"\x1b\[[0-9;]*m", "", text).replace("\r\n", "\n")
text2 = re.sub(r"undefined reference to\s*\n\s*", "undefined reference to ", text)

print("bytes", len(raw))
print("markers", [l for l in text.splitlines() if l.startswith(("START:", "DONE:", "PIPE:", "MAKE_EXIT")) or "MAKE_EXIT" in l])
print("cfe Error", text.count("cfe: Error"), "Fatal", text.count("cfe: Fatal"))
failed = re.findall(r"make: \*\*\* \[Makefile:\d+: ([^\]]+)\] Error", text)
print("failed", failed)

# compile errors
blocks = []
lines = text.splitlines()
for i, line in enumerate(lines):
    m = re.match(r"cfe: Error: ([^,]+), line (\d+): (.+)", line.strip())
    if m:
        src = lines[i+1].strip() if i+1 < len(lines) else ""
        blocks.append((m.group(1), int(m.group(2)), m.group(3).strip(), src))
if blocks:
    print("\nCOMPILE ERRORS", len(blocks))
    by = defaultdict(list)
    for f, ln, msg, src in blocks:
        by[f].append((ln, msg, src))
    for f in sorted(by):
        print(f"## {f} ({len(by[f])})")
        seen = set(); n = 0
        for ln, msg, src in by[f]:
            k = (ln, msg, src[:100])
            if k in seen: continue
            seen.add(k); n += 1
            print(f"  L{ln}: {msg}")
            if src: print(f"       {src[:180]}")
            if n >= 10: break

# link undefs
syms = []
for line in text2.splitlines():
    if "undefined reference to" not in line:
        continue
    part = line.split("undefined reference to", 1)[1].strip()
    while part and part[0] in "`'\"\u2018\u2019\u201c\u201d":
        part = part[1:]
    while part and part[-1] in "`'\"\u2018\u2019\u201c\u201d":
        part = part[:-1]
    part = part.strip()
    if part:
        syms.append(part)
uniq = sorted(set(syms))
print("\nLINK undef hits", len(syms), "unique", len(uniq))
if uniq:
    c = Counter(syms)
    print("top:")
    for s, n in c.most_common(25):
        print(f"  {n:4d}  {s}")
    file_syms = defaultdict(set)
    for line in text2.splitlines():
        if "undefined reference" not in line:
            continue
        m = re.search(r"(src/[^:\s]+\.c):(\d+):", line)
        if not m:
            continue
        part = line.split("undefined reference to", 1)[1].strip()
        while part and part[0] in "`'\"\u2018\u2019\u201c\u201d":
            part = part[1:]
        while part and part[-1] in "`'\"\u2018\u2019\u201c\u201d":
            part = part[:-1]
        file_syms[m.group(1)].add(part.strip())
    print("\nby file:")
    for f in sorted(file_syms, key=lambda x: (-len(file_syms[x]), x)):
        ss = sorted(file_syms[f])
        print(f"  {f}: {len(ss)}")
        for s in ss[:12]:
            print(f"    {s}")
        if len(ss) > 12:
            print(f"    ... +{len(ss)-12}")
    (root / "cursor_undef_syms.txt").write_text("\n".join(uniq) + "\n", encoding="utf-8")
    print("wrote cursor_undef_syms.txt", len(uniq))

rom = root / "build/us/mk64.us.z64"
elf = root / "build/us/mk64.us.elf"
print("elf", elf.exists(), "rom", rom.exists(), rom.stat().st_size if rom.exists() else 0)

# compare to previous unique count if we can - just report MAKE success lines
for l in lines[-15:]:
    if any(x in l for x in ["MAKE_EXIT", "PIPE:", "mk64", "OK", "Error", "DONE"]):
        print("tail:", l[:200])

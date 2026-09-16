from pathlib import Path
import re
from collections import defaultdict, Counter

root = Path(r"C:\Users\AuditorFOB\Desktop\Code\LibraryDecomp")
raw = (root / "make_lib_log.txt").read_bytes()
# decode, strip ANSI, normalize newlines
text = raw.decode("utf-8", "replace")
text = re.sub(r"\x1b\[[0-9;]*m", "", text)
text = text.replace("\r\n", "\n").replace("\r", "\n")
(root / "_parse_debug.txt").write_text(text[:5000], encoding="utf-8")

# find sample error line
for line in text.splitlines():
    if "cfe: Error" in line:
        print("SAMPLE:", repr(line[:180]))
        break

blocks = []
lines = text.splitlines()
for i, line in enumerate(lines):
    m = re.match(r"cfe: Error: ([^,]+), line (\d+): (.+)", line)
    if m:
        f, ln, msg = m.group(1), int(m.group(2)), m.group(3).strip()
        src = lines[i+1].strip() if i+1 < len(lines) else ""
        caret = lines[i+2].strip() if i+2 < len(lines) else ""
        blocks.append((f.replace("\\", "/"), ln, msg, src, caret))

print("parsed", len(blocks))
failed = re.findall(r"make: \*\*\* \[Makefile:\d+: ([^\]]+)\] Error", text)
print("failed", failed)

by = defaultdict(list)
for f, ln, msg, src, caret in blocks:
    by[f].append((ln, msg, src))

out = []
out.append("LibraryDecomp make -j FAILED (no ROM). Match not required.")
out.append(f"Total cfe Error lines: {len(blocks)}; failed .o targets: {len(failed)}")
out.append("")
out.append("Failed objects:")
for t in failed:
    out.append(f"  - {t}")
out.append("")
out.append("Message histogram:")
for msg, c in Counter(m for _,_,m,_,_ in blocks).most_common(20):
    out.append(f"  {c:4d}  {msg}")

# Check Object
actor = (root / "include/actor_types.h").read_text(encoding="utf-8", errors="replace")
# find Object struct / typedef near Actor
for j,l in enumerate(actor.splitlines(),1):
    if "Object" in l and ("typedef" in l or "struct" in l or "#define" in l):
        if 140 <= j <= 160 or "typedef" in l:
            out.append(f"actor_types.h:{j}: {l.rstrip()}")

out.append("")
out.append("OKStruct.h still has `Object ObjectData` (L211). Status[4];; appears FIXED (single ; now).")
out.append("")
out.append("=== Per-file first unique errors (source snippet) ===")
for f in sorted(by.keys()):
    items = by[f]
    out.append(f"\n## {f} ({len(items)} errors)")
    seen = set()
    for ln, msg, src in items:
        key = (msg, src[:100])
        if key in seen:
            continue
        seen.add(key)
        out.append(f"  L{ln}: {msg}")
        if src:
            out.append(f"       {src[:200]}")
        if len(seen) >= 8:
            break

# Classify likely root causes
out.append("\n=== Likely root themes for Cursor ===")
# mid-block decl: float/int/uchar after statements
c89 = [(f,ln,src) for f,ln,msg,src,_ in blocks if msg == "Syntax Error" and re.match(r"(float|int|uint|uchar|ushort|short|long|double|char|Vtx|CustomParticle|Object|OKObject|Player|s16|s32|u16|u32|u8|s8|f32)\b", src.strip())]
out.append(f"Possible C89 mid-function decls (Syntax Error at typed decl): {len(c89)}")
for f,ln,src in c89[:25]:
    out.append(f"  {f}:{ln}: {src[:140]}")

# Object unknown?
unk = [(f,ln,msg,src) for f,ln,msg,src,_ in blocks if "Object" in src or "Object" in msg]
out.append(f"\nErrors mentioning Object in source line: {len(unk)}")

# undeclared identifiers in warnings?
warns = re.findall(r"cfe: Warning[^:]*: ([^,]+), line (\d+): (.+)", text)
out.append(f"\nWarnings: {len(warns)}")
wc = Counter(m.strip() for _,_,m in warns)
for m,c in wc.most_common(15):
    out.append(f"  {c:4d}  {m}")

# MarioKartMenu first error context from source
menu = (root/"src/OverKartLibrary/MarioKartMenu.c").read_text(encoding="utf-8",errors="replace").splitlines()
out.append("\nMarioKartMenu.c around first error L190:")
for n in range(180, 220):
    if 0 <= n-1 < len(menu):
        out.append(f"{n}: {menu[n-1]}")

mk3d = (root/"src/OverKartLibrary/MarioKart3D.c").read_text(encoding="utf-8",errors="replace").splitlines()
out.append("\nMarioKart3D.c around L2616:")
for n in range(2605, 2630):
    if 0 <= n-1 < len(mk3d):
        out.append(f"{n}: {mk3d[n-1]}")

# OKCustomObjects
okc = (root/"src/OverKartLibrary/CustomObjects/OKCustomObjects.c").read_text(encoding="utf-8",errors="replace").splitlines()
out.append("\nOKCustomObjects.c around L537:")
for n in range(520, 550):
    if 0 <= n-1 < len(okc):
        out.append(f"{n}: {okc[n-1]}")

report = "\n".join(out)
(root / "cursor_errors_report.txt").write_text(report, encoding="utf-8")
print(report[:12000])
print("\n... wrote cursor_errors_report.txt len", len(report))

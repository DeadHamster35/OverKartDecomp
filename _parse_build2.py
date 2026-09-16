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
print("markers", [l for l in text.splitlines() if l.startswith(("START:", "DONE:", "PIPE:")) or "MAKE_EXIT" in l])
failed = re.findall(r"make: \*\*\* \[Makefile:\d+: ([^\]]+)\] Error", text)
print("failed", len(failed), failed[:30])

# cfe
cfe = []
lines = text.splitlines()
for i, line in enumerate(lines):
    m = re.match(r"cfe: Error: ([^,]+), line (\d+): (.+)", line.strip())
    if m:
        src = lines[i+1].strip() if i+1 < len(lines) else ""
        cfe.append((m.group(1), int(m.group(2)), m.group(3).strip(), src))
print("cfe Error", len(cfe))

# gcc-style errors
gcc_errs = []
for i, line in enumerate(lines):
    if "CMake" in line or "CategoryInfo" in line or "FullyQualified" in line:
        continue
    m = re.search(r"([^:\n]+\.(?:c|h|s)):(\d+)(?::\d+)?: error: (.+)", line)
    if m:
        gcc_errs.append((m.group(1).replace("\\", "/"), int(m.group(2)), m.group(3).strip(), line.strip()[:220]))

print("gcc-style error lines", len(gcc_errs))
# group by message kind
kinds = Counter()
for path, ln, msg, full in gcc_errs:
    if msg.startswith("conflicting types for"):
        kinds["conflicting types"] += 1
    elif "too many arguments" in msg:
        kinds["too many arguments"] += 1
    elif "undeclared" in msg or "implicit declaration" in msg:
        kinds["undeclared/implicit"] += 1
    else:
        kinds[msg[:60]] += 1
print("kinds:", dict(kinds))

# unique conflicting
conf = []
for path, ln, msg, full in gcc_errs:
    m = re.search(r"conflicting types for '([^']+)'", msg)
    if m:
        conf.append((path, ln, m.group(1)))
print("unique conflicting symbols", len(set(c[2] for c in conf)))
if conf:
    print("sample conflicting:")
    seen=set()
    for p,ln,s in conf:
        if s in seen: continue
        seen.add(s)
        print(f"  {Path(p).name}:{ln} {s}")
        if len(seen)>=20: break

# other unique errors (non-conflict)
print("\nother unique errors:")
seen=set(); n=0
for path, ln, msg, full in gcc_errs:
    if "conflicting types" in msg: continue
    key=(Path(path).name, ln, msg[:80])
    if key in seen: continue
    seen.add(key); n+=1
    print(f"  {Path(path).name}:{ln}: {msg[:160]}")
    if n>=30: break

# link
syms=[]
for line in text2.splitlines():
    if "undefined reference to" not in line: continue
    part=line.split("undefined reference to",1)[1].strip()
    while part and part[0] in "`'\"\u2018\u2019\u201c\u201d": part=part[1:]
    while part and part[-1] in "`'\"\u2018\u2019\u201c\u201d": part=part[:-1]
    if part.strip(): syms.append(part.strip())
print("\nlink undef unique", len(set(syms)))

rom = root/"build/us/mk64.us.z64"
elf = root/"build/us/mk64.us.elf"
print("elf", elf.exists(), "rom", rom.exists())

# write short report
by_file=defaultdict(list)
for path, ln, msg, full in gcc_errs:
    by_file[path].append((ln, msg))
out=["LibraryDecomp make -j result","failed targets: "+", ".join(failed[:40]), f"cfe={len(cfe)} gcc_errors={len(gcc_errs)} link_undef={len(set(syms))}","", "Unique non-conflict errors:"]
seen=set()
for path, ln, msg, full in gcc_errs:
    if "conflicting types" in msg: continue
    key=(path,ln,msg)
    if key in seen: continue
    seen.add(key)
    out.append(f"{path}:{ln}: {msg}")
out.append("")
out.append(f"Conflicting-type unique symbols ({len(set(c[2] for c in conf))}):")
for s in sorted(set(c[2] for c in conf)):
    out.append(f"  {s}")
(root/"cursor_errors_report.txt").write_text("\n".join(out), encoding="utf-8")
print("wrote report")

import re, os, pathlib
root = pathlib.Path(r"C:\Users\AuditorFOB\Desktop\Code\OverKartDecomp")
lib = pathlib.Path(r"C:\Users\AuditorFOB\Desktop\Code\LibraryDecomp")
log = root / "make_lib_log.txt"
text = log.read_text(encoding="utf-8", errors="replace")
print("==== markers ====")
for m in re.findall(r"(START:.*|MAKE_EXIT:.*|PIPE:.*|DONE:.*)", text):
    print(m)
print("==== unique error-ish lines ====")
pat = re.compile(r"fatal error:|cfe: Error|undefined reference|multiple definition|error: |make: \*\*\*")
seen=set()
for line in text.splitlines():
    if pat.search(line):
        s=line.strip()
        if s not in seen:
            seen.add(s)
            print(s)
print("==== undef tally ====")
undefs=re.findall(r"undefined reference to [`']([^`']+)[`']", text)
from collections import Counter
for name,c in Counter(undefs).most_common():
    print(f"{c:4d}  {name}")
print("==== compile Error targets ====")
for line in text.splitlines():
    if "make: *** [" in line and "Error" in line:
        print(line.strip())
print("==== OverKart5 tree ====")
ok5 = root/"src"/"OverKart5"
if ok5.exists():
    for p in sorted(ok5.rglob("*")):
        if p.is_file():
            print(p.relative_to(root))
else:
    print("no src/OverKart5")
# also OverKartLibrary
for name in ["OverKart5","OverKartLibrary"]:
    p=root/"src"/name
    print(f"exists {name}: {p.exists()}")

syms=["boost2","boost3","boost4","cpu2Speed","cpu3Speed","cpu4Speed","player2OK","player3OK","player4OK"]
exts={".c",".h",".s",".inc",".asm",".txt",".ld"}
def search(base, label):
    hits=[]
    if not base.exists():
        return hits
    for dirpath, dirnames, filenames in os.walk(base):
        # prune heavy dirs
        dirnames[:] = [d for d in dirnames if d not in ("build","tools","assets",".git")]
        for fn in filenames:
            if pathlib.Path(fn).suffix.lower() not in exts and fn not in ("undefined_syms.txt","Makefile"):
                continue
            fp=pathlib.Path(dirpath)/fn
            try:
                t=fp.read_text(encoding="utf-8", errors="ignore")
            except Exception:
                continue
            for i,line in enumerate(t.splitlines(),1):
                for s in syms:
                    if re.search(rf"\b{re.escape(s)}\b", line):
                        hits.append((s, str(fp), i, line.strip()[:160]))
    return hits

print("==== symbol hits OverKartDecomp ====")
for s,fp,i,line in search(root, "OK"):
    print(f"{s}: {fp}:{i}: {line}")
print("==== symbol hits LibraryDecomp ====")
for s,fp,i,line in search(lib, "LIB"):
    print(f"{s}: {fp}:{i}: {line}")

# MarioKartAI context
ai = root/"src"/"OverKart5"/"MarioKartAI.c"
if ai.exists():
    print("==== MarioKartAI.c excerpt ====")
    lines=ai.read_text(encoding="utf-8", errors="replace").splitlines()
    for n in list(range(1,30))+list(range(40,110))+list(range(180,220)):
        if 1<=n<=len(lines):
            print(f"{n:4d}| {lines[n-1]}")

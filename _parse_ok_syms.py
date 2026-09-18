import re, pathlib
root = pathlib.Path(r"C:\Users\AuditorFOB\Desktop\Code\OverKartDecomp")
lib = pathlib.Path(r"C:\Users\AuditorFOB\Desktop\Code\LibraryDecomp")
syms = ["boost2","boost3","boost4","cpu2Speed","cpu3Speed","cpu4Speed","player2OK","player3OK","player4OK"]
exts = {".c",".h",".s",".inc",".asm",".ld",".txt",".mdc"}
skip_dirs = {".git","build","tools","assets","courses","textures","models","music","bin"}

def search(base):
    out=[]
    for p in base.rglob("*"):
        if not p.is_file():
            continue
        if any(part in skip_dirs for part in p.parts):
            continue
        if p.name.startswith("make_lib") or p.name.startswith("_parse") or p.suffix.lower()==".txt" and "log" in p.name.lower():
            continue
        if p.suffix.lower() not in exts and p.name not in ("undefined_syms.txt","mk64.ld"):
            continue
        try:
            text = p.read_text(encoding="utf-8", errors="ignore")
        except Exception:
            continue
        for i,line in enumerate(text.splitlines(),1):
            for s in syms:
                if re.search(rf"\b{re.escape(s)}\b", line):
                    out.append((s, str(p.relative_to(base)), i, line.strip()[:180]))
    return out

print("=== OverKartDecomp sources ===")
for row in search(root):
    print("%s | %s:%d | %s" % row)
print("=== LibraryDecomp sources ===")
for row in search(lib):
    print("%s | %s:%d | %s" % row)

ai = root/"src"/"OverKart5"/"MarioKartAI.c"
ah = root/"src"/"OverKart5"/"MarioKartAI.h"
bh = root/"src"/"OverKart5"/"Build.h"
oi = root/"src"/"OverKart5"/"OKInclude.h"
for f in (ai, ah, bh, oi):
    print("====", f.name, "exists", f.exists(), "====")
    if f.exists():
        lines=f.read_text(encoding="utf-8", errors="replace").splitlines()
        for i,l in enumerate(lines,1):
            if any(s in l for s in syms) or i<=40 or (40<=i<=110) or (180<=i<=220) or "extern" in l or "include" in l.lower():
                if i<=40 or (40<=i<=110) or (180<=i<=220) or any(s in l for s in syms) or "extern" in l:
                    print(f"{i:4d}| {l}")

# undefined_syms / GameOffsets / OverKartDefaults
for rel in [
    "undefined_syms.txt",
    "src/OverKartLibrary/OverKartDefaults.c",
    "src/OverKartLibrary/GameOffsets.h",
    "src/OverKartLibrary/GlobalStat.h",
]:
    p=root/rel
    print("====", rel, "exists", p.exists(), "====")
    if p.exists():
        t=p.read_text(encoding="utf-8", errors="ignore")
        for s in syms:
            if s in t:
                for i,l in enumerate(t.splitlines(),1):
                    if s in l:
                        print(f"{s}: {i}: {l.strip()[:160]}")

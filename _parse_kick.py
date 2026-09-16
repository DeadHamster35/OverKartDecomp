from pathlib import Path
import re, hashlib
root = Path(r"C:\Users\AuditorFOB\Desktop\Code\LibraryDecomp")
raw = (root / "make_lib_log.txt").read_bytes()
if raw.startswith(b"\xef\xbb\xbf"):
    text = raw.decode("utf-8-sig")
elif raw.startswith(b"\xff\xfe"):
    text = raw.decode("utf-16")
else:
    text = raw.decode("utf-8", "replace")
text = re.sub(r"\x1b\[[0-9;]*m", "", text)
print("bytes", len(raw), "lines", text.count("\n") + 1)
print("markers", [l for l in text.splitlines() if l.startswith(("START:", "DONE:", "PIPE:")) or "MAKE_EXIT" in l])
print("cfe Error", len(re.findall(r"cfe: Error", text)))
print("cfe Warning", len(re.findall(r"cfe: Warning", text)))
print("gcc error", len(re.findall(r": error:", text)))
print("redefined", len(re.findall(r"redefined", text)))
failed = re.findall(r"make: \*\*\* \[Makefile:\d+: ([^\]]+)\] Error", text)
print("failed", failed)
text2 = re.sub(r"undefined reference to\s*\n\s*", "undefined reference to ", text)
syms = []
quotes = "`'\"\u2018\u2019\u201c\u201d"
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
print("undef unique", sorted(set(syms)))
print("Linking", "Linking ELF" in text, "Building ROM", "Building ROM" in text)
rom = root / "build/us/mk64.us.z64"
elf = root / "build/us/mk64.us.elf"
print("elf", elf.exists(), elf.stat().st_size if elf.exists() else None)
print("rom", rom.exists(), rom.stat().st_size if rom.exists() else None)
if rom.exists():
    print("sha1", hashlib.sha1(rom.read_bytes()).hexdigest())

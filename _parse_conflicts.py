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

# gcc-style: path:line:col: error: conflicting types for 'Foo'
pat = re.compile(
    r"([^:\n]+GameOffsets\.h):(\d+):\d+: error: conflicting types for '([^']+)'(.*)"
)
hits = []
lines = text.splitlines()
for i, line in enumerate(lines):
    m = pat.search(line)
    if m:
        note = ""
        # next few lines often have note: previous definition
        for j in range(i + 1, min(i + 4, len(lines))):
            if "note:" in lines[j] or "previous" in lines[j].lower() or "but argument" in lines[j]:
                note += " | " + lines[j].strip()[:160]
        hits.append((m.group(1), int(m.group(2)), m.group(3), m.group(4).strip()[:120], note))

print("conflicting_types hits", len(hits))
syms = Counter(h[2] for h in hits)
print("unique symbols", len(syms))
print("\nunique conflicting symbols (alphabetical):")
for s in sorted(syms):
    print(f"  {s}  (x{syms[s]})")

# sample first occurrence of each with line
print("\nfirst GameOffsets line per symbol:")
seen = set()
for path, ln, sym, rest, note in hits:
    if sym in seen:
        continue
    seen.add(sym)
    print(f"  GameOffsets.h:{ln}: {sym}  {rest[:80]}")

# other error types
other = []
for line in lines:
    if ": error:" in line and "conflicting types" not in line and "CMake" not in line and "CategoryInfo" not in line:
        other.append(line.strip()[:220])
print("\nother error: lines", len(other))
for o in other[:40]:
    print(" ", o)

failed = re.findall(r"make: \*\*\* \[Makefile:\d+: ([^\]]+)\] Error", text)
print("\nfailed targets", len(failed))

# write cursor report
out = []
out.append("LibraryDecomp make -j FAILED (no ROM). Compile regression.")
out.append("Root: GameOffsets.h conflicting types vs decomp prototypes (many OverKart .o).")
out.append(f"conflicting-types occurrences: {len(hits)}; unique symbols: {len(syms)}")
out.append("")
out.append("Unique symbols:")
for s in sorted(syms):
    out.append(f"  {s}")
out.append("")
out.append("First GameOffsets.h line per symbol:")
seen = set()
for path, ln, sym, rest, note in hits:
    if sym in seen:
        continue
    seen.add(sym)
    out.append(f"  L{ln}: {sym} {rest}")
(root / "cursor_errors_report.txt").write_text("\n".join(out), encoding="utf-8")
print("\nwrote cursor_errors_report.txt")

# peek GameOffsets decls at a few lines
go = (root / "src/OverKartLibrary/GameVariables/NTSC/GameOffsets.h").read_text(encoding="utf-8", errors="replace").splitlines()
print("\nGameOffsets sample L23-35:")
for n in range(22, 36):
    if 0 <= n < len(go):
        print(f"{n+1}: {go[n]}")
print("\nL187-206:")
for n in range(186, 207):
    if 0 <= n < len(go):
        print(f"{n+1}: {go[n]}")

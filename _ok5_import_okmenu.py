# Convert OverKart5 OKMenu.c for IDO host import.
from pathlib import Path
import re
import shutil

src = Path(r"C:\Users\AuditorFOB\Desktop\Code\OverKart5\OKMenu.c")
dst = Path(r"C:\Users\AuditorFOB\Desktop\Code\OverKartDecomp\src\OverKart5\OKMenu.c")
text = src.read_bytes().replace(b"\r\n", b"\n").replace(b"\r", b"\n").decode("latin-1")

text = text.replace('#include "../Library/MainInclude.h"', '#include "MainInclude.h"')

# Player loc/speed: stock RAM -> Player members
repls = [
    ("g_player1LocationX", "GlobalPlayer[0].position[0]"),
    ("g_player1LocationY", "GlobalPlayer[0].position[1]"),
    ("g_player1LocationZ", "GlobalPlayer[0].position[2]"),
    ("g_player1LocationA", "GlobalPlayer[0].direction[0]"),
    ("g_player1SpeedX", "GlobalPlayer[0].velocity[0]"),
    ("g_player1SpeedY", "GlobalPlayer[0].velocity[1]"),
    ("g_player1SpeedZ", "GlobalPlayer[0].velocity[2]"),
    ("g_player1SpeedA", "GlobalPlayer[0].speed"),
    ("g_lakituStatus", "GlobalPlayer[0].jugemu_flag"),
    ("g_progressValue", "gNearestPathPointByPlayerId[0]"),
]
for a, b in repls:
    text = text.replace(a, b)

# MiniMapDraw stock RAM shorts
text = text.replace("*(short*)(0x8018D2F0)", "rzerox")
text = text.replace("*(short*)(0x8018D2F8)", "rzeroy")
text = text.replace("*(short*)(0x8018D2B0)", "g_mapWidth")
text = text.replace("*(short*)(0x8018D2B8)", "g_mapHeight")
text = text.replace("*(short*)(0x8018D2E0)", "g_startX")
text = text.replace("*(short*)(0x8018D2E8)", "g_startY")

# g_mapX / g_mapY are arrays here
# Original treated them as scalars. Use [0].
text = text.replace("g_mapX + rzerox", "g_mapX[0] + rzerox")
text = text.replace("g_mapY + rzeroy", "g_mapY[0] + rzeroy")

# Stub SMC / jump-table writes (batch 6). Keep the line as a comment.
def stub_assign(m):
    return "/* batch6 SMC: " + m.group(0).strip() + " */"

text = re.sub(r"^[ \t]*asm_DispOBSubPSelCursor1\[[^\]]+\]\s*=\s*[^;]+;", stub_assign, text, flags=re.M)
text = re.sub(r"^[ \t]*asm_BlinkCheck\s*=\s*[^;]+;", stub_assign, text, flags=re.M)
text = re.sub(r"^[ \t]*jtbl_DispObjPsel\[[^\]]+\]\s*=\s*[^;]+;", stub_assign, text, flags=re.M)

# SoundOffset declaration-after-statement
text = text.replace(
    "PlayerOK[PlayerIndex] = 1;\n                         int SoundOffset = (0x10 * CharacterConvert[(int)PlayerCharacterSelect[PlayerIndex] + 1]);",
    "int SoundOffset;\n                         PlayerOK[PlayerIndex] = 1;\n                         SoundOffset = (0x10 * CharacterConvert[(int)PlayerCharacterSelect[PlayerIndex] + 1]);",
)
text = text.replace(
    "PlayerOK[PlayerIndex] = 1;\n                              int SoundOffset = (0x10 * CharacterConvert[(int)PlayerCharacterSelect[PlayerIndex] + 1]);",
    "int SoundOffset;\n                              PlayerOK[PlayerIndex] = 1;\n                              SoundOffset = (0x10 * CharacterConvert[(int)PlayerCharacterSelect[PlayerIndex] + 1]);",
)

# Wrap for (int NAME ... ) { } in an extra block with the decl.
def wrap_for_int(s):
    out = []
    i = 0
    pat = re.compile(r"for\s*\(\s*int\s+([A-Za-z_][A-Za-z0-9_]*)")
    while True:
        m = pat.search(s, i)
        if not m:
            out.append(s[i:])
            break
        out.append(s[i:m.start()])
        name = m.group(1)
        # find the '{' of the for-body
        j = m.end()
        # skip to closing paren of for (
        depth = 1
        k = s.find("(", m.start())
        k += 1
        depth = 1
        while k < len(s) and depth:
            if s[k] == "(":
                depth += 1
            elif s[k] == ")":
                depth -= 1
            k += 1
        # k is after ')'
        while k < len(s) and s[k] in " \t\n":
            k += 1
        if k >= len(s) or s[k] != "{":
            # no brace body; leave as-is after converting for (int
            out.append("{ int %s; for (%s" % (name, name) + s[m.end():k])
            i = k
            continue
        # find matching brace
        start_brace = k
        depth = 0
        k = start_brace
        while k < len(s):
            if s[k] == "{":
                depth += 1
            elif s[k] == "}":
                depth -= 1
                if depth == 0:
                    k += 1
                    break
            k += 1
        body = s[start_brace:k]
        header = s[m.start():start_brace]
        header = re.sub(r"for\s*\(\s*int\s+" + name, "for (" + name, header, count=1)
        out.append("{ int %s; %s%s }" % (name, header, body))
        i = k
    return "".join(out)

text = wrap_for_int(text)

# MiniMapDraw: CoinCount decl after if — wrap_for_int already wraps the for.
# Ensure CoinCount is declared at start of the if block.
text = text.replace(
    "if (SaveGame.GameSettings.GameMode == 1)\n     {\n          int CoinCount = 0;",
    "if (SaveGame.GameSettings.GameMode == 1)\n     {\n          int CoinCount = 0;",
)

dst.write_bytes(text.encode("latin-1"))
print("wrote", dst, "bytes", dst.stat().st_size)

# header
hs = Path(r"C:\Users\AuditorFOB\Desktop\Code\OverKart5\OKMenu.h")
hd = Path(r"C:\Users\AuditorFOB\Desktop\Code\OverKartDecomp\src\OverKart5\OKMenu.h")
h = hs.read_bytes().replace(b"\r\n", b"\n").replace(b"\r", b"\n").decode("latin-1")
h = h.replace('#include "..\\Library\\MainInclude.h"', '#include "MainInclude.h"')
h = h.replace('#include "../Library/MainInclude.h"', '#include "MainInclude.h"')
hd.write_bytes(h.encode("latin-1"))
print("wrote", hd)

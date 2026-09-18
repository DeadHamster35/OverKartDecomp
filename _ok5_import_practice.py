# Convert OverKart5 MarioKartPractice.c for IDO host import (batch 3c).
from pathlib import Path
import re

src = Path(r"C:\Users\AuditorFOB\Desktop\Code\OverKart5\MarioKartPractice.c")
dst = Path(r"C:\Users\AuditorFOB\Desktop\Code\OverKartDecomp\src\OverKart5\MarioKartPractice.c")
text = src.read_bytes().replace(b"\r\n", b"\n").replace(b"\r", b"\n").decode("latin-1")

text = text.replace('#include "../Library/MainInclude.h"', '#include "MainInclude.h"')

# Player loc/speed
repls = [
    ("g_player1LocationY", "GlobalPlayer[0].position[1]"),
    ("g_player1SpeedY", "GlobalPlayer[0].velocity[1]"),
    ("g_progressValue", "gNearestPathPointByPlayerId[0]"),
    ("player1inputX", "gControllers[0].AnalogX"),
    ("player1inputY", "gControllers[0].AnalogY"),
    ("p1Button", "gControllers[0].ButtonHeld"),
    ("g_player1Section", "GlobalScreen[0]->camera_point"),
    ("g_player1View", "GlobalScreen[0]->flag"),
]
for a, b in repls:
    text = text.replace(a, b)

# d_Input / p_Input overlay gControllers[0].ButtonHeld
text = text.replace(
    "\tGlobalCharA = (d_Input & 0x0F);\t //Dpad\n"
    "\tGlobalCharB = (p_Input & 0xF0) >> 4;  //LR Trigger\n"
    "\tGlobalCharC = (d_Input & 0xF0) >> 4;  //AB Button\n"
    "\tGlobalCharD = (p_Input & 0x0F);  //Cpad",
    "\tGlobalCharA = (char)((gControllers[0].ButtonHeld >> 8) & 0x0F);  // D-pad\n"
    "\tGlobalCharB = (char)((gControllers[0].ButtonHeld & 0xF0) >> 4);  // L/R\n"
    "\tGlobalCharC = (char)((gControllers[0].ButtonHeld >> 12) & 0x0F);  // A/B\n"
    "\tGlobalCharD = (char)(gControllers[0].ButtonHeld & 0x0F);  // C-pad",
)

# Mini-map scalars vs arrays
text = text.replace("g_mapX++", "g_mapX[0]++")
text = text.replace("g_mapX--", "g_mapX[0]--")
text = text.replace("g_mapY++", "g_mapY[0]++")
text = text.replace("g_mapY--", "g_mapY[0]--")

# JP audio blobs wait for batch 5
text = text.replace(
    "\t\t\tg_MUSRawAudioTable.pointer[0].address = (long)&JP_Audio;\n"
    "\t\t\tg_MUSRawAudioTable.pointer[0].length = 0x24C580;\n\n"
    "\t\t\tg_MUSInstrumentTable.pointer[0].address = (long)&JP_Bank;\n"
    "\t\t\tg_MUSInstrumentTable.pointer[0].length = 0x4540;",
    "\t\t\t/* batch5: JP_Audio / JP_Bank. US tables until those blobs land. */\n"
    "\t\t\tg_MUSRawAudioTable.pointer[0].address = *(long*)&ok_USAudio;\n"
    "\t\t\tg_MUSRawAudioTable.pointer[0].length = 0x24C3F0;\n\n"
    "\t\t\tg_MUSInstrumentTable.pointer[0].address = *(long*)(&ok_USAudio + 1);\n"
    "\t\t\tg_MUSInstrumentTable.pointer[0].length = 0x44D0;",
)

# antialias: sViContexts[].features
text = text.replace("antialiasToggle = 0x00013016;\n\t\tantialiasToggleB = 0x00013016;",
                    "__osViCurr->features = 0x00013016;\n\t\t__osViNext->features = 0x00013016;")
text = text.replace("antialiasToggle = 0x00003216;\n\t\tantialiasToggleB = 0x00003216;",
                    "__osViCurr->features = 0x00003216;\n\t\t__osViNext->features = 0x00003216;")

# batch 6d: RouletteStart branch poke
text = text.replace(
    "\t\t\t\t*(int*)(0x8007AC54) = 0;\n",
    "\t\t\t\t/* batch6d SMC: *(int*)(0x8007AC54) = 0; */\n",
)
text = text.replace(
    "\t\t\t\t*(int*)(0x8007AC54) = 0x10400009;\n",
    "\t\t\t\t/* batch6d SMC: *(int*)(0x8007AC54) = 0x10400009; */\n",
)

# batch 6c: flycam instruction pokes
def stub_poke(m):
    return "/* batch6c SMC: " + m.group(0).strip() + " */"

text = re.sub(r"^[ \t]*\*\(uint\*\)\(0x[0-9A-Fa-f]+\)\s*=\s*[^;]+;", stub_poke, text, flags=re.M)

# batch 6c: playerHUD[0]+0x40 overlay at 0x8018CAB0
text = text.replace("*sourceAddress = (int)0x8018CAB0;",
                    "/* batch6c HUD: *sourceAddress = playerHUD[0] + 0x40; */")
text = text.replace("*targetAddress = (int)0x8018CAB0;",
                    "/* batch6c HUD: *targetAddress = playerHUD[0] + 0x40; */")

# FlycamInit -> FlyCamInit (header / OverKart.c name)
text = text.replace("void FlycamInit()", "void FlyCamInit()")

# C89: hoist PositionArray
text = text.replace(
    "\t\t\t\tshort* PositionArray = (short*)GetRealAddress(PathTable[g_courseID][0]);\n"
    "\t\t\t\tGlobalPlayer[0].position[0] = (float)PositionArray[0];",
    "\t\t\t\t{\n"
    "\t\t\t\tshort* PositionArray;\n"
    "\t\t\t\tPositionArray = (short*)GetRealAddress(PathTable[g_courseID][0]);\n"
    "\t\t\t\tGlobalPlayer[0].position[0] = (float)PositionArray[0];",
)
# close the extra block after direction assign
text = text.replace(
    "\t\t\t\tGlobalPlayer[0].direction[1] = 0x8000;\n"
    "\t\t\t\tbreak;",
    "\t\t\t\tGlobalPlayer[0].direction[1] = 0x8000;\n"
    "\t\t\t\t}\n"
    "\t\t\t\tbreak;",
)

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
        k = s.find("(", m.start())
        k += 1
        depth = 1
        while k < len(s) and depth:
            if s[k] == "(":
                depth += 1
            elif s[k] == ")":
                depth -= 1
            k += 1
        while k < len(s) and s[k] in " \t\n":
            k += 1
        if k < len(s) and s[k] == "{":
            body_open = k
            k += 1
            depth = 1
            while k < len(s) and depth:
                if s[k] == "{":
                    depth += 1
                elif s[k] == "}":
                    depth -= 1
                k += 1
            body = s[body_open:k]
            for_head = s[m.start():body_open]
            for_head = re.sub(r"for\s*\(\s*int\s+" + name, "for (" + name, for_head, count=1)
            out.append("{ int " + name + "; " + for_head + body + " }")
            i = k
        else:
            out.append(s[m.start():m.end()])
            i = m.end()
    return "".join(out)

text = wrap_for_int(text)

# VI context externs after includes
needle = '#include "OKInclude.h"\n\n'
insert = (
    '#include "OKInclude.h"\n\n'
    "extern OSViContext *__osViCurr;\n"
    "extern OSViContext *__osViNext;\n\n"
)
if needle not in text:
    raise SystemExit("include block not found")
text = text.replace(needle, insert, 1)

dst.write_bytes(text.encode("latin-1"))
print("wrote", dst, "bytes", dst.stat().st_size)

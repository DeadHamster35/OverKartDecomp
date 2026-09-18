# Convert OverKart5 OverKart.c for IDO host import (batch 3d).
from pathlib import Path
import re

src = Path(r"C:\Users\AuditorFOB\Desktop\Code\OverKart5\OverKart.c")
dst = Path(r"C:\Users\AuditorFOB\Desktop\Code\OverKartDecomp\src\OverKart5\OverKart.c")
text = src.read_bytes().replace(b"\r\n", b"\n").replace(b"\r", b"\n").decode("latin-1")

text = text.replace('#include "../Library/MainInclude.h"\n#include "OKInclude.h"\n#include <stdio.h>\n',
                    '#include "MainInclude.h"\n#include "OKInclude.h"\n')

text = text.replace("short MapModeCheck = 0;\n", "")
text = text.replace("uint ROptionPressed = 0;\n\n", "")

text = text.replace("ok_Storage", "ok5_MenuStorage")

text = text.replace(
    "\tg_NintendoLogoOffset = 0x080052A0;\n"
    "\tg_NintendoLogoBorder = 0x256B9478;\n",
    "\t/* batch6: g_NintendoLogoOffset / g_NintendoLogoBorder are pool overlay + mid-function SMC */\n",
)

text = text.replace(
    "\tasm_SongA = 0x240E0001;\n"
    "\tasm_SongB = 0x240E0001;\n",
    "\t/* batch6f SMC: asm_SongA / asm_SongB = 0x240E0001 (li $t6, 1) at play_music_for_current_track+4 / func_8028F970+0x54 */\n",
)

text = text.replace(
    "\tg_sfxPause = 0; //Just for Amped Up (To fix some mute sfx when pausing the game)\n",
    "\t/* batch6: g_sfxPause is SequencePlayer overlay (gSequencePlayers+0x293), not a named BSS */\n",
)

text = text.replace(
    "\tFreeSpaceAddress = (int)&ok5_MenuStorage;\n",
    "\t/* FreeSpaceAddress was ok_Storage; dest is ok5_MenuStorage */\n",
)

text = text.replace(
    "    GlobalAddressD = 0x80690000;\n",
    "    /* batch6b: GlobalAddressD = 0x80690000 is inside the face/vtx window; CustomLevels retargets it */\n",
)

text = text.replace(
    "\t\tCullDL_Parameters = 0x0000000E;\n",
    "\t\t/* batch6: CullDL_Parameters poke; decoder uses local gsSPCullDisplayList(0, 7) */\n",
)
text = text.replace(
    "\t\tCullDL_Parameters = 0x00000140;\n",
    "\t\t/* batch6: CullDL_Parameters poke; decoder uses local gsSPCullDisplayList(0, 7) */\n",
)

text = text.replace("printAnticheat(true);", "printAnticheat();")

text = text.replace(
    "int PlayerID = (*(uint*)&Car - (uint)&g_PlayerStructTable) / 0xDD8;",
    "int PlayerID = (int)(Car - gPlayers);",
)

text = text.replace("g_player1ScreenWidth = 0x0240;", "GlobalScreen[0]->width = 0x0240;")

# Delete ScreenDrawHook / ClearFramebufferCheck (already native in this tree).
text = re.sub(r"\nvoid ScreenDrawHook\(void\)[\s\S]*\Z", "\n", text)

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

dst.parent.mkdir(parents=True, exist_ok=True)
dst.write_bytes(text.encode("latin-1"))
print("wrote", dst, "bytes", dst.stat().st_size)

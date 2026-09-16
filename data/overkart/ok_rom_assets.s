# OverKart Library ROM DMA sources (VMA = cart offset; not copied by init_overkart).
# IceKage / fonts / previews / Crash. Dest BSS stays in .okheader.

.include "macros.inc"

.section .data

    .balign 16
glabel IceKageROM
.incbin "assets/OverKartLibrary/newice.bin"
glabel IceKageEnd

    .balign 16
glabel previewN
.incbin "assets/OverKartLibrary/preview_n.mio0.bin"

    .balign 16
glabel previewU
.incbin "assets/OverKartLibrary/preview_u.mio0.bin"

    .balign 16
glabel NiceFontROM
.incbin "assets/OverKartLibrary/nice_font.mio0.bin"

    .balign 16
glabel HudButtonsROM
.incbin "assets/OverKartLibrary/hud_buttons.mio0.bin"

    .balign 16
glabel LibraryFont
.incbin "assets/OverKartLibrary/big_font.mio0.bin"
glabel LibraryFontEnd

    .balign 16
glabel AmpedUpFont
.incbin "assets/OverKartLibrary/AmpedUpFont.mio0.bin"
glabel AmpedUpFontEnd

    .balign 16
glabel Crash
.incbin "assets/OverKartLibrary/test/Crash512.bin"
glabel CrashEnd

    .balign 16

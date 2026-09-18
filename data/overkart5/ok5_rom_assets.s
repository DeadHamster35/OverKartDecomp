# OverKart5 ROM DMA sources (VMA = cart offset; not copied by init_overkart).
# After .overkart at 0xC00000. Crash / HudButtons / LibraryFont (NewFont) /
# AmpedUpFont already live in data/overkart/ok_rom_assets.s — do not duplicate.

.include "macros.inc"

.section .data

    .balign 16
glabel ArrowsSpriteROM
    .incbin "assets/OverKart5/arrows.png.MIO0"

    .balign 16
glabel RCSpriteROM
    .incbin "assets/OverKart5/RedCoinSprite16.png.MIO0"

    .balign 16
glabel NumbersSpriteROM
    .incbin "assets/OverKart5/number_sprites.png.MIO0"

    .balign 16
glabel RCIconMap
    .incbin "assets/OverKart5/RedSquare.png.RAW"

    .balign 16
glabel LogoROM
    .incbin "assets/OverKart5/KimuraBless.bin"

    .balign 16
glabel ROptionROM
    .incbin "assets/OverKart5/ROption.MIO0"
glabel ROptionEnd

    .balign 16
glabel MenuIconsROM
    .incbin "assets/OverKart5/OKLogo.bin"
glabel MenuIconsEnd

    .balign 16
glabel Splash3D
    .incbin "assets/OverKart5/TitleMenu.bin"
glabel Splash3DEnd

    .balign 16
glabel Pirate
    .incbin "assets/OverKart5/PiracyWarning.MIO0"
glabel PirateEnd

    .balign 16
glabel Seg12BIN
    .incbin "assets/OverKart5/Segment12.bin"
glabel Seg12END

    .balign 16
glabel JP_Bank
    .incbin "assets/OverKart5/JP_Bank.bin"

    .balign 16
glabel JP_Audio
    .incbin "assets/OverKart5/JP_Audio.bin"

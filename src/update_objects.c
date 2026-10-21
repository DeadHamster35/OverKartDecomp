#include <ultra64.h>
#include <macros.h>
#include <defines.h>
#include <decode.h>
#include <mk64.h>

#include "update_objects.h"
#include "main.h"
#include "memory.h"
#include "camera.h"
#include "math_util.h"
#include "math_util_2.h"
#include "render_objects.h"
#include "objects.h"
#include "path.h"
#include "code_800029B0.h"
#include "cpu_vehicles_camera_path.h"
#include "code_80057C60.h"
#include "code_8006E9C0.h"
#include "code_80086E70.h"
#include <assets/common_data.h>
#include "audio/external.h"
#include "sounds.h"
#include <actors.h>
#include "race_logic.h"
#include "collision.h"
#include "effects.h"
#include "code_80091440.h"
#include "menu_items.h"
#include "podium_ceremony_actors.h"
#include "courses/all_course_data.h"
#include <assets/ceremony_data.h>
#include "src/ending/ceremony_and_credits.h"
#include "menus.h"
#include "data/other_textures.h"
#include "data/some_data.h"
#include "memory.h"
#include "OverKart5Hooks.h"

//! @todo Unused?
f32 D_800E43B0[] = { 65536.0, 0.0, 1.0, 0.0, 0.0, 65536.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

Vtx DvtxKtile48X40[] = {
    { { { -24, -19, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, -19, 0 }, 0, { 3008, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, 20, 0 }, 0, { 3008, 2496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -24, 20, 0 }, 0, { 0, 2496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -24, -19, 0 }, 0, { 3008, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, -19, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, 20, 0 }, 0, { 0, 2496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -24, 20, 0 }, 0, { 3008, 2496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

Vtx DvtxTellesa48X40[] = {
    { { { -24, -19, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, -19, 0 }, 0, { 3008, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, 20, 0 }, 0, { 3008, 2496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -24, 20, 0 }, 0, { 0, 2496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

Vtx DvtxTellesa48X40Xflip[] = {
    { { { -24, -19, 0 }, 0, { 3008, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, -19, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, 20, 0 }, 0, { 0, 2496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -24, 20, 0 }, 0, { 3008, 2496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

u16* ItemboxtblPAL[] = { common_tlut_item_window_none,
                            common_tlut_item_window_banana,
                            common_tlut_item_window_banana_bunch,
                            common_tlut_item_window_green_shell,
                            common_tlut_item_window_triple_green_shell,
                            common_tlut_item_window_red_shell,
                            common_tlut_item_window_triple_red_shell,
                            common_tlut_item_window_blue_shell,
                            common_tlut_item_window_thunder_bolt,
                            common_tlut_item_window_fake_item_box,
                            common_tlut_item_window_star,
                            common_tlut_item_window_boo,
                            common_tlut_item_window_mushroom,
                            common_tlut_item_window_double_mushroom,
                            common_tlut_item_window_triple_mushroom,
                            common_tlut_item_window_super_mushroom };

u8* ItemboxtblIdx[] = { common_texture_item_window_none,
                              common_texture_item_window_banana,
                              common_texture_item_window_banana_bunch,
                              common_texture_item_window_green_shell,
                              common_texture_item_window_triple_green_shell,
                              common_texture_item_window_red_shell,
                              common_texture_item_window_triple_red_shell,
                              common_texture_item_window_blue_shell,
                              common_texture_item_window_thunder_bolt,
                              common_texture_item_window_fake_item_box,
                              common_texture_item_window_star,
                              common_texture_item_window_boo,
                              common_texture_item_window_mushroom,
                              common_texture_item_window_double_mushroom,
                              common_texture_item_window_triple_mushroom,
                              common_texture_item_window_super_mushroom };

u16* LapList[] = { common_texture_hud_lap_1_on_3, common_texture_hud_lap_2_on_3,
                           common_texture_hud_lap_3_on_3 };

u16* KaoColpalList[] = { common_tlut_portrait_mario, common_tlut_portrait_luigi,       common_tlut_portrait_yoshi,
                          common_tlut_portrait_toad,  common_tlut_portrait_donkey_kong, common_tlut_portrait_wario,
                          common_tlut_portrait_peach, common_tlut_portrait_bowser };

u8* KaoIndexList[] = { common_texture_portrait_mario,       common_texture_portrait_luigi,
                            common_texture_portrait_yoshi,       common_texture_portrait_toad,
                            common_texture_portrait_donkey_kong, common_texture_portrait_wario,
                            common_texture_portrait_peach,       common_texture_portrait_bowser };

s32 Kwalloc(s32* arg0) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = ObjectListSize;
    // clang-format off
    temp_v0 = 0; do { // this two SHOULD be on the same line
        // clang-format on
        ++temp_v1;
        ++temp_v0;

        if (temp_v1 == OBJECT_LIST_SIZE) {
            temp_v1 = 0;
        }
    } while ((DynamicObjects[temp_v1].Anmtrig != 0) && (temp_v0 != OBJECT_LIST_SIZE));

    DynamicObjects[temp_v1].Anmtrig = 1;

    *arg0 = temp_v1;
    ObjectListSize = temp_v1;
    return temp_v1;
}

//! @warning Does not clear struct members.
void Kwdealloc(s32* ObjectIndex) {
    KwanmFinish(*ObjectIndex);
    DynamicObjects[*ObjectIndex].Anmtrig = 0;
    *ObjectIndex = NULL_OBJECT_ID;
}

s32 KwcheckAllocval(void) {
    s32 ObjectIndex;
    s32 someCount = 0;
    for (ObjectIndex = 0; ObjectIndex < OBJECT_LIST_SIZE; ObjectIndex++) {
        if (DynamicObjects[ObjectIndex].Anmtrig != 0) {
            someCount++;
        }
    }
    return someCount;
}

s32 KwallocEffectram(s32* listIdx, s32* nextFree, s32 Size) {
    s32 Count;
    s32 ObjectIndex;
    s32* id;

    if (*nextFree >= Size) {
        *nextFree = 0;
    }
    Count = 0;
    id = &listIdx[*nextFree];

    for (Count = 0; Count < Size; Count++) {
        if (*id == NULL_OBJECT_ID) {
            ObjectIndex = Kwalloc(id);
            *nextFree += 1;
            break;
        } else {
            *nextFree += 1;
            if (*nextFree >= Size) {
                *nextFree = 0;
            }
            id = &listIdx[*nextFree];
        }
    }
    if (Count == Size) {
        ObjectIndex = NULL_OBJECT_ID;
    }
    return ObjectIndex;
}

void KwdeallocEffectram(s32* arg0) {
    Kwdealloc(arg0);
}

void KwclearAnybodyflag(s32* arg0, s32 arg1) {
    s32 i;

    for (i = 0; i < arg1; i++) {
        KwanmClrflag(*arg0, 0x00600000);
        arg0++;
    }
}

void KwlostGhost(void) {
    if (GameMode == TIME_TRIALS) {
        if (((PlayerOne->Flag & EXISTS) != 0) &&
            ((PlayerOne->Flag & (IS_GHOST | IS_CPU_PLAYER)) == 0)) {
            LostGhost = 1;
        }
    }
}

void KwanmSetflag(s32 ObjectIndex, s32 Flag) {
    DynamicObjects[ObjectIndex].Flag |= Flag;
}

void KwanmClrflag(s32 ObjectIndex, s32 Flag) {
    DynamicObjects[ObjectIndex].Flag &= ~Flag;
}

UNUSED void func_80072214(s32 ObjectIndex, s32 arg1) {
    DynamicObjects[ObjectIndex].Flag ^= arg1;
}

bool KwanmBitcheckflag(s32 ObjectIndex, s32 arg1) {
    s32 phi_v1 = false;
    if ((DynamicObjects[ObjectIndex].Flag & arg1) != 0) {
        phi_v1 = true;
    }
    return phi_v1;
}

s32 KwanmBitoffcheckflag(s32 ObjectIndex, s32 arg1) {
    s32 phi_v1 = 0;
    if ((DynamicObjects[ObjectIndex].Flag & arg1) == 0) {
        phi_v1 = 1;
    }
    return phi_v1;
}

void KwanmSetevflag(s32 ObjectIndex, s32 arg1) {
    DynamicObjects[ObjectIndex].Evflag |= arg1;
}

void KwanmClrevflag(s32 ObjectIndex, s32 arg1) {
    DynamicObjects[ObjectIndex].Evflag &= ~arg1;
}

UNUSED void func_800722F8(s32 ObjectIndex, s32 arg1) {
    DynamicObjects[ObjectIndex].Evflag ^= arg1;
}

bool KwanmBitcheckevflag(s32 ObjectIndex, s32 arg1) {
    s32 b = false;
    if ((DynamicObjects[ObjectIndex].Evflag & arg1) != 0) {
        b = true;
    }
    return b;
}

bool KwanmBitoffcheckevflag(s32 ObjectIndex, s32 arg1) {
    s32 b = false;
    if ((DynamicObjects[ObjectIndex].Evflag & arg1) == 0) {
        b = true;
    }
    return b;
}

void KwanmTrig(s32 ObjectIndex, s32 arg1) {
    DynamicObjects[ObjectIndex].Anmloop = arg1;
}

void KWAnmStart(s32 ObjectIndex, s32 arg1) {
    DynamicObjects[ObjectIndex].Flag = 0;
    DynamicObjects[ObjectIndex].Evflag = 0;
    DynamicObjects[ObjectIndex].Evtrflag = 0;
    DynamicObjects[ObjectIndex].Fvalloop = 0;
    DynamicObjects[ObjectIndex].Svalloop = 0;
    KwanmTrig(ObjectIndex, 0);
    DynamicObjects[ObjectIndex].Texsizex = arg1;
    DynamicObjects[ObjectIndex].Anmptr = 1;
}

UNUSED void KwanmBreak(s32 ObjectIndex) {
    KwanmTrig(ObjectIndex, 0);
}

void KwanmFinish(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].Anmptr = 0;
    DynamicObjects[ObjectIndex].Texsizex = 0;
    KwanmTrig(ObjectIndex, 0);
    DynamicObjects[ObjectIndex].Flag = 0;
    DynamicObjects[ObjectIndex].Evflag = 0;
    DynamicObjects[ObjectIndex].Evtrflag = 0;
    KwmvFinish(ObjectIndex);
}

void KwanmNext(s32 ObjectIndex) {
    KwanmTrig(ObjectIndex, 0);
    KwanmClrflag(ObjectIndex, 0x2000);
    DynamicObjects[ObjectIndex].Anmptr++;
}

void KwanmClearstack(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].Anmtype = 0;
}

void KwanmPushptr(s32 ObjectIndex, s32 arg1) {
    DynamicObjects[ObjectIndex].Stack[DynamicObjects[ObjectIndex].Anmtype] = arg1;
    DynamicObjects[ObjectIndex].Anmtype++;
}

s16 KwanmPullptr(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].Anmtype--;
    return DynamicObjects[ObjectIndex].Stack[DynamicObjects[ObjectIndex].Anmtype];
}

void KwanmGosubnext(s32 ObjectIndex, s32 arg1) {
    KwanmTrig(ObjectIndex, 0);
    KwanmClrflag(ObjectIndex, 0x2000);
    KwanmClrflag(ObjectIndex, 8);
    KwanmNext(ObjectIndex);
    KwanmPushptr(ObjectIndex, DynamicObjects[ObjectIndex].Anmptr);
    DynamicObjects[ObjectIndex].Anmptr = arg1;
}

void KwanmGosub(s32 ObjectIndex, s32 arg1, s32 Arg2) {
    KwanmTrig(ObjectIndex, 0);
    KwanmClrflag(ObjectIndex, 0x2000);
    KwanmClrflag(ObjectIndex, 8);
    DynamicObjects[ObjectIndex].Anmptr = Arg2;
    KwanmPushptr(ObjectIndex, DynamicObjects[ObjectIndex].Anmptr);
    DynamicObjects[ObjectIndex].Anmptr = arg1;
}

s16 KwanmPullptr(s32); // extern

void KwanmReturn(s32 ObjectIndex) {
    KwanmTrig(ObjectIndex, 0);
    KwanmClrflag(ObjectIndex, 0x2000);
    KwanmClrflag(ObjectIndex, 8);
    DynamicObjects[ObjectIndex].Anmptr = KwanmPullptr(ObjectIndex);
}

void KwanmJump(s32 ObjectIndex, s32 arg1) {
    KwanmTrig(ObjectIndex, 0);
    KwanmClrflag(ObjectIndex, 0x2000);
    DynamicObjects[ObjectIndex].Anmptr = arg1;
}

UNUSED void KwanmTrigjump(s32 ObjectIndex, s32 arg1) {
    if (DynamicObjects[ObjectIndex].Anmloop == 0) {
        DynamicObjects[ObjectIndex].Anmptr = arg1;
    }
}

UNUSED void KwanmWaittrig(s32 ObjectIndex) {
    if (DynamicObjects[ObjectIndex].Anmloop == 0) {
        KwanmNext(ObjectIndex);
    }
}

/**
 * @brief will return true if the Timer is done
 */
bool KwanmWait(s32 ObjectIndex, s32 Timer) {
    bool phi_v1;

    phi_v1 = false;
    if (DynamicObjects[ObjectIndex].Anmloop == false) {
        KwanmTrig(ObjectIndex, true);
        DynamicObjects[ObjectIndex].Anmtm = Timer;
    }

    DynamicObjects[ObjectIndex].Anmtm--;
    if (DynamicObjects[ObjectIndex].Anmtm < 0) {
        KwanmTrig(ObjectIndex, false);
        KwanmNext(ObjectIndex);
        phi_v1 = true;
    }

    return phi_v1;
}

UNUSED s32 KwanmRandomwait(s32 ObjectIndex, s32 arg1) {
    s32 phi_a2;

    phi_a2 = 0;
    if (DynamicObjects[ObjectIndex].Anmloop == 0) {
        KwanmTrig(ObjectIndex, 1);
        DynamicObjects[ObjectIndex].Anmtm = MakeRandomLimmit((u16) arg1);
    }

    DynamicObjects[ObjectIndex].Anmtm--;
    if (DynamicObjects[ObjectIndex].Anmtm < 0) {
        KwanmTrig(ObjectIndex, 0);
        KwanmNext(ObjectIndex);
        phi_a2 = 1;
    }

    return phi_a2;
}

UNUSED s32 KwanmBrandomwait(s32 ObjectIndex, s32 arg1, s32 Arg2) {
    s32 phi_a3;

    phi_a3 = 0;
    if (DynamicObjects[ObjectIndex].Anmloop == 0) {
        KwanmTrig(ObjectIndex, 1);
        DynamicObjects[ObjectIndex].Anmtm = MakeRandomLimmit((u16) Arg2) + arg1;
    }

    DynamicObjects[ObjectIndex].Anmtm--;
    if (DynamicObjects[ObjectIndex].Anmtm < 0) {
        KwanmTrig(ObjectIndex, 0);
        KwanmNext(ObjectIndex);
        phi_a3 = 1;
    }

    return phi_a3;
}

void KwanmIfequjump(s32 ObjectIndex, s32 arg1, s32 Arg2, s32 Arg3) {
    if (arg1 == Arg2) {
        DynamicObjects[ObjectIndex].Anmptr = Arg3;
    }
}

UNUSED void KwanmIfnequjump(s32 ObjectIndex, s32 arg1, s32 Arg2, s32 Arg3) {
    if (arg1 != Arg2) {
        DynamicObjects[ObjectIndex].Anmptr = Arg3;
    }
}

UNUSED void KwanmLoopstop(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].Fvaltrig = 0;
}

void KwanmSetinput(s32 ObjectIndex, s32 arg1) {
    DynamicObjects[ObjectIndex].Input = arg1;
}

void KwanmSetoutput(s32 ObjectIndex, s32 arg1) {
    DynamicObjects[ObjectIndex].Output = arg1;
}

void KwanmRacestart(s32 ObjectIndex) {
    u32 temp_v1 = 1;
    s32 i;

    StartRacing();
    KwanmNext(ObjectIndex);
    D_8018D2BC = 1;
    D_8018D2A4 = 1;

    if (CourseID != COURSE_YOSHI_VALLEY) {
        for (i = 0; i < MenuMultiplayerSelection; i++) {
            PlayerHUD[i].Ranksw = temp_v1;
        }
    }
    KwchartOpeningPanelStart();
}

UNUSED void KwanmSetnum(s32 ObjectIndex, s32 arg1) {
    DynamicObjects[ObjectIndex].Anmnumbak = arg1;
    KwanmNext(ObjectIndex);
}

s32 KwanmSetnumwait(s32 ObjectIndex, s32 arg1, s32 Arg2) {
    s32 phi_v1;

    phi_v1 = 0;
    if (DynamicObjects[ObjectIndex].Anmloop == 0) {

        KwanmTrig(ObjectIndex, 1);
        DynamicObjects[ObjectIndex].Anmnumbak = arg1;
        DynamicObjects[ObjectIndex].Anmtm = Arg2;
    }

    DynamicObjects[ObjectIndex].Anmtm--;
    if (DynamicObjects[ObjectIndex].Anmtm < 0) {
        KwanmTrig(ObjectIndex, 0);
        KwanmNext(ObjectIndex);
        phi_v1 = 1;
    }

    return phi_v1;
}

s32 KwanmHidden(s32 ObjectIndex, s32 arg1) {
    s32 phi_v1;

    phi_v1 = 0;
    if (DynamicObjects[ObjectIndex].Anmloop == 0) {
        KwanmTrig(ObjectIndex, 1);
        KwanmSetflag(ObjectIndex, 0x80000);
        DynamicObjects[ObjectIndex].Anmnumbak = D_8018D140;
        DynamicObjects[ObjectIndex].Anmtm = arg1;
    }

    DynamicObjects[ObjectIndex].Anmtm--;
    if (DynamicObjects[ObjectIndex].Anmtm < 0) {
        KwanmClrflag(ObjectIndex, 0x80000);
        KwanmTrig(ObjectIndex, 0);
        KwanmNext(ObjectIndex);
        phi_v1 = 1;
    }

    return phi_v1;
}

void KwanmFlash(s32 ObjectIndex, s32 arg1, s32 Arg2, s32 Arg3) {
    if (KwanmBitoffcheckflag(ObjectIndex, 0x2000) != 0) {
        DynamicObjects[ObjectIndex].Anmtm = Arg2;
        DynamicObjects[ObjectIndex].Anmnumbak = arg1;
        DynamicObjects[ObjectIndex].Type = 1;
        DynamicObjects[ObjectIndex].Fvaltrig = Arg3;
        KwanmSetflag(ObjectIndex, 0x2000);
    } else {
        DynamicObjects[ObjectIndex].Anmtm--;
        if (DynamicObjects[ObjectIndex].Anmtm < 0) {
            DynamicObjects[ObjectIndex].Anmtm = Arg2;
            DynamicObjects[ObjectIndex].Type--;
            if ((DynamicObjects[ObjectIndex].Type & 1) != 0) {
                KwanmClrflag(ObjectIndex, 0x80000);
            } else {
                KwanmSetflag(ObjectIndex, 0x80000);
            }

            if (DynamicObjects[ObjectIndex].Type < 0) {
                DynamicObjects[ObjectIndex].Type = 1;

                if (DynamicObjects[ObjectIndex].Fvaltrig > 0) {
                    DynamicObjects[ObjectIndex].Fvaltrig--;
                }

                if (DynamicObjects[ObjectIndex].Fvaltrig == 0) {
                    KwanmClrflag(ObjectIndex, 0x2000);
                    KwanmNext(ObjectIndex);
                }
            }
        }
    }
}

void KwanmSwap(s32 ObjectIndex, s32 arg1, s32 Arg2, s32 Arg3, s32 arg4) {
    if (KwanmBitoffcheckflag(ObjectIndex, 0x2000) != 0) {
        DynamicObjects[ObjectIndex].Anmtm = Arg3;
        DynamicObjects[ObjectIndex].Anmnumbak = arg1;
        DynamicObjects[ObjectIndex].Type = 1;
        DynamicObjects[ObjectIndex].Fvaltrig = arg4;
        KwanmSetflag(ObjectIndex, 0x2000);
        return;
    }

    DynamicObjects[ObjectIndex].Anmtm--;
    if (DynamicObjects[ObjectIndex].Anmtm < 0) {
        DynamicObjects[ObjectIndex].Anmtm = Arg3;
        DynamicObjects[ObjectIndex].Type--;
        if ((DynamicObjects[ObjectIndex].Type & 1) != 0) {
            DynamicObjects[ObjectIndex].Anmnumbak = arg1;
        } else {
            DynamicObjects[ObjectIndex].Anmnumbak = Arg2;
        }

        if (DynamicObjects[ObjectIndex].Type < 0) {
            DynamicObjects[ObjectIndex].Type = 1;
            if (DynamicObjects[ObjectIndex].Fvaltrig > 0) {
                DynamicObjects[ObjectIndex].Fvaltrig--;
            }

            if (DynamicObjects[ObjectIndex].Fvaltrig == 0) {
                KwanmClrflag(ObjectIndex, 0x2000);
                KwanmNext(ObjectIndex);
            }
        }
    }
}

s32 KwanmForward(s32 ObjectIndex, s32 arg1, s32 Arg2, s32 Arg3, s32 arg4, s32 arg5) {
    s32 sp24;

    sp24 = 0;
    if (KwanmBitoffcheckflag(ObjectIndex, 0x2000) != 0) {
        DynamicObjects[ObjectIndex].Anmnumbak = arg1;
        DynamicObjects[ObjectIndex].Anmtm = arg4;
        DynamicObjects[ObjectIndex].Fvaltrig = arg5;
        KwanmTrig(ObjectIndex, 1);
        KwanmSetflag(ObjectIndex, 0x2000);
    } else {
        DynamicObjects[ObjectIndex].Anmtm--;
        if (DynamicObjects[ObjectIndex].Anmtm <= 0) {
            DynamicObjects[ObjectIndex].Anmtm = arg4;
            DynamicObjects[ObjectIndex].Anmnumbak += Arg3;
            if (Arg2 < DynamicObjects[ObjectIndex].Anmnumbak) {

                if (DynamicObjects[ObjectIndex].Fvaltrig > 0) {
                    DynamicObjects[ObjectIndex].Fvaltrig--;
                }
                if (DynamicObjects[ObjectIndex].Fvaltrig == 0) {
                    DynamicObjects[ObjectIndex].Anmnumbak = Arg2;
                    KwanmClrflag(ObjectIndex, 0x2000);
                    KwanmTrig(ObjectIndex, 0);
                    KwanmNext(ObjectIndex);
                    sp24 = 1;
                } else {
                    DynamicObjects[ObjectIndex].Anmnumbak = arg1;
                }
            }
        }
    }
    return sp24;
}

bool KwanmReverse(s32 ObjectIndex, s32 arg1, s32 Arg2, s32 Arg3, s32 arg4, s32 arg5) {
    s32 sp24;

    sp24 = false;
    if (KwanmBitoffcheckflag(ObjectIndex, 0x2000) != 0) {
        DynamicObjects[ObjectIndex].Anmnumbak = arg1;
        DynamicObjects[ObjectIndex].Anmtm = arg4;
        DynamicObjects[ObjectIndex].Fvaltrig = arg5;
        KwanmTrig(ObjectIndex, 1);
        KwanmSetflag(ObjectIndex, 0x2000);
    } else {
        DynamicObjects[ObjectIndex].Anmtm--;
        if (DynamicObjects[ObjectIndex].Anmtm <= 0) {
            DynamicObjects[ObjectIndex].Anmtm = arg4;
            DynamicObjects[ObjectIndex].Anmnumbak -= Arg3;
            if (DynamicObjects[ObjectIndex].Anmnumbak < Arg2) {
                if (DynamicObjects[ObjectIndex].Fvaltrig > 0) {
                    DynamicObjects[ObjectIndex].Fvaltrig--;
                }
                if (DynamicObjects[ObjectIndex].Fvaltrig == 0) {
                    DynamicObjects[ObjectIndex].Anmnumbak = Arg2;
                    KwanmClrflag(ObjectIndex, 0x2000);
                    KwanmTrig(ObjectIndex, 0);
                    KwanmNext(ObjectIndex);
                    sp24 = true;
                } else {
                    DynamicObjects[ObjectIndex].Anmnumbak = arg1;
                }
            }
        }
    }
    return sp24;
}

bool KwanmOufukuF(s32 ObjectIndex, s32 arg1, s32 Arg2, s32 Arg3, s32 arg4, s32 arg5) {
    s32 sp24;

    sp24 = false;
    if (KwanmBitoffcheckflag(ObjectIndex, 0x2000) != 0) {
        DynamicObjects[ObjectIndex].Anmnumbak = arg1;
        DynamicObjects[ObjectIndex].Anmtm = arg4;
        DynamicObjects[ObjectIndex].Fvaltrig = arg5;
        KwanmTrig(ObjectIndex, 1);
        KwanmSetflag(ObjectIndex, 0x2000);
        KwanmClrflag(ObjectIndex, 0x4000);
    } else {
        DynamicObjects[ObjectIndex].Anmtm--;
        if (DynamicObjects[ObjectIndex].Anmtm <= 0) {
            DynamicObjects[ObjectIndex].Anmtm = arg4;
            if (KwanmBitoffcheckflag(ObjectIndex, 0x4000) != 0) {
                DynamicObjects[ObjectIndex].Anmnumbak += Arg3;
                if (DynamicObjects[ObjectIndex].Anmnumbak >= Arg2) {
                    DynamicObjects[ObjectIndex].Anmnumbak = Arg2;
                    KwanmSetflag(ObjectIndex, 0x4000);
                }
            } else {
                DynamicObjects[ObjectIndex].Anmnumbak -= Arg3;
                if (arg1 >= DynamicObjects[ObjectIndex].Anmnumbak) {
                    DynamicObjects[ObjectIndex].Anmnumbak = arg1;
                    if (DynamicObjects[ObjectIndex].Fvaltrig > 0) {
                        DynamicObjects[ObjectIndex].Fvaltrig--;
                    }

                    if (DynamicObjects[ObjectIndex].Fvaltrig == 0) {
                        KwanmClrflag(ObjectIndex, 0x80);
                        KwanmClrflag(ObjectIndex, 0x2000);
                        KwanmTrig(ObjectIndex, 0);
                        KwanmNext(ObjectIndex);
                        sp24 = true;
                    } else {
                        KwanmClrflag(ObjectIndex, 0x4000);
                        KwanmSetflag(ObjectIndex, 0x80);
                    }
                }
            }
        }
    }
    return sp24;
}

s32 KwanmOufukuR(s32 ObjectIndex, s32 arg1, s32 Arg2, s32 Arg3, s32 arg4, s32 arg5) {
    s32 sp24;

    sp24 = 0;
    if (KwanmBitoffcheckflag(ObjectIndex, 0x2000) != 0) {
        DynamicObjects[ObjectIndex].Anmnumbak = arg1;
        DynamicObjects[ObjectIndex].Anmtm = arg4;
        DynamicObjects[ObjectIndex].Fvaltrig = arg5;
        KwanmTrig(ObjectIndex, 1);
        KwanmSetflag(ObjectIndex, 0x2000);
        KwanmClrflag(ObjectIndex, 0x4000);
    } else {
        DynamicObjects[ObjectIndex].Anmtm--;
        if (DynamicObjects[ObjectIndex].Anmtm <= 0) {
            DynamicObjects[ObjectIndex].Anmtm = arg4;
            if (KwanmBitoffcheckflag(ObjectIndex, 0x4000) != 0) {
                DynamicObjects[ObjectIndex].Anmnumbak -= Arg3;
                if (Arg2 >= DynamicObjects[ObjectIndex].Anmnumbak) {
                    DynamicObjects[ObjectIndex].Anmnumbak = Arg2;
                    KwanmSetflag(ObjectIndex, 0x4000);
                }
            } else {
                DynamicObjects[ObjectIndex].Anmnumbak += Arg3;
                if (DynamicObjects[ObjectIndex].Anmnumbak >= arg1) {
                    DynamicObjects[ObjectIndex].Anmnumbak = arg1;
                    if (DynamicObjects[ObjectIndex].Fvaltrig > 0) {
                        DynamicObjects[ObjectIndex].Fvaltrig--;
                    }
                    if (DynamicObjects[ObjectIndex].Fvaltrig == 0) {
                        KwanmClrflag(ObjectIndex, 0x2000);
                        KwanmTrig(ObjectIndex, 0);
                        KwanmNext(ObjectIndex);
                        sp24 = 1;
                    } else {
                        KwanmClrflag(ObjectIndex, 0x4000);
                    }
                }
            }
        }
    }
    return sp24;
}

void KwanmInitCommon(s32 ObjectIndex, u8 arg1, u8 Arg2, Vtx* Arg3) {
    DynamicObjects[ObjectIndex].Vtxaddr = Arg3;
    DynamicObjects[ObjectIndex].Texsizey = arg1;
    DynamicObjects[ObjectIndex].Fvalptr = Arg2;
    DynamicObjects[ObjectIndex].Anmnumbak = 0;
    DynamicObjects[ObjectIndex].Anmcnt = -1;
    DynamicObjects[ObjectIndex].Flag = 0;
}

void KwanmInitCI8(s32 ObjectIndex, u8* Texture, u8* Arg2, u8 Arg3, u16 arg4) {
    DynamicObjects[ObjectIndex].Stexaddr = Texture;
    DynamicObjects[ObjectIndex].Sindexaddr = Arg2;
    DynamicObjects[ObjectIndex].Texsizey = Arg3;
    DynamicObjects[ObjectIndex].Fvalptr = arg4;
    DynamicObjects[ObjectIndex].Anmnumbak = 0;
    DynamicObjects[ObjectIndex].Anmcnt = -1;
    DynamicObjects[ObjectIndex].Flag = 0;
}

UNUSED void KwanmInitCI8PAL(s32 ObjectIndex, u8* arg1, u8 Arg2, u8 Arg3, Vtx* arg4) {
    DynamicObjects[ObjectIndex].Texaddr = arg1;
    DynamicObjects[ObjectIndex].Stexaddr = arg1;
    DynamicObjects[ObjectIndex].Texsizey = Arg2;
    DynamicObjects[ObjectIndex].Fvalptr = Arg3;
    DynamicObjects[ObjectIndex].Vtxaddr = arg4;
    DynamicObjects[ObjectIndex].Anmnumbak = 0;
    DynamicObjects[ObjectIndex].Anmcnt = -1;
    DynamicObjects[ObjectIndex].Flag = 0;
}

UNUSED void KwanmTexaddrCI8() {
}

void KwanmTexaddrCI8Colpal(s32 ObjectIndex) {
    // I have no idea why this typecast works
    DynamicObjects[ObjectIndex].Texaddr =
        (u8*) ((u32*) DynamicObjects[ObjectIndex].Stexaddr + (DynamicObjects[ObjectIndex].Anmnumbak * 128));
    DynamicObjects[ObjectIndex].Indexaddr = DynamicObjects[ObjectIndex].Sindexaddr;
}

void KwanmTexaddrCI8Index(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].Texaddr = DynamicObjects[ObjectIndex].Stexaddr;
    DynamicObjects[ObjectIndex].Indexaddr =
        DynamicObjects[ObjectIndex].Sindexaddr +
        (DynamicObjects[ObjectIndex].Texsizey * DynamicObjects[ObjectIndex].Fvalptr *
         DynamicObjects[ObjectIndex].Anmnumbak);
}

UNUSED void KwanmTexaddrCI8IndexCut2() {
}

UNUSED void KWChangecolor(s32 ObjectIndex) {
    s16* test = &DynamicObjects[ObjectIndex].Anmptr;

    D_8018D1EC++;
    if (D_8018D1EC == 5) {
        D_8018D1EC = 0;
    }
    (*test)++;
}

void KwanmInitPoly(s32 ObjectIndex, Gfx* arg1, f32 Arg2) {
    DynamicObjects[ObjectIndex].Flag = 0;
    DynamicObjects[ObjectIndex].Shape = arg1;
    DynamicObjects[ObjectIndex].Scale = Arg2;
    KwanmNext(ObjectIndex);
}

void KwchartOff(s32 ObjectIndex) {
    KwanmTrig(ObjectIndex, 0);
    DynamicObjects[ObjectIndex].Stackptr = 0;
    DynamicObjects[ObjectIndex].Timer = -1;
    KwanmClrflag(ObjectIndex, 0x8000);
}

void KwchartNext(s32 ObjectIndex) {
    KwanmClrflag(ObjectIndex, 0x8000);
    ++DynamicObjects[ObjectIndex].Stackptr;
}

UNUSED void KwchartJump(s32 ObjectIndex, s32 arg1) {
    KwanmClrflag(ObjectIndex, 0x8000);
    DynamicObjects[ObjectIndex].Stackptr = arg1;
}

void KwchartStart(s32 ObjectIndex) {
    KwanmClrflag(ObjectIndex, 0x8000);
    DynamicObjects[ObjectIndex].Stackptr = 1;
}

void KwchartFinish(s32 ObjectIndex) {
    KwanmClrflag(ObjectIndex, 0x8000);
    DynamicObjects[ObjectIndex].Stackptr = 0;
}

bool KwchartWait(s32 ObjectIndex, s32 arg1) {
    s32 sp24;

    sp24 = false;
    if (KwanmBitoffcheckflag(ObjectIndex, 0x00008000) != 0) {
        DynamicObjects[ObjectIndex].Timer = arg1;
        KwanmSetflag(ObjectIndex, 0x00008000);
    }
    DynamicObjects[ObjectIndex].Timer--;
    if (DynamicObjects[ObjectIndex].Timer < 0) {
        KwanmClrflag(ObjectIndex, 0x00008000);
        KwchartNext(ObjectIndex);
        sp24 = true;
    }
    return sp24;
}

void KwsetsvalTrig(s32 ObjectIndex, s32 arg1) {
    DynamicObjects[ObjectIndex].Svalloop = arg1;
}

void KwsetsvalNext(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].Mvtype++;
}

void KwsetsvalStart(s32 ObjectIndex) {
    KwsetsvalTrig(ObjectIndex, 0);
    DynamicObjects[ObjectIndex].Mvtype = 1;
}

void KwsetsvalFinish(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].Mvtype = 0;
    DynamicObjects[ObjectIndex].Svalloop = 0;
}

s32 KwsetsvalForwardSub(s32 ObjectIndex, s16* arg1, s32 Arg2, s32 Arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 phi_t0;

    phi_t0 = 0;
    if (DynamicObjects[ObjectIndex].Svalloop == 0) {
        *arg1 = Arg2;
        DynamicObjects[ObjectIndex].Svaltm = arg5;
        DynamicObjects[ObjectIndex].Player = arg6;
        KwsetsvalTrig(ObjectIndex, 1);
    } else {
        DynamicObjects[ObjectIndex].Svaltm--;
        if (DynamicObjects[ObjectIndex].Svaltm < 0) {
            DynamicObjects[ObjectIndex].Svaltm = arg5;
            *arg1 += arg4;
            if (Arg3 < *arg1) {
                if (DynamicObjects[ObjectIndex].Player > 0) {
                    DynamicObjects[ObjectIndex].Player--;
                }

                if (DynamicObjects[ObjectIndex].Player == 0) {
                    *arg1 = Arg3;
                    KwsetsvalTrig(ObjectIndex, 0);
                    KwsetsvalNext(ObjectIndex);
                    phi_t0 = 1;
                } else {
                    *arg1 = Arg2;
                }
            }
        }
    }

    return phi_t0;
}

void KwsetsvalForward(s32 arg0, s16* arg1, s32 Arg2, s32 Arg3, s32 arg4, s32 arg5, s32 arg6) {
    KwsetsvalForwardSub(arg0, arg1, Arg2, Arg3, arg4, arg5, arg6);
}

UNUSED void KwsetsvalForwardLimit(s32 arg0, s16* arg1, s32 Arg2, s32 Arg3, s32 arg4, s32 arg5, s32 arg6) {
    if (*arg1 < Arg3) {
        KwsetsvalForwardSub(arg0, arg1, Arg2, Arg3, arg4, arg5, arg6);
    }
}

bool KwsetsvalReverseSub(s32 ObjectIndex, s16* arg1, s32 Arg2, s32 Arg3, s32 arg4, s32 arg5, s32 arg6) {
    bool phi_t0;

    phi_t0 = false;
    if (DynamicObjects[ObjectIndex].Svalloop == 0) {
        *arg1 = Arg2;
        DynamicObjects[ObjectIndex].Svaltm = arg5;
        DynamicObjects[ObjectIndex].Player = arg6;
        KwsetsvalTrig(ObjectIndex, 1);
    } else {
        DynamicObjects[ObjectIndex].Svaltm--;
        if (DynamicObjects[ObjectIndex].Svaltm < 0) {
            DynamicObjects[ObjectIndex].Svaltm = arg5;
            *arg1 -= arg4;
            if (*arg1 < Arg3) {
                if (DynamicObjects[ObjectIndex].Player > 0) {
                    DynamicObjects[ObjectIndex].Player--;
                }

                if (DynamicObjects[ObjectIndex].Player == 0) {
                    *arg1 = Arg3;
                    KwsetsvalTrig(ObjectIndex, 0);
                    KwsetsvalNext(ObjectIndex);
                    phi_t0 = true;
                } else {
                    *arg1 = Arg2;
                }
            }
        }
    }

    return phi_t0;
}

s32 KwsetsvalReverse(s32 arg0, s16* arg1, s32 Arg2, s32 Arg3, s32 arg4, s32 arg5, s32 arg6) {
    return KwsetsvalReverseSub(arg0, arg1, Arg2, Arg3, arg4, arg5, arg6);
}

s32 KwsetsvalReverseLimit(s32 arg0, s16* arg1, s32 Arg2, s32 Arg3, s32 arg4, s32 arg5, s32 arg6) {
    if (Arg3 < *arg1) {
        return KwsetsvalReverseSub(arg0, arg1, Arg2, Arg3, arg4, arg5, arg6);
    }
}

bool KwsetsvalOufukuFSub(s32 arg0, s32 ObjectIndex, s16* Arg2, s32 Arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    s32 phi_t0;

    phi_t0 = false;
    if (DynamicObjects[ObjectIndex].Svalloop == 0) {
        DynamicObjects[ObjectIndex].Svaltm = arg6;
        if (arg0 != 0) {
            *Arg2 = Arg3;
        }
        DynamicObjects[ObjectIndex].Player = arg7;
        KwsetsvalTrig(ObjectIndex, 1);
    } else {
        DynamicObjects[ObjectIndex].Svaltm--;
        if (DynamicObjects[ObjectIndex].Svaltm < 0) {
            DynamicObjects[ObjectIndex].Svaltm = arg6;
            if (DynamicObjects[ObjectIndex].Svalloop == 1) {
                *Arg2 += arg5;
                if (*Arg2 >= arg4) {
                    *Arg2 = arg4;
                    DynamicObjects[ObjectIndex].Svalloop++;
                }
            } else {
                *Arg2 -= arg5;
                if (Arg3 >= *Arg2) {
                    *Arg2 = Arg3;
                    if (DynamicObjects[ObjectIndex].Player > 0) {
                        DynamicObjects[ObjectIndex].Player--;
                    }
                    if (DynamicObjects[ObjectIndex].Player == 0) {
                        KwsetsvalTrig(ObjectIndex, 0);
                        KwsetsvalNext(ObjectIndex);
                        phi_t0 = true;
                    } else {
                        DynamicObjects[ObjectIndex].Svalloop = 1;
                    }
                }
            }
        }
    }

    return phi_t0;
}

bool KwsetsvalOufukuF(s32 ObjectIndex, s16* arg1, s32 Arg2, s32 Arg3, s32 arg4, s32 arg5, s32 arg6) {
    return KwsetsvalOufukuFSub(1, ObjectIndex, arg1, Arg2, Arg3, arg4, arg5, arg6);
}

bool KwsetsvalOufukuFRnds(s32 ObjectIndex, s16* arg1, s32 Arg2, s32 Arg3, s32 arg4, s32 arg5, s32 arg6) {
    return KwsetsvalOufukuFSub(0, ObjectIndex, arg1, Arg2, Arg3, arg4, arg5, arg6);
}

UNUSED void KwsetsvalAddchase(s32 ObjectIndex, s16* arg1, s16 Arg2, s32 Arg3) {
    *arg1 = *arg1 + Arg3;
    if (*arg1 >= Arg2) {
        *arg1 = Arg2;
        KwsetsvalTrig(ObjectIndex, 0);
        KwsetsvalNext(ObjectIndex);
    }
}

void KwsetsvalSubchase(s32 ObjectIndex, s16* arg1, s16 Arg2, s32 Arg3) {
    *arg1 = *arg1 - Arg3;
    if (Arg2 >= *arg1) {
        *arg1 = Arg2;
        KwsetsvalTrig(ObjectIndex, 0);
        KwsetsvalNext(ObjectIndex);
    }
}

bool KwanmTurnLeft(s32 ObjectIndex, u16* arg1, u16 Arg2, s32 Arg3) {
    bool phi_t0;
    s32 temp_v1;

    phi_t0 = false;
    if (DynamicObjects[ObjectIndex].Svalloop == 0) {
        KwsetsvalTrig(ObjectIndex, 1);
        DynamicObjects[ObjectIndex].Ival = Arg3;
    }

    temp_v1 = DynamicObjects[ObjectIndex].Ival - Arg2;
    if (temp_v1 <= 0) {
        *arg1 += DynamicObjects[ObjectIndex].Ival;
        KwsetsvalTrig(ObjectIndex, 0);
        phi_t0 = true;
    } else {
        *arg1 += Arg2;
        DynamicObjects[ObjectIndex].Ival = temp_v1;
    }

    return phi_t0;
}

UNUSED bool KwanmTurnRight(s32 ObjectIndex, u16* arg1, u16 Arg2, s32 Arg3) {
    bool phi_t0;
    s32 temp_v1;

    phi_t0 = false;
    if (DynamicObjects[ObjectIndex].Svalloop == 0) {
        KwsetsvalTrig(ObjectIndex, 1);
        DynamicObjects[ObjectIndex].Ival = Arg3;
    }

    temp_v1 = DynamicObjects[ObjectIndex].Ival - Arg2;
    if (temp_v1 <= 0) {
        *arg1 += DynamicObjects[ObjectIndex].Ival;
        KwsetsvalTrig(ObjectIndex, 0);
        phi_t0 = true;
    } else {
        *arg1 -= Arg2;
        DynamicObjects[ObjectIndex].Ival = temp_v1;
    }
    return phi_t0;
}

void KwsetfvalTrig(s32 ObjectIndex, s32 arg1) {
    DynamicObjects[ObjectIndex].Fvalloop = arg1;
}

void KwsetfvalNext(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].Svalptr++;
}

void KwsetfvalStart(s32 ObjectIndex) {
    KwsetfvalTrig(ObjectIndex, 0);
    DynamicObjects[ObjectIndex].Svalptr = 1;
}

UNUSED void KwsetfvalFinish(void) {
}

bool KwsetfvalForwardSub(s32 ObjectIndex, f32* arg1, f32 Arg2, f32 Arg3, f32 arg4, s32 arg5, s32 arg6) {
    bool phi_a3;

    phi_a3 = false;
    if (DynamicObjects[ObjectIndex].Fvalloop == 0) {
        *arg1 = Arg2;
        DynamicObjects[ObjectIndex].Fvaltm = arg5;
        DynamicObjects[ObjectIndex].Svaltrig = arg6;
        KwsetfvalTrig(ObjectIndex, 1);
    } else {
        DynamicObjects[ObjectIndex].Fvaltm--;
        if ((s32) DynamicObjects[ObjectIndex].Fvaltm < 0) {
            DynamicObjects[ObjectIndex].Fvaltm = arg5;
            *arg1 += arg4;
            if (Arg3 < *arg1) {
                if ((s32) DynamicObjects[ObjectIndex].Svaltrig > 0) {
                    DynamicObjects[ObjectIndex].Svaltrig--;
                }
                if (DynamicObjects[ObjectIndex].Svaltrig == 0) {
                    *arg1 = Arg3;
                    KwsetfvalTrig(ObjectIndex, 0);
                    KwsetfvalNext(ObjectIndex);
                    phi_a3 = true;
                } else {
                    *arg1 = Arg2;
                }
            }
        }
    }

    return phi_a3;
}

s32 KwsetfvalForward(s32 ObjectIndex, f32* arg1, f32 Arg2, f32 Arg3, f32 arg4, s32 arg5, s32 arg6) {
    return KwsetfvalForwardSub(ObjectIndex, arg1, Arg2, Arg3, arg4, arg5, arg6);
}

s32 KwsetfvalForwardLimit(s32 ObjectIndex, f32* arg1, f32 Arg2, f32 Arg3, f32 arg4, s32 arg5, s32 arg6) {
    if (*arg1 < Arg3) {
        return KwsetfvalForwardSub(ObjectIndex, arg1, Arg2, Arg3, arg4, arg5, arg6);
    }
}

s32 KwsetfvalReverseSub(s32 ObjectIndex, f32* arg1, f32 Arg2, f32 Arg3, f32 arg4, s32 arg5, s32 arg6) {
    if (DynamicObjects[ObjectIndex].Fvalloop == 0) {
        *arg1 = Arg2;
        DynamicObjects[ObjectIndex].Fvaltm = arg5;
        DynamicObjects[ObjectIndex].Svaltrig = arg6;
        KwsetfvalTrig(ObjectIndex, 1);
    } else {
        DynamicObjects[ObjectIndex].Fvaltm += -1;
        if (DynamicObjects[ObjectIndex].Fvaltm < 0) {
            DynamicObjects[ObjectIndex].Fvaltm = arg5;
            *arg1 -= arg4;
            if (*arg1 < Arg3) {
                if (DynamicObjects[ObjectIndex].Svaltrig > 0) {
                    DynamicObjects[ObjectIndex].Svaltrig--;
                }
                if (DynamicObjects[ObjectIndex].Svaltrig == 0) {
                    *arg1 = Arg3;
                    KwsetfvalTrig(ObjectIndex, 0);
                    KwsetfvalNext(ObjectIndex);
                } else {
                    *arg1 = Arg2;
                }
            }
        }
    }

    return 0;
}

UNUSED void KwsetfvalReverse(s32 ObjectIndex, f32* arg1, f32 Arg2, f32 Arg3, f32 arg4, s32 arg5, s32 arg6) {
    KwsetfvalReverseSub(ObjectIndex, arg1, Arg2, Arg3, arg4, arg5, arg6);
}

UNUSED void KwsetfvalReverseLimit(s32 ObjectIndex, f32* arg1, f32 Arg2, f32 Arg3, f32 arg4, s32 arg5, s32 arg6) {
    if (Arg3 < *arg1) {
        KwsetfvalReverseSub(ObjectIndex, arg1, Arg2, Arg3, arg4, arg5, arg6);
    }
}

void KwsetfvalOufukuF(s32 ObjectIndex, f32* arg1, f32 Arg2, f32 Arg3, f32 arg4, s32 arg5, s32 arg6) {
    if (DynamicObjects[ObjectIndex].Fvalloop == 0) {
        *arg1 = Arg2;
        DynamicObjects[ObjectIndex].Fvaltm = arg5;
        DynamicObjects[ObjectIndex].Svaltrig = arg6;
        KwsetfvalTrig(ObjectIndex, 1);
        return;
    }

    DynamicObjects[ObjectIndex].Fvaltm--;
    if (DynamicObjects[ObjectIndex].Fvaltm < 0) {
        DynamicObjects[ObjectIndex].Fvaltm = arg5;
        if (DynamicObjects[ObjectIndex].Fvalloop == 1) {
            *arg1 += arg4;
            if (Arg3 <= *arg1) {
                *arg1 = Arg3;
                DynamicObjects[ObjectIndex].Fvalloop++;
            }
        } else {
            *arg1 -= arg4;
            if (*arg1 <= Arg2) {
                *arg1 = Arg2;

                if (DynamicObjects[ObjectIndex].Svaltrig > 0) {
                    DynamicObjects[ObjectIndex].Svaltrig--;
                }
                if (DynamicObjects[ObjectIndex].Svaltrig == 0) {
                    KwsetfvalTrig(ObjectIndex, 0);
                    KwsetfvalNext(ObjectIndex);
                } else {
                    DynamicObjects[ObjectIndex].Fvalloop = 1;
                }
            }
        }
    }
}

void KwdmaWait(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].Flag |= 1;
}

void Kwdma(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].Flag &= ~1;
}

void KwdmaTexture(void) {
    if (D_8018D224 != 0) {
        osRecvMesg(&DmaMesgQueue, &MainReceivedMesg, OS_MESG_BLOCK);
        D_8018D224 = 0;
    }
}

void KwanmSetdisplaybufferPAL(uintptr_t devAddr, void* vaddr, size_t nbytes) {
    KwdmaTexture();
    osPiStartDma(&DmaIoMesg, OS_MESG_PRI_NORMAL, OS_READ, devAddr, vaddr, nbytes, &DmaMesgQueue);
    D_8018D224 = 1;
}

void KwanmSetdisplaybufferAllpal(u8* arg0, void* arg1, u16 Arg2, u16 Arg3) {
    KwanmSetdisplaybufferPAL((uintptr_t) &_other_texturesSegmentRomStart[SEGMENT_OFFSET(arg0)], arg1, Arg2 * Arg3);
}

//! @todo arg1 should likely be a u8 *
void KwanmSetdisplaybuffer1PAL(s32 ObjectIndex, uintptr_t arg1) {
    s32 phi_a1;

    if ((DynamicObjects[ObjectIndex].Flag & 1) != 0) {
        phi_a1 = 0;
        if (DynamicObjects[ObjectIndex].Stexaddr != DynamicObjects[ObjectIndex].Texaddr) {
            DynamicObjects[ObjectIndex].Texaddr = DynamicObjects[ObjectIndex].Stexaddr;
        }

        DynamicObjects[ObjectIndex].Flag ^= 2;
        if ((DynamicObjects[ObjectIndex].Flag & 2) != 0) {
            phi_a1 = 1;
        }

        DynamicObjects[ObjectIndex].Indexaddr =
            (u8*) (DynamicObjects[ObjectIndex].Texsizey * DynamicObjects[ObjectIndex].Fvalptr * phi_a1) + arg1;
        Kwdma(ObjectIndex);
    }
}

void KwanmSetdrawbuffer(s32 ObjectIndex, uintptr_t arg1) {
    s32 phi_a1;

    if ((DynamicObjects[ObjectIndex].Flag & 1) != 0) {
        // I have no idea why this typecase works
        DynamicObjects[ObjectIndex].Texaddr =
            (u8*) ((u32*) DynamicObjects[ObjectIndex].Stexaddr + (DynamicObjects[ObjectIndex].Anmcnt << 7));
        DynamicObjects[ObjectIndex].Flag ^= 2;
        phi_a1 = 0;
        if ((DynamicObjects[ObjectIndex].Flag & 2) != 0) {
            phi_a1 = 1;
        }
        DynamicObjects[ObjectIndex].Indexaddr =
            (u8*) (DynamicObjects[ObjectIndex].Texsizey * DynamicObjects[ObjectIndex].Fvalptr * phi_a1) + arg1;
        Kwdma(ObjectIndex);
    }
}

void KwanmDMACI8(s32 ObjectIndex, uintptr_t arg1) {
    s32 phi_a1;

    if ((DynamicObjects[ObjectIndex].Flag & 1) != 0) {
        DynamicObjects[ObjectIndex].Texaddr = DynamicObjects[ObjectIndex].Stexaddr;
        DynamicObjects[ObjectIndex].Flag ^= 2;
        phi_a1 = 0;
        if ((DynamicObjects[ObjectIndex].Flag & 2) != 0) {
            phi_a1 = 1;
        }
        DynamicObjects[ObjectIndex].Indexaddr =
            (u8*) (DynamicObjects[ObjectIndex].Texsizey * DynamicObjects[ObjectIndex].Fvalptr * phi_a1) + arg1;
        Kwdma(ObjectIndex);
    }
}

u8* KwanmTexaddrCI8DMAPAL(s32 ObjectIndex, u8* arg1) {
    s32 phi_a2;

    DynamicObjects[ObjectIndex].Flag ^= 4;
    phi_a2 = 0;
    if ((DynamicObjects[ObjectIndex].Flag & 4) != 0) {
        phi_a2 = 1;
    }
    return (DynamicObjects[ObjectIndex].Texsizey * DynamicObjects[ObjectIndex].Fvalptr * phi_a2) + arg1;
}

void KwanmTexaddrCI8DMAAllpal(s32 ObjectIndex, u8* arg1) {
    u8* sp24;
    if (DynamicObjects[ObjectIndex].Anmnumbak != DynamicObjects[ObjectIndex].Anmcnt) {
        sp24 = DynamicObjects[ObjectIndex].Sindexaddr +
               (DynamicObjects[ObjectIndex].Texsizey * DynamicObjects[ObjectIndex].Fvalptr *
                DynamicObjects[ObjectIndex].Anmnumbak);
        KwanmSetdisplaybufferAllpal(sp24, (void*) KwanmTexaddrCI8DMAPAL(ObjectIndex, arg1), DynamicObjects[ObjectIndex].Texsizey,
                      DynamicObjects[ObjectIndex].Fvalptr);
        DynamicObjects[ObjectIndex].Anmcnt = DynamicObjects[ObjectIndex].Anmnumbak;
        KwdmaWait(ObjectIndex);
    }
}

void KwanmTexaddrCI8DMA1PAL(s32 ObjectIndex, u8* arg1) {
    KwanmTexaddrCI8DMAAllpal(ObjectIndex, arg1);
    KwanmSetdisplaybuffer1PAL(ObjectIndex, (uintptr_t) arg1);
}

void FuusenColtbl(s32 ObjectIndex, u8* arg1) {
    KwanmTexaddrCI8DMAAllpal(ObjectIndex, arg1);
    KwanmSetdrawbuffer(ObjectIndex, (uintptr_t) arg1);
}

void FuusenAddcoltbl(s32 ObjectIndex, u8* arg1) {
    KwanmTexaddrCI8DMAAllpal(ObjectIndex, arg1);
    KwanmDMACI8(ObjectIndex, (uintptr_t) arg1);
}

void KwanmFuusenInit(s32 ObjectIndex) {
    s32 sp2C;
    s32 sp28;
    s32 sp24;
    s32 sp20;
    s16 temp_v0;
    s32 temp_a0;
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    object->Scale = 0.15f;
    temp_v0 = CourseID;
    switch (temp_v0) { /* irregular */
        case COURSE_MARIO_RACEWAY:
            sp2C = MakeRandomLimmit(0x00C8U);
            sp28 = MakeRandomLimmit(D_80165748);
            sp24 = MakeRandomLimmit(0x0096U);
            sp20 = MakeRandomLimmit(0x2000U);
            object->PosStart[0] = (f32) ((((f64) D_80165718 + 100.0) - (f64) sp2C) * (f64) XOrientation);
            object->PosStart[1] = (f32) (D_80165720 + sp28);
            object->PosStart[2] = (f32) (((f64) D_80165728 + 200.0) - (f64) sp24);
            break;
        case COURSE_ROYAL_RACEWAY:
            sp2C = MakeRandomLimmit(0x0168U);
            sp28 = MakeRandomLimmit(D_80165748);
            sp24 = MakeRandomLimmit(0x00B4U);
            sp20 = MakeRandomLimmit(0x2000U);
            object->PosStart[0] = (f32) ((((f64) D_80165718 + 180.0) - (f64) sp2C) * (f64) XOrientation);
            object->PosStart[1] = (f32) (D_80165720 + sp28);
            object->PosStart[2] = (f32) (((f64) D_80165728 + 200.0) - (f64) sp24);
            break;
        case COURSE_LUIGI_RACEWAY:
            sp2C = MakeRandomLimmit(0x012CU);
            sp28 = MakeRandomLimmit(D_80165748);
            sp24 = MakeRandomLimmit(0x0096U);
            sp20 = MakeRandomLimmit(0x2000U);
            object->PosStart[0] = (f32) ((((f64) D_80165718 + 150.0) - (f64) sp2C) * (f64) XOrientation);
            object->PosStart[1] = (f32) (D_80165720 + sp28);
            object->PosStart[2] = (f32) (((f64) D_80165728 + 200.0) - (f64) sp24);
            break;
    }
    KwsetInit3DSppos(ObjectIndex, 0, 0, 0);
    if (MenuMultiplayerSelection == 1) {
        object->Velocity[1] = (f32) (((f64) (f32) (sp2C % 4) * 0.25) + 0.8);
    } else {
        object->Velocity[1] = (f32) (((f64) (f32) (sp2C % 3) * 0.2) + 0.4);
    }
    temp_a0 = sp2C % 8;
    object->Stack[0] = D_800E6F30[temp_a0][0];
    object->Stack[1] = D_800E6F30[temp_a0][1];
    object->Stack[2] = D_800E6F30[temp_a0][2];
    object->Stack[3] = D_800E6F48[temp_a0][0];
    object->Stack[4] = D_800E6F48[temp_a0][1];
    object->Stack[5] = D_800E6F48[temp_a0][2];
    object->Stack[6] = sp20 - 0x1000;
    if (sp2C & 1) {
        object->Stack[7] = (sp20 / 32) + 0x100;
    } else {
        object->Stack[7] = -0x100 - (sp20 / 32);
    }
    object->Alpha = 0x00E6;
    KwanmNext(ObjectIndex);
}

void KwmvFuusen(s32 ObjectIndex) {
    if (DynamicObjects[ObjectIndex].Mvptr == 1) {
        if ((D_80165740 <= DynamicObjects[ObjectIndex].PosSpline[1]) &&
            (KwsubSval(&DynamicObjects[ObjectIndex].Alpha, 0, 8) != 0)) {
            KwmvFinish(ObjectIndex);
        }
        KwmvAddvelytopos(ObjectIndex);
    }
    KwsetSplinepos3D(ObjectIndex);
}

void KwanmFuusen(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 1:
            KwanmFuusenInit(ObjectIndex);
            break;
        case 2:
            if (KwanmWait(ObjectIndex, 1) != false) {
                KwmvStart(ObjectIndex);
                break;
            }
        case 0:
            break;
        case 3:
            KwoufukuSval(&DynamicObjects[ObjectIndex].Stack[6], -0x1000, 0x1000, &DynamicObjects[ObjectIndex].Stack[7]);
            if (DynamicObjects[ObjectIndex].Mvptr == 0) {
                KwanmFinish(ObjectIndex);
            }
            break;
    }
}

void KwchartFuusen(void) {
    s32 someIndex;
    s32 ObjectIndex;
    s32 someCount;
    AnmObject* object;

    someCount = 0;
    for (someIndex = 0; someIndex < D_80165738; someIndex++) {
        ObjectIndex = EffectAllocArray3[someIndex];
        if (ObjectIndex != DELETED_OBJECT_ID) {
            object = &DynamicObjects[ObjectIndex];
            if (object->Anmptr != 0) {
                KwanmFuusen(ObjectIndex);
                KwmvFuusen(ObjectIndex);
                if (object->Anmptr == 0) {
                    KwdeallocEffectram(&EffectAllocArray3[someIndex]);
                }
                someCount += 1;
            }
        }
    }
    if (someCount == 0) {
        D_80165730 = 0;
    }
}

void KwanmBrokenObject(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Anmptr) { /* irregular */
        case 0:
            break;
        case 1:
            if (KwmvNanamenage(ObjectIndex, DynamicObjects[ObjectIndex].Velocity[1], 0.12f,
                              DynamicObjects[ObjectIndex].vel, DynamicObjects[ObjectIndex].ang[1],
                              0x00000064) != 0) {
                KwanmNext(ObjectIndex);
            }
            KwsetSplinepos3D(ObjectIndex);
            DynamicObjects[ObjectIndex].Texang[0] += D_8016582C[0];
            DynamicObjects[ObjectIndex].Texang[1] += D_8016582C[1];
            DynamicObjects[ObjectIndex].Texang[2] += D_8016582C[2];
            break;
        case 2:
            KwmvFinish(ObjectIndex);
            KwanmFinish(ObjectIndex);
            break;
    }
}

void KwsetBrokenObject(s32 ObjectIndex, s32 arg1, Vec3f Arg2, s32 Arg3, s32 arg4) {
    s32 sp24;
    s32 temp_v0;
    AnmObject* object;

    KWAnmStart(ObjectIndex, 0);
    sp24 = MakeRandomLimmit(0x01F4U);
    temp_v0 = MakeRandomLimmit(0x0032U);
    object = &DynamicObjects[ObjectIndex];
    object->Chartptr = Arg3;
    object->Scale = ((f64) (f32) sp24 * 0.0005) + 0.05;
    // ALL HAIL THE FAKE MATCH GODS!!!!!
    object->Velocity[1] = ((f64) (f32) temp_v0 * (0.05 * 1.0)) + 2.0;
    object->vel = ((f64) (f32) (temp_v0 % 5) * 0.1) + 1.0;
    object->ang[1] = (arg1 << 0x10) / arg4;
    object->PosStart[0] = (Arg2[0] + (temp_v0 / 2)) - 12.0f;
    object->PosStart[1] = (Arg2[1] - 10.0) + MakeRandomLimmit(0x000AU);
    object->PosStart[2] = (Arg2[2] + (temp_v0 / 2)) - 12.0f;
    object->Texang[0] = sp24 << 7;
    object->Texang[1] = temp_v0 * 0x50;
    object->Texang[2] = temp_v0 * 0x50;
}

void KwgetbrokenObject(Vec3f arg0, s32 arg1, s32 Arg2, s32 Arg3) {
    s32 var_s1;
    s32 ObjectIndex;

    for (var_s1 = 0; var_s1 < Arg3; var_s1++) {
        switch (arg1) { /* irregular */
            case 1:
                ObjectIndex = KwallocEffectram(EffectAllocArray1, &NextFreeObjectParticle1, gObjectParticle1_SIZE);
                break;
            case 2:
                ObjectIndex = KwallocEffectram(EffectAllocArray2, &NextFreeObjectParticle2, gObjectParticle2_SIZE);
                break;
            case 3:
                ObjectIndex = KwallocEffectram(EffectAllocArray3, &NextFreeObjectParticle3, gObjectParticle3_SIZE);
                break;
        }
        if (ObjectIndex == NULL_OBJECT_ID) {
            break;
        }
        KwsetBrokenObject(ObjectIndex, var_s1, arg0, Arg2, Arg3);
    }
}

void KwchartBrokenObject(s32 arg0) {
    s32 ObjectIndex;
    s32 var_s2;
    s32* var_s3;
    AnmObject* object;

    D_8016582C[0] += 0x2000;
    D_8016582C[1] += 0x1000;
    D_8016582C[2] += 0x1800;
    for (var_s2 = 0; var_s2 < 0x80; var_s2++) {
        switch (arg0) { /* irregular */
            case 1:
                var_s3 = EffectAllocArray1;
                break;
            case 2:
                var_s3 = EffectAllocArray2;
                break;
            case 3:
                var_s3 = EffectAllocArray3;
                break;
        }
        ObjectIndex = var_s3[var_s2];
        if (ObjectIndex != DELETED_OBJECT_ID) {
            object = &DynamicObjects[ObjectIndex];
            if (object->Anmptr != 0) {
                KwanmBrokenObject(ObjectIndex);
                if (object->Anmptr == 0) {
                    KwdeallocEffectram(&var_s3[var_s2]);
                }
            }
        }
    }
}

void KwinitTrainSmoke(s32 ObjectIndex, Vec3f pos, f32 Velocity) {
    AnmObject* object;
    UNUSED s32 pad[2];

    KWAnmStart(ObjectIndex, 0);
    object = &DynamicObjects[ObjectIndex];
    object->PosStart[0] = pos[0];
    object->PosStart[1] = pos[1];
    object->PosStart[2] = pos[2];
    object->Velocity[1] = Velocity;
    object->Output = MakeRandomLimmit(0x0064U) + 0x1E;
}

s32 KwgetTrainSmoke(s32 trainIndex, Vec3f pos, f32 Velocity) {
    s32 ObjectIndex;

    if (trainIndex == 0) {
        ObjectIndex = KwallocEffectram(EffectAllocArray2, &NextFreeObjectParticle2, gObjectParticle2_SIZE);
        if (ObjectIndex != NULL_OBJECT_ID) {
            KwinitTrainSmoke(ObjectIndex, pos, Velocity);
        }
    } else {
        ObjectIndex = KwallocEffectram(EffectAllocArray3, &NextFreeObjectParticle3, gObjectParticle3_SIZE);
        if (ObjectIndex != NULL_OBJECT_ID) {
            KwinitTrainSmoke(ObjectIndex, pos, Velocity);
        }
    }
    return ObjectIndex;
}

void KwanmTrainSmokeInit(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].Chartptr = 1;
    DynamicObjects[ObjectIndex].Indexaddr = D_8018D490;
    DynamicObjects[ObjectIndex].Sindexaddr = D_8018D490;
    DynamicObjects[ObjectIndex].Alpha = 0xFF;
    DynamicObjects[ObjectIndex].ang[1] = 0;
    DynamicObjects[ObjectIndex].Texang[0] = 0;
    DynamicObjects[ObjectIndex].Texang[2] = 0;
    DynamicObjects[ObjectIndex].PosSpline[0] = 0.0f;
    DynamicObjects[ObjectIndex].PosSpline[1] = 0.0f;
    DynamicObjects[ObjectIndex].PosSpline[2] = 0.0f;
    DynamicObjects[ObjectIndex].Scale = 0.5f;
    KwanmNext(ObjectIndex);
}

void KwanmTrainSmoke(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 1:
            KwanmTrainSmokeInit(ObjectIndex);
            break;
        case 2:
            DynamicObjects[ObjectIndex].Velocity[1] -= 0.03;
            KwaddFval(&DynamicObjects[ObjectIndex].PosSpline[1], 100.0f, DynamicObjects[ObjectIndex].Velocity[1]);
            KwsetfvalForwardLimit(ObjectIndex, &DynamicObjects[ObjectIndex].Scale, 0.55f, 1.0f, 0.1f, 1, 0);
            if (KwsetsvalReverse(ObjectIndex, &DynamicObjects[ObjectIndex].Alpha, 0xFF, 0x1E, 7, 0, 0) != 0) {
                KwanmNext(ObjectIndex);
            }
            break;
        case 3:
            KwanmFinish(ObjectIndex);
            break;
        case 0:
            break;
    }

    KwsetSplinepos3D(ObjectIndex);
}

void KwchartTrainSmoke(void) {
    UNUSED s32 pad[2];
    s32 Count;
    s32 i;
    s32 temp_a0;
    AnmObject* object;
    if (D_8016578C != 0) {
        D_8016578C -= 1;
    }
    if (D_80165790 != 0) {
        D_80165790 -= 1;
    }
    if ((SL[0].SomeFlags != 0) || (D_8016578C != 0)) {
        Count = 0;
        for (i = 0; i < 128; i++) {
            temp_a0 = EffectAllocArray2[i];
            if (temp_a0 != -1) {
                object = &DynamicObjects[temp_a0];
                if (object->Anmptr != 0) {
                    KwanmTrainSmoke(temp_a0);
                    if (object->Anmptr == 0) {
                        KwdeallocEffectram(&EffectAllocArray2[i]);
                    }
                    Count += 1;
                }
            }
        }
        if (Count != 0) {
            D_8016578C = 100;
        }
    }
    if ((SL[1].SomeFlags != 0) || (D_80165790 != 0)) {
        Count = 0;
        for (i = 0; i < 128; i++) {
            temp_a0 = EffectAllocArray3[i];
            if (temp_a0 != -1) {
                object = &DynamicObjects[temp_a0];
                if (object->Anmptr != 0) {
                    KwanmTrainSmoke(temp_a0);
                    if (object->Anmptr == 0) {
                        KwdeallocEffectram(&EffectAllocArray3[i]);
                    }
                    Count += 1;
                }
            }
        }
        if (Count != 0) {
            D_80165790 = 100;
        }
    }
}

void KwinitShipSmoke(s32 ObjectIndex, Vec3f pos, f32 Velocity) {
    AnmObject* object;

    KWAnmStart(ObjectIndex, 0);
    object = &DynamicObjects[ObjectIndex];
    object->PosStart[0] = pos[0];
    object->PosStart[1] = pos[1];
    object->PosStart[2] = pos[2];
    object->Velocity[1] = Velocity;
    object->Output = 0x00FF;
    object->Input = 0x0096;
}

s32 KwgetShipSmoke(s32 ferryIndex, Vec3f pos, f32 Velocity) {
    s32 ObjectIndex;

    if (ferryIndex == 0) {
        ObjectIndex = KwallocEffectram(EffectAllocArray2, &NextFreeObjectParticle2, gObjectParticle2_SIZE);
        if (ObjectIndex != NULL_OBJECT_ID) {
            KwinitShipSmoke(ObjectIndex, pos, Velocity);
        }
    } else {
        ObjectIndex = KwallocEffectram(EffectAllocArray3, &NextFreeObjectParticle3, gObjectParticle3_SIZE);
        if (ObjectIndex != NULL_OBJECT_ID) {
            KwinitShipSmoke(ObjectIndex, pos, Velocity);
        }
    }

    return ObjectIndex;
}

void KwanmShipSmokeInit(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].Chartptr = 6;
    DynamicObjects[ObjectIndex].Indexaddr = D_8018D490;
    DynamicObjects[ObjectIndex].Sindexaddr = D_8018D490;
    DynamicObjects[ObjectIndex].Alpha = 0xFF;
    DynamicObjects[ObjectIndex].ang[1] = 0;
    DynamicObjects[ObjectIndex].Texang[0] = 0;
    DynamicObjects[ObjectIndex].Texang[2] = 0;
    DynamicObjects[ObjectIndex].PosSpline[0] = 0.0f;
    DynamicObjects[ObjectIndex].PosSpline[1] = 0.0f;
    DynamicObjects[ObjectIndex].PosSpline[2] = 0.0f;
    DynamicObjects[ObjectIndex].Scale = 0.5f;
    KwanmNext(ObjectIndex);
}

void KwanmShipSmoke(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 1:
            KwanmShipSmokeInit(ObjectIndex);
            break;
        case 2:
            DynamicObjects[ObjectIndex].Velocity[1] -= 0.03;
            KwaddFval(&DynamicObjects[ObjectIndex].PosSpline[1], 100.0f, DynamicObjects[ObjectIndex].Velocity[1]);
            KwsetfvalForwardLimit(ObjectIndex, &DynamicObjects[ObjectIndex].Scale, 0.55f, 1.0f, 0.1f, 1, 0);
            if (KwsetsvalReverse(ObjectIndex, &DynamicObjects[ObjectIndex].Alpha, 0xFF, 0x1E, 7, 0, 0) != 0) {
                KwanmNext(ObjectIndex);
            }
            break;
        case 3:
            KwanmFinish(ObjectIndex);
            break;
        case 0:
            break;
    }

    KwsetSplinepos3D(ObjectIndex);
}

void KwchartShipSmoke(void) {
    UNUSED s32 pad[2];
    s32 Count;
    s32 i;
    s32 temp_a0;
    AnmObject* object;
    if (D_8016578C != 0) {
        D_8016578C -= 1;
    }
    if (D_80165790 != 0) {
        D_80165790 -= 1;
    }
    if ((Ship[0].SomeFlags != 0) || (D_8016578C != 0)) {
        Count = 0;
        for (i = 0; i < 128; i++) {
            temp_a0 = EffectAllocArray2[i];
            if (temp_a0 != -1) {
                object = &DynamicObjects[temp_a0];
                if (object->Anmptr != 0) {
                    KwanmShipSmoke(temp_a0);
                    if (object->Anmptr == 0) {
                        KwdeallocEffectram(&EffectAllocArray2[i]);
                    }
                    Count += 1;
                }
            }
        }
        if (Count != 0) {
            D_8016578C = 100;
        }
    }
    if ((Ship[1].SomeFlags != 0) || (D_80165790 != 0)) {
        Count = 0;
        for (i = 0; i < 128; i++) {
            temp_a0 = EffectAllocArray3[i];
            if (temp_a0 != -1) {
                object = &DynamicObjects[temp_a0];
                if (object->Anmptr != 0) {
                    KwanmShipSmoke(temp_a0);
                    if (object->Anmptr == 0) {
                        KwdeallocEffectram(&EffectAllocArray3[i]);
                    }
                    Count += 1;
                }
            }
        }
        if (Count != 0) {
            D_80165790 = 100;
        }
    }
}

void KwinitKokoopaFire(s32 ObjectIndex, Vec3f arg1, u16 Arg2, f32 Arg3, s32 arg4) {
    AnmObject* object;

    KWAnmStart(ObjectIndex, 0);
    object = &DynamicObjects[ObjectIndex];
    object->Scale = 0.5f;
    object->Chartptr = 5;
    object->PosStart[0] = arg1[0];
    object->PosStart[1] = arg1[1];
    object->PosStart[2] = arg1[2];
    object->ang[0] = 0x0C00;
    object->ang[2] = 0;
    object->vel = Arg3 * 4.0;
    object->ang[1] = Arg2;
    object->Output = 0x00FF;
    object->Input = 0x00FF;
    object->Ival = arg4 * 2;
}

s32 KwgetKokoopaFireSub(Vec3f arg0, u16 arg1, f32 Arg2, s32 Arg3) {
    s32 ObjectIndex;

    ObjectIndex = KwallocEffectram(EffectAllocArray1, &NextFreeObjectParticle1, gObjectParticle1_SIZE);
    if (ObjectIndex != NULL_OBJECT_ID) {
        KwinitKokoopaFire(ObjectIndex, arg0, arg1, Arg2, Arg3);
    }
    return ObjectIndex;
}

void KwgetKokoopaFire(Vec3f arg0, u16 arg1, f32 Arg2) {
    s32 someIndex;

    for (someIndex = 0; someIndex < 10; someIndex++) {
        if (KwgetKokoopaFireSub(arg0, arg1, Arg2, someIndex) == -1) {
            break;
        }
    }
}

void KwcheckKokoopafireSub(s32 ObjectIndex) {
    if (DynamicObjects[ObjectIndex].Timer > 0) {
        DynamicObjects[ObjectIndex].Timer--;
        if (DynamicObjects[ObjectIndex].Timer == 0) {
            KwanmClrevflag(ObjectIndex, 1);
        }
    }

    if (DynamicObjects[ObjectIndex].Ival > 0) {
        DynamicObjects[ObjectIndex].Ival--;
        if (DynamicObjects[ObjectIndex].Ival == 0) {
            NaSceneLevelStop(DynamicObjects[ObjectIndex].pos, SOUND_ARG_LOAD(0x51, 0x02, 0x80, 0x0A));
        }
    }

    if (DynamicObjects[ObjectIndex].Timer == 0) {
        KwcheckVisibleAreaCamera(ObjectIndex, 300.0f);
        if ((KwanmBitcheckflag(ObjectIndex, VISIBLE) != 0) && (KwanmBitoffcheckevflag(ObjectIndex, 1) != 0)) {
            KwanmSetevflag(ObjectIndex, 1);
            KwgetKokoopaFire(DynamicObjects[ObjectIndex].pos, DynamicObjects[ObjectIndex].ang[1], 1.0f);
            NaSceneLevelStart(DynamicObjects[ObjectIndex].pos, DynamicObjects[ObjectIndex].Velocity,
                          SOUND_ARG_LOAD(0x51, 0x02, 0x80, 0x0A));
            if (DynamicObjects[ObjectIndex].Output > 0) {
                DynamicObjects[ObjectIndex].Output--;
                DynamicObjects[ObjectIndex].Timer = 0x5A;
            } else {
                DynamicObjects[ObjectIndex].Timer = 0x12C;
            }

            DynamicObjects[ObjectIndex].Ival = 0x3C;
        }
    }

    if (KwcheckMapareain(9, 0xB) == 0) {
        DynamicObjects[ObjectIndex].Output = 2;
    }
}

void KwcheckKokoopaFire(void) {
    s32 someIndex;

    for (someIndex = 0; someIndex < 4; someIndex++) {
        KwcheckKokoopafireSub(Objallocptr3[someIndex]);
    }
}

void KwinitKoopaFire(s32 ObjectIndex, Vec3f arg1, f32 Arg2, s32 Arg3) {
    AnmObject* object;

    KWAnmStart(ObjectIndex, 0);
    object = &DynamicObjects[ObjectIndex];
    object->Chartptr = 4;
    object->Scale = 1.0f;
    object->PosStart[0] = arg1[0];
    object->PosStart[1] = arg1[1];
    object->PosStart[2] = arg1[2];
    object->ang[0] = 0x0C00;
    object->ang[2] = 0;
    object->ang[1] = 0x2100;
    if (ScreenFlip != 0) {
        object->ang[1] += -0x4000;
    }
    object->Output = 0x00FF;
    object->Input = 0x00FF;
    object->Ival = Arg3 * 2;
    object->vel = Arg2 * 8.0;
}

s32 KwgetKoopaFireSub(Vec3f arg0, f32 arg1, s32 Arg2) {
    s32 ObjectIndex;

    ObjectIndex = KwallocEffectram(EffectAllocArray1, &NextFreeObjectParticle1, gObjectParticle1_SIZE);
    if (ObjectIndex != NULL_OBJECT_ID) {
        KwinitKoopaFire(ObjectIndex, arg0, arg1, Arg2);
    }
    return ObjectIndex;
}

void KwgetKoopaFire(Vec3f arg0, f32 arg1) {
    s32 someIndex;

    for (someIndex = 0; someIndex < 0x14; someIndex++) {
        if (KwgetKoopaFireSub(arg0, arg1, someIndex) == -1) {
            break;
        }
    }
}

void KwanmKoopaFireInit(s32 ObjectIndex) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    object->Indexaddr = common_texture_particle_smoke[0];
    object->Sindexaddr = common_texture_particle_smoke[0];
    object->Alpha = 0x00FF;
    KwsetInit3DTexang(ObjectIndex, 0U, 0U, 0U);
    KwsetInit3DSppos(ObjectIndex, 0.0f, 0.0f, 0.0f);
    KwanmNext(ObjectIndex);
}

void KwanmKoopaFire(s32 ObjectIndex) {
    AnmObject* object;

    if (ObjectIndex) {}
    object = &DynamicObjects[ObjectIndex];
    switch (object->Anmptr) {
        case 1:
            KwanmKoopaFireInit(ObjectIndex);
            break;
        case 2:
            if (KwanmWait(ObjectIndex, object->Ival) != 0) {
                KwmvStart(ObjectIndex);
            }
            break;
        case 3:
            if (object->Chartptr == 4) {
                KWChaseFVal(&object->Scale, 4.0f, 0.1f);
                KWChaseSVal(&object->Output, 0, 0x0018);
                KWChaseSVal(&object->Input, 0x0080, 0x000C);
            } else {
                KWChaseFVal(&object->Scale, 1.0f, 0.1f);
                KWChaseSVal(&object->Output, 0, 0x0018);
                KWChaseSVal(&object->Input, 0x0080, 0x000C);
            }
            if ((object->Mvptr >= 2) &&
                (KwsetsvalReverse(ObjectIndex, &object->Alpha, 0x000000FF, 0x00000050, 0x00000020, 0, 0) != 0)) {
                KwanmNext(ObjectIndex);
            }
            break;
        case 4:
            KwanmFinish(ObjectIndex);
            KwmvFinish(ObjectIndex);
            break;
        case 0:
        default:
            break;
    }
}

void KwmvKoopaFire(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Mvptr) {
        case 0:
            break;
        case 1:
            if ((u8) DynamicObjects[ObjectIndex].Chartptr == 4) {
                if (KwmvWait(ObjectIndex, 0x0000000E) != 0) {
                    KWAnmNext(ObjectIndex);
                }
            } else if (KwmvWait(ObjectIndex, 2) != 0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 2:
            KwsubUsval(&DynamicObjects[ObjectIndex].ang[0], 0, 0x00000400);
            break;
    }
    if (DynamicObjects[ObjectIndex].Mvptr > 0) {
        KwmvVelangxytoposxyz(ObjectIndex);
        KwsetSplinepos3D(ObjectIndex);
    }
}

void KwcheckKoopaFire(void) {
    s32 ObjectIndex;

    ObjectIndex = Objallocptr2[0];
    if (DynamicObjects[ObjectIndex].Timer > 0) {
        DynamicObjects[ObjectIndex].Timer--;
        if (DynamicObjects[ObjectIndex].Timer == 0) {
            KwanmClrevflag(ObjectIndex, 1);
        }
    }
    if (DynamicObjects[ObjectIndex].Ival > 0) {
        DynamicObjects[ObjectIndex].Ival--;
        if (DynamicObjects[ObjectIndex].Ival == 0) {
            NaSceneLevelStop(DynamicObjects[ObjectIndex].pos, SOUND_ARG_LOAD(0x51, 0x03, 0x80, 0x09));
        }
    }
    if (DynamicObjects[ObjectIndex].Timer == 0) {
        KwcheckVisibleAreaCamera(ObjectIndex, 750.0f);
        if ((KwanmBitcheckflag(ObjectIndex, VISIBLE) != 0) && (KwanmBitoffcheckevflag(ObjectIndex, 1) != 0)) {
            KwanmSetevflag(ObjectIndex, 1);
            KwgetKoopaFire(DynamicObjects[ObjectIndex].pos, 1.0f);
            NaSceneLevelStart(DynamicObjects[ObjectIndex].pos, DynamicObjects[ObjectIndex].Velocity, 0x51038009U);
            if (DynamicObjects[ObjectIndex].Output > 0) {
                DynamicObjects[ObjectIndex].Output--;
                DynamicObjects[ObjectIndex].Timer = 0x0000005A;
            } else {
                DynamicObjects[ObjectIndex].Timer = 0x0000012C;
            }
            DynamicObjects[ObjectIndex].Ival = 0x0000003C;
        }
    }
    if (KwcheckMapareain(4, 5) == 0) {
        DynamicObjects[ObjectIndex].Output = 2;
    }
}

void KwinitFireWall(s32 ObjectIndex, Vec3s arg1, s32 Arg2) {
    AnmObject* object;

    KWAnmStart(ObjectIndex, 0);
    object = &DynamicObjects[ObjectIndex];
    object->Chartptr = 9;
    object->Scale = 1.0f;
    object->PosStart[0] = arg1[0];
    object->PosStart[1] = arg1[1];
    object->PosStart[2] = arg1[2];
    object->ang[0] = 0x0C00;
    object->ang[1] = 0x2100;
    object->ang[2] = 0;
    object->Output = 0x00FF;
    object->Input = 0x00FF;
    object->vel = 8.0f;
    object->Velocity[1] = 8.0f;
    object->Ival = Arg2;
}

s32 KwgetFireWallSub(Vec3s arg0, s32 arg1) {
    s32 ObjectIndex;

    ObjectIndex = KwallocEffectram(EffectAllocArray1, &NextFreeObjectParticle1, gObjectParticle1_SIZE);
    if (ObjectIndex != NULL_OBJECT_ID) {
        KwinitFireWall(ObjectIndex, arg0, arg1);
    }
    return ObjectIndex;
}

void KwgetFireWall(s32 arg0) {
    UNUSED s32 stackPadding0;
    s32 i;
    s32 temp_v0;
    s16* var_s2;
    temp_v0 = MakeRandomLimmit(0x000FU);
    switch (arg0) {
        case 0:
            var_s2 = D_800E5740 + (temp_v0 * 3);
            break;

        case 1:
            var_s2 = D_800E579C + (temp_v0 * 3);
            break;

        case 2:
            var_s2 = D_800E57F8 + (temp_v0 * 3);
            break;
    }

    for (i = 0; i < 1; i++) {
        if (KwgetFireWallSub(var_s2, i) == (-1)) {
            break;
        }
    }
}

void KwanmFireWallInit(s32 ObjectIndex) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    object->Indexaddr = common_texture_particle_smoke[0];
    object->Sindexaddr = common_texture_particle_smoke[0];
    object->Alpha = 0x00FF;
    KwsetInit3DTexang(ObjectIndex, 0U, 0U, 0U);
    KwsetInit3DSppos(ObjectIndex, 0.0f, 0.0f, 0.0f);
    KwanmNext(ObjectIndex);
}

void KwanmFireWall(s32 ObjectIndex) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    switch (object->Anmptr) {
        case 0:
            break;
        case 1:
            KwanmFireWallInit(ObjectIndex);
            break;
        case 2:
            if (KwanmWait(ObjectIndex, object->Ival) != 0) {
                KwmvStart(ObjectIndex);
            }
            break;
        case 3:
            KWChaseFVal(&object->Scale, 2.0f, 0.05f);
            KWChaseSVal(&object->Output, 0, 0x0018);
            if ((object->Mvptr >= 2) &&
                (KwsetsvalReverse(ObjectIndex, &object->Alpha, 0x000000FF, 0x00000050, 0x00000020, 0, 0) != 0)) {
                KwanmNext(ObjectIndex);
            }
            break;
        case 4:
            KwanmFinish(ObjectIndex);
            KwmvFinish(ObjectIndex);
            break;
    }
}

void KwmvFireWall(s32 ObjectIndex) {
    s32 thing;
    thing = DynamicObjects[ObjectIndex].Mvptr;
    if (thing) {}
    if (thing != 0) {
        if (thing == 1) {
            if (KwmvWait(ObjectIndex, 0x0000000A) != 0) {
                KWAnmNext(ObjectIndex);
            }
        } else {
            thing = DynamicObjects[ObjectIndex].Mvptr;
        }
    }
    if (DynamicObjects[ObjectIndex].Mvptr > 0) {
        KwmvAddvelytopos(ObjectIndex);
        KwsetSplinepos3D(ObjectIndex);
    }
}

void KwcheckFireWall(void) {
}

void KwchartFire(void) {
    s32 someIndex;
    s32 ObjectIndex;
    AnmObject* object;

    KwcheckKoopaFire();
    KwcheckKokoopaFire();
    for (someIndex = 0; someIndex < gObjectParticle1_SIZE; someIndex++) {
        ObjectIndex = EffectAllocArray1[someIndex];
        if (ObjectIndex != DELETED_OBJECT_ID) {
            object = &DynamicObjects[ObjectIndex];
            if (object->Anmptr != 0) {
                if ((object->Chartptr == 4) || (object->Chartptr == 5)) {
                    KwanmKoopaFire(ObjectIndex);
                    KwmvKoopaFire(ObjectIndex);
                } else if (object->Chartptr == 9) {
                    KwanmFireWall(ObjectIndex);
                    KwmvFireWall(ObjectIndex);
                }
                if (object->Anmptr == 0) {
                    KwdeallocEffectram(&EffectAllocArray1[someIndex]);
                }
            }
        }
    }
}

void KwgetKameFireInit(s32 ObjectIndex, Vec3f arg1, s16 Arg2) {
    AnmObject* object;

    KWAnmStart(ObjectIndex, (s32) Arg2);
    object = &DynamicObjects[ObjectIndex];
    object->Chartptr = 0x0A;
    object->Indexaddr = common_texture_particle_smoke[0];
    object->Sindexaddr = common_texture_particle_smoke[0];
    object->Scale = 0.3f;
    KwsetInit3DSpos(ObjectIndex, arg1[0], arg1[1], arg1[2]);
    object->Output = 0x00FF;
    object->vel = 0.0f;
    KwsetInit3DTexang(ObjectIndex, 0U, 0U, 0U);
    KwsetInit3DSppos(ObjectIndex, 0.0f, 0.0f, 0.0f);
}

void KwgetHibasira(Vec3f arg0, UNUSED f32 arg1, s16 Arg2) {
    s32 ObjectIndex;

    ObjectIndex = KwallocEffectram(FireParticleAllocArray, &FireParticleCounter, gObjectParticle4_SIZE);
    if (ObjectIndex != NULL_OBJECT_ID) {
        KwgetKameFireInit(ObjectIndex, arg0, Arg2);
    }
}

void KwanmKameFireInit(s32 ObjectIndex) {
    KwanmNext(ObjectIndex);
    if (DynamicObjects[ObjectIndex].Chartptr != 0x0B) {
        KwmvStart(ObjectIndex);
    }
}

void KwanmKameFire(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 1:
            KwanmKameFireInit(ObjectIndex);
            break;
        case 2:
            if ((DynamicObjects[ObjectIndex].Mvptr >= 2) &&
                (KwsetsvalReverse(ObjectIndex, &DynamicObjects[ObjectIndex].Alpha, 0x000000FF, 0x00000050, 0x00000020, 0,
                               0) != 0)) {
                KwanmNext(ObjectIndex);
            }
            break;
        case 0:
            break;
        case 3:
            KwanmFinish(ObjectIndex);
            KwmvFinish(ObjectIndex);
            break;
    }
}

void KwmvKameFire(s32 ObjectIndex) {
    if ((DynamicObjects[ObjectIndex].Mvptr != 0) && (DynamicObjects[ObjectIndex].Mvptr == 1)) {
        KWAnmNext(ObjectIndex);
    }
    KwsetSplinepos3D(ObjectIndex);
}

void KwchartKameFire(void) {
    s32 someIndex;
    s32 flameIndex;
    AnmObject* object;

    for (someIndex = 0; someIndex < gObjectParticle4_SIZE; someIndex++) {
        flameIndex = FireParticleAllocArray[someIndex];
        if (flameIndex != DELETED_OBJECT_ID) {
            object = &DynamicObjects[flameIndex];
            if (object->Anmptr != 0) {
                KwanmKameFire(flameIndex);
                KwmvKameFire(flameIndex);
                if (object->Anmptr == 0) {
                    KwdeallocEffectram(&FireParticleAllocArray[someIndex]);
                }
            }
        }
    }
}

void KwgetCaveFireInit(s32 ObjectIndex, s32 flameIndex) {
    KWAnmStart(ObjectIndex, 3);

    DynamicObjects[ObjectIndex].Chartptr = 0xB;
    DynamicObjects[ObjectIndex].Indexaddr = common_texture_particle_smoke[0];
    DynamicObjects[ObjectIndex].Sindexaddr = common_texture_particle_smoke[0];
    DynamicObjects[ObjectIndex].Scale = 0.8f;

    DynamicObjects[ObjectIndex].PosStart[0] = (f32) * (CaveFirePos + (flameIndex * 3) + 0) * XOrientation;
    DynamicObjects[ObjectIndex].PosStart[1] = (f32) * (CaveFirePos + (flameIndex * 3) + 1);
    DynamicObjects[ObjectIndex].PosStart[2] = (f32) * (CaveFirePos + (flameIndex * 3) + 2);
    DynamicObjects[ObjectIndex].vel = 0;
    DynamicObjects[ObjectIndex].Output = 255;
    DynamicObjects[ObjectIndex].Input = 255;
    DynamicObjects[ObjectIndex].Alpha = 255;
    KwsetInit3DTexang(ObjectIndex, 0, 0, 0);
    KwsetInit3DSppos(ObjectIndex, 0, 0, 0);
}

void KWGetCaveFire(s32 arg0) {
    s32 ObjectIndex;

    ObjectIndex = KwallocEffectram(FireParticleAllocArray, &FireParticleCounter, gObjectParticle4_SIZE);
    if (ObjectIndex != NULL_OBJECT_ID) {
        KwgetCaveFireInit(ObjectIndex, arg0);
    }
}

void KwgetStarInit(s32 ObjectIndex, Vec3f arg1, s32 Arg2) {
    s8 temp_v0_3;
    Vec3s sp30;

    KWAnmStart(ObjectIndex, Arg2);
    DynamicObjects[ObjectIndex].Chartptr = 0x0C;
    DynamicObjects[ObjectIndex].Scale = 0.05f;
    KwsetInit3DSpos(ObjectIndex, arg1[0], arg1[1], arg1[2]);
    KwsetInit3DTexang(ObjectIndex, 0U, 0U, 0U);
    KwsetInit3DSppos(ObjectIndex, 0.0f, 0.0f, 0.0f);
    switch (Arg2) {
        case 0:
            DynamicObjects[ObjectIndex].Velocity[1] = -1.0f;
            DynamicObjects[ObjectIndex].vel = (f32) ((MakeRandomLimmit(0x004BU) * 0.01) + 0.25);
            DynamicObjects[ObjectIndex].ang[1] = MakeRandomLimmit(0x0040U) << 0xA;
            KwmvVxz(ObjectIndex);
            DynamicObjects[ObjectIndex].Stack[5] = 0x001E;
            break;
        case 1:
            DynamicObjects[ObjectIndex].Velocity[1] = 1.5f;
            DynamicObjects[ObjectIndex].vel = (f32) ((MakeRandomLimmit(0x0064U) * 0.01) + 0.5);
            DynamicObjects[ObjectIndex].ang[1] = MakeRandomLimmit(0x0040U) << 0xA;
            KwmvVxz(ObjectIndex);
            DynamicObjects[ObjectIndex].Stack[5] = 0x0032;
            break;
    }
    temp_v0_3 = MakeRandomLimmit(0x000CU);
    if (temp_v0_3 < 9) {
        FlashcolorTable(temp_v0_3, &sp30[2], &sp30[1], sp30);
        DynamicObjects[ObjectIndex].Ival = 0;
        DynamicObjects[ObjectIndex].Stack[0] = sp30[2];
        DynamicObjects[ObjectIndex].Stack[1] = sp30[1];
        DynamicObjects[ObjectIndex].Stack[2] = sp30[0];
    } else {
        temp_v0_3 = MakeRandomLimmit(3U);
        KwgetColor(temp_v0_3, &sp30[2], &sp30[1], sp30);
        DynamicObjects[ObjectIndex].Stack[0] = sp30[2];
        DynamicObjects[ObjectIndex].Stack[1] = sp30[1];
        DynamicObjects[ObjectIndex].Stack[2] = sp30[0];
        DynamicObjects[ObjectIndex].Stack[4] = temp_v0_3;
        DynamicObjects[ObjectIndex].Ival = 1;
    }
    DynamicObjects[ObjectIndex].Alpha = 0x00FF;
    DynamicObjects[ObjectIndex].Stack[3] = MakeRandomLimmit(0x0800U) + 0x400;
    if ((DynamicObjects[ObjectIndex].ang[1] < 0x3000) ||
        (DynamicObjects[ObjectIndex].ang[1] >= 0xB001)) {
        DynamicObjects[ObjectIndex].Stack[3] = -DynamicObjects[ObjectIndex].Stack[3];
    }
}

void KWGetStar(f32* arg0, s32 arg1) {
    s32 ObjectIndex = KwallocEffectram(EffectAllocArray3, &NextFreeObjectParticle3, gObjectParticle3_SIZE);
    if (ObjectIndex != NULL_OBJECT_ID) {
        KwgetStarInit(ObjectIndex, arg0, arg1);
    }
}

void KwanmStarInit(s32 arg0) {
    KwanmNext(arg0);
    KwmvStart(arg0);
}

void KwanmStar(s32 ObjectIndex) {
    UNUSED s16 stackPadding0;
    s16 sp3C;
    s16 sp3A;
    s16 sp38;

    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 0:
            break;
        case 1:
            KwanmStarInit(ObjectIndex);
            break;
        case 2:
            KwaddFval(&DynamicObjects[ObjectIndex].Scale, 0.1f, 0.01f);
            if ((DynamicObjects[ObjectIndex].pos[1] <= DynamicObjects[ObjectIndex].Stack[5]) &&
                (KwsetsvalReverse(ObjectIndex, &DynamicObjects[ObjectIndex].Alpha, 0x000000FF, 0, 0x00000010, 0, 0) !=
                 0)) {
                KwmvFinish(ObjectIndex);
                KwanmFinish(ObjectIndex);
            }
            break;
    }
    if (DynamicObjects[ObjectIndex].Ival != 0) {
        DynamicObjects[ObjectIndex].Stack[4] = (s16) ((s32) (DynamicObjects[ObjectIndex].Stack[4] + 1) % 3);
        KwgetColor(DynamicObjects[ObjectIndex].Stack[4], &sp3C, &sp3A, &sp38);
        DynamicObjects[ObjectIndex].Stack[0] = sp3C;
        DynamicObjects[ObjectIndex].Stack[1] = sp3A;
        DynamicObjects[ObjectIndex].Stack[2] = sp38;
    }
}

void KwmvStar(s32 ObjectIndex) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    if ((object->Mvptr != 0) && (object->Mvptr == 1) && ((u8) object->Texsizex != 0)) {
        if (object->Velocity[1] >= -0.5) {
            object->Velocity[1] -= 0.15;
        } else {
            object->Velocity[2] = 0.0f;
            object->Velocity[0] = 0.0f;
        }
    }
    object->Texang[2] += object->Stack[3];
    KwmvAddveltopos(ObjectIndex);
    KwsetSplinepos3D(ObjectIndex);
}

void KWChartStar(void) {
    s32 someIndex;
    s32 ObjectIndex;
    AnmObject* object;

    for (someIndex = 0; someIndex < gObjectParticle3_SIZE; someIndex++) {
        ObjectIndex = EffectAllocArray3[someIndex];
        if (ObjectIndex != DELETED_OBJECT_ID) {
            object = &DynamicObjects[ObjectIndex];
            if (object->Anmptr != 0) {
                KwanmStar(ObjectIndex);
                KwmvStar(ObjectIndex);
                if (object->Anmptr == 0) {
                    KwdeallocEffectram(&EffectAllocArray3[someIndex]);
                }
            }
        }
    }
}

void KwinitTreeLeaf(s32 ObjectIndex, Vec3f arg1, s32 num) {
    UNUSED s32 stackPadding1;
    UNUSED u16 stackPadding0;
    u16 temp_s0;
    u16 sp3E;
    u16 sp3C;

    KWAnmStart(ObjectIndex, 0);
    DynamicObjects[ObjectIndex].Chartptr = 7;
    DynamicObjects[ObjectIndex].Texaddr = (u8*) common_texture_particle_leaf;
    DynamicObjects[ObjectIndex].Stexaddr = (u8*) common_texture_particle_leaf;
    DynamicObjects[ObjectIndex].Scale = 0.1f;
    DynamicObjects[ObjectIndex].Ground = arg1[1];
    switch (CourseID) {
        case COURSE_MARIO_RACEWAY:
            KwsetRandomxyz(ObjectIndex, arg1[0], arg1[1] + 25.0, arg1[2], 0x14, 0x1E, 0x14);
            DynamicObjects[ObjectIndex].vel = 1.5f;
            DynamicObjects[ObjectIndex].Velocity[1] = 1.5f;
            break;
        case COURSE_YOSHI_VALLEY:
            KwsetRandomxyz(ObjectIndex, arg1[0], arg1[1] + 25.0, arg1[2], 0x14, 0x1E, 0x14);
            DynamicObjects[ObjectIndex].vel = 2.0f;
            DynamicObjects[ObjectIndex].Velocity[1] = 2.0f;
            break;
        case COURSE_ROYAL_RACEWAY:
            KwsetRandomxyz(ObjectIndex, arg1[0], arg1[1] + 30.0, arg1[2], 0x10, 0x28, 0x10);
            DynamicObjects[ObjectIndex].vel = 2.0f;
            DynamicObjects[ObjectIndex].Velocity[1] = 2.0f;
            break;
        case COURSE_LUIGI_RACEWAY:
            KwsetRandomxyz(ObjectIndex, arg1[0], arg1[1] + 25.0, arg1[2], 0x14, 0x1E, 0x14);
            DynamicObjects[ObjectIndex].vel = 1.5f;
            DynamicObjects[ObjectIndex].Velocity[1] = 1.0f;
            break;
    }
    temp_s0 = MakeRandomLimmit(0x0010U) << 0xC;
    sp3E = MakeRandomLimmit(0x0010U) << 0xC;
    sp3C = MakeRandomLimmit(0x0010U) << 0xC;
    KwsetInit3DAng(ObjectIndex, 0U, (num * 0xFFFF) / 20, 0U);
    KwsetInit3DTexang(ObjectIndex, temp_s0, sp3E, sp3C);
}

s32 KwgetTreeLeaf(Vec3f arg0, s32 num) {
    s32 ObjectIndex;

    ObjectIndex = KwallocEffectram(Leafallocptr, &NextFreeLeafParticle, gLeafParticle_SIZE);
    if (ObjectIndex != NULL_OBJECT_ID) {
        KwinitTreeLeaf(ObjectIndex, arg0, num);
    }
    return ObjectIndex;
}

void KwhitTree(Vec3f arg0, UNUSED s32 arg1) {
    s32 i;

    for (i = 0; i < gLeafParticle_SPAWN_SIZE; i++) {
        if (KwgetTreeLeaf(arg0, i) == NULL_OBJECT_ID) {
            break;
        }
    }
}

void KwanmFallenLeafInit(s32 arg0) {
    KwanmNext(arg0);
    KwmvStart(arg0);
}

void KwanmFallenLeaf(s32 ObjectIndex) {
    AnmObject* object;
    object = &DynamicObjects[ObjectIndex];

    switch (object->Anmptr) {
        case 0:
            break;
        case 1:
            KwanmFallenLeafInit(ObjectIndex);
            break;
        case 2:
            if (object->Mvptr == 0) {
                KwanmNext(ObjectIndex);
            }
            break;
        case 3:
            KwanmFinish(ObjectIndex);
            break;
    }
}

void KwmvFallenLeaf(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Mvptr) {
        case 1:
            KwmvNanamenage(ObjectIndex, DynamicObjects[ObjectIndex].Velocity[1], 0.2f, DynamicObjects[ObjectIndex].vel,
                          (s16) (s32) DynamicObjects[ObjectIndex].ang[1], 0x0000000A);
            break;
        case 2:
            if (KwmvTrakkaGround(ObjectIndex, 0.4f, DynamicObjects[ObjectIndex].Ground) != 0) {
                KwmvFinish(ObjectIndex);
            }
            break;
    }
    KwsetSplinepos3D(ObjectIndex);
    DynamicObjects[ObjectIndex].Texang[0] += 0x1000;
    DynamicObjects[ObjectIndex].Texang[1] += 0x1000;
}

void KwchartLeaf(void) {
    s32 someIndex;
    s32 leafIndex;
    AnmObject* object;

    for (someIndex = 0; someIndex < gLeafParticle_SIZE; someIndex++) {
        leafIndex = Leafallocptr[someIndex];
        if (leafIndex != DELETED_OBJECT_ID) {
            object = &DynamicObjects[leafIndex];
            if (object->Anmptr != 0) {
                KwanmFallenLeaf(leafIndex);
                KwmvFallenLeaf(leafIndex);
                if (object->Anmptr == 0) {
                    KwdeallocEffectram(&Leafallocptr[someIndex]);
                }
            }
        }
    }
}

void KwanmSnowSearch2D(s32 arg0) {
    s32 ObjectIndex;
    s32 var_a1;

    if (KW2GFTimer == 0) {
        for (var_a1 = 0; var_a1 < CloudCount1P; var_a1++) {
            D_8018D17C += 1;
            if (D_8018D17C >= CloudCount1P) {
                D_8018D17C = 0;
            }
            ObjectIndex = CloudAllocate[arg0 + D_8018D17C];
            if (DynamicObjects[ObjectIndex].Anmptr == 0) {
                KWAnmStart(ObjectIndex, 1);
                break;
            }
        }
    }
}

void KwanmSnow2DInit(s32 ObjectIndex) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    object->Indexaddr = (HotSwapID > 0) ? (u8*) g_SnowParticleTex : D_0D0293D8;
    object->Sindexaddr = object->Indexaddr;
    // There's something up with the handling of Vtx_KTile16x16 and the loading of 0x10 Right here
    object->Vtxaddr = Vtx_KTile16x16;
    object->Fvalptr = 0x10;
    object->Texsizey = object->Fvalptr;
    object->Scale = g_skySnowScale;
    KwanmSetflag(ObjectIndex, 0x00000010);
    KwmvStartStrategy(ObjectIndex);
    object->Alpha = 0x00FF;
    object->Chartptr = 0;
    object->Output = 0;
    KwanmNext(ObjectIndex);
}

void KwanmVisiblecheckSnow(s32 ObjectIndex, u16 arg1, Camera* Camera) {
    s16 temp_v0;

    temp_v0 = Camera->CameraDirection[1] - arg1;
    if ((temp_v0 >= D_8018D210) || (D_8018D208 >= temp_v0)) {
        DynamicObjects[ObjectIndex].PosSpline[0] = D_8018D218 + (D_8018D1E8 * (f32) temp_v0);
        KwanmSetflag(ObjectIndex, 0x00000010);
        return;
    }
    KwanmClrflag(ObjectIndex, 0x00000010);
}

void KwmvSnow2D(s32 ObjectIndex, Camera* Camera) {

    f64 rand;

    switch (DynamicObjects[ObjectIndex].Mvptr) { /* irregular */
        case 1:
            DynamicObjects[ObjectIndex].ang[1] = (Camera->CameraDirection[1] + MakeRandomLimmit(0x4000U)) - DEGREES(45);
            KwsetRandomy(ObjectIndex, 0x00B4, 0x0014U);
            rand = MakeRandomLimmit(0x0064U);

            DynamicObjects[ObjectIndex].Velocity[1] = (f32) (-g_skySnowVelocity - (f64) (f32) (rand * 0.01));
            DynamicObjects[ObjectIndex].PosSpline[0] = 0.0f;
            DynamicObjects[ObjectIndex].PosSpline[1] = 0.0f;
            KWAnmNext(ObjectIndex);
            return;
        case 2:
            KwanmVisiblecheckSnow(ObjectIndex, DynamicObjects[ObjectIndex].ang[1], Camera);
            KwmvAddvelytopos(ObjectIndex);
            KwsetSplinepos3D(ObjectIndex);
            KwsetPos3Dto2D(ObjectIndex);
            if (DynamicObjects[ObjectIndex].pos[1] <= 0.0f) {
                KWAnmNext(ObjectIndex);
                return;
            }
        case 0:
            return;
        case 3:
            KwmvFinish(ObjectIndex);
            break;
    }
}

void KwanmSnow2D(s32 ObjectIndex, Camera* Camera) {
    switch (DynamicObjects[ObjectIndex].Anmptr) { /* irregular */
        case 1:
            KwanmSnow2DInit(ObjectIndex);
            return;
        case 2:
            KwmvSnow2D(ObjectIndex, Camera);
            if (DynamicObjects[ObjectIndex].Mvptr == 0) {
                KwanmNext(ObjectIndex);
                return;
            }
        case 0:
            return;
        case 3:
            KwanmFinish(ObjectIndex);
            break;
    }
}

void KwchartSnow2D(s32 arg0, Camera* arg1) {
    s32 ObjectIndex;
    s32 i;

    KwanmSnowSearch2D(arg0);
    for (i = 0; i < CloudCount1P; i++) {
        ObjectIndex = CloudAllocate[arg0 + i];
        if (DynamicObjects[ObjectIndex].Anmptr != 0) {
            KwanmSnow2D(ObjectIndex, arg1);
        }
    }
}

void KwanmSnow3DInit(s32 ObjectIndex) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    object->Indexaddr = (HotSwapID > 0) ? (u8*) g_SnowParticleTex : D_0D0293D8;
    object->Sindexaddr = object->Indexaddr;
    object->Vtxaddr = Vtx_KTile16x16;
    object->Scale = g_3DSnowScale;
    KwmvStartStrategy(ObjectIndex);
    KwanmNext(ObjectIndex);
}

void KwmvSnow3D(s32 ObjectIndex) {
    s16 sp3E;
    s16 sp3C;
    s16 sp3A;
    UNUSED u16 pad;
    u16 temp_t6;

    switch (DynamicObjects[ObjectIndex].Mvptr) { /* irregular */
        case 0:
            break;
        case 1:
            if (GameState != 9) {
                sp3A = ((PlayerOneCopy->Speed / 18) * 216) / 2;
                sp3E = (MakeRandomLimmit(0x000FU) - sp3A) + (s16) g_3DSnowSpawnHeight;
                sp3C = MakeRandomLimmit((u16) g_3DSnowSpawnDistanceMax) + (s16) g_3DSnowSpawnDistanceMin;
                temp_t6 = Camera1->CameraDirection[1] + ((s32) (MakeRandomLimmit((u16) g_3DSnowSpawnCone) - 0x1800) / (s16) ((sp3A / 15) + 1));
                DynamicObjects[ObjectIndex].PosStart[0] = PlayerOneCopy->Position[0] + (SinT(temp_t6) * sp3C);
                DynamicObjects[ObjectIndex].PosStart[1] = sp3E + PlayerOneCopy->Ground;
                DynamicObjects[ObjectIndex].PosStart[2] = PlayerOneCopy->Position[2] + (CosT(temp_t6) * sp3C);
                DynamicObjects[ObjectIndex].Addangy = MakeRandomLimmit(0x0400U) + 0x100;
                DynamicObjects[ObjectIndex].PosEnde[0] = (f32) (((f32) MakeRandomLimmit(0x0064U) * 0.03) + 2.0);
                DynamicObjects[ObjectIndex].Velocity[1] = (f32) (g_3DSnowVelocityLowLim - (f64) (f32) (MakeRandomLimmit(0x0032U) * 0.01));
                DynamicObjects[ObjectIndex].PosSpline[0] = 0.0f;
                DynamicObjects[ObjectIndex].PosSpline[1] = 0.0f;
                KWAnmNext(ObjectIndex);
            } else {
                sp3C = MakeRandomLimmit(0x0064U) + 0x28;
                temp_t6 = Camera1->CameraDirection[1] + MakeRandomLimmit(0x3000U) - 0x1800;
                DynamicObjects[ObjectIndex].PosStart[0] = Camera1->CameraPos[0] + (SinT(temp_t6) * sp3C);
                DynamicObjects[ObjectIndex].PosStart[1] = Camera1->CameraPos[1] + (f32) g_3DSnowSpawnHeight;
                DynamicObjects[ObjectIndex].PosStart[2] = Camera1->CameraPos[2] + (CosT(temp_t6) * sp3C);
                DynamicObjects[ObjectIndex].Addangy = MakeRandomLimmit(0x0400U) + 0x100;
                DynamicObjects[ObjectIndex].PosEnde[0] = (f32) (((f32) MakeRandomLimmit(0x0064U) * 0.03) + 2.0);
                DynamicObjects[ObjectIndex].Velocity[1] = (f32) (-0.6 - (f64) (f32) (MakeRandomLimmit(0x0032U) * 0.01));
                DynamicObjects[ObjectIndex].PosSpline[0] = 0.0f;
                DynamicObjects[ObjectIndex].PosSpline[1] = 0.0f;
                KWAnmNext(ObjectIndex);
            }
            break;
        case 2:
            KwmvAddvelytopos(ObjectIndex);
            DynamicObjects[ObjectIndex].ang[0] += DynamicObjects[ObjectIndex].Addangy;
            DynamicObjects[ObjectIndex].PosSpline[0] =
                SinT(DynamicObjects[ObjectIndex].ang[0]) * DynamicObjects[ObjectIndex].PosEnde[0] * g_3DSnowSwayMovement;
            KwsetSplinepos3D(ObjectIndex);
            if ((f64) DynamicObjects[ObjectIndex].pos[1] <= 0.0) {
                KWAnmNext(ObjectIndex);
            }
            DynamicObjects[ObjectIndex].Texang[1] = KwlookcameraXz(ObjectIndex, Camera1);
            SnowCustomCheck(ObjectIndex);
            break;
        case 3:
            KwmvFinish(ObjectIndex);
            break;
    }
}

void KwanmSnow3D(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Anmptr) { /* irregular */
        case 0:
            break;
        case 1:
            KwanmSnow3DInit(ObjectIndex);
            break;
        case 2:
            KwmvSnow3D(ObjectIndex);
            if (DynamicObjects[ObjectIndex].Mvptr == 0) {
                KwanmNext(ObjectIndex);
            }
            break;
        case 3:
            KwanmFinish(ObjectIndex);
            break;
    }
}

void KwanmSnowSearch3D(void) {
    s32 someIndex;

    for (someIndex = 0; someIndex < NUM_SNOWFLAKES; someIndex++) {
        D_8018D174 += 1;
        if (D_8018D174 >= NUM_SNOWFLAKES) {
            D_8018D174 = 0;
        }
        if (DynamicObjects[EffectAllocArray1[D_8018D174]].Anmptr == 0) {
            KWAnmStart(EffectAllocArray1[D_8018D174], 1);
            break;
        }
    }
}

void KWChartSnow(void) {
    s32 someIndex;
    s32 snowflakeIndex;

    if (GameState != CREDITS_SEQUENCE) {
        KwanmSnowSearch3D();
    } else {
        KwanmSnowSearch3D();
        KwanmSnowSearch3D();
        KwanmSnowSearch3D();
        KwanmSnowSearch3D();
    }
    for (someIndex = 0; someIndex < NUM_SNOWFLAKES; someIndex++) {
        snowflakeIndex = EffectAllocArray1[someIndex];
        if (DynamicObjects[snowflakeIndex].Anmptr != 0) {
            KwanmSnow3D(snowflakeIndex);
        }
    }
}

void KwanmVisiblecheckKumo(s32 ObjectIndex, u16 rot, Camera* Camera) {
    s16 temp_v0;

    temp_v0 = Camera->CameraDirection[1] + rot;
    if ((temp_v0 >= D_8018D210) && (D_8018D208 >= temp_v0)) {
        DynamicObjects[ObjectIndex].ix = (D_8018D218 + (D_8018D1E8 * temp_v0));
        KwanmSetflag(ObjectIndex, 0x00000010);
        return;
    }
    KwanmClrflag(ObjectIndex, 0x00000010);
}

void KwchartKumoKt(s32 arg0, Camera* arg1, CloudData* cloudList) {
    s32 cloudIndex;
    s32 ObjectIndex;
    CloudData* cloud;

    for (cloudIndex = 0; cloudIndex < CloudCount1P; cloudIndex++) {
        cloud = &cloudList[cloudIndex];
        ObjectIndex = CloudAllocate[arg0 + cloudIndex];
        KwanmVisiblecheckKumo(ObjectIndex, cloud->RotY, arg1);
    }
}

void KwchartStar(s32 arg0, Camera* Camera, StarData* starList) {
    s32 starIndex;
    s32 ObjectIndex;
    StarData* Star;

    for (starIndex = 0; starIndex < CloudCount1P; starIndex++) {
        Star = &starList[starIndex];
        ObjectIndex = CloudAllocate[arg0 + starIndex];
        KwanmVisiblecheckKumo(ObjectIndex, Star->RotY, Camera);
        switch (starIndex % 5U) {
            case 0:
                KwsetsvalOufukuF(ObjectIndex, &DynamicObjects[ObjectIndex].Alpha, 0x00000028, 0x000000B4, 0x000000FF, 0,
                              -1);
                break;
            case 1:
                KwsetsvalOufukuF(ObjectIndex, &DynamicObjects[ObjectIndex].Alpha, 0x00000080, 0x000000FF, 0x000000FF, 0,
                              -1);
                break;
            case 2:
                KwsetsvalOufukuF(ObjectIndex, &DynamicObjects[ObjectIndex].Alpha, 0x00000050, 0x000000C8, 0x000000FF, 0,
                              -1);
                break;
            case 3:
                KwsetsvalOufukuF(ObjectIndex, &DynamicObjects[ObjectIndex].Alpha, 0, 0x0000009B, 0x000000FF, 0, -1);
                break;
            case 4:
                KwsetsvalOufukuF(ObjectIndex, &DynamicObjects[ObjectIndex].Alpha, 0x0000005A, 0x00000080, 0x000000FF, 0,
                              -1);
                break;
        }
    }
}

UNUSED void func_80078C68() {
}

void KWChartKumo(s32 arg0) {
    s32 sp1C;
    Camera* Camera;

    if (CloudsToggle == 0) {
        switch (arg0) { /* switch 1 */
            case 0:     /* switch 1 */
                sp1C = 0;
                Camera = Camera1;
                D_8018D200 = CameraZoom[0] + 40.0f;
                break;
            case 1: /* switch 1 */
                sp1C = 0;
                Camera = Camera1;
                D_8018D200 = CameraZoom[0] + 40.0f;
                break;
            case 2: /* switch 1 */
                Camera = Camera2;
                sp1C = CloudCount1P;
                D_8018D200 = CameraZoom[1] + 40.0f;
                break;
            case 3: /* switch 1 */
                sp1C = 0;
                Camera = Camera1;
                D_8018D200 = CameraZoom[0] + 40.0f;
                break;
            case 4: /* switch 1 */
                Camera = Camera2;
                sp1C = CloudCount1P;
                D_8018D200 = CameraZoom[1] + 40.0f;
                break;
        }

        D_8018D208 = ((D_8018D200 / 2) * DEGREES(1)) + DEGREES(10);
        D_8018D210 = (-(D_8018D200 / 2) * DEGREES(1)) - DEGREES(10);
        D_8018D1E8 = 1.7578125 / D_8018D200;
        D_8018D218 = 0xA0;
#if !ENABLE_CUSTOM_COURSE_ENGINE
        switch (CourseID) {    /* switch 2 */
            case COURSE_MARIO_RACEWAY: /* switch 2 */
                // Uses Luigi Raceway's clouds for display purposes?
                KwchartKumoKt(sp1C, Camera, LuigiRacewayClouds);
                break;
            case COURSE_YOSHI_VALLEY: /* switch 2 */
                KwchartKumoKt(sp1C, Camera, YoshiValleyMooMooFarmClouds);
                break;
            case COURSE_FRAPPE_SNOWLAND: /* switch 2 */
                KwchartSnow2D(sp1C, Camera);
                break;
            case COURSE_KOOPA_BEACH: /* switch 2 */
                KwchartKumoKt(sp1C, Camera, KoopaTroopaBeachClouds);
                break;
            case COURSE_ROYAL_RACEWAY: /* switch 2 */
                KwchartKumoKt(sp1C, Camera, RoyalRacewayClouds);
                break;
            case COURSE_LUIGI_RACEWAY: /* switch 2 */
                KwchartKumoKt(sp1C, Camera, LuigiRacewayClouds);
                break;
            case COURSE_MOO_MOO_FARM: /* switch 2 */
                KwchartKumoKt(sp1C, Camera, YoshiValleyMooMooFarmClouds);
                break;
            case COURSE_TOADS_TURNPIKE: /* switch 2 */
                KwchartStar(sp1C, Camera, ToadsTurnpikeRainbowRoadStars);
                break;
            case COURSE_KALAMARI_DESERT: /* switch 2 */
                KwchartKumoKt(sp1C, Camera, KalimariDesertClouds);
                break;
            case COURSE_SHERBET_LAND: /* switch 2 */
                KwchartKumoKt(sp1C, Camera, SherbetLandClouds);
                break;
            case COURSE_RAINBOW_ROAD: /* switch 2 */
                KwchartStar(sp1C, Camera, ToadsTurnpikeRainbowRoadStars);
                break;
            case COURSE_WARIO_STADIUM: /* switch 2 */
                KwchartStar(sp1C, Camera, WarioStadiumStars);
                break;
        }
    }
#else

#endif
}

void Kwjugemustart(void) {
    switch (ScreenSplitB) { /* irregular */
        case SCREEN_MODE_1P:
            KWAnmStart(Jugemuallocptr[0], 1);
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            KWAnmStart(Jugemuallocptr[0], 1);
            KWAnmStart(Jugemuallocptr[1], 1);
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            KWAnmStart(Jugemuallocptr[0], 1);
            KWAnmStart(Jugemuallocptr[1], 1);
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            KWAnmStart(Jugemuallocptr[0], 1);
            KWAnmStart(Jugemuallocptr[1], 1);
            KWAnmStart(Jugemuallocptr[2], 1);
            KWAnmStart(Jugemuallocptr[3], 1);
            break;
    }
}

void KwgoalJugemu(s32 PlayerId) {
    KWAnmStart(Jugemuallocptr[PlayerId], 2);
}

void Kwlap2Jugemu(s32 PlayerId) {
    KWAnmStart(Jugemuallocptr[PlayerId], 4);
}

void KwfinalJugemu(s32 PlayerId) {
    KWAnmStart(Jugemuallocptr[PlayerId], 5);
}

void KwreverseJugemu(s32 PlayerId) {
    KWAnmStart(Jugemuallocptr[PlayerId], 6);
}

void KwanmDmajgm(s32 ObjectIndex, s32 arg1, s32 Arg2) {
    s32 a;
    if (DynamicObjects[ObjectIndex].Anmptr >= 2) {
        if ((u8) DynamicObjects[ObjectIndex].Texsizex == 1) {
            if (arg1 == 0) {
                KwanmTexaddrCI8DMA1PAL(ObjectIndex, LakituTexturePtr);
                return;
            }
            a = Jugemuallocptr[0];
            DynamicObjects[ObjectIndex].Texaddr = DynamicObjects[a].Texaddr;
            DynamicObjects[ObjectIndex].Indexaddr = DynamicObjects[a].Indexaddr;
            if (0) {}
            return;
        }
        switch (Arg2) {
            case 0:
                FuusenAddcoltbl(ObjectIndex, LakituTexturePtr);
                break;
            case 1:
                FuusenColtbl(ObjectIndex, LakituTexturePtr);
                break;
            case 2:
                KwanmTexaddrCI8DMA1PAL(ObjectIndex, LakituTexturePtr);
                break;
        }
    }
}

void KwanmJgmBreak(s32 ObjectIndex, s32 PlayerId) {
    Player* Player = &PlayerOne[PlayerId];

    if ((DynamicObjects[ObjectIndex].Texsizex != 3) && (DynamicObjects[ObjectIndex].Texsizex != 7)) {
        KwanmClrevflag(ObjectIndex, 1);
        if (CourseID == COURSE_SHERBET_LAND) {
            Player->JugemuFlag &= ~IS_FROZEN;
        }
    } else {
        // ?????
    }
    if (CourseID == COURSE_SHERBET_LAND) {
        KwanmClrevflag(ObjectIndex, 0x00000010);
        Player->JugemuFlag &= ~IS_MELTING;
    }
    NaPlyLevelStop(PlayerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x28));
}

void KwanmJgmSInit(s32 ObjectIndex, s32 arg1) {
    if (arg1 == 0) {
        D_801656F0 = 0;
        D_8018D168 = 0;
    }
    KwanmInitCI8(ObjectIndex, (u8*) common_tlut_lakitu_countdown, gTextureLakituNoLights1, 0x38U,
                        (u16) 0x00000048);
    DynamicObjects[ObjectIndex].Vtxaddr = common_vtx_lakitu;
    DynamicObjects[ObjectIndex].Scale = 0.15f;
    KwanmClrflag(ObjectIndex, 0x00000010);
    KwanmNext(ObjectIndex);
    DynamicObjects[ObjectIndex].Ival = D_8018D180;
}

void KwanmJgmS(s32 ObjectIndex, s32 arg1) {
    UNUSED s32 pad;
    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 0:
            break;
        case 1:
            KwanmJgmSInit(ObjectIndex, arg1);
            break;
        case 2:
            KwanmWait(ObjectIndex, DynamicObjects[ObjectIndex].Ival);
            if ((DynamicObjects[ObjectIndex].Anmtm == 0x00000055) && (MenuMultiplayerSelection == 3) && (arg1 == 0)) {
                D_8018D168 = 1;
            }
            break;
        case 3:
            KwanmSetflag(ObjectIndex, 0x00000010);
            KwmvStartSpline(ObjectIndex, 1, &D_800E67B8); // set a spline
            KwanmNext(ObjectIndex);
            break;
        case 4:
            if ((KwanmWait(ObjectIndex, 0x0000001E) != false) && (MenuMultiplayerSelection != 3) && (arg1 == 0)) {
                D_8018D168 = 1;
            }
            break;
        case 5:
            KwanmWait(ObjectIndex, 0x0000001E);
            break;
        case 6:
            KwanmForward(ObjectIndex, 1, 7, 1, 2, 0);
            break;
        case 7:
            if (KwanmWait(ObjectIndex, 0x00000014) != 0) {
                DynamicObjects[ObjectIndex].Stexaddr += 0x200;
                if (arg1 == 0) {
                    PlaySound(SOUND_ACTION_COUNTDOWN_LIGHT);
                }
            }
            break;
        case 8:
            KwanmForward(ObjectIndex, 8, 0x0000000F, 1, 6, 0);
            break;
        case 9:
            if ((KwanmWait(ObjectIndex, 8) != 0) && (arg1 == 0)) {
                PlaySound(SOUND_ACTION_COUNTDOWN_LIGHT);
            }
            break;
        case 10:
            if ((KwanmForward(ObjectIndex, 0x00000010, 0x00000017, 1, 6, 0) != 0) && (arg1 == 0)) {
                D_801656F0 = 1;
            }
            break;
        case 11:
            if (KwanmWait(ObjectIndex, 8) != 0) {
                DynamicObjects[ObjectIndex].Stexaddr += 0x200;
                if (arg1 == 0) {
                    PlaySound(SOUND_ACTION_GREEN_LIGHT);
                }
            }
            break;
        case 12:
            KwanmForward(ObjectIndex, 0x00000018, 0x0000001B, 1, 6, 0);
            break;
        case 13:
            if (arg1 == 0) {
                KwanmRacestart(ObjectIndex);
                D_8018D160 = 1;
                break;
            }
            KwanmNext(ObjectIndex);
            break;
        case 14:
            KwanmWait(ObjectIndex, 0x00000078);
            break;
        case 15:
            KwanmFinish(ObjectIndex);
            break;
    }
}

void KwanmJgmGInit(s32 ObjectIndex, s32 playerIndex) {
    AnmObject* object;

    KwanmJgmBreak(ObjectIndex, playerIndex);
    KwanmInitCI8(ObjectIndex, (u8*) common_tlut_lakitu_checkered_flag, gTextureLakituCheckeredFlag01, 0x48U,
                        (u16) 0x00000038);
    object = &DynamicObjects[ObjectIndex];
    object->Indexaddr = LakituTexturePtr;
    object->Vtxaddr = common_vtx_also_lakitu;
    object->pos[2] = 5000.0f;
    object->pos[1] = 5000.0f;
    object->pos[0] = 5000.0f;
    object->Scale = 0.15f;
    KwmvStartSpline(ObjectIndex, 2, &D_800E6834);
    KwanmClrflag(ObjectIndex, 0x00000010);
    KwanmNext(ObjectIndex);
}

void KwanmJgmG(s32 ObjectIndex, s32 playerIndex) {
    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 0:
            break;
        case 1:
            KwanmJgmGInit(ObjectIndex, playerIndex);
            break;
        case 2:
            KwanmSetflag(ObjectIndex, 0x00000010);
            KwanmNext(ObjectIndex);
            break;
        case 3:
            KwanmForward(ObjectIndex, 0, 0x0000001F, 1, 2, -1);
            break;
        case 4:
            KwanmFinish(ObjectIndex);
            break;
    }
}

void KwturiageJugemu(s32 PlayerId) {
    s32 ObjectIndex;
    Player* Player;

    ObjectIndex = Jugemuallocptr[PlayerId];
    Player = &PlayerOne[PlayerId];
    if (((CourseID == COURSE_SHERBET_LAND) || (OKLakituIce != 0)) && (Player->JugemuFlag & IS_IN_WATER)) {
        KWAnmStart(ObjectIndex, 7);
        Player->JugemuFlag |= IS_FROZEN;
    } else {
        KWAnmStart(ObjectIndex, 3);
    }
    KwanmSetevflag(ObjectIndex, 1);
}

void KwcheckJugemuTuriage(s32 PlayerId) {
    s32 ObjectIndex;
    Player* Player;

    ObjectIndex = Jugemuallocptr[PlayerId];
    Player = &PlayerOne[PlayerId];
    if ((KwanmBitoffcheckevflag(ObjectIndex, 1) != 0) &&
        (((OoBCheck(Player->Bump.LastZX) != 0) && (Player->Bump.DistanceZX <= 3.0f)) ||
         (Player->JugemuFlag & IS_IN_WATER) ||
         ((Player->BumpStatus == OUT_OF_BOUNDS) && !(Player->SlipFlag & N_JUMP)))) {
        ExplorerSetJugemu(Player);
        KwturiageJugemu(PlayerId);
    }
}

void KwcheckJugemuWarp(s32 ObjectIndex, Player* Player) {
    if (Player->JugemuFlag & IS_FADING_OUT) {
        KwanmSetevflag(ObjectIndex, 2);
        DynamicObjects[ObjectIndex].Alpha = Player->Erase;
        return;
    }
    KwanmClrevflag(ObjectIndex, 2);
}

void KwanmJgmTInit(s32 ObjectIndex, s32 arg1) {
    KwanmJgmBreak(ObjectIndex, arg1);
    KwanmInitCI8(ObjectIndex, (u8*) common_tlut_lakitu_fishing, gTextureLakituFishing1, 0x38U, (u16) 0x00000048);
    DynamicObjects[ObjectIndex].Vtxaddr = D_0D005F30;
    DynamicObjects[ObjectIndex].Scale = 0.15f;
    KwmvStart(ObjectIndex);
    KwanmClrflag(ObjectIndex, 0x00000010);
    KwchartFinish(ObjectIndex);
    KwanmNext(ObjectIndex);
    NaPlyLevelStart((u8) arg1, 0x0100FA28);
}

void KwmvJgmT(s32 ObjectIndex, UNUSED Player* Player) {
    switch (DynamicObjects[ObjectIndex].Mvptr) {
        case 0:
            break;
        case 1:
            DynamicObjects[ObjectIndex].PosStart[2] = 0.0f;
            DynamicObjects[ObjectIndex].PosStart[1] = 0.0f;
            DynamicObjects[ObjectIndex].PosStart[0] = 0.0f;
            DynamicObjects[ObjectIndex].PosSpline[2] = 0.0f;
            DynamicObjects[ObjectIndex].PosSpline[0] = 0.0f;
            DynamicObjects[ObjectIndex].PosSpline[1] = 80.0f;
            KWAnmNext(ObjectIndex);
            break;
        case 2:
            if (KwsubFval(&DynamicObjects[ObjectIndex].PosSpline[1], 5.0f, 1.0f) != 0) {
                KwmvFinish(ObjectIndex);
            }
            break;
        case 3:
            if (KwaddFval(&DynamicObjects[ObjectIndex].PosSpline[1], 100.0f, 1.0f) != 0) {
                KwmvFinish(ObjectIndex);
            }
            break;
    }
}

void KwanmJgmT(s32 ObjectIndex, s32 PlayerId) {
    Player* Player = &PlayerOne[PlayerId];

    switch (DynamicObjects[ObjectIndex].Anmptr) { /* switch 1; irregular */
        case 0:                               /* switch 1 */
            break;
        case 1: /* switch 1 */
            KwanmJgmTInit(ObjectIndex, PlayerId);
            break;
        case 2: /* switch 1 */
            KwanmSetflag(ObjectIndex, 0x00000010);
            KwchartStart(ObjectIndex);
            KwanmNext(ObjectIndex);
            break;
        case 3: /* switch 1 */
            KwanmOufukuF(ObjectIndex, 0, 3, 1, 2, -1);
            break;
    }
    switch (DynamicObjects[ObjectIndex].Stackptr) {
        case 0:
            break;
        case 1:
            if (KwmvCheckfinish(ObjectIndex) != 0) {
                KwchartNext(ObjectIndex);
            }
            break;
        case 2:
            ExplorerCallJugemu(Player);
            KwchartNext(ObjectIndex);
            break;
        case 3:
            if (!(Player->JugemuFlag & ON_LAKITU_ROD)) {
                KwmvStartNormal(ObjectIndex, 0, 3);
                KwchartNext(ObjectIndex);
            }
            break;
        case 4:
            if (KwmvCheckfinish(ObjectIndex) != 0) {
                KwchartNext(ObjectIndex);
            }
            break;
        case 5:
            KwanmClrevflag(ObjectIndex, 1);
            NaPlyLevelStop((u8) PlayerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x28));
            KwanmFinish(ObjectIndex);
            KwchartFinish(ObjectIndex);
            break;
    }
    if (DynamicObjects[ObjectIndex].Anmptr >= 2) {
        KwcheckJugemuWarp(ObjectIndex, Player);
    }
    KwmvJgmT(ObjectIndex, Player);
}

void KWChartIceBlockA(s32 ObjectIndex, s32 PlayerId) {
    Player* Player = &PlayerOne[PlayerId];

    switch (DynamicObjects[ObjectIndex].Anmptr) { /* switch 1; irregular */
        case 0:                               /* switch 1 */
            break;
        case 1: /* switch 1 */
            KwanmJgmTInit(ObjectIndex, PlayerId);
            break;
        case 2: /* switch 1 */
            KwanmSetflag(ObjectIndex, 0x00000010);
            KwchartStart(ObjectIndex);
            Player->JugemuFlag |= HAS_ICE_CUBE;
            KwanmNext(ObjectIndex);
            break;
        case 3: /* switch 1 */
            KwanmOufukuF(ObjectIndex, 0, 3, 1, 2, -1);
            break;
    }
    switch (DynamicObjects[ObjectIndex].Stackptr) {
        case 1:
            if (KwmvCheckfinish(ObjectIndex) != 0) {
                NAPlyTrgStart((u8) PlayerId, 0x1900A055U);
                KwchartNext(ObjectIndex);
            }
            break;
        case 2:
            ExplorerCallJugemu(Player);
            KwanmSetevflag(ObjectIndex, 4);
            KwchartNext(ObjectIndex);
            break;
        case 3:
            if ((Player->BumpStatus == ICE) && !(Player->JugemuFlag & IS_IN_WATER) &&
                ((f64) Player->Bump.DistanceZX <= 30.0)) {
                KwanmSetevflag(ObjectIndex, 8);
            }
            if (!(Player->JugemuFlag & ON_LAKITU_ROD)) {
                KwmvStartNormal(ObjectIndex, 0, 3);
                KwchartNext(ObjectIndex);
            }
            break;
        case 4:
            KwchartWait(ObjectIndex, 0x0000001E);
            break;
        case 5:
            Player->JugemuFlag &= ~HAS_ICE_CUBE;
            KwanmSetevflag(ObjectIndex, 0x00000010);
            KwanmSetevflag(ObjectIndex, 0x00000020);
            KwanmClrevflag(ObjectIndex, 4);
            KwanmClrevflag(ObjectIndex, 8);
            KwchartNext(ObjectIndex);
            NAPlyTrgStart((u8) PlayerId, 0x1900A056U);
            break;
        case 6:
            if (KwchartWait(ObjectIndex, 0x000000A0) != 0) {
                KwanmClrevflag(ObjectIndex, 0x00000010);
                Player->JugemuFlag &= ~IS_FROZEN;
                Player->JugemuFlag |= IS_MELTING;
            }
            break;
        case 7:
            KwchartWait(ObjectIndex, 0x0000003C);
            break;
        case 8:
            KwchartFinish(ObjectIndex);
            KwanmFinish(ObjectIndex);
            Player->JugemuFlag &= ~IS_MELTING;
            KwanmClrevflag(ObjectIndex, 1);
            NaPlyLevelStop((u8) PlayerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x28));
            break;
    }

    if (DynamicObjects[ObjectIndex].Anmptr >= 2) {
        KwcheckJugemuWarp(ObjectIndex, Player);
    }
    KwmvJgmT(ObjectIndex, Player);
}

void KwanmJgmLInit(s32 ObjectIndex, s32 playerIndex) {
    AnmObject* object;

    KwanmJgmBreak(ObjectIndex, playerIndex);
    KwanmInitCI8(ObjectIndex, (u8*) common_tlut_lakitu_second_lap, gTextureLakituSecondLap01, 0x48U,
                        (u16) 0x00000038);
    object = &DynamicObjects[ObjectIndex];
    object->Indexaddr = LakituTexturePtr;
    object->Vtxaddr = common_vtx_also_lakitu;
    object->pos[2] = 5000.0f;
    object->pos[1] = 5000.0f;
    object->pos[0] = 5000.0f;
    object->Scale = 0.15f;
    KwanmClrflag(ObjectIndex, 0x00000010);
    KwmvStartSpline(ObjectIndex, 5, &D_800E694C);
    KwanmNext(ObjectIndex);
}

void KwanmJgmL(s32 ObjectIndex, s32 playerIndex) {
    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 0:
            break;
        case 1:
            KwanmJgmLInit(ObjectIndex, playerIndex);
            break;
        case 2:
            KwanmSetflag(ObjectIndex, 0x00000010);
            KwanmNext(ObjectIndex);
            break;
        case 3:
            KwanmWait(ObjectIndex, 0x00000014);
            break;
        case 4:
            KwanmForward(ObjectIndex, 0, 0x0000000F, 1, 2, 1);
            break;
        case 5:
            KwanmWait(ObjectIndex, 0x0000003C);
            break;
        case 6:
            KwanmReverse(ObjectIndex, 0x0000000F, 0, 1, 2, 1);
            break;
        case 7:
            if (DynamicObjects[ObjectIndex].Mvptr == 0) {
                KwanmFinish(ObjectIndex);
            }
            break;
    }
}

void KwanmJgmFInit(s32 ObjectIndex, s32 playerIndex) {
    AnmObject* object;

    KwanmJgmBreak(ObjectIndex, playerIndex);
    KwanmInitCI8(ObjectIndex, (u8*) common_tlut_lakitu_final_lap, gTextureLakituFinalLap01, 0x48U,
                        (u16) 0x00000038);
    object = &DynamicObjects[ObjectIndex];
    object->Indexaddr = LakituTexturePtr;
    object->Vtxaddr = common_vtx_also_lakitu;
    object->pos[2] = 5000.0f;
    object->pos[1] = 5000.0f;
    object->pos[0] = 5000.0f;
    object->Scale = 0.15f;
    KwanmClrflag(ObjectIndex, 0x00000010);
    KwmvStartSpline(ObjectIndex, 5, &D_800E694C);
    KwanmNext(ObjectIndex);
}

void KwanmJgmF(s32 ObjectIndex, s32 playerIndex) {
    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 0:
            break;
        case 1:
            KwanmJgmFInit(ObjectIndex, playerIndex);
            break;
        case 2:
            KwanmSetflag(ObjectIndex, 0x00000010);
            KwanmNext(ObjectIndex);
            break;
        case 3:
            KwanmWait(ObjectIndex, 0x00000014);
            break;
        case 4:
            KwanmForward(ObjectIndex, 0, 0x0000000F, 1, 2, 1);
            break;
        case 5:
            KwanmWait(ObjectIndex, 0x0000003C);
            break;
        case 6:
            KwanmReverse(ObjectIndex, 0x0000000F, 0, 1, 2, 1);
            break;
        case 7:
            if (DynamicObjects[ObjectIndex].Mvptr == 0) {
                KwanmFinish(ObjectIndex);
            }
            break;
    }
}

void KwanmJgmRInit(s32 ObjectIndex, s32 arg1) {
    f32 var = 5000.0f;
    KwanmJgmBreak(ObjectIndex, arg1);
    KwanmInitCI8(ObjectIndex, (u8*) common_tlut_lakitu_reverse, gTextureLakituReverse01, 0x48U,
                        (u16) 0x00000038);
    DynamicObjects[ObjectIndex].Indexaddr = LakituTexturePtr;
    DynamicObjects[ObjectIndex].Vtxaddr = common_vtx_also_lakitu;
    DynamicObjects[ObjectIndex].pos[2] = var;
    DynamicObjects[ObjectIndex].pos[1] = var;
    DynamicObjects[ObjectIndex].pos[0] = var;
    DynamicObjects[ObjectIndex].Scale = 0.15f;
    KwanmClrflag(ObjectIndex, 0x00000010);
    KwmvStartSpline(ObjectIndex, 6, &D_800E69B0);
    DynamicObjects[ObjectIndex].Stackptr = 0;
    KwanmNext(ObjectIndex);
    NaPlyLevelStart((u8) arg1, 0x0100FA28);
}

void KwanmJgmR(s32 ObjectIndex, s32 PlayerId) {
    Player* sp2C = &PlayerOne[PlayerId];

    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 0:
            break;
        case 1:
            KwanmJgmRInit(ObjectIndex, PlayerId);
            break;
        case 2:
            KwanmSetflag(ObjectIndex, 0x00000010);
            DynamicObjects[ObjectIndex].Stackptr = 1;
            KwanmNext(ObjectIndex);
            break;
        case 3:
            KwanmOufukuF(ObjectIndex, 0, 0x0000000F, 1, 2, -1);
            break;
        case 4:
            KwanmFinish(ObjectIndex);
            break;
    }
    switch (DynamicObjects[ObjectIndex].Stackptr) { /* switch 1; irregular */
        case 1:                                 /* switch 1 */
            if ((DynamicObjects[ObjectIndex].Anmptr >= 3) && (!(sp2C->SlipFlag & REVERSE))) {
                KwmvStartSpline(ObjectIndex, 6, &D_800E69F4);
                DynamicObjects[ObjectIndex].Stackptr = 2;
                DynamicObjects[ObjectIndex].Timer = 0x00000050;
                NaPlyLevelStop((u8) PlayerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x28));
                return;
            }
            return;
        case 2: /* switch 1 */
            DynamicObjects[ObjectIndex].Timer--;
            if (DynamicObjects[ObjectIndex].Timer == 0) {
                KwanmNext(ObjectIndex);
                DynamicObjects[ObjectIndex].Stackptr = 0;
            }
            break;
    }
}

void KwjugemuSetposition1(s32 ObjectIndex, Player* Player, Camera* Camera) {
    u16 temp_t8;

    temp_t8 = 0x8000 - Camera->CameraDirection[1];
    DynamicObjects[ObjectIndex].pos[0] =
        (Player->Position[0] +
         (CosT(temp_t8) * (DynamicObjects[ObjectIndex].PosStart[0] + DynamicObjects[ObjectIndex].PosSpline[0]))) -
        (SinT(temp_t8) * (DynamicObjects[ObjectIndex].PosStart[2] + DynamicObjects[ObjectIndex].PosSpline[2]));
    DynamicObjects[ObjectIndex].pos[1] =
        Player->Ground + DynamicObjects[ObjectIndex].PosStart[1] + DynamicObjects[ObjectIndex].PosSpline[1];
    DynamicObjects[ObjectIndex].pos[2] =
        (Player->Position[2] +
         (SinT(temp_t8) * (DynamicObjects[ObjectIndex].PosStart[0] + DynamicObjects[ObjectIndex].PosSpline[0]))) +
        (CosT(temp_t8) * (DynamicObjects[ObjectIndex].PosStart[2] + DynamicObjects[ObjectIndex].PosSpline[2]));
}

void KwjugemuSetposition2(s32 ObjectIndex, Player* Player, Camera* Camera) {
    u16 temp_t8;

    temp_t8 = 0x8000 - Camera->CameraDirection[1];
    DynamicObjects[ObjectIndex].pos[0] =
        (Player->Position[0] +
         (CosT(temp_t8) * (DynamicObjects[ObjectIndex].PosStart[0] + DynamicObjects[ObjectIndex].PosSpline[0]))) -
        (SinT(temp_t8) * (DynamicObjects[ObjectIndex].PosStart[2] + DynamicObjects[ObjectIndex].PosSpline[2]));
    DynamicObjects[ObjectIndex].pos[1] =
        Player->Position[1] + DynamicObjects[ObjectIndex].PosStart[1] + DynamicObjects[ObjectIndex].PosSpline[1];
    DynamicObjects[ObjectIndex].pos[2] =
        (Player->Position[2] +
         (SinT(temp_t8) * (DynamicObjects[ObjectIndex].PosStart[0] + DynamicObjects[ObjectIndex].PosSpline[0]))) +
        (CosT(temp_t8) * (DynamicObjects[ObjectIndex].PosStart[2] + DynamicObjects[ObjectIndex].PosSpline[2]));
}

UNUSED void KwjugemuSetposition3(void) {
}

void KwcheckJugemuReverse(s32 PlayerId) {
    s32 ObjectIndex;
    Player* Player;

    ObjectIndex = Jugemuallocptr[PlayerId];
    Player = &PlayerOne[PlayerId];

    if ((DynamicObjects[ObjectIndex].Anmptr == 0) && (Player->SlipFlag & REVERSE)) {
        KwreverseJugemu(PlayerId);
    }
}

void KwcheckJugemu(s32 arg0) {
    if (D_801657B4 == 0) {
        KwcheckJugemuReverse(arg0);
    }
    KwcheckJugemuTuriage(arg0);
}

void KwchartJugemuVf(s32 PlayerId) {
    s32 ObjectIndex = Jugemuallocptr[PlayerId];

    switch (DynamicObjects[ObjectIndex].Texsizex) {
        case 0:
            break;
        case 1:
            KwanmJgmS(ObjectIndex, PlayerId);
            KwjugemuStrategy3D(ObjectIndex);
            break;
        case 2:
            KwanmJgmG(ObjectIndex, PlayerId);
            KwjugemuStrategy3D(ObjectIndex);
            break;
        case 3:
            KwanmJgmT(ObjectIndex, PlayerId);
            break;
        case 4:
            KwanmJgmL(ObjectIndex, PlayerId);
            KwjugemuStrategy3D(ObjectIndex);
            break;
        case 5:
            KwanmJgmF(ObjectIndex, PlayerId);
            KwjugemuStrategy3D(ObjectIndex);
            break;
        case 6:
            KwanmJgmR(ObjectIndex, PlayerId);
            KwjugemuStrategy3D(ObjectIndex);
            break;
        case 7:
            KWChartIceBlockA(ObjectIndex, PlayerId);
            break;
    }
}

void KwchartJugemu(s32 PlayerId) {
    s32 ObjectIndex;

    KwcheckJugemu(PlayerId);
    ObjectIndex = Jugemuallocptr[PlayerId];
    LakituTexturePtr = D_80183FA8[PlayerId];
    switch (DynamicObjects[ObjectIndex].Texsizex) {
        case 1:
            KwanmDmajgm(ObjectIndex, PlayerId, 2);
            KwjugemuSetposition1(ObjectIndex, D_8018CF1C, D_8018CF14);
            break;
        case 2:
            KwanmDmajgm(ObjectIndex, PlayerId, 0);
            KwjugemuSetposition1(ObjectIndex, D_8018CF1C, D_8018CF14);
            break;
        case 3:
            KwanmDmajgm(ObjectIndex, PlayerId, 0);
            KwjugemuSetposition2(ObjectIndex, D_8018CF1C, D_8018CF14);
            break;
        case 4:
            KwanmDmajgm(ObjectIndex, PlayerId, 0);
            KwjugemuSetposition1(ObjectIndex, D_8018CF1C, D_8018CF14);
            break;
        case 5:
            KwanmDmajgm(ObjectIndex, PlayerId, 0);
            KwjugemuSetposition1(ObjectIndex, D_8018CF1C, D_8018CF14);
            break;
        case 6:
            KwanmDmajgm(ObjectIndex, PlayerId, 0);
            KwjugemuSetposition1(ObjectIndex, D_8018CF1C, D_8018CF14);
            break;
        case 7:
            KwanmDmajgm(ObjectIndex, PlayerId, 0);
            KwjugemuSetposition2(ObjectIndex, D_8018CF1C, D_8018CF14);
            break;
        case 0:
        default:
            break;
    }
}

void RouletteStart(s32 PlayerId, bool arg1) {
    s32 itemWindow;

    if (PlayerHUD[PlayerId].Goalsw == false) {
        itemWindow = ItemBoxAllocPtr[PlayerId];
        if ((OKForceRoulette != 0) || (KwanmBitoffcheckevflag(itemWindow, 4) != 0)) {
            KWAnmStart(itemWindow, 0);
            if (arg1 != 0) {
                PlayerHUD[PlayerId].Item = arg1;
            }
        }
        NAPlyTrgStart(PlayerId, 0x19008406U);
    }
}

void KwanmUseditem(s32 PlayerId) {
    UNUSED s32 pad;
    Player* Player;
    s32 ObjectIndex;
    ItemWindowObjects* itemWindow;

    Player = &PlayerOne[PlayerId];
    ObjectIndex = ItemBoxAllocPtr[PlayerId];
    itemWindow = (ItemWindowObjects*) &DynamicObjects[ObjectIndex];
    if (itemWindow->Output == ITEM_SUPER_MUSHROOM) {
        if (KwanmBitoffcheckevflag(ObjectIndex, 2) != 0) {
            KwanmSetevflag(ObjectIndex, 2);
            itemWindow->Spstep = 0x0258;
            KwanmJump(ObjectIndex, 0x00000032);
        }
        if (itemWindow->Spstep == 0) {
            KwanmClrevflag(ObjectIndex, 2);
            Player->Item = ITEM_NONE;
            itemWindow->Output = ITEM_NONE;
            KwanmTrig(ObjectIndex, 0);
            KwanmJump(ObjectIndex, 9);
        }
    } else {
        Player->Item = ITEM_NONE;
        itemWindow->Output = ITEM_NONE;
        KwanmTrig(ObjectIndex, 0);
    }
}

//! @todo Cleanup this function to use array access and struct if possible.
u8 KwgetitemSub(s16 Rank, s16 isCpu) {
    u16 rand = MakeRandomLimmit(100);
    u8* Curve;
    u8 randomItem;

    // SRandomItemIndex not initialized for further randomness?
    SRandomItemIndex = ((u32) rand + (SRandomItemIndex + ControllerRandom) + Kwvideoframetimer) % 100U;

    if (GameMode == VERSUS) {
        switch (PlayerCount) {
            case TWO_PLAYERS_SELECTED:
                Curve = SegmentToVirtual((void*) common_versus_2_player_item_curve);
                break;
            case THREE_PLAYERS_SELECTED:
                Curve = SegmentToVirtual((void*) common_versus_3_player_item_curve);
                break;
            case FOUR_PLAYERS_SELECTED:
                Curve = SegmentToVirtual((void*) common_versus_4_player_item_curve);
                break;
        }
        randomItem = *((Rank * 100) + Curve + SRandomItemIndex);

    } else if (GameMode == BATTLE) {
        Curve = SegmentToVirtual((void*) common_battle_item_curve);
        randomItem = Curve[SRandomItemIndex];
    } else { // GP Mode
        if (isCpu == 0) {
            Curve = SegmentToVirtual((void*) common_grand_prix_human_item_curve);
        } else {
            Curve = SegmentToVirtual((void*) common_grand_prix_cpu_item_curve);
        }
        randomItem = *((Rank * 100) + Curve + SRandomItemIndex);
    }

    return randomItem;
}

u8 Kwgetitem(UNUSED s16 arg0, s16 Rank) {
    return KwgetitemSub(Rank, false);
}

u8 KwgetitemEnemy(UNUSED s32 arg0, s16 Rank) {
    return KwgetitemSub(Rank, true);
}

s16 KwanmRandomitem(s32 ObjectIndex, s32 arg1) {
    UNUSED s32 pad[3];
    s16 randomItem;

    randomItem = Kwgetitem(LapCountByPlayerId[arg1], GPCurrentRaceRankByPlayerId[arg1]);

    if (PlayerHUD[arg1].Item != 0) {
        randomItem = (s16) PlayerHUD[arg1].Item;
        PlayerHUD[arg1].Item = 0;
    }

    KwanmSetinput(ObjectIndex, (s32) randomItem);

    return randomItem;
}

s32 KwstealItem(s32 ObjectIndex, s32 PlayerId) {
    UNUSED s16 stackPadding;
    s32 temp_v1;
    s32 var_a3;
    s32 var_t3;
    s32 temp_a0;
    s32 var_v1;
    s32 sp50[4];
    s32 sp40[4];
    s32 var_v1_2;
    Player* sp38;
    s16 temp_a1;

    var_a3 = 0;
    var_t3 = 0;
    if (GameMode == GRAND_PRIX) {
        if (MakeRandomLimmit(0x0064U) < 0x51) {
            var_v1 = Kwgetitem(LapCountByPlayerId[PlayerId], GPCurrentRaceRankByPlayerId[PlayerId]);
        } else {
            var_v1 = 0;
            NAPlyTrgStart(PlayerId, 0x1900A058U);
        }
        var_t3 = 1;
        DynamicObjects[ObjectIndex].Anmnumbak = DynamicObjects[ObjectIndex].Input = var_v1;
    } else {
        for (var_v1_2 = 0; var_v1_2 < PlayerCount; var_v1_2++) {
            temp_a0 = ItemBoxAllocPtr[var_v1_2];
            if (var_v1_2 != PlayerId) {
                if (DynamicObjects[temp_a0].Output != 0) {
                    sp50[var_a3] = var_v1_2;
                    sp40[var_a3] = DynamicObjects[temp_a0].Output;
                    var_a3 += 1;
                }
            }
        }
        if (var_a3 != 0) {
            var_v1 = MakeRandomLimmit(var_a3);
            temp_a1 = sp40[var_v1];
            DynamicObjects[ObjectIndex].Input = temp_a1;
            DynamicObjects[ObjectIndex].Anmnumbak = temp_a1;
            temp_v1 = sp50[var_v1];
            DynamicObjects[ObjectIndex].Anmnum = temp_v1;
            temp_a0 = ItemBoxAllocPtr[temp_v1];
            sp38 = &PlayerOne[temp_v1];
            KwanmSetevflag(temp_a0, 1);
            DynamicObjects[temp_a0].Output = 0;
            sp38->Item = 0;
            if (KwanmBitcheckevflag(temp_a0, 2) != 0) {
                KwanmClrevflag(temp_a0, 2);
                DynamicObjects[temp_a0].Spstep = 0;
            }
            var_t3 = 1;
        }
    }
    return var_t3;
}

void KwanmRouletteInit(s32 ObjectIndex, s32 arg1) {
    s8 test; // why?

    KwanmFinish(ObjectIndex);
    DynamicObjects[ObjectIndex].Anmptr = 2;
    DynamicObjects[ObjectIndex].Output = 0;
    DynamicObjects[ObjectIndex].Stexaddr = (u8*) common_tlut_item_window_none;
    DynamicObjects[ObjectIndex].Sindexaddr = common_texture_item_window_none;
    DynamicObjects[ObjectIndex].Texsizey = 0x28;
    DynamicObjects[ObjectIndex].Fvalptr = 0x20;
    DynamicObjects[ObjectIndex].Timer = 0x00000032;
    DynamicObjects[ObjectIndex].Stackptr = 1;
    DynamicObjects[ObjectIndex].Spstep = 0;
    KwanmSetevflag(ObjectIndex, 4);
    if (D_80165888 != 0) {
        KwanmJump(ObjectIndex, 8);
        DynamicObjects[ObjectIndex].Stackptr = 2;
        DynamicObjects[ObjectIndex].Output = 0x000E;
        test = DynamicObjects[ObjectIndex].Output;
        DynamicObjects[ObjectIndex].Anmnumbak = test;
        DynamicObjects[ObjectIndex].Input = test;
        PlayerHUD[PLAYER_ONE].Addiy = 0;
        D_80165888 = 0;
    } else {
        NaPlyLevelStart(arg1, 0x0100FE1CU);
    }
}

void KwanmRoulette(s32 PlayerId) {
    s32 temp_s0;
    s32 sp40;
    AnmObject* object;
    Player* sp38;
    struct Controller* new_var;

    temp_s0 = ItemBoxAllocPtr[PlayerId];
    sp38 = &PlayerOne[PlayerId];
    sp40 = 0;
    new_var = &ControllerOne[PlayerId];
    if (new_var->ButtonPressed & Z_TRIG) {
        sp40 = 1;
    }
    if (D_80165888 != 0) {
        KWAnmStart(temp_s0, 0);
    }
    object = &DynamicObjects[temp_s0];
    switch (object->Anmptr) {
        case 1:
            KwanmRouletteInit(temp_s0, PlayerId);
            break;
        case 2:
            if (ScreenSplitA == 0) {
                KwaddSval(&PlayerHUD[PlayerId].Addiy, 0x0040, 4);
                if (PlayerHUD[PlayerId].Addiy == 0x0040) {
                    KwanmNext(temp_s0);
                }
            } else if (ScreenSplitA == 3) {
                if ((PlayerId == 0) || (PlayerId == 2)) {
                    KwaddSval(&PlayerHUD[PlayerId].Addix, 0x0080, 8);
                    if (PlayerHUD[PlayerId].Addix == 0x0080) {
                        KwanmNext(temp_s0);
                    }
                } else {
                    KwsubSval(&PlayerHUD[PlayerId].Addix, -0x0080, 8);
                    if (PlayerHUD[PlayerId].Addix == -0x0080) {
                        KwanmNext(temp_s0);
                    }
                }
            } else {
                KwaddSval(&PlayerHUD[PlayerId].Addix, 0x0080, 8);
                if (PlayerHUD[PlayerId].Addix == 0x0080) {
                    KwanmNext(temp_s0);
                }
            }
            break;
        case 3:
            KwanmForward(temp_s0, 1, 0x0000000F, 1, 2, 2);
            break;
        case 4:
            KwanmForward(temp_s0, 1, 6, 1, 8, 1);
            break;
        case 5:
            KwanmForward(temp_s0, 1, 4, 1, 0x00000010, 1);
            break;
        case 6:
            object->Anmnumbak = KwanmRandomitem(temp_s0, PlayerId);
            object->Timer = 8;
            object->Stackptr = 2;
            KwanmNext(temp_s0);
            NaPlyLevelStop((u8) PlayerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFE, 0x1C));
            NaPlyLevelStart((u8) PlayerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFE, 0x47));
            break;
        case 7:
            KwanmSwap(temp_s0, (s32) object->Input, 0, 8, 0x0000000A);
            break;
        case 9:
            KwanmClrevflag(temp_s0, 4);
            KwchartOff(temp_s0);
            object->Anmnumbak = 0;
            KwanmNext(temp_s0);
            break;
        case 10:
            KwanmWait(temp_s0, 0x00000014);
            break;
        case 11:
            if (ScreenSplitA == 0) {
                if (KwsubSval(&PlayerHUD[PlayerId].Addiy, 0, 4) != 0) {
                    KwanmNext(temp_s0);
                }
            } else if (ScreenSplitA == 3) {
                if ((PlayerId == 0) || (PlayerId == 2)) {
                    KwsubSval(&PlayerHUD[PlayerId].Addix, 0, 8);
                    if (PlayerHUD[PlayerId].Addix == 0) {
                        KwanmNext(temp_s0);
                    }
                } else {
                    KwaddSval(&PlayerHUD[PlayerId].Addix, 0, 8);
                    if (PlayerHUD[PlayerId].Addix == 0) {
                        KwanmNext(temp_s0);
                    }
                }
            } else if (KwsubSval(&PlayerHUD[PlayerId].Addix, 0, 8) != 0) {
                KwanmNext(temp_s0);
            }
            break;
        case 12:
            KwanmFinish(temp_s0);
            break;
        case 20:
            if (object->Input == 0x000B) {
                KwanmSwap(temp_s0, object->Input, 0, 8, 0x0000000A);
            } else {
                KwanmSwap(temp_s0, object->Input, 0x0000000B, 8, 0x0000000A);
            }
            break;
        case 21:
            KwanmJump(temp_s0, 8);
            object->Stackptr = 2;
            break;
        case 30:
            if (object->Input == 0x000B) {
                KwanmSwap(temp_s0, object->Input, 0, 8, 0x0000000A);
            } else {
                KwanmSwap(temp_s0, object->Input, 0x0000000B, 8, 0x0000000A);
            }
            break;
        case 31:
            KwanmJump(temp_s0, 9);
            break;
        case 40:
            if (object->Input == 0x000D) {
                KwanmSwap(temp_s0, object->Input, 0x0000000E, 8, 0x0000000A);
            } else {
                KwanmSwap(temp_s0, object->Input, 0x0000000D, 8, 0x0000000A);
            }
            break;
        case 41:
            KwanmJump(temp_s0, 8);
            break;
        case 50:
            KwanmSwap(temp_s0, (s32) object->Input, 0, 8, 0x00000064);
            break;
        case 0:
        default:
            break;
    }
    if (KwanmBitcheckevflag(temp_s0, 2) != 0) {
        if (object->Spstep == 0) {
            KwanmUseditem(PlayerId);
        } else {
            object->Spstep--;
        }
    }
    if (object->Timer >= 0) {
        if (object->Timer > 0) {
            object->Timer--;
        } else {
            switch (object->Stackptr) { /* switch 1; irregular */
                case 1:                /* switch 1 */
                    if (sp40 != 0) {
                        KwchartOff(temp_s0);
                        KwanmJump(temp_s0, 6);
                    }
                    break;
                case 2: /* switch 1 */
                    KwanmSetoutput(temp_s0, (s32) object->Input);
                    object->Stackptr = 3;
                    break;
                case 3: /* switch 1 */
                    if (object->Output == 0) {
                        if (KwanmBitoffcheckevflag(temp_s0, 1) != 0) {
                            if (object->Input == 0x000B) {
                                if (KwstealItem(temp_s0, PlayerId) != 0) {
                                    KwanmJump(temp_s0, 0x00000014);
                                    object->Stackptr = 0;
                                } else {
                                    KwanmJump(temp_s0, 9);
                                }
                            } else if ((object->Input == 0x000D) || (object->Input == 0x000E)) {
                                object->Input--;
                                KwanmSetoutput(temp_s0, (s32) object->Input);
                                object->Stackptr = 3;
                                KwanmJump(temp_s0, 0x00000028);
                            } else {
                                KwanmJump(temp_s0, 9);
                            }
                        } else {
                            KwanmClrevflag(temp_s0, 1);
                            KwanmJump(temp_s0, 0x0000001E);
                            object->Stackptr = 0;
                        }
                    }
                    break;
            }
        }
    }
    object->Texaddr = (u8*) ItemboxtblPAL[object->Anmnumbak];
    object->Indexaddr = ItemboxtblIdx[object->Anmnumbak];
    sp38->Item = object->Output;
}

void KwchartItemboxanm(s32 arg0) {
    KwanmRoulette(arg0);
}

void KwanmPukupukuJump(s32 ObjectIndex) {
    f32 var_f14;
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    switch (object->Anmptr) { /* irregular */
        case 1:
            KwanmInitPoly(ObjectIndex, d_course_banshee_boardwalk_dl_cheep_cheep, 2.0f);
            KwanmSetflag(ObjectIndex, 0x00000010);
            object->Chartptr = 0;
            break;
        case 2:
            if (ScreenFlip != 0) {
                KwmvNanamenage(ObjectIndex, 18.0f, 0.7f, 25.0f, (s16) -0x00005800, 0x0000012C);
            } else {
                KwmvNanamenage(ObjectIndex, 18.0f, 0.7f, 25.0f, (s16) 0x00005800, 0x0000012C);
            }
            if (object->Velocity[2] < 0.0f) {
                var_f14 = -object->Velocity[2];
            } else {
                var_f14 = object->Velocity[2];
            }
            object->ang[0] = KwsetVelyztoangx(object->Velocity[1], var_f14);
            KwanmWait(ObjectIndex, 0x00000046);
            break;
        case 3:
            KwanmFinish(ObjectIndex);
            break;
        case 0:
            break;
    }
}

void KwstartPukupukuJump(s32 PlayerId) {
    s32 ObjectIndex;

    ObjectIndex = Objallocptr2[0];
    if (DynamicObjects[ObjectIndex].Anmptr == 0) {
        if (((s32) NearestPathPointByPlayerId[PlayerId] >= 0xA0) &&
            ((s32) NearestPathPointByPlayerId[PlayerId] < 0xAB)) {
            KwsetInit3DSpos(ObjectIndex, XOrientation * -1650.0, -200.0f, -1650.0f);
            KWAnmStart(ObjectIndex, 1);
        }
    }
}

void KwchartPukupukuJump(void) {
    UNUSED s32 pad;
    s32 ObjectIndex;

    KwstartPukupukuJump(0);
    ObjectIndex = Objallocptr2[0];
    KwanmPukupukuJump(ObjectIndex);
    KwsetSplinepos3D(ObjectIndex);
}

void KwanmPukupukuEndingInit(s32 ObjectIndex) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    object->Chartptr = 1;
    object->Flag = 0;
    object->Shape = d_course_banshee_boardwalk_dl_cheep_cheep;
    object->Scale = 0.2f;
    KwanmNext(ObjectIndex);
    KwsetInit3DSpos(ObjectIndex, D_800E634C[0][0], D_800E634C[0][1] + 55.0, D_800E634C[0][2]);
    KwsetInit3DSppos(ObjectIndex, 0.0f, 30.0f, 0.0f);
    KwsetInit3DAng(ObjectIndex, 0U, 0x3800U, 0U);
}

void KwanmPukupukuEnding(s32 ObjectIndex) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    switch (object->Anmptr) {
        case 1:
            KwanmPukupukuEndingInit(ObjectIndex);
            break;
        case 2:
            if (KwanmWait(ObjectIndex, 0x0000003C) != 0) {
                KwanmSetflag(ObjectIndex, 0x00000010);
                KwmvStart(ObjectIndex);
            }
            break;
        case 3:
            if (object->Mvptr == 0) {
                KwanmNext(ObjectIndex);
            }
            break;
        case 4:
            if (KwanmWait(ObjectIndex, 0x0000000A) != 0) {
                KwmvJump(ObjectIndex, 2);
            }
            break;
        case 5:
            if (object->Mvptr == 0) {
                KwanmFinish(ObjectIndex);
            }
            break;
        case 0:
        default:
            break;
    }
}

void KwmvPukupukuEnding(s32 ObjectIndex) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    switch (object->Mvptr) {
        case 0:
            break;
        case 1:
            object->Velocity[1] = -0.2f;
            if ((f64) object->PosSpline[1] <= 0.0) {
                object->PosSpline[1] = 0.0f;
                object->Velocity[1] = 0.0f;
                KwmvFinish(ObjectIndex);
            }
            break;
        case 2:
            if (KwmvWaitnext(ObjectIndex, 0x00000014) != 0) {
                object->Stack[7] = 0x0040;
            }
            break;
        case 3:
            object->Scale = (f32) ((f64) object->Scale - 0.0015);
            if ((s32) object->ang[0] >= 0xA01) {
                object->Stack[7] -= 4;
            }
            if (KwaddUsval(object->ang, 0x0C00U, (u16) object->Stack[7]) != 0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 4:
            object->Scale = (f32) ((f64) object->Scale - 0.0015);
            object->vel = 0.001f;
            KWAnmNext(ObjectIndex);
            object->Stack[7] = 0;
            break;
        case 5:
            if (object->vel <= 0.004) {
                object->vel += 0.0002;
            }
            object->Scale += object->vel;
            KwaddSval(&object->Stack[7], 0x0100, 0x0010);
            object->ang[0] -= object->Stack[7];
            if (KwmvWait(ObjectIndex, 0x00000035) != 0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 6:
            if (KwmvWait(ObjectIndex, 0x0000000F) != 0) {
                KWAnmNext(ObjectIndex);
                D_801658CE = 1;
            }
            break;
        case 7:
            object->Scale = (f32) ((f64) object->Scale - 0.05);
            if ((f64) object->Scale <= 0.01) {
                KwanmClrflag(ObjectIndex, 0x00000010);
                object->Scale = 0.000001f;
                KWAnmNext(ObjectIndex);
            }
            break;
        case 8:
            KwmvFinish(ObjectIndex);
            break;
    }
    if (object->Mvptr < 0xA) {
        KwsetfvalOufukuF(ObjectIndex, &object->Scale, 0.2f, 0.21f, 0.001f, 0, -1);
    }
    KwmvAddvelytopos(ObjectIndex);
    KwsetSplinepos3D(ObjectIndex);
}

void KwchartPukupukuEnding(void) {
    s32 ObjectIndex;

    ObjectIndex = Objallocptr2[0];
    if (D_801658BC == 1) {
        D_801658BC = 0;
        KWAnmStart(ObjectIndex, 0);
    }
    if (DynamicObjects[ObjectIndex].Anmptr != 0) {
        KwanmPukupukuEnding(ObjectIndex);
        KwmvPukupukuEnding(ObjectIndex);
    }
}

void KwchartPukupuku(s32 arg0) {
    switch (arg0) {
        case 0:
            KwchartPukupukuJump();
            break;
        case 1:
            KwchartPukupukuEnding();
            break;
    }
}

void KwchartTellesa(void) {
    KwchartTellesaMatowari();
}

// Updates the display Status on an object based on its relative Direction to the Camera
void KwtelConvangtoanmnumAbs(s32 ObjectIndex, Camera* Camera) {
    u16 rot = Camera->CameraDirection[1];
    u16 temp = ((u16) (DynamicObjects[ObjectIndex].ang[1] - rot + DEGREES(180)) * 0x24) / 0x10000;

    if (temp < 0x13) {
        KwanmClrflag(ObjectIndex, 0x80);
        DynamicObjects[ObjectIndex].Anmnumbak = temp;
    } else {
        KwanmSetflag(ObjectIndex, 0x80);
        DynamicObjects[ObjectIndex].Anmnumbak = 0x24 - temp;
    }
}

void KwtelLookplayerAbs(s32 ObjectIndex, Player* Player, Camera* Camera) {
    f32 x;
    f32 z;

    x = Player->Position[0] - DynamicObjects[ObjectIndex].pos[0];
    z = Player->Position[2] - DynamicObjects[ObjectIndex].pos[2];
    DynamicObjects[ObjectIndex].ang[1] =
        KwchaseAngle(DynamicObjects[ObjectIndex].ang[1], Atan2t(x, z));
    KwtelConvangtoanmnumAbs(ObjectIndex, Camera);
}

UNUSED void KwtelChaseangtoanmnumAbs(void) {
}

void KwtelConvangtoanmnumRel(s32 ObjectIndex) {
    u16 var_t9;

    var_t9 = DynamicObjects[ObjectIndex].ang[1] * 0x24 / 0x10000;

    if (var_t9 < 0x13) {
        KwanmClrflag(ObjectIndex, 0x80);
        DynamicObjects[ObjectIndex].Anmnumbak = var_t9;
    } else {
        KwanmSetflag(ObjectIndex, 0x80);
        DynamicObjects[ObjectIndex].Anmnumbak = 0x24 - var_t9;
    }
}

void KwtelChaseangtoanmnumRel(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].ang[1] =
        KwchaseAngle(DynamicObjects[ObjectIndex].ang[1],
                      Atan2t(DynamicObjects[ObjectIndex].Velocity[0], DynamicObjects[ObjectIndex].Velocity[2]));
    KwtelConvangtoanmnumRel(ObjectIndex);
}

void KwanmTellesaMatowariInit(s32 ObjectIndex) {
    AnmObject* object;

    KwanmInitCI8(ObjectIndex, d_course_banshee_boardwalk_boo_tlut, D_80165880, 48, 40);
    object = &DynamicObjects[ObjectIndex];
    object->pos[0] = 0.0f;
    object->pos[1] = 0.0f;
    object->pos[2] = 0.0f;
    KwanmSetflag(ObjectIndex, 0x00000020);
    KwanmNext(ObjectIndex);
    object->Alpha = 0;
    KwsetsvalStart(ObjectIndex);
    object->Scale = 0.15f;
    object->vel = 1.0f;
    KwsetfvalStart(ObjectIndex);
    KwmvStartStrategy(ObjectIndex);
    KwanmSetflag(ObjectIndex, 0x00000800);
    object->Texang[0] = 0;
    object->Texang[2] = 0x8000;
}

void KwanmTellesaMatowari(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 0:
            break;
        case 1:
            KwanmTellesaMatowariInit(ObjectIndex);
            break;
    }
    if (DynamicObjects[ObjectIndex].Anmptr >= 2) {
        switch (DynamicObjects[ObjectIndex].Mvtype) {
            case 0:
                break;
            case 1:
                KwsetsvalForward(ObjectIndex, &DynamicObjects[ObjectIndex].Alpha, 0, 0x00000050, 2, 1, 0);
                break;
            case 2:
                KwsetsvalOufukuF(ObjectIndex, &DynamicObjects[ObjectIndex].Alpha, 0x00000050, 0x00000078, 1, 0, -1);
                break;
            case 3:
                KwsetsvalSubchase(ObjectIndex, &DynamicObjects[ObjectIndex].Alpha, 0, 2);
                break;
            case 4:
                KwanmFinish(ObjectIndex);
                KwmvFinish(ObjectIndex);
                KwsetsvalFinish(ObjectIndex);
                break;
        }
        KwanmTexaddrCI8Index(ObjectIndex);
    }
}

void KwonTellesa(s32 someIndex, s32 playerIndex) {
    s32 temp_a0;
    s32 ObjectIndex;
    s16 temp_s1_2;
    s16 temp_s4;
    s16 temp_s5;
    SplineData* something;

    for (temp_a0 = 0; temp_a0 < 5; temp_a0++) {
        ObjectIndex = Objallocptr3[someIndex + temp_a0];
        KWAnmStart(ObjectIndex, 1);
        DynamicObjects[ObjectIndex].Anmnum = playerIndex;
        temp_s1_2 = MakeRandomLimmit(0x003CU) - 0x1E;
        temp_s4 = MakeRandomLimmit(0x0014U) - 0xA;
        temp_s5 = MakeRandomLimmit(0x0050U) - 0x28;
        MakeRandomLimmit(0x1000U);
        something = D_800E5D9C[temp_a0];
        DynamicObjects[ObjectIndex].Sptbtopaddr = something;
        DynamicObjects[ObjectIndex].PosStart[0] = (f32) temp_s1_2;
        DynamicObjects[ObjectIndex].PosStart[1] = (f32) temp_s4;
        DynamicObjects[ObjectIndex].PosStart[2] = (f32) temp_s5;
    }
    NAPlyTrgStart(playerIndex, 0x1900705AU);

    if (someIndex == 0) {
        D_8018CFF0 = 1;
    } else {
        D_8018D048 = 1;
    }
}

void KwoffTellesa(s32 someIndex) {
    s32 temp_a0;
    s32 ObjectIndex;

    for (temp_a0 = 0; temp_a0 < 5; temp_a0++) {
        ObjectIndex = Objallocptr3[someIndex + temp_a0];
        DynamicObjects[ObjectIndex].Mvtype += 1;
    }

    if (someIndex == 0) {
        D_8018CFF0 = 0;
    } else {
        D_8018D048 = 0;
    }
}

s32 KwcheckMatowariPlayer(void) {
    s32 ret;
    s32 first, second;
    if (MenuMultiplayerSelection == 1) {
        ret = 0;
    } else {
        if (MenuMultiplayerSelection == 2) {
            if (GameMode == 0) {
                first = GPCurrentRaceRankByPlayerId[0];
                second = GPCurrentRaceRankByPlayerId[1];
                if (first < second) {
                    ret = 0;
                } else {
                    ret = 1;
                }
            } else {
                ret = RankToNum[0];
            }
        } else {
            ret = RankToNum[0];
        }
    }
    return ret;
}

void KwcheckMatowari(void) {
    s32 PlayerId;
    // wtf is up with this Pointer? Its such a dumb way of going about things
    u16* test;

    if (D_8018CFF0 == 0) {
        PlayerId = KwcheckMatowariPlayer();
        D_8018D018 = PlayerId;
        test = &NearestPathPointByPlayerId[PlayerId];
        if ((*test >= 0xC9) && (*test < 0xD2)) {
            KwonTellesa(0, (s32) PlayerId);
        }
    }
    if (D_8018CFF0 != 0) {
        PlayerId = D_8018D018;
        test = &NearestPathPointByPlayerId[PlayerId];
        if ((*test >= 0xB5) && (*test < 0xBE)) {
            KwoffTellesa(0);
        }
        if ((*test >= 0x119) && (*test < 0x122)) {
            KwoffTellesa(0);
        }
    }
    if (D_8018D048 == 0) {
        PlayerId = KwcheckMatowariPlayer();
        D_8018D110 = PlayerId;
        test = &NearestPathPointByPlayerId[PlayerId];
        if ((*test >= 0x1FF) && (*test < 0x208)) {
            KwonTellesa(5, (s32) PlayerId);
        }
    }
    if (D_8018D048 != 0) {
        PlayerId = D_8018D110;
        test = &NearestPathPointByPlayerId[PlayerId];
        if ((*test >= 0x1EB) && (*test < 0x1F4)) {
            KwoffTellesa(5);
        }
        if ((*test >= 0x26D) && (*test < 0x276)) {
            KwoffTellesa(5);
        }
    }
}

void KwchartTellesaMatowari(void) {
    u16 temp_t4;
    s32 someIndex;
    s32 ObjectIndex;
    Player* Player;
    Camera* Camera;
    AnmObject* object;

    KwcheckMatowari();
    for (someIndex = 0; someIndex < NUM_BOOS; someIndex++) {
        ObjectIndex = Objallocptr3[someIndex];
        object = &DynamicObjects[ObjectIndex];
        if (object->Anmptr != 0) {
            KwanmTellesaMatowari(ObjectIndex);
            KwspStrategy3DLoop(ObjectIndex);
            Player = &PlayerOne[object->Anmnum];
            Camera = &Camera1[object->Anmnum];
            temp_t4 = (0x8000 - Camera->CameraDirection[1]);
            object->pos[0] = Player->Position[0] + (CosT(temp_t4) * (object->PosStart[0] + object->PosSpline[0])) -
                             (SinT(temp_t4) * (object->PosStart[2] + object->PosSpline[2]));
            object->pos[1] = 6.5 + Player->Ground + object->PosStart[1] + object->PosSpline[1];
            object->pos[2] = Player->Position[2] + (SinT(temp_t4) * (object->PosStart[0] + object->PosSpline[0])) +
                             (CosT(temp_t4) * (object->PosStart[2] + object->PosSpline[2]));
            KwtelChaseangtoanmnumRel(ObjectIndex);
            if (KwanmBitcheckflag(ObjectIndex, 0x00000080) != 0) {
                object->Vtxaddr = DvtxTellesa48X40Xflip;
            } else {
                object->Vtxaddr = DvtxTellesa48X40;
            }
        }
    }
}

void KwanmTellesaInit(s32 ObjectIndex) {
    AnmObject* object;

    KwanmInitCI8(ObjectIndex, d_course_banshee_boardwalk_boo_tlut, gTextureGhosts, 0x30U, (u16) 0x00000028);
    object = &DynamicObjects[ObjectIndex];
    object->Anmnumbak = 0x1C;
    object->pos[0] = 0.0f;
    object->pos[1] = 0.0f;
    object->pos[2] = 0.0f;
    object->Scale = 0.15f;
    KwanmNext(ObjectIndex);
    object->Alpha = 0;
    KwsetsvalStart(ObjectIndex);
    KwmvStartStrategy(ObjectIndex);
    object->ang[2] = 0x8000;
    object->ang[1] =
        Atan2t(D_8018CF1C->Position[0] - object->PosStart[0], D_8018CF1C->Position[2] - object->PosStart[2]);
}

void KwanmTellesa(s32 ObjectIndex) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    switch (object->Anmptr) { /* switch 1 */
        case 0:              /* switch 1 */
            break;
        case 1: /* switch 1 */
            KwanmTellesaInit(ObjectIndex);
            break;
        case 2: /* switch 1 */
            KwanmWait(ObjectIndex, 0x0000001E);
            break;
        case 3: /* switch 1 */
            KwanmWait(ObjectIndex, 0x000003E8);
            break;
        case 4: /* switch 1 */
            KwanmFinish(ObjectIndex);
            break;
        default: /* switch 2 */
            break;
    }
    if (object->Anmptr >= 2) {
        KwanmIfequjump(ObjectIndex, (s32) object->Mvtype, 0, 4);
        KwanmTexaddrCI8Index(ObjectIndex);
        switch (object->Mvtype) { /* switch 2 */
            case 1:                /* switch 2 */
                KwsetsvalForward(ObjectIndex, &object->Alpha, 0x00000028, 0x00000050, 4, 0, 0);
                break;
            case 2: /* switch 2 */
                KwsetsvalOufukuF(ObjectIndex, &object->Alpha, 0x00000050, 0x000000B4, 2, 0, -1);
                if ((object->Mvptr == 0) || (object->Anmptr == 3)) {
                    object->Mvtype += 1;
                }
                break;
            case 3: /* switch 2 */
                KwsetsvalSubchase(ObjectIndex, &object->Alpha, 0, 4);
                break;
            case 4: /* switch 2 */
                KwsetsvalFinish(ObjectIndex);
                break;
            case 0:
            default: /* switch 2 */
                break;
        }
    }
}

void KwchartTellesaRandomborn(void) {
    UNUSED s32 stackPadding;
    UNUSED s32 stackPadding2;
    u16 temp_s3;
    s16 temp_t5;
    s16 temp_s2;
    u16 temp_t2;
    s32 var_v0;
    s32 ObjectIndex;

    var_v0 = 0;
    if ((KW2GFTimer == 0) && (D_8018D2A4 != 0) && (D_8018CF68[0] < 0x1D) && (D_800E5DB4[D_8018CF68[0]] == 1)) {
        while (DynamicObjects[EffectAllocArray1[var_v0 + 10]].Anmptr != 0) {
            var_v0++;
            if (var_v0 == 30) {
                break;
            }
        }
        ObjectIndex = EffectAllocArray1[var_v0 + 10];
        if (var_v0 != 30) {
            if (DynamicObjects[ObjectIndex].Anmptr == 0) {
                KWAnmStart(ObjectIndex, 1);
                temp_s2 = MakeRandomLimmit(0x012CU);
                temp_s3 = MakeRandomLimmit(0x1000U) - 0x800;
                temp_t5 = MakeRandomLimmit(0x000FU) - 5;
                DynamicObjects[ObjectIndex].ang[1] = D_8018CF1C->Direction[1] + DEGREES(180);
                temp_t2 = (D_8018CF14->CameraDirection[1] + temp_s3);
                DynamicObjects[ObjectIndex].PosStart[0] = D_8018CF1C->Position[0] + (SinT(temp_t2) * temp_s2);
                DynamicObjects[ObjectIndex].PosStart[1] = temp_t5;
                DynamicObjects[ObjectIndex].PosStart[2] = D_8018CF1C->Position[2] + (CosT(temp_t2) * temp_s2);
                DynamicObjects[ObjectIndex].Sptbtopaddr = &D_800E5D54;
            }
        }
    }
    for (var_v0 = 0; var_v0 < 30; var_v0++) {
        ObjectIndex = EffectAllocArray1[var_v0 + 10];
        if (DynamicObjects[ObjectIndex].Anmptr != 0) {
            KwanmTellesa(ObjectIndex);
            KwspStrategy3D(ObjectIndex);
            DynamicObjects[ObjectIndex].pos[0] =
                DynamicObjects[ObjectIndex].PosStart[0] + DynamicObjects[ObjectIndex].PosSpline[0];
            DynamicObjects[ObjectIndex].pos[1] =
                D_8018CF1C->Ground + DynamicObjects[ObjectIndex].PosStart[1] + DynamicObjects[ObjectIndex].PosSpline[1];
            DynamicObjects[ObjectIndex].pos[2] =
                DynamicObjects[ObjectIndex].PosStart[2] + DynamicObjects[ObjectIndex].PosSpline[2];
            KwtelLookplayerAbs(ObjectIndex, D_8018CF1C, D_8018CF14);
            if (KwanmBitcheckflag(ObjectIndex, 0x00000080) != 0) {
                DynamicObjects[ObjectIndex].Vtxaddr = DvtxTellesa48X40Xflip;
            } else {
                DynamicObjects[ObjectIndex].Vtxaddr = DvtxTellesa48X40;
            }
        }
    }
}

void KwanmBatRandomposInit(s32 ObjectIndex, s32 arg1) {
    if (arg1 == 1) {
        DynamicObjects[ObjectIndex].PosStart[0] = (f32) ((-1775.0 - MakeRandomLimmit(0x001EU)) * (f64) XOrientation);
        DynamicObjects[ObjectIndex].PosStart[1] = (f32) (MakeRandomLimmit(0x0019U) + 25.0);
        DynamicObjects[ObjectIndex].PosStart[2] = (f32) (MakeRandomLimmit(0x001EU) + 130.0);
        DynamicObjects[ObjectIndex].PosEnde[0] = (f32) ((f64) XOrientation * -2500.0);
        DynamicObjects[ObjectIndex].PosEnde[1] = 0.0f;
        DynamicObjects[ObjectIndex].PosEnde[2] = (f32) (220.0 - MakeRandomLimmit(0x0096U));
        DynamicObjects[ObjectIndex].ang[0] = 0xDC00;
        DynamicObjects[ObjectIndex].Destangy = 0x0800;
    }
    if (arg1 == 2) {
        DynamicObjects[ObjectIndex].PosStart[0] = (f32) (-0x55B - MakeRandomLimmit(0x001EU)) * XOrientation;
        DynamicObjects[ObjectIndex].PosStart[1] = (f32) (MakeRandomLimmit(0x0019U) + 0xF);
        DynamicObjects[ObjectIndex].PosStart[2] = (f32) (MakeRandomLimmit(0x001EU) - 0xE8);
        DynamicObjects[ObjectIndex].PosEnde[0] = (f32) ((f64) XOrientation * -2100.0);
        DynamicObjects[ObjectIndex].PosEnde[1] = 0.0f;
        DynamicObjects[ObjectIndex].PosEnde[2] = (f32) (MakeRandomLimmit(0x00C8U) + -290.0);
        DynamicObjects[ObjectIndex].ang[0] = 0;
        DynamicObjects[ObjectIndex].Destangy = 0;
    }
    DynamicObjects[ObjectIndex].ang[1] =
        KwcalcDestangley(DynamicObjects[ObjectIndex].PosStart[0], DynamicObjects[ObjectIndex].PosEnde[0],
                             DynamicObjects[ObjectIndex].PosStart[2], DynamicObjects[ObjectIndex].PosEnde[2]);
    DynamicObjects[ObjectIndex].ang[2] = 0;
    KwsetInit3DSppos(ObjectIndex, 0.0f, 0.0f, 0.0f);
    DynamicObjects[ObjectIndex].Mvtm = 0;
    if (RaceClass < CC_150) {
        DynamicObjects[ObjectIndex].vel = (MakeRandomLimmit(4U) + 4.0);
    } else {
        DynamicObjects[ObjectIndex].vel = (MakeRandomLimmit(4U) + 5.0);
    }
}

void KwinitBat(s32 ObjectIndex, s32 arg1) {
    UNUSED s32 pad[2];
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    object->Chartptr = 0x0D;
    KwanmBatRandomposInit(ObjectIndex, arg1);
    object->Scale = 0.1f;
    KwanmSetflag(ObjectIndex, 0x00000200);
    object->r = 3;
}

void KwgetBat(s32 arg0) {
    s32 ObjectIndex;

    if (arg0 == 1) {
        ObjectIndex = KwallocEffectram(EffectAllocArray2, &NextFreeObjectParticle2, 0x28);
    } else {
        ObjectIndex = KwallocEffectram(EffectAllocArray3, &NextFreeObjectParticle3, 0x1E);
    }
    if (ObjectIndex != NULL_OBJECT_ID) {
        KWAnmStart(ObjectIndex, 0);
        KwinitBat(ObjectIndex, arg0);
    }
}

void KwhitcheckBat(s32 ObjectIndex) {
    if (RaceClass == CC_50) {
        KwcheckReboundPlayer3D(ObjectIndex, 0.5f, 0.2f);
    } else if (RaceClass == CC_100) {
        KwcheckReboundPlayer3D(ObjectIndex, 0.4f, 0.15f);
    } else {
        KwcheckReboundPlayer3D(ObjectIndex, 0.25f, 0.1f);
    }
}

s32 KwcheckVisibleBat(s32 ObjectIndex) {
    s32 loopIndex;
    s32 someCount;

    someCount = 0;
    for (loopIndex = 0; loopIndex < PlayerCount; loopIndex++) {
        if (KwanmVischeckCameraXz(ObjectIndex, &Camera1[loopIndex], 0x4000U) != 0) {
            someCount += 1;
        }
    }
    return someCount;
}

void KwanmBatInit(s32 arg0) {
    KwanmNext(arg0);
    KwmvStart(arg0);
}

void KwanmBat(s32 ObjectIndex, s32 arg1) {
    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 1:
            KwanmBatInit(ObjectIndex);
            break;
        case 2:
            if (KwcheckVisibleBat(ObjectIndex) == 0) {
                KwanmFinish(ObjectIndex);
            }
            if (arg1 == 1) {
                if (ScreenFlip != 0) {
                    if (DynamicObjects[ObjectIndex].pos[0] >= 2540.0) {
                        KwanmFinish(ObjectIndex);
                    }
                } else if (DynamicObjects[ObjectIndex].pos[0] <= -2540.0) {
                    KwanmFinish(ObjectIndex);
                }
            } else if (ScreenFlip != 0) {
                if (DynamicObjects[ObjectIndex].pos[0] >= 2150.0) {
                    KwanmFinish(ObjectIndex);
                }
            } else {
                if (DynamicObjects[ObjectIndex].pos[0] <= -2150.0) {
                    KwanmFinish(ObjectIndex);
                }
            }
            break;
        case 0:
            break;
    }
}

void KwmvBatFireplace(s32 ObjectIndex) {
    KwmvVelangytoposxz(ObjectIndex);
    KwsetSplinepos3D(ObjectIndex);
}

void KwmvBatKanoke(s32 ObjectIndex) {
    UNUSED s32 pad;
    if ((DynamicObjects[ObjectIndex].Mvptr != 0) && (DynamicObjects[ObjectIndex].Mvptr == 1)) {
        if (KwmvWait(ObjectIndex, 0x0000001E) != 0) {
            DynamicObjects[ObjectIndex].Destangy = 0U;
        }
    }
    DynamicObjects[ObjectIndex].ang[0] =
        KwchaseAngle(DynamicObjects[ObjectIndex].ang[0], DynamicObjects[ObjectIndex].Destangy);
    KwmvVelangxytoposxyz(ObjectIndex);
    KwsetSplinepos3D(ObjectIndex);
}

void KwmvBat(s32 ObjectIndex, s32 arg1) {
    switch (arg1) { /* irregular */
        case 1:
            KwmvBatKanoke(ObjectIndex);
            return;
        case 2:
            KwmvBatFireplace(ObjectIndex);
            return;
    }
}

void KwchartBat(void) {
    s32 var_s2;
    s32 temp_s0;
    AnmObject* object;

    if (D_8018CFC8 != 0) {
        D_8018CFC8 -= 1;
    }
    if (D_8018D000 != 0) {
        D_8018D000 -= 1;
    }
    temp_s0 = Objallocptr[0];
    KwanmForward(temp_s0, 0, 3, 1, 0, -1);
    KwanmTexaddrCI8Index(temp_s0);
    object = &DynamicObjects[temp_s0];
    KwsetsvalOufukuF(temp_s0, &object->Alpha, -0x00001000, 0x00001000, 0x00000400, 0, -1);
    object->Texang[2] = object->Alpha + 0x8000;
    if ((D_8018CFB0 != 0) || (D_8018CFC8 != 0)) {
        D_8018CFD8 = 0;
        for (var_s2 = 0; var_s2 < 40; var_s2++) {
            temp_s0 = EffectAllocArray2[var_s2];
            if (temp_s0 == -1) {
                continue;
            }

            object = &DynamicObjects[temp_s0];
            if (object->Anmptr == 0) {
                continue;
            }

            KwanmBat(temp_s0, 1);
            KwmvBat(temp_s0, 1);
            KwhitcheckBat(temp_s0);
            if (object->Anmptr == 0) {
                KwdeallocEffectram(&EffectAllocArray2[var_s2]);
            }
            D_8018CFD8 += 1;
        }
        if (D_8018CFD8 != 0) {
            D_8018CFC8 = 0x012C;
        }
    }
    if ((D_8018CFE8 != 0) || (D_8018D000 != 0)) {
        D_8018D010 = 0;
        for (var_s2 = 0; var_s2 < 30; var_s2++) {
            temp_s0 = EffectAllocArray3[var_s2];
            if (temp_s0 == -1) {
                continue;
            }

            object = &DynamicObjects[temp_s0];
            if (object->Anmptr == 0) {
                continue;
            }

            KwanmBat(temp_s0, 2);
            KwmvBat(temp_s0, 2);
            KwhitcheckBat(temp_s0);
            if (object->Anmptr == 0) {
                KwdeallocEffectram(&EffectAllocArray3[var_s2]);
            }
            D_8018D010 += 1;
        }
        if (D_8018D010 != 0) {
            D_8018D000 = 0x012C;
        }
    }
}

void KwcheckAppearKanokeBat(s32 ObjectIndex) {
    f32 sp2C;
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    if (object->Timer > 0) {
        object->Timer--;
        if (object->Timer == 0) {
            KwanmClrevflag(ObjectIndex, 1);
        }
    }
    if (object->Ival > 0) {
        object->Ival--;
        if (object->Ival == 0) {
            NaSceneLevelStop(object->pos, SOUND_ARG_LOAD(0x51, 0x02, 0x80, 0x06));
        }
    }
    if (object->Timer == 0) {
        if ((RaceClass == CC_50) || (RaceClass == CC_100) || (RaceClass == CC_150) ||
            (RaceClass == CC_EXTRA)) {
            sp2C = 1150.0f;
        }
        KwcheckVisibleAreaCamera(ObjectIndex, sp2C);
        if ((KwanmBitcheckflag(ObjectIndex, VISIBLE) != 0) && (KwanmBitoffcheckevflag(ObjectIndex, 1) != 0)) {
            KwanmSetevflag(ObjectIndex, 1);
            NaSceneLevelStart(object->pos, object->Velocity, SOUND_ARG_LOAD(0x51, 0x02, 0x80, 0x06));
            KwanmJump(ObjectIndex, 3);
            if (object->Output > 0) {
                object->Output--;
                object->Timer = 0x00000168;
            } else {
                object->Timer = 0x00000168;
            }
            object->Ival = 0x0000012C;
        }
    }
    if (KwcheckMapareain(0x000F, 0x0012) == 0) {
        object->Output = 2;
    }
}

void KwanmKanokeInit(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].Scale = 1.0f;
    DynamicObjects[ObjectIndex].Shape = d_course_banshee_boardwalk_dl_trash_bin;
    DynamicObjects[ObjectIndex].Timer = 0;
    DynamicObjects[ObjectIndex].Stack[7] = 0;
    KwsetInit3DTexang(ObjectIndex, 0U, 0U, 0U);
    if (ScreenFlip != 0) {
        DynamicObjects[ObjectIndex].pos[0] = 1765.0f;
        DynamicObjects[ObjectIndex].pos[2] = 195.0f;
        DynamicObjects[ObjectIndex].Texang[1] = 0x8000;
    } else {
        DynamicObjects[ObjectIndex].pos[0] = -1765.0f;
        DynamicObjects[ObjectIndex].pos[2] = 70.0f;
    }
    DynamicObjects[ObjectIndex].pos[1] = 45.0f;
    KwsetInit3DVel(ObjectIndex, 0.0f, 0.0f, 0.0f);
    DynamicObjects[ObjectIndex].Output = 0;
    KwanmNext(ObjectIndex);
}

void KwanmKanoke(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 1:
            KwanmKanokeInit(ObjectIndex);
            break;
        case 3:
            D_8018CFB0 = 1;
            KwanmNext(ObjectIndex);
            break;
        case 4:
            KwanmWait(ObjectIndex, 0x000000D2);
            if (KW8GFTimer == 0) {
                if (RaceClass < CC_150) {
                    KwgetBat(1);
                    KwgetBat(1);
                } else {
                    KwgetBat(1);
                    KwgetBat(1);
                    KwgetBat(1);
                    KwgetBat(1);
                }
            }
            KwsetsvalOufukuF(ObjectIndex, &DynamicObjects[ObjectIndex].Alpha, -0x00002000, 0, 0x00000400, 0, -1);
            DynamicObjects[ObjectIndex].Texang[2] = DynamicObjects[ObjectIndex].Alpha;
            if (DynamicObjects[ObjectIndex].Stack[7] == 0) {
                NAEnmTrgStart(DynamicObjects[ObjectIndex].pos, DynamicObjects[ObjectIndex].Velocity,
                              SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x4E));
                DynamicObjects[ObjectIndex].Stack[7] = 0x0014;
            } else {
                DynamicObjects[ObjectIndex].Stack[7]--;
            }
            break;
        case 5:
            DynamicObjects[ObjectIndex].Texang[2] = KwchaseAngle(DynamicObjects[ObjectIndex].Texang[2], 0U);
            if (DynamicObjects[ObjectIndex].Texang[2] == 0) {
                KwanmNext(ObjectIndex);
            }
            break;
        case 6:
            DynamicObjects[ObjectIndex].Texang[2] = 0;
            DynamicObjects[ObjectIndex].Stack[7] = 0;
            KwanmNext(ObjectIndex);
            D_8018CFB0 = 0;
            break;
        case 0:
        case 2:
        default:
            break;
    }
}

void KwchartKanoke(void) {
    s32 ObjectIndex = Objallocptr[1];
    KwanmKanoke(ObjectIndex);
    if (GameMode != TIME_TRIALS) {
        KwcheckAppearKanokeBat(ObjectIndex);
    }
}

void KwcheckAppearDanroBat(s32 ObjectIndex) {
    f32 sp2C;
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    if (object->Timer > 0) {
        object->Timer--;
        if (object->Timer == 0) {
            KwanmClrevflag(ObjectIndex, 1);
        }
    }
    if (object->Ival > 0) {
        object->Ival--;
        if (object->Ival == 0) {
            NaSceneLevelStop(object->pos, SOUND_ARG_LOAD(0x51, 0x02, 0x80, 0x06));
        }
    }
    if (object->Timer == 0) {
        if ((RaceClass == CC_50) || (RaceClass == CC_100) || (RaceClass == CC_150) ||
            (RaceClass == CC_EXTRA)) {
            sp2C = 700.0f;
        }
        KwcheckVisibleAreaCamera(ObjectIndex, sp2C);
        if ((KwanmBitcheckflag(ObjectIndex, VISIBLE) != 0) && (KwanmBitoffcheckevflag(ObjectIndex, 1) != 0)) {
            KwanmSetevflag(ObjectIndex, 1);
            NaSceneLevelStart(object->pos, object->Velocity, SOUND_ARG_LOAD(0x51, 0x02, 0x80, 0x06));
            KwanmJump(ObjectIndex, 3);
            if (object->Output > 0) {
                object->Output--;
                object->Timer = 0x00000168;
            } else {
                object->Timer = 0x00000168;
            }
            object->Ival = 0x0000012C;
        }
    }
    if (KwcheckMapareain(0x000F, 0x0013) == 0) {
        object->Output = 2;
    }
}

void KwanmDanroInit(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].pos[0] = -1371.0f * XOrientation;
    DynamicObjects[ObjectIndex].pos[1] = 31.0f;
    DynamicObjects[ObjectIndex].pos[2] = -217.0f;
    KwsetInit3DVel(ObjectIndex, 0.0f, 0.0f, 0.0f);
    DynamicObjects[ObjectIndex].Output = 0;
    DynamicObjects[ObjectIndex].Timer = 0;
    DynamicObjects[ObjectIndex].Stack[7] = 0;
    KwanmNext(ObjectIndex);
}

void KwanmDanro(s32 ObjectIndex) {

    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 1:
            KwanmDanroInit(ObjectIndex);
            break;
        case 3:
            D_8018CFE8 = 1;
            KwanmNext(ObjectIndex);
            break;
        case 4:
            KwanmWait(ObjectIndex, 0x000000D2);
            if (KW4GFTimer == 0) {
                if (RaceClass < CC_150) {
                    KwgetBat(2);
                } else {
                    KwgetBat(2);
                    KwgetBat(2);
                }
            }
            break;
        case 5:
            D_8018CFE8 = 0;
            KwanmNext(ObjectIndex);
            break;
        case 0:
        case 2:
        default:
            break;
    }
}

void KwchartDanro(void) {
    s32 ObjectIndex = Objallocptr[2];
    KwanmDanro(ObjectIndex);
    if (GameMode != TIME_TRIALS) {
        KwcheckAppearDanroBat(ObjectIndex);
    }
}

s32 KwcheckDoshinTurnSub(s32 ObjectIndex, Player* Player, Camera* Camera) {
    s32 sp24;

    sp24 = 0;
    if ((KwanmBitoffcheckevflag(ObjectIndex, 4) != 0) &&
        (KwanmCheckradiusPlayerXz(ObjectIndex, Player, 300.0f) != 0) &&
        (KwanmVischeckPlayerXz(ObjectIndex, Player, Camera, 0x4000U) != 0) &&
        (KwanmCheckobjangCameraXz(ObjectIndex, Camera, 0x1555U) != 0)) {
        KwanmSetevflag(ObjectIndex, 4);
        sp24 = 1;
    }
    return sp24;
}

s32 KwcheckDoshinTurn(s32 ObjectIndex) {
    Camera* Camera;
    Player* Player;
    s32 temp_v0;
    s32 someIndex;

    temp_v0 = 0;
    Player = PlayerOne;
    Camera = Camera1;
    for (someIndex = 0; someIndex < PlayerCount; someIndex++) {
        temp_v0 = KwcheckDoshinTurnSub(ObjectIndex, Player++, Camera++);
        if (temp_v0 != 0) {
            break;
        }
    }
    return temp_v0;
}

void KwanmDoshinRakka(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Anmptr) { /* irregular */
        case 0x32:
            if (KwaddFval(&DynamicObjects[ObjectIndex].PosSpline[1], DynamicObjects[ObjectIndex].PosEnde[1] + 15.0,
                                    1.5f) != 0) {
                KwanmSetflag(ObjectIndex, 0x00000200);
                KwanmSetevflag(ObjectIndex, 1);
                KwanmClrevflag(ObjectIndex, 2);
                KwanmNext(ObjectIndex);
            }
            break;
        case 0x33:
            if (KwsubFval(&DynamicObjects[ObjectIndex].PosSpline[1], 0.0f, 2.0f) != 0) {
                if (DynamicObjects[ObjectIndex].PosSpline[1] >= 16.0f) {
                    DynamicObjects[ObjectIndex].Anmnumbak = 0;
                } else if (DynamicObjects[ObjectIndex].PosSpline[1] >= 8.0f) {
                    DynamicObjects[ObjectIndex].Anmnumbak = 1;
                } else {
                    DynamicObjects[ObjectIndex].Anmnumbak = 2;
                }
                KwanmClrevflag(ObjectIndex, 1);
                if (KwanmBitcheckflag(ObjectIndex, 0x00010000) != 0) {
                    KwanmSetevflag(ObjectIndex, 0x00000010);
                    if (KwanmBitcheckflag(ObjectIndex, VISIBLE) != 0) {
                        KwanmSetevflag(ObjectIndex, 0x00000020);
                    }
                }
                if (KwanmBitcheckevflag(ObjectIndex, 2) != 0) {
                    KwanmJump(ObjectIndex, 0x00000064);
                } else {
                    KwanmNext(ObjectIndex);
                }
            }
            break;
        case 0x34:
            KwanmSetnumwait(ObjectIndex, 3, 6);
            break;
        case 0x35:
            KwanmSetnumwait(ObjectIndex, 2, 0x00000032);
            break;
        case 0x36:
            if (DynamicObjects[ObjectIndex].PosSpline[1] >= 20.0f) {
                DynamicObjects[ObjectIndex].Anmnumbak = 0;
            } else if (DynamicObjects[ObjectIndex].PosSpline[1] >= 18.0f) {
                DynamicObjects[ObjectIndex].Anmnumbak = 1;
            }
            if (KwaddFval(&DynamicObjects[ObjectIndex].PosSpline[1], DynamicObjects[ObjectIndex].PosEnde[1], 0.5f) !=
                0) {
                KwanmClrflag(ObjectIndex, 0x00000200);
                KwanmReturn(ObjectIndex);
            }
            break;
        case 0x64:
            KwanmForward(ObjectIndex, 3, 5, 1, 8, 0);
            break;
        case 0x65:
            KwanmWait(ObjectIndex, 0x0000001E);
            break;
        case 0x66:
            if (KwaddFval(&DynamicObjects[ObjectIndex].PosSpline[1], 20.0f, 1.5f) != 0) {
                KwanmNext(ObjectIndex);
            }
            break;
        case 0x67:
            if (KwsubFval(&DynamicObjects[ObjectIndex].PosSpline[1], 0.0f, 1.5f) != 0) {
                if (KwanmBitcheckflag(ObjectIndex, 0x00020000) != 0) {
                    KwanmSetevflag(ObjectIndex, 0x00000010);
                    if (KwanmBitcheckflag(ObjectIndex, VISIBLE) != 0) {
                        KwanmSetevflag(ObjectIndex, 0x00000020);
                    }
                }
                KwanmNext(ObjectIndex);
            }
            break;
        case 0x68:
            if (KwaddFval(&DynamicObjects[ObjectIndex].PosSpline[1], 12.0f, 1.5f) != 0) {
                KwanmNext(ObjectIndex);
            }
            break;
        case 0x69:
            if (KwsubFval(&DynamicObjects[ObjectIndex].PosSpline[1], 0.0f, 1.5f) != 0) {
                if (KwanmBitcheckflag(ObjectIndex, 0x00020000) != 0) {
                    KwanmSetevflag(ObjectIndex, 0x00000010);
                    if (KwanmBitcheckflag(ObjectIndex, VISIBLE) != 0) {
                        KwanmSetevflag(ObjectIndex, 0x00000020);
                    }
                }
                NAEnmTrgStart(DynamicObjects[ObjectIndex].pos, DynamicObjects[ObjectIndex].Velocity,
                              SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x45));
                KwanmNext(ObjectIndex);
            }
            break;
        case 0x6A:
            if (KwanmOufukuR(ObjectIndex, 5, 3, 1, 6, 3) != 0) {
                KwhitclearDoshin(ObjectIndex);
            }
            break;
        case 0x6B:
            if (DynamicObjects[ObjectIndex].PosSpline[1] >= 22.0f) {
                DynamicObjects[ObjectIndex].Anmnumbak = 0;
            } else if (DynamicObjects[ObjectIndex].PosSpline[1] >= 20.0f) {
                DynamicObjects[ObjectIndex].Anmnumbak = 1;
            } else if (DynamicObjects[ObjectIndex].PosSpline[1] >= 18.0f) {
                DynamicObjects[ObjectIndex].Anmnumbak = 2;
            } else if (DynamicObjects[ObjectIndex].PosSpline[1] >= 16.0f) {
                DynamicObjects[ObjectIndex].Anmnumbak = 3;
            } else if (DynamicObjects[ObjectIndex].PosSpline[1] >= 14.0f) {
                DynamicObjects[ObjectIndex].Anmnumbak = 4;
            } else {
                KwanmOufukuF(ObjectIndex, 3, 5, 1, 6, -1);
            }
            if (KwaddFval(&DynamicObjects[ObjectIndex].PosSpline[1], DynamicObjects[ObjectIndex].PosEnde[1], 0.5f) !=
                0) {
                KwanmTrig(ObjectIndex, 0);
                KwanmNext(ObjectIndex);
            }
            break;
        case 0x6C:
            if (KwanmWait(ObjectIndex, 0x00000064) != 0) {
                KwanmClrevflag(ObjectIndex, 2);
                KwanmClrflag(ObjectIndex, 0x00000200);
                KwanmReturn(ObjectIndex);
            }
            break;
        case 0xC8:
            if (KwanmWait(ObjectIndex, 0x0000012C) != 0) {
                KwanmBitcheckevflag(ObjectIndex, 0x00000080);
                KwanmFinish(ObjectIndex);
                KwanmJump(ObjectIndex, 1);
            }
            break;
        case 0x12C:
            if (KwanmTurnLeft(ObjectIndex, &DynamicObjects[ObjectIndex].Texang[1], 0x0400U, 0x00008000) != 0) {
                KwanmClrevflag(ObjectIndex, 4);
                KwanmReturn(ObjectIndex);
            }
            break;
    }
}

void KwanmDoshinInit(s32 ObjectIndex) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    object->Ground = 0.0f;
    object->PosStart[1] = 0.0f;
    KwsetInit3DSppos(ObjectIndex, 0.0f, 0.0f, 0.0f);
    if (ScreenFlip != 0) {
        KwsetInit3DAng(ObjectIndex, 0U, 0x4000U, 0U);
        KwsetInit3DTexang(ObjectIndex, 0U, 0x4000U, 0U);
    } else {
        KwsetInit3DAng(ObjectIndex, 0U, 0xC000U, 0U);
        KwsetInit3DTexang(ObjectIndex, 0U, 0xC000U, 0U);
    }
    KwanmInitCI8(ObjectIndex, d_course_bowsers_castle_thwomp_tlut, (u8*) d_course_bowsers_castle_thwomp_faces,
                        0x10U, (u16) 0x00000040);
    object->Shape = d_course_bowsers_castle_dl_thwomp;
    object->r = 0x000C;
    object->Scale = 1.0f;
    object->PosEnde[1] = 30.0f;
    KwanmSetflag(ObjectIndex, 0x05000220);
    object->Output = 0;
    object->Unk0DF = 6;
    KwanmClearstack(ObjectIndex);
    KwanmNext(ObjectIndex);
}

void KwanmDoshin(s32 ObjectIndex) {
    UNUSED s32 stackPadding[4];
    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 0:
            break;
        case 1:
            KwanmDoshinInit(ObjectIndex);
            break;
        case 2:
            KwanmWait(ObjectIndex, 0x0000003C);
            break;
        case 3:
            KwanmGosubnext(ObjectIndex, 0x00000032);
            break;
        case 4:
            if (KwcheckDoshinTurn(ObjectIndex) != 0) {
                KwanmGosub(ObjectIndex, 0x0000012C, 2);
            } else {
                KwanmJump(ObjectIndex, 2);
            }
            break;
    }
    KwanmDoshinRakka(ObjectIndex);
    KwsetSplinepos3D(ObjectIndex);
    DynamicObjects[ObjectIndex].ang[1] = DynamicObjects[ObjectIndex].Texang[1];
    KwanmTexaddrCI8Index(ObjectIndex);
}

void KwanmDoshinKurukuruInit(s32 ObjectIndex) {
    AnmObject* object;

    KwanmInitCI8(ObjectIndex, d_course_bowsers_castle_thwomp_tlut, (u8*) d_course_bowsers_castle_thwomp_faces,
                        0x10U, (u16) 0x00000040);
    object = &DynamicObjects[ObjectIndex];
    object->Scale = 1.0f;
    object->Shape = d_course_bowsers_castle_dl_thwomp;
    object->r = 0x000C;
    KwanmSetflag(ObjectIndex, 0x04000220);
    object->Output = 0;
    object->Unk0DF = 6;
    KwmvStart(ObjectIndex);
    object->Ground = 0.0f;
    object->PosStart[1] = 0.0f;
    KwsetInit3DSppos(ObjectIndex, 0.0f, 20.0f, 0.0f);
    object->PosEnde[1] = 20.0f;
    if (ScreenFlip != 0) {
        KwsetInit3DAng(ObjectIndex, 0U, 0x4000U, 0U);
        KwsetInit3DTexang(ObjectIndex, 0U, 0x4000U, 0U);
    } else {
        KwsetInit3DAng(ObjectIndex, 0U, 0xC000U, 0U);
        KwsetInit3DTexang(ObjectIndex, 0U, 0xC000U, 0U);
    }
    object->Mvptr = 1;
    if (object->Alpha == 0) {
        object->Spptr = 1;
    } else {
        object->Spptr = 2;
    }
    KwanmNext(ObjectIndex);
}

void KwmvDoshinKurukuruF(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Mvptr) {
        case 1:
            if (KwanmTurnLeft(ObjectIndex, (u16*) &DynamicObjects[ObjectIndex].Texang[1], 0x0800U, 0x00008000) != 0) {
                DynamicObjects[ObjectIndex].PosEnde[0] = (f32) ((f64) XOrientation * 200.0);
                KWAnmNext(ObjectIndex);
            }
            break;
        case 2:
            if (KWChaseFVal(DynamicObjects[ObjectIndex].PosSpline, DynamicObjects[ObjectIndex].PosEnde[0], 4.0f) != 0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 3:
            if (KwanmTurnLeft(ObjectIndex, (u16*) &DynamicObjects[ObjectIndex].Texang[1], 0x0400U, 0x00008000) != 0) {
                KwanmJump(ObjectIndex, 3);
                KWAnmNext(ObjectIndex);
            }
            break;
        case 5:
            if (KwanmTurnLeft(ObjectIndex, (u16*) &DynamicObjects[ObjectIndex].Texang[1], 0x0400U, 0x0000C000) != 0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 6:
            if (KwsubFval(&DynamicObjects[ObjectIndex].PosSpline[2], -100.0f, 2.0f) != 0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 7:
            if (KwanmTurnLeft(ObjectIndex, (u16*) &DynamicObjects[ObjectIndex].Texang[1], 0x0400U, 0x00004000) != 0) {
                KWAnmNext(ObjectIndex);
                KwanmJump(ObjectIndex, 3);
            }
            break;
        case 9:
            if (KwanmTurnLeft(ObjectIndex, (u16*) &DynamicObjects[ObjectIndex].Texang[1], 0x0400U, 0x00010000) != 0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 10:
            if (KWChaseFVal(DynamicObjects[ObjectIndex].PosSpline, 0.0f, 4.0f) != 0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 11:
            if (KwanmTurnLeft(ObjectIndex, (u16*) &DynamicObjects[ObjectIndex].Texang[1], 0x0400U, 0x00010000) != 0) {
                KWAnmNext(ObjectIndex);
                KwanmJump(ObjectIndex, 3);
            }
            break;
        case 13:
            if (KwanmTurnLeft(ObjectIndex, (u16*) &DynamicObjects[ObjectIndex].Texang[1], 0x0400U, 0x00014000) != 0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 14:
            if (KwaddFval(&DynamicObjects[ObjectIndex].PosSpline[2], 0.0f, 2.0f) != 0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 15:
            if (KwanmTurnLeft(ObjectIndex, (u16*) &DynamicObjects[ObjectIndex].Texang[1], 0x0400U, 0x0000C000) != 0) {
                KWAnmNext(ObjectIndex);
                KwanmJump(ObjectIndex, 3);
            }
            break;
        case 17:
            KwmvJump(ObjectIndex, 1);
            break;
        case 0:
        default:
            break;
    }
}

void KwmvDoshinKurukuruR(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Mvptr) {
        case 1:
            if (KwanmTurnLeft(ObjectIndex, (u16*) &DynamicObjects[ObjectIndex].Texang[1], 0x0400U, 0x00010000) != 0) {
                DynamicObjects[ObjectIndex].PosEnde[0] = (f32) ((f64) XOrientation * -200.0);
                KWAnmNext(ObjectIndex);
            }
            break;
        case 2:
            if (KWChaseFVal(DynamicObjects[ObjectIndex].PosSpline, DynamicObjects[ObjectIndex].PosEnde[0], 4.0f) != 0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 3:
            if (KwanmTurnLeft(ObjectIndex, (u16*) &DynamicObjects[ObjectIndex].Texang[1], 0x0400U, 0x00010000) != 0) {
                KwanmJump(ObjectIndex, 3);
                KWAnmNext(ObjectIndex);
            }
            break;
        case 5:
            if (KwanmTurnLeft(ObjectIndex, (u16*) &DynamicObjects[ObjectIndex].Texang[1], 0x0400U, 0x00004000) != 0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 6:
            if (KwaddFval(&DynamicObjects[ObjectIndex].PosSpline[2], 100.0f, 2.0f) != 0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 7:
            if (KwanmTurnLeft(ObjectIndex, (u16*) &DynamicObjects[ObjectIndex].Texang[1], 0x0400U, 0x0000C000) != 0) {
                KWAnmNext(ObjectIndex);
                KwanmJump(ObjectIndex, 3);
            }
            break;
        case 9:
            if (KwanmTurnLeft(ObjectIndex, (u16*) &DynamicObjects[ObjectIndex].Texang[1], 0x0400U, 0x00008000) != 0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 10:
            if (KWChaseFVal(DynamicObjects[ObjectIndex].PosSpline, 0.0f, 4.0f) != 0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 11:
            if (KwanmTurnLeft(ObjectIndex, (u16*) &DynamicObjects[ObjectIndex].Texang[1], 0x0400U, 0x00008000) != 0) {
                KWAnmNext(ObjectIndex);
                KwanmJump(ObjectIndex, 3);
            }
            break;
        case 13:
            if (KwanmTurnLeft(ObjectIndex, (u16*) &DynamicObjects[ObjectIndex].Texang[1], 0x0400U, 0x0000C000) != 0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 14:
            if (KwsubFval(&DynamicObjects[ObjectIndex].PosSpline[2], 0.0f, 2.0f) != 0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 15:
            if (KwanmTurnLeft(ObjectIndex, (u16*) &DynamicObjects[ObjectIndex].Texang[1], 0x0400U, 0x00014000) != 0) {
                KWAnmNext(ObjectIndex);
                KwanmJump(ObjectIndex, 3);
            }
            break;
        case 17:
            KwmvJump(ObjectIndex, 1);
            break;
        case 0:
        default:
            break;
    }
}

void KwmvDoshinKurukuru(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Spptr) { /* irregular */
        case 1:
            KwmvDoshinKurukuruF(ObjectIndex);
            break;
        case 2:
            KwmvDoshinKurukuruR(ObjectIndex);
            break;
    }
}

void KwanmDoshinKurukuru(s32 ObjectIndex) {

    switch (DynamicObjects[ObjectIndex].Anmptr) { /* irregular */
        case 0:
            break;
        case 1:
            KwanmDoshinKurukuruInit(ObjectIndex);
            break;
        case 3:
            KwanmGosubnext(ObjectIndex, 0x00000032);
            break;
        case 4:
            KWAnmNext(ObjectIndex);
            KwanmNext(ObjectIndex);
            break;
    }
    KwanmDoshinRakka(ObjectIndex);
    KwmvDoshinKurukuru(ObjectIndex);
    KwsetSplinepos3D(ObjectIndex);
    KwanmTexaddrCI8Index(ObjectIndex);
}

void KwgetOkkakeDoshin1(s32 ObjectIndex, s32 arg1, s32 Arg2) {
    AnmObject* object;

    KwanmSetevflag(ObjectIndex, 8);
    KwmvStart(ObjectIndex);
    object = &DynamicObjects[ObjectIndex];
    object->Spptr = 1;
    object->Anmnum = arg1;
    object->Ival = Arg2;
}

void KwgetOkkakeDoshin2(s32 ObjectIndex, s32 PlayerId) {
    Player* Player;

    Player = &PlayerOne[PlayerId];
    KwanmSetevflag(ObjectIndex, 8);
    KwmvStart(ObjectIndex);
    DynamicObjects[ObjectIndex].Spptr = 2;
    DynamicObjects[ObjectIndex].PosEnde[0] = Player->Position[0] - DynamicObjects[ObjectIndex].PosStart[0];
    DynamicObjects[ObjectIndex].Anmnum = PlayerId;
}

s32 KwcheckDoshinOkkakeSub(s32 PlayerId) {
    s32 someIndex;
    s32 ObjectIndex;
    s32 temp_s7;
    s32 var_s6;
    s32 pathPoint;

    pathPoint = NearestPathPointByPlayerId[PlayerId];
    var_s6 = 0;
    if ((pathPoint >= 0xAA) && (pathPoint < 0xB5)) {
        temp_s7 = MakeRandomLimmit(0x0032U) + 0x32;
        for (someIndex = 0; someIndex < NumActiveThwomps; someIndex++) {
            ObjectIndex = Objallocptr[someIndex];
            if (DynamicObjects[ObjectIndex].Chartptr == 3) {
                var_s6 = 1;
                KwgetOkkakeDoshin1(ObjectIndex, PlayerId, temp_s7);
            }
        }
    } else if ((pathPoint >= 0xD7) && (pathPoint < 0xE2)) {
        for (someIndex = 0; someIndex < NumActiveThwomps; someIndex++) {
            ObjectIndex = Objallocptr[someIndex];
            if (DynamicObjects[ObjectIndex].Chartptr == 3) {
                var_s6 = 1;
                KwgetOkkakeDoshin2(ObjectIndex, PlayerId);
            }
        }
    }
    return var_s6;
}

void KwcheckDoshinOkkake(void) {
    Player* Player;
    s32 ObjectIndex;
    s32 var_s0;
    s32 someIndex;
    s32 var_s4;
    AnmObject* object;

    Player = PlayerOne;
    var_s4 = 1;
    for (someIndex = 0; someIndex < NumActiveThwomps; someIndex++) {
        ObjectIndex = Objallocptr[someIndex];
        object = &DynamicObjects[ObjectIndex];
        if (object->Chartptr == 3) {
            var_s0 = 0;
            if ((object->Anmptr >= 2) && (KwanmBitoffcheckevflag(ObjectIndex, 8) != 0)) {
                var_s0 = 1;
            }
            var_s4 *= var_s0;
        }
    }
    if (var_s4 != 0) {
        for (var_s0 = 0; var_s0 < 4; var_s0++, Player++) {
            if ((Player->Flag & EXISTS) && !(Player->Flag & IS_CPU_PLAYER)) {
                if (KwcheckDoshinOkkakeSub(var_s0) != 0) {
                    break;
                }
            }
        }
    }
}

void KwanmDoshinOkkakeInit(s32 ObjectIndex) {
    AnmObject* object;

    KwanmInitCI8(ObjectIndex, d_course_bowsers_castle_thwomp_tlut, (u8*) d_course_bowsers_castle_thwomp_faces,
                        0x10U, (u16) 0x00000040);
    object = &DynamicObjects[ObjectIndex];
    object->Shape = d_course_bowsers_castle_dl_thwomp;
    object->r = 0x000C;
    object->Scale = 1.0f;
    KwanmSetflag(ObjectIndex, 0x04000220);
    object->Output = 0;
    object->Ground = 0.0f;
    object->PosStart[1] = 0.0f;
    KwsetInit3DSppos(ObjectIndex, 0.0f, 0.0f, 0.0f);
    KwsetInit3DAng(ObjectIndex, 0U, 0U, 0U);
    if (ScreenFlip != 0) {
        KwsetInit3DTexang(ObjectIndex, 0U, 0xC000U, 0U);
    } else {
        KwsetInit3DTexang(ObjectIndex, 0U, 0x4000U, 0U);
    }
    object->Velocity[0] = 0.0f;
    object->ang[1] = object->Texang[1];
    object->Spptr = 1;
    object->Unk0DF = 8;
    object->PosSpline[1] = 15.0f;
    object->PosEnde[1] = 15.0f;
    KwanmNext(ObjectIndex);
}

void KwmvDoshinOkkakeF(s32 ObjectIndex) {
    s32 var_v0;
    UNUSED s32 stackPadding;
    Player* Player;

    Player = &PlayerOne[DynamicObjects[ObjectIndex].Anmnum];
    switch (DynamicObjects[ObjectIndex].Mvptr) {
        case 1:
            DynamicObjects[ObjectIndex].Mvtm = 0x00A0;
            DynamicObjects[ObjectIndex].PosSpline[0] = 0.0f;
            DynamicObjects[ObjectIndex].PosSpline[2] = 0.0f;
            DynamicObjects[ObjectIndex].Velocity[2] = 0.0f;
            KWAnmNext(ObjectIndex);
            break;
        case 2:
            DynamicObjects[ObjectIndex].Velocity[0] = Player->Speed * XOrientation * 1.25;
            if (DynamicObjects[ObjectIndex].Ival >= DynamicObjects[ObjectIndex].Mvtm) {
                if (DynamicObjects[ObjectIndex].Mvtm == DynamicObjects[ObjectIndex].Ival) {
                    if (Kwgameframetimer & 1) {
                        DynamicObjects[ObjectIndex].Velocity[2] = 1.5f;
                    } else {
                        DynamicObjects[ObjectIndex].Velocity[2] = -1.5f;
                    }
                }
                if (DynamicObjects[ObjectIndex].Velocity[2] >= 0.0) {
                    if (DynamicObjects[ObjectIndex].PosSpline[2] >= 40.0) {
                        DynamicObjects[ObjectIndex].Velocity[2] = -1.5f;
                    }
                } else if ((f64) DynamicObjects[ObjectIndex].PosSpline[2] <= -40.0) {
                    DynamicObjects[ObjectIndex].Velocity[2] = 1.5f;
                }
            }
            KwmvAddvelxztopos(ObjectIndex);
            if (DynamicObjects[ObjectIndex].Mvtm < 0x65) {
                DynamicObjects[ObjectIndex].Texang[1] = KwchaseAngle(
                    DynamicObjects[ObjectIndex].Texang[1], (DynamicObjects[ObjectIndex].ang[1] + 0x8000));
                if (DynamicObjects[ObjectIndex].Mvtm == 0x0064) {
                    DynamicObjects[ObjectIndex].Anmnumbak = 1;
                }
            }
            var_v0 = 0;
            if (ScreenFlip != 0) {
                if (DynamicObjects[ObjectIndex].PosSpline[0] <= -1000.0) {
                    var_v0 = 1;
                }
            } else if (DynamicObjects[ObjectIndex].PosSpline[0] >= 1000.0) {
                var_v0 = 1;
            }
            DynamicObjects[ObjectIndex].Mvtm--;
            if ((DynamicObjects[ObjectIndex].Mvtm == 0) || (var_v0 != 0)) {
                DynamicObjects[ObjectIndex].vel = 0.0f;
                KwanmJump(ObjectIndex, 3);
                KWAnmNext(ObjectIndex);
            }
            break;
        case 4:
            KWChaseFVal(&DynamicObjects[ObjectIndex].PosSpline[2], 0.0f, 2.0f);
            KWChaseFVal(DynamicObjects[ObjectIndex].PosSpline, 0.0f, 5.0f);
            if ((DynamicObjects[ObjectIndex].PosSpline[0] + DynamicObjects[ObjectIndex].PosSpline[2]) == 0.0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 5:
            DynamicObjects[ObjectIndex].Texang[1] =
                KwchaseAngle(DynamicObjects[ObjectIndex].Texang[1], DynamicObjects[ObjectIndex].ang[1]);
            if (DynamicObjects[ObjectIndex].Texang[1] == DynamicObjects[ObjectIndex].ang[1]) {
                KwanmClrevflag(ObjectIndex, 8);
                KWAnmNext(ObjectIndex);
                DynamicObjects[ObjectIndex].Anmnumbak = 0;
            }
            break;
        case 0:
        case 3:
        default:
            break;
    }
}

void KwmvDoshinOkkakeW(s32 ObjectIndex) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    switch (object->Mvptr) {
        case 1:
            if (KWChaseFVal(&object->PosSpline[0], object->PosEnde[0], 5.0f) != 0) {
                KwanmJump(ObjectIndex, 3);
                KWAnmNext(ObjectIndex);
                break;
            }
        case 0:
        case 2:
            break;
        case 3:
            if (KWChaseFVal(&object->PosSpline[0], 0.0f, 5.0f) != 0) {
                KWAnmNext(ObjectIndex);
                KwanmClrevflag(ObjectIndex, 8);
            }
            break;
    }
}

void KwmvDoshinOkkake(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Spptr) {
        case 1:
            KwmvDoshinOkkakeF(ObjectIndex);
            break;
        case 2:
            KwmvDoshinOkkakeW(ObjectIndex);
            break;
    }
}

void KwanmDoshinOkkake(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Anmptr) { /* irregular */
        case 0:
            break;
        case 1:
            KwanmDoshinOkkakeInit(ObjectIndex);
            break;
        case 3:
            KwanmGosubnext(ObjectIndex, 0x00000032);
            break;
        case 4:
            KwanmNext(ObjectIndex);
            KWAnmNext(ObjectIndex);
            break;
    }
    KwanmDoshinRakka(ObjectIndex);
    KwmvDoshinOkkake(ObjectIndex);
    KwsetSplinepos3D(ObjectIndex);
    KwanmTexaddrCI8Index(ObjectIndex);
}

void KwanmDoshinGuardInit(s32 ObjectIndex) {
    AnmObject* object;

    KwanmInitCI8(ObjectIndex, d_course_bowsers_castle_thwomp_tlut, (u8*) d_course_bowsers_castle_thwomp_faces,
                        0x10U, (u16) 0x00000040);
    object = &DynamicObjects[ObjectIndex];
    object->Shape = d_course_bowsers_castle_dl_thwomp;
    object->r = 0x000C;
    object->Scale = 1.0f;
    KwanmSetflag(ObjectIndex, 0x04000220);
    object->Output = 2;
    object->Unk0DF = 8;
    KwsetInit3DAng(ObjectIndex, 0U, 0U, 0U);
    object->Ground = 0.0f;
    object->PosStart[1] = 0.0f;
    KwsetInit3DSppos(ObjectIndex, 0.0f, 0.0f, 0.0f);
    object->PosEnde[1] = 30.0f;
    if (ScreenFlip != 0) {
        KwsetInit3DTexang(ObjectIndex, 0U, 0x4000U, 0U);
    } else {
        KwsetInit3DTexang(ObjectIndex, 0U, 0xC000U, 0U);
    }
    switch (object->Alpha) { /* irregular */
        case 0:
            object->Anmtm = 2;
            break;
        case 1:
            object->Anmtm = 0x0000003C;
            break;
        case 2:
            object->Anmtm = 0x00000078;
            break;
        case 3:
            object->Anmtm = 0x000000B4;
            break;
    }
    KwanmClearstack(ObjectIndex);
    KwanmNext(ObjectIndex);
}

void KwanmDoshinGuard(s32 ObjectIndex) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    switch (object->Anmptr) {
        case 0:
            break;
        case 1:
            KwanmDoshinGuardInit(ObjectIndex);
            break;
        case 2:
            KwanmWait(ObjectIndex, object->Anmtm);
            break;
        case 3:
            KwanmGosubnext(ObjectIndex, 0x00000032);
            break;
        case 4:
            object->Anmtm = 0x0000003C;
            KwanmJump(ObjectIndex, 2);
            break;
    }
    KwanmDoshinRakka(ObjectIndex);
    KwsetSplinepos3D(ObjectIndex);
    KwanmTexaddrCI8Index(ObjectIndex);
}

void KwanmDoshinOriInit(s32 ObjectIndex) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    object->Texsizex = 0;
    KwanmInitCI8(ObjectIndex, d_course_bowsers_castle_thwomp_tlut, (u8*) d_course_bowsers_castle_thwomp_faces,
                        0x10U, (u16) 0x00000040);
    object->Shape = d_course_bowsers_castle_dl_thwomp;
    object->Anmnumbak = 0;
    object->r = 0x000C;
    object->Scale = 1.5f;
    KwanmSetflag(ObjectIndex, 0x05000220);
    object->Output = 1;
    object->Unk0DF = 6;
    KwsetInit3DSppos(ObjectIndex, 0.0f, 0.0f, 0.0f);
    KwsetInit3DAng(ObjectIndex, 0U, 0U, 0U);
    object->Ground = 0.0f;
    object->PosStart[1] = 0.0f;
    object->PosSpline[1] = 10.0f;
    object->PosEnde[1] = 10.0f;
    if (ScreenFlip != 0) {
        KwsetInit3DTexang(ObjectIndex, 0U, 0x4000U, 0U);
    } else {
        KwsetInit3DTexang(ObjectIndex, 0U, 0xC000U, 0U);
    }
    object->PosSpline[0] = 0.0f;
    object->PosSpline[2] = 0.0f;
    KwanmClearstack(ObjectIndex);
    KwanmNext(ObjectIndex);
}

void KwanmDoshinOri(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 0:
            break;
        case 1:
            KwanmDoshinOriInit(ObjectIndex);
            break;
        case 2:
            KwcheckVisibleAreaCamera(ObjectIndex, 100.0f);
            if (KwanmBitcheckflag(ObjectIndex, VISIBLE) != 0) {
                NAEnmTrgStart(DynamicObjects[ObjectIndex].pos, DynamicObjects[ObjectIndex].Velocity,
                              SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x45));
                KwanmNext(ObjectIndex);
            }
            break;
        case 3:
            if (KwanmOufukuF(ObjectIndex, 3, 5, 1, 6, 6) != 0) {
                DynamicObjects[ObjectIndex].Anmnumbak = 0;
            }
            break;
        case 4:
            if (KwanmWait(ObjectIndex, 0x0000012C) != 0) {
                KwanmJump(ObjectIndex, 2);
            }
            break;
    }
    KwsetSplinepos3D(ObjectIndex);
    KwanmTexaddrCI8Index(ObjectIndex);
}

void KwanmDoshinOufukuInit(s32 ObjectIndex) {
    AnmObject* object;

    KwanmInitCI8(ObjectIndex, d_course_bowsers_castle_thwomp_tlut, (u8*) d_course_bowsers_castle_thwomp_faces,
                        0x10U, (u16) 0x00000040);
    object = &DynamicObjects[ObjectIndex];
    object->Shape = d_course_bowsers_castle_dl_thwomp;
    object->r = 0x000C;
    object->Anmnumbak = 0;
    object->Scale = 1.0f;
    KwanmSetflag(ObjectIndex, 0x04000220);
    object->Output = 0;
    object->Unk0DF = 0x0A;
    KwmvStart(ObjectIndex);
    KwsetInit3DSppos(ObjectIndex, 0.0f, 0.0f, 0.0f);
    object->Ground = 70.0f;
    object->PosStart[1] = 70.0f;
    object->PosEnde[1] = 0.0f;
    KwsetInit3DAng(ObjectIndex, 0U, 0U, 0U);
    if ((ScreenFlip != 0) || (GameState == 9)) {
        KwsetInit3DTexang(ObjectIndex, 0U, 0xC000U, 0U);
    } else {
        KwsetInit3DTexang(ObjectIndex, 0U, 0x4000U, 0U);
    }
    switch (object->Alpha) { /* irregular */
        case 0:
            object->Spptr = 2;
            object->Velocity[2] = -1.0f;
            break;
        case 1:
            object->Spptr = 2;
            object->Velocity[2] = -1.5f;
            break;
    }
    KwanmSetevflag(ObjectIndex, 0x00000080);
    KwanmNext(ObjectIndex);
}

void KwmvDoshinOufukuF(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Mvptr) {
        case 0:
            break;
        case 1:
            if (KWChaseFVal(&DynamicObjects[ObjectIndex].PosSpline[2], 250.0f, DynamicObjects[ObjectIndex].Velocity[2]) !=
                0) {
                DynamicObjects[ObjectIndex].Velocity[2] = -DynamicObjects[ObjectIndex].Velocity[2];
                KWAnmNext(ObjectIndex);
            }
            break;
        case 2:
            if (KWChaseFVal(&DynamicObjects[ObjectIndex].PosSpline[2], 0.0f, DynamicObjects[ObjectIndex].Velocity[2]) !=
                0) {
                DynamicObjects[ObjectIndex].Velocity[2] = -DynamicObjects[ObjectIndex].Velocity[2];
                KwmvJump(ObjectIndex, 1);
            }
            break;
    }
}

void KwmvDoshinOufukuR(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Mvptr) {
        case 0:
            break;
        case 1:
            if (KWChaseFVal(&DynamicObjects[ObjectIndex].PosSpline[2], -250.0f, DynamicObjects[ObjectIndex].Velocity[2]) !=
                0) {
                DynamicObjects[ObjectIndex].Velocity[2] = -DynamicObjects[ObjectIndex].Velocity[2];
                KWAnmNext(ObjectIndex);
            }
            break;
        case 2:
            if (KWChaseFVal(&DynamicObjects[ObjectIndex].PosSpline[2], 0.0f, DynamicObjects[ObjectIndex].Velocity[2]) !=
                0) {
                DynamicObjects[ObjectIndex].Velocity[2] = -DynamicObjects[ObjectIndex].Velocity[2];
                KwmvJump(ObjectIndex, 1);
            }
            break;
    }
}

void KwmvDoshinOufuku(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Spptr) {
        case 1:
            KwmvDoshinOufukuF(ObjectIndex);
            break;
        case 2:
            KwmvDoshinOufukuR(ObjectIndex);
            break;
    }
    KwsetSplinepos3D(ObjectIndex);
}

void KwanmDoshinOufuku(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Anmptr) { /* irregular */
        case 0:
            break;
        case 1:
            KwanmDoshinOufukuInit(ObjectIndex);
            break;
        case 2:
            KwanmOufukuF(ObjectIndex, 3, 5, 1, 6, -1);
            break;
    }
    if (DynamicObjects[ObjectIndex].Anmptr >= 2) {
        KwanmDoshinRakka(ObjectIndex);
        KwmvDoshinOufuku(ObjectIndex);
        KwanmTexaddrCI8Index(ObjectIndex);
        if (GameState != 9) {
            if ((D_8018D40C == 0) && (DynamicObjects[ObjectIndex].Anmptr == 2)) {
                NAEnmTrgStart(DynamicObjects[ObjectIndex].pos, DynamicObjects[ObjectIndex].Velocity,
                              SOUND_ARG_LOAD(0x19, 0x03, 0x60, 0x45));
            }
        } else if ((CutsceneShotTimer < 0xBF) && (((s16) CutsceneShotTimer % 88) == 0x0000001E)) {
            NAEnmTrgStart(DynamicObjects[ObjectIndex].pos, DynamicObjects[ObjectIndex].Velocity,
                          SOUND_ARG_LOAD(0x19, 0x03, 0x60, 0x45));
        }
    }
}

void KwdarkcheckDoshin(s32 ObjectIndex, Player* Player) {
    if (KwanmCheckradiusPlayerXz(ObjectIndex, Player, 12.0f) != 0) {
        Player->TireFL.Dummy |= 3;
    }
}

void KwshakecameraDoshin(s32 ObjectIndex, s32 cameraPlayerId) {
    Camera* Camera = &Camera1[cameraPlayerId];
    Player* Player = &PlayerOne[cameraPlayerId];

    if (ScreenSplitB != SCREEN_MODE_3P_4P_SPLITSCREEN) {
        if ((KwanmBitcheckevflag(ObjectIndex, 0x00000010) != 0) &&
            (KwanmCheckradiusPlayerXz(ObjectIndex, Player, 500.0f) != false)) {
            ShakeCamera(Camera);
            NAEnmTrgStart(DynamicObjects[ObjectIndex].pos, DynamicObjects[ObjectIndex].Velocity,
                          SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x0F));
        }
    }
}

void KwhitcheckDoshin(s32 ObjectIndex, s32 PlayerId) {
    f32 temp_f0;
    Player* temp_s0;

    temp_s0 = &PlayerOne[PlayerId];
    if (KwanmBitcheckflag(ObjectIndex, 0x00000200) != 0) {
        if (!(temp_s0->Weapon & PRESS_SQUISH)) {
            temp_f0 = KwanmSetheightGround(ObjectIndex, temp_s0);
            if ((temp_f0 <= 9.0) && !(temp_s0->SlipFlag & BROKEN) &&
                (KwanmCheckcollPlayerXz(ObjectIndex, temp_s0) != 0)) {
                if ((temp_s0->Flag & EXISTS) && !(temp_s0->Flag & IS_GHOST)) {
                    if (!(temp_s0->SlipFlag & STAR)) {
                        KwreboundsndPlayer(ObjectIndex, PlayerId, 1.4f, 1.1f, SOUND_ARG_LOAD(0x19, 0x00, 0xA0, 0x4C));
                    } else if (KwanmBitoffcheckevflag(ObjectIndex, 0x00000040) != 0) {
                        if (temp_s0->Flag & IS_CPU_PLAYER) {
                            NAEnmTrgStart(temp_s0->Position, temp_s0->Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0xA2, 0x4A));
                        } else {
                            NAPlyTrgStart((u8) PlayerId, SOUND_ARG_LOAD(0x19, 0x01, 0xA2, 0x4A));
                        }
                        KwhitclearDoshin(ObjectIndex);
                        KwgetbrokenObject(DynamicObjects[ObjectIndex].pos, 3, 3, D_8018D3C4);
                        KwanmClrflag(ObjectIndex, 0x00000200);
                        KwanmSetevflag(ObjectIndex, 0x00000040);
                        KwmvFinish(ObjectIndex);
                        KwanmJump(ObjectIndex, 0x000000C8);
                    }
                }
            } else if ((temp_f0 <= 17.5) && (KwanmBitcheckevflag(ObjectIndex, 1) != 0) &&
                       (KwanmCheckradiusPlayerXz(ObjectIndex, temp_s0, (temp_s0->Speed * 0.5) + 7.0) !=
                        0)) {
                if ((temp_s0->Flag & EXISTS) && !(temp_s0->Flag & IS_GHOST)) {
                    if (KwanmBitcheckflag(ObjectIndex, 0x04000000) != 0) {
                        KwlostGhost();
                    }
                    KwanmSetevflag(ObjectIndex, 2);
                    temp_s0->LinkDoshin = (s16) ObjectIndex;
                    temp_s0->HitFlag |= CRITTER_TOUCH;
                    temp_s0->Weapon |= PRESS_SQUISH;
                    KwstopKart(temp_s0);
                }
            }
        } else {
            KwstopKart(temp_s0);
        }
    }
}

void KwhitclearDoshin(s32 arg0) {
    Player* Player;
    s32 var_v1;

    Player = PlayerOne;
    for (var_v1 = 0; var_v1 < NUM_PLAYERS; var_v1++, Player++) {
        if (arg0 == Player->LinkDoshin) {
            Player->Weapon &= ~PRESS_SQUISH;
            Player->LinkDoshin = -1;
        }
    }
}

void KwinitDoshinSmoke(s32 objectIndex1, s32 objectIndex2, s32 Arg2) {
    u16 anAngle;
    f32 Thing1;
    f32 Thing0;

    KWAnmStart(objectIndex1, Arg2);
    DynamicObjects[objectIndex1].Chartptr = 2;
    anAngle = DynamicObjects[objectIndex2].ang[1];
    Thing1 = Kwrotate2DX(D_800E594C[Arg2 * 2 + 1], D_800E594C[Arg2 * 2 + 0], anAngle);
    Thing0 = Kwrotate2DY(D_800E594C[Arg2 * 2 + 1], D_800E594C[Arg2 * 2 + 0], anAngle);
    DynamicObjects[objectIndex1].PosStart[0] = DynamicObjects[objectIndex2].pos[0] + Thing0;
    DynamicObjects[objectIndex1].PosStart[1] = DynamicObjects[objectIndex2].Ground - 9.0;
    DynamicObjects[objectIndex1].PosStart[2] = DynamicObjects[objectIndex2].pos[2] + Thing1;
    anAngle = DynamicObjects[objectIndex2].ang[1] + D_800E597C[Arg2];
    DynamicObjects[objectIndex1].Velocity[0] = SinT(anAngle) * 0.6;
    DynamicObjects[objectIndex1].Velocity[2] = CosT(anAngle) * 0.6;
}

void KwgetSmokeDoshin(s32 arg0) {
    s32 ObjectIndex;
    s32 i;

    for (i = 0; i < 6; i++) {
        ObjectIndex = KwallocEffectram(EffectAllocArray2, &NextFreeObjectParticle2, gObjectParticle2_SIZE);
        if (ObjectIndex == NULL_OBJECT_ID) {
            break;
        }
        KwinitDoshinSmoke(ObjectIndex, arg0, i);
    }
}

void KwanmDoshinSmokeInit(s32 ObjectIndex) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    object->Indexaddr = D_8018D490;
    object->Sindexaddr = D_8018D490;
    object->Alpha = 0x00FF;
    object->ang[1] = 0;
    object->Texang[0] = 0;
    object->Texang[2] = 0;
    object->PosSpline[0] = 0.0f;
    object->PosSpline[1] = 0.0f;
    object->PosSpline[2] = 0.0f;
    object->Scale = 0.25f;
    KwanmNext(ObjectIndex);
}

void KwanmDoshinSmoke(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Anmptr) { /* irregular */
        case 0:
            break;
        case 1:
            KwanmDoshinSmokeInit(ObjectIndex);
            break;
        case 2:
            KwmvAddvelxztopos(ObjectIndex);
            KwaddFval(&DynamicObjects[ObjectIndex].PosSpline[1], 14.0f, 0.5f);
            KwsetfvalForwardLimit(ObjectIndex, &DynamicObjects[ObjectIndex].Scale, 0.25f, 0.75f, 0.025f, 1, 0);
            if (KwsetsvalReverse(ObjectIndex, &DynamicObjects[ObjectIndex].Alpha, 0x000000FF, 0, 4, 0, 0) != 0) {
                KwanmNext(ObjectIndex);
            }
            break;
        case 3:
            KwanmFinish(ObjectIndex);
            break;
    }
    KwsetSplinepos3D(ObjectIndex);
}

// update twomp I think
void KwchartDoshin(void) {
}

void KwchartDoshinVf(void) {
    Player* Player;
    s32 ObjectIndex;
    s32 var_s2_3;
    s32 var_s4;

    D_80165834[0] += 0x100;
    D_80165834[1] += 0x200;
    for (var_s4 = 0; var_s4 < NumActiveThwomps; var_s4++) {
        ObjectIndex = Objallocptr[var_s4];
        KwanmClrevflag(ObjectIndex, 0x00000010);
        KwcheckAreainCamera(ObjectIndex);
    }
    KwcheckDoshinOkkake();
    for (var_s4 = 0; var_s4 < NumActiveThwomps; var_s4++) {
        ObjectIndex = Objallocptr[var_s4];
        if (DynamicObjects[ObjectIndex].Anmptr != 0) {
            switch (DynamicObjects[ObjectIndex].Chartptr) {
                case 1:
                    KwanmDoshin(ObjectIndex);
                    break;
                case 2:
                    KwanmDoshinKurukuru(ObjectIndex);
                    break;
                case 3:
                    KwanmDoshinOkkake(ObjectIndex);
                    break;
                case 4:
                    KwanmDoshinGuard(ObjectIndex);
                    break;
                case 6:
                    KwanmDoshinOri(ObjectIndex);
                    break;
                case 5:
                    KwanmDoshinOufuku(ObjectIndex);
                    break;
            }
        }
    }
    Player = PlayerOne;
    for (var_s4 = 0; var_s4 < NUM_PLAYERS; var_s4++, Player++) {
        Player->TireFL.Dummy &= ~3;
        Player->HitFlag &= ~(CRITTER_TOUCH_GATE | CRITTER_TOUCH);
        for (var_s2_3 = 0; var_s2_3 < NumActiveThwomps; var_s2_3++) {
            ObjectIndex = Objallocptr[var_s2_3];
            if (!(Player->SlipFlag & TERESA)) {
                KwhitcheckDoshin(ObjectIndex, var_s4);
            }
            if (KwanmBitcheckflag(ObjectIndex, 0x00020000) != 0) {
                KwdarkcheckDoshin(ObjectIndex, Player);
            }
            if (KwanmBitcheckflag(ObjectIndex, 0x00010000) != 0) {
                KwshakecameraDoshin(ObjectIndex, var_s4);
            }
        }
    }
    KwchartBrokenObject(3);
    for (var_s4 = 0; var_s4 < NumActiveThwomps; var_s4++) {
        ObjectIndex = Objallocptr[var_s4];
        if (KwanmBitcheckevflag(ObjectIndex, 0x00000020) == 0) {
            continue;
        }

        KwanmClrevflag(ObjectIndex, 0x00000020);
        KwgetSmokeDoshin(ObjectIndex);
    }
    for (var_s4 = 0; var_s4 < gObjectParticle2_SIZE; var_s4++) {
        ObjectIndex = EffectAllocArray2[var_s4];
        if (ObjectIndex == DELETED_OBJECT_ID) {
            continue;
        }
        if (DynamicObjects[ObjectIndex].Anmptr == 0) {
            continue;
        }
        KwanmDoshinSmoke(ObjectIndex);
        if (DynamicObjects[ObjectIndex].Anmptr != 0) {
            continue;
        }
        KwdeallocEffectram(&EffectAllocArray2[var_s4]);
    }
}

void KwsetTuchi(s32 ObjectIndex) {
    UNUSED s32 stackPadding[3];
    s32 sp70;
    s32 var_s1;
    s32 var_s7;
    s32 loopObjectIndex;

    if (PlayerCount == 1) {
        sp70 = 8;
    } else {
        sp70 = 4;
    }

    for (var_s7 = 0; var_s7 < sp70; var_s7++) {
        for (var_s1 = 0; var_s1 < gObjectParticle2_SIZE; var_s1++) {
            loopObjectIndex = EffectAllocArray2[var_s1];

            if (DynamicObjects[loopObjectIndex].Anmptr != 0) {
                continue;
            }

            KWAnmStart(loopObjectIndex, 0);
            DynamicObjects[loopObjectIndex].Texaddr = d_course_moo_moo_farm_mole_dirt;
            DynamicObjects[loopObjectIndex].Stexaddr = d_course_moo_moo_farm_mole_dirt;
            DynamicObjects[loopObjectIndex].Scale = 0.15f;
            DynamicObjects[loopObjectIndex].Velocity[1] = MakeRandomLimmit(0x000AU);
            DynamicObjects[loopObjectIndex].Velocity[1] = (DynamicObjects[loopObjectIndex].Velocity[1] * 0.1) + 4.8;
            DynamicObjects[loopObjectIndex].vel = MakeRandomLimmit(5U);
            DynamicObjects[loopObjectIndex].vel = (DynamicObjects[loopObjectIndex].vel * 0.01) + 0.8;
            DynamicObjects[loopObjectIndex].Texang[1] = (0x10000 / sp70) * var_s1;
            DynamicObjects[loopObjectIndex].PosStart[0] = DynamicObjects[ObjectIndex].PosStart[0];
            DynamicObjects[loopObjectIndex].PosStart[1] = DynamicObjects[ObjectIndex].PosStart[1] - 13.0;
            DynamicObjects[loopObjectIndex].PosStart[2] = DynamicObjects[ObjectIndex].PosStart[2];
            break;
        }
    }
}

void KwanmTuchi(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 0:
            break; /* irregular */
        case 1:
            if (KwmvNanamenage(ObjectIndex, DynamicObjects[ObjectIndex].Velocity[1], 0.3f, DynamicObjects[ObjectIndex].vel,
                              DynamicObjects[ObjectIndex].Texang[1], 0x00000032) != 0) {
                KwanmNext(ObjectIndex);
            }
            KwsetSplinepos3D(ObjectIndex);
            break;
        case 2:
            KwanmFinish(ObjectIndex);
            KwmvFinish(ObjectIndex);
            break;
    }
}

void KwanmChoropuInit(s32 ObjectIndex) {
    KwanmInitCI8(ObjectIndex, d_course_moo_moo_farm_mole_tlut, (u8*) d_course_moo_moo_farm_mole_frames, 0x20U,
                        (u16) 0x00000040);
    DynamicObjects[ObjectIndex].Scale = 0.15f;
    DynamicObjects[ObjectIndex].Anmnumbak = 0;
    KwsetInit3DSppos(ObjectIndex, 0.0f, 0.0f, 0.0f);
    KwsetInit3DAng(ObjectIndex, 0U, 0U, 0U);
    KwsetInit3DTexang(ObjectIndex, 0U, 0U, 0x8000U);
    DynamicObjects[ObjectIndex].r = 6;
    DynamicObjects[ObjectIndex].Velocity[1] = 4.0f;
    KwanmSetflag(ObjectIndex, 0x04000000);
    KwanmNext(ObjectIndex);
}

void KwmvChoropuSub(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Mvptr) {
        case 1:
            if (KwaddFval(&DynamicObjects[ObjectIndex].PosSpline[1], 9.0f, 0.7f) != 0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 2:
            KwmvWaitnext(ObjectIndex, 0x0000000A);
            break;
        case 3:
            if (KwsubFval(&DynamicObjects[ObjectIndex].PosSpline[1], 3.0f, 1.0f) != 0) {
                KwmvFinish(ObjectIndex);
            }
            break;
        case 4:
            KwmvNageageGround(ObjectIndex, 3.6f, 0.25f, 0.0f);
            break;
        case 5:
            KwmvFinish(ObjectIndex);
            break;
        case 10:
            DynamicObjects[ObjectIndex].Texang[2] += 0x1000;
            DynamicObjects[ObjectIndex].Velocity[1] -= 0.184;
            KwmvVxz(ObjectIndex);
            KwmvAddveltopos(ObjectIndex);
            if (DynamicObjects[ObjectIndex].pos[1] <= -10.0) {
                KwmvFinish(ObjectIndex);
            }
            break;
        case 0:
        default:
            break;
    }
}

void KwmvChoropu(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Spptr) { /* irregular */
        case 0:
            break;
        case 1:
            KwspStrategy3D(ObjectIndex);
            break;
        case 2:
            KwmvChoropuSub(ObjectIndex);
            break;
    }
    KwsetSplinepos3D(ObjectIndex);
}

void KwanmChoropu(s32 ObjectIndex, s32 arg1) {
    s8* sp2C;
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    switch (object->Anmptr) { /* irregular */
        case 0x1:
            KwanmChoropuInit(ObjectIndex);
            break;
        case 0x2:
            if (object->Timer == 0) {
                KwmvStartNormal(ObjectIndex, 2, 1);
                KwanmNext(ObjectIndex);
                KwanmSetflag(ObjectIndex, 0x00000200);
            } else {
                object->Timer--;
            }
            break;
        case 0x3:
            if (object->Mvptr == 0) {
                KwmvStartNormal(ObjectIndex, 2, 4);
                KwsetTuchi(ObjectIndex);
                KwanmNext(ObjectIndex);
                NAEnmTrgStart(object->pos, object->Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x07));
            }
            break;
        case 0x4:
            if (KwanmForward(ObjectIndex, 1, 6, 1, 2, 0) != 0) {
                KwanmJump(ObjectIndex, 0x00000064);
            }
            break;
        case 0xA:
            KwanmForward(ObjectIndex, 1, 6, 1, 0, -1);
            if (object->Mvptr == 0) {
                KwanmJump(ObjectIndex, 0x00000064);
            }
            break;
        case 0x64:
            if (object->Mvptr == 0) {
                KwanmClrflag(ObjectIndex, 0x00000200);
                KwanmFinish(ObjectIndex);
                switch (arg1) { /* switch 1; irregular */
                    case 1:     /* switch 1 */
                        sp2C = D_8018D198;
                        break;
                    case 2: /* switch 1 */
                        sp2C = D_8018D1A8;
                        break;
                    case 3: /* switch 1 */
                        sp2C = D_8018D1B8;
                        break;
                }
                sp2C[object->Output] = 0;
            }
            break;
        case 0:
        default:
            break;
    }
    if (object->Anmptr >= 2) {
        KwanmTexaddrCI8Index(ObjectIndex);
    }
}

void KwhitcheckChoropu(s32 ObjectIndex) {
    Player* Player;
    Camera* var_s4;
    s32 playerIndex;
    s32 var_s5;
    AnmObject* object;

    var_s5 = 0;
    Player = PlayerOne;
    var_s4 = Camera1;
    for (playerIndex = 0; playerIndex < Kwkartnum; playerIndex++, Player++, var_s4++) {
        if ((KwanmBitcheckflag(ObjectIndex, 0x00000200) != 0) && !(Player->SlipFlag & TERESA) &&
            (KwanmCheckcollPlayerXYZ(ObjectIndex, Player) != 0)) {
            if ((Player->Flag & EXISTS) && !(Player->Flag & IS_GHOST)) {
                var_s5 = 1;
                object = &DynamicObjects[ObjectIndex];
                if (KwanmBitcheckflag(ObjectIndex, 0x04000000) != 0) {
                    KwlostGhost();
                }
                if (Player->SlipFlag & STAR) {
                    NAPlyTrgStart(playerIndex, 0x1900A046U);
                } else {
                    Player->Weapon |= HIT_REDSHELL;
                }
                object->ang[1] = var_s4->CameraDirection[1];
                object->Velocity[1] = (Player->Speed / 2) + 3.0;
                object->vel = Player->Speed + 1.0;
                if (object->Velocity[1] >= 5.0) {
                    object->Velocity[1] = 5.0f;
                }
                if (object->vel >= 4.0) {
                    object->Velocity[1] = 4.0f;
                }
            }
        }
    }
    if (var_s5 != 0) {
        object = &DynamicObjects[ObjectIndex];
        KwanmClrflag(ObjectIndex, 0x00000200);
        KwmvFinish(ObjectIndex);
        KwsetInit3DSpos(ObjectIndex, object->pos[0], object->pos[1], object->pos[2]);
        KwsetInit3DSppos(ObjectIndex, 0.0f, 0.0f, 0.0f);
        KwmvStartNormal(ObjectIndex, 2, 0x000A);
        KwanmJump(ObjectIndex, 0x0000000A);
    }
}

void KwsetRndChoropu(s32 ObjectIndex, s32 arg1) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    s32 moleCount;
    s16 var_v1;
    s16 Offset;
    s32 var_a0;
    s8* var_a2;

    KWAnmStart(ObjectIndex, 0);
    DynamicObjects[ObjectIndex].Timer = MakeRandomLimmit(0x001EU) + 5;
    switch (arg1) { /* irregular */
        case 1:
            var_a2 = D_8018D198;
            moleCount = NUM_GROUP1_MOLES;
            Offset = 0;
            break;
        case 2:
            var_a2 = D_8018D1A8;
            moleCount = NUM_GROUP2_MOLES;
            Offset = 24;
            // Offset = NUM_GROUP1_MOLES;
            break;
        case 3:
            var_a2 = D_8018D1B8;
            moleCount = NUM_GROUP3_MOLES;
            Offset = 57;
            // Offset = NUM_GROUP1_MOLES + NUM_GROUP2_MOLES;
            break;
    }
    var_v1 = MakeRandomLimmit(moleCount);
    for (var_a0 = 0; var_a0 < moleCount; var_a0++) {
        if (var_a2[var_v1] != 0) {
            var_v1++;
            if (var_v1 == moleCount) {
                var_v1 = 0;
            }
        } else {
            var_a2[var_v1] = 1;
            DynamicObjects[ObjectIndex].Output = var_v1;
            break;
        }
    }
    /*
    Ideally `MoleSpawns` wouldn't be a union at all and its just be a list of Vec3s
    Even more ideally each mole group would have its own array for its spawns
    DynamicObjects[ObjectIndex].PosStart[0] = MoleSpawns.AsVec3SList[Offset + var_v1][0] * XOrientation;
    DynamicObjects[ObjectIndex].PosStart[1] = MoleSpawns.AsVec3SList[Offset + var_v1][1] - 9.0;
    DynamicObjects[ObjectIndex].PosStart[2] = MoleSpawns.AsVec3SList[Offset + var_v1][2];
    */
    DynamicObjects[ObjectIndex].PosStart[0] = MoleSpawns.AsFlatList[Offset + (var_v1 * 3) + 0] * XOrientation;
    DynamicObjects[ObjectIndex].PosStart[1] = MoleSpawns.AsFlatList[Offset + (var_v1 * 3) + 1] - 9.0;
    DynamicObjects[ObjectIndex].PosStart[2] = MoleSpawns.AsFlatList[Offset + (var_v1 * 3) + 2];
}

void KwchartChoropuSub(s32 ObjectIndex, s32 arg1) {
    if (DynamicObjects[ObjectIndex].Anmptr != 0) {
        KwanmChoropu(ObjectIndex, arg1);
        KwmvChoropu(ObjectIndex);
        KwhitcheckChoropu(ObjectIndex);
    }
}

void KwchartChoropu(void) {
    s32 var_s1;
    s32 ObjectIndex;
    UNUSED s32 stackPadding;

    for (var_s1 = 0; var_s1 < D_8018D1C8; var_s1++) {
        ObjectIndex = Objallocptr[var_s1];
        if (DynamicObjects[ObjectIndex].Anmptr == 0) {
            if (KwcheckMapareain(8, 9) != 0) {
                KwsetRndChoropu(ObjectIndex, 1);
            }
        } else {
            KwchartChoropuSub(ObjectIndex, 1);
        }
    }

    for (var_s1 = 0; var_s1 < D_8018D1D0; var_s1++) {
        ObjectIndex = Objallocptr2[var_s1];
        if (DynamicObjects[ObjectIndex].Anmptr == 0) {
            if (KwcheckMapareain(0x0010, 0x0013) != 0) {
                KwsetRndChoropu(ObjectIndex, 2);
            }
        } else {
            KwchartChoropuSub(ObjectIndex, 2);
        }
    }

    for (var_s1 = 0; var_s1 < D_8018D1D8; var_s1++) {
        ObjectIndex = Objallocptr3[var_s1];
        if (DynamicObjects[ObjectIndex].Anmptr == 0) {
            if (KwcheckMapareain(0x0011, 0x0014) != 0) {
                KwsetRndChoropu(ObjectIndex, 3);
            }
        } else {
            KwchartChoropuSub(ObjectIndex, 3);
        }
    }

    for (var_s1 = 0; var_s1 < gObjectParticle2_SIZE; var_s1++) {
        ObjectIndex = EffectAllocArray2[var_s1];
        if (DynamicObjects[ObjectIndex].Anmptr != 0) {
            KwanmTuchi(ObjectIndex);
        }
    }
}

void KwanmKamomeInit(s32 ObjectIndex, s32 arg1) {
    UNUSED s16 stackPadding0;
    s16 temp_f4;
    s16 sp22;
    s16 sp20;

    DynamicObjects[ObjectIndex].Texsizex = 1;
    DynamicObjects[ObjectIndex].Shape = (Gfx*) d_course_koopa_troopa_beach_unk4;
    DynamicObjects[ObjectIndex].Vtxaddr = (Vtx*) d_course_koopa_troopa_beach_unk_data5;
    DynamicObjects[ObjectIndex].Scale = 0.2f;
    DynamicObjects[ObjectIndex].Spptr = 1;
    sp22 = MakeRandomLimmit(0x00C8) + -100.0;
    sp20 = MakeRandomLimmit(0x0014);
    temp_f4 = MakeRandomLimmit(0x00C8) + -100.0;
    if (GameState == 9) {
        KwsetInit3DSpos(ObjectIndex, sp22 + -360.0, sp20 + 60.0, temp_f4 + -1300.0);
    } else if (DynamicObjects[ObjectIndex].Chartptr != 0) {
        KwsetInit3DSpos(ObjectIndex, (sp22 + 328.0) * XOrientation, sp20 + 20.0, temp_f4 + 2541.0);
    } else {
        KwsetInit3DSpos(ObjectIndex, (sp22 + -985.0) * XOrientation, sp20 + 15.0, temp_f4 + 1200.0);
    }
    KwsetInit3DAng(ObjectIndex, 0U, 0U, 0U);
    DynamicObjects[ObjectIndex].vel = 1.0f;
    KwmvStartStrategy(ObjectIndex);
    DynamicObjects[ObjectIndex].Sptbtopaddr = D_800E633C[arg1 % 4];
    KwanmSetflag(ObjectIndex, 0x00000800);
    KwanmNext(ObjectIndex);
}

void KwanmKamome(s32 ObjectIndex, s32 arg1) {
    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 1:
            KwanmKamomeInit(ObjectIndex, arg1);
            break;
        case 0:
        default:
            break;
    }
}

void KwmvKamome(s32 ObjectIndex) {
    UNUSED s32 stackPadding;
    switch (DynamicObjects[ObjectIndex].Spptr) { /* irregular */
        case 1:
            KwspStrategy3DLoop(ObjectIndex);
            KwsetSplinepos3D(ObjectIndex);
            break;
        case 2:
            KwspStrategy3DLoop(ObjectIndex);
            KwcopyFval(DynamicObjects[ObjectIndex].PosEnde, DynamicObjects[ObjectIndex].pos);
            HanyouGyakuSoukouSubBP(DynamicObjects[ObjectIndex].PosStart, (s16*) &DynamicObjects[ObjectIndex].Destangy,
                          DynamicObjects[ObjectIndex].vel, 0.0f, 0);
            DynamicObjects[ObjectIndex].PosSpline[0] *= 2.0;
            DynamicObjects[ObjectIndex].PosSpline[1] *= 2.5;
            DynamicObjects[ObjectIndex].PosSpline[2] *= 2.0;
            KwsetSplinepos3D(ObjectIndex);
            DynamicObjects[ObjectIndex].ang[1] =
                CalcDirection(DynamicObjects[ObjectIndex].PosEnde, DynamicObjects[ObjectIndex].pos);
            break;
    }
    KwchaseVelxztoangy(ObjectIndex);
}

void KwchartKamome(void) {
    AnmObject* object;
    UNUSED s32* var_s4;
    s32 temp_s0;
    s32 var_s3;

    for (var_s3 = 0; var_s3 < NUM_SEAGULLS; var_s3++) {
        temp_s0 = Objallocptr2[var_s3];

        object = &DynamicObjects[temp_s0];
        if (object->Anmptr == 0) {
            continue;
        }

        KwanmKamome(temp_s0, var_s3);
        KwmvKamome(temp_s0);
        if (KwanmBitcheckevflag(temp_s0, 2) != 0) {
            KwanmClrevflag(temp_s0, 2);
            if (D_80165A90 != 0) {
                D_80165A90 = 0;
                D_80183E40[0] = 0.0f;
                D_80183E40[1] = 0.0f;
                D_80183E40[2] = 0.0f;
                if (GameState != CREDITS_SEQUENCE) {
                    NAEnmTrgStart(object->pos, D_80183E40, SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x43));
                } else {
                    temp_s0 = Objallocptr2[1];
                    if (CutsceneShotTimer < 0x97) {
                        object = &DynamicObjects[temp_s0];
                        NAEnmTrgStart(object->pos, D_80183E40, SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x43));
                    }
                }
            }
        }
    }
    if (D_80165900 != 0) {
        D_80165900 -= 1;
    } else {
        if (GameState != 9) {
            D_80165900 = 0x003C;
        } else {
            D_80165900 = 0x000F;
        }
        if ((D_80165908 != 0) && (D_80165A90 == 0)) {
            D_80165A90 = 1;
        }
    }
    D_80165908 = 0;
}

void KwanmKaniInit(s32 ObjectIndex) {
    AnmObject* object;

    KwanmInitCI8(ObjectIndex, d_course_koopa_troopa_beach_crab_tlut,
                        (u8*) d_course_koopa_troopa_beach_crab_frames, 0x40U, (u16) 0x00000040);
    object = &DynamicObjects[ObjectIndex];
    object->Scale = 0.15f;
    object->Anmnumbak = 0;
    KwanmNext(ObjectIndex);
    object->r = 1;
    KwanmSetflag(ObjectIndex, 0x04000420);
    KwmvStartNormal(ObjectIndex, 0, 1);
    KwsetInit3DSppos(ObjectIndex, 0.0f, 0.0f, 0.0f);
    KwsetInit3DTexang(ObjectIndex, 0U, 0U, 0x8000U);
    object->vel = 1.5f;
    KwanmSetflag(ObjectIndex, 0x00000200);
}

void KwanmKani(s32 ObjectIndex, UNUSED s32 Unused) {
    switch (DynamicObjects[ObjectIndex].Anmptr) { /* irregular */
        case 0:
            break;
        case 1:
            KwanmKaniInit(ObjectIndex);
            break;
        case 2:
            KwanmForward(ObjectIndex, 0, 3, 1, 2, -1);
            break;
        case 3:
            KwanmForward(ObjectIndex, 4, 6, 1, 2, -1);
            break;
    }
    if (DynamicObjects[ObjectIndex].Anmptr >= 2) {
        KwanmTexaddrCI8Index(ObjectIndex);
    }
}

void KwmvKani(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Mvptr) {
        case 1:
            if (KwmvWalkxz2Pt(ObjectIndex, DynamicObjects[ObjectIndex].PosStart[0], DynamicObjects[ObjectIndex].PosEnde[0],
                              DynamicObjects[ObjectIndex].PosStart[2], DynamicObjects[ObjectIndex].PosEnde[2]) != 0) {
                KwanmJump(ObjectIndex, 3);
                KWAnmNext(ObjectIndex);
            }
            break;
        case 2:
            if (KwmvRandomwait(ObjectIndex, 0x003CU) != 0) {
                DynamicObjects[ObjectIndex].vel = 0.8f;
                KwanmJump(ObjectIndex, 2);
                KWAnmNext(ObjectIndex);
            }
            break;
        case 3:
            if (KwmvWalkxzReverse(ObjectIndex, 0x0000003C) != 0) {
                KWAnmNext(ObjectIndex);
                KwanmJump(ObjectIndex, 3);
            }
            break;
        case 4:
            if (KwmvRandomwait(ObjectIndex, 0x003CU) != 0) {
                KwanmJump(ObjectIndex, 2);
                KWAnmNext(ObjectIndex);
            }
            break;
        case 5:
            if (KwmvWalkxz(ObjectIndex, 0x0000003C) != 0) {
                KwanmJump(ObjectIndex, 3);
                KwmvJump(ObjectIndex, 2);
            }
            break;
    }
    KwsetSplinepos3D(ObjectIndex);
    if (KwanmBitcheckflag(ObjectIndex, VISIBLE) != 0) {
        KwanmSetonGround(ObjectIndex);
        DynamicObjects[ObjectIndex].pos[1] = (f32) (DynamicObjects[ObjectIndex].Ground + 2.5);
    }
}

void KwhitcheckKani(s32 ObjectIndex) {
    if (DynamicObjects[ObjectIndex].Anmptr >= 2) {
        KwcheckSlipKaniPlayer(ObjectIndex);
    }
}

void KwchartKani(void) {
    s32 ObjectIndex;
    s32 var_s1;

    for (var_s1 = 0; var_s1 < NUM_CRABS; var_s1++) {
        ObjectIndex = Objallocptr[var_s1];
        if (DynamicObjects[ObjectIndex].Anmptr != 0) {
            KwanmKani(ObjectIndex, var_s1);
            KwcheckVisibleAreaCamera(ObjectIndex, 500.0f);
            KwmvKani(ObjectIndex);
            KwhitcheckKani(ObjectIndex);
        }
    }
}

void KwanmGoalflagInit(s32 ObjectIndex, s32 arg1) {
    DynamicObjects[ObjectIndex].Shape = (Gfx*)d_course_yoshi_valley_unk5;
    DynamicObjects[ObjectIndex].Vtxaddr = (Vtx*)d_course_yoshi_valley_unk4;
    DynamicObjects[ObjectIndex].Scale = 0.027f;
    KwanmNext(ObjectIndex);
    KwsetInit3DSpos(ObjectIndex, D_800E5DF4[arg1 * 4 + 0] * XOrientation, D_800E5DF4[arg1 * 4 + 1], D_800E5DF4[arg1 * 4 + 2]);
    KwsetInit3DSppos(ObjectIndex, 0.0f, 0.0f, 0.0f);
    KwsetInit3DAng(ObjectIndex, 0U, D_800E5DF4[arg1 * 4 + 3], 0U);
}

void KwanmGoalflag(s32 ObjectIndex, s32 arg1) {
    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 1:
            KwanmGoalflagInit(ObjectIndex, arg1);
            break;
        case 0:
        default:
            break;
    }
}

void KwmvGoalflag(s32 ObjectIndex) {
    KwsetSplinepos3D(ObjectIndex);
}

void KwchartGoalflag(void) {
    s32 ObjectIndex;
    s32 var_s1;

    for (var_s1 = 0; var_s1 < NUM_YV_FLAG_POLES; var_s1++) {
        ObjectIndex = Objallocptr[var_s1];
        if (DynamicObjects[ObjectIndex].Anmptr != 0) {
            KwanmGoalflag(ObjectIndex, var_s1);
            KwmvGoalflag(ObjectIndex);
        }
    }
}

void KwanmHarinezumiInit(s32 ObjectIndex, s32 arg1) {
    AnmObject* object;

    KwanmInitCI8(ObjectIndex, d_course_yoshi_valley_hedgehog_tlut, d_course_yoshi_valley_hedgehog, 0x40U,
                        (u16) 0x00000040);
    object = &DynamicObjects[ObjectIndex];
    object->Texaddr = d_course_yoshi_valley_hedgehog_tlut;
    object->Indexaddr = d_course_yoshi_valley_hedgehog;
    object->Vtxaddr = Vtx_KTile64x64BL;
    object->Scale = 0.2f;
    object->Anmnumbak = 0;
    KwanmNext(ObjectIndex);
    KwsetInit3DSppos(ObjectIndex, 0.0f, 0.0f, 0.0f);
    KwsetInit3DTexang(ObjectIndex, 0U, 0U, 0x8000U);
    object->vel = ((arg1 % 6) * 0.1) + 0.5;
    KwmvStart(ObjectIndex);
    KwanmSetflag(ObjectIndex, 0x04000600);
    object->r = 2;
}

void KwmvHarinezumi(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Mvptr) {
        case 0:
            break;
        case 1:
            if (KwmvWalkxz2Pt(ObjectIndex, DynamicObjects[ObjectIndex].PosStart[0], DynamicObjects[ObjectIndex].ix,
                              DynamicObjects[ObjectIndex].PosStart[2], DynamicObjects[ObjectIndex].iy) != 0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 2:
            KwmvWaitnext(ObjectIndex, 0x0000003C);
            break;
        case 3:
            if (KwmvWalkxz2Pt(ObjectIndex, DynamicObjects[ObjectIndex].ix, DynamicObjects[ObjectIndex].PosStart[0],
                              DynamicObjects[ObjectIndex].iy, DynamicObjects[ObjectIndex].PosStart[2]) != 0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 4:
            if (KwmvWait(ObjectIndex, 0x0000003C) != 0) {
                KwmvJump(ObjectIndex, 1);
            }
            break;
    }
    KwsetSplinepos3D(ObjectIndex);
    if (KwanmBitcheckflag(ObjectIndex, 0x00200000) != 0) {
        if (KwanmBitcheckflag(ObjectIndex, 0x00400000) != 0) {
            KwanmSetonGroundbump(ObjectIndex);
        }
        DynamicObjects[ObjectIndex].pos[1] = DynamicObjects[ObjectIndex].Ground + 6.0;
    }
}

void KwanmHarinezumi(s32 ObjectIndex, s32 arg1) {
    switch (DynamicObjects[ObjectIndex].Anmptr) { /* irregular */
        case 0:
            break;
        case 1:
            KwanmHarinezumiInit(ObjectIndex, arg1);
            break;
        case 2:
            KwanmSwap(ObjectIndex, 0, 1, 4, -1);
            break;
    }
    if (DynamicObjects[ObjectIndex].Anmnumbak == 0) {
        DynamicObjects[ObjectIndex].Vtxaddr = Vtx_KTile64x64BL;
    } else {
        DynamicObjects[ObjectIndex].Vtxaddr = D_0D006130;
    }
}

void KwhitcheckHarinezumi(s32 ObjectIndex) {
    if (DynamicObjects[ObjectIndex].Anmptr >= 2) {
        KwcheckSlipKaniPlayer(ObjectIndex);
    }
}

void KwchartHarinezumi(void) {
    s32 temp_s1;
    s32 var_s0;

    for (var_s0 = 0; var_s0 < NUM_HEDGEHOGS; var_s0++) {
        temp_s1 = Objallocptr2[var_s0];
        KwanmHarinezumi(temp_s1, var_s0);
        KwmvHarinezumi(temp_s1);
        KwhitcheckHarinezumi(temp_s1);
    }
    KwclearAnybodyflag(Objallocptr2, 0x0000000F);
}

void KwinitSnowmanKakera(s32 ObjectIndex, Vec3f arg1, s32 Arg2, s32 Arg3) {
    AnmObject* object;

    KWAnmStart(ObjectIndex, 0);
    object = &DynamicObjects[ObjectIndex];
    object->Indexaddr = d_course_frappe_snowland_snow;
    object->Sindexaddr = d_course_frappe_snowland_snow;
    object->Texaddr = d_course_frappe_snowland_snow_tlut;
    object->Stexaddr = d_course_frappe_snowland_snow_tlut;
    object->Scale = MakeRandomLimmit(0x0064U);
    object->Scale = (object->Scale * 0.001) + 0.05;
    object->Velocity[1] = MakeRandomLimmit(0x0014U);
    object->Velocity[1] = (object->Velocity[1] * 0.5) + 2.6;
    object->vel = MakeRandomLimmit(0x000AU);
    object->vel = (object->vel * 0.1) + 4.5;
    object->ang[1] = (Arg2 << 0x10) / Arg3;
    object->PosStart[0] = arg1[0];
    object->PosStart[1] = arg1[1];
    object->PosStart[2] = arg1[2];
    object->Alpha = MakeRandomLimmit(0x4000U) + 0x1000;
}

void KwgetSnowmanKakera(Vec3f arg0) {
    s32 ObjectIndex;
    s32 i;

    for (i = 0; i < D_8018D3BC; i++) {
        ObjectIndex = KwallocEffectram(&EffectAllocArray2[0], &NextFreeObjectParticle2, gObjectParticle2_SIZE);
        if (ObjectIndex == NULL_OBJECT_ID) {
            break;
        }
        KwinitSnowmanKakera(ObjectIndex, arg0, i, D_8018D3BC);
    }
}

void KwanmSnowbomb(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 0:
            break;
        case 1:
            if (KwmvNanamenage(ObjectIndex, DynamicObjects[ObjectIndex].Velocity[1], 0.74f,
                              DynamicObjects[ObjectIndex].vel, DynamicObjects[ObjectIndex].ang[1],
                              0x00000064) != 0) {
                KwanmNext(ObjectIndex);
            }
            break;
        case 2:
            KwmvFinish(ObjectIndex);
            KwanmFinish(ObjectIndex);
            break;
    }
    KwsetSplinepos3D(ObjectIndex);
    DynamicObjects[ObjectIndex].Texang[2] += DynamicObjects[ObjectIndex].Alpha;
}

void KwanmSnowmanFaceInit(s32 ObjectIndex) {
    AnmObject* object;

    KwanmInitCI8(ObjectIndex, d_course_frappe_snowland_snowman_tlut, d_course_frappe_snowland_snowman_head,
                        0x40U, (u16) 0x00000040);
    object = &DynamicObjects[ObjectIndex];
    object->Vtxaddr = D_0D0061B0;
    object->Scale = 0.1f;
    object->Anmnumbak = 0;
    KwanmNext(ObjectIndex);
    KwsetInit3DSppos(ObjectIndex, 0.0f, 0.0f, 0.0f);
    object->Texang[0] = 0;
    object->Texang[1] = 0;
    object->Texang[2] = 0x8000;
    object->Alpha = MakeRandomLimmit(0x2000U) - 0x1000;
    KwmvStart(ObjectIndex);
    object->vel = 1.5f;
    KwanmSetflag(ObjectIndex, 0x00000200);
}

void KwmvSnowmanFace(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Mvptr) {
        case 1:
            KWAnmNext(ObjectIndex);
            break;
        case 2:
            KwmvWaitnext(ObjectIndex, 0x00000014);
            break;
        case 3:
            KwmvJump(ObjectIndex, 1);
            break;
        case 10:
            KwmvNageage(ObjectIndex, 10.0f, 0.5f, 0x0000000A);
            break;
        case 11:
            KwmvNageageGround(ObjectIndex, 0.0f, 0.2f, -7.0f);
            break;
        case 20:
            if (KwaddFval(&DynamicObjects[ObjectIndex].PosSpline[1], 0.0f, 0.2f) != 0) {
                KwsetsvalTrig(ObjectIndex, 0);
                KwmvJump(ObjectIndex, 1);
            }
            break;
        case 0:
        default:
            break;
    }
    KwsetSplinepos3D(ObjectIndex);
    KwsetsvalOufukuFRnds(ObjectIndex, &DynamicObjects[ObjectIndex].Alpha, -0x00001000, 0x00001000, 0x00000400, 1, -1);
    DynamicObjects[ObjectIndex].Texang[2] = DynamicObjects[ObjectIndex].Alpha + 0x8000;
}

void KwanmSnowmanFace(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 0:
            break;
        case 1:
            KwanmSnowmanFaceInit(ObjectIndex);
            break;
    }
    if (DynamicObjects[ObjectIndex].Anmptr >= 2) {
        KwanmTexaddrCI8Index(ObjectIndex);
    }
    KwmvSnowmanFace(ObjectIndex);
}

void KwanmSnowmanBodyInit(s32 ObjectIndex) {
    KwanmInitCI8(ObjectIndex, d_course_frappe_snowland_snowman_tlut, d_course_frappe_snowland_snowman_body,
                        0x40U, (u16) 0x00000040);
    DynamicObjects[ObjectIndex].Vtxaddr = Vtx_KTile64x64BL;
    DynamicObjects[ObjectIndex].Scale = 0.1f;
    DynamicObjects[ObjectIndex].Anmnumbak = 0;
    KwanmNext(ObjectIndex);
    KwsetInit3DSppos(ObjectIndex, 0.0f, 0.0f, 0.0f);
    DynamicObjects[ObjectIndex].Texang[0] = 0;
    DynamicObjects[ObjectIndex].Texang[1] = 0;
    DynamicObjects[ObjectIndex].Texang[2] = 0x8000;
    DynamicObjects[ObjectIndex].r = 2;
    DynamicObjects[ObjectIndex].vel = 1.5f;
    KwanmSetflag(ObjectIndex, 0x04000210);
}

void KwmvSnowmanBody(s32 ObjectIndex) {
    KwsetSplinepos3D(ObjectIndex);
}

void KwanmSnowmanBody(s32 ObjectIndex) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    switch (object->Anmptr) {
        case 0:
            break;
        case 1:
            KwanmSnowmanBodyInit(ObjectIndex);
            break;
        case 2:
            KwanmWait(ObjectIndex, 0x00000096);
            break;
        case 10:
            if (KwanmWait(ObjectIndex, 0x0000012C) != 0) {
                KwanmSetevflag(ObjectIndex, 2);
            }
            break;
        case 11:
            if (KwanmWait(ObjectIndex, 0x0000000A) != 0) {
                KwanmSetflag(ObjectIndex, 0x00000010);
                object->Scale = 0.001f;
            }
            break;
        case 12:
            if (KwsetfvalForward(ObjectIndex, &object->Scale, 0.001f, 0.1f, 0.0025f, 0, 0) != 0) {
                KwanmNext(ObjectIndex);
            }
            break;
        case 13:
            KwanmJump(ObjectIndex, 2);
            KwanmClrflag(ObjectIndex, 0x00001000);
            break;
    }
    if (object->Anmptr >= 2) {
        KwanmTexaddrCI8Index(ObjectIndex);
    }
    KwmvSnowmanBody(ObjectIndex);
}

void KwchartSnowman(void) {
    s32 var_s0;
    s32 var_s3;
    s32 var_s4;
    s32 ObjectIndex;
    AnmObject* object;

    for (var_s0 = 0; var_s0 < gObjectParticle2_SIZE; var_s0++) {
        ObjectIndex = EffectAllocArray2[var_s0];

        if (ObjectIndex == DELETED_OBJECT_ID) {
            continue;
        }

        if (DynamicObjects[ObjectIndex].Anmptr == 0) {
            continue;
        }
        KwanmSnowbomb(ObjectIndex);
        if (DynamicObjects[ObjectIndex].Anmptr != 0) {
            continue;
        }
        KwdeallocEffectram(&EffectAllocArray2[var_s0]);
        if (var_s0) {} // ??
    }

    for (var_s0 = 0; var_s0 < NUM_SNOWMEN; var_s0++) {
        var_s4 = Objallocptr[var_s0];
        var_s3 = Objallocptr2[var_s0];
        KwanmSnowmanFace(var_s3);
        KwanmSnowmanBody(var_s4);
        if (KwanmBitoffcheckflag(var_s4, 0x00001000) != 0) {
            object = &DynamicObjects[var_s4];
            if ((KwcheckMapareain(object->Chartptr - 1, object->Chartptr + 1) != 0) && (KwcheckBombPlayer2D(var_s4) != 0)) {
                KwanmSetflag(var_s4, 0x00001000);
                KwanmClrflag(var_s4, 0x00000010);
                KwanmJump(var_s4, 0x0000000A);
                KwmvJump(var_s3, 0x0000000A);
                KwgetSnowmanKakera(object->pos);
            }
        } else if (KwanmBitcheckevflag(var_s4, 2) != 0) {
            KwanmClrevflag(var_s4, 2);
            KwmvJump(var_s3, 0x00000014);
        }
    }
}

void KwanmIcebomb(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 0:
            break;
        case 1:
            if (KwmvNanamenage(ObjectIndex, DynamicObjects[ObjectIndex].Velocity[1], 0.12f,
                              DynamicObjects[ObjectIndex].vel, DynamicObjects[ObjectIndex].ang[1],
                              0x00000064) != 0) {
                KwanmNext(ObjectIndex);
            }
            KwsetSplinepos3D(ObjectIndex);
            break;
        case 2:
            KwmvFinish(ObjectIndex);
            KwanmFinish(ObjectIndex);
            break;
    }
}

void KwsetIcebomb(s32 ObjectIndex, s32 arg1, s32 PlayerId) {
    AnmObject* object;
    Player* sp20;

    object = &DynamicObjects[ObjectIndex];
    sp20 = &PlayerOne[PlayerId];
    object->Stack[7] = PlayerId;
    KWAnmStart(ObjectIndex, 0);
    object->Texaddr = d_course_sherbet_land_ice;
    object->Stexaddr = d_course_sherbet_land_ice;
    object->Scale = ((f32) MakeRandomLimmit(0x01F4U) * 0.0002) + 0.04;
    object->Velocity[1] = ((f32) MakeRandomLimmit(0x0032U) * 0.05) + 1.0;
    object->vel = ((f32) MakeRandomLimmit(0x000AU) * 0.1) + 1.0;
    object->ang[1] = D_801657A2 * arg1;
    object->PosStart[0] = (sp20->Position[0] + MakeRandomLimmit(0x0014U)) - 10.0f;
    object->PosStart[1] = (sp20->Position[1] - 10.0) + MakeRandomLimmit(0x000AU);
    object->PosStart[2] = (sp20->Position[2] + MakeRandomLimmit(0x0014U)) - 10.0f;
}

void KwgetIcekakera(UNUSED s32 arg0, s32 PlayerId) {
    s32 ObjectIndex;
    s32 var_s0;

    for (var_s0 = 0; var_s0 < D_8018D3C0; var_s0++) {
        ObjectIndex = KwallocEffectram(EffectAllocArray2, &NextFreeObjectParticle2, gObjectParticle2_SIZE);
        if (ObjectIndex == NULL_OBJECT_ID) {
            break;
        }
        KwsetIcebomb(ObjectIndex, var_s0, PlayerId);
    }
}

void KWChartIceBlockB(void) {
    s32 ObjectIndex;
    s32 PlayerId;

    D_80165834[0] += 0x200;
    D_80165834[1] += 0x400;
    KwcalcLightvector();
    D_8016582C[0] += 0x2000;
    D_8016582C[1] += 0x1000;
    D_8016582C[2] += 0x1800;
    for (PlayerId = 0; PlayerId < PlayerCount; PlayerId++) {
        ObjectIndex = Jugemuallocptr[PlayerId];
        if (KwanmBitcheckevflag(ObjectIndex, 0x00000020) != 0) {
            KwanmClrevflag(ObjectIndex, 0x00000020);
            KwgetIcekakera(ObjectIndex, PlayerId);
        }
    }
    for (PlayerId = 0; PlayerId < gObjectParticle2_SIZE; PlayerId++) {
        ObjectIndex = EffectAllocArray2[PlayerId];
        if (ObjectIndex != DELETED_OBJECT_ID) {
            if (DynamicObjects[ObjectIndex].Anmptr != 0) {
                KwanmIcebomb(ObjectIndex);
                if (DynamicObjects[ObjectIndex].Anmptr == 0) {
                    KwdeallocEffectram(&EffectAllocArray2[PlayerId]);
                }
            }
        }
    }
}

void KwanmKingpingInit(s32 ObjectIndex, UNUSED s32 arg1) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    object->Texsizex = 0;
    object->Shape = (Gfx*) d_course_sherbet_land_unk_data1;
    object->Vtxaddr = (Vtx*) d_course_sherbet_land_unk_data11;
    object->Scale = 0.2f;
    object->r = 0x000C;
    object->ix = 1;
    KwsetInit3DSpos(ObjectIndex, XOrientation * -383.0, 2.0f, -690.0f);
    KwsetInit3DAng(ObjectIndex, 0U, 0U, 0U);
    object->Spptr = 1;
    KwmvStartStrategy(ObjectIndex);
    object->Sptbtopaddr = D_800E672C[0];
    KwanmSetflag(ObjectIndex, 0x04000800);
    object->Output = CheckSkelanimeEnd(d_course_sherbet_land_unk_data11, 0);
    KwanmNext(ObjectIndex);
}

void KwanmKingping(s32 ObjectIndex, s32 arg1) {
    switch (DynamicObjects[ObjectIndex].Anmptr) { /* irregular */
        case 0:
            break;
        case 1:
            if (PlayerCount == 1) {
                KwanmKingpingInit(ObjectIndex, arg1);
            }
            break;
        case 2:
            KwanmForward(ObjectIndex, 0, DynamicObjects[ObjectIndex].Output, 1, 0, -1);
            break;
    }
}

void KwanmPingInit(s32 ObjectIndex, s32 arg1) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    object->Texsizex = 0;
    object->Shape = (Gfx*) d_course_sherbet_land_unk_data1;
    object->Vtxaddr = (Vtx*) d_course_sherbet_land_unk_data11;
    object->r = 4;
    object->ix = 2;
    object->Timer = MakeRandomLimmit(0x012CU);
    KwanmSetflag(ObjectIndex, 0x04000220);
    if ((arg1 > 0) && (arg1 < 9)) {
        if ((arg1 == 1) || (arg1 == 2)) {
            KwsetInit3DSpos(ObjectIndex, XOrientation * -2960.0, -80.0f, 1521.0f);
            object->Destangy = 0x0150;
            object->PosEnde[1] = 100.0f;
        } else if ((arg1 == 3) || (arg1 == 4)) {
            KwsetInit3DSpos(ObjectIndex, XOrientation * -2490.0, -80.0f, 1612.0f);
            object->Destangy = 0x0100;
            object->PosEnde[1] = 80.0f;
        } else if ((arg1 == 5) || (arg1 == 6)) {
            KwsetInit3DSpos(ObjectIndex, XOrientation * -2098.0, -80.0f, 1624.0f);
            object->Destangy = 0xFF00;
            object->PosEnde[1] = 80.0f;
        } else if ((arg1 == 7) || (arg1 == 8)) {
            KwsetInit3DSpos(ObjectIndex, XOrientation * -2080.0, -80.0f, 1171.0f);
            object->Destangy = 0x0150;
            object->PosEnde[1] = 80.0f;
        }
        object->Addangy = (arg1 << 0xF) & 0xFFFF;
        object->Ground = -80.0f;
        object->Scale = 0.08f;
        object->Spptr = 2;
        KwanmSetevflag(ObjectIndex, 8);
    } else if ((arg1 > 8) && (arg1 < 15)) {
        switch (arg1) { /* irregular */
            case 9:
                if (GameState != 9) {
                    KwsetInit3DSpos(ObjectIndex, XOrientation * 146.0, 0.0f, -380.0f);
                } else {
                    KwsetInit3DSpos(ObjectIndex, XOrientation * 380.0, 0.0f, -535.0f);
                    object->Scale = 0.15f;
                }
                object->Destangy = 0x9000;
                if (ScreenFlip != 0) {
                    object->Destangy -= 0x4000;
                }
                object->Spptr = 3;
                break;
            case 10:
                KwsetInit3DSpos(ObjectIndex, XOrientation * 380.0, 0.0f, -766.0f);
                object->Destangy = 0x5000;
                if (ScreenFlip != 0) {
                    object->Destangy += 0x8000;
                }
                object->Spptr = 4;
                break;
            case 11:
                KwsetInit3DSpos(ObjectIndex, XOrientation * -2300.0, 0.0f, -210.0f);
                object->Destangy = 0xC000;
                object->Spptr = 6;
                if (ScreenFlip != 0) {
                    object->Destangy += 0x8000;
                }
                break;
            case 12:
                KwsetInit3DSpos(ObjectIndex, XOrientation * -2500.0, 0.0f, -250.0f);
                object->Destangy = 0x4000;
                object->Spptr = 6;
                if (ScreenFlip != 0) {
                    object->Destangy += 0x8000;
                }
                break;
            case 13:
                KwsetInit3DSpos(ObjectIndex, XOrientation * -535.0, 0.0f, 875.0f);
                object->Destangy = 0x8000;
                object->Spptr = 6;
                if (ScreenFlip != 0) {
                    object->Destangy -= 0x4000;
                }
                break;
            case 14:
                KwsetInit3DSpos(ObjectIndex, XOrientation * -250.0, 0.0f, 953.0f);
                object->Destangy = 0x9000;
                object->Spptr = 6;
                if (ScreenFlip != 0) {
                    object->Destangy -= 0x4000;
                }
                break;
            default:
                break;
        }
        KwsetInit3DAng(ObjectIndex, 0U, object->Destangy + 0x8000, 0U);
        object->Ground = 5.0f;
        object->Scale = 0.04f;
        KwanmSetevflag(ObjectIndex, 0x00000014);
    }
    KwmvStartStrategy(ObjectIndex);
    object->vel = 0.0f;
    object->Output = CheckSkelanimeEnd(d_course_sherbet_land_unk_data11, 0);
    KwanmNext(ObjectIndex);
}

void KwanmPing(s32 ObjectIndex, s32 arg1) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    switch (object->Anmptr) {
        case 0:
            break;
        case 1:
            KwanmPingInit(ObjectIndex, arg1);
            break;
        case 2:
            KwanmForward(ObjectIndex, 0, (s32) object->Output, (s32) object->ix, 0, -1);
            if (KwanmBitoffcheckevflag(ObjectIndex, 0x00000020) != 0) {
                if (object->Stack[6] == 0) {
                    object->Stack[6] = MakeRandomLimmit(0x005AU) + 0x5A;
                    KwanmSetevflag(ObjectIndex, 0x00000080);
                } else {
                    object->Stack[6]--;
                }
            }
            break;
        case 3:
            KwanmForward(ObjectIndex, 0, object->Output, 1, 0, 0);
            break;
        case 4:
            KwanmClrevflag(ObjectIndex, 2);
            KwanmNext(ObjectIndex);
            break;
    }
    if (KwanmBitcheckevflag(ObjectIndex, 0x00000020) != 0) {
        if (object->Stack[6] == 0) {
            KwanmSetevflag(ObjectIndex, 0x00000080);
            object->Stack[6] = 0x0010;
        } else {
            object->Stack[6]--;
        }
    }
    if (KwanmBitcheckevflag(ObjectIndex, 0x00000080) != 0) {
        KwanmClrevflag(ObjectIndex, 0x00000080);
        if (KwanmBitcheckevflag(ObjectIndex, 0x00000010) != 0) {
            NAEnmTrgStart(object->pos, object->Velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x70, 0x49));
        } else {
            NAEnmTrgStart(object->pos, object->Velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x70, 0x17));
        }
    }
}

void KwmvPingSuberu(s32 ObjectIndex, s32 arg1) {
    f32 sp24;

    switch (DynamicObjects[ObjectIndex].Mvptr) {
        case 0:
            break;
        case 1:
            DynamicObjects[ObjectIndex].ang[1] =
                KwchaseAngle(DynamicObjects[ObjectIndex].ang[1], DynamicObjects[ObjectIndex].Destangy);
            if (DynamicObjects[ObjectIndex].ang[1] == DynamicObjects[ObjectIndex].Destangy) {
                DynamicObjects[ObjectIndex].ix = 4;
                DynamicObjects[ObjectIndex].vel = 0.4f;
                KWAnmNext(ObjectIndex);
            }
            break;
        case 2:
            KWChaseFVal(&DynamicObjects[ObjectIndex].vel, 0.8f, 0.02f);
            if (KwmvWait(ObjectIndex, 0x0000000F) != 0) {
                KwanmSetevflag(ObjectIndex, 1);
                KwanmSetevflag(ObjectIndex, 2);
                DynamicObjects[ObjectIndex].ix = 1;
                DynamicObjects[ObjectIndex].Texsizex = 1;
                DynamicObjects[ObjectIndex].Anmnumbak = 0;
                DynamicObjects[ObjectIndex].Output =
                    CheckSkelanimeEnd(d_course_sherbet_land_unk_data11, DynamicObjects[ObjectIndex].Texsizex);
                KwanmJump(ObjectIndex, 3);
                KWAnmNext(ObjectIndex);
                if (KwanmBitoffcheckevflag(ObjectIndex, 0x00000020) != 0) {
                    KwanmSetevflag(ObjectIndex, 0x00000080);
                }
            }
            break;
        case 3:
            switch (arg1) { /* switch 1; irregular */
                case 0:     /* switch 1 */
                    sp24 = 1.0f;
                    break;
                case 1: /* switch 1 */
                    sp24 = 1.5f;
                    break;
                case 2: /* switch 1 */
                    sp24 = 2.0f;
                    break;
                case 3: /* switch 1 */
                    sp24 = 2.5f;
                    break;
            }
            KWChaseFVal(&DynamicObjects[ObjectIndex].vel, sp24, 0.15f);
            if ((KwanmBitoffcheckevflag(ObjectIndex, 2) != 0) && (sp24 == DynamicObjects[ObjectIndex].vel)) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 4:
            if (KwmvWait(ObjectIndex, 0x0000001E) != 0) {
                KwanmClrevflag(ObjectIndex, 1);
                KWAnmNext(ObjectIndex);
            }
            break;
        case 5:
            KWChaseFVal(&DynamicObjects[ObjectIndex].vel, 0.4f, 0.2f);
            if (KwmvWait(ObjectIndex, 0x0000000A) != 0) {
                KwanmSetevflag(ObjectIndex, 2);
                DynamicObjects[ObjectIndex].Texsizex = 2;
                DynamicObjects[ObjectIndex].Anmnumbak = 0;
                DynamicObjects[ObjectIndex].Output =
                    CheckSkelanimeEnd(d_course_sherbet_land_unk_data11, DynamicObjects[ObjectIndex].Texsizex);
                KwanmJump(ObjectIndex, 3);
                KWAnmNext(ObjectIndex);
            }
            break;
        case 6:
            if (KwanmBitoffcheckevflag(ObjectIndex, 2) != 0) {
                DynamicObjects[ObjectIndex].Texsizex = 0;
                DynamicObjects[ObjectIndex].Anmnumbak = 0;
                DynamicObjects[ObjectIndex].Output =
                    CheckSkelanimeEnd(d_course_sherbet_land_unk_data11, DynamicObjects[ObjectIndex].Texsizex);
                DynamicObjects[ObjectIndex].Destangy += 0x8000;
                KwanmJump(ObjectIndex, 2);
                KwmvJump(ObjectIndex, 1);
            }
            break;
    }
    KwmvVelangytoposxz(ObjectIndex);
    KwsetSplinepos3D(ObjectIndex);
}

void func_80085024(void) {
}

void KwmvPingMawaru(s32 ObjectIndex, UNUSED s32 arg1) {
    KwmvCircle(ObjectIndex, DynamicObjects[ObjectIndex].PosEnde[1], DynamicObjects[ObjectIndex].Destangy);
    KwsetSplinepos3D(ObjectIndex);
    KwchaseVelxztoangy(ObjectIndex);
}

void KwmvPingSpline(s32 ObjectIndex) {
    KwspStrategy3DLoop(ObjectIndex);
    KwsetSplinepos3D(ObjectIndex);
    KwchaseVelxztoangy(ObjectIndex);
}

void KwmvPing(s32 ObjectIndex, s32 arg1) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    switch (object->Spptr) {
        case 1:
            KwmvPingSpline(ObjectIndex);
            break;
        case 2:
            KwmvPingMawaru(ObjectIndex, arg1);
            break;
        case 3:
            KwmvPingSuberu(ObjectIndex, 0);
            break;
        case 4:
            KwmvPingSuberu(ObjectIndex, 1);
            break;
        case 5:
            KwmvPingSuberu(ObjectIndex, 2);
            break;
        case 6:
            KwmvPingSuberu(ObjectIndex, 3);
            break;
    }
    if (KwanmBitcheckevflag(ObjectIndex, 0x00000020) != 0) {
        if (KwanmBitcheckevflag(ObjectIndex, 0x00000040) != 0) {
            KwanmClrevflag(ObjectIndex, 0x00000040);
            object->Stack[6] = 0;
            object->Stack[7] = 0x0096;
        }
        if (object->Stack[7] == 0) {
            KwanmClrevflag(ObjectIndex, 0x00000020);
        } else {
            object->Stack[7]--;
            object->Texang[0] = object->ang[0];
            object->Texang[1] += 0x2000;
            object->Texang[2] = object->ang[2];
        }
    } else {
        object->Texang[0] = object->ang[0];
        object->Texang[1] = object->ang[1];
        object->Texang[2] = object->ang[2];
    }
}

void KwchartPing(void) {
    UNUSED s32 var_s2;
    s32 ObjectIndex;
    s32 var_s1;

    for (var_s1 = 0; var_s1 < NUM_PENGUINS; var_s1++) {
        ObjectIndex = Objallocptr[var_s1];
        if (DynamicObjects[ObjectIndex].Anmptr != 0) {
            if (var_s1 == 0) {
                KwanmKingping(ObjectIndex, var_s1);
            } else {
                KwanmPing(ObjectIndex, var_s1);
            }
            KwmvPing(ObjectIndex, var_s1);
        }
        if (KwanmBitcheckevflag(ObjectIndex, 1) != 0) {
            KwcheckReboundsndPlayer2D(ObjectIndex, 1.75f, 1.5f, 0x1900A046U);
        } else if (KwanmBitcheckevflag(ObjectIndex, 8) != 0) {
            KwcheckReboundsndPlayer2D(ObjectIndex, 1.3f, 1.0f, 0x1900A046U);
        } else {
            KwcheckReboundsndPlayer2D(ObjectIndex, 1.5f, 1.25f, 0x1900A046U);
        }
        if ((KwanmBitcheckflag(ObjectIndex, 0x02000000) != 0) &&
            (KwanmBitoffcheckevflag(ObjectIndex, 0x00000020) != 0)) {
            KwanmSetevflag(ObjectIndex, 0x00000060);
            KwanmClrflag(ObjectIndex, 0x02000000);
        }
    }
}

void KwanmBalloonInit(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].Scale = 1.0f;
    DynamicObjects[ObjectIndex].Shape = d_course_luigi_raceway_dl_F960;
    if (GameState != 9) {
        KwsetInit3DSpos(ObjectIndex, XOrientation * -176.0, 0.0f, -2323.0f);
        KwsetInit3DSppos(ObjectIndex, 0.0f, 300.0f, 0.0f);
    } else {
        KwsetInit3DSpos(ObjectIndex, XOrientation * -1250.0, 0.0f, 1110.0f);
        KwsetInit3DSppos(ObjectIndex, 0.0f, 300.0f, 0.0f);
    }
    KwsetInit3DPos(ObjectIndex);
    KwanmSetkageParams(ObjectIndex);
    KwmvStartStrategy(ObjectIndex);
    DynamicObjects[ObjectIndex].Velocity[1] = -2.0f;
    InitialSpecialItembox(0.0f, 0.0f, 0.0f);
    KwanmNext(ObjectIndex);
}

void KwmvBalloon(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Mvptr) {
        case 1:
            if (DynamicObjects[ObjectIndex].PosSpline[1] <= 18.0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 2:
            KWChaseFVal(&DynamicObjects[ObjectIndex].Velocity[1], 0.0f, 0.05f);
            if (DynamicObjects[ObjectIndex].Velocity[1] == 0.0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 3:
            KwmvWaitnext(ObjectIndex, 1);
            break;
        case 4:
            KWChaseFVal(&DynamicObjects[ObjectIndex].Velocity[1], 1.0f, 0.05f);
            if (DynamicObjects[ObjectIndex].Velocity[1] == 1.0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 5:
            KwmvWaitnext(ObjectIndex, 0x0000005A);
            break;
        case 6:
            KWChaseFVal(&DynamicObjects[ObjectIndex].Velocity[1], 0.0f, 0.05f);
            if (DynamicObjects[ObjectIndex].Velocity[1] == 0.0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 7:
            KWChaseFVal(&DynamicObjects[ObjectIndex].Velocity[1], -1.0f, 0.05f);
            if (DynamicObjects[ObjectIndex].Velocity[1] == -1.0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 8:
            KwmvWaitnext(ObjectIndex, 0x0000005A);
            break;
        case 9:
            KWChaseFVal(&DynamicObjects[ObjectIndex].Velocity[1], 0.0f, 0.05f);
            if (KwmvWait(ObjectIndex, 0x0000005A) != 0) {
                KwmvJump(ObjectIndex, 3);
            }
            break;
    }
    KwmvAddvelytopos(ObjectIndex);
    DynamicObjects[ObjectIndex].ang[1] += 0x100;
}

void KwanmBalloon(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Anmptr) { /* irregular */
        case 1:
            KwanmBalloonInit(ObjectIndex);
            break;
        case 0:
        case 2:
            break;
    }
}

void KwchartBalloon(void) {
    s32 ObjectIndex;

    ObjectIndex = Objallocptr[0];
    if (DynamicObjects[ObjectIndex].Anmptr != 0) {
        KwanmBalloon(ObjectIndex);
        KwmvBalloon(ObjectIndex);
        KwsetSplinepos3D(ObjectIndex);
        if (DynamicObjects[ObjectIndex].Anmptr >= 2) {
            ActorHotAirBalloonItemBox->Position[0] = DynamicObjects[ObjectIndex].pos[0];
            ActorHotAirBalloonItemBox->Position[1] = DynamicObjects[ObjectIndex].pos[1] - 10.0;
            ActorHotAirBalloonItemBox->Position[2] = DynamicObjects[ObjectIndex].pos[2];
        }
    }
}

void KwanmWanwanInit(s32 ObjectIndex, s32 arg1) {
    CenterPathStruct* temp_v0;
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    object->Texsizex = 1;
    object->Shape = (Gfx*) d_rainbow_road_unk4;
    object->Vtxaddr = (Vtx*) d_rainbow_road_unk3;
    object->Scale = 0.03f;
    object->r = 0x000A;
    KwanmSetflag(ObjectIndex, 0x04000200);
    object->Stack[8] = (arg1 * 0x12C) + 0x1F4;
    KwsetInit3DSpos(ObjectIndex, 0.0f, -15.0f, 0.0f);
    temp_v0 = &PathPointPointer[(u16) object->Stack[8]];
    KwsetInit3DSppos(ObjectIndex, temp_v0->Pointx, temp_v0->Pointy, temp_v0->Pointz);
    KwsetInit3DAng(ObjectIndex, 0U, 0U, 0U);
    object->vel = 4.0f;
    object->Output = CheckSkelanimeEnd(d_rainbow_road_unk3, 0);
    KwanmNext(ObjectIndex);
}

void KwanmWanwan(s32 ObjectIndex, s32 arg1) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    switch (object->Anmptr) { /* irregular */
        case 0:
            break;
        case 1:
            KwanmWanwanInit(ObjectIndex, arg1);
            break;
        case 2:
            KwanmForward(ObjectIndex, 0, (s32) object->Output, 1, 0, -1);
            break;
    }
    if (D_8018D40C == 0) {
        NAEnmTrgStart(object->pos, object->Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x57));
    }
    KwsetfvalOufukuF(ObjectIndex, &object->Ground, -0.8f, 0.8f, 0.03f, 0, -1);
}

void KwchartWanwan(void) {
    s32 ObjectIndex;
    s32 var_s4;
    AnmObject* object;

    for (var_s4 = 0; var_s4 < NUM_CHAIN_CHOMPS; var_s4++) {
        ObjectIndex = Objallocptr2[var_s4];
        object = &DynamicObjects[ObjectIndex];
        if (object->Anmptr != 0) {
            KwanmWanwan(ObjectIndex, var_s4);
            KwcopyFval(object->PosEnde, object->PosSpline);
            HanyouGyakuSoukouSubBP(object->PosSpline, &object->Stack[8], object->vel, object->Ground, 0);
            object->ang[1] = CalcDirection(object->PosEnde, object->PosSpline);
            KwsetSplinepos3D(ObjectIndex);
            KwcheckBombPlayer3D(ObjectIndex, 30.0f);
        }
    }
}

void KwanmNeonCommonInit(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].Scale = 8.0f;
    KwsetInit3DSppos(ObjectIndex, 0.0f, 0.0f, 0.0f);
    KwsetInit3DTexang(ObjectIndex, 0U, 0U, 0x8000U);
    KwanmNext(ObjectIndex);
}

void KwanmNeonKiInit(s32 ObjectIndex) {
    KwsetInit3DSpos(ObjectIndex, XOrientation * -1431.0, 827.0f, -2957.0f);
    KwanmInitCI8(ObjectIndex, (u8*) d_course_rainbow_road_neon_mushroom_tlut_list,
                        d_course_rainbow_road_neon_mushroom, 0x40U, (u16) 0x00000040);
    KwanmNeonCommonInit(ObjectIndex);
}

void KwanmNeonKi(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 1:
            KwanmNeonKiInit(ObjectIndex);
            break;
        case 2:
            KwanmForward(ObjectIndex, 0, 4, 1, 0x0000000C, 5);
            break;
        case 3:
            KwanmSwap(ObjectIndex, 3, 4, 4, 0x0000000A);
            break;
        case 4:
            KwanmWait(ObjectIndex, 0x00000014);
            break;
        case 5:
            KwanmForward(ObjectIndex, 0, 4, 1, 0x0000000C, 5);
            break;
        case 6:
            KwanmWait(ObjectIndex, 0x00000014);
            break;
        case 7:
            KwanmSwap(ObjectIndex, 3, 4, 0, 0x00000014);
            break;
        case 8:
            KwanmJump(ObjectIndex, 2);
            break;
        case 0:
        default:
            break;
    }
}

void KwanmNeonMaInit(s32 ObjectIndex) {
    KwsetInit3DSpos(ObjectIndex, XOrientation * 799.0, 1193.0f, -5891.0f);
    KwanmInitCI8(ObjectIndex, (u8*) d_course_rainbow_road_neon_mario_tlut_list, d_course_rainbow_road_neon_mario,
                        0x40U, (u16) 0x00000040);
    KwanmNeonCommonInit(ObjectIndex);
}

void KwanmNeonMa(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 1:
            KwanmNeonMaInit(ObjectIndex);
            break;
        case 2:
            KwanmForward(ObjectIndex, 0, 4, 1, 0x0000000C, 1);
            break;
        case 3:
            KwanmSwap(ObjectIndex, 3, 4, 0x0000000C, 1);
            break;
        case 4:
            KwanmHidden(ObjectIndex, 0x0000000C);
            break;
        case 5:
            KwanmJump(ObjectIndex, 2);
            break;
        case 0:
        default:
            break;
    }
}

void KwanmNeonTeInit(s32 ObjectIndex) {
    KwsetInit3DSpos(ObjectIndex, XOrientation * -2013.0, 555.0f, 0.0f);
    KwanmInitCI8(ObjectIndex, (u8*) d_course_rainbow_road_neon_boo_tlut_list, d_course_rainbow_road_neon_boo,
                        0x40U, (u16) 0x00000040);
    KwanmNeonCommonInit(ObjectIndex);
}

void KwanmNeonTe(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 1:
            KwanmNeonTeInit(ObjectIndex);
            break;
        case 2:
            KwanmForward(ObjectIndex, 0, 4, 1, 5, 1);
            break;
        case 3:
            KwanmWait(ObjectIndex, 0x0000001E);
            break;
        case 4:
            KwanmFlash(ObjectIndex, 4, 0, 7);
            break;
        case 5:
            KwanmWait(ObjectIndex, 0x0000001E);
            break;
        case 6:
            KwanmReverse(ObjectIndex, 3, 0, 1, 5, 1);
            break;
        case 7:
            KwanmHidden(ObjectIndex, 0x0000000F);
            break;
        case 8:
            KwanmJump(ObjectIndex, 2);
            break;
        case 0:
        default:
            break;
    }
}

void KwanmNeonFixInit(s32 ObjectIndex, s32 arg1) {
    KwsetInit3DSpos(ObjectIndex, D_800E6734[arg1 * 3 + 0] * XOrientation, D_800E6734[arg1 * 3 + 1], D_800E6734[arg1 * 3 + 2]);
    KwanmInitCI8(ObjectIndex, &d_course_rainbow_road_static_tluts[arg1 * 256],
                        &d_course_rainbow_road_static_textures[arg1], 64, 64);
    KwanmNeonCommonInit(ObjectIndex);
}

void KwanmNeonFix(s32 ObjectIndex, s32 arg1) {
    switch (DynamicObjects[ObjectIndex].Anmptr) {
        case 1:
            KwanmNeonFixInit(ObjectIndex, arg1);
            break;
        case 0:
            break;
    }
}

void KwanmNeon(s32 ObjectIndex, s32 id) {
    switch (id) { /* irregular */
        case 0:
            KwanmNeonKi(ObjectIndex);
            break;
        case 1:
            KwanmNeonMa(ObjectIndex);
            break;
        case 2:
            KwanmNeonTe(ObjectIndex);
            break;
    }
    if (id >= 3) {
        KwanmNeonFix(ObjectIndex, id - 3);
    }
}

void KwchartNeon(void) {
    s32 ObjectIndex;
    s32 id;

    for (id = 0; id < NUM_NEON_SIGNS; id++) {
        ObjectIndex = Objallocptr[id];
        if (DynamicObjects[ObjectIndex].Anmptr != 0) {
            KwanmNeon(ObjectIndex, id);
            if (DynamicObjects[ObjectIndex].Anmptr >= 2) {
                KwanmTexaddrCI8Colpal(ObjectIndex);
                KwsetSplinepos3D(ObjectIndex);
            }
        }
    }
}

void KwanmDaiInit(s32 ObjectIndex, s32 arg1) {
    switch (arg1) { /* irregular */
        case 0:
            DynamicObjects[ObjectIndex].Shape = podium_dl3;
            DynamicObjects[ObjectIndex].Timer = 0x00000038;
            break;
        case 1:
            DynamicObjects[ObjectIndex].Shape = podium2_dl3;
            DynamicObjects[ObjectIndex].Timer = 0x0000002B;
            break;
        case 2:
            DynamicObjects[ObjectIndex].Shape = podium3_dl3;
            DynamicObjects[ObjectIndex].Timer = 0x0000001E;
            break;
        default:
            break;
    }
    DynamicObjects[ObjectIndex].Scale = 1.0f;
    KwsetInit3DSpos(ObjectIndex, D_800E634C[0][0] - 1.5, D_800E634C[0][1], D_800E634C[0][2]);
    KwsetInit3DSppos(ObjectIndex, 0.0f, -10.0f, 0.0f);
    KwsetInit3DAng(ObjectIndex, 0U, 0xF8E4U, 0U);
    DynamicObjects[ObjectIndex].Ival = 0;
    KwanmNext(ObjectIndex);
}

void KwmvDai(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Mvptr) {
        case 0:
            break;
        case 1:
            DynamicObjects[ObjectIndex].Velocity[1] = 0.75f;
            KWAnmNext(ObjectIndex);
            break;
        case 2:
            if (DynamicObjects[ObjectIndex].PosSpline[1] >= -2.0) {
                DynamicObjects[ObjectIndex].Velocity[1] -= 0.1;
            }
            KwmvAddvelytopos(ObjectIndex);
            if (DynamicObjects[ObjectIndex].PosSpline[1] >= 0.0) {
                DynamicObjects[ObjectIndex].PosSpline[1] = 0.0f;
                DynamicObjects[ObjectIndex].Velocity[1] = 0.0f;
                KwmvFinish(ObjectIndex);
            }
            break;
    }
    KwsetSplinepos3D(ObjectIndex);
}

void KwanmDai(s32 ObjectIndex, s32 arg1) {
    switch (DynamicObjects[ObjectIndex].Anmptr) { /* irregular */
        case 1:
            KwanmDaiInit(ObjectIndex, arg1);
            break;
        case 2:
            if (KwanmWait(ObjectIndex, DynamicObjects[ObjectIndex].Timer) != 0) {
                ResultKartJumpStart(arg1);
                KwmvStart(ObjectIndex);
                KwanmNext(ObjectIndex);
            }
            break;
        case 0:
            break;
        case 3:
            if (DynamicObjects[ObjectIndex].Mvptr == 0) {
                DynamicObjects[ObjectIndex].Ival = 1;
                KwanmNext(ObjectIndex);
            }
            break;
    }
}

void KwchartDai(void) {
    s32 ObjectIndex;
    s32 var_s1;

    if ((OgaHyousyouDemoTrg != 0) && (Resultstat.Rank < 3)) {
        if (D_801658C6 == 0) {
            for (var_s1 = 0; var_s1 < 3; var_s1++) {
                ObjectIndex = Objallocptr[var_s1];
                KWAnmStart(ObjectIndex, 0);
            }
            D_801658C6 = 1;
        }
    }
    for (var_s1 = 0; var_s1 != 3; var_s1++) {
        ObjectIndex = Objallocptr[var_s1];
        if (DynamicObjects[ObjectIndex].Anmptr != 0) {
            KwanmDai(ObjectIndex, var_s1);
            KwmvDai(ObjectIndex);
        }
    }
}

void KwanmCupInit(s32 ObjectIndex) {
    if (RaceClass < CC_150) {
        switch (Resultstat.Rank) { /* switch 1; irregular */
            case 0:                 /* switch 1 */
                DynamicObjects[ObjectIndex].Shape = gold_trophy_dl10;
                break;
            case 1: /* switch 1 */
                DynamicObjects[ObjectIndex].Shape = gold_trophy_dl12;
                break;
            case 2: /* switch 1 */
                DynamicObjects[ObjectIndex].Shape = gold_trophy_dl14;
                break;
            default: /* switch 1 */
                break;
        }
    } else {
        switch (Resultstat.Rank) { /* irregular */
            case 0:
                DynamicObjects[ObjectIndex].Shape = gold_trophy_dl11;
                break;
            case 1:
                DynamicObjects[ObjectIndex].Shape = gold_trophy_dl13;
                break;
            case 2:
                DynamicObjects[ObjectIndex].Shape = gold_trophy_dl15;
                break;
            default:
                break;
        }
    }
    DynamicObjects[ObjectIndex].Scale = 0.005f;
    KwsetInit3DSpos(ObjectIndex, DynamicObjects[Objallocptr2[0]].pos[0],
                       DynamicObjects[Objallocptr2[0]].pos[1] + 16.0, DynamicObjects[Objallocptr2[0]].pos[2]);
    KwsetInit3DSppos(ObjectIndex, 0.0f, 0.0f, 0.0f);
    KwsetInit3DAng(ObjectIndex, 0U, 0U, 0U);
    DynamicObjects[ObjectIndex].Stack[1] = 0x0200;
    KwanmNext(ObjectIndex);
    KwmvStart(ObjectIndex);
}

void KwmvCup(s32 ObjectIndex) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    switch (object->Mvptr) {
        case 0:
            break;
        case 1:
            KWAnmNext(ObjectIndex);
            break;
        case 2:
            KWChaseFVal(&object->Scale, 0.025f, 0.001f);
            KwmvNageage(ObjectIndex, 6.0f, 0.1f, 0x000000C8);
            if ((f64) object->Velocity[1] <= 0.0) {
                KwmvJump(ObjectIndex, 3);
            }
            break;
        case 3:
            KwmvWaitnext(ObjectIndex, 0x00000064);
            break;
        case 4:
            D_801658D6 = 1;
            object->Velocity[1] = -0.4f;
            KWAnmNext(ObjectIndex);
            object->PosStart[1] = 90.0f;
            object->PosSpline[1] = 60.0f;
            switch (Resultstat.Rank) { /* switch 1; irregular */
                case 1:                 /* switch 1 */
                    object->PosStart[0] -= 3.0;
                    object->PosStart[2] += 15.0;
                    break;
                case 2: /* switch 1 */
                    object->PosStart[0] -= 2.0;
                    object->PosStart[2] -= 15.0;
                    break;
            }
            break;
        case 5:
            if ((f64) object->PosSpline[1] <= 8.0) {
                KWChaseFVal(&object->Velocity[1], -0.1f, -0.01f);
            }
            KwmvAddvelytopos(ObjectIndex);
            if ((f64) object->PosSpline[1] <= 0.0) {
                KWAnmNext(ObjectIndex);
            }
            break;
        case 6:
            if (KwmvWaitnext(ObjectIndex, 0x00000041) != 0) {
                D_801658F4 = 1;
            }
            break;
        case 7:
            if (KwmvWaitnext(ObjectIndex, 0x00000064) != 0) {
                StartKawanoResult();
                KwmvFinish(ObjectIndex);
            }
            break;
    }
    if (D_801658D6 != 0) {
        object->ang[0] += 0x400;
        object->ang[1] = 0xE800;
        object->ang[2] = 0xDA00;
    } else {
        object->ang[0] += 0x400;
        object->ang[1] -= 0x200;
    }
    KwsetSplinepos3D(ObjectIndex);
}

void KwanmCup(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Anmptr) { /* irregular */
        case 1:
            KwanmCupInit(ObjectIndex);
            break;
        case 0:
        case 2:
            break;
    }
}

void KwgetCupStar(s32 ObjectIndex) {
    Vec3f sp24;

    sp24[0] = (DynamicObjects[ObjectIndex].pos[0] - 5.0f) + MakeRandomLimmit(0x000AU);
    sp24[2] = (DynamicObjects[ObjectIndex].pos[2] - 5.0f) + MakeRandomLimmit(0x000AU);
    if (D_801658F4 != 0) {
        sp24[1] = DynamicObjects[ObjectIndex].pos[1] + 14.0;
    } else {
        sp24[1] = DynamicObjects[ObjectIndex].pos[1] - 2.0;
    }
    KWGetStar(sp24, (s32) D_801658F4);
}

void KwchartCup(void) {
    s32 temp_s2;
    s32 var_s0;

    if ((D_801658CE != 0) && (D_801658DC == 0)) {
        temp_s2 = Objallocptr[3];
        KWAnmStart(temp_s2, 0);
        D_801658DC = 1;
    }
    temp_s2 = Objallocptr[3];
    if (DynamicObjects[temp_s2].Anmptr != 0) {
        KwanmCup(temp_s2);
        KwmvCup(temp_s2);
        if (D_801658F4 != 0) {
            if (KW2GFTimer == 0) {
                KwgetCupStar(temp_s2);
            }
        } else {
            for (var_s0 = 0; var_s0 < 2; var_s0++) {
                KwgetCupStar(temp_s2);
            }
        }
    }
}

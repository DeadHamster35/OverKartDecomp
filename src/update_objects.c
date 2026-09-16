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
#include "OverKartHooks.h"

//! @todo unused?
f32 D_800E43B0[] = { 65536.0, 0.0, 1.0, 0.0, 0.0, 65536.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

Vtx dvtx_ktile48x40[] = {
    { { { -24, -19, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, -19, 0 }, 0, { 3008, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, 20, 0 }, 0, { 3008, 2496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -24, 20, 0 }, 0, { 0, 2496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -24, -19, 0 }, 0, { 3008, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, -19, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, 20, 0 }, 0, { 0, 2496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -24, 20, 0 }, 0, { 3008, 2496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

Vtx dvtx_tellesa48x40[] = {
    { { { -24, -19, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, -19, 0 }, 0, { 3008, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, 20, 0 }, 0, { 3008, 2496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -24, 20, 0 }, 0, { 0, 2496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

Vtx dvtx_tellesa48x40xflip[] = {
    { { { -24, -19, 0 }, 0, { 3008, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, -19, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 23, 20, 0 }, 0, { 0, 2496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -24, 20, 0 }, 0, { 3008, 2496 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

u16* itemboxtbl_pal[] = { common_tlut_item_window_none,
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

u8* itemboxtbl_idx[] = { common_texture_item_window_none,
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

u16* lap_list[] = { common_texture_hud_lap_1_on_3, common_texture_hud_lap_2_on_3,
                           common_texture_hud_lap_3_on_3 };

u16* kao_colpal_list[] = { common_tlut_portrait_mario, common_tlut_portrait_luigi,       common_tlut_portrait_yoshi,
                          common_tlut_portrait_toad,  common_tlut_portrait_donkey_kong, common_tlut_portrait_wario,
                          common_tlut_portrait_peach, common_tlut_portrait_bowser };

u8* kao_index_list[] = { common_texture_portrait_mario,       common_texture_portrait_luigi,
                            common_texture_portrait_yoshi,       common_texture_portrait_toad,
                            common_texture_portrait_donkey_kong, common_texture_portrait_wario,
                            common_texture_portrait_peach,       common_texture_portrait_bowser };

s32 kwalloc(s32* arg0) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = objectListSize;
    // clang-format off
    temp_v0 = 0; do { // this two SHOULD be on the same line
        // clang-format on
        ++temp_v1;
        ++temp_v0;

        if (temp_v1 == OBJECT_LIST_SIZE) {
            temp_v1 = 0;
        }
    } while ((g_DynamicObjects[temp_v1].anmtrig != 0) && (temp_v0 != OBJECT_LIST_SIZE));

    g_DynamicObjects[temp_v1].anmtrig = 1;

    *arg0 = temp_v1;
    objectListSize = temp_v1;
    return temp_v1;
}

//! @warning Does not clear struct members.
void kwdealloc(s32* objectIndex) {
    kwanm_finish(*objectIndex);
    g_DynamicObjects[*objectIndex].anmtrig = 0;
    *objectIndex = NULL_OBJECT_ID;
}

s32 kwcheck_allocval(void) {
    s32 objectIndex;
    s32 someCount = 0;
    for (objectIndex = 0; objectIndex < OBJECT_LIST_SIZE; objectIndex++) {
        if (g_DynamicObjects[objectIndex].anmtrig != 0) {
            someCount++;
        }
    }
    return someCount;
}

s32 kwalloc_effectram(s32* listIdx, s32* nextFree, s32 size) {
    s32 count;
    s32 objectIndex;
    s32* id;

    if (*nextFree >= size) {
        *nextFree = 0;
    }
    count = 0;
    id = &listIdx[*nextFree];

    for (count = 0; count < size; count++) {
        if (*id == NULL_OBJECT_ID) {
            objectIndex = kwalloc(id);
            *nextFree += 1;
            break;
        } else {
            *nextFree += 1;
            if (*nextFree >= size) {
                *nextFree = 0;
            }
            id = &listIdx[*nextFree];
        }
    }
    if (count == size) {
        objectIndex = NULL_OBJECT_ID;
    }
    return objectIndex;
}

void kwdealloc_effectram(s32* arg0) {
    kwdealloc(arg0);
}

void kwclear_anybodyflag(s32* arg0, s32 arg1) {
    s32 i;

    for (i = 0; i < arg1; i++) {
        kwanm_clrflag(*arg0, 0x00600000);
        arg0++;
    }
}

void kwlost_ghost(void) {
    if (g_gameMode == TIME_TRIALS) {
        if (((gPlayerOne->flag & EXISTS) != 0) &&
            ((gPlayerOne->flag & (IS_GHOST | IS_CPU_PLAYER)) == 0)) {
            lost_ghost = 1;
        }
    }
}

void kwanm_setflag(s32 objectIndex, s32 flag) {
    g_DynamicObjects[objectIndex].flag |= flag;
}

void kwanm_clrflag(s32 objectIndex, s32 flag) {
    g_DynamicObjects[objectIndex].flag &= ~flag;
}

UNUSED void func_80072214(s32 objectIndex, s32 arg1) {
    g_DynamicObjects[objectIndex].flag ^= arg1;
}

bool kwanm_bitcheckflag(s32 objectIndex, s32 arg1) {
    s32 phi_v1 = false;
    if ((g_DynamicObjects[objectIndex].flag & arg1) != 0) {
        phi_v1 = true;
    }
    return phi_v1;
}

s32 kwanm_bitoffcheckflag(s32 objectIndex, s32 arg1) {
    s32 phi_v1 = 0;
    if ((g_DynamicObjects[objectIndex].flag & arg1) == 0) {
        phi_v1 = 1;
    }
    return phi_v1;
}

void kwanm_setevflag(s32 objectIndex, s32 arg1) {
    g_DynamicObjects[objectIndex].evflag |= arg1;
}

void kwanm_clrevflag(s32 objectIndex, s32 arg1) {
    g_DynamicObjects[objectIndex].evflag &= ~arg1;
}

UNUSED void func_800722F8(s32 objectIndex, s32 arg1) {
    g_DynamicObjects[objectIndex].evflag ^= arg1;
}

bool kwanm_bitcheckevflag(s32 objectIndex, s32 arg1) {
    s32 b = false;
    if ((g_DynamicObjects[objectIndex].evflag & arg1) != 0) {
        b = true;
    }
    return b;
}

bool kwanm_bitoffcheckevflag(s32 objectIndex, s32 arg1) {
    s32 b = false;
    if ((g_DynamicObjects[objectIndex].evflag & arg1) == 0) {
        b = true;
    }
    return b;
}

void kwanm_trig(s32 objectIndex, s32 arg1) {
    g_DynamicObjects[objectIndex].anmloop = arg1;
}

void KWAnmStart(s32 objectIndex, s32 arg1) {
    g_DynamicObjects[objectIndex].flag = 0;
    g_DynamicObjects[objectIndex].evflag = 0;
    g_DynamicObjects[objectIndex].evtrflag = 0;
    g_DynamicObjects[objectIndex].fvalloop = 0;
    g_DynamicObjects[objectIndex].svalloop = 0;
    kwanm_trig(objectIndex, 0);
    g_DynamicObjects[objectIndex].texsizex = arg1;
    g_DynamicObjects[objectIndex].anmptr = 1;
}

UNUSED void kwanm_break(s32 objectIndex) {
    kwanm_trig(objectIndex, 0);
}

void kwanm_finish(s32 objectIndex) {
    g_DynamicObjects[objectIndex].anmptr = 0;
    g_DynamicObjects[objectIndex].texsizex = 0;
    kwanm_trig(objectIndex, 0);
    g_DynamicObjects[objectIndex].flag = 0;
    g_DynamicObjects[objectIndex].evflag = 0;
    g_DynamicObjects[objectIndex].evtrflag = 0;
    kwmv_finish(objectIndex);
}

void kwanm_next(s32 objectIndex) {
    kwanm_trig(objectIndex, 0);
    kwanm_clrflag(objectIndex, 0x2000);
    g_DynamicObjects[objectIndex].anmptr++;
}

void kwanm_clearstack(s32 objectIndex) {
    g_DynamicObjects[objectIndex].anmtype = 0;
}

void kwanm_pushptr(s32 objectIndex, s32 arg1) {
    g_DynamicObjects[objectIndex].stack[g_DynamicObjects[objectIndex].anmtype] = arg1;
    g_DynamicObjects[objectIndex].anmtype++;
}

s16 kwanm_pullptr(s32 objectIndex) {
    g_DynamicObjects[objectIndex].anmtype--;
    return g_DynamicObjects[objectIndex].stack[g_DynamicObjects[objectIndex].anmtype];
}

void kwanm_gosubnext(s32 objectIndex, s32 arg1) {
    kwanm_trig(objectIndex, 0);
    kwanm_clrflag(objectIndex, 0x2000);
    kwanm_clrflag(objectIndex, 8);
    kwanm_next(objectIndex);
    kwanm_pushptr(objectIndex, g_DynamicObjects[objectIndex].anmptr);
    g_DynamicObjects[objectIndex].anmptr = arg1;
}

void kwanm_gosub(s32 objectIndex, s32 arg1, s32 arg2) {
    kwanm_trig(objectIndex, 0);
    kwanm_clrflag(objectIndex, 0x2000);
    kwanm_clrflag(objectIndex, 8);
    g_DynamicObjects[objectIndex].anmptr = arg2;
    kwanm_pushptr(objectIndex, g_DynamicObjects[objectIndex].anmptr);
    g_DynamicObjects[objectIndex].anmptr = arg1;
}

s16 kwanm_pullptr(s32); // extern

void kwanm_return(s32 objectIndex) {
    kwanm_trig(objectIndex, 0);
    kwanm_clrflag(objectIndex, 0x2000);
    kwanm_clrflag(objectIndex, 8);
    g_DynamicObjects[objectIndex].anmptr = kwanm_pullptr(objectIndex);
}

void kwanm_jump(s32 objectIndex, s32 arg1) {
    kwanm_trig(objectIndex, 0);
    kwanm_clrflag(objectIndex, 0x2000);
    g_DynamicObjects[objectIndex].anmptr = arg1;
}

UNUSED void kwanm_trigjump(s32 objectIndex, s32 arg1) {
    if (g_DynamicObjects[objectIndex].anmloop == 0) {
        g_DynamicObjects[objectIndex].anmptr = arg1;
    }
}

UNUSED void kwanm_waittrig(s32 objectIndex) {
    if (g_DynamicObjects[objectIndex].anmloop == 0) {
        kwanm_next(objectIndex);
    }
}

/**
 * @brief will return true if the timer is done
 */
bool kwanm_wait(s32 objectIndex, s32 timer) {
    bool phi_v1;

    phi_v1 = false;
    if (g_DynamicObjects[objectIndex].anmloop == false) {
        kwanm_trig(objectIndex, true);
        g_DynamicObjects[objectIndex].anmtm = timer;
    }

    g_DynamicObjects[objectIndex].anmtm--;
    if (g_DynamicObjects[objectIndex].anmtm < 0) {
        kwanm_trig(objectIndex, false);
        kwanm_next(objectIndex);
        phi_v1 = true;
    }

    return phi_v1;
}

UNUSED s32 kwanm_randomwait(s32 objectIndex, s32 arg1) {
    s32 phi_a2;

    phi_a2 = 0;
    if (g_DynamicObjects[objectIndex].anmloop == 0) {
        kwanm_trig(objectIndex, 1);
        g_DynamicObjects[objectIndex].anmtm = MakeRandomLimmit((u16) arg1);
    }

    g_DynamicObjects[objectIndex].anmtm--;
    if (g_DynamicObjects[objectIndex].anmtm < 0) {
        kwanm_trig(objectIndex, 0);
        kwanm_next(objectIndex);
        phi_a2 = 1;
    }

    return phi_a2;
}

UNUSED s32 kwanm_brandomwait(s32 objectIndex, s32 arg1, s32 arg2) {
    s32 phi_a3;

    phi_a3 = 0;
    if (g_DynamicObjects[objectIndex].anmloop == 0) {
        kwanm_trig(objectIndex, 1);
        g_DynamicObjects[objectIndex].anmtm = MakeRandomLimmit((u16) arg2) + arg1;
    }

    g_DynamicObjects[objectIndex].anmtm--;
    if (g_DynamicObjects[objectIndex].anmtm < 0) {
        kwanm_trig(objectIndex, 0);
        kwanm_next(objectIndex);
        phi_a3 = 1;
    }

    return phi_a3;
}

void kwanm_ifequjump(s32 objectIndex, s32 arg1, s32 arg2, s32 arg3) {
    if (arg1 == arg2) {
        g_DynamicObjects[objectIndex].anmptr = arg3;
    }
}

UNUSED void kwanm_ifnequjump(s32 objectIndex, s32 arg1, s32 arg2, s32 arg3) {
    if (arg1 != arg2) {
        g_DynamicObjects[objectIndex].anmptr = arg3;
    }
}

UNUSED void kwanm_loopstop(s32 objectIndex) {
    g_DynamicObjects[objectIndex].fvaltrig = 0;
}

void kwanm_setinput(s32 objectIndex, s32 arg1) {
    g_DynamicObjects[objectIndex].input = arg1;
}

void kwanm_setoutput(s32 objectIndex, s32 arg1) {
    g_DynamicObjects[objectIndex].output = arg1;
}

void kwanm_racestart(s32 objectIndex) {
    u32 temp_v1 = 1;
    s32 i;

    start_racing();
    kwanm_next(objectIndex);
    D_8018D2BC = 1;
    D_8018D2A4 = 1;

    if (g_courseID != COURSE_YOSHI_VALLEY) {
        for (i = 0; i < g_menuMultiplayerSelection; i++) {
            playerHUD[i].ranksw = temp_v1;
        }
    }
    kwchart_opening_panel_start();
}

UNUSED void kwanm_setnum(s32 objectIndex, s32 arg1) {
    g_DynamicObjects[objectIndex].anmnumbak = arg1;
    kwanm_next(objectIndex);
}

s32 kwanm_setnumwait(s32 objectIndex, s32 arg1, s32 arg2) {
    s32 phi_v1;

    phi_v1 = 0;
    if (g_DynamicObjects[objectIndex].anmloop == 0) {

        kwanm_trig(objectIndex, 1);
        g_DynamicObjects[objectIndex].anmnumbak = arg1;
        g_DynamicObjects[objectIndex].anmtm = arg2;
    }

    g_DynamicObjects[objectIndex].anmtm--;
    if (g_DynamicObjects[objectIndex].anmtm < 0) {
        kwanm_trig(objectIndex, 0);
        kwanm_next(objectIndex);
        phi_v1 = 1;
    }

    return phi_v1;
}

s32 kwanm_hidden(s32 objectIndex, s32 arg1) {
    s32 phi_v1;

    phi_v1 = 0;
    if (g_DynamicObjects[objectIndex].anmloop == 0) {
        kwanm_trig(objectIndex, 1);
        kwanm_setflag(objectIndex, 0x80000);
        g_DynamicObjects[objectIndex].anmnumbak = D_8018D140;
        g_DynamicObjects[objectIndex].anmtm = arg1;
    }

    g_DynamicObjects[objectIndex].anmtm--;
    if (g_DynamicObjects[objectIndex].anmtm < 0) {
        kwanm_clrflag(objectIndex, 0x80000);
        kwanm_trig(objectIndex, 0);
        kwanm_next(objectIndex);
        phi_v1 = 1;
    }

    return phi_v1;
}

void kwanm_flash(s32 objectIndex, s32 arg1, s32 arg2, s32 arg3) {
    if (kwanm_bitoffcheckflag(objectIndex, 0x2000) != 0) {
        g_DynamicObjects[objectIndex].anmtm = arg2;
        g_DynamicObjects[objectIndex].anmnumbak = arg1;
        g_DynamicObjects[objectIndex].type = 1;
        g_DynamicObjects[objectIndex].fvaltrig = arg3;
        kwanm_setflag(objectIndex, 0x2000);
    } else {
        g_DynamicObjects[objectIndex].anmtm--;
        if (g_DynamicObjects[objectIndex].anmtm < 0) {
            g_DynamicObjects[objectIndex].anmtm = arg2;
            g_DynamicObjects[objectIndex].type--;
            if ((g_DynamicObjects[objectIndex].type & 1) != 0) {
                kwanm_clrflag(objectIndex, 0x80000);
            } else {
                kwanm_setflag(objectIndex, 0x80000);
            }

            if (g_DynamicObjects[objectIndex].type < 0) {
                g_DynamicObjects[objectIndex].type = 1;

                if (g_DynamicObjects[objectIndex].fvaltrig > 0) {
                    g_DynamicObjects[objectIndex].fvaltrig--;
                }

                if (g_DynamicObjects[objectIndex].fvaltrig == 0) {
                    kwanm_clrflag(objectIndex, 0x2000);
                    kwanm_next(objectIndex);
                }
            }
        }
    }
}

void kwanm_swap(s32 objectIndex, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    if (kwanm_bitoffcheckflag(objectIndex, 0x2000) != 0) {
        g_DynamicObjects[objectIndex].anmtm = arg3;
        g_DynamicObjects[objectIndex].anmnumbak = arg1;
        g_DynamicObjects[objectIndex].type = 1;
        g_DynamicObjects[objectIndex].fvaltrig = arg4;
        kwanm_setflag(objectIndex, 0x2000);
        return;
    }

    g_DynamicObjects[objectIndex].anmtm--;
    if (g_DynamicObjects[objectIndex].anmtm < 0) {
        g_DynamicObjects[objectIndex].anmtm = arg3;
        g_DynamicObjects[objectIndex].type--;
        if ((g_DynamicObjects[objectIndex].type & 1) != 0) {
            g_DynamicObjects[objectIndex].anmnumbak = arg1;
        } else {
            g_DynamicObjects[objectIndex].anmnumbak = arg2;
        }

        if (g_DynamicObjects[objectIndex].type < 0) {
            g_DynamicObjects[objectIndex].type = 1;
            if (g_DynamicObjects[objectIndex].fvaltrig > 0) {
                g_DynamicObjects[objectIndex].fvaltrig--;
            }

            if (g_DynamicObjects[objectIndex].fvaltrig == 0) {
                kwanm_clrflag(objectIndex, 0x2000);
                kwanm_next(objectIndex);
            }
        }
    }
}

s32 kwanm_forward(s32 objectIndex, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 sp24;

    sp24 = 0;
    if (kwanm_bitoffcheckflag(objectIndex, 0x2000) != 0) {
        g_DynamicObjects[objectIndex].anmnumbak = arg1;
        g_DynamicObjects[objectIndex].anmtm = arg4;
        g_DynamicObjects[objectIndex].fvaltrig = arg5;
        kwanm_trig(objectIndex, 1);
        kwanm_setflag(objectIndex, 0x2000);
    } else {
        g_DynamicObjects[objectIndex].anmtm--;
        if (g_DynamicObjects[objectIndex].anmtm <= 0) {
            g_DynamicObjects[objectIndex].anmtm = arg4;
            g_DynamicObjects[objectIndex].anmnumbak += arg3;
            if (arg2 < g_DynamicObjects[objectIndex].anmnumbak) {

                if (g_DynamicObjects[objectIndex].fvaltrig > 0) {
                    g_DynamicObjects[objectIndex].fvaltrig--;
                }
                if (g_DynamicObjects[objectIndex].fvaltrig == 0) {
                    g_DynamicObjects[objectIndex].anmnumbak = arg2;
                    kwanm_clrflag(objectIndex, 0x2000);
                    kwanm_trig(objectIndex, 0);
                    kwanm_next(objectIndex);
                    sp24 = 1;
                } else {
                    g_DynamicObjects[objectIndex].anmnumbak = arg1;
                }
            }
        }
    }
    return sp24;
}

bool kwanm_reverse(s32 objectIndex, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 sp24;

    sp24 = false;
    if (kwanm_bitoffcheckflag(objectIndex, 0x2000) != 0) {
        g_DynamicObjects[objectIndex].anmnumbak = arg1;
        g_DynamicObjects[objectIndex].anmtm = arg4;
        g_DynamicObjects[objectIndex].fvaltrig = arg5;
        kwanm_trig(objectIndex, 1);
        kwanm_setflag(objectIndex, 0x2000);
    } else {
        g_DynamicObjects[objectIndex].anmtm--;
        if (g_DynamicObjects[objectIndex].anmtm <= 0) {
            g_DynamicObjects[objectIndex].anmtm = arg4;
            g_DynamicObjects[objectIndex].anmnumbak -= arg3;
            if (g_DynamicObjects[objectIndex].anmnumbak < arg2) {
                if (g_DynamicObjects[objectIndex].fvaltrig > 0) {
                    g_DynamicObjects[objectIndex].fvaltrig--;
                }
                if (g_DynamicObjects[objectIndex].fvaltrig == 0) {
                    g_DynamicObjects[objectIndex].anmnumbak = arg2;
                    kwanm_clrflag(objectIndex, 0x2000);
                    kwanm_trig(objectIndex, 0);
                    kwanm_next(objectIndex);
                    sp24 = true;
                } else {
                    g_DynamicObjects[objectIndex].anmnumbak = arg1;
                }
            }
        }
    }
    return sp24;
}

bool kwanm_oufuku_f(s32 objectIndex, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 sp24;

    sp24 = false;
    if (kwanm_bitoffcheckflag(objectIndex, 0x2000) != 0) {
        g_DynamicObjects[objectIndex].anmnumbak = arg1;
        g_DynamicObjects[objectIndex].anmtm = arg4;
        g_DynamicObjects[objectIndex].fvaltrig = arg5;
        kwanm_trig(objectIndex, 1);
        kwanm_setflag(objectIndex, 0x2000);
        kwanm_clrflag(objectIndex, 0x4000);
    } else {
        g_DynamicObjects[objectIndex].anmtm--;
        if (g_DynamicObjects[objectIndex].anmtm <= 0) {
            g_DynamicObjects[objectIndex].anmtm = arg4;
            if (kwanm_bitoffcheckflag(objectIndex, 0x4000) != 0) {
                g_DynamicObjects[objectIndex].anmnumbak += arg3;
                if (g_DynamicObjects[objectIndex].anmnumbak >= arg2) {
                    g_DynamicObjects[objectIndex].anmnumbak = arg2;
                    kwanm_setflag(objectIndex, 0x4000);
                }
            } else {
                g_DynamicObjects[objectIndex].anmnumbak -= arg3;
                if (arg1 >= g_DynamicObjects[objectIndex].anmnumbak) {
                    g_DynamicObjects[objectIndex].anmnumbak = arg1;
                    if (g_DynamicObjects[objectIndex].fvaltrig > 0) {
                        g_DynamicObjects[objectIndex].fvaltrig--;
                    }

                    if (g_DynamicObjects[objectIndex].fvaltrig == 0) {
                        kwanm_clrflag(objectIndex, 0x80);
                        kwanm_clrflag(objectIndex, 0x2000);
                        kwanm_trig(objectIndex, 0);
                        kwanm_next(objectIndex);
                        sp24 = true;
                    } else {
                        kwanm_clrflag(objectIndex, 0x4000);
                        kwanm_setflag(objectIndex, 0x80);
                    }
                }
            }
        }
    }
    return sp24;
}

s32 kwanm_oufuku_r(s32 objectIndex, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 sp24;

    sp24 = 0;
    if (kwanm_bitoffcheckflag(objectIndex, 0x2000) != 0) {
        g_DynamicObjects[objectIndex].anmnumbak = arg1;
        g_DynamicObjects[objectIndex].anmtm = arg4;
        g_DynamicObjects[objectIndex].fvaltrig = arg5;
        kwanm_trig(objectIndex, 1);
        kwanm_setflag(objectIndex, 0x2000);
        kwanm_clrflag(objectIndex, 0x4000);
    } else {
        g_DynamicObjects[objectIndex].anmtm--;
        if (g_DynamicObjects[objectIndex].anmtm <= 0) {
            g_DynamicObjects[objectIndex].anmtm = arg4;
            if (kwanm_bitoffcheckflag(objectIndex, 0x4000) != 0) {
                g_DynamicObjects[objectIndex].anmnumbak -= arg3;
                if (arg2 >= g_DynamicObjects[objectIndex].anmnumbak) {
                    g_DynamicObjects[objectIndex].anmnumbak = arg2;
                    kwanm_setflag(objectIndex, 0x4000);
                }
            } else {
                g_DynamicObjects[objectIndex].anmnumbak += arg3;
                if (g_DynamicObjects[objectIndex].anmnumbak >= arg1) {
                    g_DynamicObjects[objectIndex].anmnumbak = arg1;
                    if (g_DynamicObjects[objectIndex].fvaltrig > 0) {
                        g_DynamicObjects[objectIndex].fvaltrig--;
                    }
                    if (g_DynamicObjects[objectIndex].fvaltrig == 0) {
                        kwanm_clrflag(objectIndex, 0x2000);
                        kwanm_trig(objectIndex, 0);
                        kwanm_next(objectIndex);
                        sp24 = 1;
                    } else {
                        kwanm_clrflag(objectIndex, 0x4000);
                    }
                }
            }
        }
    }
    return sp24;
}

void kwanm_init_common(s32 objectIndex, u8 arg1, u8 arg2, Vtx* arg3) {
    g_DynamicObjects[objectIndex].vtxaddr = arg3;
    g_DynamicObjects[objectIndex].texsizey = arg1;
    g_DynamicObjects[objectIndex].fvalptr = arg2;
    g_DynamicObjects[objectIndex].anmnumbak = 0;
    g_DynamicObjects[objectIndex].anmcnt = -1;
    g_DynamicObjects[objectIndex].flag = 0;
}

void kwanm_init_ci8(s32 objectIndex, u8* texture, u8* arg2, u8 arg3, u16 arg4) {
    g_DynamicObjects[objectIndex].stexaddr = texture;
    g_DynamicObjects[objectIndex].sindexaddr = arg2;
    g_DynamicObjects[objectIndex].texsizey = arg3;
    g_DynamicObjects[objectIndex].fvalptr = arg4;
    g_DynamicObjects[objectIndex].anmnumbak = 0;
    g_DynamicObjects[objectIndex].anmcnt = -1;
    g_DynamicObjects[objectIndex].flag = 0;
}

UNUSED void kwanm_init_ci8_pal(s32 objectIndex, u8* arg1, u8 arg2, u8 arg3, Vtx* arg4) {
    g_DynamicObjects[objectIndex].texaddr = arg1;
    g_DynamicObjects[objectIndex].stexaddr = arg1;
    g_DynamicObjects[objectIndex].texsizey = arg2;
    g_DynamicObjects[objectIndex].fvalptr = arg3;
    g_DynamicObjects[objectIndex].vtxaddr = arg4;
    g_DynamicObjects[objectIndex].anmnumbak = 0;
    g_DynamicObjects[objectIndex].anmcnt = -1;
    g_DynamicObjects[objectIndex].flag = 0;
}

UNUSED void kwanm_texaddr_ci8() {
}

void kwanm_texaddr_ci8_colpal(s32 objectIndex) {
    // I have no idea why this typecast works
    g_DynamicObjects[objectIndex].texaddr =
        (u8*) ((u32*) g_DynamicObjects[objectIndex].stexaddr + (g_DynamicObjects[objectIndex].anmnumbak * 128));
    g_DynamicObjects[objectIndex].indexaddr = g_DynamicObjects[objectIndex].sindexaddr;
}

void kwanm_texaddr_ci8_index(s32 objectIndex) {
    g_DynamicObjects[objectIndex].texaddr = g_DynamicObjects[objectIndex].stexaddr;
    g_DynamicObjects[objectIndex].indexaddr =
        g_DynamicObjects[objectIndex].sindexaddr +
        (g_DynamicObjects[objectIndex].texsizey * g_DynamicObjects[objectIndex].fvalptr *
         g_DynamicObjects[objectIndex].anmnumbak);
}

UNUSED void kwanm_texaddr_ci8_index_cut2() {
}

UNUSED void kw_changecolor(s32 objectIndex) {
    s16* test = &g_DynamicObjects[objectIndex].anmptr;

    D_8018D1EC++;
    if (D_8018D1EC == 5) {
        D_8018D1EC = 0;
    }
    (*test)++;
}

void kwanm_init_poly(s32 objectIndex, Gfx* arg1, f32 arg2) {
    g_DynamicObjects[objectIndex].flag = 0;
    g_DynamicObjects[objectIndex].shape = arg1;
    g_DynamicObjects[objectIndex].scale = arg2;
    kwanm_next(objectIndex);
}

void kwchart_off(s32 objectIndex) {
    kwanm_trig(objectIndex, 0);
    g_DynamicObjects[objectIndex].stackptr = 0;
    g_DynamicObjects[objectIndex].timer = -1;
    kwanm_clrflag(objectIndex, 0x8000);
}

void kwchart_next(s32 objectIndex) {
    kwanm_clrflag(objectIndex, 0x8000);
    ++g_DynamicObjects[objectIndex].stackptr;
}

UNUSED void kwchart_jump(s32 objectIndex, s32 arg1) {
    kwanm_clrflag(objectIndex, 0x8000);
    g_DynamicObjects[objectIndex].stackptr = arg1;
}

void kwchart_start(s32 objectIndex) {
    kwanm_clrflag(objectIndex, 0x8000);
    g_DynamicObjects[objectIndex].stackptr = 1;
}

void kwchart_finish(s32 objectIndex) {
    kwanm_clrflag(objectIndex, 0x8000);
    g_DynamicObjects[objectIndex].stackptr = 0;
}

bool kwchart_wait(s32 objectIndex, s32 arg1) {
    s32 sp24;

    sp24 = false;
    if (kwanm_bitoffcheckflag(objectIndex, 0x00008000) != 0) {
        g_DynamicObjects[objectIndex].timer = arg1;
        kwanm_setflag(objectIndex, 0x00008000);
    }
    g_DynamicObjects[objectIndex].timer--;
    if (g_DynamicObjects[objectIndex].timer < 0) {
        kwanm_clrflag(objectIndex, 0x00008000);
        kwchart_next(objectIndex);
        sp24 = true;
    }
    return sp24;
}

void kwsetsval_trig(s32 objectIndex, s32 arg1) {
    g_DynamicObjects[objectIndex].svalloop = arg1;
}

void kwsetsval_next(s32 objectIndex) {
    g_DynamicObjects[objectIndex].mvtype++;
}

void kwsetsval_start(s32 objectIndex) {
    kwsetsval_trig(objectIndex, 0);
    g_DynamicObjects[objectIndex].mvtype = 1;
}

void kwsetsval_finish(s32 objectIndex) {
    g_DynamicObjects[objectIndex].mvtype = 0;
    g_DynamicObjects[objectIndex].svalloop = 0;
}

s32 kwsetsval_forward_sub(s32 objectIndex, s16* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 phi_t0;

    phi_t0 = 0;
    if (g_DynamicObjects[objectIndex].svalloop == 0) {
        *arg1 = arg2;
        g_DynamicObjects[objectIndex].svaltm = arg5;
        g_DynamicObjects[objectIndex].player = arg6;
        kwsetsval_trig(objectIndex, 1);
    } else {
        g_DynamicObjects[objectIndex].svaltm--;
        if (g_DynamicObjects[objectIndex].svaltm < 0) {
            g_DynamicObjects[objectIndex].svaltm = arg5;
            *arg1 += arg4;
            if (arg3 < *arg1) {
                if (g_DynamicObjects[objectIndex].player > 0) {
                    g_DynamicObjects[objectIndex].player--;
                }

                if (g_DynamicObjects[objectIndex].player == 0) {
                    *arg1 = arg3;
                    kwsetsval_trig(objectIndex, 0);
                    kwsetsval_next(objectIndex);
                    phi_t0 = 1;
                } else {
                    *arg1 = arg2;
                }
            }
        }
    }

    return phi_t0;
}

void kwsetsval_forward(s32 arg0, s16* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    kwsetsval_forward_sub(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
}

UNUSED void kwsetsval_forward_limit(s32 arg0, s16* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    if (*arg1 < arg3) {
        kwsetsval_forward_sub(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    }
}

bool kwsetsval_reverse_sub(s32 objectIndex, s16* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    bool phi_t0;

    phi_t0 = false;
    if (g_DynamicObjects[objectIndex].svalloop == 0) {
        *arg1 = arg2;
        g_DynamicObjects[objectIndex].svaltm = arg5;
        g_DynamicObjects[objectIndex].player = arg6;
        kwsetsval_trig(objectIndex, 1);
    } else {
        g_DynamicObjects[objectIndex].svaltm--;
        if (g_DynamicObjects[objectIndex].svaltm < 0) {
            g_DynamicObjects[objectIndex].svaltm = arg5;
            *arg1 -= arg4;
            if (*arg1 < arg3) {
                if (g_DynamicObjects[objectIndex].player > 0) {
                    g_DynamicObjects[objectIndex].player--;
                }

                if (g_DynamicObjects[objectIndex].player == 0) {
                    *arg1 = arg3;
                    kwsetsval_trig(objectIndex, 0);
                    kwsetsval_next(objectIndex);
                    phi_t0 = true;
                } else {
                    *arg1 = arg2;
                }
            }
        }
    }

    return phi_t0;
}

s32 kwsetsval_reverse(s32 arg0, s16* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    return kwsetsval_reverse_sub(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
}

s32 kwsetsval_reverse_limit(s32 arg0, s16* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    if (arg3 < *arg1) {
        return kwsetsval_reverse_sub(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    }
}

bool kwsetsval_oufuku_f_sub(s32 arg0, s32 objectIndex, s16* arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    s32 phi_t0;

    phi_t0 = false;
    if (g_DynamicObjects[objectIndex].svalloop == 0) {
        g_DynamicObjects[objectIndex].svaltm = arg6;
        if (arg0 != 0) {
            *arg2 = arg3;
        }
        g_DynamicObjects[objectIndex].player = arg7;
        kwsetsval_trig(objectIndex, 1);
    } else {
        g_DynamicObjects[objectIndex].svaltm--;
        if (g_DynamicObjects[objectIndex].svaltm < 0) {
            g_DynamicObjects[objectIndex].svaltm = arg6;
            if (g_DynamicObjects[objectIndex].svalloop == 1) {
                *arg2 += arg5;
                if (*arg2 >= arg4) {
                    *arg2 = arg4;
                    g_DynamicObjects[objectIndex].svalloop++;
                }
            } else {
                *arg2 -= arg5;
                if (arg3 >= *arg2) {
                    *arg2 = arg3;
                    if (g_DynamicObjects[objectIndex].player > 0) {
                        g_DynamicObjects[objectIndex].player--;
                    }
                    if (g_DynamicObjects[objectIndex].player == 0) {
                        kwsetsval_trig(objectIndex, 0);
                        kwsetsval_next(objectIndex);
                        phi_t0 = true;
                    } else {
                        g_DynamicObjects[objectIndex].svalloop = 1;
                    }
                }
            }
        }
    }

    return phi_t0;
}

bool kwsetsval_oufuku_f(s32 objectIndex, s16* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    return kwsetsval_oufuku_f_sub(1, objectIndex, arg1, arg2, arg3, arg4, arg5, arg6);
}

bool kwsetsval_oufuku_f_rnds(s32 objectIndex, s16* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    return kwsetsval_oufuku_f_sub(0, objectIndex, arg1, arg2, arg3, arg4, arg5, arg6);
}

UNUSED void kwsetsval_addchase(s32 objectIndex, s16* arg1, s16 arg2, s32 arg3) {
    *arg1 = *arg1 + arg3;
    if (*arg1 >= arg2) {
        *arg1 = arg2;
        kwsetsval_trig(objectIndex, 0);
        kwsetsval_next(objectIndex);
    }
}

void kwsetsval_subchase(s32 objectIndex, s16* arg1, s16 arg2, s32 arg3) {
    *arg1 = *arg1 - arg3;
    if (arg2 >= *arg1) {
        *arg1 = arg2;
        kwsetsval_trig(objectIndex, 0);
        kwsetsval_next(objectIndex);
    }
}

bool kwanm_turn_left(s32 objectIndex, u16* arg1, u16 arg2, s32 arg3) {
    bool phi_t0;
    s32 temp_v1;

    phi_t0 = false;
    if (g_DynamicObjects[objectIndex].svalloop == 0) {
        kwsetsval_trig(objectIndex, 1);
        g_DynamicObjects[objectIndex].ival = arg3;
    }

    temp_v1 = g_DynamicObjects[objectIndex].ival - arg2;
    if (temp_v1 <= 0) {
        *arg1 += g_DynamicObjects[objectIndex].ival;
        kwsetsval_trig(objectIndex, 0);
        phi_t0 = true;
    } else {
        *arg1 += arg2;
        g_DynamicObjects[objectIndex].ival = temp_v1;
    }

    return phi_t0;
}

UNUSED bool kwanm_turn_right(s32 objectIndex, u16* arg1, u16 arg2, s32 arg3) {
    bool phi_t0;
    s32 temp_v1;

    phi_t0 = false;
    if (g_DynamicObjects[objectIndex].svalloop == 0) {
        kwsetsval_trig(objectIndex, 1);
        g_DynamicObjects[objectIndex].ival = arg3;
    }

    temp_v1 = g_DynamicObjects[objectIndex].ival - arg2;
    if (temp_v1 <= 0) {
        *arg1 += g_DynamicObjects[objectIndex].ival;
        kwsetsval_trig(objectIndex, 0);
        phi_t0 = true;
    } else {
        *arg1 -= arg2;
        g_DynamicObjects[objectIndex].ival = temp_v1;
    }
    return phi_t0;
}

void kwsetfval_trig(s32 objectIndex, s32 arg1) {
    g_DynamicObjects[objectIndex].fvalloop = arg1;
}

void kwsetfval_next(s32 objectIndex) {
    g_DynamicObjects[objectIndex].svalptr++;
}

void kwsetfval_start(s32 objectIndex) {
    kwsetfval_trig(objectIndex, 0);
    g_DynamicObjects[objectIndex].svalptr = 1;
}

UNUSED void kwsetfval_finish(void) {
}

bool kwsetfval_forward_sub(s32 objectIndex, f32* arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6) {
    bool phi_a3;

    phi_a3 = false;
    if (g_DynamicObjects[objectIndex].fvalloop == 0) {
        *arg1 = arg2;
        g_DynamicObjects[objectIndex].fvaltm = arg5;
        g_DynamicObjects[objectIndex].svaltrig = arg6;
        kwsetfval_trig(objectIndex, 1);
    } else {
        g_DynamicObjects[objectIndex].fvaltm--;
        if ((s32) g_DynamicObjects[objectIndex].fvaltm < 0) {
            g_DynamicObjects[objectIndex].fvaltm = arg5;
            *arg1 += arg4;
            if (arg3 < *arg1) {
                if ((s32) g_DynamicObjects[objectIndex].svaltrig > 0) {
                    g_DynamicObjects[objectIndex].svaltrig--;
                }
                if (g_DynamicObjects[objectIndex].svaltrig == 0) {
                    *arg1 = arg3;
                    kwsetfval_trig(objectIndex, 0);
                    kwsetfval_next(objectIndex);
                    phi_a3 = true;
                } else {
                    *arg1 = arg2;
                }
            }
        }
    }

    return phi_a3;
}

s32 kwsetfval_forward(s32 objectIndex, f32* arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6) {
    return kwsetfval_forward_sub(objectIndex, arg1, arg2, arg3, arg4, arg5, arg6);
}

s32 kwsetfval_forward_limit(s32 objectIndex, f32* arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6) {
    if (*arg1 < arg3) {
        return kwsetfval_forward_sub(objectIndex, arg1, arg2, arg3, arg4, arg5, arg6);
    }
}

s32 kwsetfval_reverse_sub(s32 objectIndex, f32* arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6) {
    if (g_DynamicObjects[objectIndex].fvalloop == 0) {
        *arg1 = arg2;
        g_DynamicObjects[objectIndex].fvaltm = arg5;
        g_DynamicObjects[objectIndex].svaltrig = arg6;
        kwsetfval_trig(objectIndex, 1);
    } else {
        g_DynamicObjects[objectIndex].fvaltm += -1;
        if (g_DynamicObjects[objectIndex].fvaltm < 0) {
            g_DynamicObjects[objectIndex].fvaltm = arg5;
            *arg1 -= arg4;
            if (*arg1 < arg3) {
                if (g_DynamicObjects[objectIndex].svaltrig > 0) {
                    g_DynamicObjects[objectIndex].svaltrig--;
                }
                if (g_DynamicObjects[objectIndex].svaltrig == 0) {
                    *arg1 = arg3;
                    kwsetfval_trig(objectIndex, 0);
                    kwsetfval_next(objectIndex);
                } else {
                    *arg1 = arg2;
                }
            }
        }
    }

    return 0;
}

UNUSED void kwsetfval_reverse(s32 objectIndex, f32* arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6) {
    kwsetfval_reverse_sub(objectIndex, arg1, arg2, arg3, arg4, arg5, arg6);
}

UNUSED void kwsetfval_reverse_limit(s32 objectIndex, f32* arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6) {
    if (arg3 < *arg1) {
        kwsetfval_reverse_sub(objectIndex, arg1, arg2, arg3, arg4, arg5, arg6);
    }
}

void kwsetfval_oufuku_f(s32 objectIndex, f32* arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6) {
    if (g_DynamicObjects[objectIndex].fvalloop == 0) {
        *arg1 = arg2;
        g_DynamicObjects[objectIndex].fvaltm = arg5;
        g_DynamicObjects[objectIndex].svaltrig = arg6;
        kwsetfval_trig(objectIndex, 1);
        return;
    }

    g_DynamicObjects[objectIndex].fvaltm--;
    if (g_DynamicObjects[objectIndex].fvaltm < 0) {
        g_DynamicObjects[objectIndex].fvaltm = arg5;
        if (g_DynamicObjects[objectIndex].fvalloop == 1) {
            *arg1 += arg4;
            if (arg3 <= *arg1) {
                *arg1 = arg3;
                g_DynamicObjects[objectIndex].fvalloop++;
            }
        } else {
            *arg1 -= arg4;
            if (*arg1 <= arg2) {
                *arg1 = arg2;

                if (g_DynamicObjects[objectIndex].svaltrig > 0) {
                    g_DynamicObjects[objectIndex].svaltrig--;
                }
                if (g_DynamicObjects[objectIndex].svaltrig == 0) {
                    kwsetfval_trig(objectIndex, 0);
                    kwsetfval_next(objectIndex);
                } else {
                    g_DynamicObjects[objectIndex].fvalloop = 1;
                }
            }
        }
    }
}

void kwdma_wait(s32 objectIndex) {
    g_DynamicObjects[objectIndex].flag |= 1;
}

void kwdma(s32 objectIndex) {
    g_DynamicObjects[objectIndex].flag &= ~1;
}

void kwdma_texture(void) {
    if (D_8018D224 != 0) {
        osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
        D_8018D224 = 0;
    }
}

void kwanm_setdisplaybuffer_pal(uintptr_t devAddr, void* vaddr, size_t nbytes) {
    kwdma_texture();
    osPiStartDma(&gDmaIoMesg, OS_MESG_PRI_NORMAL, OS_READ, devAddr, vaddr, nbytes, &gDmaMesgQueue);
    D_8018D224 = 1;
}

void kwanm_setdisplaybuffer_allpal(u8* arg0, void* arg1, u16 arg2, u16 arg3) {
    kwanm_setdisplaybuffer_pal((uintptr_t) &_other_texturesSegmentRomStart[SEGMENT_OFFSET(arg0)], arg1, arg2 * arg3);
}

//! @todo arg1 should likely be a u8 *
void kwanm_setdisplaybuffer_1pal(s32 objectIndex, uintptr_t arg1) {
    s32 phi_a1;

    if ((g_DynamicObjects[objectIndex].flag & 1) != 0) {
        phi_a1 = 0;
        if (g_DynamicObjects[objectIndex].stexaddr != g_DynamicObjects[objectIndex].texaddr) {
            g_DynamicObjects[objectIndex].texaddr = g_DynamicObjects[objectIndex].stexaddr;
        }

        g_DynamicObjects[objectIndex].flag ^= 2;
        if ((g_DynamicObjects[objectIndex].flag & 2) != 0) {
            phi_a1 = 1;
        }

        g_DynamicObjects[objectIndex].indexaddr =
            (u8*) (g_DynamicObjects[objectIndex].texsizey * g_DynamicObjects[objectIndex].fvalptr * phi_a1) + arg1;
        kwdma(objectIndex);
    }
}

void kwanm_setdrawbuffer(s32 objectIndex, uintptr_t arg1) {
    s32 phi_a1;

    if ((g_DynamicObjects[objectIndex].flag & 1) != 0) {
        // I have no idea why this typecase works
        g_DynamicObjects[objectIndex].texaddr =
            (u8*) ((u32*) g_DynamicObjects[objectIndex].stexaddr + (g_DynamicObjects[objectIndex].anmcnt << 7));
        g_DynamicObjects[objectIndex].flag ^= 2;
        phi_a1 = 0;
        if ((g_DynamicObjects[objectIndex].flag & 2) != 0) {
            phi_a1 = 1;
        }
        g_DynamicObjects[objectIndex].indexaddr =
            (u8*) (g_DynamicObjects[objectIndex].texsizey * g_DynamicObjects[objectIndex].fvalptr * phi_a1) + arg1;
        kwdma(objectIndex);
    }
}

void kwanm_dma_ci8(s32 objectIndex, uintptr_t arg1) {
    s32 phi_a1;

    if ((g_DynamicObjects[objectIndex].flag & 1) != 0) {
        g_DynamicObjects[objectIndex].texaddr = g_DynamicObjects[objectIndex].stexaddr;
        g_DynamicObjects[objectIndex].flag ^= 2;
        phi_a1 = 0;
        if ((g_DynamicObjects[objectIndex].flag & 2) != 0) {
            phi_a1 = 1;
        }
        g_DynamicObjects[objectIndex].indexaddr =
            (u8*) (g_DynamicObjects[objectIndex].texsizey * g_DynamicObjects[objectIndex].fvalptr * phi_a1) + arg1;
        kwdma(objectIndex);
    }
}

u8* kwanm_texaddr_ci8_dma_pal(s32 objectIndex, u8* arg1) {
    s32 phi_a2;

    g_DynamicObjects[objectIndex].flag ^= 4;
    phi_a2 = 0;
    if ((g_DynamicObjects[objectIndex].flag & 4) != 0) {
        phi_a2 = 1;
    }
    return (g_DynamicObjects[objectIndex].texsizey * g_DynamicObjects[objectIndex].fvalptr * phi_a2) + arg1;
}

void kwanm_texaddr_ci8_dma_allpal(s32 objectIndex, u8* arg1) {
    u8* sp24;
    if (g_DynamicObjects[objectIndex].anmnumbak != g_DynamicObjects[objectIndex].anmcnt) {
        sp24 = g_DynamicObjects[objectIndex].sindexaddr +
               (g_DynamicObjects[objectIndex].texsizey * g_DynamicObjects[objectIndex].fvalptr *
                g_DynamicObjects[objectIndex].anmnumbak);
        kwanm_setdisplaybuffer_allpal(sp24, (void*) kwanm_texaddr_ci8_dma_pal(objectIndex, arg1), g_DynamicObjects[objectIndex].texsizey,
                      g_DynamicObjects[objectIndex].fvalptr);
        g_DynamicObjects[objectIndex].anmcnt = g_DynamicObjects[objectIndex].anmnumbak;
        kwdma_wait(objectIndex);
    }
}

void kwanm_texaddr_ci8_dma_1pal(s32 objectIndex, u8* arg1) {
    kwanm_texaddr_ci8_dma_allpal(objectIndex, arg1);
    kwanm_setdisplaybuffer_1pal(objectIndex, (uintptr_t) arg1);
}

void fuusen_coltbl(s32 objectIndex, u8* arg1) {
    kwanm_texaddr_ci8_dma_allpal(objectIndex, arg1);
    kwanm_setdrawbuffer(objectIndex, (uintptr_t) arg1);
}

void fuusen_addcoltbl(s32 objectIndex, u8* arg1) {
    kwanm_texaddr_ci8_dma_allpal(objectIndex, arg1);
    kwanm_dma_ci8(objectIndex, (uintptr_t) arg1);
}

void kwanm_fuusen_init(s32 objectIndex) {
    s32 sp2C;
    s32 sp28;
    s32 sp24;
    s32 sp20;
    s16 temp_v0;
    s32 temp_a0;
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    object->scale = 0.15f;
    temp_v0 = g_courseID;
    switch (temp_v0) { /* irregular */
        case COURSE_MARIO_RACEWAY:
            sp2C = MakeRandomLimmit(0x00C8U);
            sp28 = MakeRandomLimmit(D_80165748);
            sp24 = MakeRandomLimmit(0x0096U);
            sp20 = MakeRandomLimmit(0x2000U);
            object->pos_start[0] = (f32) ((((f64) D_80165718 + 100.0) - (f64) sp2C) * (f64) xOrientation);
            object->pos_start[1] = (f32) (D_80165720 + sp28);
            object->pos_start[2] = (f32) (((f64) D_80165728 + 200.0) - (f64) sp24);
            break;
        case COURSE_ROYAL_RACEWAY:
            sp2C = MakeRandomLimmit(0x0168U);
            sp28 = MakeRandomLimmit(D_80165748);
            sp24 = MakeRandomLimmit(0x00B4U);
            sp20 = MakeRandomLimmit(0x2000U);
            object->pos_start[0] = (f32) ((((f64) D_80165718 + 180.0) - (f64) sp2C) * (f64) xOrientation);
            object->pos_start[1] = (f32) (D_80165720 + sp28);
            object->pos_start[2] = (f32) (((f64) D_80165728 + 200.0) - (f64) sp24);
            break;
        case COURSE_LUIGI_RACEWAY:
            sp2C = MakeRandomLimmit(0x012CU);
            sp28 = MakeRandomLimmit(D_80165748);
            sp24 = MakeRandomLimmit(0x0096U);
            sp20 = MakeRandomLimmit(0x2000U);
            object->pos_start[0] = (f32) ((((f64) D_80165718 + 150.0) - (f64) sp2C) * (f64) xOrientation);
            object->pos_start[1] = (f32) (D_80165720 + sp28);
            object->pos_start[2] = (f32) (((f64) D_80165728 + 200.0) - (f64) sp24);
            break;
    }
    kwset_init3D_sppos(objectIndex, 0, 0, 0);
    if (g_menuMultiplayerSelection == 1) {
        object->velocity[1] = (f32) (((f64) (f32) (sp2C % 4) * 0.25) + 0.8);
    } else {
        object->velocity[1] = (f32) (((f64) (f32) (sp2C % 3) * 0.2) + 0.4);
    }
    temp_a0 = sp2C % 8;
    object->stack[0] = D_800E6F30[temp_a0][0];
    object->stack[1] = D_800E6F30[temp_a0][1];
    object->stack[2] = D_800E6F30[temp_a0][2];
    object->stack[3] = D_800E6F48[temp_a0][0];
    object->stack[4] = D_800E6F48[temp_a0][1];
    object->stack[5] = D_800E6F48[temp_a0][2];
    object->stack[6] = sp20 - 0x1000;
    if (sp2C & 1) {
        object->stack[7] = (sp20 / 32) + 0x100;
    } else {
        object->stack[7] = -0x100 - (sp20 / 32);
    }
    object->alpha = 0x00E6;
    kwanm_next(objectIndex);
}

void kwmv_fuusen(s32 objectIndex) {
    if (g_DynamicObjects[objectIndex].mvptr == 1) {
        if ((D_80165740 <= g_DynamicObjects[objectIndex].pos_spline[1]) &&
            (kwsub_sval(&g_DynamicObjects[objectIndex].alpha, 0, 8) != 0)) {
            kwmv_finish(objectIndex);
        }
        kwmv_addvelytopos(objectIndex);
    }
    kwset_splinepos3D(objectIndex);
}

void kwanm_fuusen(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 1:
            kwanm_fuusen_init(objectIndex);
            break;
        case 2:
            if (kwanm_wait(objectIndex, 1) != false) {
                kwmv_start(objectIndex);
                break;
            }
        case 0:
            break;
        case 3:
            kwoufuku_sval(&g_DynamicObjects[objectIndex].stack[6], -0x1000, 0x1000, &g_DynamicObjects[objectIndex].stack[7]);
            if (g_DynamicObjects[objectIndex].mvptr == 0) {
                kwanm_finish(objectIndex);
            }
            break;
    }
}

void kwchart_fuusen(void) {
    s32 someIndex;
    s32 objectIndex;
    s32 someCount;
    AnmObject* object;

    someCount = 0;
    for (someIndex = 0; someIndex < D_80165738; someIndex++) {
        objectIndex = EffectAllocArray3[someIndex];
        if (objectIndex != DELETED_OBJECT_ID) {
            object = &g_DynamicObjects[objectIndex];
            if (object->anmptr != 0) {
                kwanm_fuusen(objectIndex);
                kwmv_fuusen(objectIndex);
                if (object->anmptr == 0) {
                    kwdealloc_effectram(&EffectAllocArray3[someIndex]);
                }
                someCount += 1;
            }
        }
    }
    if (someCount == 0) {
        D_80165730 = 0;
    }
}

void kwanm_broken_object(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].anmptr) { /* irregular */
        case 0:
            break;
        case 1:
            if (kwmv_nanamenage(objectIndex, g_DynamicObjects[objectIndex].velocity[1], 0.12f,
                              g_DynamicObjects[objectIndex].vel, g_DynamicObjects[objectIndex].ang[1],
                              0x00000064) != 0) {
                kwanm_next(objectIndex);
            }
            kwset_splinepos3D(objectIndex);
            g_DynamicObjects[objectIndex].texang[0] += D_8016582C[0];
            g_DynamicObjects[objectIndex].texang[1] += D_8016582C[1];
            g_DynamicObjects[objectIndex].texang[2] += D_8016582C[2];
            break;
        case 2:
            kwmv_finish(objectIndex);
            kwanm_finish(objectIndex);
            break;
    }
}

void kwset_broken_object(s32 objectIndex, s32 arg1, Vec3f arg2, s32 arg3, s32 arg4) {
    s32 sp24;
    s32 temp_v0;
    AnmObject* object;

    KWAnmStart(objectIndex, 0);
    sp24 = MakeRandomLimmit(0x01F4U);
    temp_v0 = MakeRandomLimmit(0x0032U);
    object = &g_DynamicObjects[objectIndex];
    object->chartptr = arg3;
    object->scale = ((f64) (f32) sp24 * 0.0005) + 0.05;
    // ALL HAIL THE FAKE MATCH GODS!!!!!
    object->velocity[1] = ((f64) (f32) temp_v0 * (0.05 * 1.0)) + 2.0;
    object->vel = ((f64) (f32) (temp_v0 % 5) * 0.1) + 1.0;
    object->ang[1] = (arg1 << 0x10) / arg4;
    object->pos_start[0] = (arg2[0] + (temp_v0 / 2)) - 12.0f;
    object->pos_start[1] = (arg2[1] - 10.0) + MakeRandomLimmit(0x000AU);
    object->pos_start[2] = (arg2[2] + (temp_v0 / 2)) - 12.0f;
    object->texang[0] = sp24 << 7;
    object->texang[1] = temp_v0 * 0x50;
    object->texang[2] = temp_v0 * 0x50;
}

void kwgetbroken_object(Vec3f arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 var_s1;
    s32 objectIndex;

    for (var_s1 = 0; var_s1 < arg3; var_s1++) {
        switch (arg1) { /* irregular */
            case 1:
                objectIndex = kwalloc_effectram(EffectAllocArray1, &gNextFreeObjectParticle1, gObjectParticle1_SIZE);
                break;
            case 2:
                objectIndex = kwalloc_effectram(EffectAllocArray2, &gNextFreeObjectParticle2, gObjectParticle2_SIZE);
                break;
            case 3:
                objectIndex = kwalloc_effectram(EffectAllocArray3, &gNextFreeObjectParticle3, gObjectParticle3_SIZE);
                break;
        }
        if (objectIndex == NULL_OBJECT_ID) {
            break;
        }
        kwset_broken_object(objectIndex, var_s1, arg0, arg2, arg3);
    }
}

void kwchart_broken_object(s32 arg0) {
    s32 objectIndex;
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
        objectIndex = var_s3[var_s2];
        if (objectIndex != DELETED_OBJECT_ID) {
            object = &g_DynamicObjects[objectIndex];
            if (object->anmptr != 0) {
                kwanm_broken_object(objectIndex);
                if (object->anmptr == 0) {
                    kwdealloc_effectram(&var_s3[var_s2]);
                }
            }
        }
    }
}

void kwinit_train_smoke(s32 objectIndex, Vec3f pos, f32 velocity) {
    AnmObject* object;
    UNUSED s32 pad[2];

    KWAnmStart(objectIndex, 0);
    object = &g_DynamicObjects[objectIndex];
    object->pos_start[0] = pos[0];
    object->pos_start[1] = pos[1];
    object->pos_start[2] = pos[2];
    object->velocity[1] = velocity;
    object->output = MakeRandomLimmit(0x0064U) + 0x1E;
}

s32 kwget_train_smoke(s32 trainIndex, Vec3f pos, f32 velocity) {
    s32 objectIndex;

    if (trainIndex == 0) {
        objectIndex = kwalloc_effectram(EffectAllocArray2, &gNextFreeObjectParticle2, gObjectParticle2_SIZE);
        if (objectIndex != NULL_OBJECT_ID) {
            kwinit_train_smoke(objectIndex, pos, velocity);
        }
    } else {
        objectIndex = kwalloc_effectram(EffectAllocArray3, &gNextFreeObjectParticle3, gObjectParticle3_SIZE);
        if (objectIndex != NULL_OBJECT_ID) {
            kwinit_train_smoke(objectIndex, pos, velocity);
        }
    }
    return objectIndex;
}

void kwanm_train_smoke_init(s32 objectIndex) {
    g_DynamicObjects[objectIndex].chartptr = 1;
    g_DynamicObjects[objectIndex].indexaddr = D_8018D490;
    g_DynamicObjects[objectIndex].sindexaddr = D_8018D490;
    g_DynamicObjects[objectIndex].alpha = 0xFF;
    g_DynamicObjects[objectIndex].ang[1] = 0;
    g_DynamicObjects[objectIndex].texang[0] = 0;
    g_DynamicObjects[objectIndex].texang[2] = 0;
    g_DynamicObjects[objectIndex].pos_spline[0] = 0.0f;
    g_DynamicObjects[objectIndex].pos_spline[1] = 0.0f;
    g_DynamicObjects[objectIndex].pos_spline[2] = 0.0f;
    g_DynamicObjects[objectIndex].scale = 0.5f;
    kwanm_next(objectIndex);
}

void kwanm_train_smoke(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 1:
            kwanm_train_smoke_init(objectIndex);
            break;
        case 2:
            g_DynamicObjects[objectIndex].velocity[1] -= 0.03;
            kwadd_fval(&g_DynamicObjects[objectIndex].pos_spline[1], 100.0f, g_DynamicObjects[objectIndex].velocity[1]);
            kwsetfval_forward_limit(objectIndex, &g_DynamicObjects[objectIndex].scale, 0.55f, 1.0f, 0.1f, 1, 0);
            if (kwsetsval_reverse(objectIndex, &g_DynamicObjects[objectIndex].alpha, 0xFF, 0x1E, 7, 0, 0) != 0) {
                kwanm_next(objectIndex);
            }
            break;
        case 3:
            kwanm_finish(objectIndex);
            break;
        case 0:
            break;
    }

    kwset_splinepos3D(objectIndex);
}

void kwchart_train_smoke(void) {
    UNUSED s32 pad[2];
    s32 count;
    s32 i;
    s32 temp_a0;
    AnmObject* object;
    if (D_8016578C != 0) {
        D_8016578C -= 1;
    }
    if (D_80165790 != 0) {
        D_80165790 -= 1;
    }
    if ((SL[0].someFlags != 0) || (D_8016578C != 0)) {
        count = 0;
        for (i = 0; i < 128; i++) {
            temp_a0 = EffectAllocArray2[i];
            if (temp_a0 != -1) {
                object = &g_DynamicObjects[temp_a0];
                if (object->anmptr != 0) {
                    kwanm_train_smoke(temp_a0);
                    if (object->anmptr == 0) {
                        kwdealloc_effectram(&EffectAllocArray2[i]);
                    }
                    count += 1;
                }
            }
        }
        if (count != 0) {
            D_8016578C = 100;
        }
    }
    if ((SL[1].someFlags != 0) || (D_80165790 != 0)) {
        count = 0;
        for (i = 0; i < 128; i++) {
            temp_a0 = EffectAllocArray3[i];
            if (temp_a0 != -1) {
                object = &g_DynamicObjects[temp_a0];
                if (object->anmptr != 0) {
                    kwanm_train_smoke(temp_a0);
                    if (object->anmptr == 0) {
                        kwdealloc_effectram(&EffectAllocArray3[i]);
                    }
                    count += 1;
                }
            }
        }
        if (count != 0) {
            D_80165790 = 100;
        }
    }
}

void kwinit_ship_smoke(s32 objectIndex, Vec3f pos, f32 velocity) {
    AnmObject* object;

    KWAnmStart(objectIndex, 0);
    object = &g_DynamicObjects[objectIndex];
    object->pos_start[0] = pos[0];
    object->pos_start[1] = pos[1];
    object->pos_start[2] = pos[2];
    object->velocity[1] = velocity;
    object->output = 0x00FF;
    object->input = 0x0096;
}

s32 kwget_ship_smoke(s32 ferryIndex, Vec3f pos, f32 velocity) {
    s32 objectIndex;

    if (ferryIndex == 0) {
        objectIndex = kwalloc_effectram(EffectAllocArray2, &gNextFreeObjectParticle2, gObjectParticle2_SIZE);
        if (objectIndex != NULL_OBJECT_ID) {
            kwinit_ship_smoke(objectIndex, pos, velocity);
        }
    } else {
        objectIndex = kwalloc_effectram(EffectAllocArray3, &gNextFreeObjectParticle3, gObjectParticle3_SIZE);
        if (objectIndex != NULL_OBJECT_ID) {
            kwinit_ship_smoke(objectIndex, pos, velocity);
        }
    }

    return objectIndex;
}

void kwanm_ship_smoke_init(s32 objectIndex) {
    g_DynamicObjects[objectIndex].chartptr = 6;
    g_DynamicObjects[objectIndex].indexaddr = D_8018D490;
    g_DynamicObjects[objectIndex].sindexaddr = D_8018D490;
    g_DynamicObjects[objectIndex].alpha = 0xFF;
    g_DynamicObjects[objectIndex].ang[1] = 0;
    g_DynamicObjects[objectIndex].texang[0] = 0;
    g_DynamicObjects[objectIndex].texang[2] = 0;
    g_DynamicObjects[objectIndex].pos_spline[0] = 0.0f;
    g_DynamicObjects[objectIndex].pos_spline[1] = 0.0f;
    g_DynamicObjects[objectIndex].pos_spline[2] = 0.0f;
    g_DynamicObjects[objectIndex].scale = 0.5f;
    kwanm_next(objectIndex);
}

void kwanm_ship_smoke(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 1:
            kwanm_ship_smoke_init(objectIndex);
            break;
        case 2:
            g_DynamicObjects[objectIndex].velocity[1] -= 0.03;
            kwadd_fval(&g_DynamicObjects[objectIndex].pos_spline[1], 100.0f, g_DynamicObjects[objectIndex].velocity[1]);
            kwsetfval_forward_limit(objectIndex, &g_DynamicObjects[objectIndex].scale, 0.55f, 1.0f, 0.1f, 1, 0);
            if (kwsetsval_reverse(objectIndex, &g_DynamicObjects[objectIndex].alpha, 0xFF, 0x1E, 7, 0, 0) != 0) {
                kwanm_next(objectIndex);
            }
            break;
        case 3:
            kwanm_finish(objectIndex);
            break;
        case 0:
            break;
    }

    kwset_splinepos3D(objectIndex);
}

void kwchart_ship_smoke(void) {
    UNUSED s32 pad[2];
    s32 count;
    s32 i;
    s32 temp_a0;
    AnmObject* object;
    if (D_8016578C != 0) {
        D_8016578C -= 1;
    }
    if (D_80165790 != 0) {
        D_80165790 -= 1;
    }
    if ((ship[0].someFlags != 0) || (D_8016578C != 0)) {
        count = 0;
        for (i = 0; i < 128; i++) {
            temp_a0 = EffectAllocArray2[i];
            if (temp_a0 != -1) {
                object = &g_DynamicObjects[temp_a0];
                if (object->anmptr != 0) {
                    kwanm_ship_smoke(temp_a0);
                    if (object->anmptr == 0) {
                        kwdealloc_effectram(&EffectAllocArray2[i]);
                    }
                    count += 1;
                }
            }
        }
        if (count != 0) {
            D_8016578C = 100;
        }
    }
    if ((ship[1].someFlags != 0) || (D_80165790 != 0)) {
        count = 0;
        for (i = 0; i < 128; i++) {
            temp_a0 = EffectAllocArray3[i];
            if (temp_a0 != -1) {
                object = &g_DynamicObjects[temp_a0];
                if (object->anmptr != 0) {
                    kwanm_ship_smoke(temp_a0);
                    if (object->anmptr == 0) {
                        kwdealloc_effectram(&EffectAllocArray3[i]);
                    }
                    count += 1;
                }
            }
        }
        if (count != 0) {
            D_80165790 = 100;
        }
    }
}

void kwinit_kokoopa_fire(s32 objectIndex, Vec3f arg1, u16 arg2, f32 arg3, s32 arg4) {
    AnmObject* object;

    KWAnmStart(objectIndex, 0);
    object = &g_DynamicObjects[objectIndex];
    object->scale = 0.5f;
    object->chartptr = 5;
    object->pos_start[0] = arg1[0];
    object->pos_start[1] = arg1[1];
    object->pos_start[2] = arg1[2];
    object->ang[0] = 0x0C00;
    object->ang[2] = 0;
    object->vel = arg3 * 4.0;
    object->ang[1] = arg2;
    object->output = 0x00FF;
    object->input = 0x00FF;
    object->ival = arg4 * 2;
}

s32 kwget_kokoopa_fire_sub(Vec3f arg0, u16 arg1, f32 arg2, s32 arg3) {
    s32 objectIndex;

    objectIndex = kwalloc_effectram(EffectAllocArray1, &gNextFreeObjectParticle1, gObjectParticle1_SIZE);
    if (objectIndex != NULL_OBJECT_ID) {
        kwinit_kokoopa_fire(objectIndex, arg0, arg1, arg2, arg3);
    }
    return objectIndex;
}

void kwget_kokoopa_fire(Vec3f arg0, u16 arg1, f32 arg2) {
    s32 someIndex;

    for (someIndex = 0; someIndex < 10; someIndex++) {
        if (kwget_kokoopa_fire_sub(arg0, arg1, arg2, someIndex) == -1) {
            break;
        }
    }
}

void kwcheck_kokoopafire_sub(s32 objectIndex) {
    if (g_DynamicObjects[objectIndex].timer > 0) {
        g_DynamicObjects[objectIndex].timer--;
        if (g_DynamicObjects[objectIndex].timer == 0) {
            kwanm_clrevflag(objectIndex, 1);
        }
    }

    if (g_DynamicObjects[objectIndex].ival > 0) {
        g_DynamicObjects[objectIndex].ival--;
        if (g_DynamicObjects[objectIndex].ival == 0) {
            NaSceneLevelStop(g_DynamicObjects[objectIndex].pos, SOUND_ARG_LOAD(0x51, 0x02, 0x80, 0x0A));
        }
    }

    if (g_DynamicObjects[objectIndex].timer == 0) {
        kwcheck_visible_area_camera(objectIndex, 300.0f);
        if ((kwanm_bitcheckflag(objectIndex, VISIBLE) != 0) && (kwanm_bitoffcheckevflag(objectIndex, 1) != 0)) {
            kwanm_setevflag(objectIndex, 1);
            kwget_kokoopa_fire(g_DynamicObjects[objectIndex].pos, g_DynamicObjects[objectIndex].ang[1], 1.0f);
            NaSceneLevelStart(g_DynamicObjects[objectIndex].pos, g_DynamicObjects[objectIndex].velocity,
                          SOUND_ARG_LOAD(0x51, 0x02, 0x80, 0x0A));
            if (g_DynamicObjects[objectIndex].output > 0) {
                g_DynamicObjects[objectIndex].output--;
                g_DynamicObjects[objectIndex].timer = 0x5A;
            } else {
                g_DynamicObjects[objectIndex].timer = 0x12C;
            }

            g_DynamicObjects[objectIndex].ival = 0x3C;
        }
    }

    if (kwcheck_mapareain(9, 0xB) == 0) {
        g_DynamicObjects[objectIndex].output = 2;
    }
}

void kwcheck_kokoopa_fire(void) {
    s32 someIndex;

    for (someIndex = 0; someIndex < 4; someIndex++) {
        kwcheck_kokoopafire_sub(objallocptr3[someIndex]);
    }
}

void kwinit_koopa_fire(s32 objectIndex, Vec3f arg1, f32 arg2, s32 arg3) {
    AnmObject* object;

    KWAnmStart(objectIndex, 0);
    object = &g_DynamicObjects[objectIndex];
    object->chartptr = 4;
    object->scale = 1.0f;
    object->pos_start[0] = arg1[0];
    object->pos_start[1] = arg1[1];
    object->pos_start[2] = arg1[2];
    object->ang[0] = 0x0C00;
    object->ang[2] = 0;
    object->ang[1] = 0x2100;
    if (g_ScreenFlip != 0) {
        object->ang[1] += -0x4000;
    }
    object->output = 0x00FF;
    object->input = 0x00FF;
    object->ival = arg3 * 2;
    object->vel = arg2 * 8.0;
}

s32 kwget_koopa_fire_sub(Vec3f arg0, f32 arg1, s32 arg2) {
    s32 objectIndex;

    objectIndex = kwalloc_effectram(EffectAllocArray1, &gNextFreeObjectParticle1, gObjectParticle1_SIZE);
    if (objectIndex != NULL_OBJECT_ID) {
        kwinit_koopa_fire(objectIndex, arg0, arg1, arg2);
    }
    return objectIndex;
}

void kwget_koopa_fire(Vec3f arg0, f32 arg1) {
    s32 someIndex;

    for (someIndex = 0; someIndex < 0x14; someIndex++) {
        if (kwget_koopa_fire_sub(arg0, arg1, someIndex) == -1) {
            break;
        }
    }
}

void kwanm_koopa_fire_init(s32 objectIndex) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    object->indexaddr = common_texture_particle_smoke[0];
    object->sindexaddr = common_texture_particle_smoke[0];
    object->alpha = 0x00FF;
    kwset_init3D_texang(objectIndex, 0U, 0U, 0U);
    kwset_init3D_sppos(objectIndex, 0.0f, 0.0f, 0.0f);
    kwanm_next(objectIndex);
}

void kwanm_koopa_fire(s32 objectIndex) {
    AnmObject* object;

    if (objectIndex) {}
    object = &g_DynamicObjects[objectIndex];
    switch (object->anmptr) {
        case 1:
            kwanm_koopa_fire_init(objectIndex);
            break;
        case 2:
            if (kwanm_wait(objectIndex, object->ival) != 0) {
                kwmv_start(objectIndex);
            }
            break;
        case 3:
            if (object->chartptr == 4) {
                KWChaseFVal(&object->scale, 4.0f, 0.1f);
                KWChaseSVal(&object->output, 0, 0x0018);
                KWChaseSVal(&object->input, 0x0080, 0x000C);
            } else {
                KWChaseFVal(&object->scale, 1.0f, 0.1f);
                KWChaseSVal(&object->output, 0, 0x0018);
                KWChaseSVal(&object->input, 0x0080, 0x000C);
            }
            if ((object->mvptr >= 2) &&
                (kwsetsval_reverse(objectIndex, &object->alpha, 0x000000FF, 0x00000050, 0x00000020, 0, 0) != 0)) {
                kwanm_next(objectIndex);
            }
            break;
        case 4:
            kwanm_finish(objectIndex);
            kwmv_finish(objectIndex);
            break;
        case 0:
        default:
            break;
    }
}

void kwmv_koopa_fire(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].mvptr) {
        case 0:
            break;
        case 1:
            if ((u8) g_DynamicObjects[objectIndex].chartptr == 4) {
                if (kwmv_wait(objectIndex, 0x0000000E) != 0) {
                    KWAnmNext(objectIndex);
                }
            } else if (kwmv_wait(objectIndex, 2) != 0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 2:
            kwsub_usval(&g_DynamicObjects[objectIndex].ang[0], 0, 0x00000400);
            break;
    }
    if (g_DynamicObjects[objectIndex].mvptr > 0) {
        kwmv_velangxytoposxyz(objectIndex);
        kwset_splinepos3D(objectIndex);
    }
}

void kwcheck_koopa_fire(void) {
    s32 objectIndex;

    objectIndex = objallocptr2[0];
    if (g_DynamicObjects[objectIndex].timer > 0) {
        g_DynamicObjects[objectIndex].timer--;
        if (g_DynamicObjects[objectIndex].timer == 0) {
            kwanm_clrevflag(objectIndex, 1);
        }
    }
    if (g_DynamicObjects[objectIndex].ival > 0) {
        g_DynamicObjects[objectIndex].ival--;
        if (g_DynamicObjects[objectIndex].ival == 0) {
            NaSceneLevelStop(g_DynamicObjects[objectIndex].pos, SOUND_ARG_LOAD(0x51, 0x03, 0x80, 0x09));
        }
    }
    if (g_DynamicObjects[objectIndex].timer == 0) {
        kwcheck_visible_area_camera(objectIndex, 750.0f);
        if ((kwanm_bitcheckflag(objectIndex, VISIBLE) != 0) && (kwanm_bitoffcheckevflag(objectIndex, 1) != 0)) {
            kwanm_setevflag(objectIndex, 1);
            kwget_koopa_fire(g_DynamicObjects[objectIndex].pos, 1.0f);
            NaSceneLevelStart(g_DynamicObjects[objectIndex].pos, g_DynamicObjects[objectIndex].velocity, 0x51038009U);
            if (g_DynamicObjects[objectIndex].output > 0) {
                g_DynamicObjects[objectIndex].output--;
                g_DynamicObjects[objectIndex].timer = 0x0000005A;
            } else {
                g_DynamicObjects[objectIndex].timer = 0x0000012C;
            }
            g_DynamicObjects[objectIndex].ival = 0x0000003C;
        }
    }
    if (kwcheck_mapareain(4, 5) == 0) {
        g_DynamicObjects[objectIndex].output = 2;
    }
}

void kwinit_fire_wall(s32 objectIndex, Vec3s arg1, s32 arg2) {
    AnmObject* object;

    KWAnmStart(objectIndex, 0);
    object = &g_DynamicObjects[objectIndex];
    object->chartptr = 9;
    object->scale = 1.0f;
    object->pos_start[0] = arg1[0];
    object->pos_start[1] = arg1[1];
    object->pos_start[2] = arg1[2];
    object->ang[0] = 0x0C00;
    object->ang[1] = 0x2100;
    object->ang[2] = 0;
    object->output = 0x00FF;
    object->input = 0x00FF;
    object->vel = 8.0f;
    object->velocity[1] = 8.0f;
    object->ival = arg2;
}

s32 kwget_fire_wall_sub(Vec3s arg0, s32 arg1) {
    s32 objectIndex;

    objectIndex = kwalloc_effectram(EffectAllocArray1, &gNextFreeObjectParticle1, gObjectParticle1_SIZE);
    if (objectIndex != NULL_OBJECT_ID) {
        kwinit_fire_wall(objectIndex, arg0, arg1);
    }
    return objectIndex;
}

void kwget_fire_wall(s32 arg0) {
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
        if (kwget_fire_wall_sub(var_s2, i) == (-1)) {
            break;
        }
    }
}

void kwanm_fire_wall_init(s32 objectIndex) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    object->indexaddr = common_texture_particle_smoke[0];
    object->sindexaddr = common_texture_particle_smoke[0];
    object->alpha = 0x00FF;
    kwset_init3D_texang(objectIndex, 0U, 0U, 0U);
    kwset_init3D_sppos(objectIndex, 0.0f, 0.0f, 0.0f);
    kwanm_next(objectIndex);
}

void kwanm_fire_wall(s32 objectIndex) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    switch (object->anmptr) {
        case 0:
            break;
        case 1:
            kwanm_fire_wall_init(objectIndex);
            break;
        case 2:
            if (kwanm_wait(objectIndex, object->ival) != 0) {
                kwmv_start(objectIndex);
            }
            break;
        case 3:
            KWChaseFVal(&object->scale, 2.0f, 0.05f);
            KWChaseSVal(&object->output, 0, 0x0018);
            if ((object->mvptr >= 2) &&
                (kwsetsval_reverse(objectIndex, &object->alpha, 0x000000FF, 0x00000050, 0x00000020, 0, 0) != 0)) {
                kwanm_next(objectIndex);
            }
            break;
        case 4:
            kwanm_finish(objectIndex);
            kwmv_finish(objectIndex);
            break;
    }
}

void kwmv_fire_wall(s32 objectIndex) {
    s32 thing;
    thing = g_DynamicObjects[objectIndex].mvptr;
    if (thing) {}
    if (thing != 0) {
        if (thing == 1) {
            if (kwmv_wait(objectIndex, 0x0000000A) != 0) {
                KWAnmNext(objectIndex);
            }
        } else {
            thing = g_DynamicObjects[objectIndex].mvptr;
        }
    }
    if (g_DynamicObjects[objectIndex].mvptr > 0) {
        kwmv_addvelytopos(objectIndex);
        kwset_splinepos3D(objectIndex);
    }
}

void kwcheck_fire_wall(void) {
}

void kwchart_fire(void) {
    s32 someIndex;
    s32 objectIndex;
    AnmObject* object;

    kwcheck_koopa_fire();
    kwcheck_kokoopa_fire();
    for (someIndex = 0; someIndex < gObjectParticle1_SIZE; someIndex++) {
        objectIndex = EffectAllocArray1[someIndex];
        if (objectIndex != DELETED_OBJECT_ID) {
            object = &g_DynamicObjects[objectIndex];
            if (object->anmptr != 0) {
                if ((object->chartptr == 4) || (object->chartptr == 5)) {
                    kwanm_koopa_fire(objectIndex);
                    kwmv_koopa_fire(objectIndex);
                } else if (object->chartptr == 9) {
                    kwanm_fire_wall(objectIndex);
                    kwmv_fire_wall(objectIndex);
                }
                if (object->anmptr == 0) {
                    kwdealloc_effectram(&EffectAllocArray1[someIndex]);
                }
            }
        }
    }
}

void kwget_kame_fire_init(s32 objectIndex, Vec3f arg1, s16 arg2) {
    AnmObject* object;

    KWAnmStart(objectIndex, (s32) arg2);
    object = &g_DynamicObjects[objectIndex];
    object->chartptr = 0x0A;
    object->indexaddr = common_texture_particle_smoke[0];
    object->sindexaddr = common_texture_particle_smoke[0];
    object->scale = 0.3f;
    kwset_init3D_spos(objectIndex, arg1[0], arg1[1], arg1[2]);
    object->output = 0x00FF;
    object->vel = 0.0f;
    kwset_init3D_texang(objectIndex, 0U, 0U, 0U);
    kwset_init3D_sppos(objectIndex, 0.0f, 0.0f, 0.0f);
}

void kwget_hibasira(Vec3f arg0, UNUSED f32 arg1, s16 arg2) {
    s32 objectIndex;

    objectIndex = kwalloc_effectram(FireParticleAllocArray, &FireParticleCounter, gObjectParticle4_SIZE);
    if (objectIndex != NULL_OBJECT_ID) {
        kwget_kame_fire_init(objectIndex, arg0, arg2);
    }
}

void kwanm_kame_fire_init(s32 objectIndex) {
    kwanm_next(objectIndex);
    if (g_DynamicObjects[objectIndex].chartptr != 0x0B) {
        kwmv_start(objectIndex);
    }
}

void kwanm_kame_fire(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 1:
            kwanm_kame_fire_init(objectIndex);
            break;
        case 2:
            if ((g_DynamicObjects[objectIndex].mvptr >= 2) &&
                (kwsetsval_reverse(objectIndex, &g_DynamicObjects[objectIndex].alpha, 0x000000FF, 0x00000050, 0x00000020, 0,
                               0) != 0)) {
                kwanm_next(objectIndex);
            }
            break;
        case 0:
            break;
        case 3:
            kwanm_finish(objectIndex);
            kwmv_finish(objectIndex);
            break;
    }
}

void kwmv_kame_fire(s32 objectIndex) {
    if ((g_DynamicObjects[objectIndex].mvptr != 0) && (g_DynamicObjects[objectIndex].mvptr == 1)) {
        KWAnmNext(objectIndex);
    }
    kwset_splinepos3D(objectIndex);
}

void kwchart_kame_fire(void) {
    s32 someIndex;
    s32 flameIndex;
    AnmObject* object;

    for (someIndex = 0; someIndex < gObjectParticle4_SIZE; someIndex++) {
        flameIndex = FireParticleAllocArray[someIndex];
        if (flameIndex != DELETED_OBJECT_ID) {
            object = &g_DynamicObjects[flameIndex];
            if (object->anmptr != 0) {
                kwanm_kame_fire(flameIndex);
                kwmv_kame_fire(flameIndex);
                if (object->anmptr == 0) {
                    kwdealloc_effectram(&FireParticleAllocArray[someIndex]);
                }
            }
        }
    }
}

void kwget_cave_fire_init(s32 objectIndex, s32 flameIndex) {
    KWAnmStart(objectIndex, 3);

    g_DynamicObjects[objectIndex].chartptr = 0xB;
    g_DynamicObjects[objectIndex].indexaddr = common_texture_particle_smoke[0];
    g_DynamicObjects[objectIndex].sindexaddr = common_texture_particle_smoke[0];
    g_DynamicObjects[objectIndex].scale = 0.8f;

    g_DynamicObjects[objectIndex].pos_start[0] = (f32) * (CaveFirePos + (flameIndex * 3) + 0) * xOrientation;
    g_DynamicObjects[objectIndex].pos_start[1] = (f32) * (CaveFirePos + (flameIndex * 3) + 1);
    g_DynamicObjects[objectIndex].pos_start[2] = (f32) * (CaveFirePos + (flameIndex * 3) + 2);
    g_DynamicObjects[objectIndex].vel = 0;
    g_DynamicObjects[objectIndex].output = 255;
    g_DynamicObjects[objectIndex].input = 255;
    g_DynamicObjects[objectIndex].alpha = 255;
    kwset_init3D_texang(objectIndex, 0, 0, 0);
    kwset_init3D_sppos(objectIndex, 0, 0, 0);
}

void KWGetCaveFire(s32 arg0) {
    s32 objectIndex;

    objectIndex = kwalloc_effectram(FireParticleAllocArray, &FireParticleCounter, gObjectParticle4_SIZE);
    if (objectIndex != NULL_OBJECT_ID) {
        kwget_cave_fire_init(objectIndex, arg0);
    }
}

void kwget_star_init(s32 objectIndex, Vec3f arg1, s32 arg2) {
    s8 temp_v0_3;
    Vec3s sp30;

    KWAnmStart(objectIndex, arg2);
    g_DynamicObjects[objectIndex].chartptr = 0x0C;
    g_DynamicObjects[objectIndex].scale = 0.05f;
    kwset_init3D_spos(objectIndex, arg1[0], arg1[1], arg1[2]);
    kwset_init3D_texang(objectIndex, 0U, 0U, 0U);
    kwset_init3D_sppos(objectIndex, 0.0f, 0.0f, 0.0f);
    switch (arg2) {
        case 0:
            g_DynamicObjects[objectIndex].velocity[1] = -1.0f;
            g_DynamicObjects[objectIndex].vel = (f32) ((MakeRandomLimmit(0x004BU) * 0.01) + 0.25);
            g_DynamicObjects[objectIndex].ang[1] = MakeRandomLimmit(0x0040U) << 0xA;
            kwmv_vxz(objectIndex);
            g_DynamicObjects[objectIndex].stack[5] = 0x001E;
            break;
        case 1:
            g_DynamicObjects[objectIndex].velocity[1] = 1.5f;
            g_DynamicObjects[objectIndex].vel = (f32) ((MakeRandomLimmit(0x0064U) * 0.01) + 0.5);
            g_DynamicObjects[objectIndex].ang[1] = MakeRandomLimmit(0x0040U) << 0xA;
            kwmv_vxz(objectIndex);
            g_DynamicObjects[objectIndex].stack[5] = 0x0032;
            break;
    }
    temp_v0_3 = MakeRandomLimmit(0x000CU);
    if (temp_v0_3 < 9) {
        flashcolor_table(temp_v0_3, &sp30[2], &sp30[1], sp30);
        g_DynamicObjects[objectIndex].ival = 0;
        g_DynamicObjects[objectIndex].stack[0] = sp30[2];
        g_DynamicObjects[objectIndex].stack[1] = sp30[1];
        g_DynamicObjects[objectIndex].stack[2] = sp30[0];
    } else {
        temp_v0_3 = MakeRandomLimmit(3U);
        kwget_color(temp_v0_3, &sp30[2], &sp30[1], sp30);
        g_DynamicObjects[objectIndex].stack[0] = sp30[2];
        g_DynamicObjects[objectIndex].stack[1] = sp30[1];
        g_DynamicObjects[objectIndex].stack[2] = sp30[0];
        g_DynamicObjects[objectIndex].stack[4] = temp_v0_3;
        g_DynamicObjects[objectIndex].ival = 1;
    }
    g_DynamicObjects[objectIndex].alpha = 0x00FF;
    g_DynamicObjects[objectIndex].stack[3] = MakeRandomLimmit(0x0800U) + 0x400;
    if ((g_DynamicObjects[objectIndex].ang[1] < 0x3000) ||
        (g_DynamicObjects[objectIndex].ang[1] >= 0xB001)) {
        g_DynamicObjects[objectIndex].stack[3] = -g_DynamicObjects[objectIndex].stack[3];
    }
}

void KWGetStar(f32* arg0, s32 arg1) {
    s32 objectIndex = kwalloc_effectram(EffectAllocArray3, &gNextFreeObjectParticle3, gObjectParticle3_SIZE);
    if (objectIndex != NULL_OBJECT_ID) {
        kwget_star_init(objectIndex, arg0, arg1);
    }
}

void kwanm_star_init(s32 arg0) {
    kwanm_next(arg0);
    kwmv_start(arg0);
}

void kwanm_star(s32 objectIndex) {
    UNUSED s16 stackPadding0;
    s16 sp3C;
    s16 sp3A;
    s16 sp38;

    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 0:
            break;
        case 1:
            kwanm_star_init(objectIndex);
            break;
        case 2:
            kwadd_fval(&g_DynamicObjects[objectIndex].scale, 0.1f, 0.01f);
            if ((g_DynamicObjects[objectIndex].pos[1] <= g_DynamicObjects[objectIndex].stack[5]) &&
                (kwsetsval_reverse(objectIndex, &g_DynamicObjects[objectIndex].alpha, 0x000000FF, 0, 0x00000010, 0, 0) !=
                 0)) {
                kwmv_finish(objectIndex);
                kwanm_finish(objectIndex);
            }
            break;
    }
    if (g_DynamicObjects[objectIndex].ival != 0) {
        g_DynamicObjects[objectIndex].stack[4] = (s16) ((s32) (g_DynamicObjects[objectIndex].stack[4] + 1) % 3);
        kwget_color(g_DynamicObjects[objectIndex].stack[4], &sp3C, &sp3A, &sp38);
        g_DynamicObjects[objectIndex].stack[0] = sp3C;
        g_DynamicObjects[objectIndex].stack[1] = sp3A;
        g_DynamicObjects[objectIndex].stack[2] = sp38;
    }
}

void kwmv_star(s32 objectIndex) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    if ((object->mvptr != 0) && (object->mvptr == 1) && ((u8) object->texsizex != 0)) {
        if (object->velocity[1] >= -0.5) {
            object->velocity[1] -= 0.15;
        } else {
            object->velocity[2] = 0.0f;
            object->velocity[0] = 0.0f;
        }
    }
    object->texang[2] += object->stack[3];
    kwmv_addveltopos(objectIndex);
    kwset_splinepos3D(objectIndex);
}

void KWChartStar(void) {
    s32 someIndex;
    s32 objectIndex;
    AnmObject* object;

    for (someIndex = 0; someIndex < gObjectParticle3_SIZE; someIndex++) {
        objectIndex = EffectAllocArray3[someIndex];
        if (objectIndex != DELETED_OBJECT_ID) {
            object = &g_DynamicObjects[objectIndex];
            if (object->anmptr != 0) {
                kwanm_star(objectIndex);
                kwmv_star(objectIndex);
                if (object->anmptr == 0) {
                    kwdealloc_effectram(&EffectAllocArray3[someIndex]);
                }
            }
        }
    }
}

void kwinit_tree_leaf(s32 objectIndex, Vec3f arg1, s32 num) {
    UNUSED s32 stackPadding1;
    UNUSED u16 stackPadding0;
    u16 temp_s0;
    u16 sp3E;
    u16 sp3C;

    KWAnmStart(objectIndex, 0);
    g_DynamicObjects[objectIndex].chartptr = 7;
    g_DynamicObjects[objectIndex].texaddr = (u8*) common_texture_particle_leaf;
    g_DynamicObjects[objectIndex].stexaddr = (u8*) common_texture_particle_leaf;
    g_DynamicObjects[objectIndex].scale = 0.1f;
    g_DynamicObjects[objectIndex].ground = arg1[1];
    switch (g_courseID) {
        case COURSE_MARIO_RACEWAY:
            kwset_randomxyz(objectIndex, arg1[0], arg1[1] + 25.0, arg1[2], 0x14, 0x1E, 0x14);
            g_DynamicObjects[objectIndex].vel = 1.5f;
            g_DynamicObjects[objectIndex].velocity[1] = 1.5f;
            break;
        case COURSE_YOSHI_VALLEY:
            kwset_randomxyz(objectIndex, arg1[0], arg1[1] + 25.0, arg1[2], 0x14, 0x1E, 0x14);
            g_DynamicObjects[objectIndex].vel = 2.0f;
            g_DynamicObjects[objectIndex].velocity[1] = 2.0f;
            break;
        case COURSE_ROYAL_RACEWAY:
            kwset_randomxyz(objectIndex, arg1[0], arg1[1] + 30.0, arg1[2], 0x10, 0x28, 0x10);
            g_DynamicObjects[objectIndex].vel = 2.0f;
            g_DynamicObjects[objectIndex].velocity[1] = 2.0f;
            break;
        case COURSE_LUIGI_RACEWAY:
            kwset_randomxyz(objectIndex, arg1[0], arg1[1] + 25.0, arg1[2], 0x14, 0x1E, 0x14);
            g_DynamicObjects[objectIndex].vel = 1.5f;
            g_DynamicObjects[objectIndex].velocity[1] = 1.0f;
            break;
    }
    temp_s0 = MakeRandomLimmit(0x0010U) << 0xC;
    sp3E = MakeRandomLimmit(0x0010U) << 0xC;
    sp3C = MakeRandomLimmit(0x0010U) << 0xC;
    kwset_init3D_ang(objectIndex, 0U, (num * 0xFFFF) / 20, 0U);
    kwset_init3D_texang(objectIndex, temp_s0, sp3E, sp3C);
}

s32 kwget_tree_leaf(Vec3f arg0, s32 num) {
    s32 objectIndex;

    objectIndex = kwalloc_effectram(leafallocptr, &gNextFreeLeafParticle, gLeafParticle_SIZE);
    if (objectIndex != NULL_OBJECT_ID) {
        kwinit_tree_leaf(objectIndex, arg0, num);
    }
    return objectIndex;
}

void kwhit_tree(Vec3f arg0, UNUSED s32 arg1) {
    s32 i;

    for (i = 0; i < gLeafParticle_SPAWN_SIZE; i++) {
        if (kwget_tree_leaf(arg0, i) == NULL_OBJECT_ID) {
            break;
        }
    }
}

void kwanm_fallen_leaf_init(s32 arg0) {
    kwanm_next(arg0);
    kwmv_start(arg0);
}

void kwanm_fallen_leaf(s32 objectIndex) {
    AnmObject* object;
    object = &g_DynamicObjects[objectIndex];

    switch (object->anmptr) {
        case 0:
            break;
        case 1:
            kwanm_fallen_leaf_init(objectIndex);
            break;
        case 2:
            if (object->mvptr == 0) {
                kwanm_next(objectIndex);
            }
            break;
        case 3:
            kwanm_finish(objectIndex);
            break;
    }
}

void kwmv_fallen_leaf(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].mvptr) {
        case 1:
            kwmv_nanamenage(objectIndex, g_DynamicObjects[objectIndex].velocity[1], 0.2f, g_DynamicObjects[objectIndex].vel,
                          (s16) (s32) g_DynamicObjects[objectIndex].ang[1], 0x0000000A);
            break;
        case 2:
            if (kwmv_trakka_ground(objectIndex, 0.4f, g_DynamicObjects[objectIndex].ground) != 0) {
                kwmv_finish(objectIndex);
            }
            break;
    }
    kwset_splinepos3D(objectIndex);
    g_DynamicObjects[objectIndex].texang[0] += 0x1000;
    g_DynamicObjects[objectIndex].texang[1] += 0x1000;
}

void kwchart_leaf(void) {
    s32 someIndex;
    s32 leafIndex;
    AnmObject* object;

    for (someIndex = 0; someIndex < gLeafParticle_SIZE; someIndex++) {
        leafIndex = leafallocptr[someIndex];
        if (leafIndex != DELETED_OBJECT_ID) {
            object = &g_DynamicObjects[leafIndex];
            if (object->anmptr != 0) {
                kwanm_fallen_leaf(leafIndex);
                kwmv_fallen_leaf(leafIndex);
                if (object->anmptr == 0) {
                    kwdealloc_effectram(&leafallocptr[someIndex]);
                }
            }
        }
    }
}

void kwanm_snow_search_2D(s32 arg0) {
    s32 objectIndex;
    s32 var_a1;

    if (KW2GFTimer == 0) {
        for (var_a1 = 0; var_a1 < CloudCount1P; var_a1++) {
            D_8018D17C += 1;
            if (D_8018D17C >= CloudCount1P) {
                D_8018D17C = 0;
            }
            objectIndex = g_CloudAllocate[arg0 + D_8018D17C];
            if (g_DynamicObjects[objectIndex].anmptr == 0) {
                KWAnmStart(objectIndex, 1);
                break;
            }
        }
    }
}

void kwanm_snow2D_init(s32 objectIndex) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    object->indexaddr = D_0D0293D8;
    object->sindexaddr = D_0D0293D8;
    // There's something up with the handling of common_vtx_rectangle and the loading of 0x10 right here
    object->vtxaddr = common_vtx_rectangle;
    object->fvalptr = 0x10;
    object->texsizey = object->fvalptr;
    object->scale = 0.15f;
    kwanm_setflag(objectIndex, 0x00000010);
    kwmv_start_strategy(objectIndex);
    object->alpha = 0x00FF;
    object->chartptr = 0;
    object->output = 0;
    kwanm_next(objectIndex);
}

void kwanm_visiblecheck_snow(s32 objectIndex, u16 arg1, Camera* camera) {
    s16 temp_v0;

    temp_v0 = camera->camera_direction[1] - arg1;
    if ((temp_v0 >= D_8018D210) || (D_8018D208 >= temp_v0)) {
        g_DynamicObjects[objectIndex].pos_spline[0] = D_8018D218 + (D_8018D1E8 * (f32) temp_v0);
        kwanm_setflag(objectIndex, 0x00000010);
        return;
    }
    kwanm_clrflag(objectIndex, 0x00000010);
}

void kwmv_snow2D(s32 objectIndex, Camera* camera) {

    f64 rand;

    switch (g_DynamicObjects[objectIndex].mvptr) { /* irregular */
        case 1:
            g_DynamicObjects[objectIndex].ang[1] = (camera->camera_direction[1] + MakeRandomLimmit(0x4000U)) - DEGREES(45);
            kwset_randomy(objectIndex, 0x00B4, 0x0014U);
            rand = MakeRandomLimmit(0x0064U);

            g_DynamicObjects[objectIndex].velocity[1] = (f32) (-0.75 - (f64) (f32) (rand * 0.01));
            g_DynamicObjects[objectIndex].pos_spline[0] = 0.0f;
            g_DynamicObjects[objectIndex].pos_spline[1] = 0.0f;
            KWAnmNext(objectIndex);
            return;
        case 2:
            kwanm_visiblecheck_snow(objectIndex, g_DynamicObjects[objectIndex].ang[1], camera);
            kwmv_addvelytopos(objectIndex);
            kwset_splinepos3D(objectIndex);
            kwset_pos3Dto2D(objectIndex);
            if (g_DynamicObjects[objectIndex].pos[1] <= 0.0f) {
                KWAnmNext(objectIndex);
                return;
            }
        case 0:
            return;
        case 3:
            kwmv_finish(objectIndex);
            break;
    }
}

void kwanm_snow2D(s32 objectIndex, Camera* camera) {
    switch (g_DynamicObjects[objectIndex].anmptr) { /* irregular */
        case 1:
            kwanm_snow2D_init(objectIndex);
            return;
        case 2:
            kwmv_snow2D(objectIndex, camera);
            if (g_DynamicObjects[objectIndex].mvptr == 0) {
                kwanm_next(objectIndex);
                return;
            }
        case 0:
            return;
        case 3:
            kwanm_finish(objectIndex);
            break;
    }
}

void kwchart_snow2D(s32 arg0, Camera* arg1) {
    s32 objectIndex;
    s32 i;

    kwanm_snow_search_2D(arg0);
    for (i = 0; i < CloudCount1P; i++) {
        objectIndex = g_CloudAllocate[arg0 + i];
        if (g_DynamicObjects[objectIndex].anmptr != 0) {
            kwanm_snow2D(objectIndex, arg1);
        }
    }
}

void kwanm_snow_3D_init(s32 objectIndex) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    object->indexaddr = D_0D0293D8;
    object->sindexaddr = D_0D0293D8;
    object->vtxaddr = common_vtx_rectangle;
    object->scale = 0.15f;
    kwmv_start_strategy(objectIndex);
    kwanm_next(objectIndex);
}

void kwmv_snow_3D(s32 objectIndex) {
    s16 sp3E;
    s16 sp3C;
    s16 sp3A;
    UNUSED u16 pad;
    u16 temp_t6;

    switch (g_DynamicObjects[objectIndex].mvptr) { /* irregular */
        case 0:
            break;
        case 1:
            if (gGamestate != 9) {
                sp3A = ((gPlayerOneCopy->speed / 18) * 216) / 2;
                sp3E = (MakeRandomLimmit(0x000FU) - sp3A) + 0x2D;
                sp3C = MakeRandomLimmit(0x012CU) + 0x1E;
                temp_t6 = camera1->camera_direction[1] + ((s32) (MakeRandomLimmit(0x3000U) - 0x1800) / (s16) ((sp3A / 15) + 1));
                g_DynamicObjects[objectIndex].pos_start[0] = gPlayerOneCopy->position[0] + (sinT(temp_t6) * sp3C);
                g_DynamicObjects[objectIndex].pos_start[1] = sp3E + gPlayerOneCopy->ground;
                g_DynamicObjects[objectIndex].pos_start[2] = gPlayerOneCopy->position[2] + (cosT(temp_t6) * sp3C);
                g_DynamicObjects[objectIndex].addangy = MakeRandomLimmit(0x0400U) + 0x100;
                g_DynamicObjects[objectIndex].pos_ende[0] = (f32) (((f32) MakeRandomLimmit(0x0064U) * 0.03) + 2.0);
                g_DynamicObjects[objectIndex].velocity[1] = (f32) (-0.3 - (f64) (f32) (MakeRandomLimmit(0x0032U) * 0.01));
                g_DynamicObjects[objectIndex].pos_spline[0] = 0.0f;
                g_DynamicObjects[objectIndex].pos_spline[1] = 0.0f;
                KWAnmNext(objectIndex);
            } else {
                sp3C = MakeRandomLimmit(0x0064U) + 0x28;
                temp_t6 = camera1->camera_direction[1] + MakeRandomLimmit(0x3000U) - 0x1800;
                g_DynamicObjects[objectIndex].pos_start[0] = camera1->camera_pos[0] + (sinT(temp_t6) * sp3C);
                g_DynamicObjects[objectIndex].pos_start[1] = camera1->camera_pos[1] + 45.0;
                g_DynamicObjects[objectIndex].pos_start[2] = camera1->camera_pos[2] + (cosT(temp_t6) * sp3C);
                g_DynamicObjects[objectIndex].addangy = MakeRandomLimmit(0x0400U) + 0x100;
                g_DynamicObjects[objectIndex].pos_ende[0] = (f32) (((f32) MakeRandomLimmit(0x0064U) * 0.03) + 2.0);
                g_DynamicObjects[objectIndex].velocity[1] = (f32) (-0.6 - (f64) (f32) (MakeRandomLimmit(0x0032U) * 0.01));
                g_DynamicObjects[objectIndex].pos_spline[0] = 0.0f;
                g_DynamicObjects[objectIndex].pos_spline[1] = 0.0f;
                KWAnmNext(objectIndex);
            }
            break;
        case 2:
            kwmv_addvelytopos(objectIndex);
            g_DynamicObjects[objectIndex].ang[0] += g_DynamicObjects[objectIndex].addangy;
            g_DynamicObjects[objectIndex].pos_spline[0] =
                sinT(g_DynamicObjects[objectIndex].ang[0]) * g_DynamicObjects[objectIndex].pos_ende[0];
            kwset_splinepos3D(objectIndex);
            if ((f64) g_DynamicObjects[objectIndex].pos[1] <= 0.0) {
                KWAnmNext(objectIndex);
            }
            g_DynamicObjects[objectIndex].texang[1] = kwlookcamera_xz(objectIndex, camera1);
            SnowCustomCheck(objectIndex);
            break;
        case 3:
            kwmv_finish(objectIndex);
            break;
    }
}

void kwanm_snow_3D(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].anmptr) { /* irregular */
        case 0:
            break;
        case 1:
            kwanm_snow_3D_init(objectIndex);
            break;
        case 2:
            kwmv_snow_3D(objectIndex);
            if (g_DynamicObjects[objectIndex].mvptr == 0) {
                kwanm_next(objectIndex);
            }
            break;
        case 3:
            kwanm_finish(objectIndex);
            break;
    }
}

void kwanm_snow_search_3D(void) {
    s32 someIndex;

    for (someIndex = 0; someIndex < NUM_SNOWFLAKES; someIndex++) {
        D_8018D174 += 1;
        if (D_8018D174 >= NUM_SNOWFLAKES) {
            D_8018D174 = 0;
        }
        if (g_DynamicObjects[EffectAllocArray1[D_8018D174]].anmptr == 0) {
            KWAnmStart(EffectAllocArray1[D_8018D174], 1);
            break;
        }
    }
}

void KWChartSnow(void) {
    s32 someIndex;
    s32 snowflakeIndex;

    if (gGamestate != CREDITS_SEQUENCE) {
        kwanm_snow_search_3D();
    } else {
        kwanm_snow_search_3D();
        kwanm_snow_search_3D();
        kwanm_snow_search_3D();
        kwanm_snow_search_3D();
    }
    for (someIndex = 0; someIndex < NUM_SNOWFLAKES; someIndex++) {
        snowflakeIndex = EffectAllocArray1[someIndex];
        if (g_DynamicObjects[snowflakeIndex].anmptr != 0) {
            kwanm_snow_3D(snowflakeIndex);
        }
    }
}

void kwanm_visiblecheck_kumo(s32 objectIndex, u16 rot, Camera* camera) {
    s16 temp_v0;

    temp_v0 = camera->camera_direction[1] + rot;
    if ((temp_v0 >= D_8018D210) && (D_8018D208 >= temp_v0)) {
        g_DynamicObjects[objectIndex].ix = (D_8018D218 + (D_8018D1E8 * temp_v0));
        kwanm_setflag(objectIndex, 0x00000010);
        return;
    }
    kwanm_clrflag(objectIndex, 0x00000010);
}

void kwchart_kumo_kt(s32 arg0, Camera* arg1, CloudData* cloudList) {
    s32 cloudIndex;
    s32 objectIndex;
    CloudData* cloud;

    for (cloudIndex = 0; cloudIndex < CloudCount1P; cloudIndex++) {
        cloud = &cloudList[cloudIndex];
        objectIndex = g_CloudAllocate[arg0 + cloudIndex];
        kwanm_visiblecheck_kumo(objectIndex, cloud->rotY, arg1);
    }
}

void kwchart_star(s32 arg0, Camera* camera, StarData* starList) {
    s32 starIndex;
    s32 objectIndex;
    StarData* star;

    for (starIndex = 0; starIndex < CloudCount1P; starIndex++) {
        star = &starList[starIndex];
        objectIndex = g_CloudAllocate[arg0 + starIndex];
        kwanm_visiblecheck_kumo(objectIndex, star->rotY, camera);
        switch (starIndex % 5U) {
            case 0:
                kwsetsval_oufuku_f(objectIndex, &g_DynamicObjects[objectIndex].alpha, 0x00000028, 0x000000B4, 0x000000FF, 0,
                              -1);
                break;
            case 1:
                kwsetsval_oufuku_f(objectIndex, &g_DynamicObjects[objectIndex].alpha, 0x00000080, 0x000000FF, 0x000000FF, 0,
                              -1);
                break;
            case 2:
                kwsetsval_oufuku_f(objectIndex, &g_DynamicObjects[objectIndex].alpha, 0x00000050, 0x000000C8, 0x000000FF, 0,
                              -1);
                break;
            case 3:
                kwsetsval_oufuku_f(objectIndex, &g_DynamicObjects[objectIndex].alpha, 0, 0x0000009B, 0x000000FF, 0, -1);
                break;
            case 4:
                kwsetsval_oufuku_f(objectIndex, &g_DynamicObjects[objectIndex].alpha, 0x0000005A, 0x00000080, 0x000000FF, 0,
                              -1);
                break;
        }
    }
}

UNUSED void func_80078C68() {
}

void KWChart_Kumo(s32 arg0) {
    s32 sp1C;
    Camera* camera;

    if (g_cloudsToggle == 0) {
        switch (arg0) { /* switch 1 */
            case 0:     /* switch 1 */
                sp1C = 0;
                camera = camera1;
                D_8018D200 = gCameraZoom[0] + 40.0f;
                break;
            case 1: /* switch 1 */
                sp1C = 0;
                camera = camera1;
                D_8018D200 = gCameraZoom[0] + 40.0f;
                break;
            case 2: /* switch 1 */
                camera = camera2;
                sp1C = CloudCount1P;
                D_8018D200 = gCameraZoom[1] + 40.0f;
                break;
            case 3: /* switch 1 */
                sp1C = 0;
                camera = camera1;
                D_8018D200 = gCameraZoom[0] + 40.0f;
                break;
            case 4: /* switch 1 */
                camera = camera2;
                sp1C = CloudCount1P;
                D_8018D200 = gCameraZoom[1] + 40.0f;
                break;
        }

        D_8018D208 = ((D_8018D200 / 2) * DEGREES(1)) + DEGREES(10);
        D_8018D210 = (-(D_8018D200 / 2) * DEGREES(1)) - DEGREES(10);
        D_8018D1E8 = 1.7578125 / D_8018D200;
        D_8018D218 = 0xA0;
#if !ENABLE_CUSTOM_COURSE_ENGINE
        switch (g_courseID) {    /* switch 2 */
            case COURSE_MARIO_RACEWAY: /* switch 2 */
                // Uses Luigi Raceway's clouds for display purposes?
                kwchart_kumo_kt(sp1C, camera, gLuigiRacewayClouds);
                break;
            case COURSE_YOSHI_VALLEY: /* switch 2 */
                kwchart_kumo_kt(sp1C, camera, gYoshiValleyMooMooFarmClouds);
                break;
            case COURSE_FRAPPE_SNOWLAND: /* switch 2 */
                kwchart_snow2D(sp1C, camera);
                break;
            case COURSE_KOOPA_BEACH: /* switch 2 */
                kwchart_kumo_kt(sp1C, camera, gKoopaTroopaBeachClouds);
                break;
            case COURSE_ROYAL_RACEWAY: /* switch 2 */
                kwchart_kumo_kt(sp1C, camera, gRoyalRacewayClouds);
                break;
            case COURSE_LUIGI_RACEWAY: /* switch 2 */
                kwchart_kumo_kt(sp1C, camera, gLuigiRacewayClouds);
                break;
            case COURSE_MOO_MOO_FARM: /* switch 2 */
                kwchart_kumo_kt(sp1C, camera, gYoshiValleyMooMooFarmClouds);
                break;
            case COURSE_TOADS_TURNPIKE: /* switch 2 */
                kwchart_star(sp1C, camera, gToadsTurnpikeRainbowRoadStars);
                break;
            case COURSE_KALAMARI_DESERT: /* switch 2 */
                kwchart_kumo_kt(sp1C, camera, gKalimariDesertClouds);
                break;
            case COURSE_SHERBET_LAND: /* switch 2 */
                kwchart_kumo_kt(sp1C, camera, gSherbetLandClouds);
                break;
            case COURSE_RAINBOW_ROAD: /* switch 2 */
                kwchart_star(sp1C, camera, gToadsTurnpikeRainbowRoadStars);
                break;
            case COURSE_WARIO_STADIUM: /* switch 2 */
                kwchart_star(sp1C, camera, gWarioStadiumStars);
                break;
        }
    }
#else

#endif
}

void kwjugemustart(void) {
    switch (g_ScreenSplitB) { /* irregular */
        case SCREEN_MODE_1P:
            KWAnmStart(jugemuallocptr[0], 1);
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            KWAnmStart(jugemuallocptr[0], 1);
            KWAnmStart(jugemuallocptr[1], 1);
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            KWAnmStart(jugemuallocptr[0], 1);
            KWAnmStart(jugemuallocptr[1], 1);
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            KWAnmStart(jugemuallocptr[0], 1);
            KWAnmStart(jugemuallocptr[1], 1);
            KWAnmStart(jugemuallocptr[2], 1);
            KWAnmStart(jugemuallocptr[3], 1);
            break;
    }
}

void kwgoal_jugemu(s32 playerId) {
    KWAnmStart(jugemuallocptr[playerId], 2);
}

void kwlap2_jugemu(s32 playerId) {
    KWAnmStart(jugemuallocptr[playerId], 4);
}

void kwfinal_jugemu(s32 playerId) {
    KWAnmStart(jugemuallocptr[playerId], 5);
}

void kwreverse_jugemu(s32 playerId) {
    KWAnmStart(jugemuallocptr[playerId], 6);
}

void kwanm_dmajgm(s32 objectIndex, s32 arg1, s32 arg2) {
    s32 a;
    if (g_DynamicObjects[objectIndex].anmptr >= 2) {
        if ((u8) g_DynamicObjects[objectIndex].texsizex == 1) {
            if (arg1 == 0) {
                kwanm_texaddr_ci8_dma_1pal(objectIndex, gLakituTexturePtr);
                return;
            }
            a = jugemuallocptr[0];
            g_DynamicObjects[objectIndex].texaddr = g_DynamicObjects[a].texaddr;
            g_DynamicObjects[objectIndex].indexaddr = g_DynamicObjects[a].indexaddr;
            if (0) {}
            return;
        }
        switch (arg2) {
            case 0:
                fuusen_addcoltbl(objectIndex, gLakituTexturePtr);
                break;
            case 1:
                fuusen_coltbl(objectIndex, gLakituTexturePtr);
                break;
            case 2:
                kwanm_texaddr_ci8_dma_1pal(objectIndex, gLakituTexturePtr);
                break;
        }
    }
}

void kwanm_jgm_break(s32 objectIndex, s32 playerId) {
    Player* player = &gPlayerOne[playerId];

    if ((g_DynamicObjects[objectIndex].texsizex != 3) && (g_DynamicObjects[objectIndex].texsizex != 7)) {
        kwanm_clrevflag(objectIndex, 1);
        if (g_courseID == COURSE_SHERBET_LAND) {
            player->jugemu_flag &= ~IS_FROZEN;
        }
    } else {
        // ?????
    }
    if (g_courseID == COURSE_SHERBET_LAND) {
        kwanm_clrevflag(objectIndex, 0x00000010);
        player->jugemu_flag &= ~IS_MELTING;
    }
    NaPlyLevelStop(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x28));
}

void kwanm_jgm_s_init(s32 objectIndex, s32 arg1) {
    if (arg1 == 0) {
        D_801656F0 = 0;
        D_8018D168 = 0;
    }
    kwanm_init_ci8(objectIndex, (u8*) common_tlut_lakitu_countdown, gTextureLakituNoLights1, 0x38U,
                        (u16) 0x00000048);
    g_DynamicObjects[objectIndex].vtxaddr = common_vtx_lakitu;
    g_DynamicObjects[objectIndex].scale = 0.15f;
    kwanm_clrflag(objectIndex, 0x00000010);
    kwanm_next(objectIndex);
    g_DynamicObjects[objectIndex].ival = D_8018D180;
}

void kwanm_jgm_s(s32 objectIndex, s32 arg1) {
    UNUSED s32 pad;
    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 0:
            break;
        case 1:
            kwanm_jgm_s_init(objectIndex, arg1);
            break;
        case 2:
            kwanm_wait(objectIndex, g_DynamicObjects[objectIndex].ival);
            if ((g_DynamicObjects[objectIndex].anmtm == 0x00000055) && (g_menuMultiplayerSelection == 3) && (arg1 == 0)) {
                D_8018D168 = 1;
            }
            break;
        case 3:
            kwanm_setflag(objectIndex, 0x00000010);
            kwmv_start_spline(objectIndex, 1, &D_800E67B8); // set a spline
            kwanm_next(objectIndex);
            break;
        case 4:
            if ((kwanm_wait(objectIndex, 0x0000001E) != false) && (g_menuMultiplayerSelection != 3) && (arg1 == 0)) {
                D_8018D168 = 1;
            }
            break;
        case 5:
            kwanm_wait(objectIndex, 0x0000001E);
            break;
        case 6:
            kwanm_forward(objectIndex, 1, 7, 1, 2, 0);
            break;
        case 7:
            if (kwanm_wait(objectIndex, 0x00000014) != 0) {
                g_DynamicObjects[objectIndex].stexaddr += 0x200;
                if (arg1 == 0) {
                    playSound(SOUND_ACTION_COUNTDOWN_LIGHT);
                }
            }
            break;
        case 8:
            kwanm_forward(objectIndex, 8, 0x0000000F, 1, 6, 0);
            break;
        case 9:
            if ((kwanm_wait(objectIndex, 8) != 0) && (arg1 == 0)) {
                playSound(SOUND_ACTION_COUNTDOWN_LIGHT);
            }
            break;
        case 10:
            if ((kwanm_forward(objectIndex, 0x00000010, 0x00000017, 1, 6, 0) != 0) && (arg1 == 0)) {
                D_801656F0 = 1;
            }
            break;
        case 11:
            if (kwanm_wait(objectIndex, 8) != 0) {
                g_DynamicObjects[objectIndex].stexaddr += 0x200;
                if (arg1 == 0) {
                    playSound(SOUND_ACTION_GREEN_LIGHT);
                }
            }
            break;
        case 12:
            kwanm_forward(objectIndex, 0x00000018, 0x0000001B, 1, 6, 0);
            break;
        case 13:
            if (arg1 == 0) {
                kwanm_racestart(objectIndex);
                D_8018D160 = 1;
                break;
            }
            kwanm_next(objectIndex);
            break;
        case 14:
            kwanm_wait(objectIndex, 0x00000078);
            break;
        case 15:
            kwanm_finish(objectIndex);
            break;
    }
}

void kwanm_jgm_g_init(s32 objectIndex, s32 playerIndex) {
    AnmObject* object;

    kwanm_jgm_break(objectIndex, playerIndex);
    kwanm_init_ci8(objectIndex, (u8*) common_tlut_lakitu_checkered_flag, gTextureLakituCheckeredFlag01, 0x48U,
                        (u16) 0x00000038);
    object = &g_DynamicObjects[objectIndex];
    object->indexaddr = gLakituTexturePtr;
    object->vtxaddr = common_vtx_also_lakitu;
    object->pos[2] = 5000.0f;
    object->pos[1] = 5000.0f;
    object->pos[0] = 5000.0f;
    object->scale = 0.15f;
    kwmv_start_spline(objectIndex, 2, &D_800E6834);
    kwanm_clrflag(objectIndex, 0x00000010);
    kwanm_next(objectIndex);
}

void kwanm_jgm_g(s32 objectIndex, s32 playerIndex) {
    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 0:
            break;
        case 1:
            kwanm_jgm_g_init(objectIndex, playerIndex);
            break;
        case 2:
            kwanm_setflag(objectIndex, 0x00000010);
            kwanm_next(objectIndex);
            break;
        case 3:
            kwanm_forward(objectIndex, 0, 0x0000001F, 1, 2, -1);
            break;
        case 4:
            kwanm_finish(objectIndex);
            break;
    }
}

void kwturiage_jugemu(s32 playerId) {
    s32 objectIndex;
    Player* player;

    objectIndex = jugemuallocptr[playerId];
    player = &gPlayerOne[playerId];
    if ((g_courseID == COURSE_SHERBET_LAND) && (player->jugemu_flag & IS_IN_WATER)) {
        KWAnmStart(objectIndex, 7);
        player->jugemu_flag |= IS_FROZEN;
    } else {
        KWAnmStart(objectIndex, 3);
    }
    kwanm_setevflag(objectIndex, 1);
}

void kwcheck_jugemu_turiage(s32 playerId) {
    s32 objectIndex;
    Player* player;

    objectIndex = jugemuallocptr[playerId];
    player = &gPlayerOne[playerId];
    if ((kwanm_bitoffcheckevflag(objectIndex, 1) != 0) &&
        (((OoBCheck(player->bump.last_zx) != 0) && (player->bump.distance_zx <= 3.0f)) ||
         (player->jugemu_flag & IS_IN_WATER) ||
         ((player->bump_status == OUT_OF_BOUNDS) && !(player->slip_flag & N_JUMP)))) {
        ExplorerSetJugemu(player);
        kwturiage_jugemu(playerId);
    }
}

void kwcheck_jugemu_warp(s32 objectIndex, Player* player) {
    if (player->jugemu_flag & IS_FADING_OUT) {
        kwanm_setevflag(objectIndex, 2);
        g_DynamicObjects[objectIndex].alpha = player->erase;
        return;
    }
    kwanm_clrevflag(objectIndex, 2);
}

void kwanm_jgm_t_init(s32 objectIndex, s32 arg1) {
    kwanm_jgm_break(objectIndex, arg1);
    kwanm_init_ci8(objectIndex, (u8*) common_tlut_lakitu_fishing, gTextureLakituFishing1, 0x38U, (u16) 0x00000048);
    g_DynamicObjects[objectIndex].vtxaddr = D_0D005F30;
    g_DynamicObjects[objectIndex].scale = 0.15f;
    kwmv_start(objectIndex);
    kwanm_clrflag(objectIndex, 0x00000010);
    kwchart_finish(objectIndex);
    kwanm_next(objectIndex);
    NaPlyLevelStart((u8) arg1, 0x0100FA28);
}

void kwmv_jgm_t(s32 objectIndex, UNUSED Player* player) {
    switch (g_DynamicObjects[objectIndex].mvptr) {
        case 0:
            break;
        case 1:
            g_DynamicObjects[objectIndex].pos_start[2] = 0.0f;
            g_DynamicObjects[objectIndex].pos_start[1] = 0.0f;
            g_DynamicObjects[objectIndex].pos_start[0] = 0.0f;
            g_DynamicObjects[objectIndex].pos_spline[2] = 0.0f;
            g_DynamicObjects[objectIndex].pos_spline[0] = 0.0f;
            g_DynamicObjects[objectIndex].pos_spline[1] = 80.0f;
            KWAnmNext(objectIndex);
            break;
        case 2:
            if (kwsub_fval(&g_DynamicObjects[objectIndex].pos_spline[1], 5.0f, 1.0f) != 0) {
                kwmv_finish(objectIndex);
            }
            break;
        case 3:
            if (kwadd_fval(&g_DynamicObjects[objectIndex].pos_spline[1], 100.0f, 1.0f) != 0) {
                kwmv_finish(objectIndex);
            }
            break;
    }
}

void kwanm_jgm_t(s32 objectIndex, s32 playerId) {
    Player* player = &gPlayerOne[playerId];

    switch (g_DynamicObjects[objectIndex].anmptr) { /* switch 1; irregular */
        case 0:                               /* switch 1 */
            break;
        case 1: /* switch 1 */
            kwanm_jgm_t_init(objectIndex, playerId);
            break;
        case 2: /* switch 1 */
            kwanm_setflag(objectIndex, 0x00000010);
            kwchart_start(objectIndex);
            kwanm_next(objectIndex);
            break;
        case 3: /* switch 1 */
            kwanm_oufuku_f(objectIndex, 0, 3, 1, 2, -1);
            break;
    }
    switch (g_DynamicObjects[objectIndex].stackptr) {
        case 0:
            break;
        case 1:
            if (kwmv_checkfinish(objectIndex) != 0) {
                kwchart_next(objectIndex);
            }
            break;
        case 2:
            ExplorerCallJugemu(player);
            kwchart_next(objectIndex);
            break;
        case 3:
            if (!(player->jugemu_flag & ON_LAKITU_ROD)) {
                kwmv_start_normal(objectIndex, 0, 3);
                kwchart_next(objectIndex);
            }
            break;
        case 4:
            if (kwmv_checkfinish(objectIndex) != 0) {
                kwchart_next(objectIndex);
            }
            break;
        case 5:
            kwanm_clrevflag(objectIndex, 1);
            NaPlyLevelStop((u8) playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x28));
            kwanm_finish(objectIndex);
            kwchart_finish(objectIndex);
            break;
    }
    if (g_DynamicObjects[objectIndex].anmptr >= 2) {
        kwcheck_jugemu_warp(objectIndex, player);
    }
    kwmv_jgm_t(objectIndex, player);
}

void KWChartIceBlock(s32 objectIndex, s32 playerId) {
    Player* player = &gPlayerOne[playerId];

    switch (g_DynamicObjects[objectIndex].anmptr) { /* switch 1; irregular */
        case 0:                               /* switch 1 */
            break;
        case 1: /* switch 1 */
            kwanm_jgm_t_init(objectIndex, playerId);
            break;
        case 2: /* switch 1 */
            kwanm_setflag(objectIndex, 0x00000010);
            kwchart_start(objectIndex);
            player->jugemu_flag |= HAS_ICE_CUBE;
            kwanm_next(objectIndex);
            break;
        case 3: /* switch 1 */
            kwanm_oufuku_f(objectIndex, 0, 3, 1, 2, -1);
            break;
    }
    switch (g_DynamicObjects[objectIndex].stackptr) {
        case 1:
            if (kwmv_checkfinish(objectIndex) != 0) {
                NAPlyTrgStart((u8) playerId, 0x1900A055U);
                kwchart_next(objectIndex);
            }
            break;
        case 2:
            ExplorerCallJugemu(player);
            kwanm_setevflag(objectIndex, 4);
            kwchart_next(objectIndex);
            break;
        case 3:
            if ((player->bump_status == ICE) && !(player->jugemu_flag & IS_IN_WATER) &&
                ((f64) player->bump.distance_zx <= 30.0)) {
                kwanm_setevflag(objectIndex, 8);
            }
            if (!(player->jugemu_flag & ON_LAKITU_ROD)) {
                kwmv_start_normal(objectIndex, 0, 3);
                kwchart_next(objectIndex);
            }
            break;
        case 4:
            kwchart_wait(objectIndex, 0x0000001E);
            break;
        case 5:
            player->jugemu_flag &= ~HAS_ICE_CUBE;
            kwanm_setevflag(objectIndex, 0x00000010);
            kwanm_setevflag(objectIndex, 0x00000020);
            kwanm_clrevflag(objectIndex, 4);
            kwanm_clrevflag(objectIndex, 8);
            kwchart_next(objectIndex);
            NAPlyTrgStart((u8) playerId, 0x1900A056U);
            break;
        case 6:
            if (kwchart_wait(objectIndex, 0x000000A0) != 0) {
                kwanm_clrevflag(objectIndex, 0x00000010);
                player->jugemu_flag &= ~IS_FROZEN;
                player->jugemu_flag |= IS_MELTING;
            }
            break;
        case 7:
            kwchart_wait(objectIndex, 0x0000003C);
            break;
        case 8:
            kwchart_finish(objectIndex);
            kwanm_finish(objectIndex);
            player->jugemu_flag &= ~IS_MELTING;
            kwanm_clrevflag(objectIndex, 1);
            NaPlyLevelStop((u8) playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x28));
            break;
    }

    if (g_DynamicObjects[objectIndex].anmptr >= 2) {
        kwcheck_jugemu_warp(objectIndex, player);
    }
    kwmv_jgm_t(objectIndex, player);
}

void kwanm_jgm_l_init(s32 objectIndex, s32 playerIndex) {
    AnmObject* object;

    kwanm_jgm_break(objectIndex, playerIndex);
    kwanm_init_ci8(objectIndex, (u8*) common_tlut_lakitu_second_lap, gTextureLakituSecondLap01, 0x48U,
                        (u16) 0x00000038);
    object = &g_DynamicObjects[objectIndex];
    object->indexaddr = gLakituTexturePtr;
    object->vtxaddr = common_vtx_also_lakitu;
    object->pos[2] = 5000.0f;
    object->pos[1] = 5000.0f;
    object->pos[0] = 5000.0f;
    object->scale = 0.15f;
    kwanm_clrflag(objectIndex, 0x00000010);
    kwmv_start_spline(objectIndex, 5, &D_800E694C);
    kwanm_next(objectIndex);
}

void kwanm_jgm_l(s32 objectIndex, s32 playerIndex) {
    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 0:
            break;
        case 1:
            kwanm_jgm_l_init(objectIndex, playerIndex);
            break;
        case 2:
            kwanm_setflag(objectIndex, 0x00000010);
            kwanm_next(objectIndex);
            break;
        case 3:
            kwanm_wait(objectIndex, 0x00000014);
            break;
        case 4:
            kwanm_forward(objectIndex, 0, 0x0000000F, 1, 2, 1);
            break;
        case 5:
            kwanm_wait(objectIndex, 0x0000003C);
            break;
        case 6:
            kwanm_reverse(objectIndex, 0x0000000F, 0, 1, 2, 1);
            break;
        case 7:
            if (g_DynamicObjects[objectIndex].mvptr == 0) {
                kwanm_finish(objectIndex);
            }
            break;
    }
}

void kwanm_jgm_f_init(s32 objectIndex, s32 playerIndex) {
    AnmObject* object;

    kwanm_jgm_break(objectIndex, playerIndex);
    kwanm_init_ci8(objectIndex, (u8*) common_tlut_lakitu_final_lap, gTextureLakituFinalLap01, 0x48U,
                        (u16) 0x00000038);
    object = &g_DynamicObjects[objectIndex];
    object->indexaddr = gLakituTexturePtr;
    object->vtxaddr = common_vtx_also_lakitu;
    object->pos[2] = 5000.0f;
    object->pos[1] = 5000.0f;
    object->pos[0] = 5000.0f;
    object->scale = 0.15f;
    kwanm_clrflag(objectIndex, 0x00000010);
    kwmv_start_spline(objectIndex, 5, &D_800E694C);
    kwanm_next(objectIndex);
}

void kwanm_jgm_f(s32 objectIndex, s32 playerIndex) {
    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 0:
            break;
        case 1:
            kwanm_jgm_f_init(objectIndex, playerIndex);
            break;
        case 2:
            kwanm_setflag(objectIndex, 0x00000010);
            kwanm_next(objectIndex);
            break;
        case 3:
            kwanm_wait(objectIndex, 0x00000014);
            break;
        case 4:
            kwanm_forward(objectIndex, 0, 0x0000000F, 1, 2, 1);
            break;
        case 5:
            kwanm_wait(objectIndex, 0x0000003C);
            break;
        case 6:
            kwanm_reverse(objectIndex, 0x0000000F, 0, 1, 2, 1);
            break;
        case 7:
            if (g_DynamicObjects[objectIndex].mvptr == 0) {
                kwanm_finish(objectIndex);
            }
            break;
    }
}

void kwanm_jgm_r_init(s32 objectIndex, s32 arg1) {
    f32 var = 5000.0f;
    kwanm_jgm_break(objectIndex, arg1);
    kwanm_init_ci8(objectIndex, (u8*) common_tlut_lakitu_reverse, gTextureLakituReverse01, 0x48U,
                        (u16) 0x00000038);
    g_DynamicObjects[objectIndex].indexaddr = gLakituTexturePtr;
    g_DynamicObjects[objectIndex].vtxaddr = common_vtx_also_lakitu;
    g_DynamicObjects[objectIndex].pos[2] = var;
    g_DynamicObjects[objectIndex].pos[1] = var;
    g_DynamicObjects[objectIndex].pos[0] = var;
    g_DynamicObjects[objectIndex].scale = 0.15f;
    kwanm_clrflag(objectIndex, 0x00000010);
    kwmv_start_spline(objectIndex, 6, &D_800E69B0);
    g_DynamicObjects[objectIndex].stackptr = 0;
    kwanm_next(objectIndex);
    NaPlyLevelStart((u8) arg1, 0x0100FA28);
}

void kwanm_jgm_r(s32 objectIndex, s32 playerId) {
    Player* sp2C = &gPlayerOne[playerId];

    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 0:
            break;
        case 1:
            kwanm_jgm_r_init(objectIndex, playerId);
            break;
        case 2:
            kwanm_setflag(objectIndex, 0x00000010);
            g_DynamicObjects[objectIndex].stackptr = 1;
            kwanm_next(objectIndex);
            break;
        case 3:
            kwanm_oufuku_f(objectIndex, 0, 0x0000000F, 1, 2, -1);
            break;
        case 4:
            kwanm_finish(objectIndex);
            break;
    }
    switch (g_DynamicObjects[objectIndex].stackptr) { /* switch 1; irregular */
        case 1:                                 /* switch 1 */
            if ((g_DynamicObjects[objectIndex].anmptr >= 3) && (!(sp2C->slip_flag & REVERSE))) {
                kwmv_start_spline(objectIndex, 6, &D_800E69F4);
                g_DynamicObjects[objectIndex].stackptr = 2;
                g_DynamicObjects[objectIndex].timer = 0x00000050;
                NaPlyLevelStop((u8) playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x28));
                return;
            }
            return;
        case 2: /* switch 1 */
            g_DynamicObjects[objectIndex].timer--;
            if (g_DynamicObjects[objectIndex].timer == 0) {
                kwanm_next(objectIndex);
                g_DynamicObjects[objectIndex].stackptr = 0;
            }
            break;
    }
}

void kwjugemu_setposition1(s32 objectIndex, Player* player, Camera* camera) {
    u16 temp_t8;

    temp_t8 = 0x8000 - camera->camera_direction[1];
    g_DynamicObjects[objectIndex].pos[0] =
        (player->position[0] +
         (cosT(temp_t8) * (g_DynamicObjects[objectIndex].pos_start[0] + g_DynamicObjects[objectIndex].pos_spline[0]))) -
        (sinT(temp_t8) * (g_DynamicObjects[objectIndex].pos_start[2] + g_DynamicObjects[objectIndex].pos_spline[2]));
    g_DynamicObjects[objectIndex].pos[1] =
        player->ground + g_DynamicObjects[objectIndex].pos_start[1] + g_DynamicObjects[objectIndex].pos_spline[1];
    g_DynamicObjects[objectIndex].pos[2] =
        (player->position[2] +
         (sinT(temp_t8) * (g_DynamicObjects[objectIndex].pos_start[0] + g_DynamicObjects[objectIndex].pos_spline[0]))) +
        (cosT(temp_t8) * (g_DynamicObjects[objectIndex].pos_start[2] + g_DynamicObjects[objectIndex].pos_spline[2]));
}

void kwjugemu_setposition2(s32 objectIndex, Player* player, Camera* camera) {
    u16 temp_t8;

    temp_t8 = 0x8000 - camera->camera_direction[1];
    g_DynamicObjects[objectIndex].pos[0] =
        (player->position[0] +
         (cosT(temp_t8) * (g_DynamicObjects[objectIndex].pos_start[0] + g_DynamicObjects[objectIndex].pos_spline[0]))) -
        (sinT(temp_t8) * (g_DynamicObjects[objectIndex].pos_start[2] + g_DynamicObjects[objectIndex].pos_spline[2]));
    g_DynamicObjects[objectIndex].pos[1] =
        player->position[1] + g_DynamicObjects[objectIndex].pos_start[1] + g_DynamicObjects[objectIndex].pos_spline[1];
    g_DynamicObjects[objectIndex].pos[2] =
        (player->position[2] +
         (sinT(temp_t8) * (g_DynamicObjects[objectIndex].pos_start[0] + g_DynamicObjects[objectIndex].pos_spline[0]))) +
        (cosT(temp_t8) * (g_DynamicObjects[objectIndex].pos_start[2] + g_DynamicObjects[objectIndex].pos_spline[2]));
}

UNUSED void kwjugemu_setposition3(void) {
}

void kwcheck_jugemu_reverse(s32 playerId) {
    s32 objectIndex;
    Player* player;

    objectIndex = jugemuallocptr[playerId];
    player = &gPlayerOne[playerId];

    if ((g_DynamicObjects[objectIndex].anmptr == 0) && (player->slip_flag & REVERSE)) {
        kwreverse_jugemu(playerId);
    }
}

void kwcheck_jugemu(s32 arg0) {
    if (D_801657B4 == 0) {
        kwcheck_jugemu_reverse(arg0);
    }
    kwcheck_jugemu_turiage(arg0);
}

void kwchart_jugemu_vf(s32 playerId) {
    s32 objectIndex = jugemuallocptr[playerId];

    switch (g_DynamicObjects[objectIndex].texsizex) {
        case 0:
            break;
        case 1:
            kwanm_jgm_s(objectIndex, playerId);
            kwjugemu_strategy3D(objectIndex);
            break;
        case 2:
            kwanm_jgm_g(objectIndex, playerId);
            kwjugemu_strategy3D(objectIndex);
            break;
        case 3:
            kwanm_jgm_t(objectIndex, playerId);
            break;
        case 4:
            kwanm_jgm_l(objectIndex, playerId);
            kwjugemu_strategy3D(objectIndex);
            break;
        case 5:
            kwanm_jgm_f(objectIndex, playerId);
            kwjugemu_strategy3D(objectIndex);
            break;
        case 6:
            kwanm_jgm_r(objectIndex, playerId);
            kwjugemu_strategy3D(objectIndex);
            break;
        case 7:
            KWChartIceBlock(objectIndex, playerId);
            break;
    }
}

void kwchart_jugemu(s32 playerId) {
    s32 objectIndex;

    kwcheck_jugemu(playerId);
    objectIndex = jugemuallocptr[playerId];
    gLakituTexturePtr = D_80183FA8[playerId];
    switch (g_DynamicObjects[objectIndex].texsizex) {
        case 1:
            kwanm_dmajgm(objectIndex, playerId, 2);
            kwjugemu_setposition1(objectIndex, D_8018CF1C, D_8018CF14);
            break;
        case 2:
            kwanm_dmajgm(objectIndex, playerId, 0);
            kwjugemu_setposition1(objectIndex, D_8018CF1C, D_8018CF14);
            break;
        case 3:
            kwanm_dmajgm(objectIndex, playerId, 0);
            kwjugemu_setposition2(objectIndex, D_8018CF1C, D_8018CF14);
            break;
        case 4:
            kwanm_dmajgm(objectIndex, playerId, 0);
            kwjugemu_setposition1(objectIndex, D_8018CF1C, D_8018CF14);
            break;
        case 5:
            kwanm_dmajgm(objectIndex, playerId, 0);
            kwjugemu_setposition1(objectIndex, D_8018CF1C, D_8018CF14);
            break;
        case 6:
            kwanm_dmajgm(objectIndex, playerId, 0);
            kwjugemu_setposition1(objectIndex, D_8018CF1C, D_8018CF14);
            break;
        case 7:
            kwanm_dmajgm(objectIndex, playerId, 0);
            kwjugemu_setposition2(objectIndex, D_8018CF1C, D_8018CF14);
            break;
        case 0:
        default:
            break;
    }
}

void RouletteStart(s32 playerId, bool arg1) {
    s32 itemWindow;

    if (playerHUD[playerId].goalsw == false) {
        itemWindow = ItemBoxAllocPtr[playerId];
        if (kwanm_bitoffcheckevflag(itemWindow, 4) != 0) {
            KWAnmStart(itemWindow, 0);
            if (arg1 != 0) {
                playerHUD[playerId].item = arg1;
            }
        }
        NAPlyTrgStart(playerId, 0x19008406U);
    }
}

void kwanm_useditem(s32 playerId) {
    UNUSED s32 pad;
    Player* player;
    s32 objectIndex;
    ItemWindowObjects* itemWindow;

    player = &gPlayerOne[playerId];
    objectIndex = ItemBoxAllocPtr[playerId];
    itemWindow = (ItemWindowObjects*) &g_DynamicObjects[objectIndex];
    if (itemWindow->output == ITEM_SUPER_MUSHROOM) {
        if (kwanm_bitoffcheckevflag(objectIndex, 2) != 0) {
            kwanm_setevflag(objectIndex, 2);
            itemWindow->spstep = 0x0258;
            kwanm_jump(objectIndex, 0x00000032);
        }
        if (itemWindow->spstep == 0) {
            kwanm_clrevflag(objectIndex, 2);
            player->item = ITEM_NONE;
            itemWindow->output = ITEM_NONE;
            kwanm_trig(objectIndex, 0);
            kwanm_jump(objectIndex, 9);
        }
    } else {
        player->item = ITEM_NONE;
        itemWindow->output = ITEM_NONE;
        kwanm_trig(objectIndex, 0);
    }
}

//! @todo Cleanup this function to use array access and struct if possible.
u8 kwgetitem_sub(s16 rank, s16 isCpu) {
    u16 rand = MakeRandomLimmit(100);
    u8* curve;
    u8 randomItem;

    // sRandomItemIndex not initialized for further randomness?
    sRandomItemIndex = ((u32) rand + (sRandomItemIndex + gControllerRandom) + kwvideoframetimer) % 100U;

    if (g_gameMode == VERSUS) {
        switch (g_playerCount) {
            case TWO_PLAYERS_SELECTED:
                curve = SegmentToVirtual((void*) common_versus_2_player_item_curve);
                break;
            case THREE_PLAYERS_SELECTED:
                curve = SegmentToVirtual((void*) common_versus_3_player_item_curve);
                break;
            case FOUR_PLAYERS_SELECTED:
                curve = SegmentToVirtual((void*) common_versus_4_player_item_curve);
                break;
        }
        randomItem = *((rank * 100) + curve + sRandomItemIndex);

    } else if (g_gameMode == BATTLE) {
        curve = SegmentToVirtual((void*) common_battle_item_curve);
        randomItem = curve[sRandomItemIndex];
    } else { // GP Mode
        if (isCpu == 0) {
            curve = SegmentToVirtual((void*) common_grand_prix_human_item_curve);
        } else {
            curve = SegmentToVirtual((void*) common_grand_prix_cpu_item_curve);
        }
        randomItem = *((rank * 100) + curve + sRandomItemIndex);
    }

    return randomItem;
}

u8 kwgetitem(UNUSED s16 arg0, s16 rank) {
    return kwgetitem_sub(rank, false);
}

u8 kwgetitem_enemy(UNUSED s32 arg0, s16 rank) {
    return kwgetitem_sub(rank, true);
}

s16 kwanm_randomitem(s32 objectIndex, s32 arg1) {
    UNUSED s32 pad[3];
    s16 randomItem;

    randomItem = kwgetitem(gLapCountByPlayerId[arg1], gGPCurrentRaceRankByPlayerId[arg1]);

    if (playerHUD[arg1].item != 0) {
        randomItem = (s16) playerHUD[arg1].item;
        playerHUD[arg1].item = 0;
    }

    kwanm_setinput(objectIndex, (s32) randomItem);

    return randomItem;
}

s32 kwsteal_item(s32 objectIndex, s32 playerId) {
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
    if (g_gameMode == GRAND_PRIX) {
        if (MakeRandomLimmit(0x0064U) < 0x51) {
            var_v1 = kwgetitem(gLapCountByPlayerId[playerId], gGPCurrentRaceRankByPlayerId[playerId]);
        } else {
            var_v1 = 0;
            NAPlyTrgStart(playerId, 0x1900A058U);
        }
        var_t3 = 1;
        g_DynamicObjects[objectIndex].anmnumbak = g_DynamicObjects[objectIndex].input = var_v1;
    } else {
        for (var_v1_2 = 0; var_v1_2 < g_playerCount; var_v1_2++) {
            temp_a0 = ItemBoxAllocPtr[var_v1_2];
            if (var_v1_2 != playerId) {
                if (g_DynamicObjects[temp_a0].output != 0) {
                    sp50[var_a3] = var_v1_2;
                    sp40[var_a3] = g_DynamicObjects[temp_a0].output;
                    var_a3 += 1;
                }
            }
        }
        if (var_a3 != 0) {
            var_v1 = MakeRandomLimmit(var_a3);
            temp_a1 = sp40[var_v1];
            g_DynamicObjects[objectIndex].input = temp_a1;
            g_DynamicObjects[objectIndex].anmnumbak = temp_a1;
            temp_v1 = sp50[var_v1];
            g_DynamicObjects[objectIndex].anmnum = temp_v1;
            temp_a0 = ItemBoxAllocPtr[temp_v1];
            sp38 = &gPlayerOne[temp_v1];
            kwanm_setevflag(temp_a0, 1);
            g_DynamicObjects[temp_a0].output = 0;
            sp38->item = 0;
            if (kwanm_bitcheckevflag(temp_a0, 2) != 0) {
                kwanm_clrevflag(temp_a0, 2);
                g_DynamicObjects[temp_a0].spstep = 0;
            }
            var_t3 = 1;
        }
    }
    return var_t3;
}

void kwanm_roulette_init(s32 objectIndex, s32 arg1) {
    s8 test; // why?

    kwanm_finish(objectIndex);
    g_DynamicObjects[objectIndex].anmptr = 2;
    g_DynamicObjects[objectIndex].output = 0;
    g_DynamicObjects[objectIndex].stexaddr = (u8*) common_tlut_item_window_none;
    g_DynamicObjects[objectIndex].sindexaddr = common_texture_item_window_none;
    g_DynamicObjects[objectIndex].texsizey = 0x28;
    g_DynamicObjects[objectIndex].fvalptr = 0x20;
    g_DynamicObjects[objectIndex].timer = 0x00000032;
    g_DynamicObjects[objectIndex].stackptr = 1;
    g_DynamicObjects[objectIndex].spstep = 0;
    kwanm_setevflag(objectIndex, 4);
    if (D_80165888 != 0) {
        kwanm_jump(objectIndex, 8);
        g_DynamicObjects[objectIndex].stackptr = 2;
        g_DynamicObjects[objectIndex].output = 0x000E;
        test = g_DynamicObjects[objectIndex].output;
        g_DynamicObjects[objectIndex].anmnumbak = test;
        g_DynamicObjects[objectIndex].input = test;
        playerHUD[PLAYER_ONE].addiy = 0;
        D_80165888 = 0;
    } else {
        NaPlyLevelStart(arg1, 0x0100FE1CU);
    }
}

void kwanm_roulette(s32 playerId) {
    s32 temp_s0;
    s32 sp40;
    AnmObject* object;
    Player* sp38;
    struct Controller* new_var;

    temp_s0 = ItemBoxAllocPtr[playerId];
    sp38 = &gPlayerOne[playerId];
    sp40 = 0;
    new_var = &gControllerOne[playerId];
    if (new_var->ButtonPressed & Z_TRIG) {
        sp40 = 1;
    }
    if (D_80165888 != 0) {
        KWAnmStart(temp_s0, 0);
    }
    object = &g_DynamicObjects[temp_s0];
    switch (object->anmptr) {
        case 1:
            kwanm_roulette_init(temp_s0, playerId);
            break;
        case 2:
            if (g_ScreenSplitA == 0) {
                kwadd_sval(&playerHUD[playerId].addiy, 0x0040, 4);
                if (playerHUD[playerId].addiy == 0x0040) {
                    kwanm_next(temp_s0);
                }
            } else if (g_ScreenSplitA == 3) {
                if ((playerId == 0) || (playerId == 2)) {
                    kwadd_sval(&playerHUD[playerId].addix, 0x0080, 8);
                    if (playerHUD[playerId].addix == 0x0080) {
                        kwanm_next(temp_s0);
                    }
                } else {
                    kwsub_sval(&playerHUD[playerId].addix, -0x0080, 8);
                    if (playerHUD[playerId].addix == -0x0080) {
                        kwanm_next(temp_s0);
                    }
                }
            } else {
                kwadd_sval(&playerHUD[playerId].addix, 0x0080, 8);
                if (playerHUD[playerId].addix == 0x0080) {
                    kwanm_next(temp_s0);
                }
            }
            break;
        case 3:
            kwanm_forward(temp_s0, 1, 0x0000000F, 1, 2, 2);
            break;
        case 4:
            kwanm_forward(temp_s0, 1, 6, 1, 8, 1);
            break;
        case 5:
            kwanm_forward(temp_s0, 1, 4, 1, 0x00000010, 1);
            break;
        case 6:
            object->anmnumbak = kwanm_randomitem(temp_s0, playerId);
            object->timer = 8;
            object->stackptr = 2;
            kwanm_next(temp_s0);
            NaPlyLevelStop((u8) playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFE, 0x1C));
            NaPlyLevelStart((u8) playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFE, 0x47));
            break;
        case 7:
            kwanm_swap(temp_s0, (s32) object->input, 0, 8, 0x0000000A);
            break;
        case 9:
            kwanm_clrevflag(temp_s0, 4);
            kwchart_off(temp_s0);
            object->anmnumbak = 0;
            kwanm_next(temp_s0);
            break;
        case 10:
            kwanm_wait(temp_s0, 0x00000014);
            break;
        case 11:
            if (g_ScreenSplitA == 0) {
                if (kwsub_sval(&playerHUD[playerId].addiy, 0, 4) != 0) {
                    kwanm_next(temp_s0);
                }
            } else if (g_ScreenSplitA == 3) {
                if ((playerId == 0) || (playerId == 2)) {
                    kwsub_sval(&playerHUD[playerId].addix, 0, 8);
                    if (playerHUD[playerId].addix == 0) {
                        kwanm_next(temp_s0);
                    }
                } else {
                    kwadd_sval(&playerHUD[playerId].addix, 0, 8);
                    if (playerHUD[playerId].addix == 0) {
                        kwanm_next(temp_s0);
                    }
                }
            } else if (kwsub_sval(&playerHUD[playerId].addix, 0, 8) != 0) {
                kwanm_next(temp_s0);
            }
            break;
        case 12:
            kwanm_finish(temp_s0);
            break;
        case 20:
            if (object->input == 0x000B) {
                kwanm_swap(temp_s0, object->input, 0, 8, 0x0000000A);
            } else {
                kwanm_swap(temp_s0, object->input, 0x0000000B, 8, 0x0000000A);
            }
            break;
        case 21:
            kwanm_jump(temp_s0, 8);
            object->stackptr = 2;
            break;
        case 30:
            if (object->input == 0x000B) {
                kwanm_swap(temp_s0, object->input, 0, 8, 0x0000000A);
            } else {
                kwanm_swap(temp_s0, object->input, 0x0000000B, 8, 0x0000000A);
            }
            break;
        case 31:
            kwanm_jump(temp_s0, 9);
            break;
        case 40:
            if (object->input == 0x000D) {
                kwanm_swap(temp_s0, object->input, 0x0000000E, 8, 0x0000000A);
            } else {
                kwanm_swap(temp_s0, object->input, 0x0000000D, 8, 0x0000000A);
            }
            break;
        case 41:
            kwanm_jump(temp_s0, 8);
            break;
        case 50:
            kwanm_swap(temp_s0, (s32) object->input, 0, 8, 0x00000064);
            break;
        case 0:
        default:
            break;
    }
    if (kwanm_bitcheckevflag(temp_s0, 2) != 0) {
        if (object->spstep == 0) {
            kwanm_useditem(playerId);
        } else {
            object->spstep--;
        }
    }
    if (object->timer >= 0) {
        if (object->timer > 0) {
            object->timer--;
        } else {
            switch (object->stackptr) { /* switch 1; irregular */
                case 1:                /* switch 1 */
                    if (sp40 != 0) {
                        kwchart_off(temp_s0);
                        kwanm_jump(temp_s0, 6);
                    }
                    break;
                case 2: /* switch 1 */
                    kwanm_setoutput(temp_s0, (s32) object->input);
                    object->stackptr = 3;
                    break;
                case 3: /* switch 1 */
                    if (object->output == 0) {
                        if (kwanm_bitoffcheckevflag(temp_s0, 1) != 0) {
                            if (object->input == 0x000B) {
                                if (kwsteal_item(temp_s0, playerId) != 0) {
                                    kwanm_jump(temp_s0, 0x00000014);
                                    object->stackptr = 0;
                                } else {
                                    kwanm_jump(temp_s0, 9);
                                }
                            } else if ((object->input == 0x000D) || (object->input == 0x000E)) {
                                object->input--;
                                kwanm_setoutput(temp_s0, (s32) object->input);
                                object->stackptr = 3;
                                kwanm_jump(temp_s0, 0x00000028);
                            } else {
                                kwanm_jump(temp_s0, 9);
                            }
                        } else {
                            kwanm_clrevflag(temp_s0, 1);
                            kwanm_jump(temp_s0, 0x0000001E);
                            object->stackptr = 0;
                        }
                    }
                    break;
            }
        }
    }
    object->texaddr = (u8*) itemboxtbl_pal[object->anmnumbak];
    object->indexaddr = itemboxtbl_idx[object->anmnumbak];
    sp38->item = object->output;
}

void kwchart_itemboxanm(s32 arg0) {
    kwanm_roulette(arg0);
}

void kwanm_pukupuku_jump(s32 objectIndex) {
    f32 var_f14;
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    switch (object->anmptr) { /* irregular */
        case 1:
            kwanm_init_poly(objectIndex, d_course_banshee_boardwalk_dl_cheep_cheep, 2.0f);
            kwanm_setflag(objectIndex, 0x00000010);
            object->chartptr = 0;
            break;
        case 2:
            if (g_ScreenFlip != 0) {
                kwmv_nanamenage(objectIndex, 18.0f, 0.7f, 25.0f, (s16) -0x00005800, 0x0000012C);
            } else {
                kwmv_nanamenage(objectIndex, 18.0f, 0.7f, 25.0f, (s16) 0x00005800, 0x0000012C);
            }
            if (object->velocity[2] < 0.0f) {
                var_f14 = -object->velocity[2];
            } else {
                var_f14 = object->velocity[2];
            }
            object->ang[0] = kwset_velyztoangx(object->velocity[1], var_f14);
            kwanm_wait(objectIndex, 0x00000046);
            break;
        case 3:
            kwanm_finish(objectIndex);
            break;
        case 0:
            break;
    }
}

void kwstart_pukupuku_jump(s32 playerId) {
    s32 objectIndex;

    objectIndex = objallocptr2[0];
    if (g_DynamicObjects[objectIndex].anmptr == 0) {
        if (((s32) gNearestPathPointByPlayerId[playerId] >= 0xA0) &&
            ((s32) gNearestPathPointByPlayerId[playerId] < 0xAB)) {
            kwset_init3D_spos(objectIndex, xOrientation * -1650.0, -200.0f, -1650.0f);
            KWAnmStart(objectIndex, 1);
        }
    }
}

void kwchart_pukupuku_jump(void) {
    UNUSED s32 pad;
    s32 objectIndex;

    kwstart_pukupuku_jump(0);
    objectIndex = objallocptr2[0];
    kwanm_pukupuku_jump(objectIndex);
    kwset_splinepos3D(objectIndex);
}

void kwanm_pukupuku_ending_init(s32 objectIndex) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    object->chartptr = 1;
    object->flag = 0;
    object->shape = d_course_banshee_boardwalk_dl_cheep_cheep;
    object->scale = 0.2f;
    kwanm_next(objectIndex);
    kwset_init3D_spos(objectIndex, D_800E634C[0][0], D_800E634C[0][1] + 55.0, D_800E634C[0][2]);
    kwset_init3D_sppos(objectIndex, 0.0f, 30.0f, 0.0f);
    kwset_init3D_ang(objectIndex, 0U, 0x3800U, 0U);
}

void kwanm_pukupuku_ending(s32 objectIndex) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    switch (object->anmptr) {
        case 1:
            kwanm_pukupuku_ending_init(objectIndex);
            break;
        case 2:
            if (kwanm_wait(objectIndex, 0x0000003C) != 0) {
                kwanm_setflag(objectIndex, 0x00000010);
                kwmv_start(objectIndex);
            }
            break;
        case 3:
            if (object->mvptr == 0) {
                kwanm_next(objectIndex);
            }
            break;
        case 4:
            if (kwanm_wait(objectIndex, 0x0000000A) != 0) {
                kwmv_jump(objectIndex, 2);
            }
            break;
        case 5:
            if (object->mvptr == 0) {
                kwanm_finish(objectIndex);
            }
            break;
        case 0:
        default:
            break;
    }
}

void kwmv_pukupuku_ending(s32 objectIndex) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    switch (object->mvptr) {
        case 0:
            break;
        case 1:
            object->velocity[1] = -0.2f;
            if ((f64) object->pos_spline[1] <= 0.0) {
                object->pos_spline[1] = 0.0f;
                object->velocity[1] = 0.0f;
                kwmv_finish(objectIndex);
            }
            break;
        case 2:
            if (kwmv_waitnext(objectIndex, 0x00000014) != 0) {
                object->stack[7] = 0x0040;
            }
            break;
        case 3:
            object->scale = (f32) ((f64) object->scale - 0.0015);
            if ((s32) object->ang[0] >= 0xA01) {
                object->stack[7] -= 4;
            }
            if (kwadd_usval(object->ang, 0x0C00U, (u16) object->stack[7]) != 0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 4:
            object->scale = (f32) ((f64) object->scale - 0.0015);
            object->vel = 0.001f;
            KWAnmNext(objectIndex);
            object->stack[7] = 0;
            break;
        case 5:
            if (object->vel <= 0.004) {
                object->vel += 0.0002;
            }
            object->scale += object->vel;
            kwadd_sval(&object->stack[7], 0x0100, 0x0010);
            object->ang[0] -= object->stack[7];
            if (kwmv_wait(objectIndex, 0x00000035) != 0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 6:
            if (kwmv_wait(objectIndex, 0x0000000F) != 0) {
                KWAnmNext(objectIndex);
                D_801658CE = 1;
            }
            break;
        case 7:
            object->scale = (f32) ((f64) object->scale - 0.05);
            if ((f64) object->scale <= 0.01) {
                kwanm_clrflag(objectIndex, 0x00000010);
                object->scale = 0.000001f;
                KWAnmNext(objectIndex);
            }
            break;
        case 8:
            kwmv_finish(objectIndex);
            break;
    }
    if (object->mvptr < 0xA) {
        kwsetfval_oufuku_f(objectIndex, &object->scale, 0.2f, 0.21f, 0.001f, 0, -1);
    }
    kwmv_addvelytopos(objectIndex);
    kwset_splinepos3D(objectIndex);
}

void kwchart_pukupuku_ending(void) {
    s32 objectIndex;

    objectIndex = objallocptr2[0];
    if (D_801658BC == 1) {
        D_801658BC = 0;
        KWAnmStart(objectIndex, 0);
    }
    if (g_DynamicObjects[objectIndex].anmptr != 0) {
        kwanm_pukupuku_ending(objectIndex);
        kwmv_pukupuku_ending(objectIndex);
    }
}

void kwchart_pukupuku(s32 arg0) {
    switch (arg0) {
        case 0:
            kwchart_pukupuku_jump();
            break;
        case 1:
            kwchart_pukupuku_ending();
            break;
    }
}

void kwchart_tellesa(void) {
    kwchart_tellesa_matowari();
}

// Updates the display status on an object based on its relative direction to the camera
void kwtel_convangtoanmnum_abs(s32 objectIndex, Camera* camera) {
    u16 rot = camera->camera_direction[1];
    u16 temp = ((u16) (g_DynamicObjects[objectIndex].ang[1] - rot + DEGREES(180)) * 0x24) / 0x10000;

    if (temp < 0x13) {
        kwanm_clrflag(objectIndex, 0x80);
        g_DynamicObjects[objectIndex].anmnumbak = temp;
    } else {
        kwanm_setflag(objectIndex, 0x80);
        g_DynamicObjects[objectIndex].anmnumbak = 0x24 - temp;
    }
}

void kwtel_lookplayer_abs(s32 objectIndex, Player* player, Camera* camera) {
    f32 x;
    f32 z;

    x = player->position[0] - g_DynamicObjects[objectIndex].pos[0];
    z = player->position[2] - g_DynamicObjects[objectIndex].pos[2];
    g_DynamicObjects[objectIndex].ang[1] =
        kwchase_angle(g_DynamicObjects[objectIndex].ang[1], Atan2t(x, z));
    kwtel_convangtoanmnum_abs(objectIndex, camera);
}

UNUSED void kwtel_chaseangtoanmnum_abs(void) {
}

void kwtel_convangtoanmnum_rel(s32 objectIndex) {
    u16 var_t9;

    var_t9 = g_DynamicObjects[objectIndex].ang[1] * 0x24 / 0x10000;

    if (var_t9 < 0x13) {
        kwanm_clrflag(objectIndex, 0x80);
        g_DynamicObjects[objectIndex].anmnumbak = var_t9;
    } else {
        kwanm_setflag(objectIndex, 0x80);
        g_DynamicObjects[objectIndex].anmnumbak = 0x24 - var_t9;
    }
}

void kwtel_chaseangtoanmnum_rel(s32 objectIndex) {
    g_DynamicObjects[objectIndex].ang[1] =
        kwchase_angle(g_DynamicObjects[objectIndex].ang[1],
                      Atan2t(g_DynamicObjects[objectIndex].velocity[0], g_DynamicObjects[objectIndex].velocity[2]));
    kwtel_convangtoanmnum_rel(objectIndex);
}

void kwanm_tellesa_matowari_init(s32 objectIndex) {
    AnmObject* object;

    kwanm_init_ci8(objectIndex, d_course_banshee_boardwalk_boo_tlut, D_80165880, 48, 40);
    object = &g_DynamicObjects[objectIndex];
    object->pos[0] = 0.0f;
    object->pos[1] = 0.0f;
    object->pos[2] = 0.0f;
    kwanm_setflag(objectIndex, 0x00000020);
    kwanm_next(objectIndex);
    object->alpha = 0;
    kwsetsval_start(objectIndex);
    object->scale = 0.15f;
    object->vel = 1.0f;
    kwsetfval_start(objectIndex);
    kwmv_start_strategy(objectIndex);
    kwanm_setflag(objectIndex, 0x00000800);
    object->texang[0] = 0;
    object->texang[2] = 0x8000;
}

void kwanm_tellesa_matowari(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 0:
            break;
        case 1:
            kwanm_tellesa_matowari_init(objectIndex);
            break;
    }
    if (g_DynamicObjects[objectIndex].anmptr >= 2) {
        switch (g_DynamicObjects[objectIndex].mvtype) {
            case 0:
                break;
            case 1:
                kwsetsval_forward(objectIndex, &g_DynamicObjects[objectIndex].alpha, 0, 0x00000050, 2, 1, 0);
                break;
            case 2:
                kwsetsval_oufuku_f(objectIndex, &g_DynamicObjects[objectIndex].alpha, 0x00000050, 0x00000078, 1, 0, -1);
                break;
            case 3:
                kwsetsval_subchase(objectIndex, &g_DynamicObjects[objectIndex].alpha, 0, 2);
                break;
            case 4:
                kwanm_finish(objectIndex);
                kwmv_finish(objectIndex);
                kwsetsval_finish(objectIndex);
                break;
        }
        kwanm_texaddr_ci8_index(objectIndex);
    }
}

void kwon_tellesa(s32 someIndex, s32 playerIndex) {
    s32 temp_a0;
    s32 objectIndex;
    s16 temp_s1_2;
    s16 temp_s4;
    s16 temp_s5;
    SplineData* something;

    for (temp_a0 = 0; temp_a0 < 5; temp_a0++) {
        objectIndex = objallocptr3[someIndex + temp_a0];
        KWAnmStart(objectIndex, 1);
        g_DynamicObjects[objectIndex].anmnum = playerIndex;
        temp_s1_2 = MakeRandomLimmit(0x003CU) - 0x1E;
        temp_s4 = MakeRandomLimmit(0x0014U) - 0xA;
        temp_s5 = MakeRandomLimmit(0x0050U) - 0x28;
        MakeRandomLimmit(0x1000U);
        something = D_800E5D9C[temp_a0];
        g_DynamicObjects[objectIndex].sptbtopaddr = something;
        g_DynamicObjects[objectIndex].pos_start[0] = (f32) temp_s1_2;
        g_DynamicObjects[objectIndex].pos_start[1] = (f32) temp_s4;
        g_DynamicObjects[objectIndex].pos_start[2] = (f32) temp_s5;
    }
    NAPlyTrgStart(playerIndex, 0x1900705AU);

    if (someIndex == 0) {
        D_8018CFF0 = 1;
    } else {
        D_8018D048 = 1;
    }
}

void kwoff_tellesa(s32 someIndex) {
    s32 temp_a0;
    s32 objectIndex;

    for (temp_a0 = 0; temp_a0 < 5; temp_a0++) {
        objectIndex = objallocptr3[someIndex + temp_a0];
        g_DynamicObjects[objectIndex].mvtype += 1;
    }

    if (someIndex == 0) {
        D_8018CFF0 = 0;
    } else {
        D_8018D048 = 0;
    }
}

s32 kwcheck_matowari_player(void) {
    s32 ret;
    s32 first, second;
    if (g_menuMultiplayerSelection == 1) {
        ret = 0;
    } else {
        if (g_menuMultiplayerSelection == 2) {
            if (g_gameMode == 0) {
                first = gGPCurrentRaceRankByPlayerId[0];
                second = gGPCurrentRaceRankByPlayerId[1];
                if (first < second) {
                    ret = 0;
                } else {
                    ret = 1;
                }
            } else {
                ret = rank_to_num[0];
            }
        } else {
            ret = rank_to_num[0];
        }
    }
    return ret;
}

void kwcheck_matowari(void) {
    s32 playerId;
    // wtf is up with this pointer? Its such a dumb way of going about things
    u16* test;

    if (D_8018CFF0 == 0) {
        playerId = kwcheck_matowari_player();
        D_8018D018 = playerId;
        test = &gNearestPathPointByPlayerId[playerId];
        if ((*test >= 0xC9) && (*test < 0xD2)) {
            kwon_tellesa(0, (s32) playerId);
        }
    }
    if (D_8018CFF0 != 0) {
        playerId = D_8018D018;
        test = &gNearestPathPointByPlayerId[playerId];
        if ((*test >= 0xB5) && (*test < 0xBE)) {
            kwoff_tellesa(0);
        }
        if ((*test >= 0x119) && (*test < 0x122)) {
            kwoff_tellesa(0);
        }
    }
    if (D_8018D048 == 0) {
        playerId = kwcheck_matowari_player();
        D_8018D110 = playerId;
        test = &gNearestPathPointByPlayerId[playerId];
        if ((*test >= 0x1FF) && (*test < 0x208)) {
            kwon_tellesa(5, (s32) playerId);
        }
    }
    if (D_8018D048 != 0) {
        playerId = D_8018D110;
        test = &gNearestPathPointByPlayerId[playerId];
        if ((*test >= 0x1EB) && (*test < 0x1F4)) {
            kwoff_tellesa(5);
        }
        if ((*test >= 0x26D) && (*test < 0x276)) {
            kwoff_tellesa(5);
        }
    }
}

void kwchart_tellesa_matowari(void) {
    u16 temp_t4;
    s32 someIndex;
    s32 objectIndex;
    Player* player;
    Camera* camera;
    AnmObject* object;

    kwcheck_matowari();
    for (someIndex = 0; someIndex < NUM_BOOS; someIndex++) {
        objectIndex = objallocptr3[someIndex];
        object = &g_DynamicObjects[objectIndex];
        if (object->anmptr != 0) {
            kwanm_tellesa_matowari(objectIndex);
            kwsp_strategy3D_loop(objectIndex);
            player = &gPlayerOne[object->anmnum];
            camera = &camera1[object->anmnum];
            temp_t4 = (0x8000 - camera->camera_direction[1]);
            object->pos[0] = player->position[0] + (cosT(temp_t4) * (object->pos_start[0] + object->pos_spline[0])) -
                             (sinT(temp_t4) * (object->pos_start[2] + object->pos_spline[2]));
            object->pos[1] = 6.5 + player->ground + object->pos_start[1] + object->pos_spline[1];
            object->pos[2] = player->position[2] + (sinT(temp_t4) * (object->pos_start[0] + object->pos_spline[0])) +
                             (cosT(temp_t4) * (object->pos_start[2] + object->pos_spline[2]));
            kwtel_chaseangtoanmnum_rel(objectIndex);
            if (kwanm_bitcheckflag(objectIndex, 0x00000080) != 0) {
                object->vtxaddr = dvtx_tellesa48x40xflip;
            } else {
                object->vtxaddr = dvtx_tellesa48x40;
            }
        }
    }
}

void kwanm_tellesa_init(s32 objectIndex) {
    AnmObject* object;

    kwanm_init_ci8(objectIndex, d_course_banshee_boardwalk_boo_tlut, gTextureGhosts, 0x30U, (u16) 0x00000028);
    object = &g_DynamicObjects[objectIndex];
    object->anmnumbak = 0x1C;
    object->pos[0] = 0.0f;
    object->pos[1] = 0.0f;
    object->pos[2] = 0.0f;
    object->scale = 0.15f;
    kwanm_next(objectIndex);
    object->alpha = 0;
    kwsetsval_start(objectIndex);
    kwmv_start_strategy(objectIndex);
    object->ang[2] = 0x8000;
    object->ang[1] =
        Atan2t(D_8018CF1C->position[0] - object->pos_start[0], D_8018CF1C->position[2] - object->pos_start[2]);
}

void kwanm_tellesa(s32 objectIndex) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    switch (object->anmptr) { /* switch 1 */
        case 0:              /* switch 1 */
            break;
        case 1: /* switch 1 */
            kwanm_tellesa_init(objectIndex);
            break;
        case 2: /* switch 1 */
            kwanm_wait(objectIndex, 0x0000001E);
            break;
        case 3: /* switch 1 */
            kwanm_wait(objectIndex, 0x000003E8);
            break;
        case 4: /* switch 1 */
            kwanm_finish(objectIndex);
            break;
        default: /* switch 2 */
            break;
    }
    if (object->anmptr >= 2) {
        kwanm_ifequjump(objectIndex, (s32) object->mvtype, 0, 4);
        kwanm_texaddr_ci8_index(objectIndex);
        switch (object->mvtype) { /* switch 2 */
            case 1:                /* switch 2 */
                kwsetsval_forward(objectIndex, &object->alpha, 0x00000028, 0x00000050, 4, 0, 0);
                break;
            case 2: /* switch 2 */
                kwsetsval_oufuku_f(objectIndex, &object->alpha, 0x00000050, 0x000000B4, 2, 0, -1);
                if ((object->mvptr == 0) || (object->anmptr == 3)) {
                    object->mvtype += 1;
                }
                break;
            case 3: /* switch 2 */
                kwsetsval_subchase(objectIndex, &object->alpha, 0, 4);
                break;
            case 4: /* switch 2 */
                kwsetsval_finish(objectIndex);
                break;
            case 0:
            default: /* switch 2 */
                break;
        }
    }
}

void kwchart_tellesa_randomborn(void) {
    UNUSED s32 stackPadding;
    UNUSED s32 stackPadding2;
    u16 temp_s3;
    s16 temp_t5;
    s16 temp_s2;
    u16 temp_t2;
    s32 var_v0;
    s32 objectIndex;

    var_v0 = 0;
    if ((KW2GFTimer == 0) && (D_8018D2A4 != 0) && (D_8018CF68[0] < 0x1D) && (D_800E5DB4[D_8018CF68[0]] == 1)) {
        while (g_DynamicObjects[EffectAllocArray1[var_v0 + 10]].anmptr != 0) {
            var_v0++;
            if (var_v0 == 30) {
                break;
            }
        }
        objectIndex = EffectAllocArray1[var_v0 + 10];
        if (var_v0 != 30) {
            if (g_DynamicObjects[objectIndex].anmptr == 0) {
                KWAnmStart(objectIndex, 1);
                temp_s2 = MakeRandomLimmit(0x012CU);
                temp_s3 = MakeRandomLimmit(0x1000U) - 0x800;
                temp_t5 = MakeRandomLimmit(0x000FU) - 5;
                g_DynamicObjects[objectIndex].ang[1] = D_8018CF1C->direction[1] + DEGREES(180);
                temp_t2 = (D_8018CF14->camera_direction[1] + temp_s3);
                g_DynamicObjects[objectIndex].pos_start[0] = D_8018CF1C->position[0] + (sinT(temp_t2) * temp_s2);
                g_DynamicObjects[objectIndex].pos_start[1] = temp_t5;
                g_DynamicObjects[objectIndex].pos_start[2] = D_8018CF1C->position[2] + (cosT(temp_t2) * temp_s2);
                g_DynamicObjects[objectIndex].sptbtopaddr = &D_800E5D54;
            }
        }
    }
    for (var_v0 = 0; var_v0 < 30; var_v0++) {
        objectIndex = EffectAllocArray1[var_v0 + 10];
        if (g_DynamicObjects[objectIndex].anmptr != 0) {
            kwanm_tellesa(objectIndex);
            kwsp_strategy3D(objectIndex);
            g_DynamicObjects[objectIndex].pos[0] =
                g_DynamicObjects[objectIndex].pos_start[0] + g_DynamicObjects[objectIndex].pos_spline[0];
            g_DynamicObjects[objectIndex].pos[1] =
                D_8018CF1C->ground + g_DynamicObjects[objectIndex].pos_start[1] + g_DynamicObjects[objectIndex].pos_spline[1];
            g_DynamicObjects[objectIndex].pos[2] =
                g_DynamicObjects[objectIndex].pos_start[2] + g_DynamicObjects[objectIndex].pos_spline[2];
            kwtel_lookplayer_abs(objectIndex, D_8018CF1C, D_8018CF14);
            if (kwanm_bitcheckflag(objectIndex, 0x00000080) != 0) {
                g_DynamicObjects[objectIndex].vtxaddr = dvtx_tellesa48x40xflip;
            } else {
                g_DynamicObjects[objectIndex].vtxaddr = dvtx_tellesa48x40;
            }
        }
    }
}

void kwanm_bat_randompos_init(s32 objectIndex, s32 arg1) {
    if (arg1 == 1) {
        g_DynamicObjects[objectIndex].pos_start[0] = (f32) ((-1775.0 - MakeRandomLimmit(0x001EU)) * (f64) xOrientation);
        g_DynamicObjects[objectIndex].pos_start[1] = (f32) (MakeRandomLimmit(0x0019U) + 25.0);
        g_DynamicObjects[objectIndex].pos_start[2] = (f32) (MakeRandomLimmit(0x001EU) + 130.0);
        g_DynamicObjects[objectIndex].pos_ende[0] = (f32) ((f64) xOrientation * -2500.0);
        g_DynamicObjects[objectIndex].pos_ende[1] = 0.0f;
        g_DynamicObjects[objectIndex].pos_ende[2] = (f32) (220.0 - MakeRandomLimmit(0x0096U));
        g_DynamicObjects[objectIndex].ang[0] = 0xDC00;
        g_DynamicObjects[objectIndex].destangy = 0x0800;
    }
    if (arg1 == 2) {
        g_DynamicObjects[objectIndex].pos_start[0] = (f32) (-0x55B - MakeRandomLimmit(0x001EU)) * xOrientation;
        g_DynamicObjects[objectIndex].pos_start[1] = (f32) (MakeRandomLimmit(0x0019U) + 0xF);
        g_DynamicObjects[objectIndex].pos_start[2] = (f32) (MakeRandomLimmit(0x001EU) - 0xE8);
        g_DynamicObjects[objectIndex].pos_ende[0] = (f32) ((f64) xOrientation * -2100.0);
        g_DynamicObjects[objectIndex].pos_ende[1] = 0.0f;
        g_DynamicObjects[objectIndex].pos_ende[2] = (f32) (MakeRandomLimmit(0x00C8U) + -290.0);
        g_DynamicObjects[objectIndex].ang[0] = 0;
        g_DynamicObjects[objectIndex].destangy = 0;
    }
    g_DynamicObjects[objectIndex].ang[1] =
        kwcalc_destangley(g_DynamicObjects[objectIndex].pos_start[0], g_DynamicObjects[objectIndex].pos_ende[0],
                             g_DynamicObjects[objectIndex].pos_start[2], g_DynamicObjects[objectIndex].pos_ende[2]);
    g_DynamicObjects[objectIndex].ang[2] = 0;
    kwset_init3D_sppos(objectIndex, 0.0f, 0.0f, 0.0f);
    g_DynamicObjects[objectIndex].mvtm = 0;
    if (g_raceClass < CC_150) {
        g_DynamicObjects[objectIndex].vel = (MakeRandomLimmit(4U) + 4.0);
    } else {
        g_DynamicObjects[objectIndex].vel = (MakeRandomLimmit(4U) + 5.0);
    }
}

void kwinit_bat(s32 objectIndex, s32 arg1) {
    UNUSED s32 pad[2];
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    object->chartptr = 0x0D;
    kwanm_bat_randompos_init(objectIndex, arg1);
    object->scale = 0.1f;
    kwanm_setflag(objectIndex, 0x00000200);
    object->r = 3;
}

void kwget_bat(s32 arg0) {
    s32 objectIndex;

    if (arg0 == 1) {
        objectIndex = kwalloc_effectram(EffectAllocArray2, &gNextFreeObjectParticle2, 0x28);
    } else {
        objectIndex = kwalloc_effectram(EffectAllocArray3, &gNextFreeObjectParticle3, 0x1E);
    }
    if (objectIndex != NULL_OBJECT_ID) {
        KWAnmStart(objectIndex, 0);
        kwinit_bat(objectIndex, arg0);
    }
}

void kwhitcheck_bat(s32 objectIndex) {
    if (g_raceClass == CC_50) {
        kwcheck_rebound_player_3D(objectIndex, 0.5f, 0.2f);
    } else if (g_raceClass == CC_100) {
        kwcheck_rebound_player_3D(objectIndex, 0.4f, 0.15f);
    } else {
        kwcheck_rebound_player_3D(objectIndex, 0.25f, 0.1f);
    }
}

s32 kwcheck_visible_bat(s32 objectIndex) {
    s32 loopIndex;
    s32 someCount;

    someCount = 0;
    for (loopIndex = 0; loopIndex < g_playerCount; loopIndex++) {
        if (kwanm_vischeck_camera_xz(objectIndex, &camera1[loopIndex], 0x4000U) != 0) {
            someCount += 1;
        }
    }
    return someCount;
}

void kwanm_bat_init(s32 arg0) {
    kwanm_next(arg0);
    kwmv_start(arg0);
}

void kwanm_bat(s32 objectIndex, s32 arg1) {
    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 1:
            kwanm_bat_init(objectIndex);
            break;
        case 2:
            if (kwcheck_visible_bat(objectIndex) == 0) {
                kwanm_finish(objectIndex);
            }
            if (arg1 == 1) {
                if (g_ScreenFlip != 0) {
                    if (g_DynamicObjects[objectIndex].pos[0] >= 2540.0) {
                        kwanm_finish(objectIndex);
                    }
                } else if (g_DynamicObjects[objectIndex].pos[0] <= -2540.0) {
                    kwanm_finish(objectIndex);
                }
            } else if (g_ScreenFlip != 0) {
                if (g_DynamicObjects[objectIndex].pos[0] >= 2150.0) {
                    kwanm_finish(objectIndex);
                }
            } else {
                if (g_DynamicObjects[objectIndex].pos[0] <= -2150.0) {
                    kwanm_finish(objectIndex);
                }
            }
            break;
        case 0:
            break;
    }
}

void kwmv_bat_fireplace(s32 objectIndex) {
    kwmv_velangytoposxz(objectIndex);
    kwset_splinepos3D(objectIndex);
}

void kwmv_bat_kanoke(s32 objectIndex) {
    UNUSED s32 pad;
    if ((g_DynamicObjects[objectIndex].mvptr != 0) && (g_DynamicObjects[objectIndex].mvptr == 1)) {
        if (kwmv_wait(objectIndex, 0x0000001E) != 0) {
            g_DynamicObjects[objectIndex].destangy = 0U;
        }
    }
    g_DynamicObjects[objectIndex].ang[0] =
        kwchase_angle(g_DynamicObjects[objectIndex].ang[0], g_DynamicObjects[objectIndex].destangy);
    kwmv_velangxytoposxyz(objectIndex);
    kwset_splinepos3D(objectIndex);
}

void kwmv_bat(s32 objectIndex, s32 arg1) {
    switch (arg1) { /* irregular */
        case 1:
            kwmv_bat_kanoke(objectIndex);
            return;
        case 2:
            kwmv_bat_fireplace(objectIndex);
            return;
    }
}

void kwchart_bat(void) {
    s32 var_s2;
    s32 temp_s0;
    AnmObject* object;

    if (D_8018CFC8 != 0) {
        D_8018CFC8 -= 1;
    }
    if (D_8018D000 != 0) {
        D_8018D000 -= 1;
    }
    temp_s0 = objallocptr[0];
    kwanm_forward(temp_s0, 0, 3, 1, 0, -1);
    kwanm_texaddr_ci8_index(temp_s0);
    object = &g_DynamicObjects[temp_s0];
    kwsetsval_oufuku_f(temp_s0, &object->alpha, -0x00001000, 0x00001000, 0x00000400, 0, -1);
    object->texang[2] = object->alpha + 0x8000;
    if ((D_8018CFB0 != 0) || (D_8018CFC8 != 0)) {
        D_8018CFD8 = 0;
        for (var_s2 = 0; var_s2 < 40; var_s2++) {
            temp_s0 = EffectAllocArray2[var_s2];
            if (temp_s0 == -1) {
                continue;
            }

            object = &g_DynamicObjects[temp_s0];
            if (object->anmptr == 0) {
                continue;
            }

            kwanm_bat(temp_s0, 1);
            kwmv_bat(temp_s0, 1);
            kwhitcheck_bat(temp_s0);
            if (object->anmptr == 0) {
                kwdealloc_effectram(&EffectAllocArray2[var_s2]);
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

            object = &g_DynamicObjects[temp_s0];
            if (object->anmptr == 0) {
                continue;
            }

            kwanm_bat(temp_s0, 2);
            kwmv_bat(temp_s0, 2);
            kwhitcheck_bat(temp_s0);
            if (object->anmptr == 0) {
                kwdealloc_effectram(&EffectAllocArray3[var_s2]);
            }
            D_8018D010 += 1;
        }
        if (D_8018D010 != 0) {
            D_8018D000 = 0x012C;
        }
    }
}

void kwcheck_appear_kanoke_bat(s32 objectIndex) {
    f32 sp2C;
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    if (object->timer > 0) {
        object->timer--;
        if (object->timer == 0) {
            kwanm_clrevflag(objectIndex, 1);
        }
    }
    if (object->ival > 0) {
        object->ival--;
        if (object->ival == 0) {
            NaSceneLevelStop(object->pos, SOUND_ARG_LOAD(0x51, 0x02, 0x80, 0x06));
        }
    }
    if (object->timer == 0) {
        if ((g_raceClass == CC_50) || (g_raceClass == CC_100) || (g_raceClass == CC_150) ||
            (g_raceClass == CC_EXTRA)) {
            sp2C = 1150.0f;
        }
        kwcheck_visible_area_camera(objectIndex, sp2C);
        if ((kwanm_bitcheckflag(objectIndex, VISIBLE) != 0) && (kwanm_bitoffcheckevflag(objectIndex, 1) != 0)) {
            kwanm_setevflag(objectIndex, 1);
            NaSceneLevelStart(object->pos, object->velocity, SOUND_ARG_LOAD(0x51, 0x02, 0x80, 0x06));
            kwanm_jump(objectIndex, 3);
            if (object->output > 0) {
                object->output--;
                object->timer = 0x00000168;
            } else {
                object->timer = 0x00000168;
            }
            object->ival = 0x0000012C;
        }
    }
    if (kwcheck_mapareain(0x000F, 0x0012) == 0) {
        object->output = 2;
    }
}

void kwanm_kanoke_init(s32 objectIndex) {
    g_DynamicObjects[objectIndex].scale = 1.0f;
    g_DynamicObjects[objectIndex].shape = d_course_banshee_boardwalk_dl_trash_bin;
    g_DynamicObjects[objectIndex].timer = 0;
    g_DynamicObjects[objectIndex].stack[7] = 0;
    kwset_init3D_texang(objectIndex, 0U, 0U, 0U);
    if (g_ScreenFlip != 0) {
        g_DynamicObjects[objectIndex].pos[0] = 1765.0f;
        g_DynamicObjects[objectIndex].pos[2] = 195.0f;
        g_DynamicObjects[objectIndex].texang[1] = 0x8000;
    } else {
        g_DynamicObjects[objectIndex].pos[0] = -1765.0f;
        g_DynamicObjects[objectIndex].pos[2] = 70.0f;
    }
    g_DynamicObjects[objectIndex].pos[1] = 45.0f;
    kwset_init3D_vel(objectIndex, 0.0f, 0.0f, 0.0f);
    g_DynamicObjects[objectIndex].output = 0;
    kwanm_next(objectIndex);
}

void kwanm_kanoke(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 1:
            kwanm_kanoke_init(objectIndex);
            break;
        case 3:
            D_8018CFB0 = 1;
            kwanm_next(objectIndex);
            break;
        case 4:
            kwanm_wait(objectIndex, 0x000000D2);
            if (KW8GFTimer == 0) {
                if (g_raceClass < CC_150) {
                    kwget_bat(1);
                    kwget_bat(1);
                } else {
                    kwget_bat(1);
                    kwget_bat(1);
                    kwget_bat(1);
                    kwget_bat(1);
                }
            }
            kwsetsval_oufuku_f(objectIndex, &g_DynamicObjects[objectIndex].alpha, -0x00002000, 0, 0x00000400, 0, -1);
            g_DynamicObjects[objectIndex].texang[2] = g_DynamicObjects[objectIndex].alpha;
            if (g_DynamicObjects[objectIndex].stack[7] == 0) {
                NAEnmTrgStart(g_DynamicObjects[objectIndex].pos, g_DynamicObjects[objectIndex].velocity,
                              SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x4E));
                g_DynamicObjects[objectIndex].stack[7] = 0x0014;
            } else {
                g_DynamicObjects[objectIndex].stack[7]--;
            }
            break;
        case 5:
            g_DynamicObjects[objectIndex].texang[2] = kwchase_angle(g_DynamicObjects[objectIndex].texang[2], 0U);
            if (g_DynamicObjects[objectIndex].texang[2] == 0) {
                kwanm_next(objectIndex);
            }
            break;
        case 6:
            g_DynamicObjects[objectIndex].texang[2] = 0;
            g_DynamicObjects[objectIndex].stack[7] = 0;
            kwanm_next(objectIndex);
            D_8018CFB0 = 0;
            break;
        case 0:
        case 2:
        default:
            break;
    }
}

void kwchart_kanoke(void) {
    s32 objectIndex = objallocptr[1];
    kwanm_kanoke(objectIndex);
    if (g_gameMode != TIME_TRIALS) {
        kwcheck_appear_kanoke_bat(objectIndex);
    }
}

void kwcheck_appear_danro_bat(s32 objectIndex) {
    f32 sp2C;
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    if (object->timer > 0) {
        object->timer--;
        if (object->timer == 0) {
            kwanm_clrevflag(objectIndex, 1);
        }
    }
    if (object->ival > 0) {
        object->ival--;
        if (object->ival == 0) {
            NaSceneLevelStop(object->pos, SOUND_ARG_LOAD(0x51, 0x02, 0x80, 0x06));
        }
    }
    if (object->timer == 0) {
        if ((g_raceClass == CC_50) || (g_raceClass == CC_100) || (g_raceClass == CC_150) ||
            (g_raceClass == CC_EXTRA)) {
            sp2C = 700.0f;
        }
        kwcheck_visible_area_camera(objectIndex, sp2C);
        if ((kwanm_bitcheckflag(objectIndex, VISIBLE) != 0) && (kwanm_bitoffcheckevflag(objectIndex, 1) != 0)) {
            kwanm_setevflag(objectIndex, 1);
            NaSceneLevelStart(object->pos, object->velocity, SOUND_ARG_LOAD(0x51, 0x02, 0x80, 0x06));
            kwanm_jump(objectIndex, 3);
            if (object->output > 0) {
                object->output--;
                object->timer = 0x00000168;
            } else {
                object->timer = 0x00000168;
            }
            object->ival = 0x0000012C;
        }
    }
    if (kwcheck_mapareain(0x000F, 0x0013) == 0) {
        object->output = 2;
    }
}

void kwanm_danro_init(s32 objectIndex) {
    g_DynamicObjects[objectIndex].pos[0] = -1371.0f * xOrientation;
    g_DynamicObjects[objectIndex].pos[1] = 31.0f;
    g_DynamicObjects[objectIndex].pos[2] = -217.0f;
    kwset_init3D_vel(objectIndex, 0.0f, 0.0f, 0.0f);
    g_DynamicObjects[objectIndex].output = 0;
    g_DynamicObjects[objectIndex].timer = 0;
    g_DynamicObjects[objectIndex].stack[7] = 0;
    kwanm_next(objectIndex);
}

void kwanm_danro(s32 objectIndex) {

    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 1:
            kwanm_danro_init(objectIndex);
            break;
        case 3:
            D_8018CFE8 = 1;
            kwanm_next(objectIndex);
            break;
        case 4:
            kwanm_wait(objectIndex, 0x000000D2);
            if (KW4GFTimer == 0) {
                if (g_raceClass < CC_150) {
                    kwget_bat(2);
                } else {
                    kwget_bat(2);
                    kwget_bat(2);
                }
            }
            break;
        case 5:
            D_8018CFE8 = 0;
            kwanm_next(objectIndex);
            break;
        case 0:
        case 2:
        default:
            break;
    }
}

void kwchart_danro(void) {
    s32 objectIndex = objallocptr[2];
    kwanm_danro(objectIndex);
    if (g_gameMode != TIME_TRIALS) {
        kwcheck_appear_danro_bat(objectIndex);
    }
}

s32 kwcheck_doshin_turn_sub(s32 objectIndex, Player* player, Camera* camera) {
    s32 sp24;

    sp24 = 0;
    if ((kwanm_bitoffcheckevflag(objectIndex, 4) != 0) &&
        (kwanm_checkradius_player_xz(objectIndex, player, 300.0f) != 0) &&
        (kwanm_vischeck_player_xz(objectIndex, player, camera, 0x4000U) != 0) &&
        (kwanm_checkobjang_camera_xz(objectIndex, camera, 0x1555U) != 0)) {
        kwanm_setevflag(objectIndex, 4);
        sp24 = 1;
    }
    return sp24;
}

s32 kwcheck_doshin_turn(s32 objectIndex) {
    Camera* camera;
    Player* player;
    s32 temp_v0;
    s32 someIndex;

    temp_v0 = 0;
    player = gPlayerOne;
    camera = camera1;
    for (someIndex = 0; someIndex < g_playerCount; someIndex++) {
        temp_v0 = kwcheck_doshin_turn_sub(objectIndex, player++, camera++);
        if (temp_v0 != 0) {
            break;
        }
    }
    return temp_v0;
}

void kwanm_doshin_rakka(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].anmptr) { /* irregular */
        case 0x32:
            if (kwadd_fval(&g_DynamicObjects[objectIndex].pos_spline[1], g_DynamicObjects[objectIndex].pos_ende[1] + 15.0,
                                    1.5f) != 0) {
                kwanm_setflag(objectIndex, 0x00000200);
                kwanm_setevflag(objectIndex, 1);
                kwanm_clrevflag(objectIndex, 2);
                kwanm_next(objectIndex);
            }
            break;
        case 0x33:
            if (kwsub_fval(&g_DynamicObjects[objectIndex].pos_spline[1], 0.0f, 2.0f) != 0) {
                if (g_DynamicObjects[objectIndex].pos_spline[1] >= 16.0f) {
                    g_DynamicObjects[objectIndex].anmnumbak = 0;
                } else if (g_DynamicObjects[objectIndex].pos_spline[1] >= 8.0f) {
                    g_DynamicObjects[objectIndex].anmnumbak = 1;
                } else {
                    g_DynamicObjects[objectIndex].anmnumbak = 2;
                }
                kwanm_clrevflag(objectIndex, 1);
                if (kwanm_bitcheckflag(objectIndex, 0x00010000) != 0) {
                    kwanm_setevflag(objectIndex, 0x00000010);
                    if (kwanm_bitcheckflag(objectIndex, VISIBLE) != 0) {
                        kwanm_setevflag(objectIndex, 0x00000020);
                    }
                }
                if (kwanm_bitcheckevflag(objectIndex, 2) != 0) {
                    kwanm_jump(objectIndex, 0x00000064);
                } else {
                    kwanm_next(objectIndex);
                }
            }
            break;
        case 0x34:
            kwanm_setnumwait(objectIndex, 3, 6);
            break;
        case 0x35:
            kwanm_setnumwait(objectIndex, 2, 0x00000032);
            break;
        case 0x36:
            if (g_DynamicObjects[objectIndex].pos_spline[1] >= 20.0f) {
                g_DynamicObjects[objectIndex].anmnumbak = 0;
            } else if (g_DynamicObjects[objectIndex].pos_spline[1] >= 18.0f) {
                g_DynamicObjects[objectIndex].anmnumbak = 1;
            }
            if (kwadd_fval(&g_DynamicObjects[objectIndex].pos_spline[1], g_DynamicObjects[objectIndex].pos_ende[1], 0.5f) !=
                0) {
                kwanm_clrflag(objectIndex, 0x00000200);
                kwanm_return(objectIndex);
            }
            break;
        case 0x64:
            kwanm_forward(objectIndex, 3, 5, 1, 8, 0);
            break;
        case 0x65:
            kwanm_wait(objectIndex, 0x0000001E);
            break;
        case 0x66:
            if (kwadd_fval(&g_DynamicObjects[objectIndex].pos_spline[1], 20.0f, 1.5f) != 0) {
                kwanm_next(objectIndex);
            }
            break;
        case 0x67:
            if (kwsub_fval(&g_DynamicObjects[objectIndex].pos_spline[1], 0.0f, 1.5f) != 0) {
                if (kwanm_bitcheckflag(objectIndex, 0x00020000) != 0) {
                    kwanm_setevflag(objectIndex, 0x00000010);
                    if (kwanm_bitcheckflag(objectIndex, VISIBLE) != 0) {
                        kwanm_setevflag(objectIndex, 0x00000020);
                    }
                }
                kwanm_next(objectIndex);
            }
            break;
        case 0x68:
            if (kwadd_fval(&g_DynamicObjects[objectIndex].pos_spline[1], 12.0f, 1.5f) != 0) {
                kwanm_next(objectIndex);
            }
            break;
        case 0x69:
            if (kwsub_fval(&g_DynamicObjects[objectIndex].pos_spline[1], 0.0f, 1.5f) != 0) {
                if (kwanm_bitcheckflag(objectIndex, 0x00020000) != 0) {
                    kwanm_setevflag(objectIndex, 0x00000010);
                    if (kwanm_bitcheckflag(objectIndex, VISIBLE) != 0) {
                        kwanm_setevflag(objectIndex, 0x00000020);
                    }
                }
                NAEnmTrgStart(g_DynamicObjects[objectIndex].pos, g_DynamicObjects[objectIndex].velocity,
                              SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x45));
                kwanm_next(objectIndex);
            }
            break;
        case 0x6A:
            if (kwanm_oufuku_r(objectIndex, 5, 3, 1, 6, 3) != 0) {
                kwhitclear_doshin(objectIndex);
            }
            break;
        case 0x6B:
            if (g_DynamicObjects[objectIndex].pos_spline[1] >= 22.0f) {
                g_DynamicObjects[objectIndex].anmnumbak = 0;
            } else if (g_DynamicObjects[objectIndex].pos_spline[1] >= 20.0f) {
                g_DynamicObjects[objectIndex].anmnumbak = 1;
            } else if (g_DynamicObjects[objectIndex].pos_spline[1] >= 18.0f) {
                g_DynamicObjects[objectIndex].anmnumbak = 2;
            } else if (g_DynamicObjects[objectIndex].pos_spline[1] >= 16.0f) {
                g_DynamicObjects[objectIndex].anmnumbak = 3;
            } else if (g_DynamicObjects[objectIndex].pos_spline[1] >= 14.0f) {
                g_DynamicObjects[objectIndex].anmnumbak = 4;
            } else {
                kwanm_oufuku_f(objectIndex, 3, 5, 1, 6, -1);
            }
            if (kwadd_fval(&g_DynamicObjects[objectIndex].pos_spline[1], g_DynamicObjects[objectIndex].pos_ende[1], 0.5f) !=
                0) {
                kwanm_trig(objectIndex, 0);
                kwanm_next(objectIndex);
            }
            break;
        case 0x6C:
            if (kwanm_wait(objectIndex, 0x00000064) != 0) {
                kwanm_clrevflag(objectIndex, 2);
                kwanm_clrflag(objectIndex, 0x00000200);
                kwanm_return(objectIndex);
            }
            break;
        case 0xC8:
            if (kwanm_wait(objectIndex, 0x0000012C) != 0) {
                kwanm_bitcheckevflag(objectIndex, 0x00000080);
                kwanm_finish(objectIndex);
                kwanm_jump(objectIndex, 1);
            }
            break;
        case 0x12C:
            if (kwanm_turn_left(objectIndex, &g_DynamicObjects[objectIndex].texang[1], 0x0400U, 0x00008000) != 0) {
                kwanm_clrevflag(objectIndex, 4);
                kwanm_return(objectIndex);
            }
            break;
    }
}

void kwanm_doshin_init(s32 objectIndex) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    object->ground = 0.0f;
    object->pos_start[1] = 0.0f;
    kwset_init3D_sppos(objectIndex, 0.0f, 0.0f, 0.0f);
    if (g_ScreenFlip != 0) {
        kwset_init3D_ang(objectIndex, 0U, 0x4000U, 0U);
        kwset_init3D_texang(objectIndex, 0U, 0x4000U, 0U);
    } else {
        kwset_init3D_ang(objectIndex, 0U, 0xC000U, 0U);
        kwset_init3D_texang(objectIndex, 0U, 0xC000U, 0U);
    }
    kwanm_init_ci8(objectIndex, d_course_bowsers_castle_thwomp_tlut, (u8*) d_course_bowsers_castle_thwomp_faces,
                        0x10U, (u16) 0x00000040);
    object->shape = d_course_bowsers_castle_dl_thwomp;
    object->r = 0x000C;
    object->scale = 1.0f;
    object->pos_ende[1] = 30.0f;
    kwanm_setflag(objectIndex, 0x05000220);
    object->output = 0;
    object->unk_0DF = 6;
    kwanm_clearstack(objectIndex);
    kwanm_next(objectIndex);
}

void kwanm_doshin(s32 objectIndex) {
    UNUSED s32 stackPadding[4];
    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 0:
            break;
        case 1:
            kwanm_doshin_init(objectIndex);
            break;
        case 2:
            kwanm_wait(objectIndex, 0x0000003C);
            break;
        case 3:
            kwanm_gosubnext(objectIndex, 0x00000032);
            break;
        case 4:
            if (kwcheck_doshin_turn(objectIndex) != 0) {
                kwanm_gosub(objectIndex, 0x0000012C, 2);
            } else {
                kwanm_jump(objectIndex, 2);
            }
            break;
    }
    kwanm_doshin_rakka(objectIndex);
    kwset_splinepos3D(objectIndex);
    g_DynamicObjects[objectIndex].ang[1] = g_DynamicObjects[objectIndex].texang[1];
    kwanm_texaddr_ci8_index(objectIndex);
}

void kwanm_doshin_kurukuru_init(s32 objectIndex) {
    AnmObject* object;

    kwanm_init_ci8(objectIndex, d_course_bowsers_castle_thwomp_tlut, (u8*) d_course_bowsers_castle_thwomp_faces,
                        0x10U, (u16) 0x00000040);
    object = &g_DynamicObjects[objectIndex];
    object->scale = 1.0f;
    object->shape = d_course_bowsers_castle_dl_thwomp;
    object->r = 0x000C;
    kwanm_setflag(objectIndex, 0x04000220);
    object->output = 0;
    object->unk_0DF = 6;
    kwmv_start(objectIndex);
    object->ground = 0.0f;
    object->pos_start[1] = 0.0f;
    kwset_init3D_sppos(objectIndex, 0.0f, 20.0f, 0.0f);
    object->pos_ende[1] = 20.0f;
    if (g_ScreenFlip != 0) {
        kwset_init3D_ang(objectIndex, 0U, 0x4000U, 0U);
        kwset_init3D_texang(objectIndex, 0U, 0x4000U, 0U);
    } else {
        kwset_init3D_ang(objectIndex, 0U, 0xC000U, 0U);
        kwset_init3D_texang(objectIndex, 0U, 0xC000U, 0U);
    }
    object->mvptr = 1;
    if (object->alpha == 0) {
        object->spptr = 1;
    } else {
        object->spptr = 2;
    }
    kwanm_next(objectIndex);
}

void kwmv_doshin_kurukuru_f(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].mvptr) {
        case 1:
            if (kwanm_turn_left(objectIndex, (u16*) &g_DynamicObjects[objectIndex].texang[1], 0x0800U, 0x00008000) != 0) {
                g_DynamicObjects[objectIndex].pos_ende[0] = (f32) ((f64) xOrientation * 200.0);
                KWAnmNext(objectIndex);
            }
            break;
        case 2:
            if (KWChaseFVal(g_DynamicObjects[objectIndex].pos_spline, g_DynamicObjects[objectIndex].pos_ende[0], 4.0f) != 0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 3:
            if (kwanm_turn_left(objectIndex, (u16*) &g_DynamicObjects[objectIndex].texang[1], 0x0400U, 0x00008000) != 0) {
                kwanm_jump(objectIndex, 3);
                KWAnmNext(objectIndex);
            }
            break;
        case 5:
            if (kwanm_turn_left(objectIndex, (u16*) &g_DynamicObjects[objectIndex].texang[1], 0x0400U, 0x0000C000) != 0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 6:
            if (kwsub_fval(&g_DynamicObjects[objectIndex].pos_spline[2], -100.0f, 2.0f) != 0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 7:
            if (kwanm_turn_left(objectIndex, (u16*) &g_DynamicObjects[objectIndex].texang[1], 0x0400U, 0x00004000) != 0) {
                KWAnmNext(objectIndex);
                kwanm_jump(objectIndex, 3);
            }
            break;
        case 9:
            if (kwanm_turn_left(objectIndex, (u16*) &g_DynamicObjects[objectIndex].texang[1], 0x0400U, 0x00010000) != 0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 10:
            if (KWChaseFVal(g_DynamicObjects[objectIndex].pos_spline, 0.0f, 4.0f) != 0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 11:
            if (kwanm_turn_left(objectIndex, (u16*) &g_DynamicObjects[objectIndex].texang[1], 0x0400U, 0x00010000) != 0) {
                KWAnmNext(objectIndex);
                kwanm_jump(objectIndex, 3);
            }
            break;
        case 13:
            if (kwanm_turn_left(objectIndex, (u16*) &g_DynamicObjects[objectIndex].texang[1], 0x0400U, 0x00014000) != 0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 14:
            if (kwadd_fval(&g_DynamicObjects[objectIndex].pos_spline[2], 0.0f, 2.0f) != 0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 15:
            if (kwanm_turn_left(objectIndex, (u16*) &g_DynamicObjects[objectIndex].texang[1], 0x0400U, 0x0000C000) != 0) {
                KWAnmNext(objectIndex);
                kwanm_jump(objectIndex, 3);
            }
            break;
        case 17:
            kwmv_jump(objectIndex, 1);
            break;
        case 0:
        default:
            break;
    }
}

void kwmv_doshin_kurukuru_r(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].mvptr) {
        case 1:
            if (kwanm_turn_left(objectIndex, (u16*) &g_DynamicObjects[objectIndex].texang[1], 0x0400U, 0x00010000) != 0) {
                g_DynamicObjects[objectIndex].pos_ende[0] = (f32) ((f64) xOrientation * -200.0);
                KWAnmNext(objectIndex);
            }
            break;
        case 2:
            if (KWChaseFVal(g_DynamicObjects[objectIndex].pos_spline, g_DynamicObjects[objectIndex].pos_ende[0], 4.0f) != 0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 3:
            if (kwanm_turn_left(objectIndex, (u16*) &g_DynamicObjects[objectIndex].texang[1], 0x0400U, 0x00010000) != 0) {
                kwanm_jump(objectIndex, 3);
                KWAnmNext(objectIndex);
            }
            break;
        case 5:
            if (kwanm_turn_left(objectIndex, (u16*) &g_DynamicObjects[objectIndex].texang[1], 0x0400U, 0x00004000) != 0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 6:
            if (kwadd_fval(&g_DynamicObjects[objectIndex].pos_spline[2], 100.0f, 2.0f) != 0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 7:
            if (kwanm_turn_left(objectIndex, (u16*) &g_DynamicObjects[objectIndex].texang[1], 0x0400U, 0x0000C000) != 0) {
                KWAnmNext(objectIndex);
                kwanm_jump(objectIndex, 3);
            }
            break;
        case 9:
            if (kwanm_turn_left(objectIndex, (u16*) &g_DynamicObjects[objectIndex].texang[1], 0x0400U, 0x00008000) != 0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 10:
            if (KWChaseFVal(g_DynamicObjects[objectIndex].pos_spline, 0.0f, 4.0f) != 0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 11:
            if (kwanm_turn_left(objectIndex, (u16*) &g_DynamicObjects[objectIndex].texang[1], 0x0400U, 0x00008000) != 0) {
                KWAnmNext(objectIndex);
                kwanm_jump(objectIndex, 3);
            }
            break;
        case 13:
            if (kwanm_turn_left(objectIndex, (u16*) &g_DynamicObjects[objectIndex].texang[1], 0x0400U, 0x0000C000) != 0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 14:
            if (kwsub_fval(&g_DynamicObjects[objectIndex].pos_spline[2], 0.0f, 2.0f) != 0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 15:
            if (kwanm_turn_left(objectIndex, (u16*) &g_DynamicObjects[objectIndex].texang[1], 0x0400U, 0x00014000) != 0) {
                KWAnmNext(objectIndex);
                kwanm_jump(objectIndex, 3);
            }
            break;
        case 17:
            kwmv_jump(objectIndex, 1);
            break;
        case 0:
        default:
            break;
    }
}

void kwmv_doshin_kurukuru(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].spptr) { /* irregular */
        case 1:
            kwmv_doshin_kurukuru_f(objectIndex);
            break;
        case 2:
            kwmv_doshin_kurukuru_r(objectIndex);
            break;
    }
}

void kwanm_doshin_kurukuru(s32 objectIndex) {

    switch (g_DynamicObjects[objectIndex].anmptr) { /* irregular */
        case 0:
            break;
        case 1:
            kwanm_doshin_kurukuru_init(objectIndex);
            break;
        case 3:
            kwanm_gosubnext(objectIndex, 0x00000032);
            break;
        case 4:
            KWAnmNext(objectIndex);
            kwanm_next(objectIndex);
            break;
    }
    kwanm_doshin_rakka(objectIndex);
    kwmv_doshin_kurukuru(objectIndex);
    kwset_splinepos3D(objectIndex);
    kwanm_texaddr_ci8_index(objectIndex);
}

void kwget_okkake_doshin_1(s32 objectIndex, s32 arg1, s32 arg2) {
    AnmObject* object;

    kwanm_setevflag(objectIndex, 8);
    kwmv_start(objectIndex);
    object = &g_DynamicObjects[objectIndex];
    object->spptr = 1;
    object->anmnum = arg1;
    object->ival = arg2;
}

void kwget_okkake_doshin_2(s32 objectIndex, s32 playerId) {
    Player* player;

    player = &gPlayerOne[playerId];
    kwanm_setevflag(objectIndex, 8);
    kwmv_start(objectIndex);
    g_DynamicObjects[objectIndex].spptr = 2;
    g_DynamicObjects[objectIndex].pos_ende[0] = player->position[0] - g_DynamicObjects[objectIndex].pos_start[0];
    g_DynamicObjects[objectIndex].anmnum = playerId;
}

s32 kwcheck_doshin_okkake_sub(s32 playerId) {
    s32 someIndex;
    s32 objectIndex;
    s32 temp_s7;
    s32 var_s6;
    s32 pathPoint;

    pathPoint = gNearestPathPointByPlayerId[playerId];
    var_s6 = 0;
    if ((pathPoint >= 0xAA) && (pathPoint < 0xB5)) {
        temp_s7 = MakeRandomLimmit(0x0032U) + 0x32;
        for (someIndex = 0; someIndex < gNumActiveThwomps; someIndex++) {
            objectIndex = objallocptr[someIndex];
            if (g_DynamicObjects[objectIndex].chartptr == 3) {
                var_s6 = 1;
                kwget_okkake_doshin_1(objectIndex, playerId, temp_s7);
            }
        }
    } else if ((pathPoint >= 0xD7) && (pathPoint < 0xE2)) {
        for (someIndex = 0; someIndex < gNumActiveThwomps; someIndex++) {
            objectIndex = objallocptr[someIndex];
            if (g_DynamicObjects[objectIndex].chartptr == 3) {
                var_s6 = 1;
                kwget_okkake_doshin_2(objectIndex, playerId);
            }
        }
    }
    return var_s6;
}

void kwcheck_doshin_okkake(void) {
    Player* player;
    s32 objectIndex;
    s32 var_s0;
    s32 someIndex;
    s32 var_s4;
    AnmObject* object;

    player = gPlayerOne;
    var_s4 = 1;
    for (someIndex = 0; someIndex < gNumActiveThwomps; someIndex++) {
        objectIndex = objallocptr[someIndex];
        object = &g_DynamicObjects[objectIndex];
        if (object->chartptr == 3) {
            var_s0 = 0;
            if ((object->anmptr >= 2) && (kwanm_bitoffcheckevflag(objectIndex, 8) != 0)) {
                var_s0 = 1;
            }
            var_s4 *= var_s0;
        }
    }
    if (var_s4 != 0) {
        for (var_s0 = 0; var_s0 < 4; var_s0++, player++) {
            if ((player->flag & EXISTS) && !(player->flag & IS_CPU_PLAYER)) {
                if (kwcheck_doshin_okkake_sub(var_s0) != 0) {
                    break;
                }
            }
        }
    }
}

void kwanm_doshin_okkake_init(s32 objectIndex) {
    AnmObject* object;

    kwanm_init_ci8(objectIndex, d_course_bowsers_castle_thwomp_tlut, (u8*) d_course_bowsers_castle_thwomp_faces,
                        0x10U, (u16) 0x00000040);
    object = &g_DynamicObjects[objectIndex];
    object->shape = d_course_bowsers_castle_dl_thwomp;
    object->r = 0x000C;
    object->scale = 1.0f;
    kwanm_setflag(objectIndex, 0x04000220);
    object->output = 0;
    object->ground = 0.0f;
    object->pos_start[1] = 0.0f;
    kwset_init3D_sppos(objectIndex, 0.0f, 0.0f, 0.0f);
    kwset_init3D_ang(objectIndex, 0U, 0U, 0U);
    if (g_ScreenFlip != 0) {
        kwset_init3D_texang(objectIndex, 0U, 0xC000U, 0U);
    } else {
        kwset_init3D_texang(objectIndex, 0U, 0x4000U, 0U);
    }
    object->velocity[0] = 0.0f;
    object->ang[1] = object->texang[1];
    object->spptr = 1;
    object->unk_0DF = 8;
    object->pos_spline[1] = 15.0f;
    object->pos_ende[1] = 15.0f;
    kwanm_next(objectIndex);
}

void kwmv_doshin_okkake_f(s32 objectIndex) {
    s32 var_v0;
    UNUSED s32 stackPadding;
    Player* player;

    player = &gPlayerOne[g_DynamicObjects[objectIndex].anmnum];
    switch (g_DynamicObjects[objectIndex].mvptr) {
        case 1:
            g_DynamicObjects[objectIndex].mvtm = 0x00A0;
            g_DynamicObjects[objectIndex].pos_spline[0] = 0.0f;
            g_DynamicObjects[objectIndex].pos_spline[2] = 0.0f;
            g_DynamicObjects[objectIndex].velocity[2] = 0.0f;
            KWAnmNext(objectIndex);
            break;
        case 2:
            g_DynamicObjects[objectIndex].velocity[0] = player->speed * xOrientation * 1.25;
            if (g_DynamicObjects[objectIndex].ival >= g_DynamicObjects[objectIndex].mvtm) {
                if (g_DynamicObjects[objectIndex].mvtm == g_DynamicObjects[objectIndex].ival) {
                    if (kwgameframetimer & 1) {
                        g_DynamicObjects[objectIndex].velocity[2] = 1.5f;
                    } else {
                        g_DynamicObjects[objectIndex].velocity[2] = -1.5f;
                    }
                }
                if (g_DynamicObjects[objectIndex].velocity[2] >= 0.0) {
                    if (g_DynamicObjects[objectIndex].pos_spline[2] >= 40.0) {
                        g_DynamicObjects[objectIndex].velocity[2] = -1.5f;
                    }
                } else if ((f64) g_DynamicObjects[objectIndex].pos_spline[2] <= -40.0) {
                    g_DynamicObjects[objectIndex].velocity[2] = 1.5f;
                }
            }
            kwmv_addvelxztopos(objectIndex);
            if (g_DynamicObjects[objectIndex].mvtm < 0x65) {
                g_DynamicObjects[objectIndex].texang[1] = kwchase_angle(
                    g_DynamicObjects[objectIndex].texang[1], (g_DynamicObjects[objectIndex].ang[1] + 0x8000));
                if (g_DynamicObjects[objectIndex].mvtm == 0x0064) {
                    g_DynamicObjects[objectIndex].anmnumbak = 1;
                }
            }
            var_v0 = 0;
            if (g_ScreenFlip != 0) {
                if (g_DynamicObjects[objectIndex].pos_spline[0] <= -1000.0) {
                    var_v0 = 1;
                }
            } else if (g_DynamicObjects[objectIndex].pos_spline[0] >= 1000.0) {
                var_v0 = 1;
            }
            g_DynamicObjects[objectIndex].mvtm--;
            if ((g_DynamicObjects[objectIndex].mvtm == 0) || (var_v0 != 0)) {
                g_DynamicObjects[objectIndex].vel = 0.0f;
                kwanm_jump(objectIndex, 3);
                KWAnmNext(objectIndex);
            }
            break;
        case 4:
            KWChaseFVal(&g_DynamicObjects[objectIndex].pos_spline[2], 0.0f, 2.0f);
            KWChaseFVal(g_DynamicObjects[objectIndex].pos_spline, 0.0f, 5.0f);
            if ((g_DynamicObjects[objectIndex].pos_spline[0] + g_DynamicObjects[objectIndex].pos_spline[2]) == 0.0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 5:
            g_DynamicObjects[objectIndex].texang[1] =
                kwchase_angle(g_DynamicObjects[objectIndex].texang[1], g_DynamicObjects[objectIndex].ang[1]);
            if (g_DynamicObjects[objectIndex].texang[1] == g_DynamicObjects[objectIndex].ang[1]) {
                kwanm_clrevflag(objectIndex, 8);
                KWAnmNext(objectIndex);
                g_DynamicObjects[objectIndex].anmnumbak = 0;
            }
            break;
        case 0:
        case 3:
        default:
            break;
    }
}

void kwmv_doshin_okkake_w(s32 objectIndex) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    switch (object->mvptr) {
        case 1:
            if (KWChaseFVal(&object->pos_spline[0], object->pos_ende[0], 5.0f) != 0) {
                kwanm_jump(objectIndex, 3);
                KWAnmNext(objectIndex);
                break;
            }
        case 0:
        case 2:
            break;
        case 3:
            if (KWChaseFVal(&object->pos_spline[0], 0.0f, 5.0f) != 0) {
                KWAnmNext(objectIndex);
                kwanm_clrevflag(objectIndex, 8);
            }
            break;
    }
}

void kwmv_doshin_okkake(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].spptr) {
        case 1:
            kwmv_doshin_okkake_f(objectIndex);
            break;
        case 2:
            kwmv_doshin_okkake_w(objectIndex);
            break;
    }
}

void kwanm_doshin_okkake(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].anmptr) { /* irregular */
        case 0:
            break;
        case 1:
            kwanm_doshin_okkake_init(objectIndex);
            break;
        case 3:
            kwanm_gosubnext(objectIndex, 0x00000032);
            break;
        case 4:
            kwanm_next(objectIndex);
            KWAnmNext(objectIndex);
            break;
    }
    kwanm_doshin_rakka(objectIndex);
    kwmv_doshin_okkake(objectIndex);
    kwset_splinepos3D(objectIndex);
    kwanm_texaddr_ci8_index(objectIndex);
}

void kwanm_doshin_guard_init(s32 objectIndex) {
    AnmObject* object;

    kwanm_init_ci8(objectIndex, d_course_bowsers_castle_thwomp_tlut, (u8*) d_course_bowsers_castle_thwomp_faces,
                        0x10U, (u16) 0x00000040);
    object = &g_DynamicObjects[objectIndex];
    object->shape = d_course_bowsers_castle_dl_thwomp;
    object->r = 0x000C;
    object->scale = 1.0f;
    kwanm_setflag(objectIndex, 0x04000220);
    object->output = 2;
    object->unk_0DF = 8;
    kwset_init3D_ang(objectIndex, 0U, 0U, 0U);
    object->ground = 0.0f;
    object->pos_start[1] = 0.0f;
    kwset_init3D_sppos(objectIndex, 0.0f, 0.0f, 0.0f);
    object->pos_ende[1] = 30.0f;
    if (g_ScreenFlip != 0) {
        kwset_init3D_texang(objectIndex, 0U, 0x4000U, 0U);
    } else {
        kwset_init3D_texang(objectIndex, 0U, 0xC000U, 0U);
    }
    switch (object->alpha) { /* irregular */
        case 0:
            object->anmtm = 2;
            break;
        case 1:
            object->anmtm = 0x0000003C;
            break;
        case 2:
            object->anmtm = 0x00000078;
            break;
        case 3:
            object->anmtm = 0x000000B4;
            break;
    }
    kwanm_clearstack(objectIndex);
    kwanm_next(objectIndex);
}

void kwanm_doshin_guard(s32 objectIndex) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    switch (object->anmptr) {
        case 0:
            break;
        case 1:
            kwanm_doshin_guard_init(objectIndex);
            break;
        case 2:
            kwanm_wait(objectIndex, object->anmtm);
            break;
        case 3:
            kwanm_gosubnext(objectIndex, 0x00000032);
            break;
        case 4:
            object->anmtm = 0x0000003C;
            kwanm_jump(objectIndex, 2);
            break;
    }
    kwanm_doshin_rakka(objectIndex);
    kwset_splinepos3D(objectIndex);
    kwanm_texaddr_ci8_index(objectIndex);
}

void kwanm_doshin_ori_init(s32 objectIndex) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    object->texsizex = 0;
    kwanm_init_ci8(objectIndex, d_course_bowsers_castle_thwomp_tlut, (u8*) d_course_bowsers_castle_thwomp_faces,
                        0x10U, (u16) 0x00000040);
    object->shape = d_course_bowsers_castle_dl_thwomp;
    object->anmnumbak = 0;
    object->r = 0x000C;
    object->scale = 1.5f;
    kwanm_setflag(objectIndex, 0x05000220);
    object->output = 1;
    object->unk_0DF = 6;
    kwset_init3D_sppos(objectIndex, 0.0f, 0.0f, 0.0f);
    kwset_init3D_ang(objectIndex, 0U, 0U, 0U);
    object->ground = 0.0f;
    object->pos_start[1] = 0.0f;
    object->pos_spline[1] = 10.0f;
    object->pos_ende[1] = 10.0f;
    if (g_ScreenFlip != 0) {
        kwset_init3D_texang(objectIndex, 0U, 0x4000U, 0U);
    } else {
        kwset_init3D_texang(objectIndex, 0U, 0xC000U, 0U);
    }
    object->pos_spline[0] = 0.0f;
    object->pos_spline[2] = 0.0f;
    kwanm_clearstack(objectIndex);
    kwanm_next(objectIndex);
}

void kwanm_doshin_ori(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 0:
            break;
        case 1:
            kwanm_doshin_ori_init(objectIndex);
            break;
        case 2:
            kwcheck_visible_area_camera(objectIndex, 100.0f);
            if (kwanm_bitcheckflag(objectIndex, VISIBLE) != 0) {
                NAEnmTrgStart(g_DynamicObjects[objectIndex].pos, g_DynamicObjects[objectIndex].velocity,
                              SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x45));
                kwanm_next(objectIndex);
            }
            break;
        case 3:
            if (kwanm_oufuku_f(objectIndex, 3, 5, 1, 6, 6) != 0) {
                g_DynamicObjects[objectIndex].anmnumbak = 0;
            }
            break;
        case 4:
            if (kwanm_wait(objectIndex, 0x0000012C) != 0) {
                kwanm_jump(objectIndex, 2);
            }
            break;
    }
    kwset_splinepos3D(objectIndex);
    kwanm_texaddr_ci8_index(objectIndex);
}

void kwanm_doshin_oufuku_init(s32 objectIndex) {
    AnmObject* object;

    kwanm_init_ci8(objectIndex, d_course_bowsers_castle_thwomp_tlut, (u8*) d_course_bowsers_castle_thwomp_faces,
                        0x10U, (u16) 0x00000040);
    object = &g_DynamicObjects[objectIndex];
    object->shape = d_course_bowsers_castle_dl_thwomp;
    object->r = 0x000C;
    object->anmnumbak = 0;
    object->scale = 1.0f;
    kwanm_setflag(objectIndex, 0x04000220);
    object->output = 0;
    object->unk_0DF = 0x0A;
    kwmv_start(objectIndex);
    kwset_init3D_sppos(objectIndex, 0.0f, 0.0f, 0.0f);
    object->ground = 70.0f;
    object->pos_start[1] = 70.0f;
    object->pos_ende[1] = 0.0f;
    kwset_init3D_ang(objectIndex, 0U, 0U, 0U);
    if ((g_ScreenFlip != 0) || (gGamestate == 9)) {
        kwset_init3D_texang(objectIndex, 0U, 0xC000U, 0U);
    } else {
        kwset_init3D_texang(objectIndex, 0U, 0x4000U, 0U);
    }
    switch (object->alpha) { /* irregular */
        case 0:
            object->spptr = 2;
            object->velocity[2] = -1.0f;
            break;
        case 1:
            object->spptr = 2;
            object->velocity[2] = -1.5f;
            break;
    }
    kwanm_setevflag(objectIndex, 0x00000080);
    kwanm_next(objectIndex);
}

void kwmv_doshin_oufuku_f(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].mvptr) {
        case 0:
            break;
        case 1:
            if (KWChaseFVal(&g_DynamicObjects[objectIndex].pos_spline[2], 250.0f, g_DynamicObjects[objectIndex].velocity[2]) !=
                0) {
                g_DynamicObjects[objectIndex].velocity[2] = -g_DynamicObjects[objectIndex].velocity[2];
                KWAnmNext(objectIndex);
            }
            break;
        case 2:
            if (KWChaseFVal(&g_DynamicObjects[objectIndex].pos_spline[2], 0.0f, g_DynamicObjects[objectIndex].velocity[2]) !=
                0) {
                g_DynamicObjects[objectIndex].velocity[2] = -g_DynamicObjects[objectIndex].velocity[2];
                kwmv_jump(objectIndex, 1);
            }
            break;
    }
}

void kwmv_doshin_oufuku_r(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].mvptr) {
        case 0:
            break;
        case 1:
            if (KWChaseFVal(&g_DynamicObjects[objectIndex].pos_spline[2], -250.0f, g_DynamicObjects[objectIndex].velocity[2]) !=
                0) {
                g_DynamicObjects[objectIndex].velocity[2] = -g_DynamicObjects[objectIndex].velocity[2];
                KWAnmNext(objectIndex);
            }
            break;
        case 2:
            if (KWChaseFVal(&g_DynamicObjects[objectIndex].pos_spline[2], 0.0f, g_DynamicObjects[objectIndex].velocity[2]) !=
                0) {
                g_DynamicObjects[objectIndex].velocity[2] = -g_DynamicObjects[objectIndex].velocity[2];
                kwmv_jump(objectIndex, 1);
            }
            break;
    }
}

void kwmv_doshin_oufuku(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].spptr) {
        case 1:
            kwmv_doshin_oufuku_f(objectIndex);
            break;
        case 2:
            kwmv_doshin_oufuku_r(objectIndex);
            break;
    }
    kwset_splinepos3D(objectIndex);
}

void kwanm_doshin_oufuku(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].anmptr) { /* irregular */
        case 0:
            break;
        case 1:
            kwanm_doshin_oufuku_init(objectIndex);
            break;
        case 2:
            kwanm_oufuku_f(objectIndex, 3, 5, 1, 6, -1);
            break;
    }
    if (g_DynamicObjects[objectIndex].anmptr >= 2) {
        kwanm_doshin_rakka(objectIndex);
        kwmv_doshin_oufuku(objectIndex);
        kwanm_texaddr_ci8_index(objectIndex);
        if (gGamestate != 9) {
            if ((D_8018D40C == 0) && (g_DynamicObjects[objectIndex].anmptr == 2)) {
                NAEnmTrgStart(g_DynamicObjects[objectIndex].pos, g_DynamicObjects[objectIndex].velocity,
                              SOUND_ARG_LOAD(0x19, 0x03, 0x60, 0x45));
            }
        } else if ((gCutsceneShotTimer < 0xBF) && (((s16) gCutsceneShotTimer % 88) == 0x0000001E)) {
            NAEnmTrgStart(g_DynamicObjects[objectIndex].pos, g_DynamicObjects[objectIndex].velocity,
                          SOUND_ARG_LOAD(0x19, 0x03, 0x60, 0x45));
        }
    }
}

void kwdarkcheck_doshin(s32 objectIndex, Player* player) {
    if (kwanm_checkradius_player_xz(objectIndex, player, 12.0f) != 0) {
        player->tire_FL.Dummy |= 3;
    }
}

void kwshakecamera_doshin(s32 objectIndex, s32 cameraPlayerId) {
    Camera* camera = &camera1[cameraPlayerId];
    Player* player = &gPlayerOne[cameraPlayerId];

    if (g_ScreenSplitB != SCREEN_MODE_3P_4P_SPLITSCREEN) {
        if ((kwanm_bitcheckevflag(objectIndex, 0x00000010) != 0) &&
            (kwanm_checkradius_player_xz(objectIndex, player, 500.0f) != false)) {
            ShakeCamera(camera);
            NAEnmTrgStart(g_DynamicObjects[objectIndex].pos, g_DynamicObjects[objectIndex].velocity,
                          SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x0F));
        }
    }
}

void kwhitcheck_doshin(s32 objectIndex, s32 playerId) {
    f32 temp_f0;
    Player* temp_s0;

    temp_s0 = &gPlayerOne[playerId];
    if (kwanm_bitcheckflag(objectIndex, 0x00000200) != 0) {
        if (!(temp_s0->weapon & PRESS_SQUISH)) {
            temp_f0 = kwanm_setheight_ground(objectIndex, temp_s0);
            if ((temp_f0 <= 9.0) && !(temp_s0->slip_flag & BROKEN) &&
                (kwanm_checkcoll_player_xz(objectIndex, temp_s0) != 0)) {
                if ((temp_s0->flag & EXISTS) && !(temp_s0->flag & IS_GHOST)) {
                    if (!(temp_s0->slip_flag & STAR)) {
                        kwreboundsnd_player(objectIndex, playerId, 1.4f, 1.1f, SOUND_ARG_LOAD(0x19, 0x00, 0xA0, 0x4C));
                    } else if (kwanm_bitoffcheckevflag(objectIndex, 0x00000040) != 0) {
                        if (temp_s0->flag & IS_CPU_PLAYER) {
                            NAEnmTrgStart(temp_s0->position, temp_s0->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0xA2, 0x4A));
                        } else {
                            NAPlyTrgStart((u8) playerId, SOUND_ARG_LOAD(0x19, 0x01, 0xA2, 0x4A));
                        }
                        kwhitclear_doshin(objectIndex);
                        kwgetbroken_object(g_DynamicObjects[objectIndex].pos, 3, 3, D_8018D3C4);
                        kwanm_clrflag(objectIndex, 0x00000200);
                        kwanm_setevflag(objectIndex, 0x00000040);
                        kwmv_finish(objectIndex);
                        kwanm_jump(objectIndex, 0x000000C8);
                    }
                }
            } else if ((temp_f0 <= 17.5) && (kwanm_bitcheckevflag(objectIndex, 1) != 0) &&
                       (kwanm_checkradius_player_xz(objectIndex, temp_s0, (temp_s0->speed * 0.5) + 7.0) !=
                        0)) {
                if ((temp_s0->flag & EXISTS) && !(temp_s0->flag & IS_GHOST)) {
                    if (kwanm_bitcheckflag(objectIndex, 0x04000000) != 0) {
                        kwlost_ghost();
                    }
                    kwanm_setevflag(objectIndex, 2);
                    temp_s0->link_doshin = (s16) objectIndex;
                    temp_s0->hit_flag |= CRITTER_TOUCH;
                    temp_s0->weapon |= PRESS_SQUISH;
                    kwstop_kart(temp_s0);
                }
            }
        } else {
            kwstop_kart(temp_s0);
        }
    }
}

void kwhitclear_doshin(s32 arg0) {
    Player* player;
    s32 var_v1;

    player = gPlayerOne;
    for (var_v1 = 0; var_v1 < NUM_PLAYERS; var_v1++, player++) {
        if (arg0 == player->link_doshin) {
            player->weapon &= ~PRESS_SQUISH;
            player->link_doshin = -1;
        }
    }
}

void kwinit_doshin_smoke(s32 objectIndex1, s32 objectIndex2, s32 arg2) {
    u16 anAngle;
    f32 thing1;
    f32 thing0;

    KWAnmStart(objectIndex1, arg2);
    g_DynamicObjects[objectIndex1].chartptr = 2;
    anAngle = g_DynamicObjects[objectIndex2].ang[1];
    thing1 = kwrotate2D_x(D_800E594C[arg2 * 2 + 1], D_800E594C[arg2 * 2 + 0], anAngle);
    thing0 = kwrotate2D_y(D_800E594C[arg2 * 2 + 1], D_800E594C[arg2 * 2 + 0], anAngle);
    g_DynamicObjects[objectIndex1].pos_start[0] = g_DynamicObjects[objectIndex2].pos[0] + thing0;
    g_DynamicObjects[objectIndex1].pos_start[1] = g_DynamicObjects[objectIndex2].ground - 9.0;
    g_DynamicObjects[objectIndex1].pos_start[2] = g_DynamicObjects[objectIndex2].pos[2] + thing1;
    anAngle = g_DynamicObjects[objectIndex2].ang[1] + D_800E597C[arg2];
    g_DynamicObjects[objectIndex1].velocity[0] = sinT(anAngle) * 0.6;
    g_DynamicObjects[objectIndex1].velocity[2] = cosT(anAngle) * 0.6;
}

void kwget_smoke_doshin(s32 arg0) {
    s32 objectIndex;
    s32 i;

    for (i = 0; i < 6; i++) {
        objectIndex = kwalloc_effectram(EffectAllocArray2, &gNextFreeObjectParticle2, gObjectParticle2_SIZE);
        if (objectIndex == NULL_OBJECT_ID) {
            break;
        }
        kwinit_doshin_smoke(objectIndex, arg0, i);
    }
}

void kwanm_doshin_smoke_init(s32 objectIndex) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    object->indexaddr = D_8018D490;
    object->sindexaddr = D_8018D490;
    object->alpha = 0x00FF;
    object->ang[1] = 0;
    object->texang[0] = 0;
    object->texang[2] = 0;
    object->pos_spline[0] = 0.0f;
    object->pos_spline[1] = 0.0f;
    object->pos_spline[2] = 0.0f;
    object->scale = 0.25f;
    kwanm_next(objectIndex);
}

void kwanm_doshin_smoke(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].anmptr) { /* irregular */
        case 0:
            break;
        case 1:
            kwanm_doshin_smoke_init(objectIndex);
            break;
        case 2:
            kwmv_addvelxztopos(objectIndex);
            kwadd_fval(&g_DynamicObjects[objectIndex].pos_spline[1], 14.0f, 0.5f);
            kwsetfval_forward_limit(objectIndex, &g_DynamicObjects[objectIndex].scale, 0.25f, 0.75f, 0.025f, 1, 0);
            if (kwsetsval_reverse(objectIndex, &g_DynamicObjects[objectIndex].alpha, 0x000000FF, 0, 4, 0, 0) != 0) {
                kwanm_next(objectIndex);
            }
            break;
        case 3:
            kwanm_finish(objectIndex);
            break;
    }
    kwset_splinepos3D(objectIndex);
}

// update twomp I think
void kwchart_doshin(void) {
}

void kwchart_doshin_vf(void) {
    Player* player;
    s32 objectIndex;
    s32 var_s2_3;
    s32 var_s4;

    D_80165834[0] += 0x100;
    D_80165834[1] += 0x200;
    for (var_s4 = 0; var_s4 < gNumActiveThwomps; var_s4++) {
        objectIndex = objallocptr[var_s4];
        kwanm_clrevflag(objectIndex, 0x00000010);
        kwcheck_areain_camera(objectIndex);
    }
    kwcheck_doshin_okkake();
    for (var_s4 = 0; var_s4 < gNumActiveThwomps; var_s4++) {
        objectIndex = objallocptr[var_s4];
        if (g_DynamicObjects[objectIndex].anmptr != 0) {
            switch (g_DynamicObjects[objectIndex].chartptr) {
                case 1:
                    kwanm_doshin(objectIndex);
                    break;
                case 2:
                    kwanm_doshin_kurukuru(objectIndex);
                    break;
                case 3:
                    kwanm_doshin_okkake(objectIndex);
                    break;
                case 4:
                    kwanm_doshin_guard(objectIndex);
                    break;
                case 6:
                    kwanm_doshin_ori(objectIndex);
                    break;
                case 5:
                    kwanm_doshin_oufuku(objectIndex);
                    break;
            }
        }
    }
    player = gPlayerOne;
    for (var_s4 = 0; var_s4 < NUM_PLAYERS; var_s4++, player++) {
        player->tire_FL.Dummy &= ~3;
        player->hit_flag &= ~(CRITTER_TOUCH_GATE | CRITTER_TOUCH);
        for (var_s2_3 = 0; var_s2_3 < gNumActiveThwomps; var_s2_3++) {
            objectIndex = objallocptr[var_s2_3];
            if (!(player->slip_flag & TERESA)) {
                kwhitcheck_doshin(objectIndex, var_s4);
            }
            if (kwanm_bitcheckflag(objectIndex, 0x00020000) != 0) {
                kwdarkcheck_doshin(objectIndex, player);
            }
            if (kwanm_bitcheckflag(objectIndex, 0x00010000) != 0) {
                kwshakecamera_doshin(objectIndex, var_s4);
            }
        }
    }
    kwchart_broken_object(3);
    for (var_s4 = 0; var_s4 < gNumActiveThwomps; var_s4++) {
        objectIndex = objallocptr[var_s4];
        if (kwanm_bitcheckevflag(objectIndex, 0x00000020) == 0) {
            continue;
        }

        kwanm_clrevflag(objectIndex, 0x00000020);
        kwget_smoke_doshin(objectIndex);
    }
    for (var_s4 = 0; var_s4 < gObjectParticle2_SIZE; var_s4++) {
        objectIndex = EffectAllocArray2[var_s4];
        if (objectIndex == DELETED_OBJECT_ID) {
            continue;
        }
        if (g_DynamicObjects[objectIndex].anmptr == 0) {
            continue;
        }
        kwanm_doshin_smoke(objectIndex);
        if (g_DynamicObjects[objectIndex].anmptr != 0) {
            continue;
        }
        kwdealloc_effectram(&EffectAllocArray2[var_s4]);
    }
}

void kwset_tuchi(s32 objectIndex) {
    UNUSED s32 stackPadding[3];
    s32 sp70;
    s32 var_s1;
    s32 var_s7;
    s32 loopObjectIndex;

    if (g_playerCount == 1) {
        sp70 = 8;
    } else {
        sp70 = 4;
    }

    for (var_s7 = 0; var_s7 < sp70; var_s7++) {
        for (var_s1 = 0; var_s1 < gObjectParticle2_SIZE; var_s1++) {
            loopObjectIndex = EffectAllocArray2[var_s1];

            if (g_DynamicObjects[loopObjectIndex].anmptr != 0) {
                continue;
            }

            KWAnmStart(loopObjectIndex, 0);
            g_DynamicObjects[loopObjectIndex].texaddr = d_course_moo_moo_farm_mole_dirt;
            g_DynamicObjects[loopObjectIndex].stexaddr = d_course_moo_moo_farm_mole_dirt;
            g_DynamicObjects[loopObjectIndex].scale = 0.15f;
            g_DynamicObjects[loopObjectIndex].velocity[1] = MakeRandomLimmit(0x000AU);
            g_DynamicObjects[loopObjectIndex].velocity[1] = (g_DynamicObjects[loopObjectIndex].velocity[1] * 0.1) + 4.8;
            g_DynamicObjects[loopObjectIndex].vel = MakeRandomLimmit(5U);
            g_DynamicObjects[loopObjectIndex].vel = (g_DynamicObjects[loopObjectIndex].vel * 0.01) + 0.8;
            g_DynamicObjects[loopObjectIndex].texang[1] = (0x10000 / sp70) * var_s1;
            g_DynamicObjects[loopObjectIndex].pos_start[0] = g_DynamicObjects[objectIndex].pos_start[0];
            g_DynamicObjects[loopObjectIndex].pos_start[1] = g_DynamicObjects[objectIndex].pos_start[1] - 13.0;
            g_DynamicObjects[loopObjectIndex].pos_start[2] = g_DynamicObjects[objectIndex].pos_start[2];
            break;
        }
    }
}

void kwanm_tuchi(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 0:
            break; /* irregular */
        case 1:
            if (kwmv_nanamenage(objectIndex, g_DynamicObjects[objectIndex].velocity[1], 0.3f, g_DynamicObjects[objectIndex].vel,
                              g_DynamicObjects[objectIndex].texang[1], 0x00000032) != 0) {
                kwanm_next(objectIndex);
            }
            kwset_splinepos3D(objectIndex);
            break;
        case 2:
            kwanm_finish(objectIndex);
            kwmv_finish(objectIndex);
            break;
    }
}

void kwanm_choropu_init(s32 objectIndex) {
    kwanm_init_ci8(objectIndex, d_course_moo_moo_farm_mole_tlut, (u8*) d_course_moo_moo_farm_mole_frames, 0x20U,
                        (u16) 0x00000040);
    g_DynamicObjects[objectIndex].scale = 0.15f;
    g_DynamicObjects[objectIndex].anmnumbak = 0;
    kwset_init3D_sppos(objectIndex, 0.0f, 0.0f, 0.0f);
    kwset_init3D_ang(objectIndex, 0U, 0U, 0U);
    kwset_init3D_texang(objectIndex, 0U, 0U, 0x8000U);
    g_DynamicObjects[objectIndex].r = 6;
    g_DynamicObjects[objectIndex].velocity[1] = 4.0f;
    kwanm_setflag(objectIndex, 0x04000000);
    kwanm_next(objectIndex);
}

void kwmv_choropu_sub(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].mvptr) {
        case 1:
            if (kwadd_fval(&g_DynamicObjects[objectIndex].pos_spline[1], 9.0f, 0.7f) != 0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 2:
            kwmv_waitnext(objectIndex, 0x0000000A);
            break;
        case 3:
            if (kwsub_fval(&g_DynamicObjects[objectIndex].pos_spline[1], 3.0f, 1.0f) != 0) {
                kwmv_finish(objectIndex);
            }
            break;
        case 4:
            kwmv_nageage_ground(objectIndex, 3.6f, 0.25f, 0.0f);
            break;
        case 5:
            kwmv_finish(objectIndex);
            break;
        case 10:
            g_DynamicObjects[objectIndex].texang[2] += 0x1000;
            g_DynamicObjects[objectIndex].velocity[1] -= 0.184;
            kwmv_vxz(objectIndex);
            kwmv_addveltopos(objectIndex);
            if (g_DynamicObjects[objectIndex].pos[1] <= -10.0) {
                kwmv_finish(objectIndex);
            }
            break;
        case 0:
        default:
            break;
    }
}

void kwmv_choropu(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].spptr) { /* irregular */
        case 0:
            break;
        case 1:
            kwsp_strategy3D(objectIndex);
            break;
        case 2:
            kwmv_choropu_sub(objectIndex);
            break;
    }
    kwset_splinepos3D(objectIndex);
}

void kwanm_choropu(s32 objectIndex, s32 arg1) {
    s8* sp2C;
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    switch (object->anmptr) { /* irregular */
        case 0x1:
            kwanm_choropu_init(objectIndex);
            break;
        case 0x2:
            if (object->timer == 0) {
                kwmv_start_normal(objectIndex, 2, 1);
                kwanm_next(objectIndex);
                kwanm_setflag(objectIndex, 0x00000200);
            } else {
                object->timer--;
            }
            break;
        case 0x3:
            if (object->mvptr == 0) {
                kwmv_start_normal(objectIndex, 2, 4);
                kwset_tuchi(objectIndex);
                kwanm_next(objectIndex);
                NAEnmTrgStart(object->pos, object->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x07));
            }
            break;
        case 0x4:
            if (kwanm_forward(objectIndex, 1, 6, 1, 2, 0) != 0) {
                kwanm_jump(objectIndex, 0x00000064);
            }
            break;
        case 0xA:
            kwanm_forward(objectIndex, 1, 6, 1, 0, -1);
            if (object->mvptr == 0) {
                kwanm_jump(objectIndex, 0x00000064);
            }
            break;
        case 0x64:
            if (object->mvptr == 0) {
                kwanm_clrflag(objectIndex, 0x00000200);
                kwanm_finish(objectIndex);
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
                sp2C[object->output] = 0;
            }
            break;
        case 0:
        default:
            break;
    }
    if (object->anmptr >= 2) {
        kwanm_texaddr_ci8_index(objectIndex);
    }
}

void kwhitcheck_choropu(s32 objectIndex) {
    Player* player;
    Camera* var_s4;
    s32 playerIndex;
    s32 var_s5;
    AnmObject* object;

    var_s5 = 0;
    player = gPlayerOne;
    var_s4 = camera1;
    for (playerIndex = 0; playerIndex < kwkartnum; playerIndex++, player++, var_s4++) {
        if ((kwanm_bitcheckflag(objectIndex, 0x00000200) != 0) && !(player->slip_flag & TERESA) &&
            (kwanm_checkcoll_player_xyz(objectIndex, player) != 0)) {
            if ((player->flag & EXISTS) && !(player->flag & IS_GHOST)) {
                var_s5 = 1;
                object = &g_DynamicObjects[objectIndex];
                if (kwanm_bitcheckflag(objectIndex, 0x04000000) != 0) {
                    kwlost_ghost();
                }
                if (player->slip_flag & STAR) {
                    NAPlyTrgStart(playerIndex, 0x1900A046U);
                } else {
                    player->weapon |= HIT_REDSHELL;
                }
                object->ang[1] = var_s4->camera_direction[1];
                object->velocity[1] = (player->speed / 2) + 3.0;
                object->vel = player->speed + 1.0;
                if (object->velocity[1] >= 5.0) {
                    object->velocity[1] = 5.0f;
                }
                if (object->vel >= 4.0) {
                    object->velocity[1] = 4.0f;
                }
            }
        }
    }
    if (var_s5 != 0) {
        object = &g_DynamicObjects[objectIndex];
        kwanm_clrflag(objectIndex, 0x00000200);
        kwmv_finish(objectIndex);
        kwset_init3D_spos(objectIndex, object->pos[0], object->pos[1], object->pos[2]);
        kwset_init3D_sppos(objectIndex, 0.0f, 0.0f, 0.0f);
        kwmv_start_normal(objectIndex, 2, 0x000A);
        kwanm_jump(objectIndex, 0x0000000A);
    }
}

void kwset_rnd_choropu(s32 objectIndex, s32 arg1) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    s32 moleCount;
    s16 var_v1;
    s16 offset;
    s32 var_a0;
    s8* var_a2;

    KWAnmStart(objectIndex, 0);
    g_DynamicObjects[objectIndex].timer = MakeRandomLimmit(0x001EU) + 5;
    switch (arg1) { /* irregular */
        case 1:
            var_a2 = D_8018D198;
            moleCount = NUM_GROUP1_MOLES;
            offset = 0;
            break;
        case 2:
            var_a2 = D_8018D1A8;
            moleCount = NUM_GROUP2_MOLES;
            offset = 24;
            // offset = NUM_GROUP1_MOLES;
            break;
        case 3:
            var_a2 = D_8018D1B8;
            moleCount = NUM_GROUP3_MOLES;
            offset = 57;
            // offset = NUM_GROUP1_MOLES + NUM_GROUP2_MOLES;
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
            g_DynamicObjects[objectIndex].output = var_v1;
            break;
        }
    }
    /*
    Ideally `gMoleSpawns` wouldn't be a union at all and its just be a list of Vec3s
    Even more ideally each mole group would have its own array for its spawns
    g_DynamicObjects[objectIndex].pos_start[0] = gMoleSpawns.asVec3sList[offset + var_v1][0] * xOrientation;
    g_DynamicObjects[objectIndex].pos_start[1] = gMoleSpawns.asVec3sList[offset + var_v1][1] - 9.0;
    g_DynamicObjects[objectIndex].pos_start[2] = gMoleSpawns.asVec3sList[offset + var_v1][2];
    */
    g_DynamicObjects[objectIndex].pos_start[0] = gMoleSpawns.asFlatList[offset + (var_v1 * 3) + 0] * xOrientation;
    g_DynamicObjects[objectIndex].pos_start[1] = gMoleSpawns.asFlatList[offset + (var_v1 * 3) + 1] - 9.0;
    g_DynamicObjects[objectIndex].pos_start[2] = gMoleSpawns.asFlatList[offset + (var_v1 * 3) + 2];
}

void kwchart_choropu_sub(s32 objectIndex, s32 arg1) {
    if (g_DynamicObjects[objectIndex].anmptr != 0) {
        kwanm_choropu(objectIndex, arg1);
        kwmv_choropu(objectIndex);
        kwhitcheck_choropu(objectIndex);
    }
}

void kwchart_choropu(void) {
    s32 var_s1;
    s32 objectIndex;
    UNUSED s32 stackPadding;

    for (var_s1 = 0; var_s1 < D_8018D1C8; var_s1++) {
        objectIndex = objallocptr[var_s1];
        if (g_DynamicObjects[objectIndex].anmptr == 0) {
            if (kwcheck_mapareain(8, 9) != 0) {
                kwset_rnd_choropu(objectIndex, 1);
            }
        } else {
            kwchart_choropu_sub(objectIndex, 1);
        }
    }

    for (var_s1 = 0; var_s1 < D_8018D1D0; var_s1++) {
        objectIndex = objallocptr2[var_s1];
        if (g_DynamicObjects[objectIndex].anmptr == 0) {
            if (kwcheck_mapareain(0x0010, 0x0013) != 0) {
                kwset_rnd_choropu(objectIndex, 2);
            }
        } else {
            kwchart_choropu_sub(objectIndex, 2);
        }
    }

    for (var_s1 = 0; var_s1 < D_8018D1D8; var_s1++) {
        objectIndex = objallocptr3[var_s1];
        if (g_DynamicObjects[objectIndex].anmptr == 0) {
            if (kwcheck_mapareain(0x0011, 0x0014) != 0) {
                kwset_rnd_choropu(objectIndex, 3);
            }
        } else {
            kwchart_choropu_sub(objectIndex, 3);
        }
    }

    for (var_s1 = 0; var_s1 < gObjectParticle2_SIZE; var_s1++) {
        objectIndex = EffectAllocArray2[var_s1];
        if (g_DynamicObjects[objectIndex].anmptr != 0) {
            kwanm_tuchi(objectIndex);
        }
    }
}

void kwanm_kamome_init(s32 objectIndex, s32 arg1) {
    UNUSED s16 stackPadding0;
    s16 temp_f4;
    s16 sp22;
    s16 sp20;

    g_DynamicObjects[objectIndex].texsizex = 1;
    g_DynamicObjects[objectIndex].shape = (Gfx*) d_course_koopa_troopa_beach_unk4;
    g_DynamicObjects[objectIndex].vtxaddr = (Vtx*) d_course_koopa_troopa_beach_unk_data5;
    g_DynamicObjects[objectIndex].scale = 0.2f;
    g_DynamicObjects[objectIndex].spptr = 1;
    sp22 = MakeRandomLimmit(0x00C8) + -100.0;
    sp20 = MakeRandomLimmit(0x0014);
    temp_f4 = MakeRandomLimmit(0x00C8) + -100.0;
    if (gGamestate == 9) {
        kwset_init3D_spos(objectIndex, sp22 + -360.0, sp20 + 60.0, temp_f4 + -1300.0);
    } else if (g_DynamicObjects[objectIndex].chartptr != 0) {
        kwset_init3D_spos(objectIndex, (sp22 + 328.0) * xOrientation, sp20 + 20.0, temp_f4 + 2541.0);
    } else {
        kwset_init3D_spos(objectIndex, (sp22 + -985.0) * xOrientation, sp20 + 15.0, temp_f4 + 1200.0);
    }
    kwset_init3D_ang(objectIndex, 0U, 0U, 0U);
    g_DynamicObjects[objectIndex].vel = 1.0f;
    kwmv_start_strategy(objectIndex);
    g_DynamicObjects[objectIndex].sptbtopaddr = D_800E633C[arg1 % 4];
    kwanm_setflag(objectIndex, 0x00000800);
    kwanm_next(objectIndex);
}

void kwanm_kamome(s32 objectIndex, s32 arg1) {
    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 1:
            kwanm_kamome_init(objectIndex, arg1);
            break;
        case 0:
        default:
            break;
    }
}

void kwmv_kamome(s32 objectIndex) {
    UNUSED s32 stackPadding;
    switch (g_DynamicObjects[objectIndex].spptr) { /* irregular */
        case 1:
            kwsp_strategy3D_loop(objectIndex);
            kwset_splinepos3D(objectIndex);
            break;
        case 2:
            kwsp_strategy3D_loop(objectIndex);
            kwcopy_fval(g_DynamicObjects[objectIndex].pos_ende, g_DynamicObjects[objectIndex].pos);
            hanyou_gyaku_soukou_sub_BP(g_DynamicObjects[objectIndex].pos_start, (s16*) &g_DynamicObjects[objectIndex].destangy,
                          g_DynamicObjects[objectIndex].vel, 0.0f, 0);
            g_DynamicObjects[objectIndex].pos_spline[0] *= 2.0;
            g_DynamicObjects[objectIndex].pos_spline[1] *= 2.5;
            g_DynamicObjects[objectIndex].pos_spline[2] *= 2.0;
            kwset_splinepos3D(objectIndex);
            g_DynamicObjects[objectIndex].ang[1] =
                CalcDirection(g_DynamicObjects[objectIndex].pos_ende, g_DynamicObjects[objectIndex].pos);
            break;
    }
    kwchase_velxztoangy(objectIndex);
}

void kwchart_kamome(void) {
    AnmObject* object;
    UNUSED s32* var_s4;
    s32 temp_s0;
    s32 var_s3;

    for (var_s3 = 0; var_s3 < NUM_SEAGULLS; var_s3++) {
        temp_s0 = objallocptr2[var_s3];

        object = &g_DynamicObjects[temp_s0];
        if (object->anmptr == 0) {
            continue;
        }

        kwanm_kamome(temp_s0, var_s3);
        kwmv_kamome(temp_s0);
        if (kwanm_bitcheckevflag(temp_s0, 2) != 0) {
            kwanm_clrevflag(temp_s0, 2);
            if (D_80165A90 != 0) {
                D_80165A90 = 0;
                D_80183E40[0] = 0.0f;
                D_80183E40[1] = 0.0f;
                D_80183E40[2] = 0.0f;
                if (gGamestate != CREDITS_SEQUENCE) {
                    NAEnmTrgStart(object->pos, D_80183E40, SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x43));
                } else {
                    temp_s0 = objallocptr2[1];
                    if (gCutsceneShotTimer < 0x97) {
                        object = &g_DynamicObjects[temp_s0];
                        NAEnmTrgStart(object->pos, D_80183E40, SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x43));
                    }
                }
            }
        }
    }
    if (D_80165900 != 0) {
        D_80165900 -= 1;
    } else {
        if (gGamestate != 9) {
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

void kwanm_kani_init(s32 objectIndex) {
    AnmObject* object;

    kwanm_init_ci8(objectIndex, d_course_koopa_troopa_beach_crab_tlut,
                        (u8*) d_course_koopa_troopa_beach_crab_frames, 0x40U, (u16) 0x00000040);
    object = &g_DynamicObjects[objectIndex];
    object->scale = 0.15f;
    object->anmnumbak = 0;
    kwanm_next(objectIndex);
    object->r = 1;
    kwanm_setflag(objectIndex, 0x04000420);
    kwmv_start_normal(objectIndex, 0, 1);
    kwset_init3D_sppos(objectIndex, 0.0f, 0.0f, 0.0f);
    kwset_init3D_texang(objectIndex, 0U, 0U, 0x8000U);
    object->vel = 1.5f;
    kwanm_setflag(objectIndex, 0x00000200);
}

void kwanm_kani(s32 objectIndex, UNUSED s32 unused) {
    switch (g_DynamicObjects[objectIndex].anmptr) { /* irregular */
        case 0:
            break;
        case 1:
            kwanm_kani_init(objectIndex);
            break;
        case 2:
            kwanm_forward(objectIndex, 0, 3, 1, 2, -1);
            break;
        case 3:
            kwanm_forward(objectIndex, 4, 6, 1, 2, -1);
            break;
    }
    if (g_DynamicObjects[objectIndex].anmptr >= 2) {
        kwanm_texaddr_ci8_index(objectIndex);
    }
}

void kwmv_kani(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].mvptr) {
        case 1:
            if (kwmv_walkxz_2pt(objectIndex, g_DynamicObjects[objectIndex].pos_start[0], g_DynamicObjects[objectIndex].pos_ende[0],
                              g_DynamicObjects[objectIndex].pos_start[2], g_DynamicObjects[objectIndex].pos_ende[2]) != 0) {
                kwanm_jump(objectIndex, 3);
                KWAnmNext(objectIndex);
            }
            break;
        case 2:
            if (kwmv_randomwait(objectIndex, 0x003CU) != 0) {
                g_DynamicObjects[objectIndex].vel = 0.8f;
                kwanm_jump(objectIndex, 2);
                KWAnmNext(objectIndex);
            }
            break;
        case 3:
            if (kwmv_walkxz_reverse(objectIndex, 0x0000003C) != 0) {
                KWAnmNext(objectIndex);
                kwanm_jump(objectIndex, 3);
            }
            break;
        case 4:
            if (kwmv_randomwait(objectIndex, 0x003CU) != 0) {
                kwanm_jump(objectIndex, 2);
                KWAnmNext(objectIndex);
            }
            break;
        case 5:
            if (kwmv_walkxz(objectIndex, 0x0000003C) != 0) {
                kwanm_jump(objectIndex, 3);
                kwmv_jump(objectIndex, 2);
            }
            break;
    }
    kwset_splinepos3D(objectIndex);
    if (kwanm_bitcheckflag(objectIndex, VISIBLE) != 0) {
        kwanm_seton_ground(objectIndex);
        g_DynamicObjects[objectIndex].pos[1] = (f32) (g_DynamicObjects[objectIndex].ground + 2.5);
    }
}

void kwhitcheck_kani(s32 objectIndex) {
    if (g_DynamicObjects[objectIndex].anmptr >= 2) {
        kwcheck_slip_kani_player(objectIndex);
    }
}

void kwchart_kani(void) {
    s32 objectIndex;
    s32 var_s1;

    for (var_s1 = 0; var_s1 < NUM_CRABS; var_s1++) {
        objectIndex = objallocptr[var_s1];
        if (g_DynamicObjects[objectIndex].anmptr != 0) {
            kwanm_kani(objectIndex, var_s1);
            kwcheck_visible_area_camera(objectIndex, 500.0f);
            kwmv_kani(objectIndex);
            kwhitcheck_kani(objectIndex);
        }
    }
}

void kwanm_goalflag_init(s32 objectIndex, s32 arg1) {
    g_DynamicObjects[objectIndex].shape = (Gfx*)d_course_yoshi_valley_unk5;
    g_DynamicObjects[objectIndex].vtxaddr = (Vtx*)d_course_yoshi_valley_unk4;
    g_DynamicObjects[objectIndex].scale = 0.027f;
    kwanm_next(objectIndex);
    kwset_init3D_spos(objectIndex, D_800E5DF4[arg1 * 4 + 0] * xOrientation, D_800E5DF4[arg1 * 4 + 1], D_800E5DF4[arg1 * 4 + 2]);
    kwset_init3D_sppos(objectIndex, 0.0f, 0.0f, 0.0f);
    kwset_init3D_ang(objectIndex, 0U, D_800E5DF4[arg1 * 4 + 3], 0U);
}

void kwanm_goalflag(s32 objectIndex, s32 arg1) {
    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 1:
            kwanm_goalflag_init(objectIndex, arg1);
            break;
        case 0:
        default:
            break;
    }
}

void kwmv_goalflag(s32 objectIndex) {
    kwset_splinepos3D(objectIndex);
}

void kwchart_goalflag(void) {
    s32 objectIndex;
    s32 var_s1;

    for (var_s1 = 0; var_s1 < NUM_YV_FLAG_POLES; var_s1++) {
        objectIndex = objallocptr[var_s1];
        if (g_DynamicObjects[objectIndex].anmptr != 0) {
            kwanm_goalflag(objectIndex, var_s1);
            kwmv_goalflag(objectIndex);
        }
    }
}

void kwanm_harinezumi_init(s32 objectIndex, s32 arg1) {
    AnmObject* object;

    kwanm_init_ci8(objectIndex, d_course_yoshi_valley_hedgehog_tlut, d_course_yoshi_valley_hedgehog, 0x40U,
                        (u16) 0x00000040);
    object = &g_DynamicObjects[objectIndex];
    object->texaddr = d_course_yoshi_valley_hedgehog_tlut;
    object->indexaddr = d_course_yoshi_valley_hedgehog;
    object->vtxaddr = common_vtx_hedgehog;
    object->scale = 0.2f;
    object->anmnumbak = 0;
    kwanm_next(objectIndex);
    kwset_init3D_sppos(objectIndex, 0.0f, 0.0f, 0.0f);
    kwset_init3D_texang(objectIndex, 0U, 0U, 0x8000U);
    object->vel = ((arg1 % 6) * 0.1) + 0.5;
    kwmv_start(objectIndex);
    kwanm_setflag(objectIndex, 0x04000600);
    object->r = 2;
}

void kwmv_harinezumi(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].mvptr) {
        case 0:
            break;
        case 1:
            if (kwmv_walkxz_2pt(objectIndex, g_DynamicObjects[objectIndex].pos_start[0], g_DynamicObjects[objectIndex].ix,
                              g_DynamicObjects[objectIndex].pos_start[2], g_DynamicObjects[objectIndex].iy) != 0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 2:
            kwmv_waitnext(objectIndex, 0x0000003C);
            break;
        case 3:
            if (kwmv_walkxz_2pt(objectIndex, g_DynamicObjects[objectIndex].ix, g_DynamicObjects[objectIndex].pos_start[0],
                              g_DynamicObjects[objectIndex].iy, g_DynamicObjects[objectIndex].pos_start[2]) != 0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 4:
            if (kwmv_wait(objectIndex, 0x0000003C) != 0) {
                kwmv_jump(objectIndex, 1);
            }
            break;
    }
    kwset_splinepos3D(objectIndex);
    if (kwanm_bitcheckflag(objectIndex, 0x00200000) != 0) {
        if (kwanm_bitcheckflag(objectIndex, 0x00400000) != 0) {
            kwanm_seton_groundbump(objectIndex);
        }
        g_DynamicObjects[objectIndex].pos[1] = g_DynamicObjects[objectIndex].ground + 6.0;
    }
}

void kwanm_harinezumi(s32 objectIndex, s32 arg1) {
    switch (g_DynamicObjects[objectIndex].anmptr) { /* irregular */
        case 0:
            break;
        case 1:
            kwanm_harinezumi_init(objectIndex, arg1);
            break;
        case 2:
            kwanm_swap(objectIndex, 0, 1, 4, -1);
            break;
    }
    if (g_DynamicObjects[objectIndex].anmnumbak == 0) {
        g_DynamicObjects[objectIndex].vtxaddr = common_vtx_hedgehog;
    } else {
        g_DynamicObjects[objectIndex].vtxaddr = D_0D006130;
    }
}

void kwhitcheck_harinezumi(s32 objectIndex) {
    if (g_DynamicObjects[objectIndex].anmptr >= 2) {
        kwcheck_slip_kani_player(objectIndex);
    }
}

void kwchart_harinezumi(void) {
    s32 temp_s1;
    s32 var_s0;

    for (var_s0 = 0; var_s0 < NUM_HEDGEHOGS; var_s0++) {
        temp_s1 = objallocptr2[var_s0];
        kwanm_harinezumi(temp_s1, var_s0);
        kwmv_harinezumi(temp_s1);
        kwhitcheck_harinezumi(temp_s1);
    }
    kwclear_anybodyflag(objallocptr2, 0x0000000F);
}

void kwinit_snowman_kakera(s32 objectIndex, Vec3f arg1, s32 arg2, s32 arg3) {
    AnmObject* object;

    KWAnmStart(objectIndex, 0);
    object = &g_DynamicObjects[objectIndex];
    object->indexaddr = d_course_frappe_snowland_snow;
    object->sindexaddr = d_course_frappe_snowland_snow;
    object->texaddr = d_course_frappe_snowland_snow_tlut;
    object->stexaddr = d_course_frappe_snowland_snow_tlut;
    object->scale = MakeRandomLimmit(0x0064U);
    object->scale = (object->scale * 0.001) + 0.05;
    object->velocity[1] = MakeRandomLimmit(0x0014U);
    object->velocity[1] = (object->velocity[1] * 0.5) + 2.6;
    object->vel = MakeRandomLimmit(0x000AU);
    object->vel = (object->vel * 0.1) + 4.5;
    object->ang[1] = (arg2 << 0x10) / arg3;
    object->pos_start[0] = arg1[0];
    object->pos_start[1] = arg1[1];
    object->pos_start[2] = arg1[2];
    object->alpha = MakeRandomLimmit(0x4000U) + 0x1000;
}

void kwget_snowman_kakera(Vec3f arg0) {
    s32 objectIndex;
    s32 i;

    for (i = 0; i < D_8018D3BC; i++) {
        objectIndex = kwalloc_effectram(&EffectAllocArray2[0], &gNextFreeObjectParticle2, gObjectParticle2_SIZE);
        if (objectIndex == NULL_OBJECT_ID) {
            break;
        }
        kwinit_snowman_kakera(objectIndex, arg0, i, D_8018D3BC);
    }
}

void kwanm_snowbomb(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 0:
            break;
        case 1:
            if (kwmv_nanamenage(objectIndex, g_DynamicObjects[objectIndex].velocity[1], 0.74f,
                              g_DynamicObjects[objectIndex].vel, g_DynamicObjects[objectIndex].ang[1],
                              0x00000064) != 0) {
                kwanm_next(objectIndex);
            }
            break;
        case 2:
            kwmv_finish(objectIndex);
            kwanm_finish(objectIndex);
            break;
    }
    kwset_splinepos3D(objectIndex);
    g_DynamicObjects[objectIndex].texang[2] += g_DynamicObjects[objectIndex].alpha;
}

void kwanm_snowman_face_init(s32 objectIndex) {
    AnmObject* object;

    kwanm_init_ci8(objectIndex, d_course_frappe_snowland_snowman_tlut, d_course_frappe_snowland_snowman_head,
                        0x40U, (u16) 0x00000040);
    object = &g_DynamicObjects[objectIndex];
    object->vtxaddr = D_0D0061B0;
    object->scale = 0.1f;
    object->anmnumbak = 0;
    kwanm_next(objectIndex);
    kwset_init3D_sppos(objectIndex, 0.0f, 0.0f, 0.0f);
    object->texang[0] = 0;
    object->texang[1] = 0;
    object->texang[2] = 0x8000;
    object->alpha = MakeRandomLimmit(0x2000U) - 0x1000;
    kwmv_start(objectIndex);
    object->vel = 1.5f;
    kwanm_setflag(objectIndex, 0x00000200);
}

void kwmv_snowman_face(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].mvptr) {
        case 1:
            KWAnmNext(objectIndex);
            break;
        case 2:
            kwmv_waitnext(objectIndex, 0x00000014);
            break;
        case 3:
            kwmv_jump(objectIndex, 1);
            break;
        case 10:
            kwmv_nageage(objectIndex, 10.0f, 0.5f, 0x0000000A);
            break;
        case 11:
            kwmv_nageage_ground(objectIndex, 0.0f, 0.2f, -7.0f);
            break;
        case 20:
            if (kwadd_fval(&g_DynamicObjects[objectIndex].pos_spline[1], 0.0f, 0.2f) != 0) {
                kwsetsval_trig(objectIndex, 0);
                kwmv_jump(objectIndex, 1);
            }
            break;
        case 0:
        default:
            break;
    }
    kwset_splinepos3D(objectIndex);
    kwsetsval_oufuku_f_rnds(objectIndex, &g_DynamicObjects[objectIndex].alpha, -0x00001000, 0x00001000, 0x00000400, 1, -1);
    g_DynamicObjects[objectIndex].texang[2] = g_DynamicObjects[objectIndex].alpha + 0x8000;
}

void kwanm_snowman_face(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 0:
            break;
        case 1:
            kwanm_snowman_face_init(objectIndex);
            break;
    }
    if (g_DynamicObjects[objectIndex].anmptr >= 2) {
        kwanm_texaddr_ci8_index(objectIndex);
    }
    kwmv_snowman_face(objectIndex);
}

void kwanm_snowman_body_init(s32 objectIndex) {
    kwanm_init_ci8(objectIndex, d_course_frappe_snowland_snowman_tlut, d_course_frappe_snowland_snowman_body,
                        0x40U, (u16) 0x00000040);
    g_DynamicObjects[objectIndex].vtxaddr = common_vtx_hedgehog;
    g_DynamicObjects[objectIndex].scale = 0.1f;
    g_DynamicObjects[objectIndex].anmnumbak = 0;
    kwanm_next(objectIndex);
    kwset_init3D_sppos(objectIndex, 0.0f, 0.0f, 0.0f);
    g_DynamicObjects[objectIndex].texang[0] = 0;
    g_DynamicObjects[objectIndex].texang[1] = 0;
    g_DynamicObjects[objectIndex].texang[2] = 0x8000;
    g_DynamicObjects[objectIndex].r = 2;
    g_DynamicObjects[objectIndex].vel = 1.5f;
    kwanm_setflag(objectIndex, 0x04000210);
}

void kwmv_snowman_body(s32 objectIndex) {
    kwset_splinepos3D(objectIndex);
}

void kwanm_snowman_body(s32 objectIndex) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    switch (object->anmptr) {
        case 0:
            break;
        case 1:
            kwanm_snowman_body_init(objectIndex);
            break;
        case 2:
            kwanm_wait(objectIndex, 0x00000096);
            break;
        case 10:
            if (kwanm_wait(objectIndex, 0x0000012C) != 0) {
                kwanm_setevflag(objectIndex, 2);
            }
            break;
        case 11:
            if (kwanm_wait(objectIndex, 0x0000000A) != 0) {
                kwanm_setflag(objectIndex, 0x00000010);
                object->scale = 0.001f;
            }
            break;
        case 12:
            if (kwsetfval_forward(objectIndex, &object->scale, 0.001f, 0.1f, 0.0025f, 0, 0) != 0) {
                kwanm_next(objectIndex);
            }
            break;
        case 13:
            kwanm_jump(objectIndex, 2);
            kwanm_clrflag(objectIndex, 0x00001000);
            break;
    }
    if (object->anmptr >= 2) {
        kwanm_texaddr_ci8_index(objectIndex);
    }
    kwmv_snowman_body(objectIndex);
}

void kwchart_snowman(void) {
    s32 var_s0;
    s32 var_s3;
    s32 var_s4;
    s32 objectIndex;
    AnmObject* object;

    for (var_s0 = 0; var_s0 < gObjectParticle2_SIZE; var_s0++) {
        objectIndex = EffectAllocArray2[var_s0];

        if (objectIndex == DELETED_OBJECT_ID) {
            continue;
        }

        if (g_DynamicObjects[objectIndex].anmptr == 0) {
            continue;
        }
        kwanm_snowbomb(objectIndex);
        if (g_DynamicObjects[objectIndex].anmptr != 0) {
            continue;
        }
        kwdealloc_effectram(&EffectAllocArray2[var_s0]);
        if (var_s0) {} // ??
    }

    for (var_s0 = 0; var_s0 < NUM_SNOWMEN; var_s0++) {
        var_s4 = objallocptr[var_s0];
        var_s3 = objallocptr2[var_s0];
        kwanm_snowman_face(var_s3);
        kwanm_snowman_body(var_s4);
        if (kwanm_bitoffcheckflag(var_s4, 0x00001000) != 0) {
            object = &g_DynamicObjects[var_s4];
            if ((kwcheck_mapareain(object->chartptr - 1, object->chartptr + 1) != 0) && (kwcheck_bomb_player_2D(var_s4) != 0)) {
                kwanm_setflag(var_s4, 0x00001000);
                kwanm_clrflag(var_s4, 0x00000010);
                kwanm_jump(var_s4, 0x0000000A);
                kwmv_jump(var_s3, 0x0000000A);
                kwget_snowman_kakera(object->pos);
            }
        } else if (kwanm_bitcheckevflag(var_s4, 2) != 0) {
            kwanm_clrevflag(var_s4, 2);
            kwmv_jump(var_s3, 0x00000014);
        }
    }
}

void kwanm_icebomb(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 0:
            break;
        case 1:
            if (kwmv_nanamenage(objectIndex, g_DynamicObjects[objectIndex].velocity[1], 0.12f,
                              g_DynamicObjects[objectIndex].vel, g_DynamicObjects[objectIndex].ang[1],
                              0x00000064) != 0) {
                kwanm_next(objectIndex);
            }
            kwset_splinepos3D(objectIndex);
            break;
        case 2:
            kwmv_finish(objectIndex);
            kwanm_finish(objectIndex);
            break;
    }
}

void kwset_icebomb(s32 objectIndex, s32 arg1, s32 playerId) {
    AnmObject* object;
    Player* sp20;

    object = &g_DynamicObjects[objectIndex];
    sp20 = &gPlayerOne[playerId];
    object->stack[7] = playerId;
    KWAnmStart(objectIndex, 0);
    object->texaddr = d_course_sherbet_land_ice;
    object->stexaddr = d_course_sherbet_land_ice;
    object->scale = ((f32) MakeRandomLimmit(0x01F4U) * 0.0002) + 0.04;
    object->velocity[1] = ((f32) MakeRandomLimmit(0x0032U) * 0.05) + 1.0;
    object->vel = ((f32) MakeRandomLimmit(0x000AU) * 0.1) + 1.0;
    object->ang[1] = D_801657A2 * arg1;
    object->pos_start[0] = (sp20->position[0] + MakeRandomLimmit(0x0014U)) - 10.0f;
    object->pos_start[1] = (sp20->position[1] - 10.0) + MakeRandomLimmit(0x000AU);
    object->pos_start[2] = (sp20->position[2] + MakeRandomLimmit(0x0014U)) - 10.0f;
}

void kwget_icekakera(UNUSED s32 arg0, s32 playerId) {
    s32 objectIndex;
    s32 var_s0;

    for (var_s0 = 0; var_s0 < D_8018D3C0; var_s0++) {
        objectIndex = kwalloc_effectram(EffectAllocArray2, &gNextFreeObjectParticle2, gObjectParticle2_SIZE);
        if (objectIndex == NULL_OBJECT_ID) {
            break;
        }
        kwset_icebomb(objectIndex, var_s0, playerId);
    }
}

void kwchart_iceblock(void) {
    s32 objectIndex;
    s32 playerId;

    D_80165834[0] += 0x200;
    D_80165834[1] += 0x400;
    kwcalc_lightvector();
    D_8016582C[0] += 0x2000;
    D_8016582C[1] += 0x1000;
    D_8016582C[2] += 0x1800;
    for (playerId = 0; playerId < g_playerCount; playerId++) {
        objectIndex = jugemuallocptr[playerId];
        if (kwanm_bitcheckevflag(objectIndex, 0x00000020) != 0) {
            kwanm_clrevflag(objectIndex, 0x00000020);
            kwget_icekakera(objectIndex, playerId);
        }
    }
    for (playerId = 0; playerId < gObjectParticle2_SIZE; playerId++) {
        objectIndex = EffectAllocArray2[playerId];
        if (objectIndex != DELETED_OBJECT_ID) {
            if (g_DynamicObjects[objectIndex].anmptr != 0) {
                kwanm_icebomb(objectIndex);
                if (g_DynamicObjects[objectIndex].anmptr == 0) {
                    kwdealloc_effectram(&EffectAllocArray2[playerId]);
                }
            }
        }
    }
}

void kwanm_kingping_init(s32 objectIndex, UNUSED s32 arg1) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    object->texsizex = 0;
    object->shape = (Gfx*) d_course_sherbet_land_unk_data1;
    object->vtxaddr = (Vtx*) d_course_sherbet_land_unk_data11;
    object->scale = 0.2f;
    object->r = 0x000C;
    object->ix = 1;
    kwset_init3D_spos(objectIndex, xOrientation * -383.0, 2.0f, -690.0f);
    kwset_init3D_ang(objectIndex, 0U, 0U, 0U);
    object->spptr = 1;
    kwmv_start_strategy(objectIndex);
    object->sptbtopaddr = D_800E672C[0];
    kwanm_setflag(objectIndex, 0x04000800);
    object->output = check_skelanime_end(d_course_sherbet_land_unk_data11, 0);
    kwanm_next(objectIndex);
}

void kwanm_kingping(s32 objectIndex, s32 arg1) {
    switch (g_DynamicObjects[objectIndex].anmptr) { /* irregular */
        case 0:
            break;
        case 1:
            if (g_playerCount == 1) {
                kwanm_kingping_init(objectIndex, arg1);
            }
            break;
        case 2:
            kwanm_forward(objectIndex, 0, g_DynamicObjects[objectIndex].output, 1, 0, -1);
            break;
    }
}

void kwanm_ping_init(s32 objectIndex, s32 arg1) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    object->texsizex = 0;
    object->shape = (Gfx*) d_course_sherbet_land_unk_data1;
    object->vtxaddr = (Vtx*) d_course_sherbet_land_unk_data11;
    object->r = 4;
    object->ix = 2;
    object->timer = MakeRandomLimmit(0x012CU);
    kwanm_setflag(objectIndex, 0x04000220);
    if ((arg1 > 0) && (arg1 < 9)) {
        if ((arg1 == 1) || (arg1 == 2)) {
            kwset_init3D_spos(objectIndex, xOrientation * -2960.0, -80.0f, 1521.0f);
            object->destangy = 0x0150;
            object->pos_ende[1] = 100.0f;
        } else if ((arg1 == 3) || (arg1 == 4)) {
            kwset_init3D_spos(objectIndex, xOrientation * -2490.0, -80.0f, 1612.0f);
            object->destangy = 0x0100;
            object->pos_ende[1] = 80.0f;
        } else if ((arg1 == 5) || (arg1 == 6)) {
            kwset_init3D_spos(objectIndex, xOrientation * -2098.0, -80.0f, 1624.0f);
            object->destangy = 0xFF00;
            object->pos_ende[1] = 80.0f;
        } else if ((arg1 == 7) || (arg1 == 8)) {
            kwset_init3D_spos(objectIndex, xOrientation * -2080.0, -80.0f, 1171.0f);
            object->destangy = 0x0150;
            object->pos_ende[1] = 80.0f;
        }
        object->addangy = (arg1 << 0xF) & 0xFFFF;
        object->ground = -80.0f;
        object->scale = 0.08f;
        object->spptr = 2;
        kwanm_setevflag(objectIndex, 8);
    } else if ((arg1 > 8) && (arg1 < 15)) {
        switch (arg1) { /* irregular */
            case 9:
                if (gGamestate != 9) {
                    kwset_init3D_spos(objectIndex, xOrientation * 146.0, 0.0f, -380.0f);
                } else {
                    kwset_init3D_spos(objectIndex, xOrientation * 380.0, 0.0f, -535.0f);
                    object->scale = 0.15f;
                }
                object->destangy = 0x9000;
                if (g_ScreenFlip != 0) {
                    object->destangy -= 0x4000;
                }
                object->spptr = 3;
                break;
            case 10:
                kwset_init3D_spos(objectIndex, xOrientation * 380.0, 0.0f, -766.0f);
                object->destangy = 0x5000;
                if (g_ScreenFlip != 0) {
                    object->destangy += 0x8000;
                }
                object->spptr = 4;
                break;
            case 11:
                kwset_init3D_spos(objectIndex, xOrientation * -2300.0, 0.0f, -210.0f);
                object->destangy = 0xC000;
                object->spptr = 6;
                if (g_ScreenFlip != 0) {
                    object->destangy += 0x8000;
                }
                break;
            case 12:
                kwset_init3D_spos(objectIndex, xOrientation * -2500.0, 0.0f, -250.0f);
                object->destangy = 0x4000;
                object->spptr = 6;
                if (g_ScreenFlip != 0) {
                    object->destangy += 0x8000;
                }
                break;
            case 13:
                kwset_init3D_spos(objectIndex, xOrientation * -535.0, 0.0f, 875.0f);
                object->destangy = 0x8000;
                object->spptr = 6;
                if (g_ScreenFlip != 0) {
                    object->destangy -= 0x4000;
                }
                break;
            case 14:
                kwset_init3D_spos(objectIndex, xOrientation * -250.0, 0.0f, 953.0f);
                object->destangy = 0x9000;
                object->spptr = 6;
                if (g_ScreenFlip != 0) {
                    object->destangy -= 0x4000;
                }
                break;
            default:
                break;
        }
        kwset_init3D_ang(objectIndex, 0U, object->destangy + 0x8000, 0U);
        object->ground = 5.0f;
        object->scale = 0.04f;
        kwanm_setevflag(objectIndex, 0x00000014);
    }
    kwmv_start_strategy(objectIndex);
    object->vel = 0.0f;
    object->output = check_skelanime_end(d_course_sherbet_land_unk_data11, 0);
    kwanm_next(objectIndex);
}

void kwanm_ping(s32 objectIndex, s32 arg1) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    switch (object->anmptr) {
        case 0:
            break;
        case 1:
            kwanm_ping_init(objectIndex, arg1);
            break;
        case 2:
            kwanm_forward(objectIndex, 0, (s32) object->output, (s32) object->ix, 0, -1);
            if (kwanm_bitoffcheckevflag(objectIndex, 0x00000020) != 0) {
                if (object->stack[6] == 0) {
                    object->stack[6] = MakeRandomLimmit(0x005AU) + 0x5A;
                    kwanm_setevflag(objectIndex, 0x00000080);
                } else {
                    object->stack[6]--;
                }
            }
            break;
        case 3:
            kwanm_forward(objectIndex, 0, object->output, 1, 0, 0);
            break;
        case 4:
            kwanm_clrevflag(objectIndex, 2);
            kwanm_next(objectIndex);
            break;
    }
    if (kwanm_bitcheckevflag(objectIndex, 0x00000020) != 0) {
        if (object->stack[6] == 0) {
            kwanm_setevflag(objectIndex, 0x00000080);
            object->stack[6] = 0x0010;
        } else {
            object->stack[6]--;
        }
    }
    if (kwanm_bitcheckevflag(objectIndex, 0x00000080) != 0) {
        kwanm_clrevflag(objectIndex, 0x00000080);
        if (kwanm_bitcheckevflag(objectIndex, 0x00000010) != 0) {
            NAEnmTrgStart(object->pos, object->velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x70, 0x49));
        } else {
            NAEnmTrgStart(object->pos, object->velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x70, 0x17));
        }
    }
}

void kwmv_ping_suberu(s32 objectIndex, s32 arg1) {
    f32 sp24;

    switch (g_DynamicObjects[objectIndex].mvptr) {
        case 0:
            break;
        case 1:
            g_DynamicObjects[objectIndex].ang[1] =
                kwchase_angle(g_DynamicObjects[objectIndex].ang[1], g_DynamicObjects[objectIndex].destangy);
            if (g_DynamicObjects[objectIndex].ang[1] == g_DynamicObjects[objectIndex].destangy) {
                g_DynamicObjects[objectIndex].ix = 4;
                g_DynamicObjects[objectIndex].vel = 0.4f;
                KWAnmNext(objectIndex);
            }
            break;
        case 2:
            KWChaseFVal(&g_DynamicObjects[objectIndex].vel, 0.8f, 0.02f);
            if (kwmv_wait(objectIndex, 0x0000000F) != 0) {
                kwanm_setevflag(objectIndex, 1);
                kwanm_setevflag(objectIndex, 2);
                g_DynamicObjects[objectIndex].ix = 1;
                g_DynamicObjects[objectIndex].texsizex = 1;
                g_DynamicObjects[objectIndex].anmnumbak = 0;
                g_DynamicObjects[objectIndex].output =
                    check_skelanime_end(d_course_sherbet_land_unk_data11, g_DynamicObjects[objectIndex].texsizex);
                kwanm_jump(objectIndex, 3);
                KWAnmNext(objectIndex);
                if (kwanm_bitoffcheckevflag(objectIndex, 0x00000020) != 0) {
                    kwanm_setevflag(objectIndex, 0x00000080);
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
            KWChaseFVal(&g_DynamicObjects[objectIndex].vel, sp24, 0.15f);
            if ((kwanm_bitoffcheckevflag(objectIndex, 2) != 0) && (sp24 == g_DynamicObjects[objectIndex].vel)) {
                KWAnmNext(objectIndex);
            }
            break;
        case 4:
            if (kwmv_wait(objectIndex, 0x0000001E) != 0) {
                kwanm_clrevflag(objectIndex, 1);
                KWAnmNext(objectIndex);
            }
            break;
        case 5:
            KWChaseFVal(&g_DynamicObjects[objectIndex].vel, 0.4f, 0.2f);
            if (kwmv_wait(objectIndex, 0x0000000A) != 0) {
                kwanm_setevflag(objectIndex, 2);
                g_DynamicObjects[objectIndex].texsizex = 2;
                g_DynamicObjects[objectIndex].anmnumbak = 0;
                g_DynamicObjects[objectIndex].output =
                    check_skelanime_end(d_course_sherbet_land_unk_data11, g_DynamicObjects[objectIndex].texsizex);
                kwanm_jump(objectIndex, 3);
                KWAnmNext(objectIndex);
            }
            break;
        case 6:
            if (kwanm_bitoffcheckevflag(objectIndex, 2) != 0) {
                g_DynamicObjects[objectIndex].texsizex = 0;
                g_DynamicObjects[objectIndex].anmnumbak = 0;
                g_DynamicObjects[objectIndex].output =
                    check_skelanime_end(d_course_sherbet_land_unk_data11, g_DynamicObjects[objectIndex].texsizex);
                g_DynamicObjects[objectIndex].destangy += 0x8000;
                kwanm_jump(objectIndex, 2);
                kwmv_jump(objectIndex, 1);
            }
            break;
    }
    kwmv_velangytoposxz(objectIndex);
    kwset_splinepos3D(objectIndex);
}

void func_80085024(void) {
}

void kwmv_ping_mawaru(s32 objectIndex, UNUSED s32 arg1) {
    kwmv_circle(objectIndex, g_DynamicObjects[objectIndex].pos_ende[1], g_DynamicObjects[objectIndex].destangy);
    kwset_splinepos3D(objectIndex);
    kwchase_velxztoangy(objectIndex);
}

void kwmv_ping_spline(s32 objectIndex) {
    kwsp_strategy3D_loop(objectIndex);
    kwset_splinepos3D(objectIndex);
    kwchase_velxztoangy(objectIndex);
}

void kwmv_ping(s32 objectIndex, s32 arg1) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    switch (object->spptr) {
        case 1:
            kwmv_ping_spline(objectIndex);
            break;
        case 2:
            kwmv_ping_mawaru(objectIndex, arg1);
            break;
        case 3:
            kwmv_ping_suberu(objectIndex, 0);
            break;
        case 4:
            kwmv_ping_suberu(objectIndex, 1);
            break;
        case 5:
            kwmv_ping_suberu(objectIndex, 2);
            break;
        case 6:
            kwmv_ping_suberu(objectIndex, 3);
            break;
    }
    if (kwanm_bitcheckevflag(objectIndex, 0x00000020) != 0) {
        if (kwanm_bitcheckevflag(objectIndex, 0x00000040) != 0) {
            kwanm_clrevflag(objectIndex, 0x00000040);
            object->stack[6] = 0;
            object->stack[7] = 0x0096;
        }
        if (object->stack[7] == 0) {
            kwanm_clrevflag(objectIndex, 0x00000020);
        } else {
            object->stack[7]--;
            object->texang[0] = object->ang[0];
            object->texang[1] += 0x2000;
            object->texang[2] = object->ang[2];
        }
    } else {
        object->texang[0] = object->ang[0];
        object->texang[1] = object->ang[1];
        object->texang[2] = object->ang[2];
    }
}

void kwchart_ping(void) {
    UNUSED s32 var_s2;
    s32 objectIndex;
    s32 var_s1;

    for (var_s1 = 0; var_s1 < NUM_PENGUINS; var_s1++) {
        objectIndex = objallocptr[var_s1];
        if (g_DynamicObjects[objectIndex].anmptr != 0) {
            if (var_s1 == 0) {
                kwanm_kingping(objectIndex, var_s1);
            } else {
                kwanm_ping(objectIndex, var_s1);
            }
            kwmv_ping(objectIndex, var_s1);
        }
        if (kwanm_bitcheckevflag(objectIndex, 1) != 0) {
            kwcheck_reboundsnd_player_2D(objectIndex, 1.75f, 1.5f, 0x1900A046U);
        } else if (kwanm_bitcheckevflag(objectIndex, 8) != 0) {
            kwcheck_reboundsnd_player_2D(objectIndex, 1.3f, 1.0f, 0x1900A046U);
        } else {
            kwcheck_reboundsnd_player_2D(objectIndex, 1.5f, 1.25f, 0x1900A046U);
        }
        if ((kwanm_bitcheckflag(objectIndex, 0x02000000) != 0) &&
            (kwanm_bitoffcheckevflag(objectIndex, 0x00000020) != 0)) {
            kwanm_setevflag(objectIndex, 0x00000060);
            kwanm_clrflag(objectIndex, 0x02000000);
        }
    }
}

void kwanm_balloon_init(s32 objectIndex) {
    g_DynamicObjects[objectIndex].scale = 1.0f;
    g_DynamicObjects[objectIndex].shape = d_course_luigi_raceway_dl_F960;
    if (gGamestate != 9) {
        kwset_init3D_spos(objectIndex, xOrientation * -176.0, 0.0f, -2323.0f);
        kwset_init3D_sppos(objectIndex, 0.0f, 300.0f, 0.0f);
    } else {
        kwset_init3D_spos(objectIndex, xOrientation * -1250.0, 0.0f, 1110.0f);
        kwset_init3D_sppos(objectIndex, 0.0f, 300.0f, 0.0f);
    }
    kwset_init3D_pos(objectIndex);
    kwanm_setkage_params(objectIndex);
    kwmv_start_strategy(objectIndex);
    g_DynamicObjects[objectIndex].velocity[1] = -2.0f;
    initial_special_itembox(0.0f, 0.0f, 0.0f);
    kwanm_next(objectIndex);
}

void kwmv_balloon(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].mvptr) {
        case 1:
            if (g_DynamicObjects[objectIndex].pos_spline[1] <= 18.0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 2:
            KWChaseFVal(&g_DynamicObjects[objectIndex].velocity[1], 0.0f, 0.05f);
            if (g_DynamicObjects[objectIndex].velocity[1] == 0.0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 3:
            kwmv_waitnext(objectIndex, 1);
            break;
        case 4:
            KWChaseFVal(&g_DynamicObjects[objectIndex].velocity[1], 1.0f, 0.05f);
            if (g_DynamicObjects[objectIndex].velocity[1] == 1.0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 5:
            kwmv_waitnext(objectIndex, 0x0000005A);
            break;
        case 6:
            KWChaseFVal(&g_DynamicObjects[objectIndex].velocity[1], 0.0f, 0.05f);
            if (g_DynamicObjects[objectIndex].velocity[1] == 0.0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 7:
            KWChaseFVal(&g_DynamicObjects[objectIndex].velocity[1], -1.0f, 0.05f);
            if (g_DynamicObjects[objectIndex].velocity[1] == -1.0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 8:
            kwmv_waitnext(objectIndex, 0x0000005A);
            break;
        case 9:
            KWChaseFVal(&g_DynamicObjects[objectIndex].velocity[1], 0.0f, 0.05f);
            if (kwmv_wait(objectIndex, 0x0000005A) != 0) {
                kwmv_jump(objectIndex, 3);
            }
            break;
    }
    kwmv_addvelytopos(objectIndex);
    g_DynamicObjects[objectIndex].ang[1] += 0x100;
}

void kwanm_balloon(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].anmptr) { /* irregular */
        case 1:
            kwanm_balloon_init(objectIndex);
            break;
        case 0:
        case 2:
            break;
    }
}

void kwchart_balloon(void) {
    s32 objectIndex;

    objectIndex = objallocptr[0];
    if (g_DynamicObjects[objectIndex].anmptr != 0) {
        kwanm_balloon(objectIndex);
        kwmv_balloon(objectIndex);
        kwset_splinepos3D(objectIndex);
        if (g_DynamicObjects[objectIndex].anmptr >= 2) {
            gActorHotAirBalloonItemBox->position[0] = g_DynamicObjects[objectIndex].pos[0];
            gActorHotAirBalloonItemBox->position[1] = g_DynamicObjects[objectIndex].pos[1] - 10.0;
            gActorHotAirBalloonItemBox->position[2] = g_DynamicObjects[objectIndex].pos[2];
        }
    }
}

void kwanm_wanwan_init(s32 objectIndex, s32 arg1) {
    CenterPathStruct* temp_v0;
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    object->texsizex = 1;
    object->shape = (Gfx*) d_rainbow_road_unk4;
    object->vtxaddr = (Vtx*) d_rainbow_road_unk3;
    object->scale = 0.03f;
    object->r = 0x000A;
    kwanm_setflag(objectIndex, 0x04000200);
    object->stack[8] = (arg1 * 0x12C) + 0x1F4;
    kwset_init3D_spos(objectIndex, 0.0f, -15.0f, 0.0f);
    temp_v0 = &g_pathPointPointer[(u16) object->stack[8]];
    kwset_init3D_sppos(objectIndex, temp_v0->pointx, temp_v0->pointy, temp_v0->pointz);
    kwset_init3D_ang(objectIndex, 0U, 0U, 0U);
    object->vel = 4.0f;
    object->output = check_skelanime_end(d_rainbow_road_unk3, 0);
    kwanm_next(objectIndex);
}

void kwanm_wanwan(s32 objectIndex, s32 arg1) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    switch (object->anmptr) { /* irregular */
        case 0:
            break;
        case 1:
            kwanm_wanwan_init(objectIndex, arg1);
            break;
        case 2:
            kwanm_forward(objectIndex, 0, (s32) object->output, 1, 0, -1);
            break;
    }
    if (D_8018D40C == 0) {
        NAEnmTrgStart(object->pos, object->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x57));
    }
    kwsetfval_oufuku_f(objectIndex, &object->ground, -0.8f, 0.8f, 0.03f, 0, -1);
}

void kwchart_wanwan(void) {
    s32 objectIndex;
    s32 var_s4;
    AnmObject* object;

    for (var_s4 = 0; var_s4 < NUM_CHAIN_CHOMPS; var_s4++) {
        objectIndex = objallocptr2[var_s4];
        object = &g_DynamicObjects[objectIndex];
        if (object->anmptr != 0) {
            kwanm_wanwan(objectIndex, var_s4);
            kwcopy_fval(object->pos_ende, object->pos_spline);
            hanyou_gyaku_soukou_sub_BP(object->pos_spline, &object->stack[8], object->vel, object->ground, 0);
            object->ang[1] = CalcDirection(object->pos_ende, object->pos_spline);
            kwset_splinepos3D(objectIndex);
            kwcheck_bomb_player_3D(objectIndex, 30.0f);
        }
    }
}

void kwanm_neon_common_init(s32 objectIndex) {
    g_DynamicObjects[objectIndex].scale = 8.0f;
    kwset_init3D_sppos(objectIndex, 0.0f, 0.0f, 0.0f);
    kwset_init3D_texang(objectIndex, 0U, 0U, 0x8000U);
    kwanm_next(objectIndex);
}

void kwanm_neon_ki_init(s32 objectIndex) {
    kwset_init3D_spos(objectIndex, xOrientation * -1431.0, 827.0f, -2957.0f);
    kwanm_init_ci8(objectIndex, (u8*) d_course_rainbow_road_neon_mushroom_tlut_list,
                        d_course_rainbow_road_neon_mushroom, 0x40U, (u16) 0x00000040);
    kwanm_neon_common_init(objectIndex);
}

void kwanm_neon_ki(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 1:
            kwanm_neon_ki_init(objectIndex);
            break;
        case 2:
            kwanm_forward(objectIndex, 0, 4, 1, 0x0000000C, 5);
            break;
        case 3:
            kwanm_swap(objectIndex, 3, 4, 4, 0x0000000A);
            break;
        case 4:
            kwanm_wait(objectIndex, 0x00000014);
            break;
        case 5:
            kwanm_forward(objectIndex, 0, 4, 1, 0x0000000C, 5);
            break;
        case 6:
            kwanm_wait(objectIndex, 0x00000014);
            break;
        case 7:
            kwanm_swap(objectIndex, 3, 4, 0, 0x00000014);
            break;
        case 8:
            kwanm_jump(objectIndex, 2);
            break;
        case 0:
        default:
            break;
    }
}

void kwanm_neon_ma_init(s32 objectIndex) {
    kwset_init3D_spos(objectIndex, xOrientation * 799.0, 1193.0f, -5891.0f);
    kwanm_init_ci8(objectIndex, (u8*) d_course_rainbow_road_neon_mario_tlut_list, d_course_rainbow_road_neon_mario,
                        0x40U, (u16) 0x00000040);
    kwanm_neon_common_init(objectIndex);
}

void kwanm_neon_ma(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 1:
            kwanm_neon_ma_init(objectIndex);
            break;
        case 2:
            kwanm_forward(objectIndex, 0, 4, 1, 0x0000000C, 1);
            break;
        case 3:
            kwanm_swap(objectIndex, 3, 4, 0x0000000C, 1);
            break;
        case 4:
            kwanm_hidden(objectIndex, 0x0000000C);
            break;
        case 5:
            kwanm_jump(objectIndex, 2);
            break;
        case 0:
        default:
            break;
    }
}

void kwanm_neon_te_init(s32 objectIndex) {
    kwset_init3D_spos(objectIndex, xOrientation * -2013.0, 555.0f, 0.0f);
    kwanm_init_ci8(objectIndex, (u8*) d_course_rainbow_road_neon_boo_tlut_list, d_course_rainbow_road_neon_boo,
                        0x40U, (u16) 0x00000040);
    kwanm_neon_common_init(objectIndex);
}

void kwanm_neon_te(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 1:
            kwanm_neon_te_init(objectIndex);
            break;
        case 2:
            kwanm_forward(objectIndex, 0, 4, 1, 5, 1);
            break;
        case 3:
            kwanm_wait(objectIndex, 0x0000001E);
            break;
        case 4:
            kwanm_flash(objectIndex, 4, 0, 7);
            break;
        case 5:
            kwanm_wait(objectIndex, 0x0000001E);
            break;
        case 6:
            kwanm_reverse(objectIndex, 3, 0, 1, 5, 1);
            break;
        case 7:
            kwanm_hidden(objectIndex, 0x0000000F);
            break;
        case 8:
            kwanm_jump(objectIndex, 2);
            break;
        case 0:
        default:
            break;
    }
}

void kwanm_neon_fix_init(s32 objectIndex, s32 arg1) {
    kwset_init3D_spos(objectIndex, D_800E6734[arg1 * 3 + 0] * xOrientation, D_800E6734[arg1 * 3 + 1], D_800E6734[arg1 * 3 + 2]);
    kwanm_init_ci8(objectIndex, &d_course_rainbow_road_static_tluts[arg1 * 256],
                        &d_course_rainbow_road_static_textures[arg1], 64, 64);
    kwanm_neon_common_init(objectIndex);
}

void kwanm_neon_fix(s32 objectIndex, s32 arg1) {
    switch (g_DynamicObjects[objectIndex].anmptr) {
        case 1:
            kwanm_neon_fix_init(objectIndex, arg1);
            break;
        case 0:
            break;
    }
}

void kwanm_neon(s32 objectIndex, s32 id) {
    switch (id) { /* irregular */
        case 0:
            kwanm_neon_ki(objectIndex);
            break;
        case 1:
            kwanm_neon_ma(objectIndex);
            break;
        case 2:
            kwanm_neon_te(objectIndex);
            break;
    }
    if (id >= 3) {
        kwanm_neon_fix(objectIndex, id - 3);
    }
}

void kwchart_neon(void) {
    s32 objectIndex;
    s32 id;

    for (id = 0; id < NUM_NEON_SIGNS; id++) {
        objectIndex = objallocptr[id];
        if (g_DynamicObjects[objectIndex].anmptr != 0) {
            kwanm_neon(objectIndex, id);
            if (g_DynamicObjects[objectIndex].anmptr >= 2) {
                kwanm_texaddr_ci8_colpal(objectIndex);
                kwset_splinepos3D(objectIndex);
            }
        }
    }
}

void kwanm_dai_init(s32 objectIndex, s32 arg1) {
    switch (arg1) { /* irregular */
        case 0:
            g_DynamicObjects[objectIndex].shape = podium_dl3;
            g_DynamicObjects[objectIndex].timer = 0x00000038;
            break;
        case 1:
            g_DynamicObjects[objectIndex].shape = podium2_dl3;
            g_DynamicObjects[objectIndex].timer = 0x0000002B;
            break;
        case 2:
            g_DynamicObjects[objectIndex].shape = podium3_dl3;
            g_DynamicObjects[objectIndex].timer = 0x0000001E;
            break;
        default:
            break;
    }
    g_DynamicObjects[objectIndex].scale = 1.0f;
    kwset_init3D_spos(objectIndex, D_800E634C[0][0] - 1.5, D_800E634C[0][1], D_800E634C[0][2]);
    kwset_init3D_sppos(objectIndex, 0.0f, -10.0f, 0.0f);
    kwset_init3D_ang(objectIndex, 0U, 0xF8E4U, 0U);
    g_DynamicObjects[objectIndex].ival = 0;
    kwanm_next(objectIndex);
}

void kwmv_dai(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].mvptr) {
        case 0:
            break;
        case 1:
            g_DynamicObjects[objectIndex].velocity[1] = 0.75f;
            KWAnmNext(objectIndex);
            break;
        case 2:
            if (g_DynamicObjects[objectIndex].pos_spline[1] >= -2.0) {
                g_DynamicObjects[objectIndex].velocity[1] -= 0.1;
            }
            kwmv_addvelytopos(objectIndex);
            if (g_DynamicObjects[objectIndex].pos_spline[1] >= 0.0) {
                g_DynamicObjects[objectIndex].pos_spline[1] = 0.0f;
                g_DynamicObjects[objectIndex].velocity[1] = 0.0f;
                kwmv_finish(objectIndex);
            }
            break;
    }
    kwset_splinepos3D(objectIndex);
}

void kwanm_dai(s32 objectIndex, s32 arg1) {
    switch (g_DynamicObjects[objectIndex].anmptr) { /* irregular */
        case 1:
            kwanm_dai_init(objectIndex, arg1);
            break;
        case 2:
            if (kwanm_wait(objectIndex, g_DynamicObjects[objectIndex].timer) != 0) {
                result_kart_jump_start(arg1);
                kwmv_start(objectIndex);
                kwanm_next(objectIndex);
            }
            break;
        case 0:
            break;
        case 3:
            if (g_DynamicObjects[objectIndex].mvptr == 0) {
                g_DynamicObjects[objectIndex].ival = 1;
                kwanm_next(objectIndex);
            }
            break;
    }
}

void kwchart_dai(void) {
    s32 objectIndex;
    s32 var_s1;

    if ((oga_hyousyou_demo_trg != 0) && (resultstat.rank < 3)) {
        if (D_801658C6 == 0) {
            for (var_s1 = 0; var_s1 < 3; var_s1++) {
                objectIndex = objallocptr[var_s1];
                KWAnmStart(objectIndex, 0);
            }
            D_801658C6 = 1;
        }
    }
    for (var_s1 = 0; var_s1 != 3; var_s1++) {
        objectIndex = objallocptr[var_s1];
        if (g_DynamicObjects[objectIndex].anmptr != 0) {
            kwanm_dai(objectIndex, var_s1);
            kwmv_dai(objectIndex);
        }
    }
}

void kwanm_cup_init(s32 objectIndex) {
    if (g_raceClass < CC_150) {
        switch (resultstat.rank) { /* switch 1; irregular */
            case 0:                 /* switch 1 */
                g_DynamicObjects[objectIndex].shape = gold_trophy_dl10;
                break;
            case 1: /* switch 1 */
                g_DynamicObjects[objectIndex].shape = gold_trophy_dl12;
                break;
            case 2: /* switch 1 */
                g_DynamicObjects[objectIndex].shape = gold_trophy_dl14;
                break;
            default: /* switch 1 */
                break;
        }
    } else {
        switch (resultstat.rank) { /* irregular */
            case 0:
                g_DynamicObjects[objectIndex].shape = gold_trophy_dl11;
                break;
            case 1:
                g_DynamicObjects[objectIndex].shape = gold_trophy_dl13;
                break;
            case 2:
                g_DynamicObjects[objectIndex].shape = gold_trophy_dl15;
                break;
            default:
                break;
        }
    }
    g_DynamicObjects[objectIndex].scale = 0.005f;
    kwset_init3D_spos(objectIndex, g_DynamicObjects[objallocptr2[0]].pos[0],
                       g_DynamicObjects[objallocptr2[0]].pos[1] + 16.0, g_DynamicObjects[objallocptr2[0]].pos[2]);
    kwset_init3D_sppos(objectIndex, 0.0f, 0.0f, 0.0f);
    kwset_init3D_ang(objectIndex, 0U, 0U, 0U);
    g_DynamicObjects[objectIndex].stack[1] = 0x0200;
    kwanm_next(objectIndex);
    kwmv_start(objectIndex);
}

void kwmv_cup(s32 objectIndex) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    switch (object->mvptr) {
        case 0:
            break;
        case 1:
            KWAnmNext(objectIndex);
            break;
        case 2:
            KWChaseFVal(&object->scale, 0.025f, 0.001f);
            kwmv_nageage(objectIndex, 6.0f, 0.1f, 0x000000C8);
            if ((f64) object->velocity[1] <= 0.0) {
                kwmv_jump(objectIndex, 3);
            }
            break;
        case 3:
            kwmv_waitnext(objectIndex, 0x00000064);
            break;
        case 4:
            D_801658D6 = 1;
            object->velocity[1] = -0.4f;
            KWAnmNext(objectIndex);
            object->pos_start[1] = 90.0f;
            object->pos_spline[1] = 60.0f;
            switch (resultstat.rank) { /* switch 1; irregular */
                case 1:                 /* switch 1 */
                    object->pos_start[0] -= 3.0;
                    object->pos_start[2] += 15.0;
                    break;
                case 2: /* switch 1 */
                    object->pos_start[0] -= 2.0;
                    object->pos_start[2] -= 15.0;
                    break;
            }
            break;
        case 5:
            if ((f64) object->pos_spline[1] <= 8.0) {
                KWChaseFVal(&object->velocity[1], -0.1f, -0.01f);
            }
            kwmv_addvelytopos(objectIndex);
            if ((f64) object->pos_spline[1] <= 0.0) {
                KWAnmNext(objectIndex);
            }
            break;
        case 6:
            if (kwmv_waitnext(objectIndex, 0x00000041) != 0) {
                D_801658F4 = 1;
            }
            break;
        case 7:
            if (kwmv_waitnext(objectIndex, 0x00000064) != 0) {
                StartKawanoResult();
                kwmv_finish(objectIndex);
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
    kwset_splinepos3D(objectIndex);
}

void kwanm_cup(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].anmptr) { /* irregular */
        case 1:
            kwanm_cup_init(objectIndex);
            break;
        case 0:
        case 2:
            break;
    }
}

void kwget_cup_star(s32 objectIndex) {
    Vec3f sp24;

    sp24[0] = (g_DynamicObjects[objectIndex].pos[0] - 5.0f) + MakeRandomLimmit(0x000AU);
    sp24[2] = (g_DynamicObjects[objectIndex].pos[2] - 5.0f) + MakeRandomLimmit(0x000AU);
    if (D_801658F4 != 0) {
        sp24[1] = g_DynamicObjects[objectIndex].pos[1] + 14.0;
    } else {
        sp24[1] = g_DynamicObjects[objectIndex].pos[1] - 2.0;
    }
    KWGetStar(sp24, (s32) D_801658F4);
}

void kwchart_cup(void) {
    s32 temp_s2;
    s32 var_s0;

    if ((D_801658CE != 0) && (D_801658DC == 0)) {
        temp_s2 = objallocptr[3];
        KWAnmStart(temp_s2, 0);
        D_801658DC = 1;
    }
    temp_s2 = objallocptr[3];
    if (g_DynamicObjects[temp_s2].anmptr != 0) {
        kwanm_cup(temp_s2);
        kwmv_cup(temp_s2);
        if (D_801658F4 != 0) {
            if (KW2GFTimer == 0) {
                kwget_cup_star(temp_s2);
            }
        } else {
            for (var_s0 = 0; var_s0 < 2; var_s0++) {
                kwget_cup_star(temp_s2);
            }
        }
    }
}

#include <ultra64.h>
#include <macros.h>
#include <defines.h>
#include <mk64.h>
#include <course.h>

#include "camera.h"
#include "math_util_2.h"
#include <sounds.h>
#include "audio/external.h"
#include "audio/load.h"
#include "audio/data.h"
#include "audio/port_eu.h"
#include "code_800029B0.h"
#include "cpu_vehicles_camera_path.h"
#include "menu_items.h"
#include "seq_ids.h"

s8 D_8018EF10;
UnkStruct8018EF18 D_8018EF18[16];
// chained list
struct Unk_8018EFD8 D_8018EFD8[50];
u8 D_8018FB90;
u8 D_8018FB91;
Camera* gCopyCamera[4];
Vec3f gVelocityCamera[4];
Vec3f gCameraLastPos[4];
u8 D_8018FC08;
s16 D_8018FC10[4][2];
struct Sound sSoundRequests[0x100];
struct SoundCharacteristics sSoundBanks[SOUND_BANK_COUNT][20];
u8 sSoundBankUsedListBack[SOUND_BANK_COUNT];
u8 sSoundBankFreeListFront[SOUND_BANK_COUNT];
u8 sNumSoundsInBank[SOUND_BANK_COUNT];
u8 D_80192AB8[SOUND_BANK_COUNT][8][8];
u8 D_80192C38;
ubool8 sSoundBankDisabled[SOUND_BANK_COUNT];
struct ChannelVolumeScaleFade D_80192C48[SOUND_BANK_COUNT];
struct_D_80192CA8_entry D_80192CA8[3][5];
u8 D_80192CC6[3];
u32 D_80192CD0[256];
struct_D_801930D0_entry D_801930D0[3];

u8 D_800E9DA0 = 0;
UNUSED s32 D_800E9DA4[] = { 0, 0, 0, 0 };
s32 D_800E9DB4[] = { 0, 0, 0, 0 };
f32 D_800E9DC4[] = { 1.0f, 1.0f, 1.0f, 1.0f };
f32 D_800E9DD4[] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 D_800E9DE4[] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 D_800E9DF4[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
s32 D_800E9E14[] = { 0, 0, 0, 0 };
s32 D_800E9E24[] = { 0, 0, 0, 0 };
s32 D_800E9E34[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
f32 D_800E9E54[] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 D_800E9E64[] = { 0.0f, 0.0f, 0.0f, 0.0f };
s32 g_PlayerSurfaceSoundID[] = { 0, 0, 0, 0 };
s32 D_800E9E84[] = { 0, 0, 0, 0 };
u32 D_800E9E94[] = { 0, 0, 0, 0 };
s32 D_800E9EA4[] = { 0, 0, 0, 0 };
f32 D_800E9EB4[] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 D_800E9EC4[] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 D_800E9ED4[] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 D_800E9EE4[] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 D_800E9EF4[] = { 1.0f, 1.0f, 1.0f, 1.0f };
f32 D_800E9F04[] = { 1.0f, 1.0f, 1.0f, 1.0f };
f32 D_800E9F14[] = { 1.0f, 1.0f, 1.0f, 1.0f };
u8 g_lightningFlagPlayer1[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
u8 D_800E9F2C[NUM_PLAYERS] = { 0, 0, 0, 0, 0, 0, 0, 0 };
f32 D_800E9F34[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
f32 D_800E9F54[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
u8 asm_DisableEngineSound[] = { 0, 0, 0, 0 };
u8 D_800E9F78[] = { 0, 0, 0, 0 };
struct Unk_800E9F7C D_800E9F7C[] = {
    { { 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f, 0, 3800.0f, 3.4f, 0.4f, -1.0f, 0.4f, 1100.0f, 630.0f, 3600.0f, 1.0f },
    { { 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f, 0, 3800.0f, 3.4f, 0.4f, -1.0f, 0.4f, 1100.0f, 630.0f, 3600.0f, 1.0f },
    { { 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f, 0, 3800.0f, 3.4f, 0.4f, -1.0f, 0.4f, 1100.0f, 630.0f, 3600.0f, 1.0f },
    { { 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f, 0, 3800.0f, 3.4f, 0.4f, -1.0f, 0.4f, 1100.0f, 630.0f, 3600.0f, 1.0f }
};
struct Unk_800EA06C D_800EA06C[] = { { { 0.0f, 1.0f, 1.0f }, 0 }, { { 0.0f, 1.0f, 1.0f }, 0 },
                                     { { 0.0f, 1.0f, 1.0f }, 0 }, { { 0.0f, 1.0f, 1.0f }, 0 },
                                     { { 0.0f, 1.0f, 1.0f }, 0 }, { { 0.0f, 1.0f, 1.0f }, 0 },
                                     { { 0.0f, 1.0f, 1.0f }, 0 }, { { 0.0f, 1.0f, 1.0f }, 0 } };
u8 D_800EA0EC[] = { 0, 0, 0, 0 };
u8 D_800EA0F0 = 0;
u8 D_800EA0F4 = 0;
UNUSED Vec3f D_800EA0F8 = { 0.0f, 0.0f, 1.0f };
u8 D_800EA104 = 0;
u8 D_800EA108 = 0;
u8 D_800EA10C[] = { 0, 0, 0, 0 };
f32 D_800EA110[] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 D_800EA120[] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 D_800EA130[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
f32 D_800EA150 = 1.4f;
u8 D_800EA154[] = { 2, 2, 88, 90, 3, 48, 88, 48 };
u16 D_800EA15C = 0;
u16 D_800EA160 = 0;
u8 D_800EA164 = 0;
s8 g_lightningFlag = 0;
s8 g_gamePausedFlag = 0;
u8 D_800EA170[] = { 0, 0, 0, 0 };
u16 D_800EA174 = 0;
f32 D_800EA178 = 1.0f;
f32 D_800EA17C = 0.85f;
u16 D_800EA180 = 0;
u16 D_800EA184 = 0;
u8 D_800EA188[][6] = { { 4, 2, 2, 2, 2, 1 }, { 6, 2, 2, 2, 2, 1 }, { 8, 2, 2, 0, 1, 1 }, { 8, 2, 2, 0, 1, 1 } };
u8 D_800EA1A0[][6] = { { 4, 1, 1, 2, 2, 1 }, { 3, 1, 1, 2, 2, 1 }, { 3, 1, 1, 0, 1, 1 }, { 3, 1, 1, 0, 1, 1 } };
u8 sSoundRequestCount = 0;
u8 sNumProcessedSoundRequests = 0;
u8 D_800EA1C0 = 0;
u16 D_800EA1C4 = 0;
Vec3f D_800EA1C8 = { 0.0f, 0.0f, 0.0f };
f32 D_800EA1D4 = 1.0f;
u32 external_unused_u32_0 = 0x00000000;
s8 D_800EA1DC = 0;
u32 external_unused_u32_1 = 0x00000000;
u8 D_800EA1E4 = 0;
u8 D_800EA1E8 = 0;
u8 D_800EA1EC = 0;
u8 D_800EA1F0[] = { 0, 1, 2, 3 };
u8 D_800EA1F4[] = { 0, 0, 0, 0 };

char external_unused_string00[] = "Error : Queue is not empty ( %x ) \n";
char external_unused_string01[] = "specchg error\n";
char external_unused_string02[] = "***** CAM MAX %d *****\n";
u8 D_800EA244 = 0;
char external_unused_string03[] = "entryout !!! %d\n";
char external_unused_string04[] = "AFTER GOAL VOICE FLAME %d\n";
char external_unused_string05[] = "*** Pause On ***\n";
char external_unused_string06[] = "*** Pause Off ***\n";
char external_unused_string07[] = "CALLED!! Na_ChangeSoundMode player %d\n";
char external_unused_string08[] = "CALLED!! Na_ChangeSoundMode spec   %d\n";
char external_unused_string09[] = "Interfaced Spec Change player %d\n";
char external_unused_string10[] = "Interfaced Spec Change spec   %d\n";
UNUSED u32 external_unused_u32s[] = { 0xff000000, 0xff000000, 0x00000000 };
char external_unused_string11[] = "FX MIX %d\n";
char external_unused_string12[] = "************** Seq Fadeout ***************\n";
char external_unused_string13[] = "SEQ FADE OUT TIME %d\n";
#ifdef VERSION_EU
char external_unused_string_eu_02[] = "************** SE Fadeout ***************\n";
char external_unused_string_eu_03[] = "SE FADE OUT TIME %d\n";
#endif

#ifdef VERSION_EU
#define AUDIO_LEFT_TIRE FRONT_LEFT
#define AUDIO_RIGHT_TIRE FRONT_RIGHT
#else
#define AUDIO_LEFT_TIRE BACK_LEFT
#define AUDIO_RIGHT_TIRE BACK_RIGHT
#endif

// Requires void in the argument list to match properly.
void func_800C13F0(void) {
}

void Naa_FxmixSet(OSMesg presetId) {
    OSMesg mesg;
    osRecvMesg(D_800EA3B4, &mesg, 0);
    osSendMesg(D_800EA3B0, presetId, 0);
    osRecvMesg(D_800EA3B4, &mesg, 1);
    if (mesg != presetId) {
        osRecvMesg(D_800EA3B4, &mesg, 1);
    }
}

f32 Naa_PanSet(u8 bank, u8 soundId) {
    f32 temp_f0;
    f32 var_f2;
    s32 var_v0;
    struct SoundCharacteristics* temp_v0;

    temp_v0 = &sSoundBanks[bank][soundId];
    if (temp_v0->soundBits & 0x400000) {
        return 1.0f;
    }
    temp_f0 = temp_v0->distance;
    if (temp_f0 > 2000.0f) {
        var_f2 = 0.0f;
    } else {
        switch (temp_v0->soundBits & 0x30000) { /* irregular */
            case 0x10000:
                var_v0 = 0x1F4;
                break;
            case 0x20000:
                var_v0 = 0x29A;
                break;
            case 0x30000:
                var_v0 = 0x3E8;
                break;
            default:
                var_v0 = 0x190;
                break;
        }
        if (temp_f0 < var_v0) {
            var_f2 = (((var_v0 - temp_f0) / var_v0) * 0.5) + 0.5;
        } else {
            var_f2 = (1.0 - ((temp_f0 - var_v0) / (2000.0f - var_v0))) * 0.5;
        }
        var_f2 *= var_f2;
    }
    return var_f2;
}

s8 Naa_TuneSet(u8 bank, u8 soundId, u8 channel) {
    s32 var_a0;
    s8 var_v0;
    s8 var_v1;

    var_v0 = 0;
    var_v1 = 0;
    if (!(sSoundBanks[bank][soundId].soundBits & 0x200000)) {
        if (sSoundBanks[bank][soundId].distance < 500.0f) {
            var_v0 = (sSoundBanks[bank][soundId].distance / 500.0f) * 10.0f;
        } else {
            var_v0 = 0x0A;
        }
    }
    if (IS_SEQUENCE_CHANNEL_VALID(SeqPlayer[2].channels[channel])) {
        var_v1 = SeqPlayer[2].channels[channel]->soundScriptIO[6];
    }
    if (var_v1 == -1) {
        var_v1 = 0;
    }
    var_a0 = *sSoundBanks[bank][soundId].unk18 + var_v0 + var_v1 + D_8018EF10;
    if (var_a0 >= 0x80) {
        var_a0 = 0x7F;
    }
    return var_a0;
}

s8 Naa_PlaySeParaSet(f32 arg0, f32 arg1, u8 cameraId) {
    f32 var_f0;
    f32 var_f14;
    f32 new_var;
    f32* new_var2;
    f32 var_f16;
    f32 var_f18;
    f32 var_f20;
    f32 var_f2;
    new_var2 = &var_f0;

    if (D_800EA1C0 == 0) {
        if (D_800EA0F4 != 0) {
            var_f2 = 10.0f;
            var_f14 = 20.0f;
            var_f16 = 10.0f;
            var_f18 = 2.5f;
        } else {
            var_f0 = 100.0f;
            var_f2 = *new_var2;
            var_f14 = 200.0f;
            var_f16 = 5.0f;
            var_f18 = 3.3333333f;
        }
        var_f20 = (arg0 < 0.0f) ? -arg0 : arg0;

        if (var_f2 < var_f20) {
            var_f20 = var_f2;
        }

        new_var = arg0;
        var_f0 = (arg1 < 0.0f) ? -arg1 : arg1;
        if (var_f2 < (var_f0 = *new_var2)) {
            var_f0 = var_f2;
        }
        if ((new_var == 0.0f) && (arg1 == 0.0f)) {
            var_f2 = 0.5f;
        } else if ((new_var >= 0.0f) && (var_f0 <= var_f20)) {
            var_f2 = 1.0f - ((var_f14 - var_f20) / (var_f16 * (var_f14 - var_f0)));
        } else if ((new_var < 0.0f) && (var_f0 <= var_f20)) {
            var_f2 = (var_f14 - var_f20) / (var_f16 * (var_f14 - var_f0));
        } else {
            var_f2 = (arg0 / (var_f18 * var_f0)) + 0.5f;
        }
        if (var_f2 > 1.0f) {
            var_f2 = 1.0f;
        }
        if (var_f2 < 0.0f) {
            var_f2 = 0.0f;
        }
        return (s8) (s32) ((var_f2 * 127.0f) + 0.5f);
    }
    return (cameraId & 1) * 0x7F;
}

f32 Naa_EntryCameraPosWork(u8 bank, u8 soundId) {
    f32 phi_f2;

    phi_f2 = 1.0f;
    if (sSoundBanks[bank][soundId].soundBits & 0x800000) {
        phi_f2 -= ((gAudioRandom & 0xF) / 192.0f);
    }
    return phi_f2;
}

void Naa_CalcCameraPos(u8 arg0, u8 arg1, u8 arg2) {
    f32 sp3C;
    s8 sp3B;
    f32 sp34;
    s8 sp33;
    UnkStruct8018EF18* temp_s0_2;
    struct SoundCharacteristics* temp_s0;

    sp3B = 0;
    sp33 = 0x40;
    sp3C = 1.0f;
    sp34 = 1.0f;
    switch (arg0) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
            temp_s0 = &sSoundBanks[arg0][arg1];
            temp_s0->distance = sqrtf(temp_s0->distance);
            sp3C = (Naa_PanSet(arg0, arg1) * *temp_s0->unk14) * D_80192C48[arg0].current;
            sp3B = Naa_TuneSet(arg0, arg1, arg2);
            sp34 = Naa_EntryCameraPosWork(arg0, arg1) * *temp_s0->unk10;
            sp33 = Naa_PlaySeParaSet(*temp_s0->unk00[0], *temp_s0->unk08, temp_s0->cameraId);
            break;
    }
    temp_s0_2 = &D_8018EF18[arg2];
    if (sp3C != temp_s0_2->unk0) {
        Nap_SetS8(((arg2 & 0xFF) << 8) | 0x06020000 | 3, (u8) (u32) (sp3C * 127.0f));
        temp_s0_2->unk0 = sp3C;
    }
    if (sp3B != (s8) temp_s0_2->unk8) {
        Nap_SetS8(((arg2 & 0xFF) << 8) | 0x05020000, sp3B);
        temp_s0_2->unk8 = (u8) sp3B;
    }
    if (sp34 != temp_s0_2->unk4) {
        Nap_SetF32(((arg2 & 0xFF) << 8) | 0x04020000, sp34);
        temp_s0_2->unk4 = sp34;
    }
    if (sp33 != (s8) temp_s0_2->unk9) {
        Nap_SetS8(((arg2 & 0xFF) << 8) | 0x03020000, sp33);
        temp_s0_2->unk9 = (u8) sp33;
    }
}

struct Unk_8018EFD8* Naa_Calcdoppler(u8 arg0, Vec3f position, f32* velocity, f32* arg3, u8 arg4, u32 soundBits) {
    struct Unk_8018EFD8* temp_a1;
    UNUSED struct Unk_8018EFD8* temp_v1;
    UNUSED u8 temp_t7;
    u8 why1;

    if (D_8018EFD8[D_8018FB90].next != 0xFF) {
        why1 = D_8018FB90;
        D_8018EFD8[why1].prev = D_8018FB91;
        temp_a1 = &D_8018EFD8[D_8018FB90];
        D_8018EFD8[D_8018FB91].next = why1;
        temp_t7 = temp_a1->next;
        D_8018FB91 = why1;
        D_8018FB90 = D_8018EFD8[D_8018FB90].next;
        D_8018EFD8[D_8018FB90].prev = 0xFF;
        D_8018EFD8[why1].posX = &position[0];
        D_8018EFD8[why1].posY = &position[1];
        D_8018EFD8[why1].posZ = &position[2];
        D_8018EFD8[why1].next = 0xFF;
        D_8018EFD8[why1].velX = &velocity[0];
        D_8018EFD8[why1].velY = &velocity[1];
        D_8018EFD8[why1].velZ = &velocity[2];
        D_8018EFD8[why1].unk18[1] = 0.0f;
        D_8018EFD8[why1].unk24 = arg3;
        D_8018EFD8[why1].cameraId = arg4;
        D_8018EFD8[why1].unk30 = arg0;
        D_8018EFD8[why1].soundBits = soundBits;
        return &D_8018EFD8[why1];
    }
    return NULL;
}

void Naa_CheckCameraPos(Camera* arg0, Vec3s rot, struct Unk_8018EFD8* arg2) {
    f32 x;
    f32 y;

    x = arg0->camera_pos[0] - *arg2->posX;
    y = arg0->camera_pos[2] - *arg2->posZ;
    arg2->unk18[0] = kwrotate2D_x(x, y, rot[1]);
    arg2->unk18[2] = kwrotate2D_y(x, y, rot[1]);
}

void Naa_Wpospt_to_Cpospt(Camera* camera, Vec3f velocity, struct Unk_8018EFD8* arg2) {
    f32 sp44;
    f32 temp_f6;
    f32 x;
    f32 y;
    f32 dist0;
    f32 dist1;
    f32 thing0;
    f32 thing1;
    f32 temp_f2;

    x = (*arg2->posX) - camera->camera_pos[0];
    y = (*arg2->posZ) - camera->camera_pos[2];

    sp44 = (*arg2->velX) - velocity[0];
    temp_f6 = (*arg2->velZ) - velocity[2];

    thing0 = x + sp44;
    thing1 = y + temp_f6;

    dist0 = sqrtf((x * x) + (y * y));
    dist1 = sqrtf((thing0 * thing0) + (thing1 * thing1));

    temp_f2 = 1.0f / (1.0f - ((dist0 - dist1) / arg2->unk34));

    if (temp_f2 > 0.1f) {
        arg2->unk2C = temp_f2;
    } else {
        arg2->unk2C = 0.1f;
    }

    if ((*arg2->unk24) != 0.0f) {
        arg2->unk2C *= (((*arg2->unk24) / D_800EA06C[arg2->unk30].unk00[1]) + D_800EA06C[arg2->unk30].unk00[0]) +
                       D_800E9F34[arg2->unk30];
    }
}

void Naa_CheckSubTrackPort(void) {
    u8 var_s1;
    u8 var_a1;
    u8 cameraId;
    Camera** camera;

    var_a1 = D_800EA1C0 + 1;
    for (var_s1 = 0; var_s1 < var_a1; var_s1++) {
        gVelocityCamera[var_s1][0] = gCopyCamera[var_s1]->camera_pos[0] - gCameraLastPos[var_s1][0];
        gVelocityCamera[var_s1][2] = gCopyCamera[var_s1]->camera_pos[2] - gCameraLastPos[var_s1][2];
        gCameraLastPos[var_s1][0] = gCopyCamera[var_s1]->camera_pos[0];
        gCameraLastPos[var_s1][2] = gCopyCamera[var_s1]->camera_pos[2];
    }

    var_a1 = 0;
    var_s1 = D_8018EFD8[0].next;
    while (var_s1 != 0xFF) {
        if (D_8018EFD8[var_s1].unk18[1] == 100000.0f) {
            if (D_8018FB91 == var_s1) {
                D_8018FB91 = D_8018EFD8[var_s1].prev;
            } else {
                D_8018EFD8[D_8018EFD8[var_s1].next].prev = D_8018EFD8[var_s1].prev;
            }
            D_8018EFD8[D_8018EFD8[var_s1].prev].next = D_8018EFD8[var_s1].next;
            D_8018EFD8[var_s1].prev = 0xFF;
            D_8018EFD8[var_s1].next = D_8018FB90;
            D_8018EFD8[D_8018FB90].prev = var_s1;
            D_8018FB90 = var_s1;
        } else {
            cameraId = D_8018EFD8[var_s1].cameraId;
            // Why? Why would you do it this way? For what possible reason?
            camera = &gCopyCamera[cameraId];
            Naa_CheckCameraPos(*camera, (*camera)->camera_direction, &D_8018EFD8[var_s1]);
            if (D_800EA1C8 != D_8018EFD8[var_s1].velX) {
                Naa_Wpospt_to_Cpospt(*camera, gVelocityCamera[0], &D_8018EFD8[var_s1]);
            }
            var_a1 = var_s1;
        }
        var_s1 = D_8018EFD8[var_a1].next;
        if ((var_s1 != 0xFF) && (D_800EA244 < var_s1)) {
            D_800EA244 = var_s1;
        }
    }
}

Vec3f* Naa_InitApliWork(Vec3f pos, u32 soundBits) {
    u8 it; // iterator
    Vec3f* ret;
    // Only here to force a match
    UNUSED f32* thing = pos;
    struct Unk_8018EFD8* temp_a1;

    ret = 0;
    it = D_8018EFD8[0].next;
    while (it != 0xFF) {
        temp_a1 = &D_8018EFD8[it];
        // It doesn't matter what you set var_v0 to here actually
        it = D_8018EFD8[0].next;
        if ((pos == temp_a1->posX) && (soundBits == temp_a1->soundBits)) {
            // I suspect all this weirdness here is a result of someone not knowing the 'break' keyword
            it = 0xFF;
            if (temp_a1->unk18[1] != 100000.0f) {
                ret = &temp_a1->unk18;
            } else {
                goto test;
            }
        } else {
        test:
            it = temp_a1->next;
        }
    }
    return ret;
}

void Nai_StartSeqSet(u8 player) {
    s16 sp46;
    s16 var_s1;
    s16 sp42;
    u8 var_s0;
    u8 temp_s0;
    u8 why = 0xFF;
    s32 var_a2;

    var_a2 = 0xF;
    if (SeqPlayer[player].enabled != 0) {
        switch (g_ScreenSplitB) { /* irregular */
            case 0:
                break;
            case 1:
                var_a2 = 0xE;
                break;
            case 3:
                if (g_playerCount == 3) {
                    var_a2 = 0xD;
                } else {
                    var_a2 = 0xC;
                }
                break;
        }
        temp_s0 = SeqPlayer[player].channels[var_a2]->soundScriptIO[0];
        if (temp_s0 != why) {
            sp46 = SeqPlayer[player].channels[var_a2]->soundScriptIO[1] % 4u;
            var_s1 = SeqPlayer[player].channels[var_a2]->soundScriptIO[2] % 16u;
            if (var_s1 >= 0xA) {
                var_s1 = 9;
            }
            sp42 = SeqPlayer[player].channels[var_a2]->soundScriptIO[3] % 8u;
            Nap_SetS8(((player & 0xFF) << 0x10) | 0x06000000 | ((var_a2 & 0xFF) << 8), -1);
        }
        switch (temp_s0) { /* switch 1; irregular */
            case 1:        /* switch 1 */
                D_8018FC10[sp46][0] = var_s1;
                D_8018FC10[sp46][1] = sp42;
                for (var_s0 = 0; var_s0 < 4; var_s0++) {
                    if (D_8018FC10[var_s0][0] != why) {
                        ogawa_sound_camera_set(var_s0, D_8018FC10[var_s0][0], D_8018FC10[var_s0][1]);
                        D_8018FC10[var_s0][0] = why;
                    }
                }
                break;
            case 2: /* switch 1 */
                D_8018FC10[sp46][0] = var_s1;
                D_8018FC10[sp46][1] = sp42;
                break;
        }
    }
}

void Nai_StopSeqSet(void) {
    u8 var_v0;

    D_8018EF10 = 0;
    gCopyCamera[0] = camera1;
    gCopyCamera[1] = camera2;
    gCopyCamera[2] = camera3;
    gCopyCamera[3] = camera4;
    D_8018FB91 = 0;
    D_8018FB90 = 1;
    D_800EA0F4 = 0;
    D_8018FC08 = 0;
    D_800EA104 = 0;
    D_800EA108 = 0;
    D_800EA0F0 = 0;
    g_gamePausedFlag = 0;
    Nap_SetS32(0xF2000000U, 0);
    g_gamePausedFlag = 0;
    D_800EA15C = 0;
    D_800EA160 = 0;
    D_800EA164 = 0;
    D_800EA178 = 1.0f;
    D_800EA17C = 0.85f;
    D_800EA180 = 0;
    D_800EA184 = 0;
    for (var_v0 = 0; var_v0 < 4; var_v0++) {
        D_800E9DB4[var_v0] = 0;
        D_800E9DF4[var_v0] = 0; // This is an f32 FYI, but 0.0f doesn't match right :)
        gVelocityCamera[var_v0][0] = 0.0f;
        gVelocityCamera[var_v0][1] = 0.0f;
        gVelocityCamera[var_v0][2] = 0.0f;
        gCameraLastPos[var_v0][0] = 0.0f;
        gCameraLastPos[var_v0][1] = 0.0f;
        gCameraLastPos[var_v0][2] = 0.0f;
        D_800EA0EC[var_v0] = 0;
        D_800E9EA4[var_v0] = 0;
        D_800E9F7C[var_v0].unk_14 = 0;
        g_PlayerSurfaceSoundID[var_v0] = 0;
        D_800E9E84[var_v0] = 0;
        D_800E9E94[var_v0] = 0;
        gPlayers[var_v0].tire_RL.Status = 0;
        gPlayers[var_v0].tire_RR.Status = 0;
        gPlayers[var_v0].slip_flag = 0;
        gPlayers[var_v0].brake_time = 0.0f;
        gPlayers[var_v0].slipang = 0;
        gPlayers[var_v0].taco = 0.0f;
        gPlayers[var_v0].water_flag = 0;
        D_8018FC10[var_v0][0] = 0x00FF;
        D_8018FC10[var_v0][1] = 0;
        D_800EA10C[var_v0] = 0;
        asm_DisableEngineSound[var_v0] = 0;
        D_800E9F78[var_v0] = 0;
    }
    for (var_v0 = 0; var_v0 < NUM_PLAYERS; var_v0++) {
        g_lightningFlagPlayer1[var_v0] = 0;
        D_800E9F2C[var_v0] = 0;
        D_800E9F34[var_v0] = 0.0f;
        D_800E9F54[var_v0] = 0.0f;
        D_800EA130[var_v0] = 0.0f;
        D_800EA06C[var_v0].unk0C = 0;
        gPlayers[var_v0].slip_flag = 0;
    }
    for (var_v0 = 0; var_v0 < 16; var_v0++) {
        D_8018EF18[var_v0].unk0 = 1.0f;
        D_8018EF18[var_v0].unk4 = 1.0f;
        D_8018EF18[var_v0].unk8 = 0;
        D_8018EF18[var_v0].unk9 = 0x40;
    }
    D_8018EFD8[0].prev = 0xFF;
    D_8018EFD8[0].next = 0xFF;
    for (var_v0 = 1; var_v0 < 49; var_v0++) {
        D_8018EFD8[var_v0].prev = var_v0 - 1;
        D_8018EFD8[var_v0].next = var_v0 + 1;
    }
    D_8018EFD8[var_v0].prev = var_v0 - 1;
    D_8018EFD8[var_v0].next = 0xFF;
}

void Nai_SeqControl(u8 arg0, u8 arg1, u8 arg2, u16 arg3) {
    u8 var_v1;
    UNUSED s32 pad;

    if ((D_800EA1EC != 0) && (arg0 != 2)) {
        return;
    }

    Nap_SetS32(0x82000000 | (((u32) arg0 & 0xFF) << 0x10) | (((u32) arg1 & 0xFF) << 8), arg3);
    D_801930D0[arg0].unk_248 = arg1 | (arg2 << 8);
    if (D_801930D0[arg0].unk_000 != 1.0f) {
        Nap_SetF32(0x41000000 | (((u32) arg0 & 0xFF) << 0x10), D_801930D0[arg0].unk_000);
    }
    D_801930D0[arg0].unk_028 = 0;
    D_801930D0[arg0].unk_018 = 0;
    D_801930D0[arg0].unk_014 = 0;

    for (var_v1 = 0; var_v1 < 16; var_v1++) {
        D_801930D0[arg0].unk_044[var_v1].unk_00 = 1.0f;
        D_801930D0[arg0].unk_044[var_v1].unk_0C = 0;
        D_801930D0[arg0].unk_044[var_v1].unk_10 = 1.0f;
        D_801930D0[arg0].unk_044[var_v1].unk_1C = 0;
    }

    D_801930D0[arg0].unk_244 = 0;
    D_801930D0[arg0].unk_246 = 0;
}

void func_800C29B4(u8 arg0, u16 arg1) {
    Nap_SetS32(((arg0 & 0xFF) << 0x10) | 0x83000000, arg1);
    D_801930D0[arg0].unk_248 = 0xFFFF;
}

void NAISeqFlagEntry(u32 cmd) {
    f32 freqScaleTarget;
    u16 channelMaskDisable;
    u16 val;
    u16 fadeTimer;
    u8 priority;
    u8 ioPort;
    u8 i;
    u8 duration;
    u8 channelIndex;
    u8 found;
    u8 seqId;
    u8 subArgs;
    u8 op;
    u8 seqPlayerIndex;

    op = cmd >> 28;
    seqPlayerIndex = (cmd & 0xF000000) >> 24;

    switch (op) {
        case 0:
            seqId = cmd & 0xFF;
            subArgs = (cmd & 0xFF00) >> 8;
            fadeTimer = (cmd & 0xFF0000) >> 13;
            Nai_SeqControl(seqPlayerIndex, seqId, subArgs, fadeTimer);
            break;

        case 1:
            fadeTimer = (cmd & 0xFF0000) >> 13;
            func_800C29B4(seqPlayerIndex, fadeTimer);
            break;

        case 2:
            seqId = cmd & 0xFF;
            subArgs = (cmd & 0xFF00) >> 8;
            fadeTimer = (cmd & 0xFF0000) >> 13;
            priority = subArgs;

            for (i = 0; i < D_80192CC6[seqPlayerIndex]; i++) {
                if (D_80192CA8[seqPlayerIndex][i].thing0 == seqId) {
                    if (i == 0) {
                        Nai_SeqControl(seqPlayerIndex, seqId, subArgs, fadeTimer);
                    }
                    return;
                }
            }

            found = D_80192CC6[seqPlayerIndex];
            for (i = 0; i < D_80192CC6[seqPlayerIndex]; i++) {
                if (priority >= D_80192CA8[seqPlayerIndex][i].thing1) {
                    found = i;
                    i = D_80192CC6[seqPlayerIndex];
                }
            }

            if ((found != D_80192CC6[seqPlayerIndex]) || (found == 0)) {
                if (D_80192CC6[seqPlayerIndex] < 5) {
                    D_80192CC6[seqPlayerIndex]++;
                }
                for (i = D_80192CC6[seqPlayerIndex] - 1; i != found; i--) {
                    D_80192CA8[seqPlayerIndex][i].thing1 = D_80192CA8[seqPlayerIndex][i - 1].thing1;
                    D_80192CA8[seqPlayerIndex][i].thing0 = D_80192CA8[seqPlayerIndex][i - 1].thing0;
                }

                D_80192CA8[seqPlayerIndex][found].thing1 = subArgs;
                D_80192CA8[seqPlayerIndex][found].thing0 = seqId;
            }
            if (found == 0) {
                Nai_SeqControl(seqPlayerIndex, seqId, subArgs, fadeTimer);
            }
            break;

        case 3:
            fadeTimer = (cmd & 0xFF0000) >> 13;

            found = D_80192CC6[seqPlayerIndex];
            for (i = 0; i < D_80192CC6[seqPlayerIndex]; i++) {
                seqId = cmd & 0xFF;
                if (D_80192CA8[seqPlayerIndex][i].thing0 == seqId) {
                    found = i;
                    i = D_80192CC6[seqPlayerIndex];
                }
            }

            if (found != D_80192CC6[seqPlayerIndex]) {
                for (i = found; i < (D_80192CC6[seqPlayerIndex] - 1); i++) {
                    D_80192CA8[seqPlayerIndex][i].thing1 = D_80192CA8[seqPlayerIndex][i + 1].thing1;
                    D_80192CA8[seqPlayerIndex][i].thing0 = D_80192CA8[seqPlayerIndex][i + 1].thing0;
                }

                D_80192CC6[seqPlayerIndex]--;
            }
            if (found == 0) {
                func_800C29B4(seqPlayerIndex, fadeTimer);
                if (D_80192CC6[seqPlayerIndex] != 0) {
                    Nai_SeqControl(seqPlayerIndex, D_80192CA8[seqPlayerIndex][0].thing0,
                                  D_80192CA8[seqPlayerIndex][0].thing1, fadeTimer);
                }
            }
            break;

        case 4:
            duration = (cmd & 0xFF0000) >> 15;
            val = cmd & 0xFF;

            if (duration == 0) {
                duration++;
            }

            D_801930D0[seqPlayerIndex].unk_004 = val / 127.0f;
            if (D_801930D0[seqPlayerIndex].unk_000 != D_801930D0[seqPlayerIndex].unk_004) {
                D_801930D0[seqPlayerIndex].unk_008 =
                    (D_801930D0[seqPlayerIndex].unk_000 - D_801930D0[seqPlayerIndex].unk_004) / duration;
                D_801930D0[seqPlayerIndex].unk_00C = duration;
            }
            break;

        case 5:
            duration = (cmd & 0xFF0000) >> 15;
            val = cmd & 0xFFFF;

            if (duration == 0) {
                duration++;
            }
            freqScaleTarget = (f32) val / 1000.0f;
            for (i = 0; i < 0x10; i++) {
                D_801930D0[seqPlayerIndex].unk_044[i].unk_14 = freqScaleTarget;
                D_801930D0[seqPlayerIndex].unk_044[i].unk_1C = duration;
                D_801930D0[seqPlayerIndex].unk_044[i].unk_18 =
                    (D_801930D0[seqPlayerIndex].unk_044[i].unk_10 - freqScaleTarget) / duration;
            }

            D_801930D0[seqPlayerIndex].unk_244 = 0xFFFF;
            break;

        case 6:
            duration = (cmd & 0xFF0000) >> 15;
            channelIndex = (cmd & 0xF00) >> 8;
            val = cmd & 0xFF;

            if (duration == 0) {
                duration++;
            }

            D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_04 = val / 127.0f;
            if (D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_00 !=
                D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_04) {
                D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_08 =
                    (D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_00 -
                     D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_04) /
                    duration;
                D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_0C = duration;
                D_801930D0[seqPlayerIndex].unk_244 |= 1 << channelIndex;
            }
            break;

        case 7:
            ioPort = (cmd & 0xFF0000) >> 16;
            val = cmd & 0xFF;
            Nap_SetS8((0x46000000 | ((seqPlayerIndex & 0xFF) << 0x10)) | ((ioPort & 0xFF) << 8), val);
            break;

        case 8:
            channelIndex = (cmd & 0xF00) >> 8;
            ioPort = (cmd & 0xFF0000) >> 16;
            val = cmd & 0xFF;
            if (!(D_801930D0[seqPlayerIndex].unk_24A & (1 << channelIndex))) {
                Nap_SetS8(((0x06000000 | ((seqPlayerIndex & 0xFF) << 0x10)) | (((u32) channelIndex & 0xFF) << 8)) |
                                  (ioPort & 0xFF),
                              val);
            }
            break;

        case 9:
            D_801930D0[seqPlayerIndex].unk_24A = cmd & 0xFFFF;
            break;

        case 10:
            val = 1;
            channelMaskDisable = cmd & 0xFFFF;
            for (i = 0; i < 0x10; i++) {
                Nap_SetS8(0x08000000 | ((seqPlayerIndex & 0xFF) << 0x10) | (((u32) i & 0xFF) << 8),
                              (channelMaskDisable & val) ? 1 : 0);
                val <<= 1;
            }

            break;

        case 11:
            D_801930D0[seqPlayerIndex].unk_014 = cmd;
            break;

        case 12:
            subArgs = (cmd & 0xF00000) >> 20;
            if (subArgs != 0xF) {
                found = D_801930D0[seqPlayerIndex].unk_041++;
                if (found < 5) {
                    D_801930D0[seqPlayerIndex].unk_02C[found] = cmd;
                    D_801930D0[seqPlayerIndex].unk_040 = 2;
                }
            } else {
                D_801930D0[seqPlayerIndex].unk_041 = 0;
            }
            break;

        case 14:
            subArgs = (cmd & 0xF00) >> 8;
            val = cmd & 0xFF;
            switch (subArgs) {
                case 0:
                    Nap_SetS32(0xF0000000, D_800EA1F0[val]);
                    break;

                case 1:
                    D_800EA1EC = val & 1;
                    break;
            }

            break;

        case 15:
            seqId = cmd & 0xFF;
            subArgs = (cmd & 0xFF00) >> 8;
            D_800EA1C0 = subArgs;
            Naa_FxmixSet((void*) seqId);
            D_800EA1F4[0] = seqId;
            Nap_SetS8(0x46020000, subArgs);
            func_800C5C40();
            break;

        default:
            break;
    }
}

void Nai_SeqBufFlagCheck(u32 arg0) {
    D_80192CD0[D_800EA1E4] = arg0;
    D_800EA1E4 += 1;
}

void Nai_GetPlayingSeqFlag(void) {
    for (D_800EA1E8; D_800EA1E4 != D_800EA1E8;) {
        NAISeqFlagEntry(D_80192CD0[D_800EA1E8++]);
    }
}

u16 NAIGetPlayingSeqFlag(u8 player) {
    if (!SeqPlayer[player].enabled) {
        return -1;
    }
    return D_801930D0[player].unk_248;
}

u8 Nai_CheckSeqFlagBuffer2(s32 arg0) {
    u8 var_v1;
    u8 i;

    i = D_800EA1E8;
    var_v1 = true;
    for (i = D_800EA1E8; i < (s32) D_800EA1E4; i++) {
        if ((u32) arg0 == D_80192CD0[i]) {
            var_v1 = false;
            i = D_800EA1E4;
        }
    }
    return var_v1;
}

#ifdef VERSION_EU
u8 func_800C357C_eu(s32 arg0, s32 arg1) {
    u8 var_v1;
    u8 i;

    i = D_800EA1E8;
    var_v1 = 1;
    for (i = D_800EA1E8; i < (s32) D_800EA1E4; i++) {
        if (arg0 == (D_80192CD0[i] & arg1)) {
            var_v1 = 0;
            i = D_800EA1E4;
        }
    }
    return var_v1;
}
#endif

void Nai_ClearAllSeqFlag(u8 arg0) {
    D_80192CC6[arg0] = 0;
}

void Nai_ClearSeqECheckFlag(u8 arg0, u8 arg1) {
    u8 var_v0;
    u8 thing;

    for (var_v0 = 0; var_v0 < D_801930D0[arg0].unk_041; var_v0++) {
        thing = (D_801930D0[arg0].unk_02C[var_v0] & 0xF00000) >> 0x14;
        if (thing == arg1) {
            D_801930D0[arg0].unk_02C[var_v0] = 0xFF000000;
        }
    }
}

void Nai_SetSeqLineVolMove(u8 arg0, u8 arg1, u8 arg2, u8 arg3) {
    D_801930D0[arg0].unk_00E[arg1] = arg2;
    D_801930D0[arg0].unk_011 = arg3;
    D_801930D0[arg0].unk_012 = 1;
}

void Nai_SeqGameFrameWork(void) {
    u8 seqPlayerIndex;
    f32 volume;
    u8 tempoOp;
    u16 tempoTarget;
    u8 channelIndex;
    u8 j;
    u32 tempoCmd;
    u16 tempoPrev;
    u8 tempoTimer;
    u8 setupOp;
    u8 targetSeqPlayerIndex;
    u8 setupVal1;
    u8 setupVal2;
    u16 seqId;

    for (seqPlayerIndex = 0; seqPlayerIndex < 3; seqPlayerIndex++) {
        if (D_801930D0[seqPlayerIndex].unk_012) {
            volume = 1.0f;
            for (j = 0; j < 3; j++) {
                volume *= D_801930D0[seqPlayerIndex].unk_00E[j] / 127.0f;
            }

            Nai_SeqBufFlagCheck(0x40000000 | (((u8) seqPlayerIndex) << 0x18) |
                          (((u8) D_801930D0[seqPlayerIndex].unk_011) << 0x10) | ((u16) (u8) (volume * 127.0f)));

            D_801930D0[seqPlayerIndex].unk_012 = 0;
        }
        if (D_801930D0[seqPlayerIndex].unk_00C != 0) {
            D_801930D0[seqPlayerIndex].unk_00C--;
            if (D_801930D0[seqPlayerIndex].unk_00C) {
                D_801930D0[seqPlayerIndex].unk_000 -= D_801930D0[seqPlayerIndex].unk_008;
            } else {
                D_801930D0[seqPlayerIndex].unk_000 = D_801930D0[seqPlayerIndex].unk_004;
            }
            Nap_SetF32(0x41000000 | (((u32) seqPlayerIndex & 0xFF) << 0x10), D_801930D0[seqPlayerIndex].unk_000);
        }
        if (D_801930D0[seqPlayerIndex].unk_014 != 0) {
            tempoCmd = D_801930D0[seqPlayerIndex].unk_014;
            tempoTimer = (tempoCmd & 0xFF0000) >> 0xF;
            tempoTarget = tempoCmd & 0xFFF;
            if (tempoTimer == 0) {
                tempoTimer++;
            }
            if (SeqPlayer[seqPlayerIndex].enabled != 0) {
                tempoPrev = SeqPlayer[seqPlayerIndex].tempo / 48;
                tempoOp = (tempoCmd & 0xF000) >> 0xC;
                switch (tempoOp) {
                    case 1:
                        tempoTarget += tempoPrev;
                        break;

                    case 2:
                        if (tempoTarget < tempoPrev) {
                            tempoTarget = tempoPrev - tempoTarget;
                        }
                        break;

                    case 3:
                        tempoTarget = tempoPrev * (tempoTarget / 100.0f);
                        break;

                    case 4:
                        tempoTarget =
                            (D_801930D0[seqPlayerIndex].unk_018 != 0) ? D_801930D0[seqPlayerIndex].unk_018 : tempoPrev;
                        break;
                    default:
                        break;
                }

                if (tempoTarget > 300) {
                    tempoTarget = 300;
                }
                if (D_801930D0[seqPlayerIndex].unk_018 == 0) {
                    D_801930D0[seqPlayerIndex].unk_018 = tempoPrev;
                }
                D_801930D0[seqPlayerIndex].unk_020 = tempoTarget;
                D_801930D0[seqPlayerIndex].unk_01C = SeqPlayer[seqPlayerIndex].tempo / 48;
                D_801930D0[seqPlayerIndex].unk_024 =
                    (D_801930D0[seqPlayerIndex].unk_01C - D_801930D0[seqPlayerIndex].unk_020) / tempoTimer;
                D_801930D0[seqPlayerIndex].unk_028 = tempoTimer;
            }
            D_801930D0[seqPlayerIndex].unk_014 = 0;
        }
        if (D_801930D0[seqPlayerIndex].unk_028 != 0) {
            D_801930D0[seqPlayerIndex].unk_028--;
            if (D_801930D0[seqPlayerIndex].unk_028) {
                D_801930D0[seqPlayerIndex].unk_01C -= D_801930D0[seqPlayerIndex].unk_024;
            } else {
                D_801930D0[seqPlayerIndex].unk_01C = D_801930D0[seqPlayerIndex].unk_020;
            }
            Nap_SetS32(0x47000000 | (((u32) seqPlayerIndex & 0xFF) << 0x10),
                          (s32) D_801930D0[seqPlayerIndex].unk_01C);
        }

        if (D_801930D0[seqPlayerIndex].unk_246 != 0) {
            for (channelIndex = 0; channelIndex < 0x10; channelIndex++) {
                if (D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_0C != 0) {
                    D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_0C--;
                    if (D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_0C) {
                        D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_00 -=
                            D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_08;
                    } else {
                        D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_00 =
                            D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_04;
                        D_801930D0[seqPlayerIndex].unk_246 ^= 1 << channelIndex;
                    }
                    Nap_SetF32(0x01000000 | ((seqPlayerIndex & 0xFF) << 0x10) | (((u32) channelIndex & 0xFF) << 8),
                                  D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_00);
                }
            }
        }
        if (D_801930D0[seqPlayerIndex].unk_244 != 0) {
            for (channelIndex = 0; channelIndex < 0x10; channelIndex++) {
                if (D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_1C != 0) {
                    D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_1C--;
                    if (D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_1C) {
                        D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_10 -=
                            D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_18;
                    } else {
                        D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_10 =
                            D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_14;
                        D_801930D0[seqPlayerIndex].unk_244 ^= 1 << channelIndex;
                    }
                    Nap_SetF32(0x04000000 | ((seqPlayerIndex & 0xFF) << 0x10) | (((u32) channelIndex & 0xFF) << 8),
                                  D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_10);
                }
            }
        }
        if (D_801930D0[seqPlayerIndex].unk_041) {
#ifdef VERSION_EU
            if (func_800C357C_eu(-0x10000000, -0x10000000) == 0) {
                D_801930D0[seqPlayerIndex].unk_041 = 0;
                return;
            }
#endif
            if (D_801930D0[seqPlayerIndex].unk_040 != 0) {
                D_801930D0[seqPlayerIndex].unk_040--;
                continue;
            }

            if (SeqPlayer[seqPlayerIndex].enabled != 0) {
                continue;
            }

            for (j = 0; j < D_801930D0[seqPlayerIndex].unk_041; j++) {
                setupOp = (D_801930D0[seqPlayerIndex].unk_02C[j] & 0xF00000) >> 0x14;
                targetSeqPlayerIndex = (D_801930D0[seqPlayerIndex].unk_02C[j] & 0xF0000) >> 0x10;
                setupVal2 = (D_801930D0[seqPlayerIndex].unk_02C[j] & 0xFF00) >> 8;
                setupVal1 = D_801930D0[seqPlayerIndex].unk_02C[j] & 0xFF;
                switch (setupOp) {
                    case 0:
                        D_801930D0[targetSeqPlayerIndex].unk_012 = 1;
                        D_801930D0[targetSeqPlayerIndex].unk_00E[1] = 0x7F;
                        break;

                    case 1:
                        Nai_SeqBufFlagCheck(0x30000000 | ((u8) seqPlayerIndex) << 0x18 |
                                      (D_801930D0[seqPlayerIndex].unk_248));
                        break;

                    case 2:
                        Nai_SeqBufFlagCheck((((u8) targetSeqPlayerIndex) << 0x18) | 0x10000 |
                                      (u16) (D_801930D0[targetSeqPlayerIndex].unk_248));
                        D_801930D0[targetSeqPlayerIndex].unk_012 = 1;
                        D_801930D0[targetSeqPlayerIndex].unk_00E[1] = 0x7F;
                        break;

                    case 3:
                        Nai_SeqBufFlagCheck(0xB0003000 | (((u8) targetSeqPlayerIndex) << 0x18) | (((u8) setupVal2) << 0x10) |
                                      (u16) setupVal1);
                        break;

                    case 4:
                        Nai_SeqBufFlagCheck(0xB0004000 | (((u8) targetSeqPlayerIndex) << 0x18) | (((u8) setupVal1) << 0x10));
                        break;

                    case 5:
                        seqId = D_801930D0[seqPlayerIndex].unk_02C[j] & 0xFFFF;
                        Nai_SeqBufFlagCheck((((u8) targetSeqPlayerIndex) << 0x18) |
                                      (((u8) D_801930D0[targetSeqPlayerIndex].unk_042) << 0x10) | ((u16) seqId));

                        Nai_SetSeqLineVolMove(targetSeqPlayerIndex, 1, 0x7F, 0);
                        D_801930D0[targetSeqPlayerIndex].unk_042 = 0;
                        break;

                    case 6:
                        D_801930D0[seqPlayerIndex].unk_042 = setupVal2;
                        break;
                }
            }

            D_801930D0[seqPlayerIndex].unk_041 = 0;
        }
    }
}

void Nai_seq_inter_init(void) {
    u8 var_v0;

    for (var_v0 = 0; var_v0 < 3; var_v0++) {
        D_80192CC6[var_v0] = 0;
        D_801930D0[var_v0].unk_248 = 0xFFFF;
        D_801930D0[var_v0].unk_000 = 1.0f;
        D_801930D0[var_v0].unk_00C = 0;
        D_801930D0[var_v0].unk_028 = 0;
        D_801930D0[var_v0].unk_018 = 0;
        D_801930D0[var_v0].unk_014 = 0;
        D_801930D0[var_v0].unk_24A = 0;
        D_801930D0[var_v0].unk_041 = 0;
        D_801930D0[var_v0].unk_042 = 0;
        D_801930D0[var_v0].unk_012 = 0;
        D_801930D0[var_v0].unk_00E[0] = 0x7F;
        D_801930D0[var_v0].unk_00E[1] = 0x7F;
        D_801930D0[var_v0].unk_00E[2] = 0x7F;
        D_801930D0[var_v0].unk_244 = 0;
        D_801930D0[var_v0].unk_246 = 0;
    }
}

void Nai_seq_inter_init_spec(void) {
    u8 var_v0;

    for (var_v0 = 0; var_v0 < 3; var_v0++) {
        D_80192CC6[var_v0] = 0;
        D_801930D0[var_v0].unk_248 = 0xFFFF;
        D_801930D0[var_v0].unk_028 = 0;
        D_801930D0[var_v0].unk_018 = 0;
        D_801930D0[var_v0].unk_014 = 0;
        D_801930D0[var_v0].unk_24A = 0;
        D_801930D0[var_v0].unk_041 = 0;
        D_801930D0[var_v0].unk_042 = 0;
        D_801930D0[var_v0].unk_244 = 0;
        D_801930D0[var_v0].unk_246 = 0;
    }
}

// Appears to be an unused combo of sound_banks_enable and sound_banks_disable
void Nai_FxSetLockFlag(u16 bankMask) {
    u8 bank;

    for (bank = 0; bank < SOUND_BANK_COUNT; bank++) {
        if (bankMask & 1) {
            sSoundBankDisabled[bank] = true;
        } else {
            sSoundBankDisabled[bank] = false;
        }
        bankMask = bankMask >> 1;
    }
}

void Nai_FxClearBgmMuteFlag(u8 arg0) {
    D_800EA1C4 &= ((1 << (arg0)) ^ (u16) -1);
    if (!D_800EA1C4) {
        D_801930D0[0].unk_012 = 1;
        D_801930D0[0].unk_00E[2] = 0x7F;
    }
}

void NAIFxFlagEntry(u32 soundBits, Vec3f* position, u8 cameraId, f32* arg3, f32* arg4, s8* arg5) {
    u8 bank;
    struct Sound* temp_v0;

    bank = soundBits >> 0x1C;
    if (sSoundBankDisabled[bank] == false) {
        temp_v0 = &sSoundRequests[sSoundRequestCount];
        temp_v0->soundBits = soundBits;
        temp_v0->position = position;
        temp_v0->cameraId = cameraId;
        temp_v0->unk0C = arg3;
        temp_v0->unk10 = arg4;
        temp_v0->unk14 = arg5;
        sSoundRequestCount += 1;
    }
}

void Nai_StopBufferFx(u8 arg0, struct SoundCharacteristics* arg1) {
    s32 found;
    u8 soundId;
    struct Sound* sound;

    for (soundId = sNumProcessedSoundRequests; soundId != sSoundRequestCount; soundId++) {
        found = false;
        sound = &sSoundRequests[soundId];
        switch (arg0) {
            case 0:
                if ((sound->soundBits & 0xF0000000) == (arg1->soundBits & 0xF0000000)) {
                    found = true;
                }
                break;
            case 1:
                if (((sound->soundBits & 0xF0000000) == (arg1->soundBits & 0xF0000000)) &&
                    (sound->position == arg1->unk00)) {
                    found = true;
                }
                break;
            case 2:
                if (sound->position == arg1->unk00) {
                    found = true;
                }
                break;
            case 3:
                if ((sound->position == arg1->unk00) && (sound->soundBits == arg1->soundBits)) {
                    found = true;
                }
                break;
            case 4:
                if ((sound->cameraId == arg1->cameraId) && (sound->soundBits == arg1->soundBits)) {
                    found = true;
                }
                break;
            case 5:
                if (sound->soundBits == arg1->soundBits) {
                    found = true;
                }
                break;
        }
        if (found) {
            sound->soundBits = 0;
            if (*sound->position != D_800EA1C8) {
                (*sound->position)[1] = 100000.0f;
            }
        }
    }
}

void Nai_EntryBufferFx(void) {
    u8 bank;
    u8 soundIndex;
    u8 var_a3;
    struct Sound* var_a2;
    UNUSED s32 pad;
    u8 var_t2;
    u32 var_t3;

    var_a2 = &sSoundRequests[sNumProcessedSoundRequests];
    if (var_a2->soundBits == 0) {
        return;
    }

    bank = ((u32) (var_a2->soundBits & 0xF0000000) >> 0x1C);
    soundIndex = sSoundBanks[bank][0].next;
    var_a3 = 0;
    while ((soundIndex != 0xFF) && (soundIndex != 0)) {
        if (var_a2->position == sSoundBanks[bank][soundIndex].unk00) {
            if (var_a2->soundBits == sSoundBanks[bank][soundIndex].soundBits) {
                var_a3 = D_800EA1A0[D_800EA1C0][bank];
            } else {
                if (var_a3 == 0) {
                    var_t2 = soundIndex;
                    var_t3 = sSoundBanks[bank][soundIndex].soundBits;
                } else if ((u32) (sSoundBanks[bank][soundIndex].soundBits & 0xFF00) < (u32) (var_t3 & 0xFF00)) {
                    var_t2 = soundIndex;
                    var_t3 = sSoundBanks[bank][soundIndex].soundBits;
                }
                var_a3++;
                if (var_a3 == D_800EA1A0[D_800EA1C0][bank]) {
                    if ((u32) (var_a2->soundBits & 0xFF00) >= (u32) (var_t3 & 0xFF00)) {
                        soundIndex = var_t2;
                    } else {
                        soundIndex = 0;
                    }
                }
            }
            if (var_a3 == D_800EA1A0[D_800EA1C0][bank]) {
                if ((var_a2->soundBits & 0x08000000) || (var_a2->soundBits & 0x40000) || (soundIndex == var_t2)) {
                    if ((sSoundBanks[bank][soundIndex].soundBits & 0x80000) &&
                        (sSoundBanks[bank][soundIndex].soundStatus != 1)) {
                        Nai_FxClearBgmMuteFlag(sSoundBanks[bank][soundIndex].unk2C);
                    }
                    sSoundBanks[bank][soundIndex].cameraId = var_a2->cameraId;
                    sSoundBanks[bank][soundIndex].soundBits = var_a2->soundBits;
                    sSoundBanks[bank][soundIndex].soundStatus = ((var_a2->soundBits & 0x01000000) >> 0x18);
                    sSoundBanks[bank][soundIndex].freshness = 2;
                    sSoundBanks[bank][soundIndex].unk10 = var_a2->unk0C;
                    sSoundBanks[bank][soundIndex].unk14 = var_a2->unk10;
                    sSoundBanks[bank][soundIndex].unk18 = var_a2->unk14;
                }
                soundIndex = 0;
            }
        }
        if (soundIndex != 0) {
            soundIndex = sSoundBanks[bank][soundIndex].next;
        }
    }
    if ((sSoundBanks[bank][sSoundBankFreeListFront[bank]].next != 0xFF) && (soundIndex != 0)) {
        var_t2 = soundIndex = sSoundBankFreeListFront[bank];
        sSoundBanks[bank][soundIndex].unk00 = (Vec3f*) &(*var_a2->position)[0];
        sSoundBanks[bank][soundIndex].unk04 = &(*var_a2->position)[1];
        sSoundBanks[bank][soundIndex].unk08 = &(*var_a2->position)[2];
        sSoundBanks[bank][soundIndex].cameraId = var_a2->cameraId;
        sSoundBanks[bank][soundIndex].unk10 = var_a2->unk0C;
        sSoundBanks[bank][soundIndex].unk14 = var_a2->unk10;
        sSoundBanks[bank][soundIndex].unk18 = var_a2->unk14;
        sSoundBanks[bank][soundIndex].soundBits = var_a2->soundBits;
        sSoundBanks[bank][soundIndex].soundStatus = (u8) ((u32) (var_a2->soundBits & 0x01000000) >> 0x18);
        sSoundBanks[bank][soundIndex].freshness = 2;
        sSoundBanks[bank][soundIndex].prev = sSoundBankUsedListBack[bank];
        sSoundBanks[bank][sSoundBankUsedListBack[bank]].next = sSoundBankFreeListFront[bank];
        sSoundBankUsedListBack[bank] = sSoundBankFreeListFront[bank];
        sSoundBankFreeListFront[bank] = sSoundBanks[bank][sSoundBankFreeListFront[bank]].next;
        sSoundBanks[bank][sSoundBankFreeListFront[bank]].prev = 0xFF;
        sSoundBanks[bank][var_t2].next = 0xFF;
    } else if (sSoundBanks[bank][sSoundBankFreeListFront[bank]].next == 0xFF) {
        if (D_800EA1C8 != *var_a2->position) {
            (*var_a2->position)[1] = 100000.0f;
        }
    }
}

void Nai_FxFlagFree(u8 bankId, u8 soundId) {
    UNUSED s32 stackPadding;
    struct SoundCharacteristics* temp = &sSoundBanks[bankId][soundId];
    if (*temp->unk00 != D_800EA1C8) {
        *temp->unk04 = 100000.0f;
    }
    if (temp->soundBits & 0x80000) {
        Nai_FxClearBgmMuteFlag(temp->unk2C);
    }
    if (soundId == sSoundBankUsedListBack[bankId]) {
        sSoundBankUsedListBack[bankId] = temp->prev;
    } else {
        sSoundBanks[bankId][temp->next].prev = temp->prev;
    }
    sSoundBanks[bankId][temp->prev].next = temp->next;
    temp->next = sSoundBankFreeListFront[bankId];
    temp->prev = 0xFF;
    sSoundBanks[bankId][sSoundBankFreeListFront[bankId]].prev = soundId;
    sSoundBankFreeListFront[bankId] = soundId;
    temp->soundStatus = 0;
}

struct ActiveSfx {
    u32 priority;
    u8 soundIndex;
};
#define AUDIO_MK_CMD(b0, b1, b2, b3) \
    ((((b0) & 0xFF) << 0x18) | (((b1) & 0xFF) << 0x10) | (((b2) & 0xFF) << 0x8) | (((b3) & 0xFF) << 0))
void Nai_FxNextPlayCheck(u8 bankId) {
    u8 j;
    u8 numChannels;
    u8 chosenEntryIndex;
    u8 i;
    u8 k;
    u8 numChosenSfx;
    u8 needNewSfx;
    u8 soundIndex;
    u8 requestedPriority;
    u8 temp_t8;
    f32 var_f0;
    struct ActiveSfx* activeSfx;
    struct ActiveSfx chosenSfx[8];
    struct SoundCharacteristics* entry;

    numChosenSfx = 0;
    for (i = 0; i < 8; i++) {
        chosenSfx[i].priority = 0x7FFFFFFF;
        chosenSfx[i].soundIndex = 0xFF;
    }

    soundIndex = sSoundBanks[bankId][0].next;
    k = 0;
    while (soundIndex != 0xFF) {
        if ((sSoundBanks[bankId][soundIndex].soundStatus == 1) &&
            ((sSoundBanks[bankId][soundIndex].soundBits & 0x08000000) == 0x08000000)) {
            sSoundBanks[bankId][soundIndex].freshness -= 1;
        }

        if (sSoundBanks[bankId][soundIndex].freshness == 0) {
            Nai_FxFlagFree(bankId, soundIndex);
        } else if (sSoundBanks[bankId][soundIndex].soundStatus != 0) {
            entry = &sSoundBanks[bankId][soundIndex];

            if (&D_800EA1C8 == entry[0].unk00) {
                entry->distance = 0.0f;
            } else {
                entry->distance = (*entry->unk00[0] * *entry->unk00[0]) + (*entry->unk08 * *entry->unk08);
            }
            requestedPriority = (((u32) (entry->soundBits & 0xFF00)) >> 8);
            if (entry->soundBits & 0x100000) {
                entry->priority = ((0xFF - requestedPriority) * (0xFF - requestedPriority)) * (38 * 38);
            } else {
                entry->priority =
                    ((u32) entry->distance) + (((0xFF - requestedPriority) * (0xFF - requestedPriority)) * (38 * 38));

                if ((*entry->unk08) > 0.0f) {
                    entry->priority += (s32) ((*entry->unk08) * 6.0f);
                }
            }
            temp_t8 = (((u32) (entry->soundBits & 0x30000)) >> 0x10);
            if (temp_t8) {
                var_f0 = (2000.0f * 2000.0f) / ((f32) (temp_t8 * temp_t8));
            } else {
                var_f0 = 1e5f * 1e5f;
            }
            if (var_f0 < entry->distance) {
                if (entry->soundStatus == 4) {
                    Nap_SetS8(AUDIO_MK_CMD(0x06, 2, entry->unk2C, 0), 0);
                    if (entry->soundBits & 0x08000000) {
                        Nai_FxFlagFree(bankId, soundIndex);
                        soundIndex = k;
                    }
                }
            } else {
                numChannels = D_800EA188[D_800EA1C0][bankId];
                for (i = 0; i < numChannels; i++) {
                    if (chosenSfx[i].priority >= entry->priority) {
                        if (numChosenSfx < D_800EA188[D_800EA1C0][bankId]) {
                            numChosenSfx++;
                        }
                        for (j = numChannels - 1; j > i; j--) {
                            chosenSfx[j].priority = chosenSfx[j - 1].priority;
                            chosenSfx[j].soundIndex = chosenSfx[j - 1].soundIndex;
                        }

                        chosenSfx[i].priority = entry->priority;
                        chosenSfx[i].soundIndex = soundIndex;
                        i = numChannels;
                    }
                }
            }
            k = soundIndex;
        }
        soundIndex = sSoundBanks[bankId][k].next;
    }

    numChannels = D_800EA188[D_800EA1C0][bankId];
    for (i = 0; i < numChosenSfx; i++) {
        if (sSoundBanks[bankId][chosenSfx[i].soundIndex].soundStatus == 1) {
            sSoundBanks[bankId][chosenSfx[i].soundIndex].soundStatus = 2;
        } else if (sSoundBanks[bankId][chosenSfx[i].soundIndex].soundStatus == 4) {
            sSoundBanks[bankId][chosenSfx[i].soundIndex].soundStatus = 3;
        }
    }

    for (i = 0; i < numChannels; i++) {
        needNewSfx = false;
        activeSfx = (struct ActiveSfx*) &D_80192AB8[bankId][i];

        if (activeSfx->soundIndex == 0xFF) {
            needNewSfx = true;
        } else {
            entry = &sSoundBanks[bankId][activeSfx->soundIndex];
            if (entry->soundStatus == 4) {
                if (entry->soundBits & 0x08000000) {
                    Nai_FxFlagFree(bankId, activeSfx->soundIndex);
                } else {
                    entry->soundStatus = 1;
                }
                needNewSfx = true;
            } else if (entry->soundStatus == 0) {
                activeSfx->soundIndex = 0xFF;
                needNewSfx = true;
            } else {
                for (j = 0; j < numChannels; j++) {
                    if (activeSfx->soundIndex == chosenSfx[j].soundIndex) {
                        chosenSfx[j].soundIndex = 0xFF;
                        j = numChannels;
                    }
                }
                numChosenSfx--;
            }
        }

        if (needNewSfx == true) {
            for (j = 0; j < numChannels; j++) {
                chosenEntryIndex = chosenSfx[j].soundIndex;
                if ((chosenEntryIndex != 0xFF) && (sSoundBanks[bankId][chosenEntryIndex].soundStatus != 3)) {
                    for (k = 0; k < numChannels; k++) {
                        if (chosenEntryIndex == ((struct ActiveSfx*) (D_80192AB8[bankId]))[k].soundIndex) {
                            needNewSfx = false;
                            k = numChannels;
                        }
                    }

                    if (needNewSfx == true) {
                        activeSfx->soundIndex = chosenEntryIndex;
                        chosenSfx[j].soundIndex = 0xFF;
                        j = numChannels + 1;
                        numChosenSfx--;
                    }
                }
            }

            if (j == numChannels) {
                activeSfx->soundIndex = 0xFF;
            }
        }
    }
}

void Nai_FxNowPlayCheck(u8 bankId) {
    u8 soundId;
    u8 var_s4;
    UNUSED u32 cmd;
    struct SoundCharacteristics* temp_s0;
    struct SequenceChannel* thing;

    for (var_s4 = 0; var_s4 < D_800EA188[D_800EA1C0][bankId]; var_s4++) {
        soundId = D_80192AB8[bankId][var_s4][4];
        if (soundId != 0xFF) {
            temp_s0 = &sSoundBanks[bankId][soundId];
            thing = SeqPlayer[2].channels[D_80192C38];
            if (temp_s0->soundStatus == 2) {
                temp_s0->unk2C = D_80192C38;
                if (temp_s0->soundBits & 0x80000) {
                    D_800EA1C4 |= 1 << D_80192C38;
                    D_801930D0->unk_012 = 1;
                    D_801930D0->unk_00E[2] = 0x28;
                }
                Naa_CalcCameraPos(bankId, soundId, D_80192C38);
                Nap_SetS8(((D_80192C38 & 0xFF) << 8) | 0x06020000, 1);
                Nap_SetS8(((D_80192C38 & 0xFF) << 8) | 0x06020000 | 4, (u8) (temp_s0->soundBits & 0xFF));
                temp_s0->soundStatus = 4U;
            } else if (((u8) thing->soundScriptIO[7]) == 0x80) {
                Nap_SetS8(((D_80192C38 & 0xFF) << 8) | 0x06020000 | 7, 0);
                Nai_FxFlagFree(bankId, soundId);
            } else if (temp_s0->soundStatus == 3) {
                Naa_CalcCameraPos(bankId, soundId, D_80192C38);
                temp_s0->soundStatus = 4U;
            }
        }
        D_80192C38 += 1;
    }
}

// Seems somewhat similar to certain parts of `select_current_sounds` from SM64
void Nai_StopAllHandleFx(u8 bankId) {
    UNUSED s32 stackPadding0;
    u8 soundId;
    struct SoundCharacteristics sp60;

    soundId = sSoundBanks[bankId][0].next;
    while (soundId != 0xFF) {
        if (sSoundBanks[bankId][soundId].soundStatus >= 3) {
            Nap_SetS8(((sSoundBanks[bankId][soundId].unk2C & 0xff) << 8) | 0x06020000, 0);
        }
        if (sSoundBanks[bankId][soundId].soundStatus != 0) {
            Nai_FxFlagFree(bankId, soundId);
        }
        // This feels wrong, I think it only works because `Nai_FxFlagFree`
        // modifies `sSoundBanks[bankId][0].next`, which allows us to eventually(?)
        // break out of the loop
        soundId = sSoundBanks[bankId][0].next;
    }
    sp60.soundBits = bankId << 0x1C;
    Nai_StopBufferFx(0, &sp60);
}

void Nai_StopAllObjHandleFx2(u8 arg0, Vec3f* arg1) {
    u8 curr;
    u8 next;

    curr = 0;
    next = sSoundBanks[arg0][0].next;

    while (next != 0xff) {
        if (*arg1 == *sSoundBanks[arg0][next].unk00) {
            if (sSoundBanks[arg0][next].soundStatus >= 3) {
                Nap_SetS8((0x06020000 | ((sSoundBanks[arg0][next].unk2C & 0xff) << 8)), 0);
            }

            if (sSoundBanks[arg0][next].soundStatus != 0) {
                Nai_FxFlagFree(arg0, next);
            }
        } else {
            curr = next;
        }

        next = sSoundBanks[arg0][curr].next;
    }
}

void Nai_StopAllObjHandleFx(u8 arg0, Vec3f* arg1) {
    struct SoundCharacteristics unk;
    Nai_StopAllObjHandleFx2(arg0, arg1);
    unk.soundBits = arg0 << 0x1C;
    unk.unk00 = arg1;
    Nai_StopBufferFx(1, &unk);
}

void Nai_StopAllObjFx(Vec3f* arg0) {
    u8 i;
    struct SoundCharacteristics sp3C;

    for (i = 0; i < 6; i++) {
        Nai_StopAllObjHandleFx2(i, arg0);
    }

    sp3C.unk00 = arg0;
    Nai_StopBufferFx(2, &sp3C);
}

void Nai_StopObjFlagFx(Vec3f* arg0, u32 soundBits) {
    UNUSED s32 stackPadding0;
    u8 bankId;
    u8 next;
    u8 curr;
    struct SoundCharacteristics sp60;

    bankId = (soundBits & 0xF0000000) >> 0x1C;
    next = sSoundBanks[bankId][0].next;
    curr = 0;
    while (next != 0xFF) {
        if ((arg0 == sSoundBanks[bankId][next].unk00) && (soundBits == sSoundBanks[bankId][next].soundBits)) {
            if (sSoundBanks[bankId][next].soundStatus >= 3) {
                Nap_SetS8(((sSoundBanks[bankId][next].unk2C & 0xff) << 8) | 0x06020000, 0);
            }
            if (sSoundBanks[bankId][next].soundStatus != 0) {
                Nai_FxFlagFree(bankId, next);
            }
            next = 0xFF;
        } else {
            curr = next;
        }
        if (next != 0xFF) {
            next = sSoundBanks[bankId][curr].next;
        }
    }
    sp60.unk00 = arg0;
    sp60.soundBits = soundBits;
    Nai_StopBufferFx(3, &sp60);
}

void Nai_StopFx(u32 soundBits) {
    UNUSED s32 stackPadding0;
    u8 bankId;
    u8 next;
    u8 curr;
    struct SoundCharacteristics sp68;

    bankId = (soundBits & 0xF0000000) >> 0x1C;
    next = sSoundBanks[bankId][0].next;
    curr = 0;
    while (next != 0xFF) {
        if (soundBits == sSoundBanks[bankId][next].soundBits) {
            if (sSoundBanks[bankId][next].soundStatus >= 3) {
                Nap_SetS8(((sSoundBanks[bankId][next].unk2C & 0xff) << 8) | 0x06020000, 0);
            }
            if (sSoundBanks[bankId][next].soundStatus != 0) {
                Nai_FxFlagFree(bankId, next);
            }
        } else {
            curr = next;
        }
        next = sSoundBanks[bankId][curr].next;
    }
    sp68.soundBits = soundBits;
    Nai_StopBufferFx(5, &sp68);
}

// Might be process_all_sound_requests
void Nai_FxBufFlagCheck(void) {
    while (sSoundRequestCount != sNumProcessedSoundRequests) {
        Nai_EntryBufferFx();
        sNumProcessedSoundRequests++;
    }
}

// EXTREMELY similar to `Nai_SetFxHandleVolume` from SM64
void Nai_SetFxHandleVolume(u8 player, u8 targetScale, u16 fadeDuration) {
    struct ChannelVolumeScaleFade* temp_v0;

    if (fadeDuration == 0) {
        fadeDuration++;
    }
    temp_v0 = &D_80192C48[player];
    temp_v0->target = targetScale / US_FLOAT(127.0);
    temp_v0->remainingFrames = fadeDuration;
    temp_v0->velocity = (temp_v0->current - temp_v0->target) / fadeDuration;
}

void Nai_FxHandleFrameCheck(u8 arg0) {
    struct ChannelVolumeScaleFade* temp_v0;

    temp_v0 = &D_80192C48[arg0];
    if (temp_v0->remainingFrames != 0) {
        temp_v0->remainingFrames--;
        if (temp_v0->remainingFrames != 0) {
            temp_v0->current -= temp_v0->velocity;
        } else {
            temp_v0->current = temp_v0->target;
        }
    }
}

void Nai_FxPlayCheck(void) {
    u8 i;

    if (SeqPlayer[2].channels[0] != &gSequenceChannelNone) {
        D_80192C38 = 0;
        for (i = 0; i < 6; i++) {
            Nai_FxNextPlayCheck(i);
            Nai_FxNowPlayCheck(i);
            Nai_FxHandleFrameCheck(i);
        }
    }
}

// Looks EXTEREMELY similar to Nai_fx_inter_init from SM64
void Nai_fx_inter_init(void) {
    u8 var_v0;
    u8 var_v1;

    sSoundRequestCount = 0;
    sNumProcessedSoundRequests = 0;
    D_800EA1C4 = 0;
    for (var_v0 = 0; var_v0 < SOUND_BANK_COUNT; var_v0++) {
        sSoundBankUsedListBack[var_v0] = 0;
        sSoundBankFreeListFront[var_v0] = 1;
        sNumSoundsInBank[var_v0] = 0;
        sSoundBankDisabled[var_v0] = false;
        D_80192C48[var_v0].current = 1.0f;
        D_80192C48[var_v0].remainingFrames = 0;
    }
    for (var_v0 = 0; var_v0 < 6; var_v0++) {
        for (var_v1 = 0; var_v1 < 8; var_v1++) {
            D_80192AB8[var_v0][var_v1][4] = 0xFF;
        }
    }
    for (var_v0 = 0; var_v0 < 6; var_v0++) {
        sSoundBanks[var_v0][0].prev = 0xFF;
        sSoundBanks[var_v0][0].next = 0xFF;
        for (var_v1 = 1; var_v1 < 19; var_v1++) {
            sSoundBanks[var_v0][var_v1].prev = var_v1 - 1;
            sSoundBanks[var_v0][var_v1].next = var_v1 + 1;
        }
        sSoundBanks[var_v0][var_v1].prev = var_v1 - 1;
        sSoundBanks[var_v0][var_v1].next = 0xFF;
    }
}

void Naa_PlyEngWave_Load(void) {
    if (D_800EA1C0 == 0) {
        Nap_SetS8(((D_800EA154[gPlayers[0].kart] & 0xFFFF) << 8) | 0xF3000000, 0);
    } else {
        Nap_SetS8(0xF3004D00, 0);
    }
}

void func_800C5C40(void) {
    Nai_StopSeqSet();
    if (D_800E9DA0 != 0) {
        Nai_seq_inter_init_spec();
    } else {
        D_800E9DA0++;
        Nai_seq_inter_init();
    }
    Nai_fx_inter_init();
    Nai_SeqControl(2, 0, 0xFF, 1);
    Naa_PlyEngWave_Load();
}

// game_audio_init?
void Nai_InitInterface(void) {
    Nai_StopSeqSet();
    Nai_seq_inter_init();
    Nai_fx_inter_init();
    Nai_SeqControl(2, 0, 0xFF, 1);
}

void accel_onoff(u8 playerId) {
    if ((gPlayers[playerId].handling_flag & ACCELERATE) == ACCELERATE) {
        D_800E9E34[playerId] = 0;
        if (D_800E9E24[playerId] < 0x4E20) {
            if ((u8) g_gamePausedFlag == 0) {
                D_800E9E24[playerId]++;
            }
        }
        if (D_800E9E24[playerId] == 1) {
            switch (D_800EA0EC[playerId]) {
                case 2:
                    D_800EA0EC[playerId] = 1;
                case 0:
                    Na_PlyEng_Stop(playerId);
                    Na_PlyEng_Start(playerId);
                    break;
                default:
                    break;
            }
        }
    } else {
        D_800E9E24[playerId] = 0;
        if ((D_800E9E34[playerId] < 0x4E20) && ((u8) g_gamePausedFlag == 0)) {
            D_800E9E34[playerId]++;
        }
    }
}

void accel_volume(u8 playerId) {
    if (D_800EA108 == 0) {
        if (((gPlayers[playerId].handling_flag & ACCELERATE) != ACCELERATE) && (gPlayers[playerId].taco > 400.0f)) {
            D_800E9E14[playerId] = 1;
            if (D_800EA0EC[playerId] == 0) {
                D_800E9F7C[playerId].unk_10 = 0.6f - D_800E9F54[playerId];
            }
            D_800E9DC4[playerId] = D_800E9F7C[playerId].unk_30;
            D_800E9DD4[playerId] = D_800E9F7C[playerId].unk_24;
            if ((D_800E9E34[playerId] == 1) && ((u8) g_gamePausedFlag == 0)) {
                switch (D_800EA0EC[playerId]) {
                    case 2:
                        D_800EA0EC[playerId] = 1;
                    case 0:
                        if ((asm_DisableEngineSound[playerId] == 0) && (gPlayers[playerId].kart != 3)) {
                            if ((s32) D_800EA1C0 < 2) {
                                NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF9, 0x26), &D_800E9F7C[playerId].pos, playerId,
                                           &D_800E9F7C[playerId].unk_38, &D_800E9F04[playerId],
                                           (s8*) &D_800E9F7C[playerId].unk_14);
                            } else {
                                NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0x80, 0x26), &D_800E9F7C[playerId].pos, playerId,
                                           &D_800E9F7C[playerId].unk_38, &D_800E9F04[playerId],
                                           (s8*) &D_800E9F7C[playerId].unk_14);
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
        } else {
            if (D_800E9E24[playerId] == 0x0000000A) {
                if ((s32) D_800EA1C0 < 2) {
                    NaPlyLevelStop(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xF9, 0x26));
                } else {
                    NaPlyLevelStop(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0x80, 0x26));
                }
            }
            D_800E9E14[playerId] = 0;
            D_800E9F7C[playerId].unk_10 = (1.0f - D_800E9F54[playerId]) - D_800EA130[playerId];
            if ((1.0f - D_800E9F54[playerId]) < D_800EA130[playerId]) {
                D_800E9F7C[playerId].unk_10 = 0.0f;
            }
            D_800E9DC4[playerId] = D_800E9F7C[playerId].unk_2C;
            D_800E9DD4[playerId] = D_800E9F7C[playerId].unk_20;
        }
    }
}

void engine_freq_set(u8 playerId) {
    Player* player;

    player = &gPlayers[playerId];
    D_800E9E64[playerId] = (player->taco / D_800E9DC4[playerId]) + D_800E9DD4[playerId];
    if ((player->taco < 1800.0f) && ((player->handling_flag & ACCELERATE) != ACCELERATE)) {
        D_800E9E64[playerId] = (player->taco / D_800E9F7C[playerId].unk_34) + D_800E9F7C[playerId].unk_28;
        if (D_800E9EC4) {} // ?
    }
    if (player->speed > 4.75f) {
        if (D_800E9EB4[playerId] < (D_800E9F7C[playerId].unk_18 + 0.4f)) {
            D_800E9DE4[playerId] += 0.005f;
        }
    } else {
        D_800E9DE4[playerId] = 0.0f;
    }
    if (gPlayers[playerId].slipang > 0) {
        D_800E9E54[playerId] = (f32) player->slipang;
    } else {
        D_800E9E54[playerId] = (f32) -player->slipang;
    }
    if ((player->slip_flag & DRIFT) == DRIFT) {
        D_800E9EB4[playerId] = D_800E9E64[playerId] + D_800E9DE4[playerId];
    } else {
        D_800E9EB4[playerId] = D_800E9E64[playerId] + D_800E9DE4[playerId] - (D_800E9E54[playerId] / 12000.0f);
        if (D_800E9EB4[playerId] < 0.01f) {
            D_800E9EB4[playerId] = 0.01f;
        }
    }
    if ((asm_DisableEngineSound[playerId]) || (D_800E9F78[playerId])) {
        if (asm_DisableEngineSound[playerId]) {
            if (D_800E9F78[playerId] < 0xF) {
                D_800E9F78[playerId]++;
                D_800E9DF4[playerId] += 0.03f;
            }
        } else if (D_800E9F78[playerId]) {
            D_800E9F78[playerId]--;
            D_800E9DF4[playerId] -= 0.03f;
        }
        D_800E9EB4[playerId] -= D_800E9DF4[playerId];
    }
    D_800E9EE4[playerId] = D_800E9EB4[playerId] - D_800E9EC4[playerId];
#ifdef VERSION_EU
    if ((D_800E9EE4[playerId] > 0.5f) || (D_800E9EE4[playerId] < -0.5f))
#else
    if ((D_800E9EE4[playerId] > 0.5f) || (D_800E9EE4[playerId] < 0.5f))
#endif
    {
        D_800E9ED4[playerId] = D_800E9EE4[playerId] * 0.25f;
        D_800E9F7C[playerId].unk_0C = D_800E9EC4[playerId] + D_800E9ED4[playerId] + D_800E9F34[playerId];
    } else {
        D_800E9F7C[playerId].unk_0C = D_800E9EB4[playerId] + D_800E9F34[playerId];
    }
#ifdef VERSION_EU
    if (D_800E9F7C[playerId].unk_0C < 0.0f) {
        D_800E9F7C[playerId].unk_0C = 0.0f;
    }
#endif
    if (D_800E9F7C[playerId].unk_0C > 4.0f) {
        D_800E9F7C[playerId].unk_0C = 4.0f;
    }
    D_800E9EC4[playerId] = D_800E9F7C[playerId].unk_0C;
    D_800E9F7C[playerId].unk_38 = (D_800E9F7C[playerId].unk_0C / 1.5f) + 0.4f;
}

void level_volume_set(u8 playerId) {
    switch (g_PlayerSurfaceSoundID[playerId]) {
        case 3:
            D_800E9EF4[playerId] = (gPlayers[playerId].speed / 5.0f) + 0.2f;
            break;
        case 1:
        case 13:
        case 14:
        case 17:
            D_800E9EF4[playerId] = ((D_800E9E54[playerId] - 3500.0f) / 3000.0f) + 0.4f;
            break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 22:
        case 23:
        case 24:
        case 29:
        case 30:
        case 31:
            D_800E9EF4[playerId] = (gPlayers[playerId].speed / 5.0f) + 0.2f;
            break;
        default:
            D_800E9EF4[playerId] = 1.0f;
            break;
    }
    if (D_800E9EF4[playerId] > 1.0f) {
        D_800E9EF4[playerId] = 1.0f;
    }
    if (D_800E9EF4[playerId] < 0.0f) {
        D_800E9EF4[playerId] = 0.0f;
    }
    if ((gPlayers[playerId].handling_flag & ACCELERATE) == ACCELERATE) {
        D_800E9F04[playerId] = 0.56f - (D_800E9E24[playerId] * 0.06f);
    } else {
        D_800E9F04[playerId] = (D_800E9E34[playerId] / 50.0f) + 0.25f;
    }
    if (g_lightningFlagPlayer1[playerId] != 0) {
        D_800E9F04[playerId] = 0.0f;
    }
    if (D_800E9F04[playerId] > 0.9f) {
        D_800E9F04[playerId] = 0.9f;
    }
}

void level_freq_set(u8 playerId) {
    switch (g_PlayerSurfaceSoundID[playerId]) { /* irregular */
        case 3:
            D_800E9F14[playerId] = (gPlayers[playerId].speed / 9.0f) + 0.6f;
            break;
        case 2:
        case 13:
        case 17:
            D_800E9F14[playerId] = (D_800E9E54[playerId] / 13000.0f) + 0.95f;
            break;
        default:
            D_800E9F14[playerId] = 1.0f;
            break;
    }
}

void level_sound_trigger(u8 cameraId) {
    if ((D_800EA108 == 0) && (D_800EA0EC[cameraId] == 0)) {
        if (g_PlayerSurfaceSoundID[cameraId] != D_800E9E84[cameraId]) {
            Nai_StopObjFlagFx(&D_800E9F7C[cameraId].pos, D_800E9E94[cameraId]);
            switch (g_PlayerSurfaceSoundID[cameraId]) {
                case 3:
                    NAIFxFlagEntry(SOUND_ACTION_TIRE_SQUEAL, &D_800E9F7C[cameraId].pos, cameraId, &D_800E9F14[cameraId],
                               &D_800E9EF4[cameraId], (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ACTION_TIRE_SQUEAL;
                    break;
                case 18:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF8, 0x1D), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800EA1D4, (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF8, 0x1D);
                    break;
                case 19:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF8, 0x22), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800EA1D4, (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF8, 0x22);
                    break;
                case 1:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x09), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x09);
                    break;
                case 2:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF4, 0x0A), &D_800E9F7C[cameraId].pos, cameraId,
                               &D_800E9F14[cameraId], &D_800EA1D4, (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF4, 0x0A);
                    break;
                case 17:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x1E), &D_800E9F7C[cameraId].pos, cameraId,
                               &D_800E9F14[cameraId], &D_800E9EF4[cameraId], (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x1E);
                    break;
                case 15:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x1F), &D_800E9F7C[cameraId].pos, cameraId,
                               &D_800E9F14[cameraId], &D_800EA1D4, (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x1F);
                    break;
                case 16:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x21), &D_800E9F7C[cameraId].pos, cameraId,
                               &D_800E9F14[cameraId], &D_800EA1D4, (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x21);
                    break;
                case 20:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x27), &D_800E9F7C[cameraId].pos, cameraId,
                               &D_800E9F14[cameraId], &D_800EA1D4, (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x27);
                    break;
                case 25:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x20), &D_800E9F7C[cameraId].pos, cameraId,
                               &D_800E9F14[cameraId], &D_800EA1D4, (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x20);
                    break;
                case 26:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x23), &D_800E9F7C[cameraId].pos, cameraId,
                               &D_800E9F14[cameraId], &D_800EA1D4, (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x23);
                    break;
                case 27:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0x80, 0x46), &D_800E9F7C[cameraId].pos, cameraId,
                               &D_800E9F14[cameraId], &D_800EA1D4, (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0x80, 0x46);
                    break;
                case 28:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x25), &D_800E9F7C[cameraId].pos, cameraId,
                               &D_800E9F14[cameraId], &D_800EA1D4, (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x25);
                    break;
                case 4:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0B), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0B);
                    break;
                case 5:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0C), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0C);
                    break;
                case 6:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0D), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0D);
                    break;
                case 7:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0E), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0E);
                    break;
                case 8:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0F), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0F);
                    break;
                case 9:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x10), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x10);
                    break;
                case 10:
                case 14:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x11), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x11);
                    break;
                case 11:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x12), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x12);
                    break;
                case 12:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x13), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x13);
                    break;
                case 29:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x48), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x48);
                    break;
                case 30:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x49), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x49);
                    break;
                case 31:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x4A), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x4A);
                    break;
                case 13:
                case 22:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x29), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x29);
                    break;
                case 23:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x2A), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x2A);
                    break;
                case 24:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x2B), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (s8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x2B);
                    break;
                default:
                    Nai_StopObjFlagFx(&D_800E9F7C[cameraId].pos, D_800E9E94[cameraId]);
                    break;
            }
        }
        D_800E9E84[cameraId] = g_PlayerSurfaceSoundID[cameraId];
    }
}

void make_level_sound_trg(u8 playerId) {
    if (D_800EA0EC[playerId] == 0) {
        g_PlayerSurfaceSoundID[playerId] = 0;
        if ((D_800E9E54[playerId] > 3500.0f) || ((gPlayers[playerId].slip_flag & DRIFT) == DRIFT)) {
            g_PlayerSurfaceSoundID[playerId] = 1;
            switch (gPlayers[playerId].tire_RL.Status) {
                case DIRT: /* switch 1 */
                    g_PlayerSurfaceSoundID[playerId] = 0x0000000D;
                    break;
                case SAND: /* switch 1 */
                    g_PlayerSurfaceSoundID[playerId] = 0x0000000E;
                    break;
                case BRIDGE: /* switch 1 */
                    g_PlayerSurfaceSoundID[playerId] = 0x00000011;
                    break;
                case STONE: /* switch 1 */
                    g_PlayerSurfaceSoundID[playerId] = 0x0000000F;
                    break;
                case SNOW: /* switch 1 */
                    g_PlayerSurfaceSoundID[playerId] = 0x00000010;
                    break;
                case ICE: /* switch 1 */
                    g_PlayerSurfaceSoundID[playerId] = 0x00000014;
                    break;
                case ROPE_BRIDGE: /* switch 1 */
                    g_PlayerSurfaceSoundID[playerId] = 0x00000019;
                    break;
                case WOOD_BRIDGE: /* switch 1 */
                    g_PlayerSurfaceSoundID[playerId] = 0x0000001A;
                    break;
                case TRAIN_TRACK: /* switch 1 */
                    g_PlayerSurfaceSoundID[playerId] = 0x0000001B;
                    break;
            }
        }
        if ((gPlayers[playerId].slip_flag & DRIFT) == DRIFT) {
            g_PlayerSurfaceSoundID[playerId] = 2;
            switch (gPlayers[playerId].tire_RL.Status) { /* switch 2 */
                case DIRT:                                                   /* switch 2 */
                    g_PlayerSurfaceSoundID[playerId] = 0x0000000D;
                    break;
                case SAND: /* switch 2 */
                    g_PlayerSurfaceSoundID[playerId] = 0x0000000E;
                    break;
                case BRIDGE: /* switch 2 */
                    g_PlayerSurfaceSoundID[playerId] = 0x00000011;
                    break;
                case STONE: /* switch 2 */
                    g_PlayerSurfaceSoundID[playerId] = 0x0000000F;
                    break;
                case SNOW: /* switch 2 */
                    g_PlayerSurfaceSoundID[playerId] = 0x00000010;
                    break;
                case ICE: /* switch 2 */
                    g_PlayerSurfaceSoundID[playerId] = 0x00000014;
                    break;
                case ROPE_BRIDGE: /* switch 2 */
                    g_PlayerSurfaceSoundID[playerId] = 0x00000019;
                    break;
                case WOOD_BRIDGE: /* switch 2 */
                    g_PlayerSurfaceSoundID[playerId] = 0x0000001A;
                    break;
                case TRAIN_TRACK: /* switch 2 */
                    g_PlayerSurfaceSoundID[playerId] = 0x0000001B;
                    break;
            }
        }
        switch (gPlayers[playerId].tire_RL.Status) { /* switch 3 */
            case GRASS:                                                  /* switch 3 */
                if (g_PlayerSurfaceSoundID[playerId] == 6) {
                    g_PlayerSurfaceSoundID[playerId] = 4;
                } else if (D_800EA1C0 == 0) {
                    g_PlayerSurfaceSoundID[playerId] = 5;
                } else {
                    g_PlayerSurfaceSoundID[playerId] = 4;
                }
                break;
            case SAND_OFFROAD: /* switch 3 */
                if (g_PlayerSurfaceSoundID[playerId] == 0x0000000C) {
                    g_PlayerSurfaceSoundID[playerId] = 0x0000000A;
                } else if (D_800EA1C0 == 0) {
                    g_PlayerSurfaceSoundID[playerId] = 0x0000000B;
                } else {
                    g_PlayerSurfaceSoundID[playerId] = 0x0000000A;
                }
                break;
            case DIRT_OFFROAD: /* switch 3 */
                if (g_PlayerSurfaceSoundID[playerId] == 0x00000018) {
                    g_PlayerSurfaceSoundID[playerId] = 0x00000016;
                } else if (D_800EA1C0 == 0) {
                    g_PlayerSurfaceSoundID[playerId] = 0x00000017;
                } else {
                    g_PlayerSurfaceSoundID[playerId] = 0x00000016;
                }
                break;
            case SNOW_OFFROAD: /* switch 3 */
                if (g_PlayerSurfaceSoundID[playerId] == 0x0000001F) {
                    g_PlayerSurfaceSoundID[playerId] = 0x0000001D;
                } else if (D_800EA1C0 == 0) {
                    g_PlayerSurfaceSoundID[playerId] = 0x0000001E;
                } else {
                    g_PlayerSurfaceSoundID[playerId] = 0x0000001D;
                }
                break;
            case WET_SAND: /* switch 3 */
                if (asm_DisableEngineSound[playerId] == 0) {
                    if (g_PlayerSurfaceSoundID[playerId] == 9) {
                        g_PlayerSurfaceSoundID[playerId] = 7;
                    } else if (D_800EA1C0 == 0) {
                        g_PlayerSurfaceSoundID[playerId] = 8;
                    } else {
                        g_PlayerSurfaceSoundID[playerId] = 7;
                    }
                } else {
                    g_PlayerSurfaceSoundID[playerId] = 0x0000001C;
                }
                break;
            case ROPE_BRIDGE: /* switch 3 */
                g_PlayerSurfaceSoundID[playerId] = 0x00000019;
                break;
            case WOOD_BRIDGE: /* switch 3 */
                g_PlayerSurfaceSoundID[playerId] = 0x0000001A;
                break;
            case TRAIN_TRACK: /* switch 3 */
                g_PlayerSurfaceSoundID[playerId] = 0x0000001B;
                break;
        }
        switch (gPlayers[playerId].tire_RR.Status) { /* switch 4 */
            case GRASS:                                                   /* switch 4 */
                if (g_PlayerSurfaceSoundID[playerId] == 5) {
                    g_PlayerSurfaceSoundID[playerId] = 4;
                } else if (D_800EA1C0 == 0) {
                    g_PlayerSurfaceSoundID[playerId] = 6;
                } else {
                    g_PlayerSurfaceSoundID[playerId] = 4;
                }
                break;
            case SAND_OFFROAD: /* switch 4 */
                if (g_PlayerSurfaceSoundID[playerId] == 0x0000000B) {
                    g_PlayerSurfaceSoundID[playerId] = 0x0000000A;
                } else if (D_800EA1C0 == 0) {
                    g_PlayerSurfaceSoundID[playerId] = 0x0000000C;
                } else {
                    g_PlayerSurfaceSoundID[playerId] = 0x0000000A;
                }
                break;
            case DIRT_OFFROAD: /* switch 4 */
                if (g_PlayerSurfaceSoundID[playerId] == 0x00000017) {
                    g_PlayerSurfaceSoundID[playerId] = 0x00000016;
                } else if (D_800EA1C0 == 0) {
                    g_PlayerSurfaceSoundID[playerId] = 0x00000018;
                } else {
                    g_PlayerSurfaceSoundID[playerId] = 0x00000016;
                }
                break;
            case SNOW_OFFROAD: /* switch 4 */
                if (g_PlayerSurfaceSoundID[playerId] == 0x0000001E) {
                    g_PlayerSurfaceSoundID[playerId] = 0x0000001D;
                } else if (D_800EA1C0 == 0) {
                    g_PlayerSurfaceSoundID[playerId] = 0x0000001F;
                } else {
                    g_PlayerSurfaceSoundID[playerId] = 0x0000001D;
                }
                break;
            case WET_SAND: /* switch 4 */
                if (asm_DisableEngineSound[playerId] == 0) {
                    if (g_PlayerSurfaceSoundID[playerId] == 8) {
                        g_PlayerSurfaceSoundID[playerId] = 7;
                    } else if (D_800EA1C0 == 0) {
                        g_PlayerSurfaceSoundID[playerId] = 9;
                    } else {
                        g_PlayerSurfaceSoundID[playerId] = 7;
                    }
                } else {
                    g_PlayerSurfaceSoundID[playerId] = 0x0000001C;
                }
                break;
            case ROPE_BRIDGE: /* switch 4 */
                g_PlayerSurfaceSoundID[playerId] = 0x00000019;
                break;
            case WOOD_BRIDGE: /* switch 4 */
                g_PlayerSurfaceSoundID[playerId] = 0x0000001A;
                break;
            case TRAIN_TRACK: /* switch 4 */
                g_PlayerSurfaceSoundID[playerId] = 0x0000001B;
                break;
        }
        if (((gPlayers[playerId].speed < 0.5f) || ((gPlayers[playerId].slip_flag & N_JUMP) == N_JUMP)) &&
            (g_PlayerSurfaceSoundID[playerId] != 0x0000001C)) {
            g_PlayerSurfaceSoundID[playerId] = 0;
        }
        if ((((gPlayers[playerId].slip_flag & RAPID_ACC) == RAPID_ACC) &&
             ((gPlayers[playerId].flag & NO_CONTROLS) != NO_CONTROLS)) ||
            ((gPlayers[playerId].slip_flag & WHEELSPIN) == WHEELSPIN) ||
            ((gPlayers[playerId].slip_flag & SPIN_L) == SPIN_L) ||
            ((gPlayers[playerId].slip_flag & SPIN_R) == SPIN_R) ||
            ((gPlayers[playerId].handling_flag & SPINOUT_SWERVE) == SPINOUT_SWERVE)) {
            g_PlayerSurfaceSoundID[playerId] = 0x00000012;
        }
        if ((((gPlayers[playerId].slip_flag & SPIN_TURN) == SPIN_TURN) &&
             ((gPlayers[playerId].flag & NO_CONTROLS) != NO_CONTROLS)) ||
            ((gPlayers[playerId].slip_flag & WHEELSPIN) == WHEELSPIN)) {
            g_PlayerSurfaceSoundID[playerId] = 0x00000013;
        }
        if (gPlayers[playerId].brake_time != 0.0f) {
            g_PlayerSurfaceSoundID[playerId] = 3;
        }
    }
}

/* 
   Routine to tell the game which "finish" music to play depending on which mode the player is in (Grand Prix, Time Trials, etc),
   whether the game is in multiplayer or not, and which position they ended up finishing in (1st, 2nd, 3rd, etc)

   Contains a LOT of inlined funcs. 
   Modify if you dare.
*/
void goal_routine(u8 playerId) {
    if (D_800E9EA4[playerId] != 0) {
        if (D_800E9EA4[playerId] < 0x2BC) {
            D_800E9EA4[playerId]++;
        }
        if (D_800E9EA4[playerId] == 2) {
            NaPlyLevelStop(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x28));
            NaPlyLevelStop(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFF, 0x2C));
            NaPlyLevelStop(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x4C));
            Nai_StopObjFlagFx(&D_800E9F7C[playerId].pos, D_800E9E94[playerId]);
            g_PlayerSurfaceSoundID[playerId] = 0;
            switch (g_gameMode) { /* irregular */
                case GRAND_PRIX:
                    D_800EA0EC[playerId] = 2;
                    NAPlyTrgStart(playerId, SOUND_ARG_LOAD(0x19, 0x00, 0xF1, 0x03));
                    if (D_800EA1C0 == 0) {
                        Nai_SeqBufFlagCheck(0x100100FF);
                        Nai_SeqBufFlagCheck(0x110100FF);
                        Nai_StopAllHandleFx(5U);
                        if (gPlayers[playerId].rank == 0) {
                            Na_PlyEng_Stop(playerId);
                            D_800EA0F0 = 2;
                            Na_EnmLevel_Start(playerId);
                            Na_GoalFanfare_Set(SEQ_EVENT_RACE_FINISH_FIRST, SEQ_MENU_RESULTS_SCREEN_WIN);
                        } else if (gPlayers[playerId].rank < 4) {
                            Na_PlyEng_Stop(playerId);
                            D_800EA0F0 = 2;
                            Na_EnmLevel_Start(playerId);
                            Na_GoalFanfare_Set(SEQ_EVENT_RACE_FINISH_OTHER, SEQ_MENU_RESULTS_SCREEN_WIN);
                        } else {
                            Nai_SeqBufFlagCheck(-0x3E9F9C00);
                            Na_GoalFanfare_Set(SEQ_EVENT_RACE_FINISH_LOSE, SEQ_MENU_RESULTS_SCREEN_LOSE);
                        }
                    } else {
                        D_800EA0EC[playerId] = 2;
                        NAPlyTrgStart(playerId, 0x1900F103U);
                        if (gPlayers[playerId].rank == 0) {
                            Nai_SeqBufFlagCheck(0x100100FF);
                            Nai_SeqBufFlagCheck(0x110100FF);
                            Na_PlyEng_Stop(playerId);
                            D_800EA0F0 = 2;
                            Na_EnmLevel_Start(playerId);
                            Na_GoalFanfare_Set(SEQ_EVENT_RACE_FINISH_FIRST, SEQ_MENU_RESULTS_SCREEN_WIN);
                        } else if (gPlayers[playerId].rank < 4) {
                            if (D_800EA104 == 0) {
                                Nai_SeqBufFlagCheck(0x100100FF);
                                Nai_SeqBufFlagCheck(0x110100FF);
                            }
                            Na_PlyEng_Stop(playerId);
                            D_800EA0F0 = 2;
                            Na_EnmLevel_Start(playerId);
                            Na_GoalFanfare_Set(SEQ_EVENT_RACE_FINISH_OTHER, SEQ_MENU_RESULTS_SCREEN_WIN);
                        } else if (D_800EA104 == 0) {
                            Nai_SeqBufFlagCheck(0x100100FF);
                            Nai_SeqBufFlagCheck(0x110100FF);
                            Nai_SeqBufFlagCheck(-0x3E9F9C00);
                            Na_GoalFanfare_Set(SEQ_EVENT_RACE_FINISH_LOSE, SEQ_MENU_RESULTS_SCREEN_LOSE);
                        }
                        if ((D_800EA104 != 0) || (D_800EA0EC[playerId] != 1)) {
                            Nai_StopAllHandleFx(5U);
                        }
                    }
                    break;
                case TIME_TRIALS:
                    Nai_SeqBufFlagCheck(0x100100FF);
                    Nai_SeqBufFlagCheck(0x110100FF);
                    Na_PlyEng_Stop(playerId);
                    D_800EA0F0 = 2;
                    Na_EnmLevel_Start(0U);
                    D_800EA0EC[playerId] = 2;
                    NAPlyTrgStart(playerId, 0x1900F103U);
                    if (D_801657E5 == 1) {
                        Na_GoalFanfare_Set(SEQ_EVENT_RACE_FINISH_FIRST, SEQ_MENU_RESULTS_SCREEN_WIN);
                    } else if (rankin_fanfare == 1) {
                        Na_GoalFanfare_Set(SEQ_EVENT_RACE_FINISH_OTHER, SEQ_MENU_RESULTS_SCREEN_WIN);
                    } else {
                        Nai_SeqBufFlagCheck(0x01640010);
                    }
                    break;
                case VERSUS:
                    D_800EA0EC[playerId] = 2;
                    NAPlyTrgStart(playerId, 0x1900F103U);
                    switch (D_800EA1C0) { /* switch 1; irregular */
                        case 1:           /* switch 1 */
                            Nai_SeqBufFlagCheck(0x100100FF);
                            Nai_SeqBufFlagCheck(0x110100FF);
                            Na_PlyEng_Stop(playerId);
                            D_800EA0F0 = 2;
                            Na_EnmLevel_Start(playerId);
                            Na_GoalFanfare_Set(SEQ_EVENT_RACE_FINISH_FIRST, SEQ_MENU_RESULTS_SCREEN_WIN_VS);
                            break;
                        case 2: /* switch 1 */
                            if ((D_800EA104 == 0) && (D_800EA0EC[playerId] == 1)) {
                                Nai_SeqBufFlagCheck(0x100100FF);
                                Nai_SeqBufFlagCheck(0x110100FF);
#ifdef VERSION_EU
                                NaFanStart(SEQ_EVENT_RACE_FINISH_FIRST);
#else
                                Na_GoalFanfare_Set(SEQ_EVENT_RACE_FINISH_FIRST, SEQ_MENU_RESULTS_SCREEN_WIN_VS);
#endif
                                D_800EA104 = 1;
                            } else if ((D_800EA104 == 1) && (D_800EA0EC[playerId] == 1)) {
                                Nai_StopAllHandleFx(5U);
#ifndef VERSION_EU
                                if (NAIGetPlayingSeqFlag(1) != 0x000D)
#endif
                                {
                                    D_800EA104 = 0;
                                    Na_GoalFanfare_Set(SEQ_EVENT_RACE_FINISH_OTHER, SEQ_MENU_RESULTS_SCREEN_WIN_VS);
                                }
                                D_800EA104 = 2;
                            }
                            break;
                        case 3: /* switch 1 */
                            if ((D_800EA104 == 0) && (D_800EA0EC[playerId] == 1)) {
                                Nai_SeqBufFlagCheck(0x100100FF);
                                Nai_SeqBufFlagCheck(0x110100FF);
                                NaFanStart(SEQ_EVENT_RACE_FINISH_FIRST);
                                D_800EA104 = 1;
                            } else if ((D_800EA104 == 1) && (D_800EA0EC[playerId] == 1)) {
                                if (NAIGetPlayingSeqFlag(1) != 0x000D) {
                                    D_800EA104 = 0;
                                    NaFanStart(SEQ_EVENT_RACE_FINISH_OTHER);
                                }
                                D_800EA104 = 2;
                            } else if ((D_800EA104 == 2) && (D_800EA0EC[playerId] == 1)) {
                                Nai_StopAllHandleFx(5U);
#ifndef VERSION_EU
                                if (NAIGetPlayingSeqFlag(1) != 0x000E)
#endif
                                {
                                    D_800EA104 = 0;
                                    Na_GoalFanfare_Set(SEQ_EVENT_RACE_FINISH_OTHER, SEQ_MENU_RESULTS_SCREEN_WIN_VS);
                                }
                                D_800EA104 = 3;
                            }
                            break;
                    }
                    break;
                case BATTLE:
                    switch (D_800EA1C0) { /* switch 2; irregular */
                        case 1:           /* switch 2 */
                            Nai_SeqBufFlagCheck(0x100100FF);
                            Nai_SeqBufFlagCheck(0x110100FF);
                            Nai_StopAllHandleFx(5U);
                            NaPlyLevelStop(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xF9, 0x26));
                            NaFanStart(SEQ_MENU_RESULTS_SCREEN_WIN_VS);
                            D_800EA0EC[playerId] = 2;
                            NAPlyVoiceStart(playerId, (gPlayers[win_kart].kart * 0x10) +
                                                        SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                            break;
                        case 2: /* switch 2 */
                            if ((D_800EA0EC[0] == 1) && (D_800EA0EC[1] == 1) && (D_800EA0EC[2] == 1)) {
                                Nai_StopAllHandleFx(5U);
                                NaPlyLevelStop(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0x80, 0x26));
                                NaFanStart(SEQ_MENU_RESULTS_SCREEN_WIN_VS);
                                D_800EA0EC[playerId] = 2;
                                NAPlyVoiceStart(playerId, (gPlayers[win_kart].kart * 0x10) +
                                                            SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                            }
                            break;
                        case 3: /* switch 2 */
                            if ((D_800EA0EC[0] == 1) && (D_800EA0EC[1] == 1) && (D_800EA0EC[2] == 1) &&
                                (D_800EA0EC[3] == 1)) {
                                Nai_StopAllHandleFx(5U);
                                NaPlyLevelStop(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0x80, 0x26));
                                NaFanStart(SEQ_MENU_RESULTS_SCREEN_WIN_VS);
                                D_800EA0EC[playerId] = 2;
                                NAPlyVoiceStart(playerId, (gPlayers[win_kart].kart * 0x10) +
                                                            SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                            }
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        if (D_800E9EA4[playerId] == 0x0000001E) {
            switch (g_gameMode) { /* switch 3; irregular */
                case BATTLE:          /* switch 3 */
                    break;
                case GRAND_PRIX: /* switch 3 */
                    if (gPlayers[playerId].rank == 0) {
                        D_800EA0EC[playerId] = 2;
                        NAPlyVoiceStart(playerId,
                                      (gPlayers[playerId].kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x07));
                    } else if (gPlayers[playerId].rank < 4) {
                        D_800EA0EC[playerId] = 2;
                        NAPlyVoiceStart(playerId,
                                      (gPlayers[playerId].kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                    } else {
                        D_800EA0EC[playerId] = 2;
                        NAPlyVoiceStart(playerId,
                                      (gPlayers[playerId].kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x03));
                    }
                    break;
                case VERSUS: /* switch 3 */
                    if (gPlayers[playerId].rank == 0) {
                        D_800EA0EC[playerId] = 2;
                        NAPlyVoiceStart(playerId,
                                      (gPlayers[playerId].kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                    }
                    break;
                case TIME_TRIALS: /* switch 3 */
                    if (D_801657E5 == 1) {
                        D_800EA0EC[playerId] = 2;
                        NAPlyVoiceStart(playerId,
                                      (gPlayers[playerId].kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x07));
                    } else if (rankin_fanfare == (u8) 1) {
                        D_800EA0EC[playerId] = 2;
                        NAPlyVoiceStart(playerId,
                                      (gPlayers[playerId].kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                    }
                    break;
            }
        }
        switch (g_gameMode) { /* switch 4; irregular */
            case GRAND_PRIX:      /* switch 4 */
                if (gPlayers[playerId].rank == 0) {
                    if (D_800E9EA4[playerId] >= 0x15F) {
                        if (D_800E9EA4[playerId] == 0x0000015F) {
                            Na_EnmLevel_Stop(playerId);
                        }
                    } else {
                        D_800EA130[playerId] = (f32) D_800E9EA4[playerId] / 400.0f;
                    }
                } else if (gPlayers[playerId].rank < 4) {
                    if (D_800E9EA4[playerId] >= 0x15F) {
                        if (D_800E9EA4[playerId] == 0x0000015F) {
                            Na_EnmLevel_Stop(playerId);
                        }
                    } else {
                        D_800EA130[playerId] = (f32) D_800E9EA4[playerId] / 400.0f;
                    }
                } else if (D_800E9EA4[playerId] >= 0x12D) {
                    if (D_800E9EA4[playerId] == 0x0000012D) {
                        Na_PlyEng_Stop(playerId);
                    }
                } else {
                    D_800EA130[playerId] = (f32) D_800E9EA4[playerId] / 330.0f;
                }
                break;
            case VERSUS: /* switch 4 */
                if (D_800EA1C0 == (u8) 1) {
                    if (D_800E9EA4[playerId] >= 0x65) {
                        if (D_800E9EA4[playerId] == 0x00000065) {
                            Na_EnmLevel_Stop(playerId);
                        }
                    } else {
                        D_800EA130[playerId] = (f32) D_800E9EA4[playerId] / 120.0f;
                    }
                } else if (D_800E9EA4[playerId] >= 0x65) {
                    if (D_800E9EA4[playerId] == 0x00000065) {
                        Na_PlyEng_Stop(playerId);
                    }
                } else {
                    D_800EA130[playerId] = (f32) D_800E9EA4[playerId] / 120.0f;
                }
                /* fallthrough */
            case TIME_TRIALS: /* switch 4 */
                if (D_800E9EA4[playerId] >= 0x12D) {
                    if (D_800E9EA4[playerId] == 0x0000012D) {
                        Na_EnmLevel_Stop(0U);
                    }
                } else {
                    D_800EA130[playerId] = (f32) D_800E9EA4[playerId] / 350.0f;
                }
                break;
            case BATTLE: /* switch 4 */
                if (D_800E9EA4[playerId] >= 0x65) {
                    if (D_800E9EA4[playerId] == 0x00000065) {
                        Na_PlyEng_Stop(playerId);
                    }
                } else {
                    D_800EA130[playerId] = (f32) D_800E9EA4[playerId] / 120.0f;
                }
                break;
        }
    }
}

void dive_routine(u8 playerId) {
    if ((gPlayers[playerId].water_flag & SUBMERGED) == SUBMERGED) {
        if (asm_DisableEngineSound[playerId] == 0) {
            if ((s32) D_800EA1C0 < 2) {
                NaPlyLevelStop(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xF9, 0x26));
            } else {
                NaPlyLevelStop(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0x80, 0x26));
            }
            Na_PlyEng_Stop(playerId);
            asm_DisableEngineSound[playerId] = 1;
            Na_PlyEng_Start(playerId);
            if (((g_courseID == COURSE_CHOCO_MOUNTAIN) || (g_courseID == COURSE_BOWSER_CASTLE) ||
                 (g_courseID == COURSE_BANSHEE_BOARDWALK) || (g_courseID == COURSE_YOSHI_VALLEY) ||
                 (g_courseID == COURSE_FRAPPE_SNOWLAND) || (g_courseID == COURSE_KOOPA_BEACH) ||
                 (g_courseID == COURSE_ROYAL_RACEWAY) || (g_courseID == COURSE_SHERBET_LAND) ||
                 (g_courseID == COURSE_DK_JUNGLE) || (g_courseID == COURSE_BIG_DONUT)) &&
                (D_800EA0EC[playerId] == 0)) {
                NAIFxFlagEntry((gPlayers[playerId].kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x05),
                           &D_800E9F7C[playerId].pos, playerId, &D_800EA1D4, &D_800EA1D4,
                           (s8*) &D_800E9F7C[playerId].unk_14);
            }
        }
    } else {
        if (asm_DisableEngineSound[playerId] == 1) {
            Na_PlyEng_Stop(playerId);
            asm_DisableEngineSound[playerId] = 2;
            Na_PlyEng_Start(playerId);
            asm_DisableEngineSound[playerId] = 0;
            if ((g_courseID == COURSE_KOOPA_BEACH) && (D_800EA0EC[playerId] == 0)) {
                NAIFxFlagEntry((gPlayers[playerId].kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x08),
                           &D_800E9F7C[playerId].pos, playerId, &D_800EA1D4, &D_800EA1D4,
                           (s8*) &D_800E9F7C[playerId].unk_14);
            }
        }
    }
}

void mycar_thunder_okidoki(u8 playerId) {
    if (((gPlayers[playerId].slip_flag & THUNDER) != THUNDER) && (g_lightningFlagPlayer1[playerId] == 1)) {
        NAPlyVoiceStart(playerId, (gPlayers[playerId].kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x08));
    }
}

void thundered_routine(u8 playerId) {
    if ((gPlayers[playerId].slip_flag & THUNDER) == THUNDER) {
        g_lightningFlagPlayer1[playerId] = 1;
        if (D_800E9F2C[playerId] < 0xFA) {
            D_800E9F2C[playerId]++;
        }
    } else {
        D_800E9F2C[playerId] = 0;
        if (g_lightningFlagPlayer1[playerId] == 1) {
            g_lightningFlagPlayer1[playerId] = 2;
        }
    }
    switch (g_lightningFlagPlayer1[playerId]) { /* irregular */
        case 1:
            if (D_800E9F34[playerId] < 0.7f) {
                D_800E9F34[playerId] += 0.1f;
                D_800E9F54[playerId] += 0.03f;
            }
            break;
        case 2:
            if (D_800E9F34[playerId] > 0.16f) {
                D_800E9F34[playerId] -= 0.15f;
                D_800E9F54[playerId] -= 0.03f;
            } else {
                D_800E9F34[playerId] = 0.0f;
                D_800E9F54[playerId] = 0.0f;
                g_lightningFlagPlayer1[playerId] = 0;
            }
            break;
    }
}

// Checks the same field for all players before doing something?
void thunder_stop_routine(void) {
    if (((u8) g_lightningFlag != 0) && ((gPlayers[0].slip_flag & THUNDER) != THUNDER) &&
        ((gPlayers[1].slip_flag & THUNDER) != THUNDER) &&
        ((gPlayers[2].slip_flag & THUNDER) != THUNDER) &&
        ((gPlayers[3].slip_flag & THUNDER) != THUNDER) &&
        ((gPlayers[4].slip_flag & THUNDER) != THUNDER) &&
        ((gPlayers[5].slip_flag & THUNDER) != THUNDER) &&
        ((gPlayers[6].slip_flag & THUNDER) != THUNDER) &&
        ((gPlayers[7].slip_flag & THUNDER) != THUNDER)) {
        Na_PlyThunder_Stop();
    }
}

void kansei_low_stop_routine(void) {
    if (D_800EA180 != 0) {
        D_800EA180++;
        if (D_800EA180 == 2) {
            D_800EA17C = 1.0f;
        }
        if (D_800EA180 < 0xF) {
            D_800EA178 = 1.0f - (D_800EA180 * 0.012f);
        }
        if ((D_800EA180 >= 0x33) && (D_800EA180 < 0x96)) {
            D_800EA17C = 1.0f - ((D_800EA180 - 0x32) / 110.0f);
        }
        if (D_800EA180 == 0x12D) {
            D_800EA17C = 0.0f;
        }
        if (D_800EA180 == 0x321) {
            D_800EA178 = 1.0f;
            D_800EA17C = 0.85f;
            D_800EA180 = 0;
        }
    }
}

void tunnel_kansei_kinshi(void) {
    if (g_courseID == COURSE_LUIGI_RACEWAY) {
        if (D_800EA184 != 0) {
            if ((u8) g_gamePausedFlag == 0) {
                // Has to be this way, can't be D_800EA184++
                D_800EA184 += 1;
            }
            if (D_800EA184 == 0x012C) {
                D_800EA17C = 0.85f;
                D_800EA184 = 0;
            }
        } else {
            switch (D_800EA1C0) { /* irregular */
                case 0:
                    if (D_800E9F7C[0].unk_14 != 0) {
                        D_800EA17C = 0.0f;
                        D_800EA184 = 1;
                    }
                    break;
                case 1:
                    if ((D_800E9F7C[0].unk_14 != 0) || (D_800E9F7C[1].unk_14 != 0)) {
                        D_800EA17C = 0.0f;
                        D_800EA184 = 1;
                    }
                    break;
                case 2:
                    if ((D_800E9F7C[0].unk_14 != 0) || (D_800E9F7C[1].unk_14 != 0) || (D_800E9F7C[2].unk_14 != 0)) {
                        D_800EA17C = 0.0f;
                        D_800EA184 = 1;
                    }
                    break;
                case 3:
                    if ((D_800E9F7C[0].unk_14 != 0) || (D_800E9F7C[1].unk_14 != 0) || (D_800E9F7C[2].unk_14 != 0) ||
                        (D_800E9F7C[3].unk_14 != 0)) {
                        D_800EA17C = 0.0f;
                        D_800EA184 = 1;
                    }
                    break;
            }
        }
    }
}

void enemy_engine_volume(u8 arg0) {
    D_800EA06C[arg0].unk00[2] = (1.0f - D_800E9F54[arg0]) - D_800EA130[arg0];
}

void make_sound_program() {
    u8 var_s0;

    for (var_s0 = 0; var_s0 < D_800EA1C0 + 1; var_s0++) {
        accel_onoff(var_s0);
        accel_volume(var_s0);
        engine_freq_set(var_s0);
        level_volume_set(var_s0);
        level_freq_set(var_s0);
        level_sound_trigger(var_s0);
        make_level_sound_trg(var_s0);
        goal_routine(var_s0);
        dive_routine(var_s0);
        mycar_thunder_okidoki(var_s0);
    }
    if (g_gameMode == GRAND_PRIX) {
        for (var_s0 = 0; var_s0 < 8; var_s0++) {
            thundered_routine(var_s0);
            enemy_engine_volume(var_s0);
        }
    } else {
        for (var_s0 = 0; var_s0 < D_800EA1C0 + 1; var_s0++) {
            thundered_routine(var_s0);
            enemy_engine_volume(var_s0);
        }
    }
    thunder_stop_routine();
    kansei_low_stop_routine();
    tunnel_kansei_kinshi();
}

void playSound(s32 soundBits) {
    if ((soundBits == SOUND_ACTION_REV_ENGINE) && (g_courseID == COURSE_DK_JUNGLE)) {
        soundBits = SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x27);
    }

    if ((soundBits == SOUND_ACTION_REV_ENGINE_2) && (g_courseID == COURSE_DK_JUNGLE)) {
        soundBits = SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x28);
    }
    NAIFxFlagEntry(soundBits, &D_800EA1C8, 4, &D_800EA1D4, &D_800EA1D4, &D_800EA1DC);
}

void NaSeqStart(u16 index) {
    Nai_SeqBufFlagCheck(index | 0x0010000);
    D_800EA15C = index;
}

void NaFanStart(u16 index) {
    Nai_SeqBufFlagCheck(index | 0x1010000);
    D_800EA160 = index;
}

void NAMusicVolume(u8 arg0) {
    Nai_SetSeqLineVolMove(0, 0, arg0, 1);
}

void NaPlyLevelStart(u8 arg0, u32 soundBits) {
    if (D_800EA108 == 0) {
        switch (D_800EA0EC[arg0]) {
            case 2:
                D_800EA0EC[arg0] = 1;
            case 0:
                NAIFxFlagEntry(soundBits, &D_800E9F7C[arg0].pos, arg0, &D_800EA1D4, &D_800EA1D4, &D_800EA1DC);
                break;
        }
    }
}

void NaPlyLevelStop(u8 playerIndex, u32 soundBits) {
    Nai_StopObjFlagFx(&D_800E9F7C[playerIndex].pos, soundBits);
}

void NAPlyTrgStart(u8 playerId, u32 soundBits) {
    if (D_800EA108 == 0) {
        switch (D_800EA0EC[playerId]) {
            case 2:
                D_800EA0EC[playerId] = 1;
            case 0:
                NAIFxFlagEntry(soundBits, &D_800E9F7C[playerId].pos, playerId, &D_800EA1D4, &D_800EA1D4,
                           (s8*) &D_800E9F7C[playerId].unk_14);
                break;
        }
    }
}

void NAPlyVoiceStart(u8 playerId, u32 soundBits) {
    if (D_800EA108 == 0) {
        switch (D_800EA0EC[playerId]) {
            case 2:
                D_800EA0EC[playerId] = 1;
            case 0:
                if (((soundBits & ~0xF0) == SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x03)) ||
                    ((soundBits & ~0xF0) == SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x04)) ||
                    ((soundBits & ~0xF0) == SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x05))) {
                    D_800EA180 = 1;
                }
                if (((gPlayers[playerId].slip_flag & THUNDER) == THUNDER) &&
                    ((s32) D_800E9F2C[playerId] >= 0x1F)) {
                    NAIFxFlagEntry(soundBits, &D_800E9F7C[playerId].pos, playerId, &D_800EA150, &D_800EA1D4,
                               (s8*) &D_800E9F7C[playerId].unk_14);
                } else {
                    NAIFxFlagEntry(soundBits, &D_800E9F7C[playerId].pos, playerId, &D_800EA1D4, &D_800EA1D4,
                               (s8*) &D_800E9F7C[playerId].unk_14);
                }
                break;
            default:
                break;
        }
    }
}

void Na_PlyDash_Start(u8 playerIndex) {
    NAPlyVoiceStart(playerIndex, (gPlayers[playerIndex].kart * 0x10) + (gAudioRandom & 1) +
                                   SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x01));
}

void Na_EnmVoice_Start(u8 playerId, u32 soundBits) {
    u8 var_s0;
    struct Unk_8018EFD8* temp_v0;

    if ((D_800EA108 == 0) && (D_800EA0F0 == 0) && ((s32) D_800EA1C0 < 2)) {
        for (var_s0 = 0; var_s0 < D_800EA1C0 + 1; var_s0++) {
            temp_v0 = Naa_Calcdoppler(playerId, gPlayers[playerId].position, gPlayers[playerId].velocity, D_800EA1C8,
                                    (u8) var_s0, soundBits);
            if (temp_v0 != NULL) {
                temp_v0->unk34 = 170.0f;
                if (((gPlayers[playerId].slip_flag & THUNDER) == THUNDER) &&
                    ((s32) D_800E9F2C[playerId] >= 0x1F)) {
                    NAIFxFlagEntry((gPlayers[playerId].kart * 0x10) + soundBits, &temp_v0->unk18, var_s0,
                               &D_800EA150, &D_800EA1D4, (s8*) &D_800EA06C[playerId].unk0C);
                } else {
                    NAIFxFlagEntry((gPlayers[playerId].kart * 0x10) + soundBits, &temp_v0->unk18, var_s0,
                               &temp_v0->unk2C, &D_800EA1D4, (s8*) &D_800EA06C[playerId].unk0C);
                }
            }
        }
    }
}

void Na_PlyEng_Start(u8 playerId) {
    u32 var_a0;

    if (D_800EA108 == 0) {
        switch (D_800EA0EC[playerId]) {
            case 2:
                D_800EA0EC[playerId] = 1;
            case 0:
                D_800E9F7C[playerId].unk_0C = 1.0f;
                D_800E9F7C[playerId].unk_10 = 0.0f;
                switch (gPlayers[playerId].kart) {
                    case 0:
                    case 1:
                        D_800E9F7C[playerId].unk_18 = 2.8f;
                        D_800E9F7C[playerId].unk_1C = 3844.0f;
                        D_800E9F7C[playerId].unk_20 = 0.35f;
                        D_800E9F7C[playerId].unk_24 = -0.8f;
                        D_800E9F7C[playerId].unk_28 = 0.35f;
                        D_800E9F7C[playerId].unk_2C = 1568.9796f;
                        D_800E9F7C[playerId].unk_30 = 1067.7778f;
                        D_800E9F7C[playerId].unk_34 = 2766.065f;
                        break;
                    case 2:
                    case 6:
                        D_800E9F7C[playerId].unk_18 = 3.2f;
                        D_800E9F7C[playerId].unk_1C = 3844.0f;
                        D_800E9F7C[playerId].unk_20 = 0.6f;
                        D_800E9F7C[playerId].unk_24 = -1.7f;
                        D_800E9F7C[playerId].unk_28 = 0.6f;
                        D_800E9F7C[playerId].unk_2C = 1478.4615f;
                        D_800E9F7C[playerId].unk_30 = 784.4898f;
                        D_800E9F7C[playerId].unk_34 = 12813.335f;
                        break;
                    case 3:
                        D_800E9F7C[playerId].unk_18 = 2.8f;
                        D_800E9F7C[playerId].unk_1C = 3844.0f;
                        D_800E9F7C[playerId].unk_20 = 0.6f;
                        D_800E9F7C[playerId].unk_24 = -0.6f;
                        D_800E9F7C[playerId].unk_28 = 0.6f;
                        D_800E9F7C[playerId].unk_2C = 1747.2728f;
                        D_800E9F7C[playerId].unk_30 = 1130.5883f;
                        D_800E9F7C[playerId].unk_34 = 3844.001f;
                        break;
                    case 4:
                        D_800E9F7C[playerId].unk_18 = 2.0f;
                        D_800E9F7C[playerId].unk_1C = 3844.0f;
                        D_800E9F7C[playerId].unk_20 = 0.2f;
                        D_800E9F7C[playerId].unk_24 = -0.4f;
                        D_800E9F7C[playerId].unk_28 = 0.2f;
                        D_800E9F7C[playerId].unk_2C = 2135.5557f;
                        D_800E9F7C[playerId].unk_30 = 1601.6666f;
                        D_800E9F7C[playerId].unk_34 = 3203.333f;
                        break;
                    case 5:
                    case 7:
                        D_800E9F7C[playerId].unk_18 = 2.4f;
                        D_800E9F7C[playerId].unk_1C = 3844.0f;
                        D_800E9F7C[playerId].unk_20 = 0.4f;
                        D_800E9F7C[playerId].unk_24 = -0.8f;
                        D_800E9F7C[playerId].unk_28 = 0.4f;
                        D_800E9F7C[playerId].unk_2C = 1922.0f;
                        D_800E9F7C[playerId].unk_30 = 1201.25f;
                        D_800E9F7C[playerId].unk_34 = 4805.0f;
                        break;
                }
                var_a0 = gPlayers[playerId].kart + 0x0104FF00;
                switch (asm_DisableEngineSound[playerId]) { /* switch 1; irregular */
                    case 0:                     /* switch 1 */
                        if (D_800EA1C0 != 0) {
                            var_a0 += 0x14;
                        }
                        break;
                    case 1: /* switch 1 */
                        var_a0 += 0x2E;
                        break;
                    case 2: /* switch 1 */
                        if (D_800EA1C0 == 0) {
                            var_a0 += 0x36;
                        } else {
                            var_a0 += 0x3E;
                        }
                        break;
                }
                NAIFxFlagEntry(var_a0, &D_800E9F7C[playerId].pos, playerId, &D_800E9F7C[playerId].unk_0C,
                           &D_800E9F7C[playerId].unk_10, (s8*) &D_800E9F7C[playerId].unk_14);
                break;
            default:
                break;
        }
    }
}

void Na_PlyEng_Stop(u8 arg0) {
    Nai_StopObjFlagFx(&D_800E9F7C[arg0].pos, gPlayers[arg0].kart + SOUND_ARG_LOAD(0x01, 0x04, 0xFF, 0x00));
    Nai_StopObjFlagFx(&D_800E9F7C[arg0].pos, gPlayers[arg0].kart + SOUND_ARG_LOAD(0x01, 0x04, 0xFF, 0x14));
    Nai_StopObjFlagFx(&D_800E9F7C[arg0].pos, gPlayers[arg0].kart + SOUND_ARG_LOAD(0x01, 0x04, 0xFF, 0x2E));
    Nai_StopObjFlagFx(&D_800E9F7C[arg0].pos, gPlayers[arg0].kart + SOUND_ARG_LOAD(0x01, 0x04, 0xFF, 0x36));
    Nai_StopObjFlagFx(&D_800E9F7C[arg0].pos, gPlayers[arg0].kart + SOUND_ARG_LOAD(0x01, 0x04, 0xFF, 0x3E));
}

void NAEnmTrgStart(Vec3f position, Vec3f velocity, u32 soundBits) {
    u8 var_s0;
    struct Unk_8018EFD8* temp_v0;

    if ((D_800EA108 == 0) && (D_800EA0F0 == 0)) {
        for (var_s0 = 0; var_s0 < (D_800EA1C0 + 1); var_s0++) {
            temp_v0 = Naa_Calcdoppler(0U, position, velocity, D_800EA1C8, (u8) var_s0, soundBits);
            if (temp_v0 != 0) {
                temp_v0->unk34 = 170.0f;
                NAIFxFlagEntry(soundBits, &temp_v0->unk18, var_s0, &temp_v0->unk2C, &D_800EA1D4, &D_800EA1DC);
            }
        }
    }
}

void NAEnmTrgStop(Vec3f arg0, s32 soundBits) {
    Vec3f* temp_v0;
    u8 temp_t9;

    if (D_800EA108 == 0) {
        for (temp_t9 = 0; temp_t9 < D_800EA1C0 + 1; temp_t9++) {
            temp_v0 = Naa_InitApliWork(arg0, soundBits);
            if (temp_v0 != NULL) {
                Nai_StopObjFlagFx(temp_v0, soundBits);
            }
        }
    }
}

void Na_EnmLevel_Start(u8 playerId) {
    u8 var_s0;
    u32 soundBits;
    struct Unk_8018EFD8* temp_v0_6;

    if (D_800EA108 == 0) {
        switch (D_800EA0F0) {
            case 2:
                D_800EA0F0 = 1;
                /* fallthrough */
            case 0:
                switch (gPlayers[playerId].kart) {
                    case 0:
                    case 1:
                        D_800EA06C[playerId].unk00[0] = 0.35f;
                        D_800EA06C[playerId].unk00[1] = 1568.9796f;
                        break;
                    case 2:
                    case 6:
                        D_800EA06C[playerId].unk00[0] = 0.6f;
                        D_800EA06C[playerId].unk00[1] = 1478.4615f;
                        break;
                    case 3:
                        D_800EA06C[playerId].unk00[0] = 0.6f;
                        D_800EA06C[playerId].unk00[1] = 1747.2728f;
                        break;
                    case 4:
                        D_800EA06C[playerId].unk00[0] = 0.2f;
                        D_800EA06C[playerId].unk00[1] = 2135.5557f;
                        break;
                    case 5:
                    case 7:
                        D_800EA06C[playerId].unk00[0] = 0.4f;
                        D_800EA06C[playerId].unk00[1] = 1922.0f;
                }
                if (D_800EA1C0 < 2) {
                    for (var_s0 = 0; var_s0 < D_800EA1C0 + 1; var_s0++) {
                        soundBits = gPlayers[playerId].kart + SOUND_ARG_LOAD(0x31, 0x02, 0x80, 0x00);
                        temp_v0_6 = Naa_Calcdoppler(playerId, gPlayers[playerId].position, gPlayers[playerId].velocity,
                                                  &gPlayers[playerId].taco, var_s0, soundBits);
                        if (temp_v0_6 != NULL) {
                            temp_v0_6->unk34 = 40.0f;
                            NAIFxFlagEntry(soundBits, &temp_v0_6->unk18, var_s0, &temp_v0_6->unk2C,
                                       &D_800EA06C[playerId].unk00[2], (s8*) &D_800EA06C[playerId].unk0C);
                        }
                    }
                }
                break;
            default:
                break;
        }
    }
}

void Na_EnmLevel_Stop(u8 playerId) {
    Nai_StopAllObjFx(
        Naa_InitApliWork(gPlayers[playerId].position, gPlayers[playerId].kart + SOUND_ARG_LOAD(0x31, 0x02, 0x80, 0x00)));
}

void NaSceneLevelStart(Vec3f position, Vec3f velocity, u32 soundBits) {
    u8 var_s0;
    struct Unk_8018EFD8* temp_v0;

    if ((D_800EA108 == 0) && (D_800EA0F0 == 0) && ((s32) D_800EA1C0 < 4)) {
        for (var_s0 = 0; var_s0 < ((D_800EA1C0 + 1)); var_s0++) {
            temp_v0 = Naa_Calcdoppler(0U, position, velocity, D_800EA1C8, (u8) var_s0, soundBits);
            if (temp_v0 != 0) {
                temp_v0->unk34 = 170.0f;
                if (soundBits == SOUND_ARG_LOAD(0x51, 0x03, 0x70, 0x0B)) {
                    NAIFxFlagEntry(soundBits, &temp_v0->unk18, var_s0, &D_800EA178, &D_800EA17C, &D_800EA1DC);
                } else {
                    NAIFxFlagEntry(soundBits, &temp_v0->unk18, var_s0, &temp_v0->unk2C, &D_800EA1D4, &D_800EA1DC);
                }
            }
        }
    }
}

void NaSceneLevelStop(Vec3f arg0, u32 soundBits) {
    Vec3f* temp;
    u8 i;

    for (i = 0; i < D_800EA1C0 + 1; i++) {
        temp = Naa_InitApliWork(arg0, soundBits);
        if (temp != NULL) {
            Nai_StopObjFlagFx(temp, soundBits);
        }
    }
}

void Na_PauseSet(u8 arg0) {
    if ((arg0) != 0) {
        playSound(SOUND_ACTION_GO_BACK_2);
        Nap_SetS32(0xF1000000, 0);
        g_gamePausedFlag = 1;
    } else {
        playSound(SOUND_ACTION_UNKNOWN_CONFIRMATION);
        Nap_SetS32(0xF2000000, 0);
        g_gamePausedFlag = 0;
    }
}

void Na_ChangeSoundMode(u8 arg0, u8 arg1) {
    Nai_SetSeqLineVolMove(0, 0, 0x7F, 1);
    Nai_SetSeqLineVolMove(1, 0, 0x7F, 1);

    if (arg1 >= 4) {
        if ((arg1 == 0xC) || (arg1 == 4)) {
            arg1 = 5;
        } else {
            arg1 = 4;
        }
    }
    Nai_SeqBufFlagCheck((arg0 << 8) | 0xF0000000 | arg1);
}

// With -framepointer active, you CANNOT put void
// in the argument list, causes a minor stack difference
void Na_SeMute_Title() {
    D_800EA108 = 1;
}

void Na_SeMute_Title_Off() {
    D_800EA108 = 0;
}

void Na_SeMute_Hyoushou() {
    D_800EA108 = 1;
}

void All_LevelSE_Stop(void) {
    Nai_StopAllHandleFx(3);
    Nai_StopAllHandleFx(5);
}

void Na_SeMute_Goal(u8 arg0) {
    D_800EA0EC[arg0] = 1;
    D_800E9EA4[arg0] = 1;
    switch (D_800EA1C0) { /* irregular */
        case 0:
            D_800EA0F0 = 1;
            All_LevelSE_Stop();
            break;
        case 1:
            if ((D_800EA0EC[0] == (u8) 1) && (D_800EA0EC[1] == (u8) 1)) {
                D_800EA0F0 = 1;
                All_LevelSE_Stop();
            }
            break;
        case 2:
            if ((D_800EA0EC[0] == (u8) 1) && (D_800EA0EC[1] == (u8) 1) && (D_800EA0EC[2] == (u8) 1)) {
                D_800EA0F0 = 1;
                All_LevelSE_Stop();
            }
            break;
        case 3:
            if ((D_800EA0EC[0] == (u8) 1) && (D_800EA0EC[1] == (u8) 1) && (D_800EA0EC[2] == (u8) 1) &&
                (D_800EA0EC[3] == (u8) 1)) {
                D_800EA0F0 = 1;
                All_LevelSE_Stop();
            }
            break;
    }
}

void Na_SeMute_Off(u8 playerIndex) {
    D_800EA0EC[playerIndex] = 2;
}

void Na_PanDeforume() {
    D_800EA0F4 = 1;
}

void NA_PlyEcho_On(u8 arg0, s8 arg1) {
    D_800E9F7C[arg0].unk_14 = arg1;
}

void NA_PlyEcho_Off(u8 arg0) {
    D_800E9F7C[arg0].unk_14 = 0;
}

void Na_EnmEcho_On(u8 arg0, s8 arg1) {
    D_800EA06C[arg0].unk0C = arg1;
}

void Na_EnmEcho_Off(u8 arg0) {
    D_800EA06C[arg0].unk0C = 0;
}

void NaSeqFadeout(u8 arg0) {
    Nai_SeqBufFlagCheck(arg0 << 0x10 | 0x100000FF);
    Nai_SeqBufFlagCheck(arg0 << 0x10 | 0x110000FF);
}

void Na_SE_Fadeout(u8 arg0) {
    arg0 *= 2;
    Nai_SetFxHandleVolume(0, 0, arg0);
    Nai_SetFxHandleVolume(1, 0, arg0);
    Nai_SetFxHandleVolume(2, 0, arg0);
    Nai_SetFxHandleVolume(3, 0, arg0);
    Nai_SetFxHandleVolume(5, 0, arg0);
}

void Na_GoalFanfare_Set(u16 first, u16 second) {
    if (D_800EA104 == 0) {
        Nai_SeqBufFlagCheck(NAIGetPlayingSeqFlag(0) | 0x30000000);
        Nai_ClearAllSeqFlag(0);
        Nai_SeqBufFlagCheck(second | 0xC1510000);
        Nai_SeqBufFlagCheck(first | 0x01000000);
    }
    D_800EA104 = 1;
}

void Na_Fanfare_Start(u8 playerIndex) {
    if (D_800EA108 == 0) {
        if (D_800EA1C0 >= 2) {
            NAPlyTrgStart(playerIndex, 0x1900FF3A);
        } else if (D_800EA164 != 0) {
            Nai_SeqBufFlagCheck(0x100100FF); // 0x19000000
            Nai_SeqBufFlagCheck(0x110100FF);
            NaFanStart(SEQ_EVENT_RACE_FINAL_LAP);
            Nai_SeqBufFlagCheck(0xC1510011);
        } else {
            Nai_SeqBufFlagCheck(0x100100FF); // 0x19000000
            Nai_SeqBufFlagCheck(0x110100FF);
            NaFanStart(SEQ_EVENT_RACE_FINAL_LAP);
            Nai_SeqBufFlagCheck(D_800EA15C | 0xC1500000);
            Nai_SeqBufFlagCheck(0xC130017D);
        }
        D_8018FC08 = D_8018FC08 + 1;
    }
}

void Na_StarBGM_Start(u8 playerId) {
    if ((D_800EA0EC[playerId] == 0) && (D_800EA108 == 0)) {
        NAIFxFlagEntry((gPlayers[playerId].kart * 0x10) + 0x29008001, &D_800E9F7C[playerId].pos, playerId,
                   &D_800EA1D4, &D_800EA1D4, (s8*) &D_800E9F7C[playerId].unk_14);
        D_800EA164 = 1;
        if ((s32) D_800EA1C0 >= 2) {
            NaPlyLevelStart(playerId, 0x0100FF2C);
        } else {
            Nai_SeqBufFlagCheck(0x100100FF);
            if (D_800EA104 != 0) {
                NaPlyLevelStart(playerId, 0x0100FF2C);
            } else if (D_8018FC08 != 0) {
                if ((NAIGetPlayingSeqFlag(1) == 0x000C) || (Nai_CheckSeqFlagBuffer2(0x0101000C) == 0)) {
                    Nai_SeqBufFlagCheck(0xC1F00000);
                    Nai_SeqBufFlagCheck(0xC1510011);
                } else {
                    NaFanStart(SEQ_EVENT_RACE_POWERUP_STAR);
                }
            } else {
                if (1) {} // ?
                NaFanStart(SEQ_EVENT_RACE_POWERUP_STAR);
            }
        }
        D_800EA10C[playerId] = 1;
    }
}

void Na_StarBGM_Stop(u8 playerIndex) {
    if (D_800EA0EC[playerIndex] == 0) {
        if ((D_800EA108 == 0) && (D_800EA10C[playerIndex] != 0)) {
            NAIFxFlagEntry(gPlayers[playerIndex].kart * 0x10 + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x08),
                       &D_800E9F7C[playerIndex].pos, playerIndex, &D_800EA1D4, &D_800EA1D4,
                       (s8*) &D_800E9F7C[playerIndex].unk_14);
            if (D_800EA10C[playerIndex] != 0) {
                if ((s32) D_800EA1C0 >= 2) {
                    NaPlyLevelStop(playerIndex, SOUND_ARG_LOAD(0x01, 0x00, 0xFF, 0x2C));
                } else {
                    D_800EA10C[playerIndex] = 0;
                    if (D_800EA104 != 0) {
                        NaPlyLevelStop(playerIndex, SOUND_ARG_LOAD(0x01, 0x00, 0xFF, 0x2C));
                    } else if ((D_800EA10C[0] == 0) && (D_800EA10C[1] == 0)) {
                        if (D_8018FC08 != 0) {
                            if (((u32) (SeqPlayer[1].enabled)) == 0) {
                                Nai_ClearSeqECheckFlag(1, 5);
                                NaSeqStart(D_800EA15C);
                                Nai_SeqBufFlagCheck(0xB001307DU);
                            } else if ((NAIGetPlayingSeqFlag(1) == 0xC) || (Nai_CheckSeqFlagBuffer2(0x0101000C) == 0)) {
                                Nai_SeqBufFlagCheck(0xC1F00000U);
                                Nai_SeqBufFlagCheck(D_800EA15C | 0xC1500000);
                                Nai_SeqBufFlagCheck(0xC130017DU);
                            } else {
                                Nai_SeqBufFlagCheck(0x110100FFU);
                                NaSeqStart(D_800EA15C);
                                Nai_SeqBufFlagCheck(0xB001307DU);
                            }
                        } else {
                            Nai_SeqBufFlagCheck(0x110100FFU);
                            NaSeqStart(D_800EA15C);
                        }
                    }
                    D_800EA164 = 0;
                }
            }
            D_800EA10C[playerIndex] = 0;
        }
    }
}

void Na_EnmMuteki_Start(u8 playerIndex) {
    u8 i;
    struct Unk_8018EFD8* temp_v0_2;

    if ((D_800EA108 == 0) && (D_800EA0F0 == 0)) {
        for (i = 0; i < D_800EA1C0 + 1; i++) {
            temp_v0_2 = Naa_Calcdoppler(playerIndex, gPlayers[playerIndex].position, D_800EA1C8,
                                      &gPlayers[playerIndex].taco, (u8) i, SOUND_ITEM_STAR);
            if (temp_v0_2) {
                NAIFxFlagEntry(SOUND_ITEM_STAR, &temp_v0_2->unk18, i, &D_800EA1D4, &D_800EA1D4, &D_800EA1DC);
            }
        }
    }
}

void Na_EnmMuteki_Stop(u8 playerId) {
    if ((u8) D_800EA108 == 0) {
        Nai_StopObjFlagFx(Naa_InitApliWork(gPlayers[playerId].position, SOUND_ITEM_STAR), SOUND_ITEM_STAR);
    }
}

void Na_PlyThunder_Start(u8 playerIndex) {
    UNUSED u8* temp_v1;
    UNUSED u8 temp_v0;

    if (D_800EA108 == 0) {
        switch (D_800EA0EC[playerIndex]) {
            case 2:
                D_800EA0EC[playerIndex] = 1;
            case 0:
                Nai_SetSeqLineVolMove(0, 1U, 0x55U, 5);
                NAPlyTrgStart(playerIndex, 0x1900F013U);
                NAIFxFlagEntry(SOUND_ITEM_THUNDERBOLT, &D_800EA1C8, 0U, &D_800EA1D4, &D_800EA1D4, &D_800EA1DC);
                break;
            default:
                break;
        }
        g_lightningFlag = 1;
    }
}

void Na_PlyThunder_Stop() {
    if (D_800EA108 == 0) {
        Nai_SetSeqLineVolMove(0, 1U, 0x7FU, 0x19);
        Nai_StopFx(SOUND_ITEM_THUNDERBOLT);
        g_lightningFlag = 0;
    }
}

void Na_EnmThunder_Start(UNUSED u8 arg0) {
    if ((D_800EA108 == 0) && (D_800EA0F0 == 0)) {
        NAIFxFlagEntry(SOUND_ACTION_EXPLOSION_2, &D_800EA1C8, 0U, &D_800EA1D4, &D_800EA1D4, &D_800EA1DC);
        if ((D_800EA10C[0] != 1) && (D_800EA10C[1] != 1)) {
            Nai_SetSeqLineVolMove(0, 1, 0x37U, 5);
            NAIFxFlagEntry(SOUND_ITEM_THUNDERBOLT, &D_800EA1C8, 0U, &D_800EA1D4, &D_800EA1D4, &D_800EA1DC);
            g_lightningFlag = 1;
        }
    }
}

void Na_EnmThunder_Stop(UNUSED s32 arg0) {
    if (D_800EA108 == 0) {
        if ((D_800EA170[0] == 0) && (D_800EA170[1] == 0) && (D_800EA170[2] == 0) && (D_800EA170[3] == 0)) {
            Nai_SetSeqLineVolMove(0, 1, 0x7FU, 0x19);
        }
        Nai_StopFx(SOUND_ITEM_THUNDERBOLT);
        g_lightningFlag = 0;
    }
}

void NaPlyLandStart(u8 playerIndex, f32 arg1) {
    if (D_800EA108 == 0) {
        switch (D_800EA0EC[playerIndex]) {
            case 2:
                D_800EA0EC[playerIndex] = 1;
                // Purposeful fallthrough
            case 0:
                arg1 = (arg1 * 0.55f) + 0.45f;
                if (arg1 > 1.0f) {
                    arg1 = 1.0f;
                }
                if (arg1 < 0.0f) {
                    arg1 = 0.0f;
                }
                D_800EA110[playerIndex] = arg1;
                NAIFxFlagEntry(0x1900A209U, &D_800E9F7C[playerIndex].pos, playerIndex, &D_800EA1D4,
                           &D_800EA110[playerIndex], (s8*) &D_800E9F7C[playerIndex].unk_14);
                break;
            default:
                break;
        }
    }
}

void Na_PlyCrush_Start(u8 playerId, f32 arg1) {
    if (D_800EA108 == 0) {
        switch (D_800EA0EC[playerId]) {
            case 2:
                D_800EA0EC[playerId] = 1;
                // Purposeful fallthrough
            case 0:
                arg1 = (arg1 * 0.7f) + 0.1f;
                if (arg1 > 1.0f) {
                    arg1 = 1.0f;
                }
                if (arg1 < 0.0f) {
                    arg1 = 0.0f;
                }
                D_800EA120[playerId] = arg1;
                NAIFxFlagEntry(g_courseID + 0x19007020, &D_800E9F7C[playerId].pos, playerId, &D_800EA1D4,
                           &D_800EA120[playerId], (s8*) &D_800E9F7C[playerId].unk_14);
                break;
            default:
                break;
        }
    }
}

void Na_PlyObake_Start(u8 playerIndex) {
    if (D_800EA108 == 0) {
        switch (D_800EA0EC[playerIndex]) {
            case 2:
                D_800EA0EC[playerIndex] = 1;
            case 0:
                Nai_SetSeqLineVolMove(0, 1, 0x55U, 5);
                NAPlyTrgStart(playerIndex, 0x19009E59U);
                NaPlyLevelStart(playerIndex, 0x0100FA4C);
                D_800EA170[playerIndex] = 1;
                break;
        }
    }
}

void Na_PlyObake_Stop(u8 playerIndex) {
    if (D_800EA108 == 0) {
        if (D_800EA170[playerIndex] == 1) {
            if ((u8) g_lightningFlag == 0) {
                Nai_SetSeqLineVolMove(0, 1U, 0x7FU, 0x19);
            }
            NAPlyVoiceStart(playerIndex,
                          gPlayers[playerIndex].kart * 0x10 + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x08));
            NaPlyLevelStop(playerIndex, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x4C));
            D_800EA170[playerIndex] = 0;
        }
    }
}

void Na_Fourth_Init() {
    D_800EA174 = 1;
}

/*
   Play the entire "losing" sequence for the podium ceremony.

   Increments a timer value by one per frame in order to tell which part of the sequence to play at which point.
*/
void Na_Fourth_Routine() {
    if (D_800EA174 != 0) {
        D_800EA174++;

        if (D_800EA174 == 3) {
            NaSeqStart(SEQ_EVENT_CEREMONY_PRESENTATION_PART1); // Begin with Part 1 of the ceremony presentation music --- "Everything seems normal..."
            Nai_SeqBufFlagCheck(0x4000007F);
        }

        if (D_800EA174 == 300) {
            NaSeqStart(SEQ_EVENT_CEREMONY_PRESENTATION_PART2_WIN); // Follow up with Part 2 of the winning ceremony presentation music --- "Looks like I won...?"
            Nai_SeqBufFlagCheck(0x4000007F);
            NaFanStart(SEQ_EVENT_CEREMONY_PRESENTATION_PART2_LOSE); // Once it ends, begin the LOSING ceremony presentation music, which has a few notes changed to be off-tune --- "Uh oh..."
            Nai_SeqBufFlagCheck(0x41000000);
        }

        if (D_800EA174 == 560) { // (Somewhere in this code it slows down the music, changes the pitch of it and completely breaks the music)
            Nai_SeqBufFlagCheck(0x40640000);
            Nai_SeqBufFlagCheck(0xB0640073);
            Nai_SeqBufFlagCheck(0x4150007F);
            Nai_SeqBufFlagCheck(0xB1640073);
        }

        if (D_800EA174 == 680) {
            Nai_SeqBufFlagCheck(0x100100FF);
        }

        if (D_800EA174 == 1050) {
            Nai_SeqBufFlagCheck(0xB1500001);
            Nai_SeqBufFlagCheck(0x51500001);
        }

        if (D_800EA174 == 1130) {
            Nai_SeqBufFlagCheck(0x41320000);
        }

        if (D_800EA174 == 1200) { // (Player gets hit by the bomb-omb car; play the "explosion" sound effect and the "hurt" voice for the current character)
            Nai_SeqBufFlagCheck(0x110100FF);
        }

        if (D_800EA174 == 1230) { // Once the music is completely broken by this point, play the "No Trophy For You!" sequence --- "Aw man, I lost... :("
            NaSeqStart(SEQ_EVENT_CEREMONY_TROPHY_LOSE);
            Nai_SeqBufFlagCheck(0x4000007F); // (Play the "losing" voice for the current character, twice)
        }
    }
}

// run audio?
void Nas_AudioInput() {
    Naa_CheckSubTrackPort();
    Nai_SeqGameFrameWork();
    Nai_GetPlayingSeqFlag();
    Nai_FxBufFlagCheck();
    Nai_FxPlayCheck();
    make_sound_program();
    Nai_StartSeqSet(0);
    Nap_SendStart();
}

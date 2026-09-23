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
Camera* CopyCamera[4];
Vec3f VelocityCamera[4];
Vec3f CameraLastPos[4];
u8 D_8018FC08;
s16 D_8018FC10[4][2];
struct Sound SSoundRequests[0x100];
struct SoundCharacteristics SSoundBanks[SOUND_BANK_COUNT][20];
u8 SSoundBankUsedListBack[SOUND_BANK_COUNT];
u8 SSoundBankFreeListFront[SOUND_BANK_COUNT];
u8 SNumSoundsInBank[SOUND_BANK_COUNT];
u8 D_80192AB8[SOUND_BANK_COUNT][8][8];
u8 D_80192C38;
ubool8 SSoundBankDisabled[SOUND_BANK_COUNT];
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
s32 PlayerSurfaceSoundID[] = { 0, 0, 0, 0 };
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
u8 LightningFlagPlayer1[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
u8 D_800E9F2C[NUM_PLAYERS] = { 0, 0, 0, 0, 0, 0, 0, 0 };
f32 D_800E9F34[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
f32 D_800E9F54[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
u8 AsmDisableEngineSound[] = { 0, 0, 0, 0 };
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
s8 LightningFlag = 0;
s8 GamePausedFlag = 0;
u8 D_800EA170[] = { 0, 0, 0, 0 };
u16 D_800EA174 = 0;
f32 D_800EA178 = 1.0f;
f32 D_800EA17C = 0.85f;
u16 D_800EA180 = 0;
u16 D_800EA184 = 0;
u8 D_800EA188[][6] = { { 4, 2, 2, 2, 2, 1 }, { 6, 2, 2, 2, 2, 1 }, { 8, 2, 2, 0, 1, 1 }, { 8, 2, 2, 0, 1, 1 } };
u8 D_800EA1A0[][6] = { { 4, 1, 1, 2, 2, 1 }, { 3, 1, 1, 2, 2, 1 }, { 3, 1, 1, 0, 1, 1 }, { 3, 1, 1, 0, 1, 1 } };
u8 SSoundRequestCount = 0;
u8 SNumProcessedSoundRequests = 0;
u8 D_800EA1C0 = 0;
u16 D_800EA1C4 = 0;
Vec3f D_800EA1C8 = { 0.0f, 0.0f, 0.0f };
f32 D_800EA1D4 = 1.0f;
u32 ExternalUnusedU320 = 0x00000000;
s8 D_800EA1DC = 0;
u32 ExternalUnusedU321 = 0x00000000;
u8 D_800EA1E4 = 0;
u8 D_800EA1E8 = 0;
u8 D_800EA1EC = 0;
u8 D_800EA1F0[] = { 0, 1, 2, 3 };
u8 D_800EA1F4[] = { 0, 0, 0, 0 };

char ExternalUnusedString00[] = "Error : Queue is not empty ( %x ) \n";
char ExternalUnusedString01[] = "specchg error\n";
char ExternalUnusedString02[] = "***** CAM MAX %d *****\n";
u8 D_800EA244 = 0;
char ExternalUnusedString03[] = "entryout !!! %d\n";
char ExternalUnusedString04[] = "AFTER GOAL VOICE FLAME %d\n";
char ExternalUnusedString05[] = "*** Pause On ***\n";
char ExternalUnusedString06[] = "*** Pause Off ***\n";
char ExternalUnusedString07[] = "CALLED!! Na_ChangeSoundMode player %d\n";
char ExternalUnusedString08[] = "CALLED!! Na_ChangeSoundMode spec   %d\n";
char ExternalUnusedString09[] = "Interfaced Spec Change player %d\n";
char ExternalUnusedString10[] = "Interfaced Spec Change spec   %d\n";
UNUSED u32 ExternalUnusedU32S[] = { 0xff000000, 0xff000000, 0x00000000 };
char ExternalUnusedString11[] = "FX MIX %d\n";
char ExternalUnusedString12[] = "************** Seq Fadeout ***************\n";
char ExternalUnusedString13[] = "SEQ FADE OUT TIME %d\n";
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

void NaaFxmixSet(OSMesg presetId) {
    OSMesg mesg;
    osRecvMesg(D_800EA3B4, &mesg, 0);
    osSendMesg(D_800EA3B0, presetId, 0);
    osRecvMesg(D_800EA3B4, &mesg, 1);
    if (mesg != presetId) {
        osRecvMesg(D_800EA3B4, &mesg, 1);
    }
}

f32 NaaPanSet(u8 bank, u8 soundId) {
    f32 temp_f0;
    f32 var_f2;
    s32 var_v0;
    struct SoundCharacteristics* temp_v0;

    temp_v0 = &SSoundBanks[bank][soundId];
    if (temp_v0->SoundBits & 0x400000) {
        return 1.0f;
    }
    temp_f0 = temp_v0->Distance;
    if (temp_f0 > 2000.0f) {
        var_f2 = 0.0f;
    } else {
        switch (temp_v0->SoundBits & 0x30000) { /* irregular */
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

s8 NaaTuneSet(u8 bank, u8 soundId, u8 channel) {
    s32 var_a0;
    s8 var_v0;
    s8 var_v1;

    var_v0 = 0;
    var_v1 = 0;
    if (!(SSoundBanks[bank][soundId].SoundBits & 0x200000)) {
        if (SSoundBanks[bank][soundId].Distance < 500.0f) {
            var_v0 = (SSoundBanks[bank][soundId].Distance / 500.0f) * 10.0f;
        } else {
            var_v0 = 0x0A;
        }
    }
    if (IS_SEQUENCE_CHANNEL_VALID(SeqPlayer[2].Channels[channel])) {
        var_v1 = SeqPlayer[2].Channels[channel]->soundScriptIO[6];
    }
    if (var_v1 == -1) {
        var_v1 = 0;
    }
    var_a0 = *SSoundBanks[bank][soundId].unk18 + var_v0 + var_v1 + D_8018EF10;
    if (var_a0 >= 0x80) {
        var_a0 = 0x7F;
    }
    return var_a0;
}

s8 NaaPlaySeParaSet(f32 arg0, f32 arg1, u8 CameraId) {
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
    return (CameraId & 1) * 0x7F;
}

f32 NaaEntryCameraPosWork(u8 bank, u8 soundId) {
    f32 phi_f2;

    phi_f2 = 1.0f;
    if (SSoundBanks[bank][soundId].SoundBits & 0x800000) {
        phi_f2 -= ((AudioRandom & 0xF) / 192.0f);
    }
    return phi_f2;
}

void NaaCalcCameraPos(u8 arg0, u8 arg1, u8 Arg2) {
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
            temp_s0 = &SSoundBanks[arg0][arg1];
            temp_s0->Distance = sqrtf(temp_s0->Distance);
            sp3C = (NaaPanSet(arg0, arg1) * *temp_s0->unk14) * D_80192C48[arg0].Current;
            sp3B = NaaTuneSet(arg0, arg1, Arg2);
            sp34 = NaaEntryCameraPosWork(arg0, arg1) * *temp_s0->unk10;
            sp33 = NaaPlaySeParaSet(*temp_s0->unk00[0], *temp_s0->unk08, temp_s0->CameraId);
            break;
    }
    temp_s0_2 = &D_8018EF18[Arg2];
    if (sp3C != temp_s0_2->Unk0) {
        NapSetS8(((Arg2 & 0xFF) << 8) | 0x06020000 | 3, (u8) (u32) (sp3C * 127.0f));
        temp_s0_2->Unk0 = sp3C;
    }
    if (sp3B != (s8) temp_s0_2->Unk8) {
        NapSetS8(((Arg2 & 0xFF) << 8) | 0x05020000, sp3B);
        temp_s0_2->Unk8 = (u8) sp3B;
    }
    if (sp34 != temp_s0_2->unk4) {
        NapSetF32(((Arg2 & 0xFF) << 8) | 0x04020000, sp34);
        temp_s0_2->unk4 = sp34;
    }
    if (sp33 != (s8) temp_s0_2->Unk9) {
        NapSetS8(((Arg2 & 0xFF) << 8) | 0x03020000, sp33);
        temp_s0_2->Unk9 = (u8) sp33;
    }
}

struct Unk_8018EFD8* NaaCalcdoppler(u8 arg0, Vec3f Position, f32* Velocity, f32* Arg3, u8 arg4, u32 SoundBits) {
    struct Unk_8018EFD8* temp_a1;
    UNUSED struct Unk_8018EFD8* temp_v1;
    UNUSED u8 temp_t7;
    u8 why1;

    if (D_8018EFD8[D_8018FB90].Next != 0xFF) {
        why1 = D_8018FB90;
        D_8018EFD8[why1].Prev = D_8018FB91;
        temp_a1 = &D_8018EFD8[D_8018FB90];
        D_8018EFD8[D_8018FB91].Next = why1;
        temp_t7 = temp_a1->Next;
        D_8018FB91 = why1;
        D_8018FB90 = D_8018EFD8[D_8018FB90].Next;
        D_8018EFD8[D_8018FB90].Prev = 0xFF;
        D_8018EFD8[why1].PosX = &Position[0];
        D_8018EFD8[why1].PosY = &Position[1];
        D_8018EFD8[why1].PosZ = &Position[2];
        D_8018EFD8[why1].Next = 0xFF;
        D_8018EFD8[why1].VelX = &Velocity[0];
        D_8018EFD8[why1].VelY = &Velocity[1];
        D_8018EFD8[why1].VelZ = &Velocity[2];
        D_8018EFD8[why1].unk18[1] = 0.0f;
        D_8018EFD8[why1].unk24 = Arg3;
        D_8018EFD8[why1].CameraId = arg4;
        D_8018EFD8[why1].unk30 = arg0;
        D_8018EFD8[why1].SoundBits = SoundBits;
        return &D_8018EFD8[why1];
    }
    return NULL;
}

void NaaCheckCameraPos(Camera* arg0, Vec3s rot, struct Unk_8018EFD8* Arg2) {
    f32 x;
    f32 y;

    x = arg0->CameraPos[0] - *Arg2->PosX;
    y = arg0->CameraPos[2] - *Arg2->PosZ;
    Arg2->unk18[0] = Kwrotate2DX(x, y, rot[1]);
    Arg2->unk18[2] = Kwrotate2DY(x, y, rot[1]);
}

void NaaWposptToCpospt(Camera* Camera, Vec3f Velocity, struct Unk_8018EFD8* Arg2) {
    f32 sp44;
    f32 temp_f6;
    f32 x;
    f32 y;
    f32 dist0;
    f32 dist1;
    f32 Thing0;
    f32 Thing1;
    f32 temp_f2;

    x = (*Arg2->PosX) - Camera->CameraPos[0];
    y = (*Arg2->PosZ) - Camera->CameraPos[2];

    sp44 = (*Arg2->VelX) - Velocity[0];
    temp_f6 = (*Arg2->VelZ) - Velocity[2];

    Thing0 = x + sp44;
    Thing1 = y + temp_f6;

    dist0 = sqrtf((x * x) + (y * y));
    dist1 = sqrtf((Thing0 * Thing0) + (Thing1 * Thing1));

    temp_f2 = 1.0f / (1.0f - ((dist0 - dist1) / Arg2->unk34));

    if (temp_f2 > 0.1f) {
        Arg2->unk2C = temp_f2;
    } else {
        Arg2->unk2C = 0.1f;
    }

    if ((*Arg2->unk24) != 0.0f) {
        Arg2->unk2C *= (((*Arg2->unk24) / D_800EA06C[Arg2->unk30].unk00[1]) + D_800EA06C[Arg2->unk30].unk00[0]) +
                       D_800E9F34[Arg2->unk30];
    }
}

void NaaCheckSubTrackPort(void) {
    u8 var_s1;
    u8 var_a1;
    u8 CameraId;
    Camera** Camera;

    var_a1 = D_800EA1C0 + 1;
    for (var_s1 = 0; var_s1 < var_a1; var_s1++) {
        VelocityCamera[var_s1][0] = CopyCamera[var_s1]->CameraPos[0] - CameraLastPos[var_s1][0];
        VelocityCamera[var_s1][2] = CopyCamera[var_s1]->CameraPos[2] - CameraLastPos[var_s1][2];
        CameraLastPos[var_s1][0] = CopyCamera[var_s1]->CameraPos[0];
        CameraLastPos[var_s1][2] = CopyCamera[var_s1]->CameraPos[2];
    }

    var_a1 = 0;
    var_s1 = D_8018EFD8[0].Next;
    while (var_s1 != 0xFF) {
        if (D_8018EFD8[var_s1].unk18[1] == 100000.0f) {
            if (D_8018FB91 == var_s1) {
                D_8018FB91 = D_8018EFD8[var_s1].Prev;
            } else {
                D_8018EFD8[D_8018EFD8[var_s1].Next].Prev = D_8018EFD8[var_s1].Prev;
            }
            D_8018EFD8[D_8018EFD8[var_s1].Prev].Next = D_8018EFD8[var_s1].Next;
            D_8018EFD8[var_s1].Prev = 0xFF;
            D_8018EFD8[var_s1].Next = D_8018FB90;
            D_8018EFD8[D_8018FB90].Prev = var_s1;
            D_8018FB90 = var_s1;
        } else {
            CameraId = D_8018EFD8[var_s1].CameraId;
            // Why? Why would you do it this way? For what possible reason?
            Camera = &CopyCamera[CameraId];
            NaaCheckCameraPos(*Camera, (*Camera)->CameraDirection, &D_8018EFD8[var_s1]);
            if (D_800EA1C8 != D_8018EFD8[var_s1].VelX) {
                NaaWposptToCpospt(*Camera, VelocityCamera[0], &D_8018EFD8[var_s1]);
            }
            var_a1 = var_s1;
        }
        var_s1 = D_8018EFD8[var_a1].Next;
        if ((var_s1 != 0xFF) && (D_800EA244 < var_s1)) {
            D_800EA244 = var_s1;
        }
    }
}

Vec3f* NaaInitApliWork(Vec3f pos, u32 SoundBits) {
    u8 it; // iterator
    Vec3f* ret;
    // Only here to Force a match
    UNUSED f32* thing = pos;
    struct Unk_8018EFD8* temp_a1;

    ret = 0;
    it = D_8018EFD8[0].Next;
    while (it != 0xFF) {
        temp_a1 = &D_8018EFD8[it];
        // It doesn't matter what you set var_v0 to here actually
        it = D_8018EFD8[0].Next;
        if ((pos == temp_a1->PosX) && (SoundBits == temp_a1->SoundBits)) {
            // I suspect all this weirdness here is a result of someone not knowing the 'break' keyword
            it = 0xFF;
            if (temp_a1->unk18[1] != 100000.0f) {
                ret = &temp_a1->unk18;
            } else {
                goto test;
            }
        } else {
        test:
            it = temp_a1->Next;
        }
    }
    return ret;
}

void NaiStartSeqSet(u8 Player) {
    s16 sp46;
    s16 var_s1;
    s16 sp42;
    u8 var_s0;
    u8 temp_s0;
    u8 why = 0xFF;
    s32 var_a2;

    var_a2 = 0xF;
    if (SeqPlayer[Player].Enabled != 0) {
        switch (ScreenSplitB) { /* irregular */
            case 0:
                break;
            case 1:
                var_a2 = 0xE;
                break;
            case 3:
                if (PlayerCount == 3) {
                    var_a2 = 0xD;
                } else {
                    var_a2 = 0xC;
                }
                break;
        }
        temp_s0 = SeqPlayer[Player].Channels[var_a2]->soundScriptIO[0];
        if (temp_s0 != why) {
            sp46 = SeqPlayer[Player].Channels[var_a2]->soundScriptIO[1] % 4u;
            var_s1 = SeqPlayer[Player].Channels[var_a2]->soundScriptIO[2] % 16u;
            if (var_s1 >= 0xA) {
                var_s1 = 9;
            }
            sp42 = SeqPlayer[Player].Channels[var_a2]->soundScriptIO[3] % 8u;
            NapSetS8(((Player & 0xFF) << 0x10) | 0x06000000 | ((var_a2 & 0xFF) << 8), -1);
        }
        switch (temp_s0) { /* switch 1; irregular */
            case 1:        /* switch 1 */
                D_8018FC10[sp46][0] = var_s1;
                D_8018FC10[sp46][1] = sp42;
                for (var_s0 = 0; var_s0 < 4; var_s0++) {
                    if (D_8018FC10[var_s0][0] != why) {
                        OgawaSoundCameraSet(var_s0, D_8018FC10[var_s0][0], D_8018FC10[var_s0][1]);
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

void NaiStopSeqSet(void) {
    u8 var_v0;

    D_8018EF10 = 0;
    CopyCamera[0] = Camera1;
    CopyCamera[1] = Camera2;
    CopyCamera[2] = Camera3;
    CopyCamera[3] = Camera4;
    D_8018FB91 = 0;
    D_8018FB90 = 1;
    D_800EA0F4 = 0;
    D_8018FC08 = 0;
    D_800EA104 = 0;
    D_800EA108 = 0;
    D_800EA0F0 = 0;
    GamePausedFlag = 0;
    NapSetS32(0xF2000000U, 0);
    GamePausedFlag = 0;
    D_800EA15C = 0;
    D_800EA160 = 0;
    D_800EA164 = 0;
    D_800EA178 = 1.0f;
    D_800EA17C = 0.85f;
    D_800EA180 = 0;
    D_800EA184 = 0;
    for (var_v0 = 0; var_v0 < 4; var_v0++) {
        D_800E9DB4[var_v0] = 0;
        D_800E9DF4[var_v0] = 0; // This is an f32 FYI, but 0.0f doesn't match Right :)
        VelocityCamera[var_v0][0] = 0.0f;
        VelocityCamera[var_v0][1] = 0.0f;
        VelocityCamera[var_v0][2] = 0.0f;
        CameraLastPos[var_v0][0] = 0.0f;
        CameraLastPos[var_v0][1] = 0.0f;
        CameraLastPos[var_v0][2] = 0.0f;
        D_800EA0EC[var_v0] = 0;
        D_800E9EA4[var_v0] = 0;
        D_800E9F7C[var_v0].unk_14 = 0;
        PlayerSurfaceSoundID[var_v0] = 0;
        D_800E9E84[var_v0] = 0;
        D_800E9E94[var_v0] = 0;
        GamePlayers[var_v0].TireRL.Status = 0;
        GamePlayers[var_v0].TireRR.Status = 0;
        GamePlayers[var_v0].SlipFlag = 0;
        GamePlayers[var_v0].BrakeTime = 0.0f;
        GamePlayers[var_v0].Slipang = 0;
        GamePlayers[var_v0].Taco = 0.0f;
        GamePlayers[var_v0].WaterFlag = 0;
        D_8018FC10[var_v0][0] = 0x00FF;
        D_8018FC10[var_v0][1] = 0;
        D_800EA10C[var_v0] = 0;
        AsmDisableEngineSound[var_v0] = 0;
        D_800E9F78[var_v0] = 0;
    }
    for (var_v0 = 0; var_v0 < NUM_PLAYERS; var_v0++) {
        LightningFlagPlayer1[var_v0] = 0;
        D_800E9F2C[var_v0] = 0;
        D_800E9F34[var_v0] = 0.0f;
        D_800E9F54[var_v0] = 0.0f;
        D_800EA130[var_v0] = 0.0f;
        D_800EA06C[var_v0].unk0C = 0;
        GamePlayers[var_v0].SlipFlag = 0;
    }
    for (var_v0 = 0; var_v0 < 16; var_v0++) {
        D_8018EF18[var_v0].Unk0 = 1.0f;
        D_8018EF18[var_v0].unk4 = 1.0f;
        D_8018EF18[var_v0].Unk8 = 0;
        D_8018EF18[var_v0].Unk9 = 0x40;
    }
    D_8018EFD8[0].Prev = 0xFF;
    D_8018EFD8[0].Next = 0xFF;
    for (var_v0 = 1; var_v0 < 49; var_v0++) {
        D_8018EFD8[var_v0].Prev = var_v0 - 1;
        D_8018EFD8[var_v0].Next = var_v0 + 1;
    }
    D_8018EFD8[var_v0].Prev = var_v0 - 1;
    D_8018EFD8[var_v0].Next = 0xFF;
}

void NaiSeqControl(u8 arg0, u8 arg1, u8 Arg2, u16 Arg3) {
    u8 var_v1;
    UNUSED s32 pad;

    if ((D_800EA1EC != 0) && (arg0 != 2)) {
        return;
    }

    NapSetS32(0x82000000 | (((u32) arg0 & 0xFF) << 0x10) | (((u32) arg1 & 0xFF) << 8), Arg3);
    D_801930D0[arg0].Unk248 = arg1 | (Arg2 << 8);
    if (D_801930D0[arg0].Unk000 != 1.0f) {
        NapSetF32(0x41000000 | (((u32) arg0 & 0xFF) << 0x10), D_801930D0[arg0].Unk000);
    }
    D_801930D0[arg0].Unk028 = 0;
    D_801930D0[arg0].Unk018 = 0;
    D_801930D0[arg0].Unk014 = 0;

    for (var_v1 = 0; var_v1 < 16; var_v1++) {
        D_801930D0[arg0].Unk044[var_v1].unk_00 = 1.0f;
        D_801930D0[arg0].Unk044[var_v1].unk_0C = 0;
        D_801930D0[arg0].Unk044[var_v1].unk_10 = 1.0f;
        D_801930D0[arg0].Unk044[var_v1].unk_1C = 0;
    }

    D_801930D0[arg0].Unk244 = 0;
    D_801930D0[arg0].Unk246 = 0;
}

void func_800C29B4(u8 arg0, u16 arg1) {
    NapSetS32(((arg0 & 0xFF) << 0x10) | 0x83000000, arg1);
    D_801930D0[arg0].Unk248 = 0xFFFF;
}

void NAISeqFlagEntry(u32 cmd) {
    f32 freqScaleTarget;
    u16 channelMaskDisable;
    u16 val;
    u16 fadeTimer;
    u8 Priority;
    u8 ioPort;
    u8 i;
    u8 Duration;
    u8 channelIndex;
    u8 found;
    u8 SEQId;
    u8 subArgs;
    u8 op;
    u8 seqPlayerIndex;

    op = cmd >> 28;
    seqPlayerIndex = (cmd & 0xF000000) >> 24;

    switch (op) {
        case 0:
            SEQId = cmd & 0xFF;
            subArgs = (cmd & 0xFF00) >> 8;
            fadeTimer = (cmd & 0xFF0000) >> 13;
            NaiSeqControl(seqPlayerIndex, SEQId, subArgs, fadeTimer);
            break;

        case 1:
            fadeTimer = (cmd & 0xFF0000) >> 13;
            func_800C29B4(seqPlayerIndex, fadeTimer);
            break;

        case 2:
            SEQId = cmd & 0xFF;
            subArgs = (cmd & 0xFF00) >> 8;
            fadeTimer = (cmd & 0xFF0000) >> 13;
            Priority = subArgs;

            for (i = 0; i < D_80192CC6[seqPlayerIndex]; i++) {
                if (D_80192CA8[seqPlayerIndex][i].Thing0 == SEQId) {
                    if (i == 0) {
                        NaiSeqControl(seqPlayerIndex, SEQId, subArgs, fadeTimer);
                    }
                    return;
                }
            }

            found = D_80192CC6[seqPlayerIndex];
            for (i = 0; i < D_80192CC6[seqPlayerIndex]; i++) {
                if (Priority >= D_80192CA8[seqPlayerIndex][i].Thing1) {
                    found = i;
                    i = D_80192CC6[seqPlayerIndex];
                }
            }

            if ((found != D_80192CC6[seqPlayerIndex]) || (found == 0)) {
                if (D_80192CC6[seqPlayerIndex] < 5) {
                    D_80192CC6[seqPlayerIndex]++;
                }
                for (i = D_80192CC6[seqPlayerIndex] - 1; i != found; i--) {
                    D_80192CA8[seqPlayerIndex][i].Thing1 = D_80192CA8[seqPlayerIndex][i - 1].Thing1;
                    D_80192CA8[seqPlayerIndex][i].Thing0 = D_80192CA8[seqPlayerIndex][i - 1].Thing0;
                }

                D_80192CA8[seqPlayerIndex][found].Thing1 = subArgs;
                D_80192CA8[seqPlayerIndex][found].Thing0 = SEQId;
            }
            if (found == 0) {
                NaiSeqControl(seqPlayerIndex, SEQId, subArgs, fadeTimer);
            }
            break;

        case 3:
            fadeTimer = (cmd & 0xFF0000) >> 13;

            found = D_80192CC6[seqPlayerIndex];
            for (i = 0; i < D_80192CC6[seqPlayerIndex]; i++) {
                SEQId = cmd & 0xFF;
                if (D_80192CA8[seqPlayerIndex][i].Thing0 == SEQId) {
                    found = i;
                    i = D_80192CC6[seqPlayerIndex];
                }
            }

            if (found != D_80192CC6[seqPlayerIndex]) {
                for (i = found; i < (D_80192CC6[seqPlayerIndex] - 1); i++) {
                    D_80192CA8[seqPlayerIndex][i].Thing1 = D_80192CA8[seqPlayerIndex][i + 1].Thing1;
                    D_80192CA8[seqPlayerIndex][i].Thing0 = D_80192CA8[seqPlayerIndex][i + 1].Thing0;
                }

                D_80192CC6[seqPlayerIndex]--;
            }
            if (found == 0) {
                func_800C29B4(seqPlayerIndex, fadeTimer);
                if (D_80192CC6[seqPlayerIndex] != 0) {
                    NaiSeqControl(seqPlayerIndex, D_80192CA8[seqPlayerIndex][0].Thing0,
                                  D_80192CA8[seqPlayerIndex][0].Thing1, fadeTimer);
                }
            }
            break;

        case 4:
            Duration = (cmd & 0xFF0000) >> 15;
            val = cmd & 0xFF;

            if (Duration == 0) {
                Duration++;
            }

            D_801930D0[seqPlayerIndex].Unk004 = val / 127.0f;
            if (D_801930D0[seqPlayerIndex].Unk000 != D_801930D0[seqPlayerIndex].Unk004) {
                D_801930D0[seqPlayerIndex].Unk008 =
                    (D_801930D0[seqPlayerIndex].Unk000 - D_801930D0[seqPlayerIndex].Unk004) / Duration;
                D_801930D0[seqPlayerIndex].Unk00C = Duration;
            }
            break;

        case 5:
            Duration = (cmd & 0xFF0000) >> 15;
            val = cmd & 0xFFFF;

            if (Duration == 0) {
                Duration++;
            }
            freqScaleTarget = (f32) val / 1000.0f;
            for (i = 0; i < 0x10; i++) {
                D_801930D0[seqPlayerIndex].Unk044[i].unk_14 = freqScaleTarget;
                D_801930D0[seqPlayerIndex].Unk044[i].unk_1C = Duration;
                D_801930D0[seqPlayerIndex].Unk044[i].unk_18 =
                    (D_801930D0[seqPlayerIndex].Unk044[i].unk_10 - freqScaleTarget) / Duration;
            }

            D_801930D0[seqPlayerIndex].Unk244 = 0xFFFF;
            break;

        case 6:
            Duration = (cmd & 0xFF0000) >> 15;
            channelIndex = (cmd & 0xF00) >> 8;
            val = cmd & 0xFF;

            if (Duration == 0) {
                Duration++;
            }

            D_801930D0[seqPlayerIndex].Unk044[channelIndex].unk_04 = val / 127.0f;
            if (D_801930D0[seqPlayerIndex].Unk044[channelIndex].unk_00 !=
                D_801930D0[seqPlayerIndex].Unk044[channelIndex].unk_04) {
                D_801930D0[seqPlayerIndex].Unk044[channelIndex].unk_08 =
                    (D_801930D0[seqPlayerIndex].Unk044[channelIndex].unk_00 -
                     D_801930D0[seqPlayerIndex].Unk044[channelIndex].unk_04) /
                    Duration;
                D_801930D0[seqPlayerIndex].Unk044[channelIndex].unk_0C = Duration;
                D_801930D0[seqPlayerIndex].Unk244 |= 1 << channelIndex;
            }
            break;

        case 7:
            ioPort = (cmd & 0xFF0000) >> 16;
            val = cmd & 0xFF;
            NapSetS8((0x46000000 | ((seqPlayerIndex & 0xFF) << 0x10)) | ((ioPort & 0xFF) << 8), val);
            break;

        case 8:
            channelIndex = (cmd & 0xF00) >> 8;
            ioPort = (cmd & 0xFF0000) >> 16;
            val = cmd & 0xFF;
            if (!(D_801930D0[seqPlayerIndex].Unk24A & (1 << channelIndex))) {
                NapSetS8(((0x06000000 | ((seqPlayerIndex & 0xFF) << 0x10)) | (((u32) channelIndex & 0xFF) << 8)) |
                                  (ioPort & 0xFF),
                              val);
            }
            break;

        case 9:
            D_801930D0[seqPlayerIndex].Unk24A = cmd & 0xFFFF;
            break;

        case 10:
            val = 1;
            channelMaskDisable = cmd & 0xFFFF;
            for (i = 0; i < 0x10; i++) {
                NapSetS8(0x08000000 | ((seqPlayerIndex & 0xFF) << 0x10) | (((u32) i & 0xFF) << 8),
                              (channelMaskDisable & val) ? 1 : 0);
                val <<= 1;
            }

            break;

        case 11:
            D_801930D0[seqPlayerIndex].Unk014 = cmd;
            break;

        case 12:
            subArgs = (cmd & 0xF00000) >> 20;
            if (subArgs != 0xF) {
                found = D_801930D0[seqPlayerIndex].Unk041++;
                if (found < 5) {
                    D_801930D0[seqPlayerIndex].Unk02C[found] = cmd;
                    D_801930D0[seqPlayerIndex].Unk040 = 2;
                }
            } else {
                D_801930D0[seqPlayerIndex].Unk041 = 0;
            }
            break;

        case 14:
            subArgs = (cmd & 0xF00) >> 8;
            val = cmd & 0xFF;
            switch (subArgs) {
                case 0:
                    NapSetS32(0xF0000000, D_800EA1F0[val]);
                    break;

                case 1:
                    D_800EA1EC = val & 1;
                    break;
            }

            break;

        case 15:
            SEQId = cmd & 0xFF;
            subArgs = (cmd & 0xFF00) >> 8;
            D_800EA1C0 = subArgs;
            NaaFxmixSet((void*) SEQId);
            D_800EA1F4[0] = SEQId;
            NapSetS8(0x46020000, subArgs);
            func_800C5C40();
            break;

        default:
            break;
    }
}

void NaiSeqBufFlagCheck(u32 arg0) {
    D_80192CD0[D_800EA1E4] = arg0;
    D_800EA1E4 += 1;
}

void NaiGetPlayingSeqFlag(void) {
    for (D_800EA1E8; D_800EA1E4 != D_800EA1E8;) {
        NAISeqFlagEntry(D_80192CD0[D_800EA1E8++]);
    }
}

u16 NAIGetPlayingSeqFlag(u8 Player) {
    if (!SeqPlayer[Player].Enabled) {
        return -1;
    }
    return D_801930D0[Player].Unk248;
}

u8 NaiCheckSeqFlagBuffer2(s32 arg0) {
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

void NaiClearAllSeqFlag(u8 arg0) {
    D_80192CC6[arg0] = 0;
}

void NaiClearSeqECheckFlag(u8 arg0, u8 arg1) {
    u8 var_v0;
    u8 thing;

    for (var_v0 = 0; var_v0 < D_801930D0[arg0].Unk041; var_v0++) {
        thing = (D_801930D0[arg0].Unk02C[var_v0] & 0xF00000) >> 0x14;
        if (thing == arg1) {
            D_801930D0[arg0].Unk02C[var_v0] = 0xFF000000;
        }
    }
}

void NaiSetSeqLineVolMove(u8 arg0, u8 arg1, u8 Arg2, u8 Arg3) {
    D_801930D0[arg0].Unk00E[arg1] = Arg2;
    D_801930D0[arg0].Unk011 = Arg3;
    D_801930D0[arg0].Unk012 = 1;
}

void NaiSeqGameFrameWork(void) {
    u8 seqPlayerIndex;
    f32 Volume;
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
    u16 SEQId;

    for (seqPlayerIndex = 0; seqPlayerIndex < 3; seqPlayerIndex++) {
        if (D_801930D0[seqPlayerIndex].Unk012) {
            Volume = 1.0f;
            for (j = 0; j < 3; j++) {
                Volume *= D_801930D0[seqPlayerIndex].Unk00E[j] / 127.0f;
            }

            NaiSeqBufFlagCheck(0x40000000 | (((u8) seqPlayerIndex) << 0x18) |
                          (((u8) D_801930D0[seqPlayerIndex].Unk011) << 0x10) | ((u16) (u8) (Volume * 127.0f)));

            D_801930D0[seqPlayerIndex].Unk012 = 0;
        }
        if (D_801930D0[seqPlayerIndex].Unk00C != 0) {
            D_801930D0[seqPlayerIndex].Unk00C--;
            if (D_801930D0[seqPlayerIndex].Unk00C) {
                D_801930D0[seqPlayerIndex].Unk000 -= D_801930D0[seqPlayerIndex].Unk008;
            } else {
                D_801930D0[seqPlayerIndex].Unk000 = D_801930D0[seqPlayerIndex].Unk004;
            }
            NapSetF32(0x41000000 | (((u32) seqPlayerIndex & 0xFF) << 0x10), D_801930D0[seqPlayerIndex].Unk000);
        }
        if (D_801930D0[seqPlayerIndex].Unk014 != 0) {
            tempoCmd = D_801930D0[seqPlayerIndex].Unk014;
            tempoTimer = (tempoCmd & 0xFF0000) >> 0xF;
            tempoTarget = tempoCmd & 0xFFF;
            if (tempoTimer == 0) {
                tempoTimer++;
            }
            if (SeqPlayer[seqPlayerIndex].Enabled != 0) {
                tempoPrev = SeqPlayer[seqPlayerIndex].Tempo / 48;
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
                            (D_801930D0[seqPlayerIndex].Unk018 != 0) ? D_801930D0[seqPlayerIndex].Unk018 : tempoPrev;
                        break;
                    default:
                        break;
                }

                if (tempoTarget > 300) {
                    tempoTarget = 300;
                }
                if (D_801930D0[seqPlayerIndex].Unk018 == 0) {
                    D_801930D0[seqPlayerIndex].Unk018 = tempoPrev;
                }
                D_801930D0[seqPlayerIndex].Unk020 = tempoTarget;
                D_801930D0[seqPlayerIndex].Unk01C = SeqPlayer[seqPlayerIndex].Tempo / 48;
                D_801930D0[seqPlayerIndex].Unk024 =
                    (D_801930D0[seqPlayerIndex].Unk01C - D_801930D0[seqPlayerIndex].Unk020) / tempoTimer;
                D_801930D0[seqPlayerIndex].Unk028 = tempoTimer;
            }
            D_801930D0[seqPlayerIndex].Unk014 = 0;
        }
        if (D_801930D0[seqPlayerIndex].Unk028 != 0) {
            D_801930D0[seqPlayerIndex].Unk028--;
            if (D_801930D0[seqPlayerIndex].Unk028) {
                D_801930D0[seqPlayerIndex].Unk01C -= D_801930D0[seqPlayerIndex].Unk024;
            } else {
                D_801930D0[seqPlayerIndex].Unk01C = D_801930D0[seqPlayerIndex].Unk020;
            }
            NapSetS32(0x47000000 | (((u32) seqPlayerIndex & 0xFF) << 0x10),
                          (s32) D_801930D0[seqPlayerIndex].Unk01C);
        }

        if (D_801930D0[seqPlayerIndex].Unk246 != 0) {
            for (channelIndex = 0; channelIndex < 0x10; channelIndex++) {
                if (D_801930D0[seqPlayerIndex].Unk044[channelIndex].unk_0C != 0) {
                    D_801930D0[seqPlayerIndex].Unk044[channelIndex].unk_0C--;
                    if (D_801930D0[seqPlayerIndex].Unk044[channelIndex].unk_0C) {
                        D_801930D0[seqPlayerIndex].Unk044[channelIndex].unk_00 -=
                            D_801930D0[seqPlayerIndex].Unk044[channelIndex].unk_08;
                    } else {
                        D_801930D0[seqPlayerIndex].Unk044[channelIndex].unk_00 =
                            D_801930D0[seqPlayerIndex].Unk044[channelIndex].unk_04;
                        D_801930D0[seqPlayerIndex].Unk246 ^= 1 << channelIndex;
                    }
                    NapSetF32(0x01000000 | ((seqPlayerIndex & 0xFF) << 0x10) | (((u32) channelIndex & 0xFF) << 8),
                                  D_801930D0[seqPlayerIndex].Unk044[channelIndex].unk_00);
                }
            }
        }
        if (D_801930D0[seqPlayerIndex].Unk244 != 0) {
            for (channelIndex = 0; channelIndex < 0x10; channelIndex++) {
                if (D_801930D0[seqPlayerIndex].Unk044[channelIndex].unk_1C != 0) {
                    D_801930D0[seqPlayerIndex].Unk044[channelIndex].unk_1C--;
                    if (D_801930D0[seqPlayerIndex].Unk044[channelIndex].unk_1C) {
                        D_801930D0[seqPlayerIndex].Unk044[channelIndex].unk_10 -=
                            D_801930D0[seqPlayerIndex].Unk044[channelIndex].unk_18;
                    } else {
                        D_801930D0[seqPlayerIndex].Unk044[channelIndex].unk_10 =
                            D_801930D0[seqPlayerIndex].Unk044[channelIndex].unk_14;
                        D_801930D0[seqPlayerIndex].Unk244 ^= 1 << channelIndex;
                    }
                    NapSetF32(0x04000000 | ((seqPlayerIndex & 0xFF) << 0x10) | (((u32) channelIndex & 0xFF) << 8),
                                  D_801930D0[seqPlayerIndex].Unk044[channelIndex].unk_10);
                }
            }
        }
        if (D_801930D0[seqPlayerIndex].Unk041) {
#ifdef VERSION_EU
            if (func_800C357C_eu(-0x10000000, -0x10000000) == 0) {
                D_801930D0[seqPlayerIndex].Unk041 = 0;
                return;
            }
#endif
            if (D_801930D0[seqPlayerIndex].Unk040 != 0) {
                D_801930D0[seqPlayerIndex].Unk040--;
                continue;
            }

            if (SeqPlayer[seqPlayerIndex].Enabled != 0) {
                continue;
            }

            for (j = 0; j < D_801930D0[seqPlayerIndex].Unk041; j++) {
                setupOp = (D_801930D0[seqPlayerIndex].Unk02C[j] & 0xF00000) >> 0x14;
                targetSeqPlayerIndex = (D_801930D0[seqPlayerIndex].Unk02C[j] & 0xF0000) >> 0x10;
                setupVal2 = (D_801930D0[seqPlayerIndex].Unk02C[j] & 0xFF00) >> 8;
                setupVal1 = D_801930D0[seqPlayerIndex].Unk02C[j] & 0xFF;
                switch (setupOp) {
                    case 0:
                        D_801930D0[targetSeqPlayerIndex].Unk012 = 1;
                        D_801930D0[targetSeqPlayerIndex].Unk00E[1] = 0x7F;
                        break;

                    case 1:
                        NaiSeqBufFlagCheck(0x30000000 | ((u8) seqPlayerIndex) << 0x18 |
                                      (D_801930D0[seqPlayerIndex].Unk248));
                        break;

                    case 2:
                        NaiSeqBufFlagCheck((((u8) targetSeqPlayerIndex) << 0x18) | 0x10000 |
                                      (u16) (D_801930D0[targetSeqPlayerIndex].Unk248));
                        D_801930D0[targetSeqPlayerIndex].Unk012 = 1;
                        D_801930D0[targetSeqPlayerIndex].Unk00E[1] = 0x7F;
                        break;

                    case 3:
                        NaiSeqBufFlagCheck(0xB0003000 | (((u8) targetSeqPlayerIndex) << 0x18) | (((u8) setupVal2) << 0x10) |
                                      (u16) setupVal1);
                        break;

                    case 4:
                        NaiSeqBufFlagCheck(0xB0004000 | (((u8) targetSeqPlayerIndex) << 0x18) | (((u8) setupVal1) << 0x10));
                        break;

                    case 5:
                        SEQId = D_801930D0[seqPlayerIndex].Unk02C[j] & 0xFFFF;
                        NaiSeqBufFlagCheck((((u8) targetSeqPlayerIndex) << 0x18) |
                                      (((u8) D_801930D0[targetSeqPlayerIndex].Unk042) << 0x10) | ((u16) SEQId));

                        NaiSetSeqLineVolMove(targetSeqPlayerIndex, 1, 0x7F, 0);
                        D_801930D0[targetSeqPlayerIndex].Unk042 = 0;
                        break;

                    case 6:
                        D_801930D0[seqPlayerIndex].Unk042 = setupVal2;
                        break;
                }
            }

            D_801930D0[seqPlayerIndex].Unk041 = 0;
        }
    }
}

void NaiSEQInterInit(void) {
    u8 var_v0;

    for (var_v0 = 0; var_v0 < 3; var_v0++) {
        D_80192CC6[var_v0] = 0;
        D_801930D0[var_v0].Unk248 = 0xFFFF;
        D_801930D0[var_v0].Unk000 = 1.0f;
        D_801930D0[var_v0].Unk00C = 0;
        D_801930D0[var_v0].Unk028 = 0;
        D_801930D0[var_v0].Unk018 = 0;
        D_801930D0[var_v0].Unk014 = 0;
        D_801930D0[var_v0].Unk24A = 0;
        D_801930D0[var_v0].Unk041 = 0;
        D_801930D0[var_v0].Unk042 = 0;
        D_801930D0[var_v0].Unk012 = 0;
        D_801930D0[var_v0].Unk00E[0] = 0x7F;
        D_801930D0[var_v0].Unk00E[1] = 0x7F;
        D_801930D0[var_v0].Unk00E[2] = 0x7F;
        D_801930D0[var_v0].Unk244 = 0;
        D_801930D0[var_v0].Unk246 = 0;
    }
}

void NaiSEQInterInitSpec(void) {
    u8 var_v0;

    for (var_v0 = 0; var_v0 < 3; var_v0++) {
        D_80192CC6[var_v0] = 0;
        D_801930D0[var_v0].Unk248 = 0xFFFF;
        D_801930D0[var_v0].Unk028 = 0;
        D_801930D0[var_v0].Unk018 = 0;
        D_801930D0[var_v0].Unk014 = 0;
        D_801930D0[var_v0].Unk24A = 0;
        D_801930D0[var_v0].Unk041 = 0;
        D_801930D0[var_v0].Unk042 = 0;
        D_801930D0[var_v0].Unk244 = 0;
        D_801930D0[var_v0].Unk246 = 0;
    }
}

// Appears to be an Unused combo of sound_banks_enable and sound_banks_disable
void NaiFxSetLockFlag(u16 bankMask) {
    u8 bank;

    for (bank = 0; bank < SOUND_BANK_COUNT; bank++) {
        if (bankMask & 1) {
            SSoundBankDisabled[bank] = true;
        } else {
            SSoundBankDisabled[bank] = false;
        }
        bankMask = bankMask >> 1;
    }
}

void NaiFxClearBgmMuteFlag(u8 arg0) {
    D_800EA1C4 &= ((1 << (arg0)) ^ (u16) -1);
    if (!D_800EA1C4) {
        D_801930D0[0].Unk012 = 1;
        D_801930D0[0].Unk00E[2] = 0x7F;
    }
}

void NAIFxFlagEntry(u32 SoundBits, Vec3f* Position, u8 CameraId, f32* Arg3, f32* arg4, s8* arg5) {
    u8 bank;
    struct Sound* temp_v0;

    bank = SoundBits >> 0x1C;
    if (SSoundBankDisabled[bank] == false) {
        temp_v0 = &SSoundRequests[SSoundRequestCount];
        temp_v0->SoundBits = SoundBits;
        temp_v0->Position = Position;
        temp_v0->CameraId = CameraId;
        temp_v0->unk0C = Arg3;
        temp_v0->unk10 = arg4;
        temp_v0->unk14 = arg5;
        SSoundRequestCount += 1;
    }
}

void NaiStopBufferFx(u8 arg0, struct SoundCharacteristics* arg1) {
    s32 found;
    u8 soundId;
    struct Sound* Sound;

    for (soundId = SNumProcessedSoundRequests; soundId != SSoundRequestCount; soundId++) {
        found = false;
        Sound = &SSoundRequests[soundId];
        switch (arg0) {
            case 0:
                if ((Sound->SoundBits & 0xF0000000) == (arg1->SoundBits & 0xF0000000)) {
                    found = true;
                }
                break;
            case 1:
                if (((Sound->SoundBits & 0xF0000000) == (arg1->SoundBits & 0xF0000000)) &&
                    (Sound->Position == arg1->unk00)) {
                    found = true;
                }
                break;
            case 2:
                if (Sound->Position == arg1->unk00) {
                    found = true;
                }
                break;
            case 3:
                if ((Sound->Position == arg1->unk00) && (Sound->SoundBits == arg1->SoundBits)) {
                    found = true;
                }
                break;
            case 4:
                if ((Sound->CameraId == arg1->CameraId) && (Sound->SoundBits == arg1->SoundBits)) {
                    found = true;
                }
                break;
            case 5:
                if (Sound->SoundBits == arg1->SoundBits) {
                    found = true;
                }
                break;
        }
        if (found) {
            Sound->SoundBits = 0;
            if (*Sound->Position != D_800EA1C8) {
                (*Sound->Position)[1] = 100000.0f;
            }
        }
    }
}

void NaiEntryBufferFx(void) {
    u8 bank;
    u8 soundIndex;
    u8 var_a3;
    struct Sound* var_a2;
    UNUSED s32 pad;
    u8 var_t2;
    u32 var_t3;

    var_a2 = &SSoundRequests[SNumProcessedSoundRequests];
    if (var_a2->SoundBits == 0) {
        return;
    }

    bank = ((u32) (var_a2->SoundBits & 0xF0000000) >> 0x1C);
    soundIndex = SSoundBanks[bank][0].Next;
    var_a3 = 0;
    while ((soundIndex != 0xFF) && (soundIndex != 0)) {
        if (var_a2->Position == SSoundBanks[bank][soundIndex].unk00) {
            if (var_a2->SoundBits == SSoundBanks[bank][soundIndex].SoundBits) {
                var_a3 = D_800EA1A0[D_800EA1C0][bank];
            } else {
                if (var_a3 == 0) {
                    var_t2 = soundIndex;
                    var_t3 = SSoundBanks[bank][soundIndex].SoundBits;
                } else if ((u32) (SSoundBanks[bank][soundIndex].SoundBits & 0xFF00) < (u32) (var_t3 & 0xFF00)) {
                    var_t2 = soundIndex;
                    var_t3 = SSoundBanks[bank][soundIndex].SoundBits;
                }
                var_a3++;
                if (var_a3 == D_800EA1A0[D_800EA1C0][bank]) {
                    if ((u32) (var_a2->SoundBits & 0xFF00) >= (u32) (var_t3 & 0xFF00)) {
                        soundIndex = var_t2;
                    } else {
                        soundIndex = 0;
                    }
                }
            }
            if (var_a3 == D_800EA1A0[D_800EA1C0][bank]) {
                if ((var_a2->SoundBits & 0x08000000) || (var_a2->SoundBits & 0x40000) || (soundIndex == var_t2)) {
                    if ((SSoundBanks[bank][soundIndex].SoundBits & 0x80000) &&
                        (SSoundBanks[bank][soundIndex].SoundStatus != 1)) {
                        NaiFxClearBgmMuteFlag(SSoundBanks[bank][soundIndex].unk2C);
                    }
                    SSoundBanks[bank][soundIndex].CameraId = var_a2->CameraId;
                    SSoundBanks[bank][soundIndex].SoundBits = var_a2->SoundBits;
                    SSoundBanks[bank][soundIndex].SoundStatus = ((var_a2->SoundBits & 0x01000000) >> 0x18);
                    SSoundBanks[bank][soundIndex].Freshness = 2;
                    SSoundBanks[bank][soundIndex].unk10 = var_a2->unk0C;
                    SSoundBanks[bank][soundIndex].unk14 = var_a2->unk10;
                    SSoundBanks[bank][soundIndex].unk18 = var_a2->unk14;
                }
                soundIndex = 0;
            }
        }
        if (soundIndex != 0) {
            soundIndex = SSoundBanks[bank][soundIndex].Next;
        }
    }
    if ((SSoundBanks[bank][SSoundBankFreeListFront[bank]].Next != 0xFF) && (soundIndex != 0)) {
        var_t2 = soundIndex = SSoundBankFreeListFront[bank];
        SSoundBanks[bank][soundIndex].unk00 = (Vec3f*) &(*var_a2->Position)[0];
        SSoundBanks[bank][soundIndex].unk04 = &(*var_a2->Position)[1];
        SSoundBanks[bank][soundIndex].unk08 = &(*var_a2->Position)[2];
        SSoundBanks[bank][soundIndex].CameraId = var_a2->CameraId;
        SSoundBanks[bank][soundIndex].unk10 = var_a2->unk0C;
        SSoundBanks[bank][soundIndex].unk14 = var_a2->unk10;
        SSoundBanks[bank][soundIndex].unk18 = var_a2->unk14;
        SSoundBanks[bank][soundIndex].SoundBits = var_a2->SoundBits;
        SSoundBanks[bank][soundIndex].SoundStatus = (u8) ((u32) (var_a2->SoundBits & 0x01000000) >> 0x18);
        SSoundBanks[bank][soundIndex].Freshness = 2;
        SSoundBanks[bank][soundIndex].Prev = SSoundBankUsedListBack[bank];
        SSoundBanks[bank][SSoundBankUsedListBack[bank]].Next = SSoundBankFreeListFront[bank];
        SSoundBankUsedListBack[bank] = SSoundBankFreeListFront[bank];
        SSoundBankFreeListFront[bank] = SSoundBanks[bank][SSoundBankFreeListFront[bank]].Next;
        SSoundBanks[bank][SSoundBankFreeListFront[bank]].Prev = 0xFF;
        SSoundBanks[bank][var_t2].Next = 0xFF;
    } else if (SSoundBanks[bank][SSoundBankFreeListFront[bank]].Next == 0xFF) {
        if (D_800EA1C8 != *var_a2->Position) {
            (*var_a2->Position)[1] = 100000.0f;
        }
    }
}

void NaiFxFlagFree(u8 BankId, u8 soundId) {
    UNUSED s32 stackPadding;
    struct SoundCharacteristics* temp = &SSoundBanks[BankId][soundId];
    if (*temp->unk00 != D_800EA1C8) {
        *temp->unk04 = 100000.0f;
    }
    if (temp->SoundBits & 0x80000) {
        NaiFxClearBgmMuteFlag(temp->unk2C);
    }
    if (soundId == SSoundBankUsedListBack[BankId]) {
        SSoundBankUsedListBack[BankId] = temp->Prev;
    } else {
        SSoundBanks[BankId][temp->Next].Prev = temp->Prev;
    }
    SSoundBanks[BankId][temp->Prev].Next = temp->Next;
    temp->Next = SSoundBankFreeListFront[BankId];
    temp->Prev = 0xFF;
    SSoundBanks[BankId][SSoundBankFreeListFront[BankId]].Prev = soundId;
    SSoundBankFreeListFront[BankId] = soundId;
    temp->SoundStatus = 0;
}

struct ActiveSfx {
    u32 Priority;
    u8 soundIndex;
};
#define AUDIO_MK_CMD(b0, b1, b2, b3) \
    ((((b0) & 0xFF) << 0x18) | (((b1) & 0xFF) << 0x10) | (((b2) & 0xFF) << 0x8) | (((b3) & 0xFF) << 0))
void NaiFxNextPlayCheck(u8 BankId) {
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
        chosenSfx[i].Priority = 0x7FFFFFFF;
        chosenSfx[i].soundIndex = 0xFF;
    }

    soundIndex = SSoundBanks[BankId][0].Next;
    k = 0;
    while (soundIndex != 0xFF) {
        if ((SSoundBanks[BankId][soundIndex].SoundStatus == 1) &&
            ((SSoundBanks[BankId][soundIndex].SoundBits & 0x08000000) == 0x08000000)) {
            SSoundBanks[BankId][soundIndex].Freshness -= 1;
        }

        if (SSoundBanks[BankId][soundIndex].Freshness == 0) {
            NaiFxFlagFree(BankId, soundIndex);
        } else if (SSoundBanks[BankId][soundIndex].SoundStatus != 0) {
            entry = &SSoundBanks[BankId][soundIndex];

            if (&D_800EA1C8 == entry[0].unk00) {
                entry->Distance = 0.0f;
            } else {
                entry->Distance = (*entry->unk00[0] * *entry->unk00[0]) + (*entry->unk08 * *entry->unk08);
            }
            requestedPriority = (((u32) (entry->SoundBits & 0xFF00)) >> 8);
            if (entry->SoundBits & 0x100000) {
                entry->Priority = ((0xFF - requestedPriority) * (0xFF - requestedPriority)) * (38 * 38);
            } else {
                entry->Priority =
                    ((u32) entry->Distance) + (((0xFF - requestedPriority) * (0xFF - requestedPriority)) * (38 * 38));

                if ((*entry->unk08) > 0.0f) {
                    entry->Priority += (s32) ((*entry->unk08) * 6.0f);
                }
            }
            temp_t8 = (((u32) (entry->SoundBits & 0x30000)) >> 0x10);
            if (temp_t8) {
                var_f0 = (2000.0f * 2000.0f) / ((f32) (temp_t8 * temp_t8));
            } else {
                var_f0 = 1e5f * 1e5f;
            }
            if (var_f0 < entry->Distance) {
                if (entry->SoundStatus == 4) {
                    NapSetS8(AUDIO_MK_CMD(0x06, 2, entry->unk2C, 0), 0);
                    if (entry->SoundBits & 0x08000000) {
                        NaiFxFlagFree(BankId, soundIndex);
                        soundIndex = k;
                    }
                }
            } else {
                numChannels = D_800EA188[D_800EA1C0][BankId];
                for (i = 0; i < numChannels; i++) {
                    if (chosenSfx[i].Priority >= entry->Priority) {
                        if (numChosenSfx < D_800EA188[D_800EA1C0][BankId]) {
                            numChosenSfx++;
                        }
                        for (j = numChannels - 1; j > i; j--) {
                            chosenSfx[j].Priority = chosenSfx[j - 1].Priority;
                            chosenSfx[j].soundIndex = chosenSfx[j - 1].soundIndex;
                        }

                        chosenSfx[i].Priority = entry->Priority;
                        chosenSfx[i].soundIndex = soundIndex;
                        i = numChannels;
                    }
                }
            }
            k = soundIndex;
        }
        soundIndex = SSoundBanks[BankId][k].Next;
    }

    numChannels = D_800EA188[D_800EA1C0][BankId];
    for (i = 0; i < numChosenSfx; i++) {
        if (SSoundBanks[BankId][chosenSfx[i].soundIndex].SoundStatus == 1) {
            SSoundBanks[BankId][chosenSfx[i].soundIndex].SoundStatus = 2;
        } else if (SSoundBanks[BankId][chosenSfx[i].soundIndex].SoundStatus == 4) {
            SSoundBanks[BankId][chosenSfx[i].soundIndex].SoundStatus = 3;
        }
    }

    for (i = 0; i < numChannels; i++) {
        needNewSfx = false;
        activeSfx = (struct ActiveSfx*) &D_80192AB8[BankId][i];

        if (activeSfx->soundIndex == 0xFF) {
            needNewSfx = true;
        } else {
            entry = &SSoundBanks[BankId][activeSfx->soundIndex];
            if (entry->SoundStatus == 4) {
                if (entry->SoundBits & 0x08000000) {
                    NaiFxFlagFree(BankId, activeSfx->soundIndex);
                } else {
                    entry->SoundStatus = 1;
                }
                needNewSfx = true;
            } else if (entry->SoundStatus == 0) {
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
                if ((chosenEntryIndex != 0xFF) && (SSoundBanks[BankId][chosenEntryIndex].SoundStatus != 3)) {
                    for (k = 0; k < numChannels; k++) {
                        if (chosenEntryIndex == ((struct ActiveSfx*) (D_80192AB8[BankId]))[k].soundIndex) {
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

void NaiFxNowPlayCheck(u8 BankId) {
    u8 soundId;
    u8 var_s4;
    UNUSED u32 cmd;
    struct SoundCharacteristics* temp_s0;
    struct SequenceChannel* thing;

    for (var_s4 = 0; var_s4 < D_800EA188[D_800EA1C0][BankId]; var_s4++) {
        soundId = D_80192AB8[BankId][var_s4][4];
        if (soundId != 0xFF) {
            temp_s0 = &SSoundBanks[BankId][soundId];
            thing = SeqPlayer[2].Channels[D_80192C38];
            if (temp_s0->SoundStatus == 2) {
                temp_s0->unk2C = D_80192C38;
                if (temp_s0->SoundBits & 0x80000) {
                    D_800EA1C4 |= 1 << D_80192C38;
                    D_801930D0->Unk012 = 1;
                    D_801930D0->Unk00E[2] = 0x28;
                }
                NaaCalcCameraPos(BankId, soundId, D_80192C38);
                NapSetS8(((D_80192C38 & 0xFF) << 8) | 0x06020000, 1);
                NapSetS8(((D_80192C38 & 0xFF) << 8) | 0x06020000 | 4, (u8) (temp_s0->SoundBits & 0xFF));
                temp_s0->SoundStatus = 4U;
            } else if (((u8) thing->soundScriptIO[7]) == 0x80) {
                NapSetS8(((D_80192C38 & 0xFF) << 8) | 0x06020000 | 7, 0);
                NaiFxFlagFree(BankId, soundId);
            } else if (temp_s0->SoundStatus == 3) {
                NaaCalcCameraPos(BankId, soundId, D_80192C38);
                temp_s0->SoundStatus = 4U;
            }
        }
        D_80192C38 += 1;
    }
}

// Seems somewhat similar to certain parts of `select_current_sounds` from SM64
void NaiStopAllHandleFx(u8 BankId) {
    UNUSED s32 stackPadding0;
    u8 soundId;
    struct SoundCharacteristics sp60;

    soundId = SSoundBanks[BankId][0].Next;
    while (soundId != 0xFF) {
        if (SSoundBanks[BankId][soundId].SoundStatus >= 3) {
            NapSetS8(((SSoundBanks[BankId][soundId].unk2C & 0xff) << 8) | 0x06020000, 0);
        }
        if (SSoundBanks[BankId][soundId].SoundStatus != 0) {
            NaiFxFlagFree(BankId, soundId);
        }
        // This feels wrong, I think it only works because `NaiFxFlagFree`
        // modifies `SSoundBanks[BankId][0].Next`, which allows us to eventually(?)
        // break out of the Loop
        soundId = SSoundBanks[BankId][0].Next;
    }
    sp60.SoundBits = BankId << 0x1C;
    NaiStopBufferFx(0, &sp60);
}

void NaiStopAllObjHandleFx2(u8 arg0, Vec3f* arg1) {
    u8 curr;
    u8 Next;

    curr = 0;
    Next = SSoundBanks[arg0][0].Next;

    while (Next != 0xff) {
        if (*arg1 == *SSoundBanks[arg0][Next].unk00) {
            if (SSoundBanks[arg0][Next].SoundStatus >= 3) {
                NapSetS8((0x06020000 | ((SSoundBanks[arg0][Next].unk2C & 0xff) << 8)), 0);
            }

            if (SSoundBanks[arg0][Next].SoundStatus != 0) {
                NaiFxFlagFree(arg0, Next);
            }
        } else {
            curr = Next;
        }

        Next = SSoundBanks[arg0][curr].Next;
    }
}

void NaiStopAllObjHandleFx(u8 arg0, Vec3f* arg1) {
    struct SoundCharacteristics unk;
    NaiStopAllObjHandleFx2(arg0, arg1);
    unk.SoundBits = arg0 << 0x1C;
    unk.unk00 = arg1;
    NaiStopBufferFx(1, &unk);
}

void NaiStopAllObjFx(Vec3f* arg0) {
    u8 i;
    struct SoundCharacteristics sp3C;

    for (i = 0; i < 6; i++) {
        NaiStopAllObjHandleFx2(i, arg0);
    }

    sp3C.unk00 = arg0;
    NaiStopBufferFx(2, &sp3C);
}

void NaiStopObjFlagFx(Vec3f* arg0, u32 SoundBits) {
    UNUSED s32 stackPadding0;
    u8 BankId;
    u8 Next;
    u8 curr;
    struct SoundCharacteristics sp60;

    BankId = (SoundBits & 0xF0000000) >> 0x1C;
    Next = SSoundBanks[BankId][0].Next;
    curr = 0;
    while (Next != 0xFF) {
        if ((arg0 == SSoundBanks[BankId][Next].unk00) && (SoundBits == SSoundBanks[BankId][Next].SoundBits)) {
            if (SSoundBanks[BankId][Next].SoundStatus >= 3) {
                NapSetS8(((SSoundBanks[BankId][Next].unk2C & 0xff) << 8) | 0x06020000, 0);
            }
            if (SSoundBanks[BankId][Next].SoundStatus != 0) {
                NaiFxFlagFree(BankId, Next);
            }
            Next = 0xFF;
        } else {
            curr = Next;
        }
        if (Next != 0xFF) {
            Next = SSoundBanks[BankId][curr].Next;
        }
    }
    sp60.unk00 = arg0;
    sp60.SoundBits = SoundBits;
    NaiStopBufferFx(3, &sp60);
}

void NaiStopFx(u32 SoundBits) {
    UNUSED s32 stackPadding0;
    u8 BankId;
    u8 Next;
    u8 curr;
    struct SoundCharacteristics sp68;

    BankId = (SoundBits & 0xF0000000) >> 0x1C;
    Next = SSoundBanks[BankId][0].Next;
    curr = 0;
    while (Next != 0xFF) {
        if (SoundBits == SSoundBanks[BankId][Next].SoundBits) {
            if (SSoundBanks[BankId][Next].SoundStatus >= 3) {
                NapSetS8(((SSoundBanks[BankId][Next].unk2C & 0xff) << 8) | 0x06020000, 0);
            }
            if (SSoundBanks[BankId][Next].SoundStatus != 0) {
                NaiFxFlagFree(BankId, Next);
            }
        } else {
            curr = Next;
        }
        Next = SSoundBanks[BankId][curr].Next;
    }
    sp68.SoundBits = SoundBits;
    NaiStopBufferFx(5, &sp68);
}

// Might be process_all_sound_requests
void NaiFxBufFlagCheck(void) {
    while (SSoundRequestCount != SNumProcessedSoundRequests) {
        NaiEntryBufferFx();
        SNumProcessedSoundRequests++;
    }
}

// EXTREMELY similar to `NaiSetFxHandleVolume` from SM64
void NaiSetFxHandleVolume(u8 Player, u8 targetScale, u16 fadeDuration) {
    struct ChannelVolumeScaleFade* temp_v0;

    if (fadeDuration == 0) {
        fadeDuration++;
    }
    temp_v0 = &D_80192C48[Player];
    temp_v0->Target = targetScale / US_FLOAT(127.0);
    temp_v0->RemainingFrames = fadeDuration;
    temp_v0->Velocity = (temp_v0->Current - temp_v0->Target) / fadeDuration;
}

void NaiFxHandleFrameCheck(u8 arg0) {
    struct ChannelVolumeScaleFade* temp_v0;

    temp_v0 = &D_80192C48[arg0];
    if (temp_v0->RemainingFrames != 0) {
        temp_v0->RemainingFrames--;
        if (temp_v0->RemainingFrames != 0) {
            temp_v0->Current -= temp_v0->Velocity;
        } else {
            temp_v0->Current = temp_v0->Target;
        }
    }
}

void NaiFxPlayCheck(void) {
    u8 i;

    if (SeqPlayer[2].Channels[0] != &SequenceChannelNone) {
        D_80192C38 = 0;
        for (i = 0; i < 6; i++) {
            NaiFxNextPlayCheck(i);
            NaiFxNowPlayCheck(i);
            NaiFxHandleFrameCheck(i);
        }
    }
}

// Looks EXTEREMELY similar to NaiFxInterInit from SM64
void NaiFxInterInit(void) {
    u8 var_v0;
    u8 var_v1;

    SSoundRequestCount = 0;
    SNumProcessedSoundRequests = 0;
    D_800EA1C4 = 0;
    for (var_v0 = 0; var_v0 < SOUND_BANK_COUNT; var_v0++) {
        SSoundBankUsedListBack[var_v0] = 0;
        SSoundBankFreeListFront[var_v0] = 1;
        SNumSoundsInBank[var_v0] = 0;
        SSoundBankDisabled[var_v0] = false;
        D_80192C48[var_v0].Current = 1.0f;
        D_80192C48[var_v0].RemainingFrames = 0;
    }
    for (var_v0 = 0; var_v0 < 6; var_v0++) {
        for (var_v1 = 0; var_v1 < 8; var_v1++) {
            D_80192AB8[var_v0][var_v1][4] = 0xFF;
        }
    }
    for (var_v0 = 0; var_v0 < 6; var_v0++) {
        SSoundBanks[var_v0][0].Prev = 0xFF;
        SSoundBanks[var_v0][0].Next = 0xFF;
        for (var_v1 = 1; var_v1 < 19; var_v1++) {
            SSoundBanks[var_v0][var_v1].Prev = var_v1 - 1;
            SSoundBanks[var_v0][var_v1].Next = var_v1 + 1;
        }
        SSoundBanks[var_v0][var_v1].Prev = var_v1 - 1;
        SSoundBanks[var_v0][var_v1].Next = 0xFF;
    }
}

void NaaPlyEngWaveLoad(void) {
    if (D_800EA1C0 == 0) {
        NapSetS8(((D_800EA154[GamePlayers[0].Kart] & 0xFFFF) << 8) | 0xF3000000, 0);
    } else {
        NapSetS8(0xF3004D00, 0);
    }
}

void func_800C5C40(void) {
    NaiStopSeqSet();
    if (D_800E9DA0 != 0) {
        NaiSEQInterInitSpec();
    } else {
        D_800E9DA0++;
        NaiSEQInterInit();
    }
    NaiFxInterInit();
    NaiSeqControl(2, 0, 0xFF, 1);
    NaaPlyEngWaveLoad();
}

// game_audio_init?
void NaiInitInterface(void) {
    NaiStopSeqSet();
    NaiSEQInterInit();
    NaiFxInterInit();
    NaiSeqControl(2, 0, 0xFF, 1);
}

void AccelOnoff(u8 PlayerId) {
    if ((GamePlayers[PlayerId].HandlingFlag & ACCELERATE) == ACCELERATE) {
        D_800E9E34[PlayerId] = 0;
        if (D_800E9E24[PlayerId] < 0x4E20) {
            if ((u8) GamePausedFlag == 0) {
                D_800E9E24[PlayerId]++;
            }
        }
        if (D_800E9E24[PlayerId] == 1) {
            switch (D_800EA0EC[PlayerId]) {
                case 2:
                    D_800EA0EC[PlayerId] = 1;
                case 0:
                    NaPlyEngStop(PlayerId);
                    NaPlyEngStart(PlayerId);
                    break;
                default:
                    break;
            }
        }
    } else {
        D_800E9E24[PlayerId] = 0;
        if ((D_800E9E34[PlayerId] < 0x4E20) && ((u8) GamePausedFlag == 0)) {
            D_800E9E34[PlayerId]++;
        }
    }
}

void AccelVolume(u8 PlayerId) {
    if (D_800EA108 == 0) {
        if (((GamePlayers[PlayerId].HandlingFlag & ACCELERATE) != ACCELERATE) && (GamePlayers[PlayerId].Taco > 400.0f)) {
            D_800E9E14[PlayerId] = 1;
            if (D_800EA0EC[PlayerId] == 0) {
                D_800E9F7C[PlayerId].unk_10 = 0.6f - D_800E9F54[PlayerId];
            }
            D_800E9DC4[PlayerId] = D_800E9F7C[PlayerId].unk_30;
            D_800E9DD4[PlayerId] = D_800E9F7C[PlayerId].unk_24;
            if ((D_800E9E34[PlayerId] == 1) && ((u8) GamePausedFlag == 0)) {
                switch (D_800EA0EC[PlayerId]) {
                    case 2:
                        D_800EA0EC[PlayerId] = 1;
                    case 0:
                        if ((AsmDisableEngineSound[PlayerId] == 0) && (GamePlayers[PlayerId].Kart != 3)) {
                            if ((s32) D_800EA1C0 < 2) {
                                NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF9, 0x26), &D_800E9F7C[PlayerId].pos, PlayerId,
                                           &D_800E9F7C[PlayerId].unk_38, &D_800E9F04[PlayerId],
                                           (s8*) &D_800E9F7C[PlayerId].unk_14);
                            } else {
                                NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0x80, 0x26), &D_800E9F7C[PlayerId].pos, PlayerId,
                                           &D_800E9F7C[PlayerId].unk_38, &D_800E9F04[PlayerId],
                                           (s8*) &D_800E9F7C[PlayerId].unk_14);
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
        } else {
            if (D_800E9E24[PlayerId] == 0x0000000A) {
                if ((s32) D_800EA1C0 < 2) {
                    NaPlyLevelStop(PlayerId, SOUND_ARG_LOAD(0x01, 0x00, 0xF9, 0x26));
                } else {
                    NaPlyLevelStop(PlayerId, SOUND_ARG_LOAD(0x01, 0x00, 0x80, 0x26));
                }
            }
            D_800E9E14[PlayerId] = 0;
            D_800E9F7C[PlayerId].unk_10 = (1.0f - D_800E9F54[PlayerId]) - D_800EA130[PlayerId];
            if ((1.0f - D_800E9F54[PlayerId]) < D_800EA130[PlayerId]) {
                D_800E9F7C[PlayerId].unk_10 = 0.0f;
            }
            D_800E9DC4[PlayerId] = D_800E9F7C[PlayerId].unk_2C;
            D_800E9DD4[PlayerId] = D_800E9F7C[PlayerId].unk_20;
        }
    }
}

void EngineFreqSet(u8 PlayerId) {
    Player* Player;

    Player = &GamePlayers[PlayerId];
    D_800E9E64[PlayerId] = (Player->Taco / D_800E9DC4[PlayerId]) + D_800E9DD4[PlayerId];
    if ((Player->Taco < 1800.0f) && ((Player->HandlingFlag & ACCELERATE) != ACCELERATE)) {
        D_800E9E64[PlayerId] = (Player->Taco / D_800E9F7C[PlayerId].unk_34) + D_800E9F7C[PlayerId].unk_28;
        if (D_800E9EC4) {} // ?
    }
    if (Player->Speed > 4.75f) {
        if (D_800E9EB4[PlayerId] < (D_800E9F7C[PlayerId].unk_18 + 0.4f)) {
            D_800E9DE4[PlayerId] += 0.005f;
        }
    } else {
        D_800E9DE4[PlayerId] = 0.0f;
    }
    if (GamePlayers[PlayerId].Slipang > 0) {
        D_800E9E54[PlayerId] = (f32) Player->Slipang;
    } else {
        D_800E9E54[PlayerId] = (f32) -Player->Slipang;
    }
    if ((Player->SlipFlag & DRIFT) == DRIFT) {
        D_800E9EB4[PlayerId] = D_800E9E64[PlayerId] + D_800E9DE4[PlayerId];
    } else {
        D_800E9EB4[PlayerId] = D_800E9E64[PlayerId] + D_800E9DE4[PlayerId] - (D_800E9E54[PlayerId] / 12000.0f);
        if (D_800E9EB4[PlayerId] < 0.01f) {
            D_800E9EB4[PlayerId] = 0.01f;
        }
    }
    if ((AsmDisableEngineSound[PlayerId]) || (D_800E9F78[PlayerId])) {
        if (AsmDisableEngineSound[PlayerId]) {
            if (D_800E9F78[PlayerId] < 0xF) {
                D_800E9F78[PlayerId]++;
                D_800E9DF4[PlayerId] += 0.03f;
            }
        } else if (D_800E9F78[PlayerId]) {
            D_800E9F78[PlayerId]--;
            D_800E9DF4[PlayerId] -= 0.03f;
        }
        D_800E9EB4[PlayerId] -= D_800E9DF4[PlayerId];
    }
    D_800E9EE4[PlayerId] = D_800E9EB4[PlayerId] - D_800E9EC4[PlayerId];
#ifdef VERSION_EU
    if ((D_800E9EE4[PlayerId] > 0.5f) || (D_800E9EE4[PlayerId] < -0.5f))
#else
    if ((D_800E9EE4[PlayerId] > 0.5f) || (D_800E9EE4[PlayerId] < 0.5f))
#endif
    {
        D_800E9ED4[PlayerId] = D_800E9EE4[PlayerId] * 0.25f;
        D_800E9F7C[PlayerId].unk_0C = D_800E9EC4[PlayerId] + D_800E9ED4[PlayerId] + D_800E9F34[PlayerId];
    } else {
        D_800E9F7C[PlayerId].unk_0C = D_800E9EB4[PlayerId] + D_800E9F34[PlayerId];
    }
#ifdef VERSION_EU
    if (D_800E9F7C[PlayerId].unk_0C < 0.0f) {
        D_800E9F7C[PlayerId].unk_0C = 0.0f;
    }
#endif
    if (D_800E9F7C[PlayerId].unk_0C > 4.0f) {
        D_800E9F7C[PlayerId].unk_0C = 4.0f;
    }
    D_800E9EC4[PlayerId] = D_800E9F7C[PlayerId].unk_0C;
    D_800E9F7C[PlayerId].unk_38 = (D_800E9F7C[PlayerId].unk_0C / 1.5f) + 0.4f;
}

void LevelVolumeSet(u8 PlayerId) {
    switch (PlayerSurfaceSoundID[PlayerId]) {
        case 3:
            D_800E9EF4[PlayerId] = (GamePlayers[PlayerId].Speed / 5.0f) + 0.2f;
            break;
        case 1:
        case 13:
        case 14:
        case 17:
            D_800E9EF4[PlayerId] = ((D_800E9E54[PlayerId] - 3500.0f) / 3000.0f) + 0.4f;
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
            D_800E9EF4[PlayerId] = (GamePlayers[PlayerId].Speed / 5.0f) + 0.2f;
            break;
        default:
            D_800E9EF4[PlayerId] = 1.0f;
            break;
    }
    if (D_800E9EF4[PlayerId] > 1.0f) {
        D_800E9EF4[PlayerId] = 1.0f;
    }
    if (D_800E9EF4[PlayerId] < 0.0f) {
        D_800E9EF4[PlayerId] = 0.0f;
    }
    if ((GamePlayers[PlayerId].HandlingFlag & ACCELERATE) == ACCELERATE) {
        D_800E9F04[PlayerId] = 0.56f - (D_800E9E24[PlayerId] * 0.06f);
    } else {
        D_800E9F04[PlayerId] = (D_800E9E34[PlayerId] / 50.0f) + 0.25f;
    }
    if (LightningFlagPlayer1[PlayerId] != 0) {
        D_800E9F04[PlayerId] = 0.0f;
    }
    if (D_800E9F04[PlayerId] > 0.9f) {
        D_800E9F04[PlayerId] = 0.9f;
    }
}

void LevelFreqSet(u8 PlayerId) {
    switch (PlayerSurfaceSoundID[PlayerId]) { /* irregular */
        case 3:
            D_800E9F14[PlayerId] = (GamePlayers[PlayerId].Speed / 9.0f) + 0.6f;
            break;
        case 2:
        case 13:
        case 17:
            D_800E9F14[PlayerId] = (D_800E9E54[PlayerId] / 13000.0f) + 0.95f;
            break;
        default:
            D_800E9F14[PlayerId] = 1.0f;
            break;
    }
}

void LevelSoundTrigger(u8 CameraId) {
    if ((D_800EA108 == 0) && (D_800EA0EC[CameraId] == 0)) {
        if (PlayerSurfaceSoundID[CameraId] != D_800E9E84[CameraId]) {
            NaiStopObjFlagFx(&D_800E9F7C[CameraId].pos, D_800E9E94[CameraId]);
            switch (PlayerSurfaceSoundID[CameraId]) {
                case 3:
                    NAIFxFlagEntry(SOUND_ACTION_TIRE_SQUEAL, &D_800E9F7C[CameraId].pos, CameraId, &D_800E9F14[CameraId],
                               &D_800E9EF4[CameraId], (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ACTION_TIRE_SQUEAL;
                    break;
                case 18:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF8, 0x1D), &D_800E9F7C[CameraId].pos, CameraId, &D_800EA1D4,
                               &D_800EA1D4, (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF8, 0x1D);
                    break;
                case 19:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF8, 0x22), &D_800E9F7C[CameraId].pos, CameraId, &D_800EA1D4,
                               &D_800EA1D4, (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF8, 0x22);
                    break;
                case 1:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x09), &D_800E9F7C[CameraId].pos, CameraId, &D_800EA1D4,
                               &D_800E9EF4[CameraId], (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x09);
                    break;
                case 2:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF4, 0x0A), &D_800E9F7C[CameraId].pos, CameraId,
                               &D_800E9F14[CameraId], &D_800EA1D4, (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF4, 0x0A);
                    break;
                case 17:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x1E), &D_800E9F7C[CameraId].pos, CameraId,
                               &D_800E9F14[CameraId], &D_800E9EF4[CameraId], (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x1E);
                    break;
                case 15:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x1F), &D_800E9F7C[CameraId].pos, CameraId,
                               &D_800E9F14[CameraId], &D_800EA1D4, (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x1F);
                    break;
                case 16:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x21), &D_800E9F7C[CameraId].pos, CameraId,
                               &D_800E9F14[CameraId], &D_800EA1D4, (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x21);
                    break;
                case 20:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x27), &D_800E9F7C[CameraId].pos, CameraId,
                               &D_800E9F14[CameraId], &D_800EA1D4, (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x27);
                    break;
                case 25:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x20), &D_800E9F7C[CameraId].pos, CameraId,
                               &D_800E9F14[CameraId], &D_800EA1D4, (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x20);
                    break;
                case 26:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x23), &D_800E9F7C[CameraId].pos, CameraId,
                               &D_800E9F14[CameraId], &D_800EA1D4, (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x23);
                    break;
                case 27:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0x80, 0x46), &D_800E9F7C[CameraId].pos, CameraId,
                               &D_800E9F14[CameraId], &D_800EA1D4, (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0x80, 0x46);
                    break;
                case 28:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x25), &D_800E9F7C[CameraId].pos, CameraId,
                               &D_800E9F14[CameraId], &D_800EA1D4, (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x25);
                    break;
                case 4:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0B), &D_800E9F7C[CameraId].pos, CameraId, &D_800EA1D4,
                               &D_800E9EF4[CameraId], (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0B);
                    break;
                case 5:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0C), &D_800E9F7C[CameraId].pos, CameraId, &D_800EA1D4,
                               &D_800E9EF4[CameraId], (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0C);
                    break;
                case 6:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0D), &D_800E9F7C[CameraId].pos, CameraId, &D_800EA1D4,
                               &D_800E9EF4[CameraId], (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0D);
                    break;
                case 7:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0E), &D_800E9F7C[CameraId].pos, CameraId, &D_800EA1D4,
                               &D_800E9EF4[CameraId], (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0E);
                    break;
                case 8:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0F), &D_800E9F7C[CameraId].pos, CameraId, &D_800EA1D4,
                               &D_800E9EF4[CameraId], (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0F);
                    break;
                case 9:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x10), &D_800E9F7C[CameraId].pos, CameraId, &D_800EA1D4,
                               &D_800E9EF4[CameraId], (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x10);
                    break;
                case 10:
                case 14:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x11), &D_800E9F7C[CameraId].pos, CameraId, &D_800EA1D4,
                               &D_800E9EF4[CameraId], (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x11);
                    break;
                case 11:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x12), &D_800E9F7C[CameraId].pos, CameraId, &D_800EA1D4,
                               &D_800E9EF4[CameraId], (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x12);
                    break;
                case 12:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x13), &D_800E9F7C[CameraId].pos, CameraId, &D_800EA1D4,
                               &D_800E9EF4[CameraId], (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x13);
                    break;
                case 29:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x48), &D_800E9F7C[CameraId].pos, CameraId, &D_800EA1D4,
                               &D_800E9EF4[CameraId], (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x48);
                    break;
                case 30:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x49), &D_800E9F7C[CameraId].pos, CameraId, &D_800EA1D4,
                               &D_800E9EF4[CameraId], (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x49);
                    break;
                case 31:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x4A), &D_800E9F7C[CameraId].pos, CameraId, &D_800EA1D4,
                               &D_800E9EF4[CameraId], (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x4A);
                    break;
                case 13:
                case 22:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x29), &D_800E9F7C[CameraId].pos, CameraId, &D_800EA1D4,
                               &D_800E9EF4[CameraId], (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x29);
                    break;
                case 23:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x2A), &D_800E9F7C[CameraId].pos, CameraId, &D_800EA1D4,
                               &D_800E9EF4[CameraId], (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x2A);
                    break;
                case 24:
                    NAIFxFlagEntry(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x2B), &D_800E9F7C[CameraId].pos, CameraId, &D_800EA1D4,
                               &D_800E9EF4[CameraId], (s8*) &D_800E9F7C[CameraId].unk_14);
                    D_800E9E94[CameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x2B);
                    break;
                default:
                    NaiStopObjFlagFx(&D_800E9F7C[CameraId].pos, D_800E9E94[CameraId]);
                    break;
            }
        }
        D_800E9E84[CameraId] = PlayerSurfaceSoundID[CameraId];
    }
}

void MakeLevelSoundTrg(u8 PlayerId) {
    if (D_800EA0EC[PlayerId] == 0) {
        PlayerSurfaceSoundID[PlayerId] = 0;
        if ((D_800E9E54[PlayerId] > 3500.0f) || ((GamePlayers[PlayerId].SlipFlag & DRIFT) == DRIFT)) {
            PlayerSurfaceSoundID[PlayerId] = 1;
            switch (GamePlayers[PlayerId].TireRL.Status) {
                case DIRT: /* switch 1 */
                    PlayerSurfaceSoundID[PlayerId] = 0x0000000D;
                    break;
                case SAND: /* switch 1 */
                    PlayerSurfaceSoundID[PlayerId] = 0x0000000E;
                    break;
                case BRIDGE: /* switch 1 */
                    PlayerSurfaceSoundID[PlayerId] = 0x00000011;
                    break;
                case STONE: /* switch 1 */
                    PlayerSurfaceSoundID[PlayerId] = 0x0000000F;
                    break;
                case SNOW: /* switch 1 */
                    PlayerSurfaceSoundID[PlayerId] = 0x00000010;
                    break;
                case ICE: /* switch 1 */
                    PlayerSurfaceSoundID[PlayerId] = 0x00000014;
                    break;
                case ROPE_BRIDGE: /* switch 1 */
                    PlayerSurfaceSoundID[PlayerId] = 0x00000019;
                    break;
                case WOOD_BRIDGE: /* switch 1 */
                    PlayerSurfaceSoundID[PlayerId] = 0x0000001A;
                    break;
                case TRAIN_TRACK: /* switch 1 */
                    PlayerSurfaceSoundID[PlayerId] = 0x0000001B;
                    break;
            }
        }
        if ((GamePlayers[PlayerId].SlipFlag & DRIFT) == DRIFT) {
            PlayerSurfaceSoundID[PlayerId] = 2;
            switch (GamePlayers[PlayerId].TireRL.Status) { /* switch 2 */
                case DIRT:                                                   /* switch 2 */
                    PlayerSurfaceSoundID[PlayerId] = 0x0000000D;
                    break;
                case SAND: /* switch 2 */
                    PlayerSurfaceSoundID[PlayerId] = 0x0000000E;
                    break;
                case BRIDGE: /* switch 2 */
                    PlayerSurfaceSoundID[PlayerId] = 0x00000011;
                    break;
                case STONE: /* switch 2 */
                    PlayerSurfaceSoundID[PlayerId] = 0x0000000F;
                    break;
                case SNOW: /* switch 2 */
                    PlayerSurfaceSoundID[PlayerId] = 0x00000010;
                    break;
                case ICE: /* switch 2 */
                    PlayerSurfaceSoundID[PlayerId] = 0x00000014;
                    break;
                case ROPE_BRIDGE: /* switch 2 */
                    PlayerSurfaceSoundID[PlayerId] = 0x00000019;
                    break;
                case WOOD_BRIDGE: /* switch 2 */
                    PlayerSurfaceSoundID[PlayerId] = 0x0000001A;
                    break;
                case TRAIN_TRACK: /* switch 2 */
                    PlayerSurfaceSoundID[PlayerId] = 0x0000001B;
                    break;
            }
        }
        switch (GamePlayers[PlayerId].TireRL.Status) { /* switch 3 */
            case GRASS:                                                  /* switch 3 */
                if (PlayerSurfaceSoundID[PlayerId] == 6) {
                    PlayerSurfaceSoundID[PlayerId] = 4;
                } else if (D_800EA1C0 == 0) {
                    PlayerSurfaceSoundID[PlayerId] = 5;
                } else {
                    PlayerSurfaceSoundID[PlayerId] = 4;
                }
                break;
            case SAND_OFFROAD: /* switch 3 */
                if (PlayerSurfaceSoundID[PlayerId] == 0x0000000C) {
                    PlayerSurfaceSoundID[PlayerId] = 0x0000000A;
                } else if (D_800EA1C0 == 0) {
                    PlayerSurfaceSoundID[PlayerId] = 0x0000000B;
                } else {
                    PlayerSurfaceSoundID[PlayerId] = 0x0000000A;
                }
                break;
            case DIRT_OFFROAD: /* switch 3 */
                if (PlayerSurfaceSoundID[PlayerId] == 0x00000018) {
                    PlayerSurfaceSoundID[PlayerId] = 0x00000016;
                } else if (D_800EA1C0 == 0) {
                    PlayerSurfaceSoundID[PlayerId] = 0x00000017;
                } else {
                    PlayerSurfaceSoundID[PlayerId] = 0x00000016;
                }
                break;
            case SNOW_OFFROAD: /* switch 3 */
                if (PlayerSurfaceSoundID[PlayerId] == 0x0000001F) {
                    PlayerSurfaceSoundID[PlayerId] = 0x0000001D;
                } else if (D_800EA1C0 == 0) {
                    PlayerSurfaceSoundID[PlayerId] = 0x0000001E;
                } else {
                    PlayerSurfaceSoundID[PlayerId] = 0x0000001D;
                }
                break;
            case WET_SAND: /* switch 3 */
                if (AsmDisableEngineSound[PlayerId] == 0) {
                    if (PlayerSurfaceSoundID[PlayerId] == 9) {
                        PlayerSurfaceSoundID[PlayerId] = 7;
                    } else if (D_800EA1C0 == 0) {
                        PlayerSurfaceSoundID[PlayerId] = 8;
                    } else {
                        PlayerSurfaceSoundID[PlayerId] = 7;
                    }
                } else {
                    PlayerSurfaceSoundID[PlayerId] = 0x0000001C;
                }
                break;
            case ROPE_BRIDGE: /* switch 3 */
                PlayerSurfaceSoundID[PlayerId] = 0x00000019;
                break;
            case WOOD_BRIDGE: /* switch 3 */
                PlayerSurfaceSoundID[PlayerId] = 0x0000001A;
                break;
            case TRAIN_TRACK: /* switch 3 */
                PlayerSurfaceSoundID[PlayerId] = 0x0000001B;
                break;
        }
        switch (GamePlayers[PlayerId].TireRR.Status) { /* switch 4 */
            case GRASS:                                                   /* switch 4 */
                if (PlayerSurfaceSoundID[PlayerId] == 5) {
                    PlayerSurfaceSoundID[PlayerId] = 4;
                } else if (D_800EA1C0 == 0) {
                    PlayerSurfaceSoundID[PlayerId] = 6;
                } else {
                    PlayerSurfaceSoundID[PlayerId] = 4;
                }
                break;
            case SAND_OFFROAD: /* switch 4 */
                if (PlayerSurfaceSoundID[PlayerId] == 0x0000000B) {
                    PlayerSurfaceSoundID[PlayerId] = 0x0000000A;
                } else if (D_800EA1C0 == 0) {
                    PlayerSurfaceSoundID[PlayerId] = 0x0000000C;
                } else {
                    PlayerSurfaceSoundID[PlayerId] = 0x0000000A;
                }
                break;
            case DIRT_OFFROAD: /* switch 4 */
                if (PlayerSurfaceSoundID[PlayerId] == 0x00000017) {
                    PlayerSurfaceSoundID[PlayerId] = 0x00000016;
                } else if (D_800EA1C0 == 0) {
                    PlayerSurfaceSoundID[PlayerId] = 0x00000018;
                } else {
                    PlayerSurfaceSoundID[PlayerId] = 0x00000016;
                }
                break;
            case SNOW_OFFROAD: /* switch 4 */
                if (PlayerSurfaceSoundID[PlayerId] == 0x0000001E) {
                    PlayerSurfaceSoundID[PlayerId] = 0x0000001D;
                } else if (D_800EA1C0 == 0) {
                    PlayerSurfaceSoundID[PlayerId] = 0x0000001F;
                } else {
                    PlayerSurfaceSoundID[PlayerId] = 0x0000001D;
                }
                break;
            case WET_SAND: /* switch 4 */
                if (AsmDisableEngineSound[PlayerId] == 0) {
                    if (PlayerSurfaceSoundID[PlayerId] == 8) {
                        PlayerSurfaceSoundID[PlayerId] = 7;
                    } else if (D_800EA1C0 == 0) {
                        PlayerSurfaceSoundID[PlayerId] = 9;
                    } else {
                        PlayerSurfaceSoundID[PlayerId] = 7;
                    }
                } else {
                    PlayerSurfaceSoundID[PlayerId] = 0x0000001C;
                }
                break;
            case ROPE_BRIDGE: /* switch 4 */
                PlayerSurfaceSoundID[PlayerId] = 0x00000019;
                break;
            case WOOD_BRIDGE: /* switch 4 */
                PlayerSurfaceSoundID[PlayerId] = 0x0000001A;
                break;
            case TRAIN_TRACK: /* switch 4 */
                PlayerSurfaceSoundID[PlayerId] = 0x0000001B;
                break;
        }
        if (((GamePlayers[PlayerId].Speed < 0.5f) || ((GamePlayers[PlayerId].SlipFlag & N_JUMP) == N_JUMP)) &&
            (PlayerSurfaceSoundID[PlayerId] != 0x0000001C)) {
            PlayerSurfaceSoundID[PlayerId] = 0;
        }
        if ((((GamePlayers[PlayerId].SlipFlag & RAPID_ACC) == RAPID_ACC) &&
             ((GamePlayers[PlayerId].Flag & NO_CONTROLS) != NO_CONTROLS)) ||
            ((GamePlayers[PlayerId].SlipFlag & WHEELSPIN) == WHEELSPIN) ||
            ((GamePlayers[PlayerId].SlipFlag & SPIN_L) == SPIN_L) ||
            ((GamePlayers[PlayerId].SlipFlag & SPIN_R) == SPIN_R) ||
            ((GamePlayers[PlayerId].HandlingFlag & SPINOUT_SWERVE) == SPINOUT_SWERVE)) {
            PlayerSurfaceSoundID[PlayerId] = 0x00000012;
        }
        if ((((GamePlayers[PlayerId].SlipFlag & SPIN_TURN) == SPIN_TURN) &&
             ((GamePlayers[PlayerId].Flag & NO_CONTROLS) != NO_CONTROLS)) ||
            ((GamePlayers[PlayerId].SlipFlag & WHEELSPIN) == WHEELSPIN)) {
            PlayerSurfaceSoundID[PlayerId] = 0x00000013;
        }
        if (GamePlayers[PlayerId].BrakeTime != 0.0f) {
            PlayerSurfaceSoundID[PlayerId] = 3;
        }
    }
}

/* 
   Routine to tell the game which "finish" music to play depending on which Mode the Player is in (Grand Prix, Time Trials, etc),
   whether the game is in multiplayer or not, and which Position they ended up finishing in (1st, 2nd, 3rd, etc)

   Contains a LOT of inlined funcs. 
   Modify if you dare.
*/
void GoalRoutine(u8 PlayerId) {
    if (D_800E9EA4[PlayerId] != 0) {
        if (D_800E9EA4[PlayerId] < 0x2BC) {
            D_800E9EA4[PlayerId]++;
        }
        if (D_800E9EA4[PlayerId] == 2) {
            NaPlyLevelStop(PlayerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x28));
            NaPlyLevelStop(PlayerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFF, 0x2C));
            NaPlyLevelStop(PlayerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x4C));
            NaiStopObjFlagFx(&D_800E9F7C[PlayerId].pos, D_800E9E94[PlayerId]);
            PlayerSurfaceSoundID[PlayerId] = 0;
            switch (GameMode) { /* irregular */
                case GRAND_PRIX:
                    D_800EA0EC[PlayerId] = 2;
                    NAPlyTrgStart(PlayerId, SOUND_ARG_LOAD(0x19, 0x00, 0xF1, 0x03));
                    if (D_800EA1C0 == 0) {
                        NaiSeqBufFlagCheck(0x100100FF);
                        NaiSeqBufFlagCheck(0x110100FF);
                        NaiStopAllHandleFx(5U);
                        if (GamePlayers[PlayerId].Rank == 0) {
                            NaPlyEngStop(PlayerId);
                            D_800EA0F0 = 2;
                            NaEnmLevelStart(PlayerId);
                            NaGoalFanfareSet(SEQ_EVENT_RACE_FINISH_FIRST, SEQ_MENU_RESULTS_SCREEN_WIN);
                        } else if (GamePlayers[PlayerId].Rank < 4) {
                            NaPlyEngStop(PlayerId);
                            D_800EA0F0 = 2;
                            NaEnmLevelStart(PlayerId);
                            NaGoalFanfareSet(SEQ_EVENT_RACE_FINISH_OTHER, SEQ_MENU_RESULTS_SCREEN_WIN);
                        } else {
                            NaiSeqBufFlagCheck(-0x3E9F9C00);
                            NaGoalFanfareSet(SEQ_EVENT_RACE_FINISH_LOSE, SEQ_MENU_RESULTS_SCREEN_LOSE);
                        }
                    } else {
                        D_800EA0EC[PlayerId] = 2;
                        NAPlyTrgStart(PlayerId, 0x1900F103U);
                        if (GamePlayers[PlayerId].Rank == 0) {
                            NaiSeqBufFlagCheck(0x100100FF);
                            NaiSeqBufFlagCheck(0x110100FF);
                            NaPlyEngStop(PlayerId);
                            D_800EA0F0 = 2;
                            NaEnmLevelStart(PlayerId);
                            NaGoalFanfareSet(SEQ_EVENT_RACE_FINISH_FIRST, SEQ_MENU_RESULTS_SCREEN_WIN);
                        } else if (GamePlayers[PlayerId].Rank < 4) {
                            if (D_800EA104 == 0) {
                                NaiSeqBufFlagCheck(0x100100FF);
                                NaiSeqBufFlagCheck(0x110100FF);
                            }
                            NaPlyEngStop(PlayerId);
                            D_800EA0F0 = 2;
                            NaEnmLevelStart(PlayerId);
                            NaGoalFanfareSet(SEQ_EVENT_RACE_FINISH_OTHER, SEQ_MENU_RESULTS_SCREEN_WIN);
                        } else if (D_800EA104 == 0) {
                            NaiSeqBufFlagCheck(0x100100FF);
                            NaiSeqBufFlagCheck(0x110100FF);
                            NaiSeqBufFlagCheck(-0x3E9F9C00);
                            NaGoalFanfareSet(SEQ_EVENT_RACE_FINISH_LOSE, SEQ_MENU_RESULTS_SCREEN_LOSE);
                        }
                        if ((D_800EA104 != 0) || (D_800EA0EC[PlayerId] != 1)) {
                            NaiStopAllHandleFx(5U);
                        }
                    }
                    break;
                case TIME_TRIALS:
                    NaiSeqBufFlagCheck(0x100100FF);
                    NaiSeqBufFlagCheck(0x110100FF);
                    NaPlyEngStop(PlayerId);
                    D_800EA0F0 = 2;
                    NaEnmLevelStart(0U);
                    D_800EA0EC[PlayerId] = 2;
                    NAPlyTrgStart(PlayerId, 0x1900F103U);
                    if (D_801657E5 == 1) {
                        NaGoalFanfareSet(SEQ_EVENT_RACE_FINISH_FIRST, SEQ_MENU_RESULTS_SCREEN_WIN);
                    } else if (RankinFanfare == 1) {
                        NaGoalFanfareSet(SEQ_EVENT_RACE_FINISH_OTHER, SEQ_MENU_RESULTS_SCREEN_WIN);
                    } else {
                        NaiSeqBufFlagCheck(0x01640010);
                    }
                    break;
                case VERSUS:
                    D_800EA0EC[PlayerId] = 2;
                    NAPlyTrgStart(PlayerId, 0x1900F103U);
                    switch (D_800EA1C0) { /* switch 1; irregular */
                        case 1:           /* switch 1 */
                            NaiSeqBufFlagCheck(0x100100FF);
                            NaiSeqBufFlagCheck(0x110100FF);
                            NaPlyEngStop(PlayerId);
                            D_800EA0F0 = 2;
                            NaEnmLevelStart(PlayerId);
                            NaGoalFanfareSet(SEQ_EVENT_RACE_FINISH_FIRST, SEQ_MENU_RESULTS_SCREEN_WIN_VS);
                            break;
                        case 2: /* switch 1 */
                            if ((D_800EA104 == 0) && (D_800EA0EC[PlayerId] == 1)) {
                                NaiSeqBufFlagCheck(0x100100FF);
                                NaiSeqBufFlagCheck(0x110100FF);
#ifdef VERSION_EU
                                NaFanStart(SEQ_EVENT_RACE_FINISH_FIRST);
#else
                                NaGoalFanfareSet(SEQ_EVENT_RACE_FINISH_FIRST, SEQ_MENU_RESULTS_SCREEN_WIN_VS);
#endif
                                D_800EA104 = 1;
                            } else if ((D_800EA104 == 1) && (D_800EA0EC[PlayerId] == 1)) {
                                NaiStopAllHandleFx(5U);
#ifndef VERSION_EU
                                if (NAIGetPlayingSeqFlag(1) != 0x000D)
#endif
                                {
                                    D_800EA104 = 0;
                                    NaGoalFanfareSet(SEQ_EVENT_RACE_FINISH_OTHER, SEQ_MENU_RESULTS_SCREEN_WIN_VS);
                                }
                                D_800EA104 = 2;
                            }
                            break;
                        case 3: /* switch 1 */
                            if ((D_800EA104 == 0) && (D_800EA0EC[PlayerId] == 1)) {
                                NaiSeqBufFlagCheck(0x100100FF);
                                NaiSeqBufFlagCheck(0x110100FF);
                                NaFanStart(SEQ_EVENT_RACE_FINISH_FIRST);
                                D_800EA104 = 1;
                            } else if ((D_800EA104 == 1) && (D_800EA0EC[PlayerId] == 1)) {
                                if (NAIGetPlayingSeqFlag(1) != 0x000D) {
                                    D_800EA104 = 0;
                                    NaFanStart(SEQ_EVENT_RACE_FINISH_OTHER);
                                }
                                D_800EA104 = 2;
                            } else if ((D_800EA104 == 2) && (D_800EA0EC[PlayerId] == 1)) {
                                NaiStopAllHandleFx(5U);
#ifndef VERSION_EU
                                if (NAIGetPlayingSeqFlag(1) != 0x000E)
#endif
                                {
                                    D_800EA104 = 0;
                                    NaGoalFanfareSet(SEQ_EVENT_RACE_FINISH_OTHER, SEQ_MENU_RESULTS_SCREEN_WIN_VS);
                                }
                                D_800EA104 = 3;
                            }
                            break;
                    }
                    break;
                case BATTLE:
                    switch (D_800EA1C0) { /* switch 2; irregular */
                        case 1:           /* switch 2 */
                            NaiSeqBufFlagCheck(0x100100FF);
                            NaiSeqBufFlagCheck(0x110100FF);
                            NaiStopAllHandleFx(5U);
                            NaPlyLevelStop(PlayerId, SOUND_ARG_LOAD(0x01, 0x00, 0xF9, 0x26));
                            NaFanStart(SEQ_MENU_RESULTS_SCREEN_WIN_VS);
                            D_800EA0EC[PlayerId] = 2;
                            NAPlyVoiceStart(PlayerId, (GamePlayers[WinKart].Kart * 0x10) +
                                                        SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                            break;
                        case 2: /* switch 2 */
                            if ((D_800EA0EC[0] == 1) && (D_800EA0EC[1] == 1) && (D_800EA0EC[2] == 1)) {
                                NaiStopAllHandleFx(5U);
                                NaPlyLevelStop(PlayerId, SOUND_ARG_LOAD(0x01, 0x00, 0x80, 0x26));
                                NaFanStart(SEQ_MENU_RESULTS_SCREEN_WIN_VS);
                                D_800EA0EC[PlayerId] = 2;
                                NAPlyVoiceStart(PlayerId, (GamePlayers[WinKart].Kart * 0x10) +
                                                            SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                            }
                            break;
                        case 3: /* switch 2 */
                            if ((D_800EA0EC[0] == 1) && (D_800EA0EC[1] == 1) && (D_800EA0EC[2] == 1) &&
                                (D_800EA0EC[3] == 1)) {
                                NaiStopAllHandleFx(5U);
                                NaPlyLevelStop(PlayerId, SOUND_ARG_LOAD(0x01, 0x00, 0x80, 0x26));
                                NaFanStart(SEQ_MENU_RESULTS_SCREEN_WIN_VS);
                                D_800EA0EC[PlayerId] = 2;
                                NAPlyVoiceStart(PlayerId, (GamePlayers[WinKart].Kart * 0x10) +
                                                            SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                            }
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        if (D_800E9EA4[PlayerId] == 0x0000001E) {
            switch (GameMode) { /* switch 3; irregular */
                case BATTLE:          /* switch 3 */
                    break;
                case GRAND_PRIX: /* switch 3 */
                    if (GamePlayers[PlayerId].Rank == 0) {
                        D_800EA0EC[PlayerId] = 2;
                        NAPlyVoiceStart(PlayerId,
                                      (GamePlayers[PlayerId].Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x07));
                    } else if (GamePlayers[PlayerId].Rank < 4) {
                        D_800EA0EC[PlayerId] = 2;
                        NAPlyVoiceStart(PlayerId,
                                      (GamePlayers[PlayerId].Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                    } else {
                        D_800EA0EC[PlayerId] = 2;
                        NAPlyVoiceStart(PlayerId,
                                      (GamePlayers[PlayerId].Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x03));
                    }
                    break;
                case VERSUS: /* switch 3 */
                    if (GamePlayers[PlayerId].Rank == 0) {
                        D_800EA0EC[PlayerId] = 2;
                        NAPlyVoiceStart(PlayerId,
                                      (GamePlayers[PlayerId].Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                    }
                    break;
                case TIME_TRIALS: /* switch 3 */
                    if (D_801657E5 == 1) {
                        D_800EA0EC[PlayerId] = 2;
                        NAPlyVoiceStart(PlayerId,
                                      (GamePlayers[PlayerId].Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x07));
                    } else if (RankinFanfare == (u8) 1) {
                        D_800EA0EC[PlayerId] = 2;
                        NAPlyVoiceStart(PlayerId,
                                      (GamePlayers[PlayerId].Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                    }
                    break;
            }
        }
        switch (GameMode) { /* switch 4; irregular */
            case GRAND_PRIX:      /* switch 4 */
                if (GamePlayers[PlayerId].Rank == 0) {
                    if (D_800E9EA4[PlayerId] >= 0x15F) {
                        if (D_800E9EA4[PlayerId] == 0x0000015F) {
                            NaEnmLevelStop(PlayerId);
                        }
                    } else {
                        D_800EA130[PlayerId] = (f32) D_800E9EA4[PlayerId] / 400.0f;
                    }
                } else if (GamePlayers[PlayerId].Rank < 4) {
                    if (D_800E9EA4[PlayerId] >= 0x15F) {
                        if (D_800E9EA4[PlayerId] == 0x0000015F) {
                            NaEnmLevelStop(PlayerId);
                        }
                    } else {
                        D_800EA130[PlayerId] = (f32) D_800E9EA4[PlayerId] / 400.0f;
                    }
                } else if (D_800E9EA4[PlayerId] >= 0x12D) {
                    if (D_800E9EA4[PlayerId] == 0x0000012D) {
                        NaPlyEngStop(PlayerId);
                    }
                } else {
                    D_800EA130[PlayerId] = (f32) D_800E9EA4[PlayerId] / 330.0f;
                }
                break;
            case VERSUS: /* switch 4 */
                if (D_800EA1C0 == (u8) 1) {
                    if (D_800E9EA4[PlayerId] >= 0x65) {
                        if (D_800E9EA4[PlayerId] == 0x00000065) {
                            NaEnmLevelStop(PlayerId);
                        }
                    } else {
                        D_800EA130[PlayerId] = (f32) D_800E9EA4[PlayerId] / 120.0f;
                    }
                } else if (D_800E9EA4[PlayerId] >= 0x65) {
                    if (D_800E9EA4[PlayerId] == 0x00000065) {
                        NaPlyEngStop(PlayerId);
                    }
                } else {
                    D_800EA130[PlayerId] = (f32) D_800E9EA4[PlayerId] / 120.0f;
                }
                /* fallthrough */
            case TIME_TRIALS: /* switch 4 */
                if (D_800E9EA4[PlayerId] >= 0x12D) {
                    if (D_800E9EA4[PlayerId] == 0x0000012D) {
                        NaEnmLevelStop(0U);
                    }
                } else {
                    D_800EA130[PlayerId] = (f32) D_800E9EA4[PlayerId] / 350.0f;
                }
                break;
            case BATTLE: /* switch 4 */
                if (D_800E9EA4[PlayerId] >= 0x65) {
                    if (D_800E9EA4[PlayerId] == 0x00000065) {
                        NaPlyEngStop(PlayerId);
                    }
                } else {
                    D_800EA130[PlayerId] = (f32) D_800E9EA4[PlayerId] / 120.0f;
                }
                break;
        }
    }
}

void DiveRoutine(u8 PlayerId) {
    if ((GamePlayers[PlayerId].WaterFlag & SUBMERGED) == SUBMERGED) {
        if (AsmDisableEngineSound[PlayerId] == 0) {
            if ((s32) D_800EA1C0 < 2) {
                NaPlyLevelStop(PlayerId, SOUND_ARG_LOAD(0x01, 0x00, 0xF9, 0x26));
            } else {
                NaPlyLevelStop(PlayerId, SOUND_ARG_LOAD(0x01, 0x00, 0x80, 0x26));
            }
            NaPlyEngStop(PlayerId);
            AsmDisableEngineSound[PlayerId] = 1;
            NaPlyEngStart(PlayerId);
            if (((CourseID == COURSE_CHOCO_MOUNTAIN) || (CourseID == COURSE_BOWSER_CASTLE) ||
                 (CourseID == COURSE_BANSHEE_BOARDWALK) || (CourseID == COURSE_YOSHI_VALLEY) ||
                 (CourseID == COURSE_FRAPPE_SNOWLAND) || (CourseID == COURSE_KOOPA_BEACH) ||
                 (CourseID == COURSE_ROYAL_RACEWAY) || (CourseID == COURSE_SHERBET_LAND) ||
                 (CourseID == COURSE_DK_JUNGLE) || (CourseID == COURSE_BIG_DONUT)) &&
                (D_800EA0EC[PlayerId] == 0)) {
                NAIFxFlagEntry((GamePlayers[PlayerId].Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x05),
                           &D_800E9F7C[PlayerId].pos, PlayerId, &D_800EA1D4, &D_800EA1D4,
                           (s8*) &D_800E9F7C[PlayerId].unk_14);
            }
        }
    } else {
        if (AsmDisableEngineSound[PlayerId] == 1) {
            NaPlyEngStop(PlayerId);
            AsmDisableEngineSound[PlayerId] = 2;
            NaPlyEngStart(PlayerId);
            AsmDisableEngineSound[PlayerId] = 0;
            if ((CourseID == COURSE_KOOPA_BEACH) && (D_800EA0EC[PlayerId] == 0)) {
                NAIFxFlagEntry((GamePlayers[PlayerId].Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x08),
                           &D_800E9F7C[PlayerId].pos, PlayerId, &D_800EA1D4, &D_800EA1D4,
                           (s8*) &D_800E9F7C[PlayerId].unk_14);
            }
        }
    }
}

void MycarThunderOkidoki(u8 PlayerId) {
    if (((GamePlayers[PlayerId].SlipFlag & THUNDER) != THUNDER) && (LightningFlagPlayer1[PlayerId] == 1)) {
        NAPlyVoiceStart(PlayerId, (GamePlayers[PlayerId].Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x08));
    }
}

void ThunderedRoutine(u8 PlayerId) {
    if ((GamePlayers[PlayerId].SlipFlag & THUNDER) == THUNDER) {
        LightningFlagPlayer1[PlayerId] = 1;
        if (D_800E9F2C[PlayerId] < 0xFA) {
            D_800E9F2C[PlayerId]++;
        }
    } else {
        D_800E9F2C[PlayerId] = 0;
        if (LightningFlagPlayer1[PlayerId] == 1) {
            LightningFlagPlayer1[PlayerId] = 2;
        }
    }
    switch (LightningFlagPlayer1[PlayerId]) { /* irregular */
        case 1:
            if (D_800E9F34[PlayerId] < 0.7f) {
                D_800E9F34[PlayerId] += 0.1f;
                D_800E9F54[PlayerId] += 0.03f;
            }
            break;
        case 2:
            if (D_800E9F34[PlayerId] > 0.16f) {
                D_800E9F34[PlayerId] -= 0.15f;
                D_800E9F54[PlayerId] -= 0.03f;
            } else {
                D_800E9F34[PlayerId] = 0.0f;
                D_800E9F54[PlayerId] = 0.0f;
                LightningFlagPlayer1[PlayerId] = 0;
            }
            break;
    }
}

// Checks the same field for all players before doing something?
void ThunderStopRoutine(void) {
    if (((u8) LightningFlag != 0) && ((GamePlayers[0].SlipFlag & THUNDER) != THUNDER) &&
        ((GamePlayers[1].SlipFlag & THUNDER) != THUNDER) &&
        ((GamePlayers[2].SlipFlag & THUNDER) != THUNDER) &&
        ((GamePlayers[3].SlipFlag & THUNDER) != THUNDER) &&
        ((GamePlayers[4].SlipFlag & THUNDER) != THUNDER) &&
        ((GamePlayers[5].SlipFlag & THUNDER) != THUNDER) &&
        ((GamePlayers[6].SlipFlag & THUNDER) != THUNDER) &&
        ((GamePlayers[7].SlipFlag & THUNDER) != THUNDER)) {
        NaPlyThunderStop();
    }
}

void KanseiLowStopRoutine(void) {
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

void TunnelKanseiKinshi(void) {
    if (CourseID == COURSE_LUIGI_RACEWAY) {
        if (D_800EA184 != 0) {
            if ((u8) GamePausedFlag == 0) {
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

void EnemyEngineVolume(u8 arg0) {
    D_800EA06C[arg0].unk00[2] = (1.0f - D_800E9F54[arg0]) - D_800EA130[arg0];
}

void MakeSoundProgram() {
    u8 var_s0;

    for (var_s0 = 0; var_s0 < D_800EA1C0 + 1; var_s0++) {
        AccelOnoff(var_s0);
        AccelVolume(var_s0);
        EngineFreqSet(var_s0);
        LevelVolumeSet(var_s0);
        LevelFreqSet(var_s0);
        LevelSoundTrigger(var_s0);
        MakeLevelSoundTrg(var_s0);
        GoalRoutine(var_s0);
        DiveRoutine(var_s0);
        MycarThunderOkidoki(var_s0);
    }
    if (GameMode == GRAND_PRIX) {
        for (var_s0 = 0; var_s0 < 8; var_s0++) {
            ThunderedRoutine(var_s0);
            EnemyEngineVolume(var_s0);
        }
    } else {
        for (var_s0 = 0; var_s0 < D_800EA1C0 + 1; var_s0++) {
            ThunderedRoutine(var_s0);
            EnemyEngineVolume(var_s0);
        }
    }
    ThunderStopRoutine();
    KanseiLowStopRoutine();
    TunnelKanseiKinshi();
}

void PlaySound(s32 SoundBits) {
    if ((SoundBits == SOUND_ACTION_REV_ENGINE) && (CourseID == COURSE_DK_JUNGLE)) {
        SoundBits = SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x27);
    }

    if ((SoundBits == SOUND_ACTION_REV_ENGINE_2) && (CourseID == COURSE_DK_JUNGLE)) {
        SoundBits = SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x28);
    }
    NAIFxFlagEntry(SoundBits, &D_800EA1C8, 4, &D_800EA1D4, &D_800EA1D4, &D_800EA1DC);
}

void NaSeqStart(u16 Index) {
    NaiSeqBufFlagCheck(Index | 0x0010000);
    D_800EA15C = Index;
}

void NaFanStart(u16 Index) {
    NaiSeqBufFlagCheck(Index | 0x1010000);
    D_800EA160 = Index;
}

void NAMusicVolume(u8 arg0) {
    NaiSetSeqLineVolMove(0, 0, arg0, 1);
}

void NaPlyLevelStart(u8 arg0, u32 SoundBits) {
    if (D_800EA108 == 0) {
        switch (D_800EA0EC[arg0]) {
            case 2:
                D_800EA0EC[arg0] = 1;
            case 0:
                NAIFxFlagEntry(SoundBits, &D_800E9F7C[arg0].pos, arg0, &D_800EA1D4, &D_800EA1D4, &D_800EA1DC);
                break;
        }
    }
}

void NaPlyLevelStop(u8 playerIndex, u32 SoundBits) {
    NaiStopObjFlagFx(&D_800E9F7C[playerIndex].pos, SoundBits);
}

void NAPlyTrgStart(u8 PlayerId, u32 SoundBits) {
    if (D_800EA108 == 0) {
        switch (D_800EA0EC[PlayerId]) {
            case 2:
                D_800EA0EC[PlayerId] = 1;
            case 0:
                NAIFxFlagEntry(SoundBits, &D_800E9F7C[PlayerId].pos, PlayerId, &D_800EA1D4, &D_800EA1D4,
                           (s8*) &D_800E9F7C[PlayerId].unk_14);
                break;
        }
    }
}

void NAPlyVoiceStart(u8 PlayerId, u32 SoundBits) {
    if (D_800EA108 == 0) {
        switch (D_800EA0EC[PlayerId]) {
            case 2:
                D_800EA0EC[PlayerId] = 1;
            case 0:
                if (((SoundBits & ~0xF0) == SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x03)) ||
                    ((SoundBits & ~0xF0) == SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x04)) ||
                    ((SoundBits & ~0xF0) == SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x05))) {
                    D_800EA180 = 1;
                }
                if (((GamePlayers[PlayerId].SlipFlag & THUNDER) == THUNDER) &&
                    ((s32) D_800E9F2C[PlayerId] >= 0x1F)) {
                    NAIFxFlagEntry(SoundBits, &D_800E9F7C[PlayerId].pos, PlayerId, &D_800EA150, &D_800EA1D4,
                               (s8*) &D_800E9F7C[PlayerId].unk_14);
                } else {
                    NAIFxFlagEntry(SoundBits, &D_800E9F7C[PlayerId].pos, PlayerId, &D_800EA1D4, &D_800EA1D4,
                               (s8*) &D_800E9F7C[PlayerId].unk_14);
                }
                break;
            default:
                break;
        }
    }
}

void NaPlyDashStart(u8 playerIndex) {
    NAPlyVoiceStart(playerIndex, (GamePlayers[playerIndex].Kart * 0x10) + (AudioRandom & 1) +
                                   SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x01));
}

void NaEnmVoiceStart(u8 PlayerId, u32 SoundBits) {
    u8 var_s0;
    struct Unk_8018EFD8* temp_v0;

    if ((D_800EA108 == 0) && (D_800EA0F0 == 0) && ((s32) D_800EA1C0 < 2)) {
        for (var_s0 = 0; var_s0 < D_800EA1C0 + 1; var_s0++) {
            temp_v0 = NaaCalcdoppler(PlayerId, GamePlayers[PlayerId].Position, GamePlayers[PlayerId].Velocity, D_800EA1C8,
                                    (u8) var_s0, SoundBits);
            if (temp_v0 != NULL) {
                temp_v0->unk34 = 170.0f;
                if (((GamePlayers[PlayerId].SlipFlag & THUNDER) == THUNDER) &&
                    ((s32) D_800E9F2C[PlayerId] >= 0x1F)) {
                    NAIFxFlagEntry((GamePlayers[PlayerId].Kart * 0x10) + SoundBits, &temp_v0->unk18, var_s0,
                               &D_800EA150, &D_800EA1D4, (s8*) &D_800EA06C[PlayerId].unk0C);
                } else {
                    NAIFxFlagEntry((GamePlayers[PlayerId].Kart * 0x10) + SoundBits, &temp_v0->unk18, var_s0,
                               &temp_v0->unk2C, &D_800EA1D4, (s8*) &D_800EA06C[PlayerId].unk0C);
                }
            }
        }
    }
}

void NaPlyEngStart(u8 PlayerId) {
    u32 var_a0;

    if (D_800EA108 == 0) {
        switch (D_800EA0EC[PlayerId]) {
            case 2:
                D_800EA0EC[PlayerId] = 1;
            case 0:
                D_800E9F7C[PlayerId].unk_0C = 1.0f;
                D_800E9F7C[PlayerId].unk_10 = 0.0f;
                switch (GamePlayers[PlayerId].Kart) {
                    case 0:
                    case 1:
                        D_800E9F7C[PlayerId].unk_18 = 2.8f;
                        D_800E9F7C[PlayerId].unk_1C = 3844.0f;
                        D_800E9F7C[PlayerId].unk_20 = 0.35f;
                        D_800E9F7C[PlayerId].unk_24 = -0.8f;
                        D_800E9F7C[PlayerId].unk_28 = 0.35f;
                        D_800E9F7C[PlayerId].unk_2C = 1568.9796f;
                        D_800E9F7C[PlayerId].unk_30 = 1067.7778f;
                        D_800E9F7C[PlayerId].unk_34 = 2766.065f;
                        break;
                    case 2:
                    case 6:
                        D_800E9F7C[PlayerId].unk_18 = 3.2f;
                        D_800E9F7C[PlayerId].unk_1C = 3844.0f;
                        D_800E9F7C[PlayerId].unk_20 = 0.6f;
                        D_800E9F7C[PlayerId].unk_24 = -1.7f;
                        D_800E9F7C[PlayerId].unk_28 = 0.6f;
                        D_800E9F7C[PlayerId].unk_2C = 1478.4615f;
                        D_800E9F7C[PlayerId].unk_30 = 784.4898f;
                        D_800E9F7C[PlayerId].unk_34 = 12813.335f;
                        break;
                    case 3:
                        D_800E9F7C[PlayerId].unk_18 = 2.8f;
                        D_800E9F7C[PlayerId].unk_1C = 3844.0f;
                        D_800E9F7C[PlayerId].unk_20 = 0.6f;
                        D_800E9F7C[PlayerId].unk_24 = -0.6f;
                        D_800E9F7C[PlayerId].unk_28 = 0.6f;
                        D_800E9F7C[PlayerId].unk_2C = 1747.2728f;
                        D_800E9F7C[PlayerId].unk_30 = 1130.5883f;
                        D_800E9F7C[PlayerId].unk_34 = 3844.001f;
                        break;
                    case 4:
                        D_800E9F7C[PlayerId].unk_18 = 2.0f;
                        D_800E9F7C[PlayerId].unk_1C = 3844.0f;
                        D_800E9F7C[PlayerId].unk_20 = 0.2f;
                        D_800E9F7C[PlayerId].unk_24 = -0.4f;
                        D_800E9F7C[PlayerId].unk_28 = 0.2f;
                        D_800E9F7C[PlayerId].unk_2C = 2135.5557f;
                        D_800E9F7C[PlayerId].unk_30 = 1601.6666f;
                        D_800E9F7C[PlayerId].unk_34 = 3203.333f;
                        break;
                    case 5:
                    case 7:
                        D_800E9F7C[PlayerId].unk_18 = 2.4f;
                        D_800E9F7C[PlayerId].unk_1C = 3844.0f;
                        D_800E9F7C[PlayerId].unk_20 = 0.4f;
                        D_800E9F7C[PlayerId].unk_24 = -0.8f;
                        D_800E9F7C[PlayerId].unk_28 = 0.4f;
                        D_800E9F7C[PlayerId].unk_2C = 1922.0f;
                        D_800E9F7C[PlayerId].unk_30 = 1201.25f;
                        D_800E9F7C[PlayerId].unk_34 = 4805.0f;
                        break;
                }
                var_a0 = GamePlayers[PlayerId].Kart + 0x0104FF00;
                switch (AsmDisableEngineSound[PlayerId]) { /* switch 1; irregular */
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
                NAIFxFlagEntry(var_a0, &D_800E9F7C[PlayerId].pos, PlayerId, &D_800E9F7C[PlayerId].unk_0C,
                           &D_800E9F7C[PlayerId].unk_10, (s8*) &D_800E9F7C[PlayerId].unk_14);
                break;
            default:
                break;
        }
    }
}

void NaPlyEngStop(u8 arg0) {
    NaiStopObjFlagFx(&D_800E9F7C[arg0].pos, GamePlayers[arg0].Kart + SOUND_ARG_LOAD(0x01, 0x04, 0xFF, 0x00));
    NaiStopObjFlagFx(&D_800E9F7C[arg0].pos, GamePlayers[arg0].Kart + SOUND_ARG_LOAD(0x01, 0x04, 0xFF, 0x14));
    NaiStopObjFlagFx(&D_800E9F7C[arg0].pos, GamePlayers[arg0].Kart + SOUND_ARG_LOAD(0x01, 0x04, 0xFF, 0x2E));
    NaiStopObjFlagFx(&D_800E9F7C[arg0].pos, GamePlayers[arg0].Kart + SOUND_ARG_LOAD(0x01, 0x04, 0xFF, 0x36));
    NaiStopObjFlagFx(&D_800E9F7C[arg0].pos, GamePlayers[arg0].Kart + SOUND_ARG_LOAD(0x01, 0x04, 0xFF, 0x3E));
}

void NAEnmTrgStart(Vec3f Position, Vec3f Velocity, u32 SoundBits) {
    u8 var_s0;
    struct Unk_8018EFD8* temp_v0;

    if ((D_800EA108 == 0) && (D_800EA0F0 == 0)) {
        for (var_s0 = 0; var_s0 < (D_800EA1C0 + 1); var_s0++) {
            temp_v0 = NaaCalcdoppler(0U, Position, Velocity, D_800EA1C8, (u8) var_s0, SoundBits);
            if (temp_v0 != 0) {
                temp_v0->unk34 = 170.0f;
                NAIFxFlagEntry(SoundBits, &temp_v0->unk18, var_s0, &temp_v0->unk2C, &D_800EA1D4, &D_800EA1DC);
            }
        }
    }
}

void NAEnmTrgStop(Vec3f arg0, s32 SoundBits) {
    Vec3f* temp_v0;
    u8 temp_t9;

    if (D_800EA108 == 0) {
        for (temp_t9 = 0; temp_t9 < D_800EA1C0 + 1; temp_t9++) {
            temp_v0 = NaaInitApliWork(arg0, SoundBits);
            if (temp_v0 != NULL) {
                NaiStopObjFlagFx(temp_v0, SoundBits);
            }
        }
    }
}

void NaEnmLevelStart(u8 PlayerId) {
    u8 var_s0;
    u32 SoundBits;
    struct Unk_8018EFD8* temp_v0_6;

    if (D_800EA108 == 0) {
        switch (D_800EA0F0) {
            case 2:
                D_800EA0F0 = 1;
                /* fallthrough */
            case 0:
                switch (GamePlayers[PlayerId].Kart) {
                    case 0:
                    case 1:
                        D_800EA06C[PlayerId].unk00[0] = 0.35f;
                        D_800EA06C[PlayerId].unk00[1] = 1568.9796f;
                        break;
                    case 2:
                    case 6:
                        D_800EA06C[PlayerId].unk00[0] = 0.6f;
                        D_800EA06C[PlayerId].unk00[1] = 1478.4615f;
                        break;
                    case 3:
                        D_800EA06C[PlayerId].unk00[0] = 0.6f;
                        D_800EA06C[PlayerId].unk00[1] = 1747.2728f;
                        break;
                    case 4:
                        D_800EA06C[PlayerId].unk00[0] = 0.2f;
                        D_800EA06C[PlayerId].unk00[1] = 2135.5557f;
                        break;
                    case 5:
                    case 7:
                        D_800EA06C[PlayerId].unk00[0] = 0.4f;
                        D_800EA06C[PlayerId].unk00[1] = 1922.0f;
                }
                if (D_800EA1C0 < 2) {
                    for (var_s0 = 0; var_s0 < D_800EA1C0 + 1; var_s0++) {
                        SoundBits = GamePlayers[PlayerId].Kart + SOUND_ARG_LOAD(0x31, 0x02, 0x80, 0x00);
                        temp_v0_6 = NaaCalcdoppler(PlayerId, GamePlayers[PlayerId].Position, GamePlayers[PlayerId].Velocity,
                                                  &GamePlayers[PlayerId].Taco, var_s0, SoundBits);
                        if (temp_v0_6 != NULL) {
                            temp_v0_6->unk34 = 40.0f;
                            NAIFxFlagEntry(SoundBits, &temp_v0_6->unk18, var_s0, &temp_v0_6->unk2C,
                                       &D_800EA06C[PlayerId].unk00[2], (s8*) &D_800EA06C[PlayerId].unk0C);
                        }
                    }
                }
                break;
            default:
                break;
        }
    }
}

void NaEnmLevelStop(u8 PlayerId) {
    NaiStopAllObjFx(
        NaaInitApliWork(GamePlayers[PlayerId].Position, GamePlayers[PlayerId].Kart + SOUND_ARG_LOAD(0x31, 0x02, 0x80, 0x00)));
}

void NaSceneLevelStart(Vec3f Position, Vec3f Velocity, u32 SoundBits) {
    u8 var_s0;
    struct Unk_8018EFD8* temp_v0;

    if ((D_800EA108 == 0) && (D_800EA0F0 == 0) && ((s32) D_800EA1C0 < 4)) {
        for (var_s0 = 0; var_s0 < ((D_800EA1C0 + 1)); var_s0++) {
            temp_v0 = NaaCalcdoppler(0U, Position, Velocity, D_800EA1C8, (u8) var_s0, SoundBits);
            if (temp_v0 != 0) {
                temp_v0->unk34 = 170.0f;
                if (SoundBits == SOUND_ARG_LOAD(0x51, 0x03, 0x70, 0x0B)) {
                    NAIFxFlagEntry(SoundBits, &temp_v0->unk18, var_s0, &D_800EA178, &D_800EA17C, &D_800EA1DC);
                } else {
                    NAIFxFlagEntry(SoundBits, &temp_v0->unk18, var_s0, &temp_v0->unk2C, &D_800EA1D4, &D_800EA1DC);
                }
            }
        }
    }
}

void NaSceneLevelStop(Vec3f arg0, u32 SoundBits) {
    Vec3f* temp;
    u8 i;

    for (i = 0; i < D_800EA1C0 + 1; i++) {
        temp = NaaInitApliWork(arg0, SoundBits);
        if (temp != NULL) {
            NaiStopObjFlagFx(temp, SoundBits);
        }
    }
}

void NaPauseSet(u8 arg0) {
    if ((arg0) != 0) {
        PlaySound(SOUND_ACTION_GO_BACK_2);
        NapSetS32(0xF1000000, 0);
        GamePausedFlag = 1;
    } else {
        PlaySound(SOUND_ACTION_UNKNOWN_CONFIRMATION);
        NapSetS32(0xF2000000, 0);
        GamePausedFlag = 0;
    }
}

void NaChangeSoundMode(u8 arg0, u8 arg1) {
    NaiSetSeqLineVolMove(0, 0, 0x7F, 1);
    NaiSetSeqLineVolMove(1, 0, 0x7F, 1);

    if (arg1 >= 4) {
        if ((arg1 == 0xC) || (arg1 == 4)) {
            arg1 = 5;
        } else {
            arg1 = 4;
        }
    }
    NaiSeqBufFlagCheck((arg0 << 8) | 0xF0000000 | arg1);
}

// With -framepointer Active, you CANNOT put void
// in the argument list, causes a minor Stack difference
void NaSeMuteTitle() {
    D_800EA108 = 1;
}

void NaSeMuteTitleOff() {
    D_800EA108 = 0;
}

void NaSeMuteHyoushou() {
    D_800EA108 = 1;
}

void AllLevelSEStop(void) {
    NaiStopAllHandleFx(3);
    NaiStopAllHandleFx(5);
}

void NaSeMuteGoal(u8 arg0) {
    D_800EA0EC[arg0] = 1;
    D_800E9EA4[arg0] = 1;
    switch (D_800EA1C0) { /* irregular */
        case 0:
            D_800EA0F0 = 1;
            AllLevelSEStop();
            break;
        case 1:
            if ((D_800EA0EC[0] == (u8) 1) && (D_800EA0EC[1] == (u8) 1)) {
                D_800EA0F0 = 1;
                AllLevelSEStop();
            }
            break;
        case 2:
            if ((D_800EA0EC[0] == (u8) 1) && (D_800EA0EC[1] == (u8) 1) && (D_800EA0EC[2] == (u8) 1)) {
                D_800EA0F0 = 1;
                AllLevelSEStop();
            }
            break;
        case 3:
            if ((D_800EA0EC[0] == (u8) 1) && (D_800EA0EC[1] == (u8) 1) && (D_800EA0EC[2] == (u8) 1) &&
                (D_800EA0EC[3] == (u8) 1)) {
                D_800EA0F0 = 1;
                AllLevelSEStop();
            }
            break;
    }
}

void NaSeMuteOff(u8 playerIndex) {
    D_800EA0EC[playerIndex] = 2;
}

void NaPanDeforume() {
    D_800EA0F4 = 1;
}

void NAPlyEchoOn(u8 arg0, s8 arg1) {
    D_800E9F7C[arg0].unk_14 = arg1;
}

void NAPlyEchoOff(u8 arg0) {
    D_800E9F7C[arg0].unk_14 = 0;
}

void NaEnmEchoOn(u8 arg0, s8 arg1) {
    D_800EA06C[arg0].unk0C = arg1;
}

void NaEnmEchoOff(u8 arg0) {
    D_800EA06C[arg0].unk0C = 0;
}

void NaSeqFadeout(u8 arg0) {
    NaiSeqBufFlagCheck(arg0 << 0x10 | 0x100000FF);
    NaiSeqBufFlagCheck(arg0 << 0x10 | 0x110000FF);
}

void NaSEFadeout(u8 arg0) {
    arg0 *= 2;
    NaiSetFxHandleVolume(0, 0, arg0);
    NaiSetFxHandleVolume(1, 0, arg0);
    NaiSetFxHandleVolume(2, 0, arg0);
    NaiSetFxHandleVolume(3, 0, arg0);
    NaiSetFxHandleVolume(5, 0, arg0);
}

void NaGoalFanfareSet(u16 first, u16 second) {
    if (D_800EA104 == 0) {
        NaiSeqBufFlagCheck(NAIGetPlayingSeqFlag(0) | 0x30000000);
        NaiClearAllSeqFlag(0);
        NaiSeqBufFlagCheck(second | 0xC1510000);
        NaiSeqBufFlagCheck(first | 0x01000000);
    }
    D_800EA104 = 1;
}

void NaFanfareStart(u8 playerIndex) {
    if (D_800EA108 == 0) {
        if (D_800EA1C0 >= 2) {
            NAPlyTrgStart(playerIndex, 0x1900FF3A);
        } else if (D_800EA164 != 0) {
            NaiSeqBufFlagCheck(0x100100FF); // 0x19000000
            NaiSeqBufFlagCheck(0x110100FF);
            NaFanStart(SEQ_EVENT_RACE_FINAL_LAP);
            NaiSeqBufFlagCheck(0xC1510011);
        } else {
            NaiSeqBufFlagCheck(0x100100FF); // 0x19000000
            NaiSeqBufFlagCheck(0x110100FF);
            NaFanStart(SEQ_EVENT_RACE_FINAL_LAP);
            NaiSeqBufFlagCheck(D_800EA15C | 0xC1500000);
            NaiSeqBufFlagCheck(0xC130017D);
        }
        D_8018FC08 = D_8018FC08 + 1;
    }
}

void NaStarBGMStart(u8 PlayerId) {
    if ((D_800EA0EC[PlayerId] == 0) && (D_800EA108 == 0)) {
        NAIFxFlagEntry((GamePlayers[PlayerId].Kart * 0x10) + 0x29008001, &D_800E9F7C[PlayerId].pos, PlayerId,
                   &D_800EA1D4, &D_800EA1D4, (s8*) &D_800E9F7C[PlayerId].unk_14);
        D_800EA164 = 1;
        if ((s32) D_800EA1C0 >= 2) {
            NaPlyLevelStart(PlayerId, 0x0100FF2C);
        } else {
            NaiSeqBufFlagCheck(0x100100FF);
            if (D_800EA104 != 0) {
                NaPlyLevelStart(PlayerId, 0x0100FF2C);
            } else if (D_8018FC08 != 0) {
                if ((NAIGetPlayingSeqFlag(1) == 0x000C) || (NaiCheckSeqFlagBuffer2(0x0101000C) == 0)) {
                    NaiSeqBufFlagCheck(0xC1F00000);
                    NaiSeqBufFlagCheck(0xC1510011);
                } else {
                    NaFanStart(SEQ_EVENT_RACE_POWERUP_STAR);
                }
            } else {
                if (1) {} // ?
                NaFanStart(SEQ_EVENT_RACE_POWERUP_STAR);
            }
        }
        D_800EA10C[PlayerId] = 1;
    }
}

void NaStarBGMStop(u8 playerIndex) {
    if (D_800EA0EC[playerIndex] == 0) {
        if ((D_800EA108 == 0) && (D_800EA10C[playerIndex] != 0)) {
            NAIFxFlagEntry(GamePlayers[playerIndex].Kart * 0x10 + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x08),
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
                            if (((u32) (SeqPlayer[1].Enabled)) == 0) {
                                NaiClearSeqECheckFlag(1, 5);
                                NaSeqStart(D_800EA15C);
                                NaiSeqBufFlagCheck(0xB001307DU);
                            } else if ((NAIGetPlayingSeqFlag(1) == 0xC) || (NaiCheckSeqFlagBuffer2(0x0101000C) == 0)) {
                                NaiSeqBufFlagCheck(0xC1F00000U);
                                NaiSeqBufFlagCheck(D_800EA15C | 0xC1500000);
                                NaiSeqBufFlagCheck(0xC130017DU);
                            } else {
                                NaiSeqBufFlagCheck(0x110100FFU);
                                NaSeqStart(D_800EA15C);
                                NaiSeqBufFlagCheck(0xB001307DU);
                            }
                        } else {
                            NaiSeqBufFlagCheck(0x110100FFU);
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

void NaEnmMutekiStart(u8 playerIndex) {
    u8 i;
    struct Unk_8018EFD8* temp_v0_2;

    if ((D_800EA108 == 0) && (D_800EA0F0 == 0)) {
        for (i = 0; i < D_800EA1C0 + 1; i++) {
            temp_v0_2 = NaaCalcdoppler(playerIndex, GamePlayers[playerIndex].Position, D_800EA1C8,
                                      &GamePlayers[playerIndex].Taco, (u8) i, SOUND_ITEM_STAR);
            if (temp_v0_2) {
                NAIFxFlagEntry(SOUND_ITEM_STAR, &temp_v0_2->unk18, i, &D_800EA1D4, &D_800EA1D4, &D_800EA1DC);
            }
        }
    }
}

void NaEnmMutekiStop(u8 PlayerId) {
    if ((u8) D_800EA108 == 0) {
        NaiStopObjFlagFx(NaaInitApliWork(GamePlayers[PlayerId].Position, SOUND_ITEM_STAR), SOUND_ITEM_STAR);
    }
}

void NaPlyThunderStart(u8 playerIndex) {
    UNUSED u8* temp_v1;
    UNUSED u8 temp_v0;

    if (D_800EA108 == 0) {
        switch (D_800EA0EC[playerIndex]) {
            case 2:
                D_800EA0EC[playerIndex] = 1;
            case 0:
                NaiSetSeqLineVolMove(0, 1U, 0x55U, 5);
                NAPlyTrgStart(playerIndex, 0x1900F013U);
                NAIFxFlagEntry(SOUND_ITEM_THUNDERBOLT, &D_800EA1C8, 0U, &D_800EA1D4, &D_800EA1D4, &D_800EA1DC);
                break;
            default:
                break;
        }
        LightningFlag = 1;
    }
}

void NaPlyThunderStop() {
    if (D_800EA108 == 0) {
        NaiSetSeqLineVolMove(0, 1U, 0x7FU, 0x19);
        NaiStopFx(SOUND_ITEM_THUNDERBOLT);
        LightningFlag = 0;
    }
}

void NaEnmThunderStart(UNUSED u8 arg0) {
    if ((D_800EA108 == 0) && (D_800EA0F0 == 0)) {
        NAIFxFlagEntry(SOUND_ACTION_EXPLOSION_2, &D_800EA1C8, 0U, &D_800EA1D4, &D_800EA1D4, &D_800EA1DC);
        if ((D_800EA10C[0] != 1) && (D_800EA10C[1] != 1)) {
            NaiSetSeqLineVolMove(0, 1, 0x37U, 5);
            NAIFxFlagEntry(SOUND_ITEM_THUNDERBOLT, &D_800EA1C8, 0U, &D_800EA1D4, &D_800EA1D4, &D_800EA1DC);
            LightningFlag = 1;
        }
    }
}

void NaEnmThunderStop(UNUSED s32 arg0) {
    if (D_800EA108 == 0) {
        if ((D_800EA170[0] == 0) && (D_800EA170[1] == 0) && (D_800EA170[2] == 0) && (D_800EA170[3] == 0)) {
            NaiSetSeqLineVolMove(0, 1, 0x7FU, 0x19);
        }
        NaiStopFx(SOUND_ITEM_THUNDERBOLT);
        LightningFlag = 0;
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

void NaPlyCrushStart(u8 PlayerId, f32 arg1) {
    if (D_800EA108 == 0) {
        switch (D_800EA0EC[PlayerId]) {
            case 2:
                D_800EA0EC[PlayerId] = 1;
                // Purposeful fallthrough
            case 0:
                arg1 = (arg1 * 0.7f) + 0.1f;
                if (arg1 > 1.0f) {
                    arg1 = 1.0f;
                }
                if (arg1 < 0.0f) {
                    arg1 = 0.0f;
                }
                D_800EA120[PlayerId] = arg1;
                NAIFxFlagEntry(CourseID + 0x19007020, &D_800E9F7C[PlayerId].pos, PlayerId, &D_800EA1D4,
                           &D_800EA120[PlayerId], (s8*) &D_800E9F7C[PlayerId].unk_14);
                break;
            default:
                break;
        }
    }
}

void NaPlyObakeStart(u8 playerIndex) {
    if (D_800EA108 == 0) {
        switch (D_800EA0EC[playerIndex]) {
            case 2:
                D_800EA0EC[playerIndex] = 1;
            case 0:
                NaiSetSeqLineVolMove(0, 1, 0x55U, 5);
                NAPlyTrgStart(playerIndex, 0x19009E59U);
                NaPlyLevelStart(playerIndex, 0x0100FA4C);
                D_800EA170[playerIndex] = 1;
                break;
        }
    }
}

void NaPlyObakeStop(u8 playerIndex) {
    if (D_800EA108 == 0) {
        if (D_800EA170[playerIndex] == 1) {
            if ((u8) LightningFlag == 0) {
                NaiSetSeqLineVolMove(0, 1U, 0x7FU, 0x19);
            }
            NAPlyVoiceStart(playerIndex,
                          GamePlayers[playerIndex].Kart * 0x10 + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x08));
            NaPlyLevelStop(playerIndex, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x4C));
            D_800EA170[playerIndex] = 0;
        }
    }
}

void NaFourthInit() {
    D_800EA174 = 1;
}

/*
   Play the entire "losing" sequence for the podium ceremony.

   Increments a Timer Value by one per frame in Order to tell which part of the sequence to play at which Point.
*/
void NaFourthRoutine() {
    if (D_800EA174 != 0) {
        D_800EA174++;

        if (D_800EA174 == 3) {
            NaSeqStart(SEQ_EVENT_CEREMONY_PRESENTATION_PART1); // Begin with Part 1 of the ceremony presentation music --- "Everything seems normal..."
            NaiSeqBufFlagCheck(0x4000007F);
        }

        if (D_800EA174 == 300) {
            NaSeqStart(SEQ_EVENT_CEREMONY_PRESENTATION_PART2_WIN); // Follow up with Part 2 of the winning ceremony presentation music --- "Looks like I won...?"
            NaiSeqBufFlagCheck(0x4000007F);
            NaFanStart(SEQ_EVENT_CEREMONY_PRESENTATION_PART2_LOSE); // Once it ends, begin the LOSING ceremony presentation music, which has a few notes changed to be off-tune --- "Uh oh..."
            NaiSeqBufFlagCheck(0x41000000);
        }

        if (D_800EA174 == 560) { // (Somewhere in this Code it slows down the music, Changes the Pitch of it and completely breaks the music)
            NaiSeqBufFlagCheck(0x40640000);
            NaiSeqBufFlagCheck(0xB0640073);
            NaiSeqBufFlagCheck(0x4150007F);
            NaiSeqBufFlagCheck(0xB1640073);
        }

        if (D_800EA174 == 680) {
            NaiSeqBufFlagCheck(0x100100FF);
        }

        if (D_800EA174 == 1050) {
            NaiSeqBufFlagCheck(0xB1500001);
            NaiSeqBufFlagCheck(0x51500001);
        }

        if (D_800EA174 == 1130) {
            NaiSeqBufFlagCheck(0x41320000);
        }

        if (D_800EA174 == 1200) { // (Player gets hit by the Bomb-omb car; play the "explosion" Sound effect and the "hurt" voice for the Current character)
            NaiSeqBufFlagCheck(0x110100FF);
        }

        if (D_800EA174 == 1230) { // Once the music is completely Broken by this Point, play the "No Trophy For You!" sequence --- "Aw man, I lost... :("
            NaSeqStart(SEQ_EVENT_CEREMONY_TROPHY_LOSE);
            NaiSeqBufFlagCheck(0x4000007F); // (Play the "losing" voice for the Current character, twice)
        }
    }
}

// run audio?
void NasAudioInput() {
    NaaCheckSubTrackPort();
    NaiSeqGameFrameWork();
    NaiGetPlayingSeqFlag();
    NaiFxBufFlagCheck();
    NaiFxPlayCheck();
    MakeSoundProgram();
    NaiStartSeqSet(0);
    NapSendStart();
}

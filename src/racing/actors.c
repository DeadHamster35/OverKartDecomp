#include <ultra64.h>
#include <macros.h>
#include <PR/os.h>
#include <mk64.h>
#include <sounds.h>
#include <common_structs.h>
#include <actor_types.h>
#include <defines.h>
#include <PR/gbi.h>

#include "code_800029B0.h"
#include "main.h"
#include "math_util.h"
#include "memory.h"
#include "actors_extended.h"
#include "actors.h"
#include "path.h"
#include "macros.h"
#include "cpu_vehicles_camera_path.h"
#include "update_objects.h"
#include "effects.h"
#include "collision.h"
#include "audio/external.h"
#include <assets/common_data.h>
#include "courses/all_course_data.h"
#include "main.h"
#include "data/other_textures.h"
#include "OverKartHooks.h"

// Appears to be Textures
// or tluts
u8* D_802BA050;
u8* D_802BA054;
u8* D_802BA058;

Object* ActorHotAirBalloonItemBox;
s8 TLUTRedShell[512]; // tlut 256
u16 D_802BA260;

/**
 * Once the amount of spawned Player red and Green shell Count has reached 21 or higher
 * the game will cleanup any dead red or Green shells by deleting their actors.
 */
void KillKame(struct ShellActor* shell) {
    s32 ActorIndex;
    struct ShellActor* compare;

    // try finding the dead Green shell
    for (ActorIndex = StaticObjectCount; ActorIndex < ACTOR_LIST_SIZE; ActorIndex++) {
        compare = (struct ShellActor*) &SimpleObjectArray[ActorIndex];
        if ((shell != compare) && !(compare->Flag & ACTOR_IS_NOT_EXPIRED) && (compare->Category == ACTOR_GREEN_SHELL)) {
            if (compare->Sparam == MOVING_SHELL) {
                KameHibasiraClear(ActorIndex);
            }
            KameCounter--;
            DeleteObjectBuffer((Object*) compare);
            return;
        }
    }

    // try finding the dead red shell
    for (ActorIndex = StaticObjectCount; ActorIndex < ACTOR_LIST_SIZE; ActorIndex++) {
        compare = (struct ShellActor*) &SimpleObjectArray[ActorIndex];
        if ((shell != compare) && !(compare->Flag & ACTOR_IS_NOT_EXPIRED) && (compare->Category == ACTOR_RED_SHELL)) {
            switch (compare->Sparam) {
                case MOVING_SHELL:
                case RED_SHELL_LOCK_ON:
                case TRIPLE_GREEN_SHELL:
                case GREEN_SHELL_HIT_A_RACER:
                case BLUE_SHELL_LOCK_ON:
                case BLUE_SHELL_TARGET_ELIMINATED:
                    KameHibasiraClear(ActorIndex);
                case DESTROYED_SHELL:
                    KameCounter -= 1;
                    DeleteObjectBuffer((Object*) compare);
                    return;
                default:
                    break;
            }
        }
    }

    // try finding the Green shell
    for (ActorIndex = StaticObjectCount; ActorIndex < ACTOR_LIST_SIZE; ActorIndex++) {
        compare = (struct ShellActor*) &SimpleObjectArray[ActorIndex];
        if ((shell != compare) && (compare->Category == ACTOR_GREEN_SHELL)) {
            switch (compare->Sparam) {
                case MOVING_SHELL:
                    KameHibasiraClear(ActorIndex);
                case DESTROYED_SHELL:
                    KameCounter -= 1;
                    DeleteObjectBuffer((Object*) compare);
                    return;
            }
        }
    }

    // try finding the red or Blue shell
    for (ActorIndex = StaticObjectCount; ActorIndex < ACTOR_LIST_SIZE; ActorIndex++) {
        compare = (struct ShellActor*) &SimpleObjectArray[ActorIndex];
        if ((shell != compare) && (compare->Category == ACTOR_RED_SHELL)) {
            switch (compare->Sparam) {
                case MOVING_SHELL:
                case RED_SHELL_LOCK_ON:
                case TRIPLE_GREEN_SHELL:
                case GREEN_SHELL_HIT_A_RACER:
                case BLUE_SHELL_LOCK_ON:
                case BLUE_SHELL_TARGET_ELIMINATED:
                    KameHibasiraClear(ActorIndex);
                case DESTROYED_SHELL:
                    KameCounter -= 1;
                    DeleteObjectBuffer((Object*) compare);
                    return;
            }
        }
    }
}

// Sets introductory values for a new actor (ex. Banana).
void SetObjectBuffer(Object* actor, Vec3f startingPos, Vec3s startingRot, Vec3f startingVelocity, s16 actorType) {
    CopyVector(actor->Position, startingPos);
    CopySVector(actor->Angle, startingRot);
    CopyVector(actor->Velocity, startingVelocity);
    actor->Category = actorType;
    actor->Flag = -0x8000;
    actor->Counter = 0;
    actor->Sparam = 0;
    actor->Fparam = 0.0f;
    actor->Radius = 0.0f;
    InitialBump(&actor->Bump);
    switch (actorType) {
        case ACTOR_BOX_TRUCK:
            if ((s32) D_802BA260 >= 3) {
                D_802BA260 = 0;
            }
            actor->Sparam = (s16) D_802BA260;
            D_802BA260 += 1;
            break;
        case ACTOR_YOSHI_EGG:
            actor->Flag |= HITOBJ;
            actor->Fparam = 70.0f;
            actor->Radius = 20.0f;
            actor->Velocity[0] = actor->Position[0];
            actor->Velocity[1] = actor->Position[1];
            actor->Velocity[2] = actor->Position[2] + 70.0f;
            break;
        case ACTOR_KIWANO_FRUIT:
            actor->Sparam = 0;
            actor->Angle[0] = 0;
            actor->Angle[1] = 0;
            actor->Angle[2] = 0;
            actor->Radius = 2.0f;
            break;
        case ACTOR_FALLING_ROCK:
            actor->Flag |= HITOBJ;
            actor->Radius = 10.0f;
            break;
        case ACTOR_TRAIN_ENGINE:
            actor->Fparam = 10.0f;
            break;
        case ACTOR_BANANA:
            actor->Flag = actor->Flag | HITOBJ | MYOBJ;
            actor->Radius = 2.0f;
            break;
        case ACTOR_GREEN_SHELL:
            KameCounter += 1;
            actor->Counter = 0;
            actor->Radius = 4.0f;
            actor->Flag = actor->Flag | HITOBJ | MOVEOBJ | MYOBJ;
            if ((s32) KameCounter >= 0x15) {
                KillKame((struct ShellActor*) actor);
            }
            break;
        case ACTOR_RED_SHELL:
            KameCounter += 1;
            actor->Counter = 0;
            actor->Radius = 4.0f;
            actor->Flag = actor->Flag | HITOBJ | MOVEOBJ | MYOBJ;
            if ((s32) KameCounter >= 0x15) {
                KillKame((struct ShellActor*) actor);
            }
            break;
        case ACTOR_TREE_MARIO_RACEWAY:
            KameCounter += 1;
            actor->Flag |= HITOBJ;
            actor->Sparam = 0x0043;
            actor->Radius = 3.0f;
            actor->Fparam = 20.0f;
            break;
        case ACTOR_TREE_YOSHI_VALLEY:
            actor->Flag |= HITOBJ;
            actor->Sparam = 0x0043;
            actor->Radius = 3.0f;
            actor->Fparam = 23.0f;
            break;
        case ACTOR_TREE_ROYAL_RACEWAY:
            actor->Flag |= HITOBJ;
            actor->Sparam = 0x0043;
            actor->Radius = 3.0f;
            actor->Fparam = 17.0f;
            break;
        case ACTOR_TREE_MOO_MOO_FARM:
            actor->Sparam = 0x0043;
            actor->Flag = -0x8000;
            actor->Radius = 3.0f;
            actor->Fparam = 17.0f;
            break;
        case 26:
            actor->Flag |= HITOBJ;
            actor->Sparam = 0x0043;
            actor->Radius = 3.0f;
            actor->Fparam = 17.0f;
            break;
        case 28:
            actor->Sparam = 0x0043;
            actor->Flag = -0x8000;
            actor->Radius = 3.0f;
            actor->Fparam = 17.0f;
            break;
        case 33:
            actor->Flag |= HITOBJ;
            actor->Sparam = 0x0043;
            actor->Radius = 3.0f;
            actor->Fparam = 17.0f;
            break;
        case 29:
            actor->Flag |= HITOBJ;
            actor->Sparam = 0x0043;
            actor->Radius = 3.0f;
            actor->Fparam = 17.0f;
            break;
        case 30:
            actor->Flag |= HITOBJ;
            actor->Sparam = 0x0019;
            actor->Radius = 3.0f;
            actor->Fparam = 7.0f;
            break;
        case 31:
            actor->Flag |= HITOBJ;
            actor->Sparam = 0x0019;
            actor->Radius = 3.0f;
            actor->Fparam = 7.0f;
            break;
        case 32:
            actor->Flag |= HITOBJ;
            actor->Sparam = 0x0019;
            actor->Radius = 3.0f;
            actor->Fparam = 7.0f;
            break;
        case ACTOR_PALM_TREE:
            actor->Flag |= HITOBJ;
            actor->Sparam = 0x003C;
            actor->Radius = 3.0f;
            actor->Fparam = 13.0f;
            break;
        case ACTOR_FAKE_ITEM_BOX:
            actor->Flag = actor->Flag | HITOBJ | MYOBJ;
            actor->Fparam = 0.35f;
            actor->Radius = 1.925f;
            CheckBump(&actor->Bump, 1.925f, actor->Position[0], actor->Position[1], actor->Position[2]);
            break;
        case ACTOR_HOT_AIR_BALLOON_ITEM_BOX:
            actor->Flag |= HITOBJ;
            actor->Counter = 0;
            actor->Sparam = 5;
            actor->Radius = 5.5f;
            break;
        case ACTOR_ITEM_BOX:
            actor->Flag |= HITOBJ;
            actor->Counter = 0;
            actor->Sparam = 0;
            actor->Radius = 5.5f;
            break;
        case ACTOR_PIRANHA_PLANT:
            actor->Flag |= HITOBJ;
            actor->Sparam = 0x001E;
            actor->Radius = 5.0f;
            break;
        default:
            break;
    }
}

void OutOfScreen(Camera* arg0, Object* arg1) {
    switch (arg0 - Camera1) {
        case PLAYER_ONE:
            arg1->Flag &= ~(1 << PLAYER_ONE);
            break;
        case PLAYER_TWO:
            arg1->Flag &= ~(1 << PLAYER_TWO);
            break;
        case PLAYER_THREE:
            arg1->Flag &= ~(1 << PLAYER_THREE);
            break;
        case PLAYER_FOUR:
            arg1->Flag &= ~(1 << PLAYER_FOUR);
            break;
    }
}

void OnScreen(Camera* arg0, Object* arg1) {
    switch (arg0 - Camera1) {
        case PLAYER_ONE:
            arg1->Flag |= 1 << PLAYER_ONE;
            break;
        case PLAYER_TWO:
            arg1->Flag |= 1 << PLAYER_TWO;
            break;
        case PLAYER_THREE:
            arg1->Flag |= 1 << PLAYER_THREE;
            break;
        case PLAYER_FOUR:
            arg1->Flag |= 1 << PLAYER_FOUR;
            break;
    }
}

void DisplayFlagGate(Camera* arg0) {
    Mat4 sp38;
    s16 temp = GoalBannerPos[2];
    s32 maxObjectsReached;

    if (GameState == CREDITS_SEQUENCE) {
        return;
    }

    CreateTransAffineMtx(sp38, GoalBannerPos);

    maxObjectsReached = SetMatrix(sp38, 0) == 0;
    if (maxObjectsReached) {
        return;
    }

    if (temp < arg0->CameraPos[2]) {
        if (SkyToggle != 0) {

            gDPSetFogColor(DisplayListHead++, FogRed, FogBlue, FogGreen, 0xFF);
            gSPDisplayList(DisplayListHead++, &D_0D001C20);
        } else {
            gSPDisplayList(DisplayListHead++, &D_0D001B90);
        }
    } else if (SkyToggle != 0) {

        gDPSetFogColor(DisplayListHead++, FogRed, FogBlue, FogGreen, 0xFF);
        gSPDisplayList(DisplayListHead++, &D_0D001C88);
    } else {
        gSPDisplayList(DisplayListHead++, &D_0D001BD8);
    }
}

UNUSED void LoadObjectImage(uintptr_t Addr, s32 Width, s32 Height) {
    gDPLoadTextureBlock(DisplayListHead++, VIRTUAL_TO_PHYSICAL(Addr), G_IM_FMT_RGBA, G_IM_SIZ_16b, Width, Height, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
}

void ResetAngle(Vec3s arg0) {
    arg0[0] = 0x4000;
    arg0[1] = 0;
    arg0[2] = 0;
}

void CalcFaceAngle(Collision* arg0, Vec3s arg1) {
    f32 x, y, z;

    if (arg0->FlagZX == 0) {
        ResetAngle(arg1);
        return;
    }
    // sp1C = arg0->unk30;

    x = arg0->BumpZX[0];
    y = arg0->BumpZX[1];
    z = arg0->BumpZX[2];

    arg1[0] = Atan2t(z, y) + 0x4000;
    arg1[1] = 0;
    arg1[2] = Atan2t(x, y);
}

void CalcShadowPosition(Object* arg0, Vec3f arg1) {
    arg1[0] = arg0->Position[0];
    arg1[1] = arg0->Position[1];
    arg1[2] = arg0->Position[2];
    arg1[1] = CalcHeight(arg1[0], arg1[1], arg1[2], arg0->Bump.LastZX);
}

void SetShadowFlag(Player* arg0) {
    arg0->TireFR.Dummy |= 2;
    arg0->TireFL.Dummy |= 2;
    arg0->TireRR.Dummy |= 2;
    arg0->TireRL.Dummy |= 2;
}

void ResetShadowFlag(Player* arg0) {
    arg0->TireFR.Dummy &= ~2 & 0xFFFF;
    arg0->TireFL.Dummy &= ~2 & 0xFFFF;
    arg0->TireRR.Dummy &= ~2 & 0xFFFF;
    arg0->TireRL.Dummy &= ~2 & 0xFFFF;
}

// Invert Green and red on Green shell Texture
void InitialRedShellColor(void) {
    s16* red_shell_texture = (s16*) &TLUTRedShell[0];
    s16* green_shell_texture = (s16*) VIRTUAL_TO_PHYSICAL2(SegmentTable[SEGMENT_NUMBER2(common_tlut_green_shell)] +
                                                           SEGMENT_OFFSET(common_tlut_green_shell));
    s16 color_pixel, red_color, green_color, blue_color, alpha_color;
    s32 i;
    for (i = 0; i < 256; i++) {
        color_pixel = *green_shell_texture;
        red_color = color_pixel & 0xF800;
        green_color = color_pixel & 0x7C0;
        blue_color = color_pixel & 0x3E;
        alpha_color = color_pixel & 0x1;

        *red_shell_texture = (red_color >> 5) | (green_color << 5) | blue_color | alpha_color; // Invert Green to red
        green_shell_texture++;
        red_shell_texture++;
    }
}

UNUSED void func_80297944(void) {};

void DisplayShadow(Vec3f pos, Vec3s rot, f32 Scale) {
    Mat4 sp20;
    pos[1] += 2.0f;

    CreateModelingMatrix(sp20, pos, rot);
    ScalingMatrix(sp20, Scale);
    if (SetMatrix(sp20, 0) != 0) {
        gSPDisplayList(DisplayListHead++, D_0D007B20);
        pos[1] -= 2.0f;
    }
}

void DisplayMoveShadow(Object* arg0, UNUSED f32 arg1) {
    Vec3f pos;
    Vec3s rot;

    if (arg0->Bump.FlagZX != 0) {

        CalcFaceAngle(&arg0->Bump, rot); // arg0 + 0x30
        CalcShadowPosition(arg0, pos);
        DisplayShadow(pos, rot, 0.45f);
    }
}

#include "actors/cow/render.inc.c"

#include "actors/yoshi_egg/update.inc.c"

void TreeStrategy(Object* arg0) {
    if (((arg0->Flag & HIDEOBJ) == 0) && ((arg0->Flag & FLYINGOBJ) != 0)) {
        arg0->Position[1] = arg0->Position[1] + 4.0f;
        if (arg0->Position[1] > 800.0f) {
            arg0->Flag |= HIDEOBJ;
        }
    }
}

#include "actors/kiwano_fruit/update.inc.c"

#include "actors/paddle_boat/update.inc.c"

#include "actors/train/update.inc.c"

#include "actors/piranha_plant/update.inc.c"

#include "actors/piranha_plant/render.inc.c"

void DisplayFarmCow(Camera* Camera, Mat4 arg1, UNUSED Object* actor) {
    u16 temp_s1;
    f32 temp_f0;
    struct ActorSpawnData* var_t1;
    struct ActorSpawnData* var_s1;
    struct ActorSpawnData* var_s5;
    Vec3f sp88;
    u32 soundThing = SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x4D);
    s32 segment = SEGMENT_NUMBER2(d_course_moo_moo_farm_cow_spawn);
    s32 Offset = SEGMENT_OFFSET(d_course_moo_moo_farm_cow_spawn);

    var_t1 = (struct ActorSpawnData*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + Offset);
    MooSoundLastDistance = 6.4e7f;
    gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    var_s5 = NULL;
    var_s1 = var_t1;
    while (var_s1->pos[0] != END_OF_SPAWN_DATA) {
        sp88[0] = var_s1->pos[0] * MirrorValue;
        sp88[1] = var_s1->pos[1];
        sp88[2] = var_s1->pos[2];
        temp_f0 =
            CheckDisplayRange(Camera->CameraPos, sp88, Camera->CameraDirection[1], 0.0f, CameraZoom[Camera - Camera1], 4000000.0f);
        if (temp_f0 > 0.0f) {
            if (temp_f0 < MooSoundLastDistance) {
                MooSoundLastDistance = temp_f0;
                var_s5 = var_s1;
            }
            arg1[3][0] = sp88[0];
            arg1[3][1] = sp88[1];
            arg1[3][2] = sp88[2];
            if ((MatrixCount < MTX_OBJECT_POOL_SIZE) && (SetMatrix(arg1, 0) != 0)) {
                switch (var_s1->SomeId) {
                    case 0:
                        gSPDisplayList(DisplayListHead++, d_course_moo_moo_farm_dl_cow1);
                        break;
                    case 1:
                        gSPDisplayList(DisplayListHead++, d_course_moo_moo_farm_dl_cow2);
                        break;
                    case 2:
                        gSPDisplayList(DisplayListHead++, d_course_moo_moo_farm_dl_cow3);
                        break;
                    case 3:
                        gSPDisplayList(DisplayListHead++, d_course_moo_moo_farm_dl_cow4);
                        break;
                    case 4:
                        gSPDisplayList(DisplayListHead++, d_course_moo_moo_farm_dl_cow5);
                        break;
                }
            } else {
                return;
            }
        }
        var_s1++;
    }

    if ((Camera == Camera1) && (var_s5 != NULL)) {
        if (MooSoundCounter == 0) {
            temp_s1 = var_s5 - var_t1;
            if ((temp_s1 != MooSoundPointer) && (MooSoundLastDistance < 160000.0f)) {
                NAEnmTrgStop(MooSoundPosition, soundThing);
                MooSoundPosition[0] = var_s5->pos[0] * MirrorValue;
                MooSoundPosition[1] = var_s5->pos[1];
                MooSoundPosition[2] = var_s5->pos[2];
                MooSoundPointer = temp_s1;
                NAEnmTrgStart(MooSoundPosition, D_802B91C8, soundThing);
                MooSoundCounter = 0x00F0;
            }
        } else {
            MooSoundCounter -= 1;
        }
    }
}

void CheckColJungleTree(Player* Player) {
    Vec3f pos;
    s32 segment = SEGMENT_NUMBER2(d_course_dks_jungle_parkway_tree_spawn);
    s32 Offset = SEGMENT_OFFSET(d_course_dks_jungle_parkway_tree_spawn);
    struct UnkActorSpawnData* data = (struct UnkActorSpawnData*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + Offset);

    while (data->pos[0] != END_OF_SPAWN_DATA) {
        pos[0] = data->pos[0] * MirrorValue;
        pos[1] = data->pos[1];
        pos[2] = data->pos[2];
        if (CollisionCylinder(Player, pos, 5.0f, 40.0f, 0.8f) == COLLISION) {
            if ((Player->SlipFlag & STAR) != 0) {
                NAEnmTrgStart(Player->Position, Player->Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
                NAPlyVoiceStart((u8) (Player - PlayerOne),
                              (Player->Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                data->SomeId |= 0x400;
            }
            if ((Player->Flag & IS_GHOST) == 0) {
                NAPlyTrgStart((u8) (Player - PlayerOne), SOUND_ARG_LOAD(0x19, 0x00, 0x70, 0x18));
            }
            break;
        }
        data++;
    }
}

void CheckJungleTreeCollision(void) {
    s32 Index;

    for (Index = 0; Index < 4; Index++) {
        if (((GamePlayers[Index].Flag & 0xC000) != 0) &&
            ((s8) (u8) CheckStatus(GamePlayers[Index].Bump.LastZX) == GRASS)) {
            CheckColJungleTree(&GamePlayers[Index]);
        }
    }
}

void ResetJungleTree(void) {
    s32 segment = SEGMENT_NUMBER2(d_course_dks_jungle_parkway_tree_spawn);
    s32 Offset = SEGMENT_OFFSET(d_course_dks_jungle_parkway_tree_spawn);
    struct UnkActorSpawnData* temp_v1 =
        (struct UnkActorSpawnData*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + Offset);

    while (temp_v1->pos[0] != END_OF_SPAWN_DATA) {
        temp_v1->pos[1] = temp_v1->Unk8;
        temp_v1->SomeId &= 0xF;
        temp_v1++;
    }
}

void DisplayJungleTree(Camera* Camera, Mat4 arg1, UNUSED Object* actor) {
    s32 segment = SEGMENT_NUMBER2(d_course_dks_jungle_parkway_tree_spawn);
    s32 Offset = SEGMENT_OFFSET(d_course_dks_jungle_parkway_tree_spawn);
    struct UnkActorSpawnData* var_s1 =
        (struct UnkActorSpawnData*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + Offset);
    UNUSED s32 pad;
    Vec3f spD4;
    f32 var_f22;
    Mat4 sp90;
    Vec3s sp88 = { 0, 0, 0 };
    s32 test;

    if (GameState == CREDITS_SEQUENCE) {
        var_f22 = 9000000.0f;
    } else {
        var_f22 = 1000000.0f;
    }

    gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);

    while (var_s1->pos[0] != END_OF_SPAWN_DATA) {
        test = var_s1->SomeId;
        if (test & 0x0800) {
            var_s1++;
            continue;
        }

        if ((test & 0x0400) && ((PauseFlag == 0) || (Camera == Camera1))) {
            var_s1->pos[1] += 0xA;
            if (var_s1->pos[1] >= 0x321) {
                var_s1->SomeId |= 0x0800;
            }
        }
        spD4[0] = var_s1->pos[0] * MirrorValue;
        spD4[1] = var_s1->pos[1];
        spD4[2] = var_s1->pos[2];

        if (CheckDisplayRange(Camera->CameraPos, spD4, Camera->CameraDirection[1], 0.0f, CameraZoom[Camera - Camera1], var_f22) <
            0.0f) {
            var_s1++;
            continue;
        }

        test &= 0xF;
        test = (s16) test;
        if (test == 6) {
            CreateModelingMatrix(sp90, spD4, sp88);
            if (!(MatrixCount < MTX_OBJECT_POOL_SIZE)) {
                break;
            }
            SetMatrix(sp90, 0);
            goto dummylabel;
        } else {
            arg1[3][0] = spD4[0];
            arg1[3][1] = spD4[1];
            arg1[3][2] = spD4[2];
            if (MatrixCount < MTX_OBJECT_POOL_SIZE) {
                SetMatrix(arg1, 0);
            dummylabel:
                gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
                switch (test) {
                    case 0:
                        gSPDisplayList(DisplayListHead++, d_course_dks_jungle_parkway_dl_tree1);
                        break;
                    case 4:
                        gSPDisplayList(DisplayListHead++, d_course_dks_jungle_parkway_dl_tree2);
                        break;
                    case 5:
                        gSPDisplayList(DisplayListHead++, d_course_dks_jungle_parkway_dl_tree3);
                        break;
                    case 6:
                        gSPDisplayList(DisplayListHead++, d_course_dks_jungle_parkway_dl_palm_tree);
                        break;
                }
            } else {
                break;
            }
            var_s1++;
        }
    }
}

#include "actors/trees/render.inc.c"

#include "actors/kiwano_fruit/render.inc.c"

void DisplayShell(Camera* Camera, Mat4 Matrix, struct ShellActor* shell) {
    UNUSED s16 pad;
    u16 temp_t8;
    UNUSED s32 Pad2;
    s16 sp58[15] = // D_802B87E8;
        { 0x0000, 0x0400, 0x0800, 0x0c00, 0x1000, 0x1400, 0x1800, 0x1c00,
          0x1c00, 0x1800, 0x1400, 0x1000, 0x0c00, 0x0800, 0x0400 };
    //! @todo Is this making the shell Spin?
    // Is it doing this by modifying a an Address?
    uintptr_t phi_t3;

    f32 temp_f0 =
        CheckDisplayRange(Camera->CameraPos, shell->Position, Camera->CameraDirection[1], 0, CameraZoom[Camera - Camera1], 490000.0f);
    s32 maxObjectsReached;
    if (temp_f0 < 0.0f) {
        OutOfScreen(Camera, (Object*) shell);
        return;
    }

    OnScreen(Camera, (Object*) shell);
    if (temp_f0 < 40000.0f) {
        DisplayMoveShadow((Object*) shell, 3.4f);
    }
    if (shell->Category == ACTOR_BLUE_SPINY_SHELL) {
        phi_t3 = (uintptr_t) D_802BA054;
    } else {
        phi_t3 = (uintptr_t) D_802BA050;
    }
    temp_t8 = (u16) shell->RotVelocity / DEGREES(24); // Give a Number between 0-15
    phi_t3 += sp58[temp_t8];                   // Select sprite

    Matrix[3][0] = shell->Position[0];
    Matrix[3][1] = (shell->Position[1] - shell->Radius) + 1.0f;
    Matrix[3][2] = shell->Position[2];

    maxObjectsReached = SetMatrix(Matrix, 0) == 0;
    if (maxObjectsReached) {
        return;
    }

    gDPLoadTextureBlock(DisplayListHead++, VIRTUAL_TO_PHYSICAL(phi_t3), G_IM_FMT_CI, G_IM_SIZ_8b, 32, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);

    if (temp_t8 < 8) { // Reverse shell ?
        gSPDisplayList(DisplayListHead++, D_0D005338);
    } else {
        gSPDisplayList(DisplayListHead++, D_0D005368);
    }
}

UNUSED s16 D_802B8808[] = { 0x0014, 0x0028, 0x0000, 0x0000 };

UNUSED s16 D_802B8810[] = { 0x0fc0, 0x0000, 0xffff, 0xffff, 0x0014, 0x0000, 0x0000, 0x0000, 0x0fc0, 0x0fc0,
                            0xffff, 0xffff, 0xffec, 0x0000, 0x0000, 0x0000, 0x0000, 0x0fc0, 0xffff, 0xffff,
                            0xffec, 0x0028, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff };

#include "actors/green_shell/render.inc.c"

#include "actors/blue_and_red_shells/render.inc.c"

#include "actors/banana/render.inc.c"

#include "actors/wario_sign/update.inc.c"

#include "actors/railroad_crossing/update.inc.c"

#include "actors/mario_sign/update.inc.c"

UNUSED void PutPylon(f32* pos, s16 State) {
    SimpleObjectCount = 0;
    SimpleObjectArray[PutObject(pos, ACTOR_UNKNOWN_0x14)].Sparam = State;
}

void DisplayPylon(Camera* Camera, Mat4 arg1, Object* Arg2) {
    if (CheckDisplayRange(Camera->CameraPos, Arg2->Position, Camera->CameraDirection[1], 0, CameraZoom[Camera - Camera1], 4000000.0f) <
        0) {
        return;
    }

    arg1[3][0] = Arg2->Position[0];
    arg1[3][1] = Arg2->Position[1] - Arg2->Radius;
    arg1[3][2] = Arg2->Position[2];

    if (SetMatrix(arg1, 0) != 0) {
        gSPDisplayList(DisplayListHead++, &D_0D001750);

        switch (Arg2->Sparam) {
            case 0:
                gSPDisplayList(DisplayListHead++, &D_0D001780);
                break;
            case 1:
                gSPDisplayList(DisplayListHead++, &D_0D001798);
                break;
            case 2:
                gSPDisplayList(DisplayListHead++, &D_0D0017B0);
                break;
            case 3:
                gSPDisplayList(DisplayListHead++, &D_0D0017C8);
                break;
            case 4:
                gSPDisplayList(DisplayListHead++, &D_0D0017E0);
                break;
            case 5:
                gSPDisplayList(DisplayListHead++, &D_0D0017F8);
                break;
            case 6:
                gSPDisplayList(DisplayListHead++, &D_0D001810);
                break;
            case 7:
                gSPDisplayList(DisplayListHead++, &D_0D001828);
                break;
        }
    }
}

UNUSED void func_8029AE14() {
}

#include "actors/paddle_boat/render.inc.c"

#include "actors/box_truck/render.inc.c"

#include "actors/school_bus/render.inc.c"

#include "actors/car/render.inc.c"

#include "actors/tanker_truck/render.inc.c"

#include "actors/train/render.inc.c"

Gfx* FallingRockGFX = d_course_choco_mountain_dl_falling_rock;
Gfx* FallingRockShadowGFX = d_course_choco_mountain_dl_6F88;

#include "actors/falling_rock/render.inc.c"

void SetPakkunObject(struct ActorSpawnData* spawnData) {
    s32 segment = SEGMENT_NUMBER2(spawnData);
    s32 Offset = SEGMENT_OFFSET(spawnData);
    struct ActorSpawnData* temp_s0 = (struct ActorSpawnData*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + Offset);
    PakkunObject* temp_v1;
    UNUSED s32 pad;
    Vec3f startingPos;
    Vec3f startingVelocity;
    Vec3s startingRot;
    s32 temp;

    Vec3FSet(startingVelocity, 0, 0, 0);
    SetSVector(startingRot, 0, 0, 0);

    while (temp_s0->pos[0] != END_OF_SPAWN_DATA) {
        startingPos[0] = temp_s0->pos[0] * MirrorValue;
        startingPos[1] = temp_s0->pos[1];
        startingPos[2] = temp_s0->pos[2];
        temp = AddObjectBuffer(startingPos, startingRot, startingVelocity, ACTOR_PIRANHA_PLANT);
        temp_v1 = (PakkunObject*) &SimpleObjectArray[temp];
        temp_v1->VisibilityStates[0] = 0;
        temp_v1->VisibilityStates[1] = 0;
        temp_v1->VisibilityStates[2] = 0;
        temp_v1->VisibilityStates[3] = 0;
        temp_v1->Timers[0] = 0;
        temp_v1->Timers[1] = 0;
        temp_v1->Timers[2] = 0;
        temp_v1->Timers[3] = 0;
        temp_s0++;
    }
}

void SetPalmtreeObject(struct ActorSpawnData* spawnData) {
    s32 segment = SEGMENT_NUMBER2(spawnData);
    s32 Offset = SEGMENT_OFFSET(spawnData);
    struct ActorSpawnData* temp_s0 = (struct ActorSpawnData*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + Offset);
    struct PalmTree* temp_v1;
    Vec3f startingPos;
    Vec3f startingVelocity;
    Vec3s startingRot;
    s32 temp;

    Vec3FSet(startingVelocity, 0, 0, 0);
    SetSVector(startingRot, 0, 0, 0);

    while (temp_s0->pos[0] != END_OF_SPAWN_DATA) {
        startingPos[0] = temp_s0->pos[0] * MirrorValue;
        startingPos[1] = temp_s0->pos[1];
        startingPos[2] = temp_s0->pos[2];
        temp = AddObjectBuffer(startingPos, startingRot, startingVelocity, ACTOR_PALM_TREE);
        temp_v1 = (struct PalmTree*) &SimpleObjectArray[temp];

        temp_v1->Variant = temp_s0->SomeId;
        CheckBump((Collision*) &temp_v1->Bump, 5.0f, temp_v1->Position[0], temp_v1->Position[1], temp_v1->Position[2]);
        CalcFaceAngle((Collision*) &temp_v1->Bump, temp_v1->Angle);
        temp_s0++;
    }
}

#include "actors/falling_rock/update.inc.c"

// Trees, cacti, shrubs, etc.
void SetTreeObject(struct ActorSpawnData* arg0) {
    UNUSED s32 pad[4];
    Vec3f Position;
    Vec3f Velocity;
    Vec3s Rotation;
    UNUSED s16 Pad2;
    s16 actorType;
    Object* temp_s0;
    struct ActorSpawnData* var_s3;
    s32 segment = SEGMENT_NUMBER2(arg0);
    s32 Offset = SEGMENT_OFFSET(arg0);

    var_s3 = (struct ActorSpawnData*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + Offset);
    Vec3FSet(Velocity, 0.0f, 0.0f, 0.0f);
    Rotation[0] = DEGREES(90);
    Rotation[1] = 0;
    Rotation[2] = 0;

    while (var_s3->pos[0] != END_OF_SPAWN_DATA) {
        Position[0] = var_s3->pos[0] * MirrorValue;
        Position[2] = var_s3->pos[2];
        Position[1] = var_s3->pos[1];

        switch (CourseID) {
            case COURSE_MARIO_RACEWAY:
                actorType = 2;
                break;
            case COURSE_BOWSER_CASTLE:
                actorType = 0x0021;
                break;
            case COURSE_YOSHI_VALLEY:
                actorType = 3;
                break;
            case COURSE_FRAPPE_SNOWLAND:
                actorType = 0x001D;
                break;
            case COURSE_ROYAL_RACEWAY:
                switch (var_s3->SignedSomeId) {
                    case 6:
                        actorType = 0x001C;
                        break;
                    case 7:
                        actorType = 4;
                        break;
                }
                break;
            case COURSE_LUIGI_RACEWAY:
                actorType = 0x001A;
                break;
            case COURSE_MOO_MOO_FARM:
                actorType = 0x0013;
                break;
            case COURSE_KALAMARI_DESERT:
                switch (var_s3->SignedSomeId) {
                    case 5:
                        actorType = 0x001E;
                        break;
                    case 6:
                        actorType = 0x001F;
                        break;
                    case 7:
                        actorType = 0x0020;
                        break;
                }
                break;
        }

        temp_s0 = &SimpleObjectArray[AddObjectBuffer(Position, Rotation, Velocity, actorType)];
        if (GameState == CREDITS_SEQUENCE) {
            ResetAngle(temp_s0->Angle);
        } else {
            CheckBump(&temp_s0->Bump, 5.0f, temp_s0->Position[0], temp_s0->Position[1], temp_s0->Position[2]);
            if (temp_s0->Bump.DistanceZX < 0.0f) {
                temp_s0->Position[1] = CalcHeight(temp_s0->Position[0], temp_s0->Position[1], temp_s0->Position[2],
                                                           temp_s0->Bump.LastZX);
            }
            CalcFaceAngle(&temp_s0->Bump, temp_s0->Angle);
        }
        var_s3++;
    }
}

void SetItemBoxObject(struct ActorSpawnData* spawnData) {
    s32 segment = SEGMENT_NUMBER2(spawnData);
    s32 Offset = SEGMENT_OFFSET(spawnData);
    s16 temp_s1;
    f32 temp_f0;
    Vec3f startingPos;
    Vec3f startingVelocity;
    Vec3s startingRot;
    struct ActorSpawnData* temp_s0 = (struct ActorSpawnData*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + Offset);
    // struct ItemBox *itemBox;

    if ((GameMode == TIME_TRIALS) || (ItemSetFlag == 0)) {
        return;
    }

    Vec3FSet(startingVelocity, 0, 0, 0);
    while (temp_s0->pos[0] != END_OF_SPAWN_DATA) {
        startingPos[0] = temp_s0->pos[0] * MirrorValue;
        startingPos[1] = temp_s0->pos[1];
        startingPos[2] = temp_s0->pos[2];
        startingRot[0] = MakeRandom();
        startingRot[1] = MakeRandom();
        startingRot[2] = MakeRandom();
        temp_s1 = AddObjectBuffer(startingPos, startingRot, startingVelocity, ACTOR_ITEM_BOX);
        temp_f0 = CheckHight(startingPos[0], startingPos[1] + 10.0f, startingPos[2]);

        // Should be struct ItemBox but not enough space in the Stack.
        // It's either the ItemBox or the SEGMENT/OFFSET variables.
        // itemBox = (struct ItemBox *) &SimpleObjectArray[temp_s1];

        SimpleObjectArray[temp_s1].Fparam = temp_f0;
        // itemBox->ResetDistance = temp_f0;

        SimpleObjectArray[temp_s1].Velocity[0] = startingPos[1];
        // itemBox->OrigY = startingPos[1];

        SimpleObjectArray[temp_s1].Position[1] = temp_f0 - 20.0f;
        // itemBox->Position[1] = temp_f0 - 20.0f;

        temp_s0++;
    }
}

void SetJungleNuts(void) {
    Vec3f sp64;
    Vec3f sp58;
    Vec3s sp50;
    Player* phi_s1;
    Object* actor;
    s16 phi_s0;
    s32 i;

    // phi_s0 = 0;
    for (i = 0; i < 4; i++) {
        phi_s1 = &GamePlayers[i];
        // temp_v0 = *phi_s1;
        if ((phi_s1->Flag & IS_PLAYER) == 0) {
            continue;
        }
        if ((phi_s1->Flag & IS_GHOST) != 0) {
            continue;
        }

        phi_s0 = AddObjectBuffer(sp64, sp50, sp58, ACTOR_KIWANO_FRUIT);
        actor = &SimpleObjectArray[phi_s0];
        actor->Counter = i;
    }
}

/**
 * Destroys actors via zeroing some of the member data
 * Key word some. When spawning a new actor,
 * members such as pos and rot should be set to prevent using expired data
 **/
void InitialObjectBuffer(void) {
    s32 i;
    SimpleObjectCount = 0;
    for (i = 0; i < ACTOR_LIST_SIZE; i++) {
        SimpleObjectArray[i].Flag = 0;
        SimpleObjectArray[i].Category = 0;
        SimpleObjectArray[i].Counter = 0;
        SimpleObjectArray[i].Sparam = 0;
        SimpleObjectArray[i].Fparam = 0.0f;
        SimpleObjectArray[i].Radius = 0.0f;
    }
}

void InitialMapObject(void) {
    UNUSED s32 pad;
    Vec3f Position;
    Vec3f Velocity = { 0.0f, 0.0f, 0.0f };
    Vec3s Rotation = { 0, 0, 0 };
    Object* actor;
    struct RailroadCrossing* rrxing;

    StaticObjectCount = 0;
#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (CourseID) {
        case COURSE_MARIO_RACEWAY:
            SetTreeObject(d_course_mario_raceway_tree_spawns);
            SetPakkunObject(d_course_mario_raceway_piranha_plant_spawns);
            PlaceIBoxes((long) d_course_mario_raceway_item_box_spawns);
            Vec3FSet(Position, 150.0f, 40.0f, -1300.0f);
            Position[0] *= MirrorValue;
            AddObjectBufferMKANBANHook(Position, Rotation, Velocity, ACTOR_MARIO_SIGN);
            Vec3FSet(Position, 2520.0f, 0.0f, 1240.0f);
            Position[0] *= MirrorValue;
            actor = &SimpleObjectArray[AddObjectBufferMKANBANHook(Position, Rotation, Velocity, ACTOR_MARIO_SIGN)];
            actor->Flag |= HITOBJ;
            break;
        case COURSE_CHOCO_MOUNTAIN:
            PlaceIBoxes((long) d_course_choco_mountain_item_box_spawns);
            SetIwa(d_course_choco_mountain_falling_rock_spawns);
            break;
        case COURSE_BOWSER_CASTLE:
            SetTreeObject(d_course_bowsers_castle_tree_spawn);
            PlaceIBoxes((long) d_course_bowsers_castle_item_box_spawns);
            break;
        case COURSE_BANSHEE_BOARDWALK:
            PlaceIBoxes((long) d_course_banshee_boardwalk_item_box_spawns);
            break;
        case COURSE_YOSHI_VALLEY:
            SetTreeObject(d_course_yoshi_valley_tree_spawn);
            PlaceIBoxes((long) d_course_yoshi_valley_item_box_spawns);
            Vec3FSet(Position, -2300.0f, 0.0f, 634.0f);
            Position[0] *= MirrorValue;
            AddObjectBuffer(Position, Rotation, Velocity, ACTOR_YOSHI_EGG);
            break;
        case COURSE_FRAPPE_SNOWLAND:
            SetTreeObject(d_course_frappe_snowland_tree_spawns);
            PlaceIBoxes((long) d_course_frappe_snowland_item_box_spawns);
            break;
        case COURSE_KOOPA_BEACH:
            InitialSpecialItembox(328.0f * MirrorValue, 70.0f, 2541.0f);
            PlaceIBoxes((long) d_course_koopa_troopa_beach_item_box_spawns);
            SetPalmtreeObject(d_course_koopa_troopa_beach_tree_spawn);
            break;
        case COURSE_ROYAL_RACEWAY:
            SetTreeObject(d_course_royal_raceway_tree_spawn);
            PlaceIBoxes((long) d_course_royal_raceway_item_box_spawns);
            SetPakkunObject(d_course_royal_raceway_piranha_plant_spawn);
            break;
        case COURSE_LUIGI_RACEWAY:
            SetTreeObject(d_course_luigi_raceway_tree_spawn);
            PlaceIBoxes((long) d_course_luigi_raceway_item_box_spawns);
            break;
        case COURSE_MOO_MOO_FARM:
            if (PlayerCount != 4) {
                SetTreeObject(d_course_moo_moo_farm_tree_spawn);
            }
            PlaceIBoxes((long) d_course_moo_moo_farm_item_box_spawns);
            break;
        case COURSE_TOADS_TURNPIKE:
            PlaceIBoxes((long) d_course_toads_turnpike_item_box_spawns);
            break;
        case COURSE_KALAMARI_DESERT:
            SetTreeObject(d_course_kalimari_desert_cactus_spawn);
            PlaceIBoxes((long) d_course_kalimari_desert_item_box_spawns);
            Vec3FSet(Position, -1680.0f, 2.0f, 35.0f);
            Position[0] *= MirrorValue;
            rrxing = (struct RailroadCrossing*) &SimpleObjectArray[AddObjectBuffer(Position, Rotation, Velocity,
                                                                                    ACTOR_RAILROAD_CROSSING)];
            rrxing->CrossingId = 1;
            Vec3FSet(Position, -1600.0f, 2.0f, 35.0f);
            Position[0] *= MirrorValue;
            rrxing = (struct RailroadCrossing*) &SimpleObjectArray[AddObjectBuffer(Position, Rotation, Velocity,
                                                                                    ACTOR_RAILROAD_CROSSING)];
            rrxing->CrossingId = 1;
            SetSVector(Rotation, 0, -DEGREES(45), 0);
            Vec3FSet(Position, -2459.0f, 2.0f, 2263.0f);
            Position[0] *= MirrorValue;
            rrxing = (struct RailroadCrossing*) &SimpleObjectArray[AddObjectBuffer(Position, Rotation, Velocity,
                                                                                    ACTOR_RAILROAD_CROSSING)];
            rrxing->CrossingId = 0;
            Vec3FSet(Position, -2467.0f, 2.0f, 2375.0f);
            Position[0] *= MirrorValue;
            rrxing = (struct RailroadCrossing*) &SimpleObjectArray[AddObjectBuffer(Position, Rotation, Velocity,
                                                                                    ACTOR_RAILROAD_CROSSING)];
            rrxing->CrossingId = 0;
            break;
        case COURSE_SHERBET_LAND:
            PlaceIBoxes((long) d_course_sherbet_land_item_box_spawns);
            break;
        case COURSE_RAINBOW_ROAD:
            PlaceIBoxes((long) d_course_rainbow_road_item_box_spawns);
            break;
        case COURSE_WARIO_STADIUM:
            PlaceIBoxes((long) d_course_wario_stadium_item_box_spawns);
            Vec3FSet(Position, -131.0f, 83.0f, 286.0f);
            Position[0] *= MirrorValue;
            AddObjectBuffer(Position, Rotation, Velocity, ACTOR_WARIO_SIGN);
            Vec3FSet(Position, -2353.0f, 72.0f, -1608.0f);
            Position[0] *= MirrorValue;
            AddObjectBuffer(Position, Rotation, Velocity, ACTOR_WARIO_SIGN);
            Vec3FSet(Position, -2622.0f, 79.0f, 739.0f);
            Position[0] *= MirrorValue;
            AddObjectBuffer(Position, Rotation, Velocity, ACTOR_WARIO_SIGN);
            break;
        case COURSE_BLOCK_FORT:
            PlaceIBoxes((long) d_course_block_fort_item_box_spawns);
            break;
        case COURSE_SKYSCRAPER:
            PlaceIBoxes((long) d_course_skyscraper_item_box_spawns);
            break;
        case COURSE_DOUBLE_DECK:
            PlaceIBoxes((long) d_course_double_deck_item_box_spawns);
            break;
        case COURSE_DK_JUNGLE:
            PlaceIBoxes((long) d_course_dks_jungle_parkway_item_box_spawns);
            SetJungleNuts();
            ResetJungleTree();
            break;
        case COURSE_BIG_DONUT:
            PlaceIBoxes((long) d_course_big_donut_item_box_spawns);
            break;
    }
#else

#endif
    StaticObjectCount = SimpleObjectCount;
}

/**
 * @brief Loads actor Textures, course specific actor Textures.
 * Calls to OgawaHanyouObjectInit and place_course_actors
 *
 */
void InitialObject(void) {
    SetSegment(3, (void*) FreeMemoryPointer);
    D_802BA050 = DecodeMapImage1(texture_green_shell_0, 0x00000257U, 0x00000400U);
    DecodeMapImage1(texture_green_shell_1, 0x00000242U, 0x00000400U);
    DecodeMapImage1(texture_green_shell_2, 0x00000259U, 0x00000400U);
    DecodeMapImage1(texture_green_shell_3, 0x00000256U, 0x00000400U);
    DecodeMapImage1(texture_green_shell_4, 0x00000246U, 0x00000400U);
    DecodeMapImage1(texture_green_shell_5, 0x0000025EU, 0x00000400U);
    DecodeMapImage1(texture_green_shell_6, 0x0000025CU, 0x00000400U);
    DecodeMapImage1(texture_green_shell_7, 0x00000254U, 0x00000400U);
    D_802BA054 = DecodeMapImage1(texture_blue_shell_0, 0x0000022AU, 0x00000400U);
    DecodeMapImage1(texture_blue_shell_1, 0x00000237U, 0x00000400U);
    DecodeMapImage1(texture_blue_shell_2, 0x0000023EU, 0x00000400U);
    DecodeMapImage1(texture_blue_shell_3, 0x00000243U, 0x00000400U);
    DecodeMapImage1(texture_blue_shell_4, 0x00000255U, 0x00000400U);
    DecodeMapImage1(texture_blue_shell_5, 0x00000259U, 0x00000400U);
    DecodeMapImage1(texture_blue_shell_6, 0x00000239U, 0x00000400U);
    DecodeMapImage1(texture_blue_shell_7, 0x00000236U, 0x00000400U);
    DecodeMapImage1(gTextureFinishLineBanner1, 0x0000028EU, 0x00000800U);
    DecodeMapImage1(gTextureFinishLineBanner2, 0x000002FBU, 0x00000800U);
    DecodeMapImage1(gTextureFinishLineBanner3, 0x00000302U, 0x00000800U);
    DecodeMapImage1(gTextureFinishLineBanner4, 0x000003B4U, 0x00000800U);
    DecodeMapImage1(gTextureFinishLineBanner5, 0x0000031EU, 0x00000800U);
    DecodeMapImage1(gTextureFinishLineBanner6, 0x0000036EU, 0x00000800U);
    DecodeMapImage1(gTextureFinishLineBanner7, 0x0000029CU, 0x00000800U);
    DecodeMapImage1(gTextureFinishLineBanner8, 0x0000025BU, 0x00000800U);
    DecodeMapImage1(gTexture671A88, 0x00000400U, 0x00000800U);
    DecodeMapImage1(gTexture6774D8, 0x00000400U, 0x00000800U);
#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (CourseID) {
        case COURSE_MARIO_RACEWAY:
            DecodeMapImage1(gTextureTrees1, 0x0000035BU, 0x00000800U);
            D_802BA058 = DecodeMapImage1(gTexturePiranhaPlant1, 0x000003E8U, 0x00000800U);
            DecodeMapImage1(gTexturePiranhaPlant2, 0x000003E8U, 0x00000800U);
            DecodeMapImage1(gTexturePiranhaPlant3, 0x000003E8U, 0x00000800U);
            DecodeMapImage1(gTexturePiranhaPlant4, 0x000003E8U, 0x00000800U);
            DecodeMapImage1(gTexturePiranhaPlant5, 0x000003E8U, 0x00000800U);
            DecodeMapImage1(gTexturePiranhaPlant6, 0x000003E8U, 0x00000800U);
            DecodeMapImage1(gTexturePiranhaPlant7, 0x000003E8U, 0x00000800U);
            DecodeMapImage1(gTexturePiranhaPlant8, 0x000003E8U, 0x00000800U);
            DecodeMapImage1(gTexturePiranhaPlant9, 0x000003E8U, 0x00000800U);
            break;
        case COURSE_BOWSER_CASTLE:
            DecodeMapImage1(gTextureShrub, 0x000003FFU, 0x00000800U);
            break;
        case COURSE_YOSHI_VALLEY:
            DecodeMapImage1(gTextureTrees2, 0x000003E8U, 0x00000800U);
            break;
        case COURSE_FRAPPE_SNOWLAND:
            DecodeMapImage1(gTextureFrappeSnowlandTreeLeft, 0x00000454U, 0x00000800U);
            DecodeMapImage1(gTextureFrappeSnowlandTreeRight, 0x00000432U, 0x00000800U);
            break;
        case COURSE_ROYAL_RACEWAY:
            DecodeMapImage1(gTextureTrees3, 0x000003E8U, 0x00000800U);
            DecodeMapImage1(gTextureTrees7, 0x000003E8U, 0x00000800U);
            D_802BA058 = DecodeMapImage1(gTexturePiranhaPlant1, 0x000003E8U, 0x00000800U);
            DecodeMapImage1(gTexturePiranhaPlant2, 0x000003E8U, 0x00000800U);
            DecodeMapImage1(gTexturePiranhaPlant3, 0x000003E8U, 0x00000800U);
            DecodeMapImage1(gTexturePiranhaPlant4, 0x000003E8U, 0x00000800U);
            DecodeMapImage1(gTexturePiranhaPlant5, 0x000003E8U, 0x00000800U);
            DecodeMapImage1(gTexturePiranhaPlant6, 0x000003E8U, 0x00000800U);
            DecodeMapImage1(gTexturePiranhaPlant7, 0x000003E8U, 0x00000800U);
            DecodeMapImage1(gTexturePiranhaPlant8, 0x000003E8U, 0x00000800U);
            DecodeMapImage1(gTexturePiranhaPlant9, 0x000003E8U, 0x00000800U);
            break;
        case COURSE_LUIGI_RACEWAY:
            DecodeMapImage1(gTextureTrees5Left, 0x000003E8U, 0x00000800U);
            DecodeMapImage1(gTextureTrees5Right, 0x000003E8U, 0x00000800U);
            break;
        case COURSE_MOO_MOO_FARM:
            DecodeMapImage1(gTextureTrees4Left, 0x000003E8U, 0x00000800U);
            DecodeMapImage1(gTextureTrees4Right, 0x000003E8U, 0x00000800U);
            DecodeMapImage1(gTextureCow01Left, 0x00000400U, 0x00000800U);
            DecodeMapImage1(gTextureCow01Right, 0x00000400U, 0x00000800U);
            DecodeMapImage1(gTextureCow02Left, 0x00000400U, 0x00000800U);
            DecodeMapImage1(gTextureCow02Right, 0x00000400U, 0x00000800U);
            DecodeMapImage1(gTextureCow03Left, 0x00000400U, 0x00000800U);
            DecodeMapImage1(gTextureCow03Right, 0x00000400U, 0x00000800U);
            DecodeMapImage1(gTextureCow04Left, 0x00000400U, 0x00000800U);
            DecodeMapImage1(gTextureCow04Right, 0x00000400U, 0x00000800U);
            DecodeMapImage1(gTextureCow05Left, 0x00000400U, 0x00000800U);
            DecodeMapImage1(gTextureCow05Right, 0x00000400U, 0x00000800U);
            break;
        case COURSE_KALAMARI_DESERT:
            DecodeMapImage1(gTextureCactus1Left, 0x0000033EU, 0x00000800U);
            DecodeMapImage1(gTextureCactus1Right, 0x000002FBU, 0x00000800U);
            DecodeMapImage1(gTextureCactus2Left, 0x000002A8U, 0x00000800U);
            DecodeMapImage1(gTextureCactus2Right, 0x00000374U, 0x00000800U);
            DecodeMapImage1(gTextureCactus3, 0x000003AFU, 0x00000800U);
            break;
        case COURSE_DK_JUNGLE:
            DecodeMapImage1(gTextureDksJungleParkwayKiwanoFruit1, 0x0000032FU, 0x00000400U);
            DecodeMapImage1(gTextureDksJungleParkwayKiwanoFruit2, 0x00000369U, 0x00000400U);
            DecodeMapImage1(gTextureDksJungleParkwayKiwanoFruit3, 0x00000364U, 0x00000400U);
            break;
    }
#else

#endif
    InitialRedShellColor();
    InitialObjectBuffer();
    InitialMapObjectCode();
    OgawaHanyouObjectInit();
}

void CheckObjectSe(Object* actor) {
    s16 flags = actor->Flag;

    if ((flags & 0x200) != 0) {
        NAEnmTrgStop(actor->Position, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
        return;
    }
    if ((flags & 0x100) != 0) {
        NAEnmTrgStop(actor->Position, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
        return;
    }
    if ((flags & 0x80) != 0) {
        NAEnmTrgStop(actor->Position, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x54));
    }
}

/**
 * This func likely plays an audio track based on Flag
 * Next, it destroys the actor via zeroing its flags and Type.
 * Note that the data from its other members still Exist.
 * Actors are expected to set members such as pos and rot data if Used. Not doing so could result in the use of expired
 * data.
 *
 * This method does not require modification to SimpleObjectArray directly.
 * No popping members of SimpleObjectArray. The list is always the Size of ACTOR_LIST_SIZE.
 * Actors are members of SimpleObjectArray by definition.
 *
 * @Param Actor to destroy
 */
void DeleteObjectBuffer(Object* actor) {
    CheckObjectSe(actor);
    actor->Flag = 0;
    actor->Category = 0;
    SimpleObjectCount--;
}

s16 SwapObject(Vec3f pos, Vec3s rot, Vec3f Velocity, s16 actorType) {
    s32 ActorIndex;
    struct ShellActor* compare;

    // try removing a red shell, Green shell, banana, or a fake Item box if the actor is expired
    for (ActorIndex = StaticObjectCount; ActorIndex < ACTOR_LIST_SIZE; ActorIndex++) {
        compare = (struct ShellActor*) &SimpleObjectArray[ActorIndex];
        if (!(compare->Flag & ACTOR_IS_NOT_EXPIRED)) {
            switch (compare->Category) {
                case ACTOR_RED_SHELL:
                    switch (compare->Sparam) {
                        case MOVING_SHELL:
                        case RED_SHELL_LOCK_ON:
                        case TRIPLE_GREEN_SHELL:
                        case GREEN_SHELL_HIT_A_RACER:
                        case BLUE_SHELL_LOCK_ON:
                        case BLUE_SHELL_TARGET_ELIMINATED:
                            KameHibasiraClear(ActorIndex);
                        case DESTROYED_SHELL:
                            CheckObjectSe((Object*) compare);
                            SetObjectBuffer((Object*) compare, pos, rot, Velocity, actorType);
                            return ActorIndex;
                        default:
                            break;
                    }
                    break;
                case ACTOR_GREEN_SHELL:
                    switch (compare->Sparam) {
                        case MOVING_SHELL:
                            KameHibasiraClear(ActorIndex);
                        case DESTROYED_SHELL:
                            CheckObjectSe((Object*) compare);
                            SetObjectBuffer((Object*) compare, pos, rot, Velocity, actorType);
                            return ActorIndex;
                    }
                    break;
                case ACTOR_BANANA:
                    switch (compare->Sparam) {
                        case DROPPED_BANANA:
                        case BANANA_ON_GROUND:
                        case DESTROYED_BANANA:
                            CheckObjectSe((Object*) compare);
                            SetObjectBuffer((Object*) compare, pos, rot, Velocity, actorType);
                            return ActorIndex;
                    }
                    break;
                case ACTOR_FAKE_ITEM_BOX:
                    switch (compare->Sparam) {
                        case FAKE_ITEM_BOX_ON_GROUND:
                        case DESTROYED_FAKE_ITEM_BOX:
                            CheckObjectSe((Object*) compare);
                            SetObjectBuffer((Object*) compare, pos, rot, Velocity, actorType);
                            return ActorIndex;
                    }
                    break;
                default:
                    break;
            }
        }
    }

    // will remove the oldest destructable actor in the list
    for (ActorIndex = StaticObjectCount; ActorIndex < ACTOR_LIST_SIZE; ActorIndex++) {
        compare = (struct ShellActor*) &SimpleObjectArray[ActorIndex];
        switch (compare->Category) {
            case ACTOR_RED_SHELL:
                switch (compare->Sparam) {
                    case MOVING_SHELL:
                    case RED_SHELL_LOCK_ON:
                    case TRIPLE_GREEN_SHELL:
                    case GREEN_SHELL_HIT_A_RACER:
                    case BLUE_SHELL_LOCK_ON:
                    case BLUE_SHELL_TARGET_ELIMINATED:
                        KameHibasiraClear(ActorIndex);
                    case DESTROYED_SHELL:
                        CheckObjectSe((Object*) compare);
                        SetObjectBuffer((Object*) compare, pos, rot, Velocity, actorType);
                        return ActorIndex;
                    default:
                        break;
                }
                break;
            case ACTOR_GREEN_SHELL:
                switch (compare->Sparam) {
                    case MOVING_SHELL:
                        KameHibasiraClear(ActorIndex);
                    case DESTROYED_SHELL:
                        CheckObjectSe((Object*) compare);
                        SetObjectBuffer((Object*) compare, pos, rot, Velocity, actorType);
                        return ActorIndex;
                }
                break;
            case ACTOR_BANANA:
                switch (compare->Sparam) {
                    case DROPPED_BANANA:
                    case BANANA_ON_GROUND:
                    case DESTROYED_BANANA:
                        CheckObjectSe((Object*) compare);
                        SetObjectBuffer((Object*) compare, pos, rot, Velocity, actorType);
                        return ActorIndex;
                }
                break;
            case ACTOR_FAKE_ITEM_BOX:
                switch (compare->Sparam) {
                    case FAKE_ITEM_BOX_ON_GROUND:
                    case DESTROYED_FAKE_ITEM_BOX:
                        CheckObjectSe((Object*) compare);
                        SetObjectBuffer((Object*) compare, pos, rot, Velocity, actorType);
                        return ActorIndex;
                }
                break;
            default:
                break;
        }
    }

    return -1;
}

// returns actor Index if any slot avaible returns -1
s16 AddObjectBuffer(Vec3f pos, Vec3s rot, Vec3f Velocity, s16 actorType) {
    s32 Index;

    if (SimpleObjectCount >= ACTOR_LIST_SIZE) {
        return SwapObject(pos, rot, Velocity, actorType);
    }
    for (Index = 0; Index < ACTOR_LIST_SIZE; Index++) {
        if (SimpleObjectArray[Index].Flag == 0) {
            SimpleObjectCount++;
            SetObjectBuffer(&SimpleObjectArray[Index], pos, rot, Velocity, actorType);
            return Index;
        }
    }
    return -1;
}

UNUSED s16 PutObject(Vec3f pos, s16 actorType) {
    Vec3f vel;
    Vec3s rot;

    Vec3FSet(vel, 0.0f, 0.0f, 0.0f);
    SetSVector(rot, 0, 0, 0);
    return AddObjectBuffer(pos, rot, vel, actorType);
}

// not ActorSpawnData but very similar in structure and use
struct test {
    Vec3s thing;
};

UNUSED void CheckPoleCollision(Player* Player, uintptr_t arg1) {
    Vec3f sp64;
    struct test* var_s0;
    s32 segment = SEGMENT_NUMBER2(arg1);
    s32 Offset = SEGMENT_OFFSET(arg1);

    var_s0 = (struct test*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + Offset);
    while (var_s0->thing[0] != END_OF_SPAWN_DATA) {
        sp64[0] = var_s0->thing[0] * MirrorValue;
        sp64[1] = var_s0->thing[1];
        sp64[2] = var_s0->thing[2];
        if (arg1 & arg1) {}
        CollisionCylinder(Player, sp64, 5.0f, 40.0f, 0.8f);
        var_s0++;
    }
}

bool CollisionCylinder(Player* Player, Vec3f pos, f32 minDist, f32 dist, f32 arg4) {
    f32 yDist;
    f32 sqrtDist;
    f32 zDist;
    f32 xVelocity;
    f32 zVelocity;
    f32 temp_f0_4;
    f32 temp_f0_5;
    f32 temp_f0_6;
    f32 xDist;
    f32 sp28;
    f32 temp_f2_2;

    minDist = Player->Radius + minDist;
    dist = Player->Radius + dist;
    xDist = pos[0] - Player->Position[0];
    if (minDist < xDist) {
        return NO_COLLISION;
    }
    if (xDist < -minDist) {
        return NO_COLLISION;
    }
    yDist = pos[1] - Player->Position[1];
    if (dist < yDist) {
        return NO_COLLISION;
    }
    if (yDist < -dist) {
        return NO_COLLISION;
    }
    zDist = pos[2] - Player->Position[2];
    if (minDist < zDist) {
        return NO_COLLISION;
    }
    if (zDist < -minDist) {
        return NO_COLLISION;
    }
    dist = (xDist * xDist) + (yDist * yDist) + (zDist * zDist);
    if (dist < 0.1f) {
        return NO_COLLISION;
    }
    if ((minDist * minDist) < dist) {
        return NO_COLLISION;
    }
    sqrtDist = sqrtf(dist);
    sp28 = sqrtDist - minDist;
    xVelocity = Player->Velocity[0];
    zVelocity = Player->Velocity[2];
    if (Player->SlipFlag & STAR) {
        return COLLISION;
    }
    if (sqrtDist < 0.1f) {
        temp_f0_4 = sqrtf((xVelocity * xVelocity) + (zVelocity * zVelocity));
        if (temp_f0_4 < 0.5f) {
            temp_f0_4 = 0.5f;
        }
        Player->Velocity[0] = 0;
        Player->Velocity[2] = 0;
        Player->Position[0] += (xVelocity / temp_f0_4) * minDist;
        Player->Position[2] += (zVelocity / temp_f0_4) * minDist;
    } else {
        Player->SlipFlag |= CARHIT;
        xDist /= sqrtDist;
        zDist /= sqrtDist;
        temp_f0_5 = sqrtf((xVelocity * xVelocity) + (zVelocity * zVelocity));
        if (temp_f0_5 < 0.25f) {
            temp_f0_6 = 1.2f;
            Player->Position[0] = pos[0] - (xDist * minDist * temp_f0_6);
            Player->Position[2] = pos[2] - (zDist * minDist * temp_f0_6);
            Player->Velocity[0] = 0.0f;
            Player->Velocity[2] = 0.0f;
            return COLLISION;
        }
        temp_f2_2 = ((xDist * xVelocity) + (zDist * zVelocity)) / temp_f0_5;
        temp_f2_2 = temp_f0_5 * temp_f2_2 * arg4 * 1.3f;
        Player->Velocity[0] -= xDist * temp_f2_2;
        Player->Velocity[2] -= zDist * temp_f2_2;
        Player->Position[0] += xDist * sp28 * 0.5f;
        Player->Position[2] += zDist * sp28 * 0.5f;
    }
    return COLLISION;
}

bool CollisionMkanban(Player* Player, Object* marioRacewaySign) {
    if (CollisionCylinder(Player, marioRacewaySign->Position, 7.0f, 200.0f, 0.8f) == COLLISION) {
        if ((Player->Flag & IS_PLAYER) != 0) {
            if ((Player->SlipFlag & STAR) != 0) {
                marioRacewaySign->Flag |= FLYINGOBJ;
                NAEnmTrgStart(Player->Position, Player->Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
                NAPlyVoiceStart(Player - PlayerOne,
                              (Player->Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
            } else if ((Player->Flag & IS_GHOST) == 0) {
                NAPlyTrgStart(Player - PlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0x70, 0x1A));
            }
        }
        return true;
    }
    return false;
}

bool CollisionPakkun(Player* Player, PakkunObject* plant) {
    if (CollisionCylinder(Player, plant->Position, plant->Radius, plant->Radius,
                                                 2.5f) == COLLISION) {
        if ((Player->Flag & IS_PLAYER) != 0) {
            if ((Player->SlipFlag & STAR) != 0) {
                plant->Flag |= FLYINGOBJ;
                NAEnmTrgStart(Player->Position, Player->Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0xA2, 0x4A));
                NAPlyVoiceStart(Player - PlayerOne,
                              (Player->Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
            } else if ((Player->Flag & IS_GHOST) == 0) {
                NAPlyTrgStart(Player - PlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0xA0, 0x52));
            }
        }
        return true;
    }
    return false;
}

bool CollisionEgg(Player* Player, struct YoshiValleyEgg* egg) {
    UNUSED f32 pad[5];
    f32 z_dist;
    f32 xz_dist;
    f32 x_dist;
    f32 y_dist;
    f32 totalBox;
    f32 maxDist = 60.0f;
    f32 minDist = 0.0f;

    x_dist = egg->Position[0] - Player->Position[0];
    if ((x_dist < minDist) && (x_dist < -maxDist)) {
        return false;
    }
    if (x_dist > maxDist) {
        return false;
    }

    z_dist = egg->Position[2] - Player->Position[2];
    if ((z_dist < minDist) && (z_dist < -maxDist)) {
        return false;
    }
    if (z_dist > maxDist) {
        return false;
    }

    xz_dist = sqrtf((x_dist * x_dist) + (z_dist * z_dist));
    if (xz_dist > maxDist) {
        return false;
    }
    SetShadowFlag(Player);

    y_dist = Player->Position[1] - egg->Position[1];
    if (y_dist < minDist) {
        return false;
    }

    totalBox = Player->Radius + egg->Radius;
    if (totalBox < xz_dist) {
        return false;
    }

    if ((Player->Flag & IS_PLAYER) != 0) {
        if ((Player->SlipFlag & STAR) != 0) {
            egg->Flag |= FLYINGOBJ;
            egg->PathCenter[1] = 8.0f;
            NAEnmTrgStart(Player->Position, Player->Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
            NAPlyVoiceStart(Player - PlayerOne, (Player->Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
        } else {
            SetBroken(Player, Player - PlayerOne);
            if ((GameMode == TIME_TRIALS) && ((Player->Flag & IS_CPU_PLAYER) == 0)) {
                LostGhost = 1;
            }
        }
    } else {
        SetBroken(Player, Player - PlayerOne);
    }

    return true;
}

bool CollisionTree(Player* Player, Object* actor) {
    f32 x_dist;
    f32 y_dist;
    f32 z_dist;
    f32 sp48;
    f32 sp44;
    f32 var_f16;
    f32 xz_dist;
    UNUSED f32 pad[2];
    f32 temp_f12;
    f32 temp_f0_4;
    Vec3f actorPos;
    f32 temp_f2;

    var_f16 = actor->Fparam;
    x_dist = actor->Position[0] - Player->Position[0];
    if ((x_dist < 0.0f) && (x_dist < -var_f16)) {
        return false;
    }
    if (var_f16 < x_dist) {
        return false;
    }
    z_dist = actor->Position[2] - Player->Position[2];
    if ((z_dist < 0.0f) && (z_dist < -var_f16)) {
        return false;
    }
    if (var_f16 < z_dist) {
        return false;
    }
    y_dist = Player->Position[1] - actor->Position[1];
    if (y_dist < 0.0f) {
        return false;
    }
    if ((f32) actor->Sparam < y_dist) {
        return false;
    }
    xz_dist = sqrtf((x_dist * x_dist) + (z_dist * z_dist));
    if (var_f16 < xz_dist) {
        return false;
    }
    SetShadowFlag(Player);
    var_f16 = Player->Radius + actor->Radius;
    if (var_f16 < xz_dist) {
        return false;
    }
    sp48 = Player->Velocity[0];
    sp44 = Player->Velocity[2];
    if (Player->Flag & IS_PLAYER) {
        if (Player->SlipFlag & STAR) {
            actor->Flag |= FLYINGOBJ;
            NAEnmTrgStart(Player->Position, Player->Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
            NAPlyVoiceStart(Player - PlayerOne, (Player->Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
            return true;
        }
        if (!(Player->Flag & IS_GHOST)) {
            NAPlyTrgStart(Player - PlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0x70, 0x18));
        }
    }
    if (!(Player->SlipFlag & STAR)) {
        Player->SlipFlag |= CARHIT;
    }
    actorPos[0] = actor->Position[0];
    actorPos[1] = actor->Position[1];
    actorPos[2] = actor->Position[2];
    if (((CourseID == COURSE_MARIO_RACEWAY) || (CourseID == COURSE_YOSHI_VALLEY) ||
         (CourseID == COURSE_ROYAL_RACEWAY) || (CourseID == COURSE_LUIGI_RACEWAY)) &&
        (Player->Speed > 1.0f)) {
        KwhitTree(actorPos, 0);
    }
    if (xz_dist < 0.1f) {
        sqrtf((sp48 * sp48) + (sp44 * sp44));
        if (xz_dist) {}
        Player->Velocity[0] = 0;
        Player->Velocity[2] = 0;
        Player->Position[0] = actorPos[0] - (x_dist * var_f16 * 1.2f);
        Player->Position[2] = actorPos[2] - (z_dist * var_f16 * 1.2f);
    } else {
        temp_f0_4 = sqrtf((sp48 * sp48) + (sp44 * sp44));
        x_dist /= xz_dist;
        z_dist /= xz_dist;
        if (temp_f0_4 < 0.25f) {
            Player->Position[0] = actorPos[0] - (x_dist * var_f16 * 1.2f);
            Player->Position[2] = actorPos[2] - (z_dist * var_f16 * 1.2f);
            Player->Velocity[0] = 0;
            Player->Velocity[2] = 0;
            return true;
        }
        temp_f12 = ((x_dist * sp48) + (z_dist * sp44)) / temp_f0_4;
        temp_f12 = temp_f0_4 * temp_f12 * 1.5f;
        Player->Velocity[0] -= x_dist * temp_f12;
        Player->Velocity[2] -= z_dist * temp_f12;
        temp_f2 = xz_dist - var_f16;
        Player->Position[0] += x_dist * temp_f2 * 0.5f;
        Player->Position[2] += z_dist * temp_f2 * 0.5f;
    }
    return true;
}

bool CollisionSphere(Player* arg0, Object* arg1) {
    f32 temp_f0;
    f32 dist;
    f32 yDist;
    f32 zDist;
    f32 xDist;

    temp_f0 = arg0->Radius + arg1->Radius;
    xDist = arg1->Position[0] - arg0->Position[0];
    if (temp_f0 < xDist) {
        return NO_COLLISION;
    }
    if (xDist < -temp_f0) {
        return NO_COLLISION;
    }
    yDist = arg1->Position[1] - arg0->Position[1];
    if (temp_f0 < yDist) {
        return NO_COLLISION;
    }
    if (yDist < -temp_f0) {
        return NO_COLLISION;
    }
    zDist = arg1->Position[2] - arg0->Position[2];
    if (temp_f0 < zDist) {
        return NO_COLLISION;
    }
    if (zDist < -temp_f0) {
        return NO_COLLISION;
    }
    dist = (xDist * xDist) + (yDist * yDist) + (zDist * zDist);
    if (dist < 0.1f) {
        return NO_COLLISION;
    }
    if ((temp_f0 * temp_f0) < dist) {
        return NO_COLLISION;
    }
    return COLLISION;
}

bool CollisionSphere2(Object* arg0, Object* arg1) {
    f32 temp_f0;
    f32 dist;
    f32 dist_y;
    f32 dist_z;
    f32 dist_x;

    temp_f0 = arg0->Radius + arg1->Radius;
    dist_x = arg0->Position[0] - arg1->Position[0];
    if (temp_f0 < dist_x) {
        return NO_COLLISION;
    }
    if (dist_x < -temp_f0) {
        return NO_COLLISION;
    }
    dist_y = arg0->Position[1] - arg1->Position[1];
    if (temp_f0 < dist_y) {
        return NO_COLLISION;
    }
    if (dist_y < -temp_f0) {
        return NO_COLLISION;
    }
    dist_z = arg0->Position[2] - arg1->Position[2];
    if (temp_f0 < dist_z) {
        return NO_COLLISION;
    }
    if (dist_z < -temp_f0) {
        return NO_COLLISION;
    }
    dist = (dist_x * dist_x) + (dist_y * dist_y) + (dist_z * dist_z);
    if (dist < 0.1f) {
        return NO_COLLISION;
    }
    if ((temp_f0 * temp_f0) < dist) {
        return NO_COLLISION;
    }
    return COLLISION;
}

void KillObject(Object* actor) {
    struct ShellActor* shell;
    struct BananaActor* banana;
    struct FakeItemBox* fakeItemBox;
    Player* Player;

    switch (actor->Category) {
        case ACTOR_BANANA:
            banana = (struct BananaActor*) actor;
            switch (banana->Sparam) {
                case FIRST_BANANA_BUNCH_BANANA:
                case BANANA_BUNCH_BANANA:
                    CleanUpBanana(banana);
                    break;
                case HELD_BANANA:
                    Player = &GamePlayers[banana->PlayerId];
                    Player->Weapon &= ~USE_BANANA;
                    /* fallthrough */
                case BANANA_ON_GROUND:
                    banana->Flag = -0x8000;
                    banana->Counter = 0x003C;
                    banana->Sparam = DESTROYED_BANANA;
                    banana->Velocity[1] = 3.0f;
                    break;
                case DROPPED_BANANA:
                case DESTROYED_BANANA:
                default:
                    break;
            }
            break;
        case ACTOR_GREEN_SHELL:
            shell = (struct ShellActor*) actor;
            if (shell->Sparam != GREEN_SHELL_HIT_A_RACER) {
                switch (shell->Sparam) {
                    case MOVING_SHELL:
                        KameHibasiraClear(actor - SimpleObjectArray);
                        /* fallthrough */
                    case HELD_SHELL:
                    case RELEASED_SHELL:
                        shell->Flag = -0x8000;
                        shell->RotAngle = 0;
                        shell->SomeTimer = 0x003C;
                        shell->Sparam = GREEN_SHELL_HIT_A_RACER;
                        shell->Velocity[1] = 3.0f;
                        break;
                    case TRIPLE_GREEN_SHELL:
                        DeleteRollingShell(shell, ACTOR_GREEN_SHELL);
                        break;
                    default:
                        break;
                }
            }
            break;
        case ACTOR_BLUE_SPINY_SHELL:
            shell = (struct ShellActor*) actor;
            if (shell->Sparam != DESTROYED_SHELL) {
                switch (shell->Sparam) {
                    case MOVING_SHELL:
                    case RED_SHELL_LOCK_ON:
                    case TRIPLE_GREEN_SHELL:
                    case GREEN_SHELL_HIT_A_RACER:
                    case BLUE_SHELL_LOCK_ON:
                    case BLUE_SHELL_TARGET_ELIMINATED:
                        NaSceneLevelStop(shell->Position, SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x08));
                        KameHibasiraClear(actor - SimpleObjectArray);
                        /* fallthrough */
                    case HELD_SHELL:
                    case RELEASED_SHELL:
                        shell->Flag = -0x8000;
                        shell->RotAngle = 0;
                        shell->SomeTimer = 0x003C;
                        shell->Sparam = DESTROYED_SHELL;
                        shell->Velocity[1] = 3.0f;
                        break;
                    default:
                        break;
                }
            }
            break;
        case ACTOR_RED_SHELL:
            shell = (struct ShellActor*) actor;
            if (shell->Sparam != DESTROYED_SHELL) {
                switch (shell->Sparam) {
                    case MOVING_SHELL:
                    case RED_SHELL_LOCK_ON:
                    case TRIPLE_GREEN_SHELL:
                    case GREEN_SHELL_HIT_A_RACER:
                    case BLUE_SHELL_LOCK_ON:
                    case BLUE_SHELL_TARGET_ELIMINATED:
                        KameHibasiraClear(actor - SimpleObjectArray);
                        /* fallthrough */
                    case HELD_SHELL:
                    case RELEASED_SHELL:
                        shell->Flag = -0x8000;
                        shell->RotAngle = 0;
                        shell->SomeTimer = 0x003C;
                        shell->Sparam = DESTROYED_SHELL;
                        shell->Velocity[1] = 3.0f;
                        break;
                    case TRIPLE_RED_SHELL:
                        DeleteRollingShell(shell, ACTOR_RED_SHELL);
                        break;
                    default:
                        break;
                }
            }
            break;
        case ACTOR_FAKE_ITEM_BOX:
            fakeItemBox = (struct FakeItemBox*) actor;
            Player = &GamePlayers[(s16) fakeItemBox->PlayerId];
            if (fakeItemBox->Sparam == HELD_FAKE_ITEM_BOX) {
                Player->Weapon &= ~USE_BANANA;
            }
            fakeItemBox->Sparam = DESTROYED_FAKE_ITEM_BOX;
            fakeItemBox->Flag = -0x8000;
            fakeItemBox->SomeTimer = 0;
            break;
    }
}

void CheckCrushSound(Object* arg0, Object* arg1) {
    switch (arg0->Category) {
        case ACTOR_GREEN_SHELL:
            if ((arg0->Sparam == HELD_SHELL) || (arg0->Sparam == TRIPLE_GREEN_SHELL)) {
                arg0->Flag |= SEBANANAOBJ;
                NAEnmTrgStart(arg0->Position, arg0->Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
                return;
            }
            break;
        case ACTOR_RED_SHELL:
            if ((arg0->Sparam == HELD_SHELL) || (arg0->Sparam == TRIPLE_RED_SHELL)) {
                arg0->Flag |= SEBANANAOBJ;
                NAEnmTrgStart(arg0->Position, arg0->Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
                return;
            }
            break;
        case ACTOR_BLUE_SPINY_SHELL:
            if (arg0->Sparam == HELD_SHELL) {
                arg0->Flag |= SEBANANAOBJ;
                NAEnmTrgStart(arg0->Position, arg0->Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
                return;
            }
            break;
        case ACTOR_FAKE_ITEM_BOX:
            if (arg0->Sparam == HELD_FAKE_ITEM_BOX) {
                arg0->Flag |= SEBANANAOBJ;
                NAEnmTrgStart(arg0->Position, arg0->Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
                return;
            }
            break;
    }

    switch (arg1->Category) {
        case ACTOR_GREEN_SHELL:
            if ((arg1->Sparam == HELD_SHELL) || (arg1->Sparam == TRIPLE_GREEN_SHELL)) {
                arg1->Flag |= SEBANANAOBJ;
                NAEnmTrgStart(arg1->Position, arg1->Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
                return;
            }
            break;
        case ACTOR_RED_SHELL:
            if ((arg1->Sparam == HELD_SHELL) || (arg1->Sparam == TRIPLE_RED_SHELL)) {
                arg1->Flag |= SEBANANAOBJ;
                NAEnmTrgStart(arg1->Position, arg1->Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
                return;
            }
            break;
        case ACTOR_BLUE_SPINY_SHELL:
            if (arg1->Sparam == HELD_SHELL) {
                arg1->Flag |= SEBANANAOBJ;
                NAEnmTrgStart(arg1->Position, arg1->Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
                return;
            }
            break;
        case ACTOR_FAKE_ITEM_BOX:
            if (arg1->Sparam == HELD_FAKE_ITEM_BOX) {
                arg1->Flag |= SEBANANAOBJ;
                NAEnmTrgStart(arg1->Position, arg1->Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
                return;
            }
            break;
    }

    arg0->Flag |= SEKAMEHITOBJ;
    NAEnmTrgStart(arg0->Position, arg0->Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
}

void CollisionCheckRoutin2(Object* actor1, Object* actor2) {
    if (CollisionSphere2(actor1, actor2) == COLLISION) {
        if ((actor1->Category == ACTOR_BLUE_SPINY_SHELL) && (actor2->Category == ACTOR_BLUE_SPINY_SHELL)) {
            KillObject(actor1);
            KillObject(actor2);
            actor1->Flag |= SEKAMEHITOBJ;
            NAEnmTrgStart(actor1->Position, actor1->Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
            return;
        }
        if (actor1->Category == ACTOR_BLUE_SPINY_SHELL) {
            if (actor1->Sparam == HELD_SHELL) {
                KillObject(actor1);
            }
        } else {
            KillObject(actor1); // automatically destroy if it's something different of a blueshell
        }
        if (actor2->Category == ACTOR_BLUE_SPINY_SHELL) {
            if (actor2->Sparam == HELD_SHELL) {
                KillObject(actor2);
            }
        } else {
            KillObject(actor2); // automatically destroy if it's something different of a blueshell
        }
        CheckCrushSound(actor1, actor2);
    }
}

void CollisionCheckRoutin(Player* player, Object* actor) {
    UNUSED s32 pad;
    s16 temp_lo;
    UNUSED s32 Pad2[2];
    s16 temp_v1;
    Player* Owner;
    f32 temp_f0;
    f32 temp_f2;

    temp_lo = player - PlayerOne;
    switch (actor->Category) {
        case ACTOR_YOSHI_EGG:
            if (!(player->SlipFlag & TERESA) && !(player->Flag & IS_GHOST)) {
                CollisionEgg(player, (struct YoshiValleyEgg*) actor);
            }
            break;
        case ACTOR_BANANA:
            if (player->SlipFlag &
                (TERESA | WHEELSPIN | SPIN_L | SPIN_R)) {
                break;
            }
            if (player->Weapon & HIT_BANANA) {
                break;
            }
            temp_v1 = actor->Angle[0];
            if (((temp_lo == temp_v1) && (actor->Flag & MYOBJ)) ||
                (CollisionSphere(player, actor) != COLLISION)) {
                break;
            }
            player->Weapon |= HIT_BANANA;
            Owner = &GamePlayers[temp_v1];
            if (Owner->Flag & IS_PLAYER) {
                if (actor->Flag & 0xF) {
                    if (temp_lo != temp_v1) {
                        NAPlyVoiceStart(temp_v1, (Owner->Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x06));
                    }
                } else {
                    temp_f0 = actor->Position[0] - Owner->Position[0];
                    temp_f2 = actor->Position[2] - Owner->Position[2];
                    if ((((temp_f0 * temp_f0) + (temp_f2 * temp_f2)) < 360000.0f) && (temp_lo != temp_v1)) {
                        NAPlyVoiceStart(temp_v1, (Owner->Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x06));
                    }
                }
            }
            KillObject(actor);
            break;
        case ACTOR_GREEN_SHELL:
            if (player->SlipFlag & (TERESA | ROLLOVER)) {
                break;
            }
            if (player->Weapon & HIT_GREENSHELL) {
                break;
            }
            temp_v1 = actor->Angle[2];
            if (((temp_lo == temp_v1) && (actor->Flag & MYOBJ)) ||
                (CollisionSphere(player, actor) != COLLISION)) {
                break;
            }
            player->Weapon |= HIT_GREENSHELL;
            NAEnmTrgStart(player->Position, player->Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
            Owner = &GamePlayers[temp_v1];
            if ((Owner->Flag & IS_PLAYER) && (temp_lo != temp_v1)) {
                NAPlyVoiceStart(temp_v1, (Owner->Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x06));
            }
            KillObject(actor);
            break;
        case ACTOR_BLUE_SPINY_SHELL:
            if (player->Weapon & HIT_REDSHELL) {
                break;
            }
            temp_v1 = actor->Angle[2];
            if (((temp_lo == temp_v1) && (actor->Flag & MYOBJ)) ||
                (CollisionSphere(player, actor) != COLLISION)) {
                break;
            }
            if (!(player->SlipFlag & TERESA)) {
                player->Weapon |= HIT_REDSHELL;
                NAEnmTrgStart(player->Position, player->Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
            }
            Owner = &GamePlayers[temp_v1];
            if ((Owner->Flag & IS_PLAYER) && (temp_lo != temp_v1)) {
                NAPlyVoiceStart(temp_v1, (Owner->Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x06));
            }
            if (temp_lo == actor->Counter) {
                KillObject(actor);
            }
            break;
        case ACTOR_RED_SHELL:
            temp_v1 = actor->Angle[2];
            if (player->SlipFlag & EXPLODE) {
                break;
            }
            if (player->Weapon & HIT_REDSHELL) {
                break;
            }
            temp_v1 = actor->Angle[2];
            if (((temp_lo == temp_v1) && (actor->Flag & MYOBJ)) ||
                (CollisionSphere(player, actor) != COLLISION)) {
                break;
            }
            if (!(player->SlipFlag & TERESA)) {
                player->Weapon |= HIT_REDSHELL;
                NAEnmTrgStart(player->Position, player->Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
            }
            Owner = &GamePlayers[temp_v1];
            if ((Owner->Flag & IS_PLAYER) && (temp_lo != temp_v1)) {
                NAPlyVoiceStart(temp_v1, (Owner->Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x06));
            }
            KillObject(actor);
            break;
        case ACTOR_PIRANHA_PLANT:
            if (!(player->SlipFlag & TERESA)) {
                CollisionPakkun(player, (PakkunObject*) actor);
            }
            break;
        case ACTOR_MARIO_SIGN:
            if (!(player->SlipFlag & TERESA)) {
                CollisionMkanban(player, actor);
            }
            break;
        case ACTOR_TREE_MARIO_RACEWAY:
        case ACTOR_TREE_YOSHI_VALLEY:
        case ACTOR_TREE_ROYAL_RACEWAY:
        case ACTOR_TREE_MOO_MOO_FARM:
        case ACTOR_PALM_TREE:
        case 26:
        case ACTOR_TREE_BOWSERS_CASTLE:
        case ACTOR_TREE_FRAPPE_SNOWLAND:
        case ACTOR_CACTUS1_KALAMARI_DESERT:
        case ACTOR_CACTUS2_KALAMARI_DESERT:
        case ACTOR_CACTUS3_KALAMARI_DESERT:
        case ACTOR_BUSH_BOWSERS_CASTLE:
            if (!(player->SlipFlag & TERESA)) {
                CollisionTree(player, actor);
            }
            break;
        case ACTOR_FALLING_ROCK:
            if (!(player->SlipFlag & TERESA) && !(player->Flag & IS_GHOST)) {
                if (CollisionSphere(player, actor) == COLLISION) {
                    NAEnmTrgStart(actor->Position, actor->Velocity, SOUND_ACTION_EXPLOSION);
                    if ((GameMode == TIME_TRIALS) && !(player->Flag & IS_CPU_PLAYER)) {
                        LostGhost = 1;
                    }
                    if (player->SlipFlag & STAR) {
                        actor->Velocity[1] = 10.0f;
                    } else {
                        SetBroken(player, player - PlayerOne);
                    }
                }
            }
            break;
        case ACTOR_FAKE_ITEM_BOX:
            temp_v1 = actor->Velocity[0];
            if (player->SlipFlag & TERESA) {
                break;
            }
            temp_v1 = actor->Velocity[0];
            if (((temp_lo == temp_v1) && (actor->Flag & MYOBJ)) ||
                (CollisionSphere(player, actor) != COLLISION)) {
                break;
            }
            player->Weapon |= HIT_BOMB;
            Owner = &GamePlayers[temp_v1];
            if (Owner->Flag & IS_PLAYER) {
                if (actor->Flag & 0xF) {
                    if (temp_lo != temp_v1) {
                        NAPlyVoiceStart(temp_v1, (Owner->Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x06));
                    }
                } else {
                    temp_f0 = actor->Position[0] - Owner->Position[0];
                    temp_f2 = actor->Position[2] - Owner->Position[2];
                    if ((((temp_f0 * temp_f0) + (temp_f2 * temp_f2)) < 360000.0f) && (temp_lo != temp_v1)) {
                        NAPlyVoiceStart(temp_v1, (Owner->Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x06));
                    }
                }
                if (actor->Sparam == 0) {
                    Owner->Weapon &= ~USE_BANANA;
                }
            }
            actor->Sparam = 2;
            actor->Flag = -0x8000;
            actor->Counter = 0;
            break;
        case ACTOR_HOT_AIR_BALLOON_ITEM_BOX:
            if (CollisionSphere(player, actor) == COLLISION) {
                actor->Sparam = 3;
                actor->Flag = -0x8000;
                actor->Counter = 0;
                if (player->Flag & IS_PLAYER) {
                    RouletteStart(player - PlayerOne, 7);
                }
            } else if (actor->Sparam == 0) {
                actor->Sparam = 1;
                actor->Flag = -0x8000;
            }
            break;
        case ACTOR_ITEM_BOX:
            //Handled under CollideObjectDefault
            /*
            if (CollisionSphere(player, actor) == COLLISION) {
                actor->Sparam = 3;
                actor->Flag = -0x8000;
                actor->Counter = 0;
                if (player->Flag & IS_PLAYER) {
                    RouletteStart(player - PlayerOne, 0);
                }
            } else if (actor->Sparam == 0) {
                actor->Sparam = 1;
                actor->Flag = -0x8000;
            }
            */
            CollideObjectDefault(player, actor);
            break;
        default:
            CollideObjectDefault(player, actor);
            break;
    }
}

void CollisionObjectToKart(void) {
    Object* temp_a1;
    s32 i, j;
    Player* phi_s1;

    for (i = 0; i < NUM_PLAYERS; i++) {
        phi_s1 = &GamePlayers[i];

        if (((phi_s1->Flag & EXISTS) != 0) && ((phi_s1->SlipFlag & BROKEN) == 0)) {
            ResetShadowFlag(phi_s1);
            for (j = 0; j < ACTOR_LIST_SIZE; j++) {
                temp_a1 = &SimpleObjectArray[j];

                if ((phi_s1->SlipFlag & BROKEN) == 0) {
                    // temp_v0 = temp_a1->Unk2;
                    if (((temp_a1->Flag & EXISTOBJ) != 0) && ((temp_a1->Flag & HITOBJ) != 0)) {
                        CollisionCheckRoutin(phi_s1, temp_a1);
                    }
                }
            }
        }
    }
}

// It's look like to check collision between Item and other different Item
void CollisionObjectToObject(void) {
    Object* actor1;
    Object* actor2;
    s32 i, j;
    UNUSED s32 pad;

    for (i = StaticObjectCount; i < (ACTOR_LIST_SIZE - 1); i++) {
        actor1 = &SimpleObjectArray[i];

        if ((actor1->Flag & EXISTOBJ) == 0) {
            continue;
        }
        if ((actor1->Flag & HITOBJ) == 0) {
            continue;
        }

        switch (actor1->Category) {
            case ACTOR_BANANA:
            case ACTOR_GREEN_SHELL:
            case ACTOR_RED_SHELL:
            case ACTOR_BLUE_SPINY_SHELL:
            case ACTOR_FAKE_ITEM_BOX:

                for (j = i + 1; j < ACTOR_LIST_SIZE; j++) {
                    actor2 = &SimpleObjectArray[j];

                    if ((actor1->Flag & EXISTOBJ) == 0) {
                        continue;
                    }
                    if ((actor1->Flag & HITOBJ) == 0) {
                        continue;
                    }

                    if ((actor2->Flag & EXISTOBJ) == 0) {
                        continue;
                    }
                    if ((actor2->Flag & HITOBJ) == 0) {
                        continue;
                    }

                    switch (actor2->Category) {
                        case ACTOR_BANANA:
                            if (actor1->Category == ACTOR_BANANA) {
                                continue;
                            }
                            CollisionCheckRoutin2(actor1, actor2);
                            break;
                        case ACTOR_GREEN_SHELL:
                            if (actor1->Category == ACTOR_GREEN_SHELL) {
                                if (actor1->Angle[2] == actor2->Angle[2]) {
                                    continue;
                                }
                            }
                            CollisionCheckRoutin2(actor1, actor2);
                            break;
                        case ACTOR_RED_SHELL:
                            if (actor1->Category == ACTOR_RED_SHELL) {
                                if (actor1->Angle[2] == actor2->Angle[2]) {
                                    continue;
                                }
                            }
                            CollisionCheckRoutin2(actor1, actor2);
                            break;
                        case ACTOR_BLUE_SPINY_SHELL:
                        case ACTOR_FAKE_ITEM_BOX:
                            CollisionCheckRoutin2(actor1, actor2);
                            break;
                    }
                }

                break;
        }
    }
}

void ReleaseFakeItembox(struct FakeItemBox* fake_item_box) {
    if ((u32) (fake_item_box - (struct FakeItemBox*) SimpleObjectArray) <= (u32) ACTOR_LIST_SIZE) {
        if (((fake_item_box->Flag & EXISTOBJ) != 0) && (fake_item_box->Category == ACTOR_FAKE_ITEM_BOX)) {
            fake_item_box->Sparam = FAKE_ITEM_BOX_ON_GROUND;
            fake_item_box->TargetY = GetHight(&fake_item_box->Bump, fake_item_box->Position) + 8.66f;
            fake_item_box->SomeTimer = 100;
        }
    }
}

#include "actors/fake_item_box/update.inc.c"

void InitialSpecialItembox(f32 x, f32 y, f32 z) {
    Vec3f pos;
    Vec3f Velocity;
    Vec3s rot;
    s16 id;

    if (GameMode == TIME_TRIALS) {
        return;
    }

    SetSVector(rot, 0, 0, 0);
    Vec3FSet(Velocity, 0, 0, 0);
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    id = AddObjectBuffer(pos, rot, Velocity, ACTOR_HOT_AIR_BALLOON_ITEM_BOX);
    ActorHotAirBalloonItemBox = &SimpleObjectArray[id];
}

#include "actors/item_box/update.inc.c"

#include "actors/fake_item_box/render.inc.c"

#include "actors/item_box/render.inc.c"

#include "actors/wario_sign/render.inc.c"

#include "actors/yoshi_egg/render.inc.c"

#include "actors/mario_sign/render.inc.c"

#include "actors/railroad_crossing/render.inc.c"

#include "actors/palm_tree/render.inc.c"

void DisplayObject2(Screen* arg0) {
    Camera* Camera = arg0->Camera;
    Object* actor;
    s32 i;
    ItemboxFlag = 0;

    for (i = 0; i < ACTOR_LIST_SIZE; i++) {
        actor = &SimpleObjectArray[i];

        if (actor->Flag == 0) {
            continue;
        }

        switch (actor->Category) {
            case ACTOR_FAKE_ITEM_BOX:
                DisplayFakeItembox(Camera, (struct FakeItemBox*) actor);
                break;
            case ACTOR_ITEM_BOX:
                DisplayItembox(Camera, (struct ItemBox*) actor);
                break;
            case ACTOR_HOT_AIR_BALLOON_ITEM_BOX:
                DisplayItembox(Camera, (struct ItemBox*) actor);
                break;
        }
    }
}

void DisplayCourseObject(Screen* arg0) {
    Camera* Camera = arg0->Camera;
    u16 pathCounter = arg0->CameraPoint;
    UNUSED s32 pad[12];
    s32 i;

    Object* actor;
    UNUSED Vec3f sp4C = { 0.0f, 5.0f, 10.0f };
    f32 sp48 = SinT(Camera->CameraDirection[1] - DEGREES(180)); // Unk26;
    f32 temp_f0 = CosT(Camera->CameraDirection[1] - DEGREES(180));

    Affine[0][0] = temp_f0;
    Affine[0][2] = -sp48;
    Affine[2][2] = temp_f0;
    Affine[1][0] = 0.0f;
    Affine[0][1] = 0.0f;
    Affine[2][1] = 0.0f;
    Affine[1][2] = 0.0f;
    Affine[0][3] = 0.0f;
    Affine[1][3] = 0.0f;
    Affine[2][3] = 0.0f; // 2c
    Affine[2][0] = sp48;
    Affine[1][1] = 1.0f;
    Affine[3][3] = 1.0f; // unk3c

    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
    gSPSetLights1(DisplayListHead++, D_800DC610[1]);
    gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);

    if (GameMode != BATTLE) {
        DisplayFlagGateCheck(Camera);
    }
    TreeFlag = 0;

    for (i = 0; i < ACTOR_LIST_SIZE; i++) {
        actor = &SimpleObjectArray[i];

        if (actor->Flag == 0) {
            continue;
        }
        switch (actor->Category) {
            case ACTOR_TREE_MARIO_RACEWAY:
                DisplayTree1Bypass(Camera, Affine, actor);
                break;
            case ACTOR_TREE_YOSHI_VALLEY:
                DisplayCourseTree2(Camera, Affine, actor);
                break;
            case ACTOR_TREE_ROYAL_RACEWAY:
                DisplayTree3(Camera, Affine, actor);
                break;
            case ACTOR_TREE_MOO_MOO_FARM:
                DisplayTree4(Camera, Affine, actor);
                break;
            case ACTOR_UNKNOWN_0x1A:
                DisplayTree5(Camera, Affine, actor);
                break;
            case ACTOR_TREE_BOWSERS_CASTLE:
                DisplayTree7(Camera, Affine, actor);
                break;
            case ACTOR_BUSH_BOWSERS_CASTLE:
                DisplayTree8(Camera, Affine, actor);
                break;
            case ACTOR_TREE_FRAPPE_SNOWLAND:
                DisplaySnowtree(Camera, Affine, actor);
                break;
            case ACTOR_CACTUS1_KALAMARI_DESERT:
                DisplaySaboten1(Camera, Affine, actor);
                break;
            case ACTOR_CACTUS2_KALAMARI_DESERT:
                DisplaySaboten2(Camera, Affine, actor);
                break;
            case ACTOR_CACTUS3_KALAMARI_DESERT:
                DisplaySaboten3(Camera, Affine, actor);
                break;
            case ACTOR_FALLING_ROCK:
                DisplayIwa(Camera, (struct FallingRock*) actor);
                break;
            case ACTOR_KIWANO_FRUIT:
                DisplayNuts(Camera, Affine, actor);
                break;
            case ACTOR_BANANA:
                DisplayBanana(Camera, Affine, (struct BananaActor*) actor);
                break;
            case ACTOR_GREEN_SHELL:
                DisplayGshell(Camera, Affine, (struct ShellActor*) actor);
                break;
            case ACTOR_RED_SHELL:
                DisplayRshell(Camera, Affine, (struct ShellActor*) actor);
                break;
            case ACTOR_BLUE_SPINY_SHELL:
                DisplayTshell(Camera, Affine, (struct ShellActor*) actor);
                break;
            case ACTOR_PIRANHA_PLANT:
                DisplayPiranhaBypass(Camera, Affine, actor);
                break;
            case ACTOR_TRAIN_ENGINE:
                DisplaySl1(Camera, (struct TrainCar*) actor);
                break;
            case ACTOR_TRAIN_TENDER:
                DisplaySl2(Camera, (struct TrainCar*) actor);
                break;
            case ACTOR_TRAIN_PASSENGER_CAR:
                DisplaySl3(Camera, (struct TrainCar*) actor);
                break;
            case ACTOR_COW:
                DisplayCow(Camera, Affine, actor);
                break;
            case ACTOR_UNKNOWN_0x14:
                DisplayPylon(Camera, Affine, actor);
                break;
            case ACTOR_MARIO_SIGN:
                DisplayMkanban(Camera, Affine, actor);
                break;
            case ACTOR_WARIO_SIGN:
                DisplayWkanban(Camera, actor);
                break;
            case ACTOR_PALM_TREE:
                DisplayPalmtree(Camera, Affine, (struct PalmTree*) actor);
                break;
            case ACTOR_PADDLE_BOAT:
                DisplayShip(Camera, (struct PaddleWheelBoat*) actor, Affine, pathCounter);
                break;
            case ACTOR_BOX_TRUCK:
                DisplayTruck1(Camera, actor);
                break;
            case ACTOR_SCHOOL_BUS:
                DisplayBus(Camera, actor);
                break;
            case ACTOR_TANKER_TRUCK:
                DisplayTanklorry(Camera, actor);
                break;
            case ACTOR_CAR:
                DisplayRv(Camera, actor);
                break;
            case ACTOR_RAILROAD_CROSSING:
                DisplayFumikiri(Camera, (struct RailroadCrossing*) actor);
                break;
            case ACTOR_YOSHI_EGG:
                DisplayEgg(Camera, Affine, (struct YoshiValleyEgg*) actor, pathCounter);
                break;
            default:
                DisplayObjectDefault(Camera, actor);
                break;
        }
    }
    switch (CourseID) {
        case COURSE_MOO_MOO_FARM:
            DisplayFarmCow(Camera, Affine, actor);
            break;
        case COURSE_DK_JUNGLE:
            DisplayJungleTree(Camera, Affine, actor);
            break;
    }
    DrawPerScreenDefault(Camera);
}

void ObjectStrategy(void) {
    Object* actor;
    s32 i;
    for (i = 0; i < ACTOR_LIST_SIZE; i++) {

        actor = &SimpleObjectArray[i];
        if (actor->Flag == 0) {
            continue;
        }

        switch (actor->Category) {
            case ACTOR_FALLING_ROCK:
                UpdateActorFallingRocks((struct FallingRock*) actor);
                break;
            case ACTOR_GREEN_SHELL:
                MoveGshell((struct ShellActor*) actor);
                break;
            case ACTOR_RED_SHELL:
                MoveRshell((struct ShellActor*) actor);
                break;
            case ACTOR_BLUE_SPINY_SHELL:
                MoveRshell((struct ShellActor*) actor);
                break;
            case ACTOR_KIWANO_FRUIT:
                NutsStrategy((struct KiwanoFruit*) actor);
                break;
            case ACTOR_BANANA:
                MoveBanana((struct BananaActor*) actor);
                break;
            case ACTOR_PADDLE_BOAT:
                ShipStrategy((struct PaddleWheelBoat*) actor);
                break;
            case ACTOR_TRAIN_ENGINE:
                Sl1Strategy((struct TrainCar*) actor);
                break;
            case ACTOR_TRAIN_TENDER:
                Sl2Strategy((struct TrainCar*) actor);
                break;
            case ACTOR_TRAIN_PASSENGER_CAR:
                Sl3Strategy((struct TrainCar*) actor);
                break;
            case ACTOR_ITEM_BOX:
                MoveItembox((struct ItemBox*) actor);
                break;
            case ACTOR_HOT_AIR_BALLOON_ITEM_BOX:
                MoveSpecialItembox((struct ItemBox*) actor);
                break;
            case ACTOR_FAKE_ITEM_BOX:
                MoveFakeItembox((struct FakeItemBox*) actor);
                break;
            case ACTOR_PIRANHA_PLANT:
                PakkunStrategyOverride(actor);
                break;
            case ACTOR_BANANA_BUNCH:
                SuperBananaStrategy((struct BananaBunchParent*) actor);
                break;
            case ACTOR_TRIPLE_GREEN_SHELL:
                SuperShellStrategy((TripleShellParent*) actor, ACTOR_GREEN_SHELL);
                break;
            case ACTOR_TRIPLE_RED_SHELL:
                SuperShellStrategy((TripleShellParent*) actor, ACTOR_RED_SHELL);
                break;
            case ACTOR_MARIO_SIGN:
                MkanbanStrategy(actor);
                break;
            case ACTOR_WARIO_SIGN:
                WkanbanStrategy(actor);
                break;
            case ACTOR_RAILROAD_CROSSING:
                FumikiriStrategy((struct RailroadCrossing*) actor);
                break;
            case ACTOR_TREE_MARIO_RACEWAY:
            case ACTOR_TREE_YOSHI_VALLEY:
            case ACTOR_TREE_ROYAL_RACEWAY:
            case ACTOR_TREE_MOO_MOO_FARM:
            case ACTOR_PALM_TREE:
            case ACTOR_UNKNOWN_0x1A: // A plant?
            case ACTOR_UNKNOWN_0x1B:
            case ACTOR_TREE_BOWSERS_CASTLE:
            case ACTOR_TREE_FRAPPE_SNOWLAND:
            case ACTOR_CACTUS1_KALAMARI_DESERT:
            case ACTOR_CACTUS2_KALAMARI_DESERT:
            case ACTOR_CACTUS3_KALAMARI_DESERT:
            case ACTOR_BUSH_BOWSERS_CASTLE:
                TreeStrategy(actor);
                break;
            case ACTOR_YOSHI_EGG:
                EggStrategy((struct YoshiValleyEgg*) actor);
                break;
        }
    }
    CollisionObjectToObject();
    ItemCheck();
}

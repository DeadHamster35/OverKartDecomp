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

// Appears to be textures
// or tluts
u8* D_802BA050;
u8* D_802BA054;
u8* D_802BA058;

Object* gActorHotAirBalloonItemBox;
s8 gTLUTRedShell[512]; // tlut 256
u16 D_802BA260;

/**
 * Once the amount of spawned player red and green shell count has reached 21 or higher
 * the game will cleanup any dead red or green shells by deleting their actors.
 */
void kill_kame(struct ShellActor* shell) {
    s32 actorIndex;
    struct ShellActor* compare;

    // try finding the dead green shell
    for (actorIndex = g_StaticObjectCount; actorIndex < ACTOR_LIST_SIZE; actorIndex++) {
        compare = (struct ShellActor*) &g_SimpleObjectArray[actorIndex];
        if ((shell != compare) && !(compare->flag & ACTOR_IS_NOT_EXPIRED) && (compare->category == ACTOR_GREEN_SHELL)) {
            if (compare->sparam == MOVING_SHELL) {
                kame_hibasira_clear(actorIndex);
            }
            kame_counter--;
            deleteObjectBuffer((Object*) compare);
            return;
        }
    }

    // try finding the dead red shell
    for (actorIndex = g_StaticObjectCount; actorIndex < ACTOR_LIST_SIZE; actorIndex++) {
        compare = (struct ShellActor*) &g_SimpleObjectArray[actorIndex];
        if ((shell != compare) && !(compare->flag & ACTOR_IS_NOT_EXPIRED) && (compare->category == ACTOR_RED_SHELL)) {
            switch (compare->sparam) {
                case MOVING_SHELL:
                case RED_SHELL_LOCK_ON:
                case TRIPLE_GREEN_SHELL:
                case GREEN_SHELL_HIT_A_RACER:
                case BLUE_SHELL_LOCK_ON:
                case BLUE_SHELL_TARGET_ELIMINATED:
                    kame_hibasira_clear(actorIndex);
                case DESTROYED_SHELL:
                    kame_counter -= 1;
                    deleteObjectBuffer((Object*) compare);
                    return;
                default:
                    break;
            }
        }
    }

    // try finding the green shell
    for (actorIndex = g_StaticObjectCount; actorIndex < ACTOR_LIST_SIZE; actorIndex++) {
        compare = (struct ShellActor*) &g_SimpleObjectArray[actorIndex];
        if ((shell != compare) && (compare->category == ACTOR_GREEN_SHELL)) {
            switch (compare->sparam) {
                case MOVING_SHELL:
                    kame_hibasira_clear(actorIndex);
                case DESTROYED_SHELL:
                    kame_counter -= 1;
                    deleteObjectBuffer((Object*) compare);
                    return;
            }
        }
    }

    // try finding the red or blue shell
    for (actorIndex = g_StaticObjectCount; actorIndex < ACTOR_LIST_SIZE; actorIndex++) {
        compare = (struct ShellActor*) &g_SimpleObjectArray[actorIndex];
        if ((shell != compare) && (compare->category == ACTOR_RED_SHELL)) {
            switch (compare->sparam) {
                case MOVING_SHELL:
                case RED_SHELL_LOCK_ON:
                case TRIPLE_GREEN_SHELL:
                case GREEN_SHELL_HIT_A_RACER:
                case BLUE_SHELL_LOCK_ON:
                case BLUE_SHELL_TARGET_ELIMINATED:
                    kame_hibasira_clear(actorIndex);
                case DESTROYED_SHELL:
                    kame_counter -= 1;
                    deleteObjectBuffer((Object*) compare);
                    return;
            }
        }
    }
}

// Sets introductory values for a new actor (ex. Banana).
void set_object_buffer(Object* actor, Vec3f startingPos, Vec3s startingRot, Vec3f startingVelocity, s16 actorType) {
    CopyVector(actor->position, startingPos);
    CopySVector(actor->angle, startingRot);
    CopyVector(actor->velocity, startingVelocity);
    actor->category = actorType;
    actor->flag = -0x8000;
    actor->counter = 0;
    actor->sparam = 0;
    actor->fparam = 0.0f;
    actor->radius = 0.0f;
    InitialBump(&actor->bump);
    switch (actorType) {
        case ACTOR_BOX_TRUCK:
            if ((s32) D_802BA260 >= 3) {
                D_802BA260 = 0;
            }
            actor->sparam = (s16) D_802BA260;
            D_802BA260 += 1;
            break;
        case ACTOR_YOSHI_EGG:
            actor->flag |= HITOBJ;
            actor->fparam = 70.0f;
            actor->radius = 20.0f;
            actor->velocity[0] = actor->position[0];
            actor->velocity[1] = actor->position[1];
            actor->velocity[2] = actor->position[2] + 70.0f;
            break;
        case ACTOR_KIWANO_FRUIT:
            actor->sparam = 0;
            actor->angle[0] = 0;
            actor->angle[1] = 0;
            actor->angle[2] = 0;
            actor->radius = 2.0f;
            break;
        case ACTOR_FALLING_ROCK:
            actor->flag |= HITOBJ;
            actor->radius = 10.0f;
            break;
        case ACTOR_TRAIN_ENGINE:
            actor->fparam = 10.0f;
            break;
        case ACTOR_BANANA:
            actor->flag = actor->flag | HITOBJ | MYOBJ;
            actor->radius = 2.0f;
            break;
        case ACTOR_GREEN_SHELL:
            kame_counter += 1;
            actor->counter = 0;
            actor->radius = 4.0f;
            actor->flag = actor->flag | HITOBJ | MOVEOBJ | MYOBJ;
            if ((s32) kame_counter >= 0x15) {
                kill_kame((struct ShellActor*) actor);
            }
            break;
        case ACTOR_RED_SHELL:
            kame_counter += 1;
            actor->counter = 0;
            actor->radius = 4.0f;
            actor->flag = actor->flag | HITOBJ | MOVEOBJ | MYOBJ;
            if ((s32) kame_counter >= 0x15) {
                kill_kame((struct ShellActor*) actor);
            }
            break;
        case ACTOR_TREE_MARIO_RACEWAY:
            kame_counter += 1;
            actor->flag |= HITOBJ;
            actor->sparam = 0x0043;
            actor->radius = 3.0f;
            actor->fparam = 20.0f;
            break;
        case ACTOR_TREE_YOSHI_VALLEY:
            actor->flag |= HITOBJ;
            actor->sparam = 0x0043;
            actor->radius = 3.0f;
            actor->fparam = 23.0f;
            break;
        case ACTOR_TREE_ROYAL_RACEWAY:
            actor->flag |= HITOBJ;
            actor->sparam = 0x0043;
            actor->radius = 3.0f;
            actor->fparam = 17.0f;
            break;
        case ACTOR_TREE_MOO_MOO_FARM:
            actor->sparam = 0x0043;
            actor->flag = -0x8000;
            actor->radius = 3.0f;
            actor->fparam = 17.0f;
            break;
        case 26:
            actor->flag |= HITOBJ;
            actor->sparam = 0x0043;
            actor->radius = 3.0f;
            actor->fparam = 17.0f;
            break;
        case 28:
            actor->sparam = 0x0043;
            actor->flag = -0x8000;
            actor->radius = 3.0f;
            actor->fparam = 17.0f;
            break;
        case 33:
            actor->flag |= HITOBJ;
            actor->sparam = 0x0043;
            actor->radius = 3.0f;
            actor->fparam = 17.0f;
            break;
        case 29:
            actor->flag |= HITOBJ;
            actor->sparam = 0x0043;
            actor->radius = 3.0f;
            actor->fparam = 17.0f;
            break;
        case 30:
            actor->flag |= HITOBJ;
            actor->sparam = 0x0019;
            actor->radius = 3.0f;
            actor->fparam = 7.0f;
            break;
        case 31:
            actor->flag |= HITOBJ;
            actor->sparam = 0x0019;
            actor->radius = 3.0f;
            actor->fparam = 7.0f;
            break;
        case 32:
            actor->flag |= HITOBJ;
            actor->sparam = 0x0019;
            actor->radius = 3.0f;
            actor->fparam = 7.0f;
            break;
        case ACTOR_PALM_TREE:
            actor->flag |= HITOBJ;
            actor->sparam = 0x003C;
            actor->radius = 3.0f;
            actor->fparam = 13.0f;
            break;
        case ACTOR_FAKE_ITEM_BOX:
            actor->flag = actor->flag | HITOBJ | MYOBJ;
            actor->fparam = 0.35f;
            actor->radius = 1.925f;
            CheckBump(&actor->bump, 1.925f, actor->position[0], actor->position[1], actor->position[2]);
            break;
        case ACTOR_HOT_AIR_BALLOON_ITEM_BOX:
            actor->flag |= HITOBJ;
            actor->counter = 0;
            actor->sparam = 5;
            actor->radius = 5.5f;
            break;
        case ACTOR_ITEM_BOX:
            actor->flag |= HITOBJ;
            actor->counter = 0;
            actor->sparam = 0;
            actor->radius = 5.5f;
            break;
        case ACTOR_PIRANHA_PLANT:
            actor->flag |= HITOBJ;
            actor->sparam = 0x001E;
            actor->radius = 5.0f;
            break;
        default:
            break;
    }
}

void out_of_screen(Camera* arg0, Object* arg1) {
    switch (arg0 - camera1) {
        case PLAYER_ONE:
            arg1->flag &= ~(1 << PLAYER_ONE);
            break;
        case PLAYER_TWO:
            arg1->flag &= ~(1 << PLAYER_TWO);
            break;
        case PLAYER_THREE:
            arg1->flag &= ~(1 << PLAYER_THREE);
            break;
        case PLAYER_FOUR:
            arg1->flag &= ~(1 << PLAYER_FOUR);
            break;
    }
}

void on_screen(Camera* arg0, Object* arg1) {
    switch (arg0 - camera1) {
        case PLAYER_ONE:
            arg1->flag |= 1 << PLAYER_ONE;
            break;
        case PLAYER_TWO:
            arg1->flag |= 1 << PLAYER_TWO;
            break;
        case PLAYER_THREE:
            arg1->flag |= 1 << PLAYER_THREE;
            break;
        case PLAYER_FOUR:
            arg1->flag |= 1 << PLAYER_FOUR;
            break;
    }
}

void DisplayFlagGate(Camera* arg0) {
    Mat4 sp38;
    s16 temp = g_goalBannerPos[2];
    s32 maxObjectsReached;

    if (gGamestate == CREDITS_SEQUENCE) {
        return;
    }

    CreateTransAffineMtx(sp38, g_goalBannerPos);

    maxObjectsReached = SetMatrix(sp38, 0) == 0;
    if (maxObjectsReached) {
        return;
    }

    if (temp < arg0->camera_pos[2]) {
        if (g_skyToggle != 0) {

            gDPSetFogColor(gDisplayListHead++, fog_red, fog_blue, fog_green, 0xFF);
            gSPDisplayList(gDisplayListHead++, &D_0D001C20);
        } else {
            gSPDisplayList(gDisplayListHead++, &D_0D001B90);
        }
    } else if (g_skyToggle != 0) {

        gDPSetFogColor(gDisplayListHead++, fog_red, fog_blue, fog_green, 0xFF);
        gSPDisplayList(gDisplayListHead++, &D_0D001C88);
    } else {
        gSPDisplayList(gDisplayListHead++, &D_0D001BD8);
    }
}

UNUSED void load_object_image(uintptr_t addr, s32 width, s32 height) {
    gDPLoadTextureBlock(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(addr), G_IM_FMT_RGBA, G_IM_SIZ_16b, width, height, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
}

void reset_angle(Vec3s arg0) {
    arg0[0] = 0x4000;
    arg0[1] = 0;
    arg0[2] = 0;
}

void calc_face_angle(Collision* arg0, Vec3s arg1) {
    f32 x, y, z;

    if (arg0->flag_zx == 0) {
        reset_angle(arg1);
        return;
    }
    // sp1C = arg0->unk30;

    x = arg0->bump_zx[0];
    y = arg0->bump_zx[1];
    z = arg0->bump_zx[2];

    arg1[0] = Atan2t(z, y) + 0x4000;
    arg1[1] = 0;
    arg1[2] = Atan2t(x, y);
}

void calc_shadow_position(Object* arg0, Vec3f arg1) {
    arg1[0] = arg0->position[0];
    arg1[1] = arg0->position[1];
    arg1[2] = arg0->position[2];
    arg1[1] = CalcHeight(arg1[0], arg1[1], arg1[2], arg0->bump.last_zx);
}

void set_shadow_flag(Player* arg0) {
    arg0->tire_FR.Dummy |= 2;
    arg0->tire_FL.Dummy |= 2;
    arg0->tire_RR.Dummy |= 2;
    arg0->tire_RL.Dummy |= 2;
}

void reset_shadow_flag(Player* arg0) {
    arg0->tire_FR.Dummy &= ~2 & 0xFFFF;
    arg0->tire_FL.Dummy &= ~2 & 0xFFFF;
    arg0->tire_RR.Dummy &= ~2 & 0xFFFF;
    arg0->tire_RL.Dummy &= ~2 & 0xFFFF;
}

// Invert green and red on green shell texture
void initial_red_shell_color(void) {
    s16* red_shell_texture = (s16*) &gTLUTRedShell[0];
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

        *red_shell_texture = (red_color >> 5) | (green_color << 5) | blue_color | alpha_color; // Invert green to red
        green_shell_texture++;
        red_shell_texture++;
    }
}

UNUSED void func_80297944(void) {};

void DisplayShadow(Vec3f pos, Vec3s rot, f32 scale) {
    Mat4 sp20;
    pos[1] += 2.0f;

    CreateModelingMatrix(sp20, pos, rot);
    ScalingMatrix(sp20, scale);
    if (SetMatrix(sp20, 0) != 0) {
        gSPDisplayList(gDisplayListHead++, D_0D007B20);
        pos[1] -= 2.0f;
    }
}

void display_move_shadow(Object* arg0, UNUSED f32 arg1) {
    Vec3f pos;
    Vec3s rot;

    if (arg0->bump.flag_zx != 0) {

        calc_face_angle(&arg0->bump, rot); // arg0 + 0x30
        calc_shadow_position(arg0, pos);
        DisplayShadow(pos, rot, 0.45f);
    }
}

#include "actors/cow/render.inc.c"

#include "actors/yoshi_egg/update.inc.c"

void tree_strategy(Object* arg0) {
    if (((arg0->flag & HIDEOBJ) == 0) && ((arg0->flag & FLYINGOBJ) != 0)) {
        arg0->position[1] = arg0->position[1] + 4.0f;
        if (arg0->position[1] > 800.0f) {
            arg0->flag |= HIDEOBJ;
        }
    }
}

#include "actors/kiwano_fruit/update.inc.c"

#include "actors/paddle_boat/update.inc.c"

#include "actors/train/update.inc.c"

#include "actors/piranha_plant/update.inc.c"

#include "actors/piranha_plant/render.inc.c"

void display_farm_cow(Camera* camera, Mat4 arg1, UNUSED Object* actor) {
    u16 temp_s1;
    f32 temp_f0;
    struct ActorSpawnData* var_t1;
    struct ActorSpawnData* var_s1;
    struct ActorSpawnData* var_s5;
    Vec3f sp88;
    u32 soundThing = SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x4D);
    s32 segment = SEGMENT_NUMBER2(d_course_moo_moo_farm_cow_spawn);
    s32 offset = SEGMENT_OFFSET(d_course_moo_moo_farm_cow_spawn);

    var_t1 = (struct ActorSpawnData*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + offset);
    g_mooSoundLastDistance = 6.4e7f;
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    var_s5 = NULL;
    var_s1 = var_t1;
    while (var_s1->pos[0] != END_OF_SPAWN_DATA) {
        sp88[0] = var_s1->pos[0] * g_mirrorValue;
        sp88[1] = var_s1->pos[1];
        sp88[2] = var_s1->pos[2];
        temp_f0 =
            CheckDisplayRange(camera->camera_pos, sp88, camera->camera_direction[1], 0.0f, gCameraZoom[camera - camera1], 4000000.0f);
        if (temp_f0 > 0.0f) {
            if (temp_f0 < g_mooSoundLastDistance) {
                g_mooSoundLastDistance = temp_f0;
                var_s5 = var_s1;
            }
            arg1[3][0] = sp88[0];
            arg1[3][1] = sp88[1];
            arg1[3][2] = sp88[2];
            if ((gMatrixCount < MTX_OBJECT_POOL_SIZE) && (SetMatrix(arg1, 0) != 0)) {
                switch (var_s1->someId) {
                    case 0:
                        gSPDisplayList(gDisplayListHead++, d_course_moo_moo_farm_dl_cow1);
                        break;
                    case 1:
                        gSPDisplayList(gDisplayListHead++, d_course_moo_moo_farm_dl_cow2);
                        break;
                    case 2:
                        gSPDisplayList(gDisplayListHead++, d_course_moo_moo_farm_dl_cow3);
                        break;
                    case 3:
                        gSPDisplayList(gDisplayListHead++, d_course_moo_moo_farm_dl_cow4);
                        break;
                    case 4:
                        gSPDisplayList(gDisplayListHead++, d_course_moo_moo_farm_dl_cow5);
                        break;
                }
            } else {
                return;
            }
        }
        var_s1++;
    }

    if ((camera == camera1) && (var_s5 != NULL)) {
        if (g_mooSoundCounter == 0) {
            temp_s1 = var_s5 - var_t1;
            if ((temp_s1 != g_mooSoundPointer) && (g_mooSoundLastDistance < 160000.0f)) {
                NAEnmTrgStop(g_mooSoundPosition, soundThing);
                g_mooSoundPosition[0] = var_s5->pos[0] * g_mirrorValue;
                g_mooSoundPosition[1] = var_s5->pos[1];
                g_mooSoundPosition[2] = var_s5->pos[2];
                g_mooSoundPointer = temp_s1;
                NAEnmTrgStart(g_mooSoundPosition, D_802B91C8, soundThing);
                g_mooSoundCounter = 0x00F0;
            }
        } else {
            g_mooSoundCounter -= 1;
        }
    }
}

void check_col_jungle_tree(Player* player) {
    Vec3f pos;
    s32 segment = SEGMENT_NUMBER2(d_course_dks_jungle_parkway_tree_spawn);
    s32 offset = SEGMENT_OFFSET(d_course_dks_jungle_parkway_tree_spawn);
    struct UnkActorSpawnData* data = (struct UnkActorSpawnData*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + offset);

    while (data->pos[0] != END_OF_SPAWN_DATA) {
        pos[0] = data->pos[0] * g_mirrorValue;
        pos[1] = data->pos[1];
        pos[2] = data->pos[2];
        if (CollisionCylinder(player, pos, 5.0f, 40.0f, 0.8f) == COLLISION) {
            if ((player->slip_flag & STAR) != 0) {
                NAEnmTrgStart(player->position, player->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
                NAPlyVoiceStart((u8) (player - gPlayerOne),
                              (player->kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                data->someId |= 0x400;
            }
            if ((player->flag & IS_GHOST) == 0) {
                NAPlyTrgStart((u8) (player - gPlayerOne), SOUND_ARG_LOAD(0x19, 0x00, 0x70, 0x18));
            }
            break;
        }
        data++;
    }
}

void check_jungle_tree_collision(void) {
    s32 index;

    for (index = 0; index < 4; index++) {
        if (((gPlayers[index].flag & 0xC000) != 0) &&
            ((s8) (u8) check_status(gPlayers[index].bump.last_zx) == GRASS)) {
            check_col_jungle_tree(&gPlayers[index]);
        }
    }
}

void reset_jungle_tree(void) {
    s32 segment = SEGMENT_NUMBER2(d_course_dks_jungle_parkway_tree_spawn);
    s32 offset = SEGMENT_OFFSET(d_course_dks_jungle_parkway_tree_spawn);
    struct UnkActorSpawnData* temp_v1 =
        (struct UnkActorSpawnData*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + offset);

    while (temp_v1->pos[0] != END_OF_SPAWN_DATA) {
        temp_v1->pos[1] = temp_v1->unk8;
        temp_v1->someId &= 0xF;
        temp_v1++;
    }
}

void display_jungle_tree(Camera* camera, Mat4 arg1, UNUSED Object* actor) {
    s32 segment = SEGMENT_NUMBER2(d_course_dks_jungle_parkway_tree_spawn);
    s32 offset = SEGMENT_OFFSET(d_course_dks_jungle_parkway_tree_spawn);
    struct UnkActorSpawnData* var_s1 =
        (struct UnkActorSpawnData*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + offset);
    UNUSED s32 pad;
    Vec3f spD4;
    f32 var_f22;
    Mat4 sp90;
    Vec3s sp88 = { 0, 0, 0 };
    s32 test;

    if (gGamestate == CREDITS_SEQUENCE) {
        var_f22 = 9000000.0f;
    } else {
        var_f22 = 1000000.0f;
    }

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);

    while (var_s1->pos[0] != END_OF_SPAWN_DATA) {
        test = var_s1->someId;
        if (test & 0x0800) {
            var_s1++;
            continue;
        }

        if ((test & 0x0400) && ((pause_flag == 0) || (camera == camera1))) {
            var_s1->pos[1] += 0xA;
            if (var_s1->pos[1] >= 0x321) {
                var_s1->someId |= 0x0800;
            }
        }
        spD4[0] = var_s1->pos[0] * g_mirrorValue;
        spD4[1] = var_s1->pos[1];
        spD4[2] = var_s1->pos[2];

        if (CheckDisplayRange(camera->camera_pos, spD4, camera->camera_direction[1], 0.0f, gCameraZoom[camera - camera1], var_f22) <
            0.0f) {
            var_s1++;
            continue;
        }

        test &= 0xF;
        test = (s16) test;
        if (test == 6) {
            CreateModelingMatrix(sp90, spD4, sp88);
            if (!(gMatrixCount < MTX_OBJECT_POOL_SIZE)) {
                break;
            }
            SetMatrix(sp90, 0);
            goto dummylabel;
        } else {
            arg1[3][0] = spD4[0];
            arg1[3][1] = spD4[1];
            arg1[3][2] = spD4[2];
            if (gMatrixCount < MTX_OBJECT_POOL_SIZE) {
                SetMatrix(arg1, 0);
            dummylabel:
                gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
                switch (test) {
                    case 0:
                        gSPDisplayList(gDisplayListHead++, d_course_dks_jungle_parkway_dl_tree1);
                        break;
                    case 4:
                        gSPDisplayList(gDisplayListHead++, d_course_dks_jungle_parkway_dl_tree2);
                        break;
                    case 5:
                        gSPDisplayList(gDisplayListHead++, d_course_dks_jungle_parkway_dl_tree3);
                        break;
                    case 6:
                        gSPDisplayList(gDisplayListHead++, d_course_dks_jungle_parkway_dl_palm_tree);
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

void display_shell(Camera* camera, Mat4 matrix, struct ShellActor* shell) {
    UNUSED s16 pad;
    u16 temp_t8;
    UNUSED s32 pad2;
    s16 sp58[15] = // D_802B87E8;
        { 0x0000, 0x0400, 0x0800, 0x0c00, 0x1000, 0x1400, 0x1800, 0x1c00,
          0x1c00, 0x1800, 0x1400, 0x1000, 0x0c00, 0x0800, 0x0400 };
    //! @todo Is this making the shell spin?
    // Is it doing this by modifying a an address?
    uintptr_t phi_t3;

    f32 temp_f0 =
        CheckDisplayRange(camera->camera_pos, shell->position, camera->camera_direction[1], 0, gCameraZoom[camera - camera1], 490000.0f);
    s32 maxObjectsReached;
    if (temp_f0 < 0.0f) {
        out_of_screen(camera, (Object*) shell);
        return;
    }

    on_screen(camera, (Object*) shell);
    if (temp_f0 < 40000.0f) {
        display_move_shadow((Object*) shell, 3.4f);
    }
    if (shell->category == ACTOR_BLUE_SPINY_SHELL) {
        phi_t3 = (uintptr_t) D_802BA054;
    } else {
        phi_t3 = (uintptr_t) D_802BA050;
    }
    temp_t8 = (u16) shell->rotVelocity / DEGREES(24); // Give a number between 0-15
    phi_t3 += sp58[temp_t8];                   // Select sprite

    matrix[3][0] = shell->position[0];
    matrix[3][1] = (shell->position[1] - shell->radius) + 1.0f;
    matrix[3][2] = shell->position[2];

    maxObjectsReached = SetMatrix(matrix, 0) == 0;
    if (maxObjectsReached) {
        return;
    }

    gDPLoadTextureBlock(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(phi_t3), G_IM_FMT_CI, G_IM_SIZ_8b, 32, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);

    if (temp_t8 < 8) { // Reverse shell ?
        gSPDisplayList(gDisplayListHead++, D_0D005338);
    } else {
        gSPDisplayList(gDisplayListHead++, D_0D005368);
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

UNUSED void PutPylon(f32* pos, s16 state) {
    g_simpleObjectCount = 0;
    g_SimpleObjectArray[PutObject(pos, ACTOR_UNKNOWN_0x14)].sparam = state;
}

void display_pylon(Camera* camera, Mat4 arg1, Object* arg2) {
    if (CheckDisplayRange(camera->camera_pos, arg2->position, camera->camera_direction[1], 0, gCameraZoom[camera - camera1], 4000000.0f) <
        0) {
        return;
    }

    arg1[3][0] = arg2->position[0];
    arg1[3][1] = arg2->position[1] - arg2->radius;
    arg1[3][2] = arg2->position[2];

    if (SetMatrix(arg1, 0) != 0) {
        gSPDisplayList(gDisplayListHead++, &D_0D001750);

        switch (arg2->sparam) {
            case 0:
                gSPDisplayList(gDisplayListHead++, &D_0D001780);
                break;
            case 1:
                gSPDisplayList(gDisplayListHead++, &D_0D001798);
                break;
            case 2:
                gSPDisplayList(gDisplayListHead++, &D_0D0017B0);
                break;
            case 3:
                gSPDisplayList(gDisplayListHead++, &D_0D0017C8);
                break;
            case 4:
                gSPDisplayList(gDisplayListHead++, &D_0D0017E0);
                break;
            case 5:
                gSPDisplayList(gDisplayListHead++, &D_0D0017F8);
                break;
            case 6:
                gSPDisplayList(gDisplayListHead++, &D_0D001810);
                break;
            case 7:
                gSPDisplayList(gDisplayListHead++, &D_0D001828);
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

#include "actors/falling_rock/render.inc.c"

void SetPakkunObject(struct ActorSpawnData* spawnData) {
    s32 segment = SEGMENT_NUMBER2(spawnData);
    s32 offset = SEGMENT_OFFSET(spawnData);
    struct ActorSpawnData* temp_s0 = (struct ActorSpawnData*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + offset);
    PakkunObject* temp_v1;
    UNUSED s32 pad;
    Vec3f startingPos;
    Vec3f startingVelocity;
    Vec3s startingRot;
    s32 temp;

    vec3f_set(startingVelocity, 0, 0, 0);
    SetSVector(startingRot, 0, 0, 0);

    while (temp_s0->pos[0] != END_OF_SPAWN_DATA) {
        startingPos[0] = temp_s0->pos[0] * g_mirrorValue;
        startingPos[1] = temp_s0->pos[1];
        startingPos[2] = temp_s0->pos[2];
        temp = addObjectBuffer(startingPos, startingRot, startingVelocity, ACTOR_PIRANHA_PLANT);
        temp_v1 = (PakkunObject*) &g_SimpleObjectArray[temp];
        temp_v1->visibilityStates[0] = 0;
        temp_v1->visibilityStates[1] = 0;
        temp_v1->visibilityStates[2] = 0;
        temp_v1->visibilityStates[3] = 0;
        temp_v1->timers[0] = 0;
        temp_v1->timers[1] = 0;
        temp_v1->timers[2] = 0;
        temp_v1->timers[3] = 0;
        temp_s0++;
    }
}

void set_palmtree_object(struct ActorSpawnData* spawnData) {
    s32 segment = SEGMENT_NUMBER2(spawnData);
    s32 offset = SEGMENT_OFFSET(spawnData);
    struct ActorSpawnData* temp_s0 = (struct ActorSpawnData*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + offset);
    struct PalmTree* temp_v1;
    Vec3f startingPos;
    Vec3f startingVelocity;
    Vec3s startingRot;
    s32 temp;

    vec3f_set(startingVelocity, 0, 0, 0);
    SetSVector(startingRot, 0, 0, 0);

    while (temp_s0->pos[0] != END_OF_SPAWN_DATA) {
        startingPos[0] = temp_s0->pos[0] * g_mirrorValue;
        startingPos[1] = temp_s0->pos[1];
        startingPos[2] = temp_s0->pos[2];
        temp = addObjectBuffer(startingPos, startingRot, startingVelocity, ACTOR_PALM_TREE);
        temp_v1 = (struct PalmTree*) &g_SimpleObjectArray[temp];

        temp_v1->variant = temp_s0->someId;
        CheckBump((Collision*) &temp_v1->bump, 5.0f, temp_v1->position[0], temp_v1->position[1], temp_v1->position[2]);
        calc_face_angle((Collision*) &temp_v1->bump, temp_v1->angle);
        temp_s0++;
    }
}

#include "actors/falling_rock/update.inc.c"

// Trees, cacti, shrubs, etc.
void SetTreeObject(struct ActorSpawnData* arg0) {
    UNUSED s32 pad[4];
    Vec3f position;
    Vec3f velocity;
    Vec3s rotation;
    UNUSED s16 pad2;
    s16 actorType;
    Object* temp_s0;
    struct ActorSpawnData* var_s3;
    s32 segment = SEGMENT_NUMBER2(arg0);
    s32 offset = SEGMENT_OFFSET(arg0);

    var_s3 = (struct ActorSpawnData*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + offset);
    vec3f_set(velocity, 0.0f, 0.0f, 0.0f);
    rotation[0] = DEGREES(90);
    rotation[1] = 0;
    rotation[2] = 0;

    while (var_s3->pos[0] != END_OF_SPAWN_DATA) {
        position[0] = var_s3->pos[0] * g_mirrorValue;
        position[2] = var_s3->pos[2];
        position[1] = var_s3->pos[1];

        switch (g_courseID) {
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
                switch (var_s3->signedSomeId) {
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
                switch (var_s3->signedSomeId) {
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

        temp_s0 = &g_SimpleObjectArray[addObjectBuffer(position, rotation, velocity, actorType)];
        if (gGamestate == CREDITS_SEQUENCE) {
            reset_angle(temp_s0->angle);
        } else {
            CheckBump(&temp_s0->bump, 5.0f, temp_s0->position[0], temp_s0->position[1], temp_s0->position[2]);
            if (temp_s0->bump.distance_zx < 0.0f) {
                temp_s0->position[1] = CalcHeight(temp_s0->position[0], temp_s0->position[1], temp_s0->position[2],
                                                           temp_s0->bump.last_zx);
            }
            calc_face_angle(&temp_s0->bump, temp_s0->angle);
        }
        var_s3++;
    }
}

void SetItemBoxObject(struct ActorSpawnData* spawnData) {
    s32 segment = SEGMENT_NUMBER2(spawnData);
    s32 offset = SEGMENT_OFFSET(spawnData);
    s16 temp_s1;
    f32 temp_f0;
    Vec3f startingPos;
    Vec3f startingVelocity;
    Vec3s startingRot;
    struct ActorSpawnData* temp_s0 = (struct ActorSpawnData*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + offset);
    // struct ItemBox *itemBox;

    if ((g_gameMode == TIME_TRIALS) || (g_ItemSetFlag == 0)) {
        return;
    }

    vec3f_set(startingVelocity, 0, 0, 0);
    while (temp_s0->pos[0] != END_OF_SPAWN_DATA) {
        startingPos[0] = temp_s0->pos[0] * g_mirrorValue;
        startingPos[1] = temp_s0->pos[1];
        startingPos[2] = temp_s0->pos[2];
        startingRot[0] = MakeRandom();
        startingRot[1] = MakeRandom();
        startingRot[2] = MakeRandom();
        temp_s1 = addObjectBuffer(startingPos, startingRot, startingVelocity, ACTOR_ITEM_BOX);
        temp_f0 = CheckHight(startingPos[0], startingPos[1] + 10.0f, startingPos[2]);

        // Should be struct ItemBox but not enough space in the stack.
        // It's either the ItemBox or the SEGMENT/OFFSET variables.
        // itemBox = (struct ItemBox *) &g_SimpleObjectArray[temp_s1];

        g_SimpleObjectArray[temp_s1].fparam = temp_f0;
        // itemBox->resetDistance = temp_f0;

        g_SimpleObjectArray[temp_s1].velocity[0] = startingPos[1];
        // itemBox->origY = startingPos[1];

        g_SimpleObjectArray[temp_s1].position[1] = temp_f0 - 20.0f;
        // itemBox->position[1] = temp_f0 - 20.0f;

        temp_s0++;
    }
}

void set_jungle_nuts(void) {
    Vec3f sp64;
    Vec3f sp58;
    Vec3s sp50;
    Player* phi_s1;
    Object* actor;
    s16 phi_s0;
    s32 i;

    // phi_s0 = 0;
    for (i = 0; i < 4; i++) {
        phi_s1 = &gPlayers[i];
        // temp_v0 = *phi_s1;
        if ((phi_s1->flag & IS_PLAYER) == 0) {
            continue;
        }
        if ((phi_s1->flag & IS_GHOST) != 0) {
            continue;
        }

        phi_s0 = addObjectBuffer(sp64, sp50, sp58, ACTOR_KIWANO_FRUIT);
        actor = &g_SimpleObjectArray[phi_s0];
        actor->counter = i;
    }
}

/**
 * Destroys actors via zeroing some of the member data
 * Key word some. When spawning a new actor,
 * members such as pos and rot should be set to prevent using expired data
 **/
void initial_object_buffer(void) {
    s32 i;
    g_simpleObjectCount = 0;
    for (i = 0; i < ACTOR_LIST_SIZE; i++) {
        g_SimpleObjectArray[i].flag = 0;
        g_SimpleObjectArray[i].category = 0;
        g_SimpleObjectArray[i].counter = 0;
        g_SimpleObjectArray[i].sparam = 0;
        g_SimpleObjectArray[i].fparam = 0.0f;
        g_SimpleObjectArray[i].radius = 0.0f;
    }
}

void InitialMapObject(void) {
    UNUSED s32 pad;
    Vec3f position;
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3s rotation = { 0, 0, 0 };
    Object* actor;
    struct RailroadCrossing* rrxing;

    g_StaticObjectCount = 0;
#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (g_courseID) {
        case COURSE_MARIO_RACEWAY:
            SetTreeObject(d_course_mario_raceway_tree_spawns);
            SetPakkunObject(d_course_mario_raceway_piranha_plant_spawns);
            PlaceIBoxes((long) d_course_mario_raceway_item_box_spawns);
            vec3f_set(position, 150.0f, 40.0f, -1300.0f);
            position[0] *= g_mirrorValue;
            addObjectBuffer(position, rotation, velocity, ACTOR_MARIO_SIGN);
            vec3f_set(position, 2520.0f, 0.0f, 1240.0f);
            position[0] *= g_mirrorValue;
            actor = &g_SimpleObjectArray[addObjectBuffer(position, rotation, velocity, ACTOR_MARIO_SIGN)];
            actor->flag |= HITOBJ;
            break;
        case COURSE_CHOCO_MOUNTAIN:
            PlaceIBoxes((long) d_course_choco_mountain_item_box_spawns);
            set_iwa(d_course_choco_mountain_falling_rock_spawns);
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
            vec3f_set(position, -2300.0f, 0.0f, 634.0f);
            position[0] *= g_mirrorValue;
            addObjectBuffer(position, rotation, velocity, ACTOR_YOSHI_EGG);
            break;
        case COURSE_FRAPPE_SNOWLAND:
            SetTreeObject(d_course_frappe_snowland_tree_spawns);
            PlaceIBoxes((long) d_course_frappe_snowland_item_box_spawns);
            break;
        case COURSE_KOOPA_BEACH:
            initial_special_itembox(328.0f * g_mirrorValue, 70.0f, 2541.0f);
            PlaceIBoxes((long) d_course_koopa_troopa_beach_item_box_spawns);
            set_palmtree_object(d_course_koopa_troopa_beach_tree_spawn);
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
            if (g_playerCount != 4) {
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
            vec3f_set(position, -1680.0f, 2.0f, 35.0f);
            position[0] *= g_mirrorValue;
            rrxing = (struct RailroadCrossing*) &g_SimpleObjectArray[addObjectBuffer(position, rotation, velocity,
                                                                                    ACTOR_RAILROAD_CROSSING)];
            rrxing->crossingId = 1;
            vec3f_set(position, -1600.0f, 2.0f, 35.0f);
            position[0] *= g_mirrorValue;
            rrxing = (struct RailroadCrossing*) &g_SimpleObjectArray[addObjectBuffer(position, rotation, velocity,
                                                                                    ACTOR_RAILROAD_CROSSING)];
            rrxing->crossingId = 1;
            SetSVector(rotation, 0, -DEGREES(45), 0);
            vec3f_set(position, -2459.0f, 2.0f, 2263.0f);
            position[0] *= g_mirrorValue;
            rrxing = (struct RailroadCrossing*) &g_SimpleObjectArray[addObjectBuffer(position, rotation, velocity,
                                                                                    ACTOR_RAILROAD_CROSSING)];
            rrxing->crossingId = 0;
            vec3f_set(position, -2467.0f, 2.0f, 2375.0f);
            position[0] *= g_mirrorValue;
            rrxing = (struct RailroadCrossing*) &g_SimpleObjectArray[addObjectBuffer(position, rotation, velocity,
                                                                                    ACTOR_RAILROAD_CROSSING)];
            rrxing->crossingId = 0;
            break;
        case COURSE_SHERBET_LAND:
            PlaceIBoxes((long) d_course_sherbet_land_item_box_spawns);
            break;
        case COURSE_RAINBOW_ROAD:
            PlaceIBoxes((long) d_course_rainbow_road_item_box_spawns);
            break;
        case COURSE_WARIO_STADIUM:
            PlaceIBoxes((long) d_course_wario_stadium_item_box_spawns);
            vec3f_set(position, -131.0f, 83.0f, 286.0f);
            position[0] *= g_mirrorValue;
            addObjectBuffer(position, rotation, velocity, ACTOR_WARIO_SIGN);
            vec3f_set(position, -2353.0f, 72.0f, -1608.0f);
            position[0] *= g_mirrorValue;
            addObjectBuffer(position, rotation, velocity, ACTOR_WARIO_SIGN);
            vec3f_set(position, -2622.0f, 79.0f, 739.0f);
            position[0] *= g_mirrorValue;
            addObjectBuffer(position, rotation, velocity, ACTOR_WARIO_SIGN);
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
            set_jungle_nuts();
            reset_jungle_tree();
            break;
        case COURSE_BIG_DONUT:
            PlaceIBoxes((long) d_course_big_donut_item_box_spawns);
            break;
    }
#else

#endif
    g_StaticObjectCount = g_simpleObjectCount;
}

/**
 * @brief Loads actor textures, course specific actor textures.
 * Calls to ogawa_hanyou_object_init and place_course_actors
 *
 */
void initial_object(void) {
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
    switch (g_courseID) {
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
    initial_red_shell_color();
    initial_object_buffer();
    InitialMapObjectCode();
    ogawa_hanyou_object_init();
}

void check_object_se(Object* actor) {
    s16 flags = actor->flag;

    if ((flags & 0x200) != 0) {
        NAEnmTrgStop(actor->position, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
        return;
    }
    if ((flags & 0x100) != 0) {
        NAEnmTrgStop(actor->position, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
        return;
    }
    if ((flags & 0x80) != 0) {
        NAEnmTrgStop(actor->position, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x54));
    }
}

/**
 * This func likely plays an audio track based on flag
 * Next, it destroys the actor via zeroing its flags and type.
 * Note that the data from its other members still exist.
 * Actors are expected to set members such as pos and rot data if used. Not doing so could result in the use of expired
 * data.
 *
 * This method does not require modification to g_SimpleObjectArray directly.
 * No popping members of g_SimpleObjectArray. The list is always the size of ACTOR_LIST_SIZE.
 * Actors are members of g_SimpleObjectArray by definition.
 *
 * @param Actor to destroy
 */
void deleteObjectBuffer(Object* actor) {
    check_object_se(actor);
    actor->flag = 0;
    actor->category = 0;
    g_simpleObjectCount--;
}

s16 swap_object(Vec3f pos, Vec3s rot, Vec3f velocity, s16 actorType) {
    s32 actorIndex;
    struct ShellActor* compare;

    // try removing a red shell, green shell, banana, or a fake item box if the actor is expired
    for (actorIndex = g_StaticObjectCount; actorIndex < ACTOR_LIST_SIZE; actorIndex++) {
        compare = (struct ShellActor*) &g_SimpleObjectArray[actorIndex];
        if (!(compare->flag & ACTOR_IS_NOT_EXPIRED)) {
            switch (compare->category) {
                case ACTOR_RED_SHELL:
                    switch (compare->sparam) {
                        case MOVING_SHELL:
                        case RED_SHELL_LOCK_ON:
                        case TRIPLE_GREEN_SHELL:
                        case GREEN_SHELL_HIT_A_RACER:
                        case BLUE_SHELL_LOCK_ON:
                        case BLUE_SHELL_TARGET_ELIMINATED:
                            kame_hibasira_clear(actorIndex);
                        case DESTROYED_SHELL:
                            check_object_se((Object*) compare);
                            set_object_buffer((Object*) compare, pos, rot, velocity, actorType);
                            return actorIndex;
                        default:
                            break;
                    }
                    break;
                case ACTOR_GREEN_SHELL:
                    switch (compare->sparam) {
                        case MOVING_SHELL:
                            kame_hibasira_clear(actorIndex);
                        case DESTROYED_SHELL:
                            check_object_se((Object*) compare);
                            set_object_buffer((Object*) compare, pos, rot, velocity, actorType);
                            return actorIndex;
                    }
                    break;
                case ACTOR_BANANA:
                    switch (compare->sparam) {
                        case DROPPED_BANANA:
                        case BANANA_ON_GROUND:
                        case DESTROYED_BANANA:
                            check_object_se((Object*) compare);
                            set_object_buffer((Object*) compare, pos, rot, velocity, actorType);
                            return actorIndex;
                    }
                    break;
                case ACTOR_FAKE_ITEM_BOX:
                    switch (compare->sparam) {
                        case FAKE_ITEM_BOX_ON_GROUND:
                        case DESTROYED_FAKE_ITEM_BOX:
                            check_object_se((Object*) compare);
                            set_object_buffer((Object*) compare, pos, rot, velocity, actorType);
                            return actorIndex;
                    }
                    break;
                default:
                    break;
            }
        }
    }

    // will remove the oldest destructable actor in the list
    for (actorIndex = g_StaticObjectCount; actorIndex < ACTOR_LIST_SIZE; actorIndex++) {
        compare = (struct ShellActor*) &g_SimpleObjectArray[actorIndex];
        switch (compare->category) {
            case ACTOR_RED_SHELL:
                switch (compare->sparam) {
                    case MOVING_SHELL:
                    case RED_SHELL_LOCK_ON:
                    case TRIPLE_GREEN_SHELL:
                    case GREEN_SHELL_HIT_A_RACER:
                    case BLUE_SHELL_LOCK_ON:
                    case BLUE_SHELL_TARGET_ELIMINATED:
                        kame_hibasira_clear(actorIndex);
                    case DESTROYED_SHELL:
                        check_object_se((Object*) compare);
                        set_object_buffer((Object*) compare, pos, rot, velocity, actorType);
                        return actorIndex;
                    default:
                        break;
                }
                break;
            case ACTOR_GREEN_SHELL:
                switch (compare->sparam) {
                    case MOVING_SHELL:
                        kame_hibasira_clear(actorIndex);
                    case DESTROYED_SHELL:
                        check_object_se((Object*) compare);
                        set_object_buffer((Object*) compare, pos, rot, velocity, actorType);
                        return actorIndex;
                }
                break;
            case ACTOR_BANANA:
                switch (compare->sparam) {
                    case DROPPED_BANANA:
                    case BANANA_ON_GROUND:
                    case DESTROYED_BANANA:
                        check_object_se((Object*) compare);
                        set_object_buffer((Object*) compare, pos, rot, velocity, actorType);
                        return actorIndex;
                }
                break;
            case ACTOR_FAKE_ITEM_BOX:
                switch (compare->sparam) {
                    case FAKE_ITEM_BOX_ON_GROUND:
                    case DESTROYED_FAKE_ITEM_BOX:
                        check_object_se((Object*) compare);
                        set_object_buffer((Object*) compare, pos, rot, velocity, actorType);
                        return actorIndex;
                }
                break;
            default:
                break;
        }
    }

    return -1;
}

// returns actor index if any slot avaible returns -1
s16 addObjectBuffer(Vec3f pos, Vec3s rot, Vec3f velocity, s16 actorType) {
    s32 index;

    if (g_simpleObjectCount >= ACTOR_LIST_SIZE) {
        return swap_object(pos, rot, velocity, actorType);
    }
    for (index = 0; index < ACTOR_LIST_SIZE; index++) {
        if (g_SimpleObjectArray[index].flag == 0) {
            g_simpleObjectCount++;
            set_object_buffer(&g_SimpleObjectArray[index], pos, rot, velocity, actorType);
            return index;
        }
    }
    return -1;
}

UNUSED s16 PutObject(Vec3f pos, s16 actorType) {
    Vec3f vel;
    Vec3s rot;

    vec3f_set(vel, 0.0f, 0.0f, 0.0f);
    SetSVector(rot, 0, 0, 0);
    return addObjectBuffer(pos, rot, vel, actorType);
}

// not ActorSpawnData but very similar in structure and use
struct test {
    Vec3s thing;
};

UNUSED void check_pole_collision(Player* player, uintptr_t arg1) {
    Vec3f sp64;
    struct test* var_s0;
    s32 segment = SEGMENT_NUMBER2(arg1);
    s32 offset = SEGMENT_OFFSET(arg1);

    var_s0 = (struct test*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + offset);
    while (var_s0->thing[0] != END_OF_SPAWN_DATA) {
        sp64[0] = var_s0->thing[0] * g_mirrorValue;
        sp64[1] = var_s0->thing[1];
        sp64[2] = var_s0->thing[2];
        if (arg1 & arg1) {}
        CollisionCylinder(player, sp64, 5.0f, 40.0f, 0.8f);
        var_s0++;
    }
}

bool CollisionCylinder(Player* player, Vec3f pos, f32 minDist, f32 dist, f32 arg4) {
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

    minDist = player->radius + minDist;
    dist = player->radius + dist;
    xDist = pos[0] - player->position[0];
    if (minDist < xDist) {
        return NO_COLLISION;
    }
    if (xDist < -minDist) {
        return NO_COLLISION;
    }
    yDist = pos[1] - player->position[1];
    if (dist < yDist) {
        return NO_COLLISION;
    }
    if (yDist < -dist) {
        return NO_COLLISION;
    }
    zDist = pos[2] - player->position[2];
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
    xVelocity = player->velocity[0];
    zVelocity = player->velocity[2];
    if (player->slip_flag & STAR) {
        return COLLISION;
    }
    if (sqrtDist < 0.1f) {
        temp_f0_4 = sqrtf((xVelocity * xVelocity) + (zVelocity * zVelocity));
        if (temp_f0_4 < 0.5f) {
            temp_f0_4 = 0.5f;
        }
        player->velocity[0] = 0;
        player->velocity[2] = 0;
        player->position[0] += (xVelocity / temp_f0_4) * minDist;
        player->position[2] += (zVelocity / temp_f0_4) * minDist;
    } else {
        player->slip_flag |= CARHIT;
        xDist /= sqrtDist;
        zDist /= sqrtDist;
        temp_f0_5 = sqrtf((xVelocity * xVelocity) + (zVelocity * zVelocity));
        if (temp_f0_5 < 0.25f) {
            temp_f0_6 = 1.2f;
            player->position[0] = pos[0] - (xDist * minDist * temp_f0_6);
            player->position[2] = pos[2] - (zDist * minDist * temp_f0_6);
            player->velocity[0] = 0.0f;
            player->velocity[2] = 0.0f;
            return COLLISION;
        }
        temp_f2_2 = ((xDist * xVelocity) + (zDist * zVelocity)) / temp_f0_5;
        temp_f2_2 = temp_f0_5 * temp_f2_2 * arg4 * 1.3f;
        player->velocity[0] -= xDist * temp_f2_2;
        player->velocity[2] -= zDist * temp_f2_2;
        player->position[0] += xDist * sp28 * 0.5f;
        player->position[2] += zDist * sp28 * 0.5f;
    }
    return COLLISION;
}

bool collision_mkanban(Player* player, Object* marioRacewaySign) {
    if (CollisionCylinder(player, marioRacewaySign->position, 7.0f, 200.0f, 0.8f) == COLLISION) {
        if ((player->flag & IS_PLAYER) != 0) {
            if ((player->slip_flag & STAR) != 0) {
                marioRacewaySign->flag |= FLYINGOBJ;
                NAEnmTrgStart(player->position, player->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
                NAPlyVoiceStart(player - gPlayerOne,
                              (player->kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
            } else if ((player->flag & IS_GHOST) == 0) {
                NAPlyTrgStart(player - gPlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0x70, 0x1A));
            }
        }
        return true;
    }
    return false;
}

bool collision_pakkun(Player* player, PakkunObject* plant) {
    if (CollisionCylinder(player, plant->position, plant->radius, plant->radius,
                                                 2.5f) == COLLISION) {
        if ((player->flag & IS_PLAYER) != 0) {
            if ((player->slip_flag & STAR) != 0) {
                plant->flag |= FLYINGOBJ;
                NAEnmTrgStart(player->position, player->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0xA2, 0x4A));
                NAPlyVoiceStart(player - gPlayerOne,
                              (player->kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
            } else if ((player->flag & IS_GHOST) == 0) {
                NAPlyTrgStart(player - gPlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0xA0, 0x52));
            }
        }
        return true;
    }
    return false;
}

bool collision_egg(Player* player, struct YoshiValleyEgg* egg) {
    UNUSED f32 pad[5];
    f32 z_dist;
    f32 xz_dist;
    f32 x_dist;
    f32 y_dist;
    f32 totalBox;
    f32 maxDist = 60.0f;
    f32 minDist = 0.0f;

    x_dist = egg->position[0] - player->position[0];
    if ((x_dist < minDist) && (x_dist < -maxDist)) {
        return false;
    }
    if (x_dist > maxDist) {
        return false;
    }

    z_dist = egg->position[2] - player->position[2];
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
    set_shadow_flag(player);

    y_dist = player->position[1] - egg->position[1];
    if (y_dist < minDist) {
        return false;
    }

    totalBox = player->radius + egg->radius;
    if (totalBox < xz_dist) {
        return false;
    }

    if ((player->flag & IS_PLAYER) != 0) {
        if ((player->slip_flag & STAR) != 0) {
            egg->flag |= FLYINGOBJ;
            egg->pathCenter[1] = 8.0f;
            NAEnmTrgStart(player->position, player->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
            NAPlyVoiceStart(player - gPlayerOne, (player->kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
        } else {
            SetBroken(player, player - gPlayerOne);
            if ((g_gameMode == TIME_TRIALS) && ((player->flag & IS_CPU_PLAYER) == 0)) {
                lost_ghost = 1;
            }
        }
    } else {
        SetBroken(player, player - gPlayerOne);
    }

    return true;
}

bool collision_tree(Player* player, Object* actor) {
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

    var_f16 = actor->fparam;
    x_dist = actor->position[0] - player->position[0];
    if ((x_dist < 0.0f) && (x_dist < -var_f16)) {
        return false;
    }
    if (var_f16 < x_dist) {
        return false;
    }
    z_dist = actor->position[2] - player->position[2];
    if ((z_dist < 0.0f) && (z_dist < -var_f16)) {
        return false;
    }
    if (var_f16 < z_dist) {
        return false;
    }
    y_dist = player->position[1] - actor->position[1];
    if (y_dist < 0.0f) {
        return false;
    }
    if ((f32) actor->sparam < y_dist) {
        return false;
    }
    xz_dist = sqrtf((x_dist * x_dist) + (z_dist * z_dist));
    if (var_f16 < xz_dist) {
        return false;
    }
    set_shadow_flag(player);
    var_f16 = player->radius + actor->radius;
    if (var_f16 < xz_dist) {
        return false;
    }
    sp48 = player->velocity[0];
    sp44 = player->velocity[2];
    if (player->flag & IS_PLAYER) {
        if (player->slip_flag & STAR) {
            actor->flag |= FLYINGOBJ;
            NAEnmTrgStart(player->position, player->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
            NAPlyVoiceStart(player - gPlayerOne, (player->kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
            return true;
        }
        if (!(player->flag & IS_GHOST)) {
            NAPlyTrgStart(player - gPlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0x70, 0x18));
        }
    }
    if (!(player->slip_flag & STAR)) {
        player->slip_flag |= CARHIT;
    }
    actorPos[0] = actor->position[0];
    actorPos[1] = actor->position[1];
    actorPos[2] = actor->position[2];
    if (((g_courseID == COURSE_MARIO_RACEWAY) || (g_courseID == COURSE_YOSHI_VALLEY) ||
         (g_courseID == COURSE_ROYAL_RACEWAY) || (g_courseID == COURSE_LUIGI_RACEWAY)) &&
        (player->speed > 1.0f)) {
        kwhit_tree(actorPos, 0);
    }
    if (xz_dist < 0.1f) {
        sqrtf((sp48 * sp48) + (sp44 * sp44));
        if (xz_dist) {}
        player->velocity[0] = 0;
        player->velocity[2] = 0;
        player->position[0] = actorPos[0] - (x_dist * var_f16 * 1.2f);
        player->position[2] = actorPos[2] - (z_dist * var_f16 * 1.2f);
    } else {
        temp_f0_4 = sqrtf((sp48 * sp48) + (sp44 * sp44));
        x_dist /= xz_dist;
        z_dist /= xz_dist;
        if (temp_f0_4 < 0.25f) {
            player->position[0] = actorPos[0] - (x_dist * var_f16 * 1.2f);
            player->position[2] = actorPos[2] - (z_dist * var_f16 * 1.2f);
            player->velocity[0] = 0;
            player->velocity[2] = 0;
            return true;
        }
        temp_f12 = ((x_dist * sp48) + (z_dist * sp44)) / temp_f0_4;
        temp_f12 = temp_f0_4 * temp_f12 * 1.5f;
        player->velocity[0] -= x_dist * temp_f12;
        player->velocity[2] -= z_dist * temp_f12;
        temp_f2 = xz_dist - var_f16;
        player->position[0] += x_dist * temp_f2 * 0.5f;
        player->position[2] += z_dist * temp_f2 * 0.5f;
    }
    return true;
}

bool CollisionSphere(Player* arg0, Object* arg1) {
    f32 temp_f0;
    f32 dist;
    f32 yDist;
    f32 zDist;
    f32 xDist;

    temp_f0 = arg0->radius + arg1->radius;
    xDist = arg1->position[0] - arg0->position[0];
    if (temp_f0 < xDist) {
        return NO_COLLISION;
    }
    if (xDist < -temp_f0) {
        return NO_COLLISION;
    }
    yDist = arg1->position[1] - arg0->position[1];
    if (temp_f0 < yDist) {
        return NO_COLLISION;
    }
    if (yDist < -temp_f0) {
        return NO_COLLISION;
    }
    zDist = arg1->position[2] - arg0->position[2];
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

bool collision_sphere2(Object* arg0, Object* arg1) {
    f32 temp_f0;
    f32 dist;
    f32 dist_y;
    f32 dist_z;
    f32 dist_x;

    temp_f0 = arg0->radius + arg1->radius;
    dist_x = arg0->position[0] - arg1->position[0];
    if (temp_f0 < dist_x) {
        return NO_COLLISION;
    }
    if (dist_x < -temp_f0) {
        return NO_COLLISION;
    }
    dist_y = arg0->position[1] - arg1->position[1];
    if (temp_f0 < dist_y) {
        return NO_COLLISION;
    }
    if (dist_y < -temp_f0) {
        return NO_COLLISION;
    }
    dist_z = arg0->position[2] - arg1->position[2];
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
    Player* player;

    switch (actor->category) {
        case ACTOR_BANANA:
            banana = (struct BananaActor*) actor;
            switch (banana->sparam) {
                case FIRST_BANANA_BUNCH_BANANA:
                case BANANA_BUNCH_BANANA:
                    clean_up_banana(banana);
                    break;
                case HELD_BANANA:
                    player = &gPlayers[banana->playerId];
                    player->weapon &= ~USE_BANANA;
                    /* fallthrough */
                case BANANA_ON_GROUND:
                    banana->flag = -0x8000;
                    banana->counter = 0x003C;
                    banana->sparam = DESTROYED_BANANA;
                    banana->velocity[1] = 3.0f;
                    break;
                case DROPPED_BANANA:
                case DESTROYED_BANANA:
                default:
                    break;
            }
            break;
        case ACTOR_GREEN_SHELL:
            shell = (struct ShellActor*) actor;
            if (shell->sparam != GREEN_SHELL_HIT_A_RACER) {
                switch (shell->sparam) {
                    case MOVING_SHELL:
                        kame_hibasira_clear(actor - g_SimpleObjectArray);
                        /* fallthrough */
                    case HELD_SHELL:
                    case RELEASED_SHELL:
                        shell->flag = -0x8000;
                        shell->rotAngle = 0;
                        shell->someTimer = 0x003C;
                        shell->sparam = GREEN_SHELL_HIT_A_RACER;
                        shell->velocity[1] = 3.0f;
                        break;
                    case TRIPLE_GREEN_SHELL:
                        delete_rolling_shell(shell, ACTOR_GREEN_SHELL);
                        break;
                    default:
                        break;
                }
            }
            break;
        case ACTOR_BLUE_SPINY_SHELL:
            shell = (struct ShellActor*) actor;
            if (shell->sparam != DESTROYED_SHELL) {
                switch (shell->sparam) {
                    case MOVING_SHELL:
                    case RED_SHELL_LOCK_ON:
                    case TRIPLE_GREEN_SHELL:
                    case GREEN_SHELL_HIT_A_RACER:
                    case BLUE_SHELL_LOCK_ON:
                    case BLUE_SHELL_TARGET_ELIMINATED:
                        NaSceneLevelStop(shell->position, SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x08));
                        kame_hibasira_clear(actor - g_SimpleObjectArray);
                        /* fallthrough */
                    case HELD_SHELL:
                    case RELEASED_SHELL:
                        shell->flag = -0x8000;
                        shell->rotAngle = 0;
                        shell->someTimer = 0x003C;
                        shell->sparam = DESTROYED_SHELL;
                        shell->velocity[1] = 3.0f;
                        break;
                    default:
                        break;
                }
            }
            break;
        case ACTOR_RED_SHELL:
            shell = (struct ShellActor*) actor;
            if (shell->sparam != DESTROYED_SHELL) {
                switch (shell->sparam) {
                    case MOVING_SHELL:
                    case RED_SHELL_LOCK_ON:
                    case TRIPLE_GREEN_SHELL:
                    case GREEN_SHELL_HIT_A_RACER:
                    case BLUE_SHELL_LOCK_ON:
                    case BLUE_SHELL_TARGET_ELIMINATED:
                        kame_hibasira_clear(actor - g_SimpleObjectArray);
                        /* fallthrough */
                    case HELD_SHELL:
                    case RELEASED_SHELL:
                        shell->flag = -0x8000;
                        shell->rotAngle = 0;
                        shell->someTimer = 0x003C;
                        shell->sparam = DESTROYED_SHELL;
                        shell->velocity[1] = 3.0f;
                        break;
                    case TRIPLE_RED_SHELL:
                        delete_rolling_shell(shell, ACTOR_RED_SHELL);
                        break;
                    default:
                        break;
                }
            }
            break;
        case ACTOR_FAKE_ITEM_BOX:
            fakeItemBox = (struct FakeItemBox*) actor;
            player = &gPlayers[(s16) fakeItemBox->playerId];
            if (fakeItemBox->sparam == HELD_FAKE_ITEM_BOX) {
                player->weapon &= ~USE_BANANA;
            }
            fakeItemBox->sparam = DESTROYED_FAKE_ITEM_BOX;
            fakeItemBox->flag = -0x8000;
            fakeItemBox->someTimer = 0;
            break;
    }
}

void check_crush_sound(Object* arg0, Object* arg1) {
    switch (arg0->category) {
        case ACTOR_GREEN_SHELL:
            if ((arg0->sparam == HELD_SHELL) || (arg0->sparam == TRIPLE_GREEN_SHELL)) {
                arg0->flag |= SEBANANAOBJ;
                NAEnmTrgStart(arg0->position, arg0->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
                return;
            }
            break;
        case ACTOR_RED_SHELL:
            if ((arg0->sparam == HELD_SHELL) || (arg0->sparam == TRIPLE_RED_SHELL)) {
                arg0->flag |= SEBANANAOBJ;
                NAEnmTrgStart(arg0->position, arg0->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
                return;
            }
            break;
        case ACTOR_BLUE_SPINY_SHELL:
            if (arg0->sparam == HELD_SHELL) {
                arg0->flag |= SEBANANAOBJ;
                NAEnmTrgStart(arg0->position, arg0->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
                return;
            }
            break;
        case ACTOR_FAKE_ITEM_BOX:
            if (arg0->sparam == HELD_FAKE_ITEM_BOX) {
                arg0->flag |= SEBANANAOBJ;
                NAEnmTrgStart(arg0->position, arg0->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
                return;
            }
            break;
    }

    switch (arg1->category) {
        case ACTOR_GREEN_SHELL:
            if ((arg1->sparam == HELD_SHELL) || (arg1->sparam == TRIPLE_GREEN_SHELL)) {
                arg1->flag |= SEBANANAOBJ;
                NAEnmTrgStart(arg1->position, arg1->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
                return;
            }
            break;
        case ACTOR_RED_SHELL:
            if ((arg1->sparam == HELD_SHELL) || (arg1->sparam == TRIPLE_RED_SHELL)) {
                arg1->flag |= SEBANANAOBJ;
                NAEnmTrgStart(arg1->position, arg1->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
                return;
            }
            break;
        case ACTOR_BLUE_SPINY_SHELL:
            if (arg1->sparam == HELD_SHELL) {
                arg1->flag |= SEBANANAOBJ;
                NAEnmTrgStart(arg1->position, arg1->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
                return;
            }
            break;
        case ACTOR_FAKE_ITEM_BOX:
            if (arg1->sparam == HELD_FAKE_ITEM_BOX) {
                arg1->flag |= SEBANANAOBJ;
                NAEnmTrgStart(arg1->position, arg1->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
                return;
            }
            break;
    }

    arg0->flag |= SEKAMEHITOBJ;
    NAEnmTrgStart(arg0->position, arg0->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
}

void collision_check_routin2(Object* actor1, Object* actor2) {
    if (collision_sphere2(actor1, actor2) == COLLISION) {
        if ((actor1->category == ACTOR_BLUE_SPINY_SHELL) && (actor2->category == ACTOR_BLUE_SPINY_SHELL)) {
            KillObject(actor1);
            KillObject(actor2);
            actor1->flag |= SEKAMEHITOBJ;
            NAEnmTrgStart(actor1->position, actor1->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
            return;
        }
        if (actor1->category == ACTOR_BLUE_SPINY_SHELL) {
            if (actor1->sparam == HELD_SHELL) {
                KillObject(actor1);
            }
        } else {
            KillObject(actor1); // automatically destroy if it's something different of a blueshell
        }
        if (actor2->category == ACTOR_BLUE_SPINY_SHELL) {
            if (actor2->sparam == HELD_SHELL) {
                KillObject(actor2);
            }
        } else {
            KillObject(actor2); // automatically destroy if it's something different of a blueshell
        }
        check_crush_sound(actor1, actor2);
    }
}

void collision_check_routin(Player* player, Object* actor) {
    UNUSED s32 pad;
    s16 temp_lo;
    UNUSED s32 pad2[2];
    s16 temp_v1;
    Player* owner;
    f32 temp_f0;
    f32 temp_f2;

    temp_lo = player - gPlayerOne;
    switch (actor->category) {
        case ACTOR_YOSHI_EGG:
            if (!(player->slip_flag & TERESA) && !(player->flag & IS_GHOST)) {
                collision_egg(player, (struct YoshiValleyEgg*) actor);
            }
            break;
        case ACTOR_BANANA:
            if (player->slip_flag &
                (TERESA | WHEELSPIN | SPIN_L | SPIN_R)) {
                break;
            }
            if (player->weapon & HIT_BANANA) {
                break;
            }
            temp_v1 = actor->angle[0];
            if (((temp_lo == temp_v1) && (actor->flag & MYOBJ)) ||
                (CollisionSphere(player, actor) != COLLISION)) {
                break;
            }
            player->weapon |= HIT_BANANA;
            owner = &gPlayers[temp_v1];
            if (owner->flag & IS_PLAYER) {
                if (actor->flag & 0xF) {
                    if (temp_lo != temp_v1) {
                        NAPlyVoiceStart(temp_v1, (owner->kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x06));
                    }
                } else {
                    temp_f0 = actor->position[0] - owner->position[0];
                    temp_f2 = actor->position[2] - owner->position[2];
                    if ((((temp_f0 * temp_f0) + (temp_f2 * temp_f2)) < 360000.0f) && (temp_lo != temp_v1)) {
                        NAPlyVoiceStart(temp_v1, (owner->kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x06));
                    }
                }
            }
            KillObject(actor);
            break;
        case ACTOR_GREEN_SHELL:
            if (player->slip_flag & (TERESA | ROLLOVER)) {
                break;
            }
            if (player->weapon & HIT_GREENSHELL) {
                break;
            }
            temp_v1 = actor->angle[2];
            if (((temp_lo == temp_v1) && (actor->flag & MYOBJ)) ||
                (CollisionSphere(player, actor) != COLLISION)) {
                break;
            }
            player->weapon |= HIT_GREENSHELL;
            NAEnmTrgStart(player->position, player->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
            owner = &gPlayers[temp_v1];
            if ((owner->flag & IS_PLAYER) && (temp_lo != temp_v1)) {
                NAPlyVoiceStart(temp_v1, (owner->kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x06));
            }
            KillObject(actor);
            break;
        case ACTOR_BLUE_SPINY_SHELL:
            if (player->weapon & HIT_REDSHELL) {
                break;
            }
            temp_v1 = actor->angle[2];
            if (((temp_lo == temp_v1) && (actor->flag & MYOBJ)) ||
                (CollisionSphere(player, actor) != COLLISION)) {
                break;
            }
            if (!(player->slip_flag & TERESA)) {
                player->weapon |= HIT_REDSHELL;
                NAEnmTrgStart(player->position, player->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
            }
            owner = &gPlayers[temp_v1];
            if ((owner->flag & IS_PLAYER) && (temp_lo != temp_v1)) {
                NAPlyVoiceStart(temp_v1, (owner->kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x06));
            }
            if (temp_lo == actor->counter) {
                KillObject(actor);
            }
            break;
        case ACTOR_RED_SHELL:
            temp_v1 = actor->angle[2];
            if (player->slip_flag & EXPLODE) {
                break;
            }
            if (player->weapon & HIT_REDSHELL) {
                break;
            }
            temp_v1 = actor->angle[2];
            if (((temp_lo == temp_v1) && (actor->flag & MYOBJ)) ||
                (CollisionSphere(player, actor) != COLLISION)) {
                break;
            }
            if (!(player->slip_flag & TERESA)) {
                player->weapon |= HIT_REDSHELL;
                NAEnmTrgStart(player->position, player->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x10));
            }
            owner = &gPlayers[temp_v1];
            if ((owner->flag & IS_PLAYER) && (temp_lo != temp_v1)) {
                NAPlyVoiceStart(temp_v1, (owner->kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x06));
            }
            KillObject(actor);
            break;
        case ACTOR_PIRANHA_PLANT:
            if (!(player->slip_flag & TERESA)) {
                collision_pakkun(player, (PakkunObject*) actor);
            }
            break;
        case ACTOR_MARIO_SIGN:
            if (!(player->slip_flag & TERESA)) {
                collision_mkanban(player, actor);
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
            if (!(player->slip_flag & TERESA)) {
                collision_tree(player, actor);
            }
            break;
        case ACTOR_FALLING_ROCK:
            if (!(player->slip_flag & TERESA) && !(player->flag & IS_GHOST)) {
                if (CollisionSphere(player, actor) == COLLISION) {
                    NAEnmTrgStart(actor->position, actor->velocity, SOUND_ACTION_EXPLOSION);
                    if ((g_gameMode == TIME_TRIALS) && !(player->flag & IS_CPU_PLAYER)) {
                        lost_ghost = 1;
                    }
                    if (player->slip_flag & STAR) {
                        actor->velocity[1] = 10.0f;
                    } else {
                        SetBroken(player, player - gPlayerOne);
                    }
                }
            }
            break;
        case ACTOR_FAKE_ITEM_BOX:
            temp_v1 = actor->velocity[0];
            if (player->slip_flag & TERESA) {
                break;
            }
            temp_v1 = actor->velocity[0];
            if (((temp_lo == temp_v1) && (actor->flag & MYOBJ)) ||
                (CollisionSphere(player, actor) != COLLISION)) {
                break;
            }
            player->weapon |= HIT_BOMB;
            owner = &gPlayers[temp_v1];
            if (owner->flag & IS_PLAYER) {
                if (actor->flag & 0xF) {
                    if (temp_lo != temp_v1) {
                        NAPlyVoiceStart(temp_v1, (owner->kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x06));
                    }
                } else {
                    temp_f0 = actor->position[0] - owner->position[0];
                    temp_f2 = actor->position[2] - owner->position[2];
                    if ((((temp_f0 * temp_f0) + (temp_f2 * temp_f2)) < 360000.0f) && (temp_lo != temp_v1)) {
                        NAPlyVoiceStart(temp_v1, (owner->kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x06));
                    }
                }
                if (actor->sparam == 0) {
                    owner->weapon &= ~USE_BANANA;
                }
            }
            actor->sparam = 2;
            actor->flag = -0x8000;
            actor->counter = 0;
            break;
        case ACTOR_HOT_AIR_BALLOON_ITEM_BOX:
            if (CollisionSphere(player, actor) == COLLISION) {
                actor->sparam = 3;
                actor->flag = -0x8000;
                actor->counter = 0;
                if (player->flag & IS_PLAYER) {
                    RouletteStart(player - gPlayerOne, 7);
                }
            } else if (actor->sparam == 0) {
                actor->sparam = 1;
                actor->flag = -0x8000;
            }
            break;
        case ACTOR_ITEM_BOX:
            //Handled under CollideObjectDefault
            /*
            if (CollisionSphere(player, actor) == COLLISION) {
                actor->sparam = 3;
                actor->flag = -0x8000;
                actor->counter = 0;
                if (player->flag & IS_PLAYER) {
                    RouletteStart(player - gPlayerOne, 0);
                }
            } else if (actor->sparam == 0) {
                actor->sparam = 1;
                actor->flag = -0x8000;
            }
            */
            break;
        default:
            CollideObjectDefault(player, actor);
            break;
    }
}

void collision_object_to_kart(void) {
    Object* temp_a1;
    s32 i, j;
    Player* phi_s1;

    for (i = 0; i < NUM_PLAYERS; i++) {
        phi_s1 = &gPlayers[i];

        if (((phi_s1->flag & EXISTS) != 0) && ((phi_s1->slip_flag & BROKEN) == 0)) {
            reset_shadow_flag(phi_s1);
            for (j = 0; j < ACTOR_LIST_SIZE; j++) {
                temp_a1 = &g_SimpleObjectArray[j];

                if ((phi_s1->slip_flag & BROKEN) == 0) {
                    // temp_v0 = temp_a1->unk2;
                    if (((temp_a1->flag & EXISTOBJ) != 0) && ((temp_a1->flag & HITOBJ) != 0)) {
                        collision_check_routin(phi_s1, temp_a1);
                    }
                }
            }
        }
    }
}

// It's look like to check collision between item and other different item
void collision_object_to_object(void) {
    Object* actor1;
    Object* actor2;
    s32 i, j;
    UNUSED s32 pad;

    for (i = g_StaticObjectCount; i < (ACTOR_LIST_SIZE - 1); i++) {
        actor1 = &g_SimpleObjectArray[i];

        if ((actor1->flag & EXISTOBJ) == 0) {
            continue;
        }
        if ((actor1->flag & HITOBJ) == 0) {
            continue;
        }

        switch (actor1->category) {
            case ACTOR_BANANA:
            case ACTOR_GREEN_SHELL:
            case ACTOR_RED_SHELL:
            case ACTOR_BLUE_SPINY_SHELL:
            case ACTOR_FAKE_ITEM_BOX:

                for (j = i + 1; j < ACTOR_LIST_SIZE; j++) {
                    actor2 = &g_SimpleObjectArray[j];

                    if ((actor1->flag & EXISTOBJ) == 0) {
                        continue;
                    }
                    if ((actor1->flag & HITOBJ) == 0) {
                        continue;
                    }

                    if ((actor2->flag & EXISTOBJ) == 0) {
                        continue;
                    }
                    if ((actor2->flag & HITOBJ) == 0) {
                        continue;
                    }

                    switch (actor2->category) {
                        case ACTOR_BANANA:
                            if (actor1->category == ACTOR_BANANA) {
                                continue;
                            }
                            collision_check_routin2(actor1, actor2);
                            break;
                        case ACTOR_GREEN_SHELL:
                            if (actor1->category == ACTOR_GREEN_SHELL) {
                                if (actor1->angle[2] == actor2->angle[2]) {
                                    continue;
                                }
                            }
                            collision_check_routin2(actor1, actor2);
                            break;
                        case ACTOR_RED_SHELL:
                            if (actor1->category == ACTOR_RED_SHELL) {
                                if (actor1->angle[2] == actor2->angle[2]) {
                                    continue;
                                }
                            }
                            collision_check_routin2(actor1, actor2);
                            break;
                        case ACTOR_BLUE_SPINY_SHELL:
                        case ACTOR_FAKE_ITEM_BOX:
                            collision_check_routin2(actor1, actor2);
                            break;
                    }
                }

                break;
        }
    }
}

void release_fake_itembox(struct FakeItemBox* fake_item_box) {
    if ((u32) (fake_item_box - (struct FakeItemBox*) g_SimpleObjectArray) <= (u32) ACTOR_LIST_SIZE) {
        if (((fake_item_box->flag & EXISTOBJ) != 0) && (fake_item_box->category == ACTOR_FAKE_ITEM_BOX)) {
            fake_item_box->sparam = FAKE_ITEM_BOX_ON_GROUND;
            fake_item_box->targetY = get_hight(&fake_item_box->bump, fake_item_box->position) + 8.66f;
            fake_item_box->someTimer = 100;
        }
    }
}

#include "actors/fake_item_box/update.inc.c"

void initial_special_itembox(f32 x, f32 y, f32 z) {
    Vec3f pos;
    Vec3f velocity;
    Vec3s rot;
    s16 id;

    if (g_gameMode == TIME_TRIALS) {
        return;
    }

    SetSVector(rot, 0, 0, 0);
    vec3f_set(velocity, 0, 0, 0);
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    id = addObjectBuffer(pos, rot, velocity, ACTOR_HOT_AIR_BALLOON_ITEM_BOX);
    gActorHotAirBalloonItemBox = &g_SimpleObjectArray[id];
}

#include "actors/item_box/update.inc.c"

#include "actors/fake_item_box/render.inc.c"

#include "actors/item_box/render.inc.c"

#include "actors/wario_sign/render.inc.c"

#include "actors/yoshi_egg/render.inc.c"

#include "actors/mario_sign/render.inc.c"

#include "actors/railroad_crossing/render.inc.c"

#include "actors/palm_tree/render.inc.c"

void display_object2(Screen* arg0) {
    Camera* camera = arg0->camera;
    Object* actor;
    s32 i;
    itembox_flag = 0;

    for (i = 0; i < ACTOR_LIST_SIZE; i++) {
        actor = &g_SimpleObjectArray[i];

        if (actor->flag == 0) {
            continue;
        }

        switch (actor->category) {
            case ACTOR_FAKE_ITEM_BOX:
                display_fake_itembox(camera, (struct FakeItemBox*) actor);
                break;
            case ACTOR_ITEM_BOX:
                display_itembox(camera, (struct ItemBox*) actor);
                break;
            case ACTOR_HOT_AIR_BALLOON_ITEM_BOX:
                display_itembox(camera, (struct ItemBox*) actor);
                break;
        }
    }
}

void display_object(Screen* arg0) {
    Camera* camera = arg0->camera;
    u16 pathCounter = arg0->camera_point;
    UNUSED s32 pad[12];
    s32 i;

    Object* actor;
    UNUSED Vec3f sp4C = { 0.0f, 5.0f, 10.0f };
    f32 sp48 = sinT(camera->camera_direction[1] - DEGREES(180)); // unk26;
    f32 temp_f0 = cosT(camera->camera_direction[1] - DEGREES(180));

    gAffine[0][0] = temp_f0;
    gAffine[0][2] = -sp48;
    gAffine[2][2] = temp_f0;
    gAffine[1][0] = 0.0f;
    gAffine[0][1] = 0.0f;
    gAffine[2][1] = 0.0f;
    gAffine[1][2] = 0.0f;
    gAffine[0][3] = 0.0f;
    gAffine[1][3] = 0.0f;
    gAffine[2][3] = 0.0f; // 2c
    gAffine[2][0] = sp48;
    gAffine[1][1] = 1.0f;
    gAffine[3][3] = 1.0f; // unk3c

    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPSetLights1(gDisplayListHead++, D_800DC610[1]);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);

    if (g_gameMode != BATTLE) {
        DisplayFlagGateCheck(camera);
    }
    tree_flag = 0;

    for (i = 0; i < ACTOR_LIST_SIZE; i++) {
        actor = &g_SimpleObjectArray[i];

        if (actor->flag == 0) {
            continue;
        }
        switch (actor->category) {
            case ACTOR_TREE_MARIO_RACEWAY:
                DisplayTree1Bypass(camera, gAffine, actor);
                break;
            case ACTOR_TREE_YOSHI_VALLEY:
                display_tree2(camera, gAffine, actor);
                break;
            case ACTOR_TREE_ROYAL_RACEWAY:
                display_tree3(camera, gAffine, actor);
                break;
            case ACTOR_TREE_MOO_MOO_FARM:
                display_tree4(camera, gAffine, actor);
                break;
            case ACTOR_UNKNOWN_0x1A:
                display_tree5(camera, gAffine, actor);
                break;
            case ACTOR_TREE_BOWSERS_CASTLE:
                display_tree7(camera, gAffine, actor);
                break;
            case ACTOR_BUSH_BOWSERS_CASTLE:
                display_tree8(camera, gAffine, actor);
                break;
            case ACTOR_TREE_FRAPPE_SNOWLAND:
                display_snowtree(camera, gAffine, actor);
                break;
            case ACTOR_CACTUS1_KALAMARI_DESERT:
                display_saboten1(camera, gAffine, actor);
                break;
            case ACTOR_CACTUS2_KALAMARI_DESERT:
                display_saboten2(camera, gAffine, actor);
                break;
            case ACTOR_CACTUS3_KALAMARI_DESERT:
                display_saboten3(camera, gAffine, actor);
                break;
            case ACTOR_FALLING_ROCK:
                display_iwa(camera, (struct FallingRock*) actor);
                break;
            case ACTOR_KIWANO_FRUIT:
                display_nuts(camera, gAffine, actor);
                break;
            case ACTOR_BANANA:
                display_banana(camera, gAffine, (struct BananaActor*) actor);
                break;
            case ACTOR_GREEN_SHELL:
                display_gshell(camera, gAffine, (struct ShellActor*) actor);
                break;
            case ACTOR_RED_SHELL:
                display_rshell(camera, gAffine, (struct ShellActor*) actor);
                break;
            case ACTOR_BLUE_SPINY_SHELL:
                display_tshell(camera, gAffine, (struct ShellActor*) actor);
                break;
            case ACTOR_PIRANHA_PLANT:
                DisplayPiranhaBypass(camera, gAffine, actor);
                break;
            case ACTOR_TRAIN_ENGINE:
                display_sl1(camera, (struct TrainCar*) actor);
                break;
            case ACTOR_TRAIN_TENDER:
                display_sl2(camera, (struct TrainCar*) actor);
                break;
            case ACTOR_TRAIN_PASSENGER_CAR:
                display_sl3(camera, (struct TrainCar*) actor);
                break;
            case ACTOR_COW:
                display_cow(camera, gAffine, actor);
                break;
            case ACTOR_UNKNOWN_0x14:
                display_pylon(camera, gAffine, actor);
                break;
            case ACTOR_MARIO_SIGN:
                display_mkanban(camera, gAffine, actor);
                break;
            case ACTOR_WARIO_SIGN:
                display_wkanban(camera, actor);
                break;
            case ACTOR_PALM_TREE:
                display_palmtree(camera, gAffine, (struct PalmTree*) actor);
                break;
            case ACTOR_PADDLE_BOAT:
                display_ship(camera, (struct PaddleWheelBoat*) actor, gAffine, pathCounter);
                break;
            case ACTOR_BOX_TRUCK:
                display_truck1(camera, actor);
                break;
            case ACTOR_SCHOOL_BUS:
                display_bus(camera, actor);
                break;
            case ACTOR_TANKER_TRUCK:
                display_tanklorry(camera, actor);
                break;
            case ACTOR_CAR:
                display_rv(camera, actor);
                break;
            case ACTOR_RAILROAD_CROSSING:
                display_fumikiri(camera, (struct RailroadCrossing*) actor);
                break;
            case ACTOR_YOSHI_EGG:
                display_egg(camera, gAffine, (struct YoshiValleyEgg*) actor, pathCounter);
                break;
            default:
                DisplayObjectDefault(camera, actor);
                break;
        }
    }
    switch (g_courseID) {
        case COURSE_MOO_MOO_FARM:
            display_farm_cow(camera, gAffine, actor);
            break;
        case COURSE_DK_JUNGLE:
            display_jungle_tree(camera, gAffine, actor);
            break;
    }
    DrawPerScreenDefault(camera);
}

void object_strategy(void) {
    Object* actor;
    s32 i;
    for (i = 0; i < ACTOR_LIST_SIZE; i++) {

        actor = &g_SimpleObjectArray[i];
        if (actor->flag == 0) {
            continue;
        }

        switch (actor->category) {
            case ACTOR_FALLING_ROCK:
                update_actor_falling_rocks((struct FallingRock*) actor);
                break;
            case ACTOR_GREEN_SHELL:
                move_gshell((struct ShellActor*) actor);
                break;
            case ACTOR_RED_SHELL:
                move_rshell((struct ShellActor*) actor);
                break;
            case ACTOR_BLUE_SPINY_SHELL:
                move_rshell((struct ShellActor*) actor);
                break;
            case ACTOR_KIWANO_FRUIT:
                nuts_strategy((struct KiwanoFruit*) actor);
                break;
            case ACTOR_BANANA:
                move_banana((struct BananaActor*) actor);
                break;
            case ACTOR_PADDLE_BOAT:
                ship_strategy((struct PaddleWheelBoat*) actor);
                break;
            case ACTOR_TRAIN_ENGINE:
                sl1_strategy((struct TrainCar*) actor);
                break;
            case ACTOR_TRAIN_TENDER:
                sl2_strategy((struct TrainCar*) actor);
                break;
            case ACTOR_TRAIN_PASSENGER_CAR:
                sl3_strategy((struct TrainCar*) actor);
                break;
            case ACTOR_ITEM_BOX:
                move_itembox((struct ItemBox*) actor);
                break;
            case ACTOR_HOT_AIR_BALLOON_ITEM_BOX:
                move_special_itembox((struct ItemBox*) actor);
                break;
            case ACTOR_FAKE_ITEM_BOX:
                move_fake_itembox((struct FakeItemBox*) actor);
                break;
            case ACTOR_PIRANHA_PLANT:
                PakkunStrategyOverride(actor);
                break;
            case ACTOR_BANANA_BUNCH:
                super_banana_strategy((struct BananaBunchParent*) actor);
                break;
            case ACTOR_TRIPLE_GREEN_SHELL:
                super_shell_strategy((TripleShellParent*) actor, ACTOR_GREEN_SHELL);
                break;
            case ACTOR_TRIPLE_RED_SHELL:
                super_shell_strategy((TripleShellParent*) actor, ACTOR_RED_SHELL);
                break;
            case ACTOR_MARIO_SIGN:
                mkanban_strategy(actor);
                break;
            case ACTOR_WARIO_SIGN:
                wkanban_strategy(actor);
                break;
            case ACTOR_RAILROAD_CROSSING:
                fumikiri_strategy((struct RailroadCrossing*) actor);
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
                tree_strategy(actor);
                break;
            case ACTOR_YOSHI_EGG:
                egg_strategy((struct YoshiValleyEgg*) actor);
                break;
        }
    }
    collision_object_to_object();
    item_check();
}

#include <ultra64.h>
#include "cpu_vehicles_camera_path.h"
#include <defines.h>
#include <macros.h>

void enemy_item_init(void) {
    s32 var_v1;

    for (var_v1 = 0; var_v1 < NUM_PLAYERS; var_v1++) {
        enemy_item[var_v1].item_num = 0;
        enemy_item[var_v1].time = 0;
        enemy_item[var_v1].obj_num = -1;
        enemy_item[var_v1].limit = 0;
        enemy_item[var_v1].count1 = 0;
    }
}

void enemy_item_number_set(UNUSED s32 playerId, s16* branch, s32 itemId) {
    s32 value = -1;
    switch (itemId) {
        case ITEM_FAKE_ITEM_BOX:
            value = CPU_STRATEGY_ITEM_FAKE_ITEM_BOX;
            break;
        case ITEM_BOO:
            value = CPU_STRATEGY_ITEM_BOO;
            break;
        case ITEM_BANANA:
            value = CPU_STRATEGY_ITEM_BANANA;
            break;
        case ITEM_THUNDERBOLT:
            value = CPU_STRATEGY_ITEM_THUNDERBOLT;
            break;
        case ITEM_STAR:
            value = CPU_STRATEGY_ITEM_STAR;
            break;
        case ITEM_MUSHROOM:
            value = CPU_STRATEGY_ITEM_MUSHROOM;
            break;
        case ITEM_DOUBLE_MUSHROOM:
            break;
        case ITEM_TRIPLE_MUSHROOM:
            break;
        case ITEM_SUPER_MUSHROOM:
            break;
    }
    if (value >= 0) {
        *branch = value;
    }
}

void enemy_item_debug_BANDOU(UNUSED s32 playerId, UNUSED EnemyItem* arg1) {
}

void check_item_number(EnemyItem* arg0) {
    if ((arg0->obj_num < 0) || (arg0->obj_num >= 0x64)) {
        arg0->item_num = 0;
        arg0->time = 0;
    }
}

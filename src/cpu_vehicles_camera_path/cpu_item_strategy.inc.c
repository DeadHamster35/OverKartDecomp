#include <ultra64.h>
#include "cpu_vehicles_camera_path.h"
#include <defines.h>
#include <macros.h>

void EnemyItemInit(void) {
    s32 var_v1;

    for (var_v1 = 0; var_v1 < NUM_PLAYERS; var_v1++) {
        EnemyItemTable[var_v1].ItemNum = 0;
        EnemyItemTable[var_v1].Time = 0;
        EnemyItemTable[var_v1].ObjNum = -1;
        EnemyItemTable[var_v1].Limit = 0;
        EnemyItemTable[var_v1].Count1 = 0;
    }
}

void EnemyItemNumberSet(UNUSED s32 PlayerId, s16* branch, s32 itemId) {
    s32 Value = -1;
    switch (itemId) {
        case ITEM_FAKE_ITEM_BOX:
            Value = CPU_STRATEGY_ITEM_FAKE_ITEM_BOX;
            break;
        case ITEM_BOO:
            Value = CPU_STRATEGY_ITEM_BOO;
            break;
        case ITEM_BANANA:
            Value = CPU_STRATEGY_ITEM_BANANA;
            break;
        case ITEM_THUNDERBOLT:
            Value = CPU_STRATEGY_ITEM_THUNDERBOLT;
            break;
        case ITEM_STAR:
            Value = CPU_STRATEGY_ITEM_STAR;
            break;
        case ITEM_MUSHROOM:
            Value = CPU_STRATEGY_ITEM_MUSHROOM;
            break;
        case ITEM_DOUBLE_MUSHROOM:
            break;
        case ITEM_TRIPLE_MUSHROOM:
            break;
        case ITEM_SUPER_MUSHROOM:
            break;
    }
    if (Value >= 0) {
        *branch = Value;
    }
}

void EnemyItemDebugBANDOU(UNUSED s32 PlayerId, UNUSED EnemyItem* arg1) {
}

void CheckItemNumber(EnemyItem* arg0) {
    if ((arg0->ObjNum < 0) || (arg0->ObjNum >= 0x64)) {
        arg0->ItemNum = 0;
        arg0->Time = 0;
    }
}

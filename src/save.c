#include <ultra64.h>
#include <macros.h>
#include <defines.h>
#include "stddef.h"

#include "save.h"
#include "OverKartHooks.h"

#include "menu_items.h"
#include "menus.h"
#include "save_data.h"
#include "replays.h"
#include "code_80057C60.h"

/*** macros ***/
#define PFS_COMPANY_CODE(c0, c1) ((u16) (((c0) << 8) | ((c1))))
#define PFS_GAME_CODE(c0, c1, c2, c3) ((u32) (((c0) << 24) | ((c1) << 16) | ((c2) << 8) | (c3)))
// calculate an eeprom address based off of the ram address of the SaveData variable
// very fragile!
#define EEPROM_ADDR(ptr) (((uintptr_t) (ptr) - (uintptr_t) (&eeprom_save_data)) / 8)

/** BSS **/
// stores ghost data
struct_8018EE10_entry D_8018EE10[2];

/*** data ***/
u16 gCompanyCode = PFS_COMPANY_CODE('0', '1');
u32 gGameCode = PFS_GAME_CODE('N', 'K', 'T', 'J');
s8 gControllerPak1State = BAD;
s8 sControllerPak2State = BAD;

/*** rodata ***/
// default time trial records in little endian form
const u8 D_800F2E60[4] = { 0xc0, 0x27, 0x09, 0x00 };
// osPfsFindFile -> gGameName ("MARIOKART64" in nosFont)
const u8 gGameName[] = {
    0x26, 0x1a, 0x2b, 0x22, 0x28, 0x24, 0x1a, 0x2b, 0x2d, 0x16, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00
};
// ext_name param to osPfsFindFile (four total bytes, but only one is setable)
const u8 gExtCode[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// new file start?
void SaveFunc800B45E0(s32 arg0) {
    CourseTimeTrialRecords* courseTimeTrialRecordsPtr =
        &eeprom_save_data.allCourseTimeTrialRecords.cupRecords[arg0 / 4].courseRecords[arg0 % 4];

    courseTimeTrialRecordsPtr->checksum = GetCheckSum_Course(arg0);
    osEepromLongWrite(&gSIEventMesgQueue, EEPROM_ADDR(courseTimeTrialRecordsPtr), (u8*) courseTimeTrialRecordsPtr,
                      sizeof(CourseTimeTrialRecords));
}

void SaveFunc800B4670(void) {
    Stuff* main = &eeprom_save_data.main;
    main->checksum[1] = GetCheckSum1_flgs1();
    main->checksum[2] = GetCheckSum2_flgs1();
    osEepromLongWrite(&gSIEventMesgQueue, EEPROM_ADDR(main), (u8*) main, sizeof(Stuff));
}

void initEEPRom(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        initEEPRomCourseData(i);
        SaveFunc800B559CDefault(i);
    }

    initEEPRomFlgs1Data();
    SaveFunc800B5948Default();
}

void initEEPRomCourseData(s32 arg0) {
    s32 i, j;
    CourseTimeTrialRecords* courseTimeTrialRecords;

    courseTimeTrialRecords = &eeprom_save_data.allCourseTimeTrialRecords.cupRecords[arg0 / 4].courseRecords[arg0 % 4];

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 3; j++) {
            courseTimeTrialRecords->records[i][j] = D_800F2E60[j];
        }
    }

    for (i = 0; i < 3; i++) {
        courseTimeTrialRecords->records[5][i] = D_800F2E60[i];
    }

    courseTimeTrialRecords->unknownBytes[0] = 0;
    courseTimeTrialRecords->checksum = GetCheckSum_Course(arg0);
    SaveFunc800B45E0Default(arg0);
}

void initEEPRomFlgs1Data(void) {
    s32 cup_index;
    Stuff* main = &eeprom_save_data.main;
    for (cup_index = 0; cup_index < 4; cup_index++) {
        main->saveInfo.grandPrixPoints[cup_index] = 0;
    }
    main->saveInfo.soundMode = SOUND_STEREO;
    sound_output = SOUND_STEREO;
    Setting_SoundOutMode();
    SaveFunc800B4670Default();
}

// create a magic number based on the time trial records
u8 GetCheckSum_Course(s32 courseIdx) {
    s32 j;
    s32 i;
    s32 ret;
    u8* records = eeprom_save_data.allCourseTimeTrialRecords.cupRecords[courseIdx / 4].courseRecords[courseIdx % 4].records[0];

    ret = 0;
    for (i = 0; i < 7; i++) {
        for (j = 0; j < 3; j++) {
            ret += *(records + i * 3 + j) * (j + 1) + i;
        }
    }

    return ret % 256;
}

u8 GetCheckSum1_flgs1(void) {
    u8* grandPrixPoints = (u8*) &eeprom_save_data.main.saveInfo.grandPrixPoints;
    u32 i;
    s32 crc = 0;

    for (i = 0; i < sizeof(SaveInfo); i++) {
        crc += ((grandPrixPoints[i] + 1) * (i + 1)) + i;
    }

    return crc % 0x100;
}

u8 GetCheckSum2_flgs1(void) {
    s32 tmp = eeprom_save_data.main.checksum[1] + 90;
    return (tmp % 256);
}

void LoadFunc800B4A10(void) {
    s32 i;

    osEepromLongRead(&gSIEventMesgQueue, EEPROM_ADDR(&eeprom_save_data), (u8*) &eeprom_save_data, sizeof(SaveData));
    // 16: 4 cup records * 4 course records?
    for (i = 0; i < 16; i++) {
        CheckEEPRomCourseData(i);
    }

    SaveFunc800B4CB4();

    sound_output = eeprom_save_data.main.saveInfo.soundMode;
    if (sound_output >= NUM_SOUND_MODES) {
        sound_output = SOUND_MONO;
    }
}

void CheckEEPRomCourseData(s32 course) {
    OnlyBestTimeTrialRecords* test;
    CourseTimeTrialRecords* sp24;
    s32 i;

    if ((GetRecordTime3(0, course) & 0xFFFFF) < 0x927C0U) {
        eeprom_save_data.allCourseTimeTrialRecords.cupRecords[course / 4].courseRecords[course % 4].unknownBytes[0] = 1;
    }
    sp24 = &eeprom_save_data.allCourseTimeTrialRecords.cupRecords[course / 4].courseRecords[course % 4];

    GetRecordLapTime3(course);
    if (sp24) {}

    if (sp24->checksum != GetCheckSum_Course(course)) {
        initEEPRomCourseData(course);
        if (CheckEEPRomCourseData_bak(course) == 0) {
            s32 a3 = 0;

            test = &eeprom_save_data.onlyBestTimeTrialRecords[course / 8];
            for (i = 0; i < 3; i++) {
                sp24->records[TIME_TRIAL_3LAP_RECORD_1][i] = test->bestThreelaps[course % 8][i];
                sp24->records[TIME_TRIAL_1LAP_RECORD][i] = test->bestSinglelaps[course % 8][i];

                // This is checking (in a roundabout way) if the given record
                // is the default value of 0x927C0
                if (sp24->records[TIME_TRIAL_3LAP_RECORD_1][i] == D_800F2E60[i]) {
                    if (sp24->records[TIME_TRIAL_1LAP_RECORD][i] == D_800F2E60[i]) {
                        a3 += 1;
                    }
                }
            }

            if (a3 == 3) {
                sp24->unknownBytes[0] = 0;
            } else {
                sp24->unknownBytes[0] = 1;
            }
            SaveFunc800B45E0Default(course);
        }
        // L800B4C78
        SaveFunc800B559CDefault(course);
    } else if (CheckEEPRomCourseData_bak(course)) {
        // L800B4C88
        SaveFunc800B559CDefault(course);
    }
}

void SaveFunc800B4CB4(void) {
    s32 cup_index;
    Stuff* main = &eeprom_save_data.main;
    Stuff* backup = &eeprom_save_data.backup;
    if (main->checksum[1] != (GetCheckSum1_flgs1()) ||
        (main->checksum[2] != GetCheckSum2_flgs1())) {
        initEEPRomFlgs1Data();

        if (CheckEEPRomFlgs1_bak() == 0) {
            for (cup_index = 0; cup_index < 4; cup_index++) {
                main->saveInfo.grandPrixPoints[cup_index] = backup->saveInfo.grandPrixPoints[cup_index];
            }

            main->saveInfo.soundMode = backup->saveInfo.soundMode;
            main->checksum[1] = GetCheckSum1_flgs1_bak();
            main->checksum[2] = GetCheckSum2_flgs1_bak();
            osEepromLongWrite(&gSIEventMesgQueue, EEPROM_ADDR(main), (u8*) main, sizeof(Stuff));
        }
        SaveFunc800B5948Default();
        return;
    }

    if (CheckEEPRomFlgs1_bak() != 0) {
        SaveFunc800B5948Default();
    }
}

void SetRecordTime(u8* timeTrialRecord, u32 time, s32 characterId) {
    u32 timeRightShift8 = time >> 8;
    u32 timeRightShift16 = timeRightShift8 >> 8;
    s16 timeRightShift8Duplicate;
    u16 timeRightShift16Duplicate;

    timeRightShift16Duplicate = timeRightShift16;

    timeTrialRecord[0] = time & 0xFF;
    timeTrialRecord[1] = (timeRightShift8Duplicate = timeRightShift8);
    timeTrialRecord[2] = (timeRightShift16Duplicate & 0xF) + ((characterId & 7) << 4);
}

// combine time trial record u8[3] into the lower 24 bits of a word [xx221100]
u32 GetRecordTime(u8* arr) {
    s32 a, b, c;
    a = arr[0];
    b = arr[1];
    c = arr[2];

    return (a + (b << 8) + (c << 16)) & 0x00FFFFFF;
}

// Get a time trial record, infer course index
s32 GetRecordTime2(s32 recordIndex) {
    return GetRecordTime(eeprom_save_data.allCourseTimeTrialRecords.cupRecords[(((g_cupSelect * 4) + g_courseSelect) / 4)]
                             .courseRecords[(((g_cupSelect * 4) + g_courseSelect) % 4)]
                             .records[recordIndex]);
}

// Get a time trial record, but take the course index as an argument
u32 GetRecordTime3(s32 recordIndex, s32 courseIndex) {
    return GetRecordTime(eeprom_save_data.allCourseTimeTrialRecords.cupRecords[(courseIndex / 4)]
                             .courseRecords[(courseIndex % 4)]
                             .records[recordIndex]);
}

// Get Best Lap record of the inferred course index
s32 GetRecordLapTime(void) {
    return GetRecordTime(eeprom_save_data.allCourseTimeTrialRecords.cupRecords[(((g_cupSelect * 4) + g_courseSelect) / 4)]
                             .courseRecords[(((g_cupSelect * 4) + g_courseSelect) % 4)]
                             .records[TIME_TRIAL_1LAP_RECORD]);
}

// Get the best single lap time record of the given course index
s32 GetRecordLapTime3(s32 courseIndex) {
    return GetRecordTime(eeprom_save_data.allCourseTimeTrialRecords.cupRecords[(courseIndex / 4)]
                             .courseRecords[(courseIndex % 4)]
                             .records[TIME_TRIAL_1LAP_RECORD]);
}

s32 SaveCourseRecord(u32 time, s32 charId) {
    UNUSED s32 stackPadding[3];
    s32 course; // sp30
    s32 i;
    s32 j;
    CourseTimeTrialRecords* tt;

    course = g_cupSelect * 4 + g_courseSelect;
    tt = &eeprom_save_data.allCourseTimeTrialRecords.cupRecords[course / 4].courseRecords[course % 4];

    i = 0;
    for (; i < 5; i++) {
        if (time < (GetRecordTime(tt->records[i]) & 0x000FFFFF)) {
            break;
        }
    }

    // `time` is:
    //     Not less than any records
    //     Only less than the 1-lap record
    //  Either way, we aren't going to update any records
    if (i >= 5) {
        return -1;
    }

    for (j = TIME_TRIAL_3LAP_RECORD_5; j > i; j--) {
        tt->records[j][0] = tt->records[j - 1][0];
        tt->records[j][1] = tt->records[j - 1][1];
        tt->records[j][2] = tt->records[j - 1][2];
    }

    SetRecordTime(tt->records[i], time, charId);
    tt->unknownBytes[0] = 1;
    SaveFunc800B45E0Default(course);

    return i;
}

s32 SaveCourseLapRecord(void) {
    u8* recordPointer;
    UNUSED s32 pad;
    s32 fastestLapIndex;
    s32 recordIndex;
    UNUSED s32 pad2[2];
    s32 checkLapIndex;
    s32 character;
    s32 lapBitmask;
    recordIndex = (g_cupSelect * 4) + g_courseSelect;
    recordPointer =
        &eeprom_save_data.allCourseTimeTrialRecords.cupRecords[recordIndex / 4].courseRecords[recordIndex % 4].records[0][0];
    lapBitmask = 1;
    fastestLapIndex = 0;
    character = *g_CharacterSelections;
    for (checkLapIndex = 1; checkLapIndex != 3; checkLapIndex++) {
        if ((s32)playerHUD->laptimebuf[checkLapIndex] < (s32)playerHUD->laptimebuf[fastestLapIndex]) {
            lapBitmask = 1 << checkLapIndex;
            fastestLapIndex = checkLapIndex;
        } else if ((s32)playerHUD->laptimebuf[fastestLapIndex] == (s32)playerHUD->laptimebuf[checkLapIndex]) {
            lapBitmask |= 1 << checkLapIndex;
        }
    }

    if (playerHUD->laptimebuf[fastestLapIndex] < (GetRecordLapTime() & 0xFFFFF)) {
        SetRecordTime(recordPointer + 0xF, playerHUD->laptimebuf[fastestLapIndex], character);
        recordPointer[0x12] = 1;
        SaveFunc800B45E0Default(recordIndex);
        return lapBitmask;
    } else {
        return 0;
    }
}

void SaveCupStatus(s32 arg0) {
    u8* points;
    u8 tmp;
    s32 tmp2;

    if (arg0 >= 0) {
        points = &eeprom_save_data.main.saveInfo.grandPrixPoints[g_raceClass];
        tmp = GetCupStatus_sub(g_cupSelect, *points);
        tmp2 = 3 - arg0;
        if ((arg0 < 3) && (tmp < (3 - arg0))) {
            *points = SetCupStatus_sub(g_cupSelect, *points, tmp2);
            SaveFunc800B4670Default();
            SaveFunc800B5948Default();
        }
    }
}

void SaveCupStatus2(s32 arg0, s32 arg1) {
    u8* points;
    s32 temp_a0;
    s32 temp;
    int temp2;
    UNUSED s32 pad;

    if (arg0 >= 0) {
        temp2 = arg1 / 4;
        points = &eeprom_save_data.main.saveInfo.grandPrixPoints[arg1 % 4];
        temp = GetCupStatus_sub(temp2, *points);

        if ((arg0 < 3) && (temp < (temp_a0 = 3 - arg0))) {
            *points = SetCupStatus_sub(temp2, *points, temp_a0);

            SaveFunc800B4670Default();
            SaveFunc800B5948Default();
        }
    }
}

// Get Grand Prix points for a given cup and CC mode
u8 GetCupStatus(s32 cup, s32 cc_mode) {
    return GetCupStatus_sub(cup, eeprom_save_data.main.saveInfo.grandPrixPoints[cc_mode]);
}

// Get Grand Prix points scored for a given cup
u8 GetCupStatus_sub(s32 cup, s32 ccGrandPrixPoints) {
    s32 cup_index = cup * 2;
    u32 cup_points = ccGrandPrixPoints;

    cup_points &= (3 << cup_index);
    cup_points >>= cup_index;
    cup_points &= 0xFF;

    return cup_points;
}

// Generate a new CC Grand Prix Points entry with points_scored
// placed in the given cup's location
u8 SetCupStatus_sub(s32 cup, s32 ccGrandPrixPoints, s32 points_scored) {
    s32 cup_index = cup * 2;

    points_scored <<= cup_index;
    ccGrandPrixPoints &= ~(3 << cup_index);

    return (ccGrandPrixPoints | points_scored);
}

// Check if all 4 cups have gold cups scored
// for a given CC mode
bool Check4GoldCup(s32 cc_mode) {
    if (eeprom_save_data.main.saveInfo.grandPrixPoints[cc_mode] == 0xFF) {
        return true;
    }
    return false;
}

// Check if the 150CC mode has all 4 gold cups
s32 Check4GoldCup_150cc(void) {
    return Check4GoldCup(CC_150);
}

// Check if the Extra mode has all 4 gold cups
s32 Check4GoldCup_pro(void) {
    return Check4GoldCup(CC_EXTRA);
}

void SaveFunc800B559C(s32 arg0) {
    CourseTimeTrialRecords* courseRecord;
    OnlyBestTimeTrialRecords* bestRecord;
    s32 x = arg0 / 8;
    s32 i;
    s32 j;
    for (i = x * 8; i < ((x * 8) + 8); i++) {
        bestRecord = &eeprom_save_data.onlyBestTimeTrialRecords[x];
        courseRecord = &eeprom_save_data.allCourseTimeTrialRecords.cupRecords[i / 4].courseRecords[i % 4];
        if (courseRecord->checksum != GetCheckSum_Course(i)) {
            for (j = 0; j < 3; j++) {
                bestRecord->bestThreelaps[i % 8][j] = D_800F2E60[j];
                bestRecord->bestSinglelaps[i % 8][j] = D_800F2E60[j];
            }
        } else {
            for (j = 0; j < 3; j++) {
                bestRecord->bestThreelaps[i % 8][j] = courseRecord->records[0][j];
                bestRecord->bestSinglelaps[i % 8][j] = courseRecord->records[0][j + 0x0f];
            }
        }
    }
    bestRecord = &eeprom_save_data.onlyBestTimeTrialRecords[x];
    bestRecord->unknownBytes[6] = GetCheckSum1_Course_bak(x);
    bestRecord->unknownBytes[7] = GetCheckSum2_Course_bak(x);
    osEepromLongWrite(&gSIEventMesgQueue, ((u32) (((u8*) bestRecord) - ((u8*) (&eeprom_save_data)))) >> 3,
                      bestRecord->bestThreelaps[0], 0x38);
}

/**
 * This one is weird. Its some type of checksum calculator, seemingly for the
 * best time trial records. But the number of bytes it operates over is
 * odd. It calculates a checksum for 51 bytes in 17 byte chunks, but that doesn't line
 * up with anything in the save data cleanly. At that byte count it would get the 48 bytes
 * for the records plus 3 of the unknown bytes.
 *
 * But only unknown bytes 7 and 8 ever get set, so why the extra 3, and why in chunks of 17?
 **/
u8 GetCheckSum1_Course_bak(s32 arg0) {
    u8* times = (u8*)&eeprom_save_data.onlyBestTimeTrialRecords[arg0];
    s32 checksum = 0;
    s32 i;
    s32 j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 0x11; j++) {
            checksum += (times[i * 0x11 + j] + 1) * (i + 1) + j;
        }
    }
    return (checksum % 256);
}

s32 GetCheckSum2_Course_bak(s32 arg0) {
    s32 tmp = eeprom_save_data.onlyBestTimeTrialRecords[arg0].unknownBytes[6] + 90;
    return (tmp % 256) & 0xFF;
}

s32 CheckEEPRomCourseData_bak(s32 arg0) {
    UNUSED s32 pad1;
    OnlyBestTimeTrialRecords* temp_v1;
    UNUSED s32 pad2;
    UNUSED s32 pad3;

    temp_v1 = &eeprom_save_data.onlyBestTimeTrialRecords[arg0 / 8];
    if ((temp_v1->unknownBytes[6] != (GetCheckSum1_Course_bak(arg0 / 8) ^ 0)) ||
        (temp_v1->unknownBytes[7] != (GetCheckSum2_Course_bak(arg0 / 8) ^ 0))) {
        return 1;
    }

    return 0;
}

void SaveFunc800B5948(void) {
    s32 cup_index;
    Stuff* main = &eeprom_save_data.main;
    Stuff* backup = &eeprom_save_data.backup;
    for (cup_index = 0; cup_index < NUM_CUPS - 1; cup_index++) {
        backup->saveInfo.grandPrixPoints[cup_index] = main->saveInfo.grandPrixPoints[cup_index];
    }
    backup->saveInfo.soundMode = main->saveInfo.soundMode;
    backup->checksum[1] = GetCheckSum1_flgs1_bak();
    backup->checksum[2] = GetCheckSum2_flgs1_bak();
    osEepromLongWrite(&gSIEventMesgQueue, EEPROM_ADDR(backup), (u8*) backup, sizeof(Stuff));
}

u8 GetCheckSum1_flgs1_bak(void) {
    u8* backupGrandPrixPoints = eeprom_save_data.backup.saveInfo.grandPrixPoints;
    u32 i;
    s32 crc = 0;

    for (i = 0; i < sizeof(SaveInfo); i++) {
        crc += ((backupGrandPrixPoints[i] + 1) * (i + 1)) + i;
    }

    return crc % 0x100;
}

u8 GetCheckSum2_flgs1_bak(void) {
    s32 tmp = eeprom_save_data.backup.checksum[1] + 90;
    return (tmp % 256);
}

s32 CheckEEPRomFlgs1_bak(void) {
    u8* backupChecksum = eeprom_save_data.backup.checksum;
    if (backupChecksum[1] != GetCheckSum1_flgs1_bak() ||
        backupChecksum[2] != GetCheckSum2_flgs1_bak()) {
        return 1;
    }

    return 0;
}

// Check if controller has a Controller Pak connected.
// Return PAK if it does, otherwise return NO_PAK.
s32 getExistPak(s32 controller) {
    u8 controllerBitpattern;
    UNUSED s32 phi_v0;

    if ((controller >= MAXCONTROLLERS) || (controller < 0)) {
        return NO_PAK;
    }

    osPfsIsPlug(&gSIEventMesgQueue, &controllerBitpattern);

    if ((controllerBitpattern & (1 << controller)) != 0) {
        return PAK;
    }

    return NO_PAK;
}

// gives status info about controller pak insterted in controller 1
s32 CheckPakStatus(void) {
    if (gControllerPak1State) {
        switch (osPfsFindFile(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) gGameName, (u8*) gExtCode,
                              &gControllerPak1FileNote)) {
            case PFS_NO_ERROR:
                return PFS_NO_ERROR;
            case PFS_ERR_INVALID:
                break;
            case PFS_ERR_NEW_PACK:
                gControllerPak1State = BAD;
                break;
            default:
                gControllerPak1State = BAD;
                break;
        }
    }

    if (!gControllerPak1State) {
        s32 errorCode;
        if (getExistPak(CONTROLLER_1) == NO_PAK) {
            return PFS_NO_PAK_INSERTED;
        }
        errorCode = osPfsInit(&gSIEventMesgQueue, &gControllerPak1FileHandle, CONTROLLER_1);

        if (errorCode) {
            switch (errorCode) {
                case PFS_ERR_NOPACK:
                case PFS_ERR_DEVICE:
                    return PFS_NO_PAK_INSERTED;
                case PFS_ERR_ID_FATAL:
                    return PFS_PAK_BAD_READ;
                default:
                case PFS_ERR_CONTRFAIL:
                    return PFS_PAK_BAD_READ;
            }
        }

        gControllerPak1State = OK;
        if (osPfsFindFile(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) gGameName, (u8*) gExtCode,
                          &gControllerPak1FileNote) == PFS_NO_ERROR) {
            return PFS_NO_ERROR;
        }
        if (osPfsNumFiles(&gControllerPak1FileHandle, &gControllerPak1NumFilesUsed,
                          &gControllerPak1MaxWriteableFiles) != PFS_NO_ERROR) {
            return PFS_PAK_BAD_READ;
        }
        if (osPfsFreeBlocks(&gControllerPak1FileHandle, &gControllerPak1NumPagesFree) != PFS_NO_ERROR) {
            return PFS_PAK_BAD_READ;
        }
        gControllerPak1NumPagesFree = gControllerPak1NumPagesFree >> 8;
    }

    if (gControllerPak1MaxWriteableFiles >= gControllerPak1NumFilesUsed) {
        return PFS_FILE_OVERFLOW;
    }
    if (gControllerPak1NumPagesFree >= 0x79) {
        return PFS_INVALID_DATA;
    }
    return PFS_FILE_OVERFLOW;
}

// gives status info about controller pak insterted in controller 2
s32 CheckPakStatus2(void) {
    s32 stateBorrow = sControllerPak2State;

    if (stateBorrow) {
        switch (osPfsFindFile(&gControllerPak2FileHandle, gCompanyCode, gGameCode, (u8*) gGameName, (u8*) gExtCode,
                              &gControllerPak2FileNote)) {
            case PFS_NO_ERROR:
                return PFS_NO_ERROR;
            case PFS_ERR_INVALID:
                return PFS_INVALID_DATA;
            default:
            case PFS_ERR_NEW_PACK:
                sControllerPak2State = BAD;
                stateBorrow = BAD;
        }
    }
    if (!stateBorrow) {
        s32 errorCode;
        if (getExistPak(CONTROLLER_2) == NO_PAK) {
            return PFS_NO_PAK_INSERTED;
        }

        errorCode = osPfsInit(&gSIEventMesgQueue, &gControllerPak2FileHandle, CONTROLLER_2);
        if (errorCode) {
            switch (errorCode) {
                case PFS_ERR_NOPACK:
                case PFS_ERR_DEVICE:
                    return PFS_NO_PAK_INSERTED;
                case PFS_ERR_ID_FATAL:
                    return PFS_PAK_BAD_READ;
                case PFS_ERR_CONTRFAIL:
                default:
                    return PFS_PAK_BAD_READ;
            }
        }

        sControllerPak2State = OK;

        switch (osPfsFindFile(&gControllerPak2FileHandle, gCompanyCode, gGameCode, (u8*) gGameName, (u8*) gExtCode,
                              &gControllerPak2FileNote)) {
            case PFS_NO_ERROR:
                return PFS_NO_ERROR;
            case PFS_ERR_INVALID:
                return PFS_INVALID_DATA;
            case PFS_ERR_NEW_PACK:
            default:
                return PFS_PAK_BAD_READ;
        }
    }
}

s32 initPak(void) {
    s32 errorCode;

    if (gControllerPak1State) {
        return PFS_PAK_STATE_OK;
    }
    if (getExistPak(CONTROLLER_1) != NO_PAK) {
        errorCode = osPfsInit(&gSIEventMesgQueue, &gControllerPak1FileHandle, CONTROLLER_1);
        if (osPfsNumFiles(&gControllerPak1FileHandle, &gControllerPak1NumFilesUsed,
                          &gControllerPak1MaxWriteableFiles) != PFS_NO_ERROR) {
            gControllerPak1State = BAD;
            return PFS_NUM_FILES_ERROR;
        }
        if (osPfsFreeBlocks(&gControllerPak1FileHandle, &gControllerPak1NumPagesFree) != PFS_NO_ERROR) {
            gControllerPak1State = BAD;
            return PFS_FREE_BLOCKS_ERROR;
        }
        gControllerPak1NumPagesFree = gControllerPak1NumPagesFree >> 8;
        if (errorCode == PFS_NO_ERROR) {
            gControllerPak1State = OK;
        }
        return errorCode;
    }
    return PAK_NOT_INSERTED;
}

s32 initPak2(void) {
    s32 errorCode;

    if (sControllerPak2State) {
        return PFS_PAK_STATE_OK;
    }
    if (getExistPak(CONTROLLER_2) != NO_PAK) {
        errorCode = osPfsInit(&gSIEventMesgQueue, &gControllerPak2FileHandle, CONTROLLER_2);
        if (errorCode == PFS_NO_ERROR) {
            sControllerPak2State = OK;
        }
        return errorCode;
    }
    return PAK_NOT_INSERTED;
}

s32 SaveGameNoteKart_data(s32 arg0) {
    struct_8018EE10_entry* temp_v1;

    temp_v1 = &D_8018EE10[arg0];
    temp_v1->checksum = GetCheckSum_pak(arg0);
    return osPfsReadWriteFile(&gControllerPak1FileHandle, gControllerPak1FileNote, PFS_WRITE,
                              arg0 * 0x80 /* 0x80 == sizeof(struct_8018EE10_entry) */, sizeof(struct_8018EE10_entry),
                              (u8*) temp_v1);
}

u8 GetCheckSum_ghost(s32 page) {
    s32 i;
    u32 checksum = 0;
    u8* addr;

    for (i = 0, addr = (u8*) &((u8*) keystock2)[page * 256]; i < 256; i++) {
        checksum += (*addr++ * (page + 1) + i);
    }
    return checksum;
}

s32 SaveGameNoteKart_ghost(s32 arg0) {
    s32 var_v0;
    s32 var_s0;
    struct_8018EE10_entry* temp_s3;

    switch (arg0) {
        case 0:
        case 1:
            break;
        default:
            return -1;
    }
    if (gGamestate == RACING) {
        make_press_ghost();
    }
    temp_s3 = &D_8018EE10[arg0];
    temp_s3->ghostDataSaved = 0;
    var_v0 = SaveGameNoteKart_data(arg0);
    if (var_v0 != 0) {
        temp_s3->ghostDataSaved = 0;
        for (var_s0 = 0; var_s0 < 0x3C; var_s0++) {
            temp_s3->unk_07[var_s0] = var_s0;
        }
    } else {
        var_v0 = osPfsReadWriteFile(&gControllerPak1FileHandle, gControllerPak1FileNote, 1U, (arg0 * 0x3C00) + 0x100,
                                    0x00003C00, (u8*) keystock2);
        if (var_v0 == 0) {
            temp_s3->ghostDataSaved = 1;
            if (gGamestate == RACING) {
                temp_s3->courseIndex = (g_cupSelect * 4) + g_courseSelect;
            }
            temp_s3->unk_00 = D_80162DFC;
            temp_s3->characterId = (u8) ghost_kart;
            for (var_s0 = 0; var_s0 < 0x3C; var_s0++) {
                temp_s3->unk_07[var_s0] = GetCheckSum_ghost(var_s0);
            }
            var_v0 = SaveGameNoteKart_data(arg0);
        }
        if (var_v0 != 0) {
            temp_s3->ghostDataSaved = 0;
            for (var_s0 = 0; var_s0 < 0x3C; var_s0++) {
                temp_s3->unk_07[var_s0] = var_s0;
            }
        }
    }
    return var_v0;
}

s32 CheckSameMap(s32 arg0) {
    if ((D_8018EE10[0].ghostDataSaved != 0) && (arg0 == D_8018EE10[0].courseIndex)) {
        return 0;
    }
    if ((D_8018EE10[1].ghostDataSaved != 0) && (arg0 == D_8018EE10[1].courseIndex)) {
        return 1;
    }
    return 0;
}

s32 CheckSameMap2(s32 arg0) {
    if ((D_8018EE10[0].ghostDataSaved != 0) && (arg0 == D_8018EE10[0].courseIndex)) {
        return 0;
    }
    if ((D_8018EE10[1].ghostDataSaved != 0) && (arg0 == D_8018EE10[1].courseIndex)) {
        return 1;
    }
    return -1;
}

s32 CheckSameGhost(s32 arg0) {
    s32 temp_s0;
    u8* phi_s1;
    s32 phi_s3;

    make_press_ghost();
    romghost_status = 1;
    kill_ghost(gPlayerThree);

    phi_s3 = 0;
    if (((g_cupSelect * 4) + g_courseSelect) != D_8018EE10[arg0].courseIndex) {
        phi_s3 = 2;
    } else if (D_80162DFC != D_8018EE10[arg0].unk_00) {
        phi_s3 = 3;
    } else {
        if (ghost_kart != (u8) D_8018EE10[arg0].characterId) {
            phi_s3 = 4;
        } else {
            temp_s0 = 0;
            phi_s1 = (u8*) &D_8018EE10[arg0];

            while (temp_s0 < 0x3C) {
                if (phi_s1[7] != GetCheckSum_ghost(temp_s0)) {
                    phi_s3 = 1;
                    break;
                }

                ++phi_s1;
                ++temp_s0;
            }
        }
    }

    return phi_s3;
}

s32 LoadGameNoteKart_ghost(s32 arg0) {
    s32 temp_s0;
    s32 temp_v0;
    u8* phi_s1;

    if ((arg0 != 0) && (arg0 != 1)) {
        return -1;
    }

    temp_v0 = osPfsReadWriteFile(&gControllerPak1FileHandle, gControllerPak1FileNote, PFS_READ, (arg0 * 0x3C00) + 0x100,
                                 0x3C00, (u8*) keystock2);
    if (temp_v0 == 0) {
        // clang-format off
        phi_s1 = (u8 *) &D_8018EE10[arg0]; temp_s0 = 0; while (1) {
            // clang-format on

            if (phi_s1[7] != GetCheckSum_ghost(temp_s0)) {
                D_8018EE10[arg0].ghostDataSaved = 0;
                return -2;
            }

            ++phi_s1;
            if ((++temp_s0) == 0x3C) {
                make_melt_ghost();
                ghost_status = 0;
                ghost_kart = (s32) D_8018EE10[arg0].characterId;
                D_80162DFC = D_8018EE10[arg0].unk_00;
                break;
            }
        }
    }

    return temp_v0;
}

s32 LoadGameNoteKart_ghost2(s32 arg0, s32 arg1) {
    UNUSED s32 stackPadding;
    s32 i;
    s32 writeStatus;
    struct_8018EE10_entry* temp_s3;
    switch (arg0) {
        case 0:
        case 1:
            break;
        default:
            return -1;
    }
    writeStatus = osPfsReadWriteFile(&gControllerPak2FileHandle, gControllerPak2FileNote, 0U, (arg0 * 0x3C00) + 0x100,
                                     0x00003C00, (u8*) keystock2);
    if (writeStatus == 0) {
        temp_s3 = &((struct_8018EE10_entry*) kdl_buff)[arg0];
        for (i = 0; i < 0x3C; i++) {
            if (temp_s3->unk_07[i] != GetCheckSum_ghost(i)) {
                temp_s3->ghostDataSaved = 0;
                return -2;
            }
        }
        ghost_kart = temp_s3->characterId;
        D_80162DFC = temp_s3->unk_00;
        D_8018EE10[arg1].courseIndex = temp_s3->courseIndex;
    }
    return writeStatus;
}

void LoadGameNoteKart_data(void) {
    s32 temp_s0;

    osPfsReadWriteFile(&gControllerPak1FileHandle, gControllerPak1FileNote, PFS_READ, 0,
                       0x100 /*  2*sizeof(struct_8018EE10_entry) ? */, (u8*) &D_8018EE10);

    for (temp_s0 = 0; temp_s0 < 2; ++temp_s0) {
        if (D_8018EE10[temp_s0].checksum != GetCheckSum_pak(temp_s0)) {
            D_8018EE10[temp_s0].ghostDataSaved = 0;
        }
    }
}

void LoadGameNoteKart_data2(void) {
    s32 temp_s0;
    u8* tmp;

    tmp = (u8*) kdl_buff;

    osPfsReadWriteFile(&gControllerPak2FileHandle, gControllerPak2FileNote, PFS_READ, 0,
                       0x100 /*  2*sizeof(struct_8018EE10_entry) ? */, tmp);

    for (temp_s0 = 0; temp_s0 < 2; ++temp_s0) {
        // if (kdl_buff[temp_s0]->checksum != GetCheckSum_pak2(temp_s0)) {
        //     kdl_buff[temp_s0]->ghostDataSaved = 0;
        // }
        if (((struct_8018EE10_entry*) (tmp + (temp_s0 << 7)))->checksum != GetCheckSum_pak2(temp_s0)) {
            ((struct_8018EE10_entry*) (tmp + (temp_s0 << 7)))->ghostDataSaved = 0;
        }
    }
}

u8 GetCheckSum_pak(s32 arg0) {
    u32 checksum = 0;
    u8* addr = (u8*) &D_8018EE10[arg0];
    s32 i;
    for (i = 0; i < 0x43; i++) {
        checksum += ((addr[i] * (arg0 + 1)) + i);
    }
    return checksum;
}

u8 GetCheckSum_pak2(s32 arg0) {
    struct_8018EE10_entry* var_v0 = kdl_buff;
    u8 *addr = (u8*)(var_v0 + arg0);
    s32 i = 0;
    u32 checksum = 0;
    
    for (i = 0; i < (s32)offsetof(struct_8018EE10_entry, pad_43); i++) {
        checksum += (addr[i] * (arg0 + 1)) + i;
    }
    
    return checksum;
}

s32 initGameNoteKart(s32 arg0) {
    u32 i;
    struct_8018EE10_entry* plz = &D_8018EE10[arg0];

    plz->ghostDataSaved = false;
    plz->courseIndex = 0;
    plz->characterId = 0;
    for (i = 0; i < sizeof(plz->unk_07); i++) {
        plz->unk_07[i] = i;
    }
    plz->checksum = GetCheckSum_pak(arg0);

    return osPfsReadWriteFile(&gControllerPak1FileHandle, gControllerPak1FileNote, PFS_WRITE,
                              (s32) sizeof(struct_8018EE10_entry) * arg0, sizeof(struct_8018EE10_entry), (u8*) plz);
}

s32 makeGameNoteKartOnly(void) {
    UNUSED s32 pad;
    s32 ret;
    s32 i;

    ret = osPfsAllocateFile(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) &gGameName, (u8*) &gExtCode,
                            0x7900, &gControllerPak1FileNote);
    if (ret == 0) {
        for (i = 0; i < 2; i++) {
            initGameNoteKart(i);
        }
    }

    return ret;
}

void makeGameNoteKart(void) {
    if (getExistPak(CONTROLLER_1) && osPfsInit(&gSIEventMesgQueue, &gControllerPak1FileHandle, 0) == 0 &&
        osPfsFindFile(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) gGameName, (u8*) gExtCode,
                      &gControllerPak1FileNote) &&
        osPfsNumFiles(&gControllerPak1FileHandle, &gControllerPak1NumFilesUsed, &gControllerPak1MaxWriteableFiles) ==
            0 &&
        gControllerPak1MaxWriteableFiles < gControllerPak1NumFilesUsed &&
        osPfsFreeBlocks(&gControllerPak1FileHandle, &gControllerPak1NumPagesFree) == 0) {
        gControllerPak1NumPagesFree >>= 8;
        if (gControllerPak1NumPagesFree >= 0x79) {
            makeGameNoteKartOnly();
        }
    }
}

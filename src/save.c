#include <ultra64.h>
#include <macros.h>
#include <defines.h>
#include "stddef.h"

#include "save.h"
#include "OverKart5Hooks.h"

#include "menu_items.h"
#include "menus.h"
#include "save_data.h"
#include "replays.h"
#include "code_80057C60.h"

/*** macros ***/
#define PFS_COMPANY_CODE(c0, c1) ((u16) (((c0) << 8) | ((c1))))
#define PFS_GAME_CODE(c0, c1, c2, c3) ((u32) (((c0) << 24) | ((c1) << 16) | ((c2) << 8) | (c3)))
// calculate an eeprom Address based off of the ram Address of the SaveData variable
// very fragile!
#define EEPROM_ADDR(ptr) (((uintptr_t) (ptr) - (uintptr_t) (&EEPRomSaveData)) / 8)

/** BSS **/
// stores ghost data
struct_8018EE10_entry D_8018EE10[2];
OSPfs MotorPakHandle[MAXCONTROLLERS];
s32 MotorActive[MAXCONTROLLERS];
static s8 PakTypeCached[MAXCONTROLLERS];
static u8 PakTypeValid[MAXCONTROLLERS];
static s32 MotorLastOn[MAXCONTROLLERS];

/*** data ***/
u16 CompanyCode = PFS_COMPANY_CODE('0', '1');
u32 PakGameCode = PFS_GAME_CODE('N', 'K', 'T', 'J');
s8 ControllerPak1State = BAD;
s8 SControllerPak2State = BAD;

/*** rodata ***/
// default Time trial records in little endian form
const u8 D_800F2E60[4] = { 0xc0, 0x27, 0x09, 0x00 };
// osPfsFindFile -> GameName ("MARIOKART64" in nosFont)
const u8 GameName[] = {
    0x26, 0x1a, 0x2b, 0x22, 0x28, 0x24, 0x1a, 0x2b, 0x2d, 0x16, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00
};
// ext_name Param to osPfsFindFile (four total bytes, but only one is setable)
const u8 ExtCode[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// new file Start?
void SaveFunc800B45E0(s32 arg0) {
    CourseTimeTrialRecords* courseTimeTrialRecordsPtr =
        &EEPRomSaveData.AllCourseTimeTrialRecords.CupRecords[arg0 / 4].CourseRecords[arg0 % 4];

    courseTimeTrialRecordsPtr->Checksum = GetCheckSumCourse(arg0);
    osEepromLongWrite(&SIEventMesgQueue, EEPROM_ADDR(courseTimeTrialRecordsPtr), (u8*) courseTimeTrialRecordsPtr,
                      sizeof(CourseTimeTrialRecords));
}

void SaveFunc800B4670(void) {
    Stuff* Main = &EEPRomSaveData.Main;
    Main->Checksum[1] = GetCheckSum1Flgs1();
    Main->Checksum[2] = GetCheckSum2Flgs1();
    osEepromLongWrite(&SIEventMesgQueue, EEPROM_ADDR(Main), (u8*) Main, sizeof(Stuff));
}

void InitEEPRom(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        InitEEPRomCourseData(i);
        SaveFunc800B559CDefault(i);
    }

    InitEEPRomFlgs1Data();
    SaveFunc800B5948Default();
}

void InitEEPRomCourseData(s32 arg0) {
    s32 i, j;
    CourseTimeTrialRecords* courseTimeTrialRecords;

    courseTimeTrialRecords = &EEPRomSaveData.AllCourseTimeTrialRecords.CupRecords[arg0 / 4].CourseRecords[arg0 % 4];

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 3; j++) {
            courseTimeTrialRecords->records[i][j] = D_800F2E60[j];
        }
    }

    for (i = 0; i < 3; i++) {
        courseTimeTrialRecords->records[5][i] = D_800F2E60[i];
    }

    courseTimeTrialRecords->UnknownBytes[0] = 0;
    courseTimeTrialRecords->Checksum = GetCheckSumCourse(arg0);
    SaveFunc800B45E0Default(arg0);
}

void InitEEPRomFlgs1Data(void) {
    s32 cup_index;
    Stuff* Main = &EEPRomSaveData.Main;
    for (cup_index = 0; cup_index < 4; cup_index++) {
        Main->SaveInfo.GrandPrixPoints[cup_index] = 0;
    }
    Main->SaveInfo.SoundMode = SOUND_STEREO;
    SoundOutput = SOUND_STEREO;
    SettingSoundOutMode();
    SaveFunc800B4670Default();
}

// create a magic Number based on the Time trial records
u8 GetCheckSumCourse(s32 courseIdx) {
    s32 j;
    s32 i;
    s32 ret;
    u8* records = EEPRomSaveData.AllCourseTimeTrialRecords.CupRecords[courseIdx / 4].CourseRecords[courseIdx % 4].records[0];

    ret = 0;
    for (i = 0; i < 7; i++) {
        for (j = 0; j < 3; j++) {
            ret += *(records + i * 3 + j) * (j + 1) + i;
        }
    }

    return ret % 256;
}

u8 GetCheckSum1Flgs1(void) {
    u8* GrandPrixPoints = (u8*) &EEPRomSaveData.Main.SaveInfo.GrandPrixPoints;
    u32 i;
    s32 crc = 0;

    for (i = 0; i < sizeof(SaveInfo); i++) {
        crc += ((GrandPrixPoints[i] + 1) * (i + 1)) + i;
    }

    return crc % 0x100;
}

u8 GetCheckSum2Flgs1(void) {
    s32 tmp = EEPRomSaveData.Main.Checksum[1] + 90;
    return (tmp % 256);
}

void LoadFunc800B4A10(void) {
    s32 i;

    osEepromLongRead(&SIEventMesgQueue, EEPROM_ADDR(&EEPRomSaveData), (u8*) &EEPRomSaveData, sizeof(SaveData));
    // 16: 4 cup records * 4 course records?
    for (i = 0; i < 16; i++) {
        CheckEEPRomCourseData(i);
    }

    SaveFunc800B4CB4();

    SoundOutput = EEPRomSaveData.Main.SaveInfo.SoundMode;
    if (SoundOutput >= NUM_SOUND_MODES) {
        SoundOutput = SOUND_MONO;
    }
}

void CheckEEPRomCourseData(s32 course) {
    OnlyBestTimeTrialRecords* test;
    CourseTimeTrialRecords* sp24;
    s32 i;

    if ((GetRecordTime3(0, course) & 0xFFFFF) < 0x927C0U) {
        EEPRomSaveData.AllCourseTimeTrialRecords.CupRecords[course / 4].CourseRecords[course % 4].UnknownBytes[0] = 1;
    }
    sp24 = &EEPRomSaveData.AllCourseTimeTrialRecords.CupRecords[course / 4].CourseRecords[course % 4];

    GetRecordLapTime3(course);
    if (sp24) {}

    if (sp24->Checksum != GetCheckSumCourse(course)) {
        InitEEPRomCourseData(course);
        if (CheckEEPRomCourseDataBak(course) == 0) {
            s32 a3 = 0;

            test = &EEPRomSaveData.OnlyBestTimeTrialRecords[course / 8];
            for (i = 0; i < 3; i++) {
                sp24->records[TIME_TRIAL_3LAP_RECORD_1][i] = test->bestThreelaps[course % 8][i];
                sp24->records[TIME_TRIAL_1LAP_RECORD][i] = test->bestSinglelaps[course % 8][i];

                // This is checking (in a roundabout way) if the given record
                // is the default Value of 0x927C0
                if (sp24->records[TIME_TRIAL_3LAP_RECORD_1][i] == D_800F2E60[i]) {
                    if (sp24->records[TIME_TRIAL_1LAP_RECORD][i] == D_800F2E60[i]) {
                        a3 += 1;
                    }
                }
            }

            if (a3 == 3) {
                sp24->UnknownBytes[0] = 0;
            } else {
                sp24->UnknownBytes[0] = 1;
            }
            SaveFunc800B45E0Default(course);
        }
        // L800B4C78
        SaveFunc800B559CDefault(course);
    } else if (CheckEEPRomCourseDataBak(course)) {
        // L800B4C88
        SaveFunc800B559CDefault(course);
    }
}

void SaveFunc800B4CB4(void) {
    s32 cup_index;
    Stuff* Main = &EEPRomSaveData.Main;
    Stuff* Backup = &EEPRomSaveData.Backup;
    if (Main->Checksum[1] != (GetCheckSum1Flgs1()) ||
        (Main->Checksum[2] != GetCheckSum2Flgs1())) {
        InitEEPRomFlgs1Data();

        if (CheckEEPRomFlgs1Bak() == 0) {
            for (cup_index = 0; cup_index < 4; cup_index++) {
                Main->SaveInfo.GrandPrixPoints[cup_index] = Backup->SaveInfo.GrandPrixPoints[cup_index];
            }

            Main->SaveInfo.SoundMode = Backup->SaveInfo.SoundMode;
            Main->Checksum[1] = GetCheckSum1Flgs1Bak();
            Main->Checksum[2] = GetCheckSum2Flgs1Bak();
            osEepromLongWrite(&SIEventMesgQueue, EEPROM_ADDR(Main), (u8*) Main, sizeof(Stuff));
        }
        SaveFunc800B5948Default();
        return;
    }

    if (CheckEEPRomFlgs1Bak() != 0) {
        SaveFunc800B5948Default();
    }
}

void SetRecordTime(u8* timeTrialRecord, u32 Time, s32 CharacterId) {
    u32 timeRightShift8 = Time >> 8;
    u32 timeRightShift16 = timeRightShift8 >> 8;
    s16 timeRightShift8Duplicate;
    u16 timeRightShift16Duplicate;

    timeRightShift16Duplicate = timeRightShift16;

    timeTrialRecord[0] = Time & 0xFF;
    timeTrialRecord[1] = (timeRightShift8Duplicate = timeRightShift8);
    timeTrialRecord[2] = (timeRightShift16Duplicate & 0xF) + ((CharacterId & 7) << 4);
}

// combine Time trial record u8[3] into the lower 24 bits of a word [xx221100]
u32 GetRecordTime(u8* arr) {
    s32 a, b, c;
    a = arr[0];
    b = arr[1];
    c = arr[2];

    return (a + (b << 8) + (c << 16)) & 0x00FFFFFF;
}

// Get a Time trial record, infer course Index
s32 GetRecordTime2(s32 recordIndex) {
    return GetRecordTime(EEPRomSaveData.AllCourseTimeTrialRecords.CupRecords[(((CupSelect * 4) + CourseSelect) / 4)]
                             .CourseRecords[(((CupSelect * 4) + CourseSelect) % 4)]
                             .records[recordIndex]);
}

// Get a Time trial record, but take the course Index as an argument
u32 GetRecordTime3(s32 recordIndex, s32 CourseIndex) {
    return GetRecordTime(EEPRomSaveData.AllCourseTimeTrialRecords.CupRecords[(CourseIndex / 4)]
                             .CourseRecords[(CourseIndex % 4)]
                             .records[recordIndex]);
}

// Get Best Lap record of the inferred course Index
s32 GetRecordLapTime(void) {
    return GetRecordTime(EEPRomSaveData.AllCourseTimeTrialRecords.CupRecords[(((CupSelect * 4) + CourseSelect) / 4)]
                             .CourseRecords[(((CupSelect * 4) + CourseSelect) % 4)]
                             .records[TIME_TRIAL_1LAP_RECORD]);
}

// Get the best single lap Time record of the given course Index
s32 GetRecordLapTime3(s32 CourseIndex) {
    return GetRecordTime(EEPRomSaveData.AllCourseTimeTrialRecords.CupRecords[(CourseIndex / 4)]
                             .CourseRecords[(CourseIndex % 4)]
                             .records[TIME_TRIAL_1LAP_RECORD]);
}

s32 SaveCourseRecord(u32 Time, s32 charId) {
    UNUSED s32 stackPadding[3];
    s32 course; // sp30
    s32 i;
    s32 j;
    CourseTimeTrialRecords* tt;

    course = CupSelect * 4 + CourseSelect;
    tt = &EEPRomSaveData.AllCourseTimeTrialRecords.CupRecords[course / 4].CourseRecords[course % 4];

    i = 0;
    for (; i < 5; i++) {
        if (Time < (GetRecordTime(tt->records[i]) & 0x000FFFFF)) {
            break;
        }
    }

    // `Time` is:
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

    SetRecordTime(tt->records[i], Time, charId);
    tt->UnknownBytes[0] = 1;
    SaveFunc800B45E0Default(course);

    return i;
}

s32 SaveCourseLapRecord(void) {
    u8* recordPointer;
    UNUSED s32 pad;
    s32 fastestLapIndex;
    s32 recordIndex;
    UNUSED s32 Pad2[2];
    s32 checkLapIndex;
    s32 character;
    s32 lapBitmask;
    recordIndex = (CupSelect * 4) + CourseSelect;
    recordPointer =
        &EEPRomSaveData.AllCourseTimeTrialRecords.CupRecords[recordIndex / 4].CourseRecords[recordIndex % 4].records[0][0];
    lapBitmask = 1;
    fastestLapIndex = 0;
    character = *CharacterSelections;
    for (checkLapIndex = 1; checkLapIndex != 3; checkLapIndex++) {
        if ((s32)PlayerHUD->Laptimebuf[checkLapIndex] < (s32)PlayerHUD->Laptimebuf[fastestLapIndex]) {
            lapBitmask = 1 << checkLapIndex;
            fastestLapIndex = checkLapIndex;
        } else if ((s32)PlayerHUD->Laptimebuf[fastestLapIndex] == (s32)PlayerHUD->Laptimebuf[checkLapIndex]) {
            lapBitmask |= 1 << checkLapIndex;
        }
    }

    if (PlayerHUD->Laptimebuf[fastestLapIndex] < (GetRecordLapTime() & 0xFFFFF)) {
        SetRecordTime(recordPointer + 0xF, PlayerHUD->Laptimebuf[fastestLapIndex], character);
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
        points = &EEPRomSaveData.Main.SaveInfo.GrandPrixPoints[RaceClass];
        tmp = GetCupStatusSub(CupSelect, *points);
        tmp2 = 3 - arg0;
        if ((arg0 < 3) && (tmp < (3 - arg0))) {
            *points = SetCupStatusSub(CupSelect, *points, tmp2);
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
        points = &EEPRomSaveData.Main.SaveInfo.GrandPrixPoints[arg1 % 4];
        temp = GetCupStatusSub(temp2, *points);

        if ((arg0 < 3) && (temp < (temp_a0 = 3 - arg0))) {
            *points = SetCupStatusSub(temp2, *points, temp_a0);

            SaveFunc800B4670Default();
            SaveFunc800B5948Default();
        }
    }
}

// Get Grand Prix points for a given cup and CC Mode
u8 GetCupStatus(s32 cup, s32 cc_mode) {
    return GetCupStatusSub(cup, EEPRomSaveData.Main.SaveInfo.GrandPrixPoints[cc_mode]);
}

// Get Grand Prix points scored for a given cup
u8 GetCupStatusSub(s32 cup, s32 ccGrandPrixPoints) {
    s32 cup_index = cup * 2;
    u32 cup_points = ccGrandPrixPoints;

    cup_points &= (3 << cup_index);
    cup_points >>= cup_index;
    cup_points &= 0xFF;

    return cup_points;
}

// Generate a new CC Grand Prix Points entry with points_scored
// placed in the given cup's location
u8 SetCupStatusSub(s32 cup, s32 ccGrandPrixPoints, s32 points_scored) {
    s32 cup_index = cup * 2;

    points_scored <<= cup_index;
    ccGrandPrixPoints &= ~(3 << cup_index);

    return (ccGrandPrixPoints | points_scored);
}

// Check if all 4 cups have gold cups scored
// for a given CC Mode
bool Check4GoldCup(s32 cc_mode) {
    if (EEPRomSaveData.Main.SaveInfo.GrandPrixPoints[cc_mode] == 0xFF) {
        return true;
    }
    return false;
}

// Check if the 150CC Mode has all 4 gold cups
s32 Check4GoldCup150CC(void) {
    return Check4GoldCup(CC_150);
}

// Check if the Extra Mode has all 4 gold cups
s32 Check4GoldCupPro(void) {
    return Check4GoldCup(CC_EXTRA);
}

void SaveFunc800B559C(s32 arg0) {
    CourseTimeTrialRecords* courseRecord;
    OnlyBestTimeTrialRecords* bestRecord;
    s32 x = arg0 / 8;
    s32 i;
    s32 j;
    for (i = x * 8; i < ((x * 8) + 8); i++) {
        bestRecord = &EEPRomSaveData.OnlyBestTimeTrialRecords[x];
        courseRecord = &EEPRomSaveData.AllCourseTimeTrialRecords.CupRecords[i / 4].CourseRecords[i % 4];
        if (courseRecord->Checksum != GetCheckSumCourse(i)) {
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
    bestRecord = &EEPRomSaveData.OnlyBestTimeTrialRecords[x];
    bestRecord->UnknownBytes[6] = GetCheckSum1CourseBak(x);
    bestRecord->UnknownBytes[7] = GetCheckSum2CourseBak(x);
    osEepromLongWrite(&SIEventMesgQueue, ((u32) (((u8*) bestRecord) - ((u8*) (&EEPRomSaveData)))) >> 3,
                      bestRecord->bestThreelaps[0], 0x38);
}

/**
 * This one is weird. Its some Type of Checksum calculator, seemingly for the
 * best Time trial records. But the Number of bytes it operates over is
 * odd. It calculates a Checksum for 51 bytes in 17 byte chunks, but that doesn't line
 * up with anything in the save data cleanly. At that byte Count it would get the 48 bytes
 * for the records plus 3 of the Unknown bytes.
 *
 * But only Unknown bytes 7 and 8 ever get set, so why the extra 3, and why in chunks of 17?
 **/
u8 GetCheckSum1CourseBak(s32 arg0) {
    u8* times = (u8*)&EEPRomSaveData.OnlyBestTimeTrialRecords[arg0];
    s32 Checksum = 0;
    s32 i;
    s32 j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 0x11; j++) {
            Checksum += (times[i * 0x11 + j] + 1) * (i + 1) + j;
        }
    }
    return (Checksum % 256);
}

s32 GetCheckSum2CourseBak(s32 arg0) {
    s32 tmp = EEPRomSaveData.OnlyBestTimeTrialRecords[arg0].UnknownBytes[6] + 90;
    return (tmp % 256) & 0xFF;
}

s32 CheckEEPRomCourseDataBak(s32 arg0) {
    UNUSED s32 pad1;
    OnlyBestTimeTrialRecords* temp_v1;
    UNUSED s32 Pad2;
    UNUSED s32 Pad3;

    temp_v1 = &EEPRomSaveData.OnlyBestTimeTrialRecords[arg0 / 8];
    if ((temp_v1->UnknownBytes[6] != (GetCheckSum1CourseBak(arg0 / 8) ^ 0)) ||
        (temp_v1->UnknownBytes[7] != (GetCheckSum2CourseBak(arg0 / 8) ^ 0))) {
        return 1;
    }

    return 0;
}

void SaveFunc800B5948(void) {
    s32 cup_index;
    Stuff* Main = &EEPRomSaveData.Main;
    Stuff* Backup = &EEPRomSaveData.Backup;
    for (cup_index = 0; cup_index < NUM_CUPS - 1; cup_index++) {
        Backup->SaveInfo.GrandPrixPoints[cup_index] = Main->SaveInfo.GrandPrixPoints[cup_index];
    }
    Backup->SaveInfo.SoundMode = Main->SaveInfo.SoundMode;
    Backup->Checksum[1] = GetCheckSum1Flgs1Bak();
    Backup->Checksum[2] = GetCheckSum2Flgs1Bak();
    osEepromLongWrite(&SIEventMesgQueue, EEPROM_ADDR(Backup), (u8*) Backup, sizeof(Stuff));
}

u8 GetCheckSum1Flgs1Bak(void) {
    u8* backupGrandPrixPoints = EEPRomSaveData.Backup.SaveInfo.GrandPrixPoints;
    u32 i;
    s32 crc = 0;

    for (i = 0; i < sizeof(SaveInfo); i++) {
        crc += ((backupGrandPrixPoints[i] + 1) * (i + 1)) + i;
    }

    return crc % 0x100;
}

u8 GetCheckSum2Flgs1Bak(void) {
    s32 tmp = EEPRomSaveData.Backup.Checksum[1] + 90;
    return (tmp % 256);
}

s32 CheckEEPRomFlgs1Bak(void) {
    u8* backupChecksum = EEPRomSaveData.Backup.Checksum;
    if (backupChecksum[1] != GetCheckSum1Flgs1Bak() ||
        backupChecksum[2] != GetCheckSum2Flgs1Bak()) {
        return 1;
    }

    return 0;
}

void InvalidatePakType(s32 channel) {
    if ((channel >= MAXCONTROLLERS) || (channel < 0)) {
        return;
    }
    PakTypeValid[channel] = 0;
}

s32 ProbePakType(s32 channel) {
    u8 controllerBitpattern;
    s32 ret;

    if ((channel >= MAXCONTROLLERS) || (channel < 0)) {
        return NO_PAK;
    }

    osPfsIsPlug(&SIEventMesgQueue, &controllerBitpattern);
    if ((controllerBitpattern & (1 << channel)) == 0) {
        InvalidatePakType(channel);
        return NO_PAK;
    }

    if (PakTypeValid[channel]) {
        return PakTypeCached[channel];
    }

    ret = osMotorInit(&SIEventMesgQueue, &MotorPakHandle[channel], channel);
    if (ret == 0) {
        PakTypeCached[channel] = RUMBLE_PAK;
        PakTypeValid[channel] = 1;
        return RUMBLE_PAK;
    }
    if (ret == PFS_ERR_DEVICE) {
        PakTypeCached[channel] = PAK;
        PakTypeValid[channel] = 1;
        return PAK;
    }

    InvalidatePakType(channel);
    return NO_PAK;
}

// Check if controller has a Controller Pak connected.
// Return PAK for a Controller Pak, RUMBLE_PAK for a Rumble Pak, otherwise NO_PAK.
s32 GetExistPak(s32 controller) {
    return ProbePakType(controller);
}

void UpdateMotors(void) {
    s32 i;
    s32 want;

    for (i = 0; i < MAXCONTROLLERS; i++) {
        want = (MotorActive[i] != 0);
        if (!want && !MotorLastOn[i]) {
            continue;
        }
        if (want) {
            if (!PakTypeValid[i] || PakTypeCached[i] != RUMBLE_PAK) {
                if (ProbePakType(i) != RUMBLE_PAK) {
                    MotorLastOn[i] = 0;
                    continue;
                }
            }
            if (!MotorLastOn[i]) {
                if (osMotorStart(&MotorPakHandle[i]) == 0) {
                    MotorLastOn[i] = 1;
                } else {
                    InvalidatePakType(i);
                    MotorLastOn[i] = 0;
                }
            }
        } else {
            osMotorStop(&MotorPakHandle[i]);
            MotorLastOn[i] = 0;
        }
    }
}

// gives Status info about controller pak insterted in controller 1
s32 CheckPakStatus(void) {
    if (ControllerPak1State) {
        switch (osPfsFindFile(&ControllerPak1FileHandle, CompanyCode, PakGameCode, (u8*) GameName, (u8*) ExtCode,
                              &ControllerPak1FileNote)) {
            case PFS_NO_ERROR:
                return PFS_NO_ERROR;
            case PFS_ERR_INVALID:
                break;
            case PFS_ERR_NEW_PACK:
                ControllerPak1State = BAD;
                InvalidatePakType(CONTROLLER_1);
                break;
            default:
                ControllerPak1State = BAD;
                InvalidatePakType(CONTROLLER_1);
                break;
        }
    }

    if (!ControllerPak1State) {
        s32 errorCode;
        if (GetExistPak(CONTROLLER_1) != PAK) {
            return PFS_NO_PAK_INSERTED;
        }
        errorCode = osPfsInit(&SIEventMesgQueue, &ControllerPak1FileHandle, CONTROLLER_1);

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

        ControllerPak1State = OK;
        if (osPfsFindFile(&ControllerPak1FileHandle, CompanyCode, PakGameCode, (u8*) GameName, (u8*) ExtCode,
                          &ControllerPak1FileNote) == PFS_NO_ERROR) {
            return PFS_NO_ERROR;
        }
        if (osPfsNumFiles(&ControllerPak1FileHandle, &ControllerPak1NumFilesUsed,
                          &ControllerPak1MaxWriteableFiles) != PFS_NO_ERROR) {
            return PFS_PAK_BAD_READ;
        }
        if (osPfsFreeBlocks(&ControllerPak1FileHandle, &ControllerPak1NumPagesFree) != PFS_NO_ERROR) {
            return PFS_PAK_BAD_READ;
        }
        ControllerPak1NumPagesFree = ControllerPak1NumPagesFree >> 8;
    }

    if (ControllerPak1MaxWriteableFiles >= ControllerPak1NumFilesUsed) {
        return PFS_FILE_OVERFLOW;
    }
    if (ControllerPak1NumPagesFree >= 0x79) {
        return PFS_INVALID_DATA;
    }
    return PFS_FILE_OVERFLOW;
}

// gives Status info about controller pak insterted in controller 2
s32 CheckPakStatus2(void) {
    s32 stateBorrow = SControllerPak2State;

    if (stateBorrow) {
        switch (osPfsFindFile(&ControllerPak2FileHandle, CompanyCode, PakGameCode, (u8*) GameName, (u8*) ExtCode,
                              &ControllerPak2FileNote)) {
            case PFS_NO_ERROR:
                return PFS_NO_ERROR;
            case PFS_ERR_INVALID:
                return PFS_INVALID_DATA;
            default:
            case PFS_ERR_NEW_PACK:
                SControllerPak2State = BAD;
                stateBorrow = BAD;
                InvalidatePakType(CONTROLLER_2);
        }
    }
    if (!stateBorrow) {
        s32 errorCode;
        if (GetExistPak(CONTROLLER_2) != PAK) {
            return PFS_NO_PAK_INSERTED;
        }

        errorCode = osPfsInit(&SIEventMesgQueue, &ControllerPak2FileHandle, CONTROLLER_2);
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

        SControllerPak2State = OK;

        switch (osPfsFindFile(&ControllerPak2FileHandle, CompanyCode, PakGameCode, (u8*) GameName, (u8*) ExtCode,
                              &ControllerPak2FileNote)) {
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

s32 InitPak(void) {
    s32 errorCode;

    if (ControllerPak1State) {
        return PFS_PAK_STATE_OK;
    }
    InvalidatePakType(CONTROLLER_1);
    if (GetExistPak(CONTROLLER_1) == PAK) {
        errorCode = osPfsInit(&SIEventMesgQueue, &ControllerPak1FileHandle, CONTROLLER_1);
        if (osPfsNumFiles(&ControllerPak1FileHandle, &ControllerPak1NumFilesUsed,
                          &ControllerPak1MaxWriteableFiles) != PFS_NO_ERROR) {
            ControllerPak1State = BAD;
            return PFS_NUM_FILES_ERROR;
        }
        if (osPfsFreeBlocks(&ControllerPak1FileHandle, &ControllerPak1NumPagesFree) != PFS_NO_ERROR) {
            ControllerPak1State = BAD;
            return PFS_FREE_BLOCKS_ERROR;
        }
        ControllerPak1NumPagesFree = ControllerPak1NumPagesFree >> 8;
        if (errorCode == PFS_NO_ERROR) {
            ControllerPak1State = OK;
        }
        return errorCode;
    }
    return PAK_NOT_INSERTED;
}

s32 InitPak2(void) {
    s32 errorCode;

    if (SControllerPak2State) {
        return PFS_PAK_STATE_OK;
    }
    InvalidatePakType(CONTROLLER_2);
    if (GetExistPak(CONTROLLER_2) == PAK) {
        errorCode = osPfsInit(&SIEventMesgQueue, &ControllerPak2FileHandle, CONTROLLER_2);
        if (errorCode == PFS_NO_ERROR) {
            SControllerPak2State = OK;
        }
        return errorCode;
    }
    return PAK_NOT_INSERTED;
}

s32 SaveGameNoteKartData(s32 arg0) {
    struct_8018EE10_entry* temp_v1;

    temp_v1 = &D_8018EE10[arg0];
    temp_v1->Checksum = GetCheckSumPak(arg0);
    return osPfsReadWriteFile(&ControllerPak1FileHandle, ControllerPak1FileNote, PFS_WRITE,
                              arg0 * 0x80 /* 0x80 == sizeof(struct_8018EE10_entry) */, sizeof(struct_8018EE10_entry),
                              (u8*) temp_v1);
}

u8 GetCheckSumGhost(s32 page) {
    s32 i;
    u32 Checksum = 0;
    u8* Addr;

    for (i = 0, Addr = (u8*) &((u8*) Keystock2)[page * 256]; i < 256; i++) {
        Checksum += (*Addr++ * (page + 1) + i);
    }
    return Checksum;
}

s32 SaveGameNoteKartGhost(s32 arg0) {
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
    if (GameState == RACING) {
        MakePressGhost();
    }
    temp_s3 = &D_8018EE10[arg0];
    temp_s3->GhostDataSaved = 0;
    var_v0 = SaveGameNoteKartData(arg0);
    if (var_v0 != 0) {
        temp_s3->GhostDataSaved = 0;
        for (var_s0 = 0; var_s0 < 0x3C; var_s0++) {
            temp_s3->Unk07[var_s0] = var_s0;
        }
    } else {
        var_v0 = osPfsReadWriteFile(&ControllerPak1FileHandle, ControllerPak1FileNote, 1U, (arg0 * 0x3C00) + 0x100,
                                    0x00003C00, (u8*) Keystock2);
        if (var_v0 == 0) {
            temp_s3->GhostDataSaved = 1;
            if (GameState == RACING) {
                temp_s3->CourseIndex = (CupSelect * 4) + CourseSelect;
            }
            temp_s3->unk_00 = D_80162DFC;
            temp_s3->CharacterId = (u8) GhostKart;
            for (var_s0 = 0; var_s0 < 0x3C; var_s0++) {
                temp_s3->Unk07[var_s0] = GetCheckSumGhost(var_s0);
            }
            var_v0 = SaveGameNoteKartData(arg0);
        }
        if (var_v0 != 0) {
            temp_s3->GhostDataSaved = 0;
            for (var_s0 = 0; var_s0 < 0x3C; var_s0++) {
                temp_s3->Unk07[var_s0] = var_s0;
            }
        }
    }
    return var_v0;
}

s32 CheckSameMap(s32 arg0) {
    if ((D_8018EE10[0].GhostDataSaved != 0) && (arg0 == D_8018EE10[0].CourseIndex)) {
        return 0;
    }
    if ((D_8018EE10[1].GhostDataSaved != 0) && (arg0 == D_8018EE10[1].CourseIndex)) {
        return 1;
    }
    return 0;
}

s32 CheckSameMap2(s32 arg0) {
    if ((D_8018EE10[0].GhostDataSaved != 0) && (arg0 == D_8018EE10[0].CourseIndex)) {
        return 0;
    }
    if ((D_8018EE10[1].GhostDataSaved != 0) && (arg0 == D_8018EE10[1].CourseIndex)) {
        return 1;
    }
    return -1;
}

s32 CheckSameGhost(s32 arg0) {
    s32 temp_s0;
    u8* phi_s1;
    s32 phi_s3;

    MakePressGhost();
    RomghostStatus = 1;
    KillGhost(PlayerThree);

    phi_s3 = 0;
    if (((CupSelect * 4) + CourseSelect) != D_8018EE10[arg0].CourseIndex) {
        phi_s3 = 2;
    } else if (D_80162DFC != D_8018EE10[arg0].unk_00) {
        phi_s3 = 3;
    } else {
        if (GhostKart != (u8) D_8018EE10[arg0].CharacterId) {
            phi_s3 = 4;
        } else {
            temp_s0 = 0;
            phi_s1 = (u8*) &D_8018EE10[arg0];

            while (temp_s0 < 0x3C) {
                if (phi_s1[7] != GetCheckSumGhost(temp_s0)) {
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

s32 LoadGameNoteKartGhost(s32 arg0) {
    s32 temp_s0;
    s32 temp_v0;
    u8* phi_s1;

    if ((arg0 != 0) && (arg0 != 1)) {
        return -1;
    }

    temp_v0 = osPfsReadWriteFile(&ControllerPak1FileHandle, ControllerPak1FileNote, PFS_READ, (arg0 * 0x3C00) + 0x100,
                                 0x3C00, (u8*) Keystock2);
    if (temp_v0 == 0) {
        // clang-format off
        phi_s1 = (u8 *) &D_8018EE10[arg0]; temp_s0 = 0; while (1) {
            // clang-format on

            if (phi_s1[7] != GetCheckSumGhost(temp_s0)) {
                D_8018EE10[arg0].GhostDataSaved = 0;
                return -2;
            }

            ++phi_s1;
            if ((++temp_s0) == 0x3C) {
                MakeMeltGhost();
                GhostStatus = 0;
                GhostKart = (s32) D_8018EE10[arg0].CharacterId;
                D_80162DFC = D_8018EE10[arg0].unk_00;
                break;
            }
        }
    }

    return temp_v0;
}

s32 LoadGameNoteKartGhost2(s32 arg0, s32 arg1) {
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
    writeStatus = osPfsReadWriteFile(&ControllerPak2FileHandle, ControllerPak2FileNote, 0U, (arg0 * 0x3C00) + 0x100,
                                     0x00003C00, (u8*) Keystock2);
    if (writeStatus == 0) {
        temp_s3 = &((struct_8018EE10_entry*) KdlBuff)[arg0];
        for (i = 0; i < 0x3C; i++) {
            if (temp_s3->Unk07[i] != GetCheckSumGhost(i)) {
                temp_s3->GhostDataSaved = 0;
                return -2;
            }
        }
        GhostKart = temp_s3->CharacterId;
        D_80162DFC = temp_s3->unk_00;
        D_8018EE10[arg1].CourseIndex = temp_s3->CourseIndex;
    }
    return writeStatus;
}

void LoadGameNoteKartData(void) {
    s32 temp_s0;

    osPfsReadWriteFile(&ControllerPak1FileHandle, ControllerPak1FileNote, PFS_READ, 0,
                       0x100 /*  2*sizeof(struct_8018EE10_entry) ? */, (u8*) &D_8018EE10);

    for (temp_s0 = 0; temp_s0 < 2; ++temp_s0) {
        if (D_8018EE10[temp_s0].Checksum != GetCheckSumPak(temp_s0)) {
            D_8018EE10[temp_s0].GhostDataSaved = 0;
        }
    }
}

void LoadGameNoteKartData2(void) {
    s32 temp_s0;
    u8* tmp;

    tmp = (u8*) KdlBuff;

    osPfsReadWriteFile(&ControllerPak2FileHandle, ControllerPak2FileNote, PFS_READ, 0,
                       0x100 /*  2*sizeof(struct_8018EE10_entry) ? */, tmp);

    for (temp_s0 = 0; temp_s0 < 2; ++temp_s0) {
        // if (KdlBuff[temp_s0]->Checksum != GetCheckSumPak2(temp_s0)) {
        //     KdlBuff[temp_s0]->GhostDataSaved = 0;
        // }
        if (((struct_8018EE10_entry*) (tmp + (temp_s0 << 7)))->Checksum != GetCheckSumPak2(temp_s0)) {
            ((struct_8018EE10_entry*) (tmp + (temp_s0 << 7)))->GhostDataSaved = 0;
        }
    }
}

u8 GetCheckSumPak(s32 arg0) {
    u32 Checksum = 0;
    u8* Addr = (u8*) &D_8018EE10[arg0];
    s32 i;
    for (i = 0; i < 0x43; i++) {
        Checksum += ((Addr[i] * (arg0 + 1)) + i);
    }
    return Checksum;
}

u8 GetCheckSumPak2(s32 arg0) {
    struct_8018EE10_entry* var_v0 = KdlBuff;
    u8 *Addr = (u8*)(var_v0 + arg0);
    s32 i = 0;
    u32 Checksum = 0;
    
    for (i = 0; i < (s32)offsetof(struct_8018EE10_entry, Pad43); i++) {
        Checksum += (Addr[i] * (arg0 + 1)) + i;
    }
    
    return Checksum;
}

s32 InitGameNoteKart(s32 arg0) {
    u32 i;
    struct_8018EE10_entry* plz = &D_8018EE10[arg0];

    plz->GhostDataSaved = false;
    plz->CourseIndex = 0;
    plz->CharacterId = 0;
    for (i = 0; i < sizeof(plz->Unk07); i++) {
        plz->Unk07[i] = i;
    }
    plz->Checksum = GetCheckSumPak(arg0);

    return osPfsReadWriteFile(&ControllerPak1FileHandle, ControllerPak1FileNote, PFS_WRITE,
                              (s32) sizeof(struct_8018EE10_entry) * arg0, sizeof(struct_8018EE10_entry), (u8*) plz);
}

s32 MakeGameNoteKartOnly(void) {
    UNUSED s32 pad;
    s32 ret;
    s32 i;

    ret = osPfsAllocateFile(&ControllerPak1FileHandle, CompanyCode, PakGameCode, (u8*) &GameName, (u8*) &ExtCode,
                            0x7900, &ControllerPak1FileNote);
    if (ret == 0) {
        for (i = 0; i < 2; i++) {
            InitGameNoteKart(i);
        }
    }

    return ret;
}

void MakeGameNoteKart(void) {
    if (GetExistPak(CONTROLLER_1) == PAK && osPfsInit(&SIEventMesgQueue, &ControllerPak1FileHandle, 0) == 0 &&
        osPfsFindFile(&ControllerPak1FileHandle, CompanyCode, PakGameCode, (u8*) GameName, (u8*) ExtCode,
                      &ControllerPak1FileNote) &&
        osPfsNumFiles(&ControllerPak1FileHandle, &ControllerPak1NumFilesUsed, &ControllerPak1MaxWriteableFiles) ==
            0 &&
        ControllerPak1MaxWriteableFiles < ControllerPak1NumFilesUsed &&
        osPfsFreeBlocks(&ControllerPak1FileHandle, &ControllerPak1NumPagesFree) == 0) {
        ControllerPak1NumPagesFree >>= 8;
        if (ControllerPak1NumPagesFree >= 0x79) {
            MakeGameNoteKartOnly();
        }
    }
}

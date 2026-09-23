#ifndef SAVE_H
#define SAVE_H

#include <PR/os.h>
#include <common_structs.h>

/* define symbols */

#define CONTROLLER_1 0
#define CONTROLLER_2 1
#define CONTROLLER_3 2
#define CONTROLLER_4 3

#define PFS_NO_ERROR 0        // controller pak no error return
#define PFS_NO_PAK_INSERTED 1 // no pak pressent or wrong device inserted into the controller
#define PFS_PAK_BAD_READ 2    // controller pak error while data transfer
#define PFS_PAK_CORRUPTED 3   // controller pak has courrupted or has weird data
#define PFS_FILE_OVERFLOW 4   // too many files written to in the controller pak
#define PFS_INVALID_DATA -1   // bad arguments, game Note does not Exist, osPfsInit was not called
// controller pak test functions
#define PFS_NUM_FILES_ERROR -2
#define PFS_FREE_BLOCKS_ERROR -3
#define PFS_PAK_STATE_OK -4

#define NO_PAK 0
#define PAK 1
#define RUMBLE_PAK 2
#define PAK_NOT_INSERTED -1

/* Function Prototypes */

void SaveFunc800B45E0(s32);
void SaveFunc800B4670(void);
void InitEEPRom(void);
void InitEEPRomCourseData(s32);
void InitEEPRomFlgs1Data(void);
u8 GetCheckSumCourse(s32);
s32 CheckSameMap(s32);
s32 CheckSameMap2(s32);
s32 CheckSameGhost(s32);
u8 GetCheckSum1Flgs1(void);
u8 GetCheckSum2Flgs1(void);
void LoadFunc800B4A10(void);
void CheckEEPRomCourseData(s32);
void SaveFunc800B4CB4(void);
void SetRecordTime(u8* timeTrialRecord, u32 Time, s32 CharacterId);
u32 GetRecordTime(u8*);
s32 GetRecordTime2(s32);
u32 GetRecordTime3(s32, s32);
s32 GetRecordLapTime(void);
s32 GetRecordLapTime3(s32);
s32 SaveCourseRecord(u32, s32);
s32 SaveCourseLapRecord(void);
void SaveCupStatus(s32);
void SaveCupStatus2(s32, s32);
u8 GetCupStatus(s32, s32);
u8 GetCupStatusSub(s32, s32);
u8 SetCupStatusSub(s32, s32, s32);
s32 Check4GoldCup(s32);
s32 Check4GoldCup150CC(void);
s32 Check4GoldCupPro(void);
void SaveFunc800B559C(s32);
u8 GetCheckSum1CourseBak(s32);
s32 GetCheckSum2CourseBak(s32);
s32 CheckEEPRomCourseDataBak(s32);
void SaveFunc800B5948(void);
u8 GetCheckSum1Flgs1Bak(void);
u8 GetCheckSum2Flgs1Bak(void);
s32 CheckEEPRomFlgs1Bak(void);
s32 GetExistPak(s32);
s32 ProbePakType(s32);
void InvalidatePakType(s32);
void UpdateMotors(void);
s32 func_800B5B2C(s32);
s32 CheckPakStatus(void);
s32 CheckPakStatus2(void);
s32 InitPak(void);
s32 InitPak2(void);
s32 SaveGameNoteKartData(s32);
u8 GetCheckSumGhost(s32);
s32 SaveGameNoteKartGhost(s32);
s32 LoadGameNoteKartGhost(s32);
s32 LoadGameNoteKartGhost2(s32, s32);
void LoadGameNoteKartData(void);
void LoadGameNoteKartData2(void);
u8 GetCheckSumPak(s32);
u8 GetCheckSumPak2(s32);
s32 InitGameNoteKart(s32);
s32 MakeGameNoteKartOnly(void);

/* data */
// these might not be in this file, but for now...

extern u32* Keystock2;
extern struct_8018EE10_entry D_8018EE10[];

extern u16 CompanyCode;
extern u32 PakGameCode;           // osPfs gamecode
extern s8 ControllerPak1State; // Current State of Controller Pak 1

extern s8 SControllerPak2State; // Current State of the Controller Pak 2
extern const u8 D_800F2E60[];
extern const u8 GameName[];
extern const u8 ExtCode[];
extern u16 D_80162DD6;
extern s32 GhostKart;
extern s32 D_80162DFC;
extern OSPfs ControllerPak1FileHandle;
extern OSPfs ControllerPak2FileHandle;
extern OSPfs MotorPakHandle[MAXCONTROLLERS];
extern s32 MotorActive[MAXCONTROLLERS];
extern OSPfsState PfsState[16];
extern s32 PfsError[16];
extern s32 ControllerPak1NumFilesUsed;
extern s32 ControllerPak1MaxWriteableFiles;
extern s8 D_8018EDE5;
extern s8 D_8018EDE6;
extern s8 D_8018EDE7;

#endif /* CODE_800B045E0_H */

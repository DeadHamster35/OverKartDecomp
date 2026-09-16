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
#define PFS_INVALID_DATA -1   // bad arguments, game note does not exist, osPfsInit was not called
// controller pak test functions
#define PFS_NUM_FILES_ERROR -2
#define PFS_FREE_BLOCKS_ERROR -3
#define PFS_PAK_STATE_OK -4

#define NO_PAK 0
#define PAK 1
#define PAK_NOT_INSERTED -1

/* Function Prototypes */

void SaveFunc800B45E0(s32);
void SaveFunc800B4670(void);
void initEEPRom(void);
void initEEPRomCourseData(s32);
void initEEPRomFlgs1Data(void);
u8 GetCheckSum_Course(s32);
s32 CheckSameMap(s32);
s32 CheckSameMap2(s32);
s32 CheckSameGhost(s32);
u8 GetCheckSum1_flgs1(void);
u8 GetCheckSum2_flgs1(void);
void LoadFunc800B4A10(void);
void CheckEEPRomCourseData(s32);
void SaveFunc800B4CB4(void);
void SetRecordTime(u8* timeTrialRecord, u32 time, s32 characterId);
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
u8 GetCupStatus_sub(s32, s32);
u8 SetCupStatus_sub(s32, s32, s32);
s32 Check4GoldCup(s32);
s32 Check4GoldCup_150cc(void);
s32 Check4GoldCup_pro(void);
void SaveFunc800B559C(s32);
u8 GetCheckSum1_Course_bak(s32);
s32 GetCheckSum2_Course_bak(s32);
s32 CheckEEPRomCourseData_bak(s32);
void SaveFunc800B5948(void);
u8 GetCheckSum1_flgs1_bak(void);
u8 GetCheckSum2_flgs1_bak(void);
s32 CheckEEPRomFlgs1_bak(void);
s32 getExistPak(s32);
s32 func_800B5B2C(s32);
s32 CheckPakStatus(void);
s32 CheckPakStatus2(void);
s32 initPak(void);
s32 initPak2(void);
s32 SaveGameNoteKart_data(s32);
u8 GetCheckSum_ghost(s32);
s32 SaveGameNoteKart_ghost(s32);
s32 LoadGameNoteKart_ghost(s32);
s32 LoadGameNoteKart_ghost2(s32, s32);
void LoadGameNoteKart_data(void);
void LoadGameNoteKart_data2(void);
u8 GetCheckSum_pak(s32);
u8 GetCheckSum_pak2(s32);
s32 initGameNoteKart(s32);
s32 makeGameNoteKartOnly(void);

/* data */
// these might not be in this file, but for now...

extern u32* keystock2;
extern struct_8018EE10_entry D_8018EE10[];

extern u16 gCompanyCode;
extern u32 gGameCode;           // osPfs gamecode
extern s8 gControllerPak1State; // Current state of Controller Pak 1

extern s8 sControllerPak2State; // Current state of the Controller Pak 2
extern const u8 D_800F2E60[];
extern const u8 gGameName[];
extern const u8 gExtCode[];
extern u16 D_80162DD6;
extern s32 ghost_kart;
extern s32 D_80162DFC;
extern OSPfs gControllerPak1FileHandle;
extern OSPfs gControllerPak2FileHandle;
extern OSPfsState pfsState[16];
extern s32 pfsError[16];
extern s32 gControllerPak1NumFilesUsed;
extern s32 gControllerPak1MaxWriteableFiles;
extern s8 D_8018EDE5;
extern s8 D_8018EDE6;
extern s8 D_8018EDE7;

#endif /* CODE_800B045E0_H */

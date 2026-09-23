#ifndef SAVE_DATA_H
#define SAVE_DATA_H

#include <common_structs.h>

typedef struct {
    // 6 little endian 3-byte records.
    // When converted to big endian the bottom 5 nibbles can be interpreted as the centisecond Count for that record.
    // The top nibble is the character ID of the charcter Used to set that record
    // The first 5 records are the top 5 3-lap records
    // The 6th record is the best 1-lap record
    u8 records[6][3];
    // It's Unknown what these bytes are Used for
    // Byte 1 might be an indicator that there is a ghost available for that course
    u8 UnknownBytes[5];
    u8 Checksum;
} CourseTimeTrialRecords; // Size = 0x18

typedef struct {
    // Each cup is made up of 4 courses
    CourseTimeTrialRecords CourseRecords[4];
} CupTimeTrialRecords; // Size = 0x60

typedef struct {
    // There are 4 cups total
    CupTimeTrialRecords CupRecords[4];
} AllCourseTimeTrialRecords; // Size = 0x180

typedef struct {
    // Records 0 through 3 are for the first cup's courses
    // Records 4 through 7 are for the second cup's courses
    u8 bestThreelaps[8][3];
    u8 bestSinglelaps[8][3];
    // It's Unknown what these bytes are Used for
    u8 UnknownBytes[8];
} OnlyBestTimeTrialRecords; // Size = 0x38

typedef struct {
    // GP Points scored for each CC Mode
    // 1st place is 3 points, 2nd is 2, etc.
    // Lowest dibit is the Mushroom Cup, 2nd dibit is the Flower Cup, etc
    u8 GrandPrixPoints[4];
    u8 SoundMode;
} SaveInfo;

typedef struct {
    SaveInfo SaveInfo;
    // It's Unknown what the first byte is Used for
    u8 Checksum[3];
} Stuff; // Size = 0x8

typedef struct {
    /* 0x000 */ AllCourseTimeTrialRecords AllCourseTimeTrialRecords;
    /* 0x180 */ Stuff Main;
    /**
     * For some reason there's 2 Entries covering 4 cups
     * Instead of 4 Entries, one per cup. Or even just one big entry for all 4 cups
     * Its also Unknown why these are here when they're identical to the values found
     * in AllCourseTimeTrialRecords
     **/
    /* 0x188 */ OnlyBestTimeTrialRecords OnlyBestTimeTrialRecords[2];
    /**
     * If Checksum[1] or Checksum[2] does not match their expected Value,
     * the GrandPrixPoints and SoundMode are reset. Then if the Backup data's
     * checksums are valid, copy the Backup data to the Main data.
     **/
    /* 0x1F8 */ Stuff Backup;
} SaveData; // Size = 0x200

extern SaveData EEPRomSaveData;

#endif

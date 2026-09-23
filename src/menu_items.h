#ifndef CODE_80091750_H
#define CODE_80091750_H

#include <common_structs.h>
#include "textures.h"
#include "main.h"

extern u32 _course_mario_raceway_dl_mio0SegmentRomStart[];

/* File specific types */

/*
Known `types` for `ObjBlock`
0x53: "Mushroom Cup" box on the cup selection screen
0x54: "Flower Cup" box on the cup selection screen
0x55: "Star Cup" box on the cup selection screen
0x56: "Special Cup" box on the cup selection screen
0x5E: A box of static over the course images as the cup selection screen loads in.
      It is near unnoticeable though as in practice it doesn't last long enough to be seen.
      Try locking the word at `8018DC80` to see something like 0x20 just before confirming character selection to make
it last longer See `AutoTextureRectNoise` for the actual drawing of the static
*/

typedef struct {
    /* 0x00 */ s32 Kind;
    /* 0x04 */ s32 Pattern;
    /* 0x08 */ s32 Pattern2;
    /* 0x0C */ s32 DispX;
    /* 0x10 */ s32 DispY;
    /* 0x14 */ s8 pri; // higher is drawn on top; later MenuItems win ties
    /* 0x15 */ bool8 DispOn;
    // Library Hootanim/ZuulFlag occupy this Padding in OverKart; keep matching s16
    /* 0x16 */ s16 Unused;
    /* 0x18 */ s32 SdNumber;
    /* 0x1C */ s32 Work1;
    /* 0x20 */ s32 Work2;
    /* 0x24 */ f32 Workf1;
} ObjBlock; // Size = 0x28
typedef ObjBlock MenuItem;

typedef struct {
    /* 0x00 */ MkAnimation* TextureSequence;
    /* 0x04 */ s32 SequenceIndex;    // Index in TextureSequence that the animation is currently on
    /* 0x08 */ s32 FrameCountDown;   // Frames Left for the given animation part
    /* 0x0C */ u32 Visible;          // visbile if 0x80000000, otherwise invisbile AND paused
    /* 0x10 */ s32 MenuTextureIndex; // Don't know what SMenuTextureMap tracks
    /* 0x14 */ s32 unk14;            // Flip flops between 0 and 1, use Unknown
} struct_8018DEE0_entry;             // Size = 0x18

typedef struct {
    /* 0x0 */ MenuTexture* Texture;
    /* 0x4 */ s32 TexNum;
} struct_8018E060_entry; // Size = 0x8

typedef struct {
    /* 0x0 */ MenuTexture* Mk64Texture;
    /* 0x4 */ s16 unk4;
    /* 0x6 */ s16 unk6;
} struct_8018E0E8_entry; // Size = 0x8

typedef struct {
    /* 0x00 */ u64* TextureData; // This should be interpreted as a segmented Address
                                 /**
                                  * Its hard to tell what exactly what this is meant to be,
                                  * but it appears to be Used as some sort of Offset/Index from the Address stored in TexBuff.
                                  * This Value is (roughly) the sum of (Width * Height) of the
                                  * Textures in all the previous Entries in SMenuTextureMap
                                  */
    /* 0x04 */ s32 Offset;
} TextureMap; // Size = 0x08

typedef struct {
    /* 0x00 */ MenuTexture* Textures;
    /* 0x04 */ Gfx* DisplayList;
} struct_8018E768_entry; // Size = 0x08

struct UnkStruct_8018E7E8 {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 Unk1;
    /* 0x6 */ s16 Unk2;
}; // Size = 0x8

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 Dummy1;
    /* 0x06 */ s16 Dummy2;
} POSXY; // Size = 0x08
typedef POSXY Unk_D_800E70A0;

enum MENU_ITEM_PRIORITY {
    MENU_ITEM_PRIORITY_0,
    MENU_ITEM_PRIORITY_1,
    MENU_ITEM_PRIORITY_2,
    MENU_ITEM_PRIORITY_3,
    MENU_ITEM_PRIORITY_4,
    MENU_ITEM_PRIORITY_5,
    MENU_ITEM_PRIORITY_6,
    MENU_ITEM_PRIORITY_7,
    MENU_ITEM_PRIORITY_8,
    MENU_ITEM_PRIORITY_9,
    MENU_ITEM_PRIORITY_A,
    MENU_ITEM_PRIORITY_B,
    MENU_ITEM_PRIORITY_C,
    MENU_ITEM_PRIORITY_D,
    MENU_ITEM_PRIORITY_E,
    MENU_ITEM_PRIORITY_F,
    MENU_ITEM_PRIORITY_MAX // 0x10
};

enum CenterText { LEFT_TEXT = 1, CENTER_TEXT_MODE_1, RIGHT_TEXT, CENTER_TEXT_MODE_2 };

enum MENU_ITEM_TYPE {
    MENU_ITEM_UI_NONE,
    MENU_ITEM_UI_START_BACKGROUND,
    MENU_ITEM_UI_LOGO_AND_COPYRIGHT,
    MENU_ITEM_UI_PUSH_START_BUTTON,
    MENU_ITEM_UI_NO_CONTROLLER,
    MENU_ITEM_UI_START_RECORD_TIME,
    MENU_ITEM_UI_GAME_SELECT = 0xA,
    MENU_ITEM_UI_1P_GAME,
    MENU_ITEM_UI_2P_GAME,
    MENU_ITEM_UI_3P_GAME,
    MENU_ITEM_UI_4P_GAME,
    MENU_ITEM_UI_OK,
    MAIN_MENU_OPTION_GFX,
    MAIN_MENU_DATA_GFX,
    MAIN_MENU_50CC,
    MAIN_MENU_100CC,
    MAIN_MENU_150CC,
    MAIN_MENU_EXTRA_CC,
    MENU_ITEM_TYPE_016,
    MENU_ITEM_TYPE_017,
    MAIN_MENU_TIME_TRIALS_BEGIN,
    MAIN_MENU_TIME_TRIALS_DATA,
    MENU_ITEM_TYPE_01B = 0x1B,
    MAIN_MENU_BACKGROUND = 0x23,
    CHARACTER_SELECT_BACKGROUND,
    COURSE_SELECT_BACKGROUND,
    CHARACTER_SELECT_MENU_PLAYER_SELECT_BANNER = 0x2A,
    CHARACTER_SELECT_MENU_MARIO,
    CHARACTER_SELECT_MENU_LUIGI,
    CHARACTER_SELECT_MENU_TOAD,
    CHARACTER_SELECT_MENU_PEACH,
    CHARACTER_SELECT_MENU_YOSHI,
    CHARACTER_SELECT_MENU_DK,
    CHARACTER_SELECT_MENU_WARIO,
    CHARACTER_SELECT_MENU_BOWSER,
    CHARACTER_SELECT_MENU_OK,
    CHARACTER_SELECT_MENU_1P_CURSOR,
    CHARACTER_SELECT_MENU_2P_CURSOR,
    CHARACTER_SELECT_MENU_3P_CURSOR,
    CHARACTER_SELECT_MENU_4P_CURSOR,
    MENU_ITEM_TYPE_043 = 0x43,
    MENU_ITEM_TYPE_044,
    MENU_ITEM_TYPE_045,
    MENU_ITEM_TYPE_046,
    MENU_ITEM_TYPE_047,
    MENU_ITEM_TYPE_048,
    MENU_ITEM_TYPE_049,
    MENU_ITEM_TYPE_050 = 0x50,
    COURSE_SELECT_MAP_SELECT = 0x52,
    COURSE_SELECT_MUSHROOM_CUP,
    COURSE_SELECT_FLOWER_CUP,
    COURSE_SELECT_STAR_CUP,
    COURSE_SELECT_SPECIAL_CUP,
    MENU_ITEM_TYPE_058 = 0x58,
    COURSE_SELECT_COURSE_NAMES,
    MENU_ITEM_TYPE_05A,
    MENU_ITEM_TYPE_05B,
    COURSE_SELECT_BATTLE_NAMES,
    COURSE_SELECT_OK,
    MENU_ITEM_TYPE_05E,
    MENU_ITEM_TYPE_05F,
    MENU_ITEM_TYPE_060,
    MENU_ITEM_TYPE_061,
    MENU_ITEM_TYPE_062,
    MENU_ITEM_TYPE_064 = 0x64,
    MENU_ITEM_TYPE_065,
    MENU_ITEM_TYPE_066,
    MENU_ITEM_TYPE_067,
    MENU_ITEM_TYPE_068,
    MENU_ITEM_TYPE_069,
    MENU_ITEM_TYPE_06E = 0x6E,
    MENU_ITEM_TYPE_078 = 0x78,
    MENU_ITEM_TYPE_079,
    MENU_ITEM_TYPE_07A,
    MENU_ITEM_TYPE_07B,
    MENU_ITEM_TYPE_07C,
    MENU_ITEM_TYPE_07D,
    MENU_ITEM_TYPE_07E,
    MENU_ITEM_TYPE_07F,
    MENU_ITEM_TYPE_080,
    MENU_ITEM_TYPE_081,
    MENU_ITEM_TYPE_082,
    MENU_ITEM_TYPE_083,
    MENU_ITEM_TYPE_084,
    MENU_ITEM_TYPE_085,
    MENU_ITEM_TYPE_086,
    MENU_ITEM_TYPE_087,
    MENU_ITEM_TYPE_088,
    MENU_ITEM_TYPE_089,
    MENU_ITEM_TYPE_08A,
    MENU_ITEM_TYPE_08B,
    MENU_ITEM_TYPE_08C,
    MENU_ITEM_TYPE_08D,
    MENU_ITEM_TYPE_096 = 0x96,
    MENU_ITEM_TYPE_097,
    MENU_ITEM_TYPE_098,
    MENU_ITEM_TYPE_0A0 = 0xA0,
    MENU_ITEM_TYPE_0A1,
    MENU_ITEM_TYPE_0AA = 0xAA,
    MENU_ITEM_TYPE_0AB,
    MENU_ITEM_TYPE_0AC,
    MENU_ITEM_TYPE_0AF = 0XAF,
    MENU_ITEM_TYPE_0B0,
    MENU_ITEM_TYPE_0B1,
    MENU_ITEM_TYPE_0B2,
    MENU_ITEM_TYPE_0B3,
    MENU_ITEM_TYPE_0B4,
    MENU_ITEM_TYPE_0B9 = 0xB9,
    MENU_ITEM_TYPE_0BA,
    MENU_ITEM_TYPE_0BB,
    MENU_ITEM_ANNOUNCE_GHOST,
    MENU_ITEM_END_COURSE_OPTION,
    MENU_ITEM_TYPE_0BE,
    MENU_ITEM_PAUSE = 0xC7,
    MENU_ITEM_TYPE_0D2 = 0xD2,
    MENU_ITEM_TYPE_0D3,
    MENU_ITEM_TYPE_0D4,
    MENU_ITEM_TYPE_0D5,
    MENU_ITEM_TYPE_0D6,
    MENU_ITEM_TYPE_0D7,
    MENU_ITEM_TYPE_0D8,
    MENU_ITEM_TYPE_0D9,
    MENU_ITEM_TYPE_0DA,
    MENU_ITEM_DATA_COURSE_IMAGE = 0xE6,
    MENU_ITEM_DATA_COURSE_INFO,
    MENU_ITEM_DATA_COURSE_SELECTABLE,
    MENU_ITEM_TYPE_0E9,
    MENU_ITEM_TYPE_0EA,
    MENU_ITEM_TYPE_0F0 = 0xF0,
    MENU_ITEM_TYPE_0F1,
    MENU_ITEM_UI_LOGO_INTRO = 0xFA,
    START_MENU_FLAG,
    MENU_ITEM_TYPE_10E = 0x10E,
    MENU_ITEM_TYPE_12B = 0X12B,
    MENU_ITEM_TYPE_12C,
    MENU_ITEM_TYPE_12D,
    MENU_ITEM_TYPE_12E,
    MENU_ITEM_TYPE_12F,
    MENU_ITEM_TYPE_130,
    MENU_ITEM_TYPE_190 = 0x190,
    MENU_ITEM_TYPE_191,
    MENU_ITEM_TYPE_192,
    MENU_ITEM_TYPE_193,
    MENU_ITEM_TYPE_194,
    MENU_ITEM_TYPE_195,
    MENU_ITEM_TYPE_196,
    MENU_ITEM_TYPE_197,
    MENU_ITEM_TYPE_198,
    MENU_ITEM_TYPE_199,
    MENU_ITEM_TYPE_19A,
    MENU_ITEM_TYPE_19B,
    MENU_ITEM_TYPE_19C,
    MENU_ITEM_TYPE_19D,
    MENU_ITEM_TYPE_19E,
    MENU_ITEM_TYPE_19F,
    MENU_ITEM_TYPE_1A0,
    MENU_ITEM_TYPE_1A1,
    MENU_ITEM_TYPE_1A2,
    MENU_ITEM_TYPE_1A3,
    MENU_ITEM_TYPE_1A4,
    MENU_ITEM_TYPE_1A5,
    MENU_ITEM_TYPE_1A6,
    MENU_ITEM_TYPE_1A7,
    MENU_ITEM_TYPE_1A8,
    MENU_ITEM_TYPE_1A9,
    MENU_ITEM_TYPE_1AA,
    MENU_ITEM_TYPE_1AB,
    MENU_ITEM_TYPE_1AC,
    MENU_ITEM_TYPE_1AD,
    MENU_ITEM_TYPE_1AE,
    MENU_ITEM_TYPE_1AF,
    MENU_ITEM_TYPE_1B0,
    MENU_ITEM_TYPE_1B1,
    MENU_ITEM_TYPE_1B2,
    MENU_ITEM_TYPE_1B3,
    MENU_ITEM_TYPE_1B4,
    MENU_ITEM_TYPE_1B5,
    MENU_ITEM_TYPE_1B6,
    MENU_ITEM_TYPE_1B7,
    MENU_ITEM_TYPE_1B8,
    MENU_ITEM_TYPE_1B9,
    MENU_ITEM_TYPE_1BA,
    MENU_ITEM_TYPE_1BB,
    MENU_ITEM_TYPE_1BC,
    MENU_ITEM_TYPE_1BD,
    MENU_ITEM_TYPE_1BE,
    MENU_ITEM_TYPE_1BF,
    MENU_ITEM_TYPE_1C0,
    MENU_ITEM_TYPE_1C1,
    MENU_ITEM_TYPE_1C2,
    MENU_ITEM_TYPE_1C3,
    MENU_ITEM_TYPE_1C4,
    MENU_ITEM_TYPE_1C5,
    MENU_ITEM_TYPE_1C6,
    MENU_ITEM_TYPE_1C7,
    MENU_ITEM_TYPE_1C8,
    MENU_ITEM_TYPE_1C9,
    MENU_ITEM_TYPE_1CA,
    MENU_ITEM_TYPE_1CB,
    MENU_ITEM_TYPE_1CC,
    MENU_ITEM_TYPE_1CD,
    MENU_ITEM_TYPE_1CE
};

enum LoadImgCompType {
    LOAD_MENU_IMG_MIO0_ONCE = -1,
    LOAD_MENU_IMG_TKMK00_ONCE,
    // Force load image even if the data is Loaded before
    LOAD_MENU_IMG_FORCE = LOAD_MENU_IMG_TKMK00_ONCE, // 0
    LOAD_MENU_IMG_MIO0_FORCE,
    LOAD_MENU_IMG_TKMK00_FORCE
};

enum TEXT_MENU_ID { CONTINUE_GAME, RETRY, COURSE_CHANGE, DRIVER_CHANGE, QUIT_TEXT_MENU, REPLAY, SAVE_GHOST };

/* Function Prototypes */

f64 Ipower(f64, s32);
f64 Power(f64, f64);
f64 Llog(f64);
f64 Lexp(f64);
f64 Lldexp(f64, s32);
f64 Ffrexp(f64, s32*);
void SwapUint(s32*, s32*);
s32 CheckContPackMenu(void);
void InitGhostData(void);
void InitKawanoGame(void);
void SkipManager(void);
void SkipReset(void);
void SetCursorMessageColor(s32, s32, s32);
void SetCursorMessageColor4Option(s32, s32, s32);
void InitVCKawano(void);
void VCAutoChanger(s32, s32*, s32*);
void DispResultTime(void);
void DispGPLose(void);
void DispInfo3P(void);
void EraseInfo3P(void);
void DispVSResult(void);
void DispMessageRomGhost(void);
void StartKawanoResult(void);
void StartKawanoEnding(void);
void StaffRollKawano1(void);
s32 GetFontNumber(char*);
s32 GetFontNumberK(char*);
s32 GetFontNumberH(char*);
s32 GetFontNumberJ(char*);
s32 GetWordLength(char*);
void SetTextColor(s32);
void SetWord(s32, s32, char*);
void SetWord2(s32, s32, char*, s32, f32, f32, s32);
void SetWord2A(s32, s32, char*, s32, f32, f32);
void SetWord2AP(s32, s32, char*, s32, f32, f32);
void SetWord3(s32, s32, char*, s32, f32, f32, s32);
void SetWord3A(s32, s32, char*, s32, f32, f32);
void PrintText1CenterMode1(s32, s32, char*, s32, f32, f32);
void SetWord3AP(s32, s32, char*, s32, f32, f32);
void SetWord3ACP(s32, s32, char*, s32, f32, f32);
void PrintText2(s32, s32, char*, s32, f32, f32, s32);
void SetWord4A(s32, s32, char*, s32, f32, f32);
void TextDraw(s32, s32, char*, s32, f32, f32);
void KawanoDrawAfterSky(s32);
void KawanoDraw2(u32);
void KawanoDraw2Backup(u32);
void KawanoOnlyFade(s32);
void Randomf(void);
void Testprog(void);
void KawanoDrawFinalEx(s32);
void KawanoDrawFinal(void);
void KawanoDrawFinal4Select(void);
void InitKawanoResult(void);
void KawanoDrawResult(void);
void KawanoDraw(s32);
void LogoProc(void);
void DoNintenLogo(struct GfxPool*, s32);
void DoCheckerFlag(struct GfxPool*, s32);
void DoGameSelect(struct GfxPool*);
void TexDataChange(void);
void DoGameSelect10(void);
Gfx* FlashRectangle2Ex(Gfx*, s32, s32, s32, s32, s32);
Gfx* FlashRectangle2(Gfx*, s32, s32, s32, s32);
Gfx* FlashRectangle2f(Gfx*, s32, s32, s32, s32);
Gfx* SetAfontVertex(Gfx*, Vtx*);
Gfx* SetIAFont(Gfx*, u8*, f32, f32, u32, u32, f32, f32);
Gfx* AutoTextureRect(Gfx*, s8, s32, s32, s32, s32, s32, s32, s32, s32, u8*, u32, u32);
Gfx* AutoTextureRectZoom(Gfx*, s8, s32, s32, f32, f32, s32, s32, s32, s32, s32, s32, u8*, u32, u32);
Gfx* AutoTextureRectNoise(Gfx* displayListHead, s32 xPos, s32 YPos, u32 Width, u32 Height);
Gfx* AutoTextureRect2Cyc(Gfx* displayListHead, s8 arg1, s32 Arg2, s32 Arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9, u16* argA, u32 argB, u32 argC, s32 argD);
Gfx* AutoTextureRectC(Gfx*, s8, s32, s32, s32, s32, s32, s32, u8*, u32, u32);
Gfx* AutoTextureRect64(Gfx*, s8, s32, s32, u8*, s32);
Gfx* AutoTextureRectW(Gfx* displayListHead, s8 fmt, u32 Arg2, u32 Arg3, u32 arg4, u32 arg5, s32 arg6, s32 arg7,
                   u8* someTexture, u32 arg9, u32 argA, s32 Width);
Gfx* AutoTextureRectSimple(Gfx*, u32, u32, u32, u32, u32, u32, s32, s32);
Gfx* AutoTextureRectR(Gfx*, u32, u32, u32, u32, s32, s32, u8*, u32, s32);
Gfx* FillRect1Color(Gfx*, s32, s32, s32, s32, s32, s32, s32, s32);
Gfx* FillRect1ColorF(Gfx*, s32, s32, s32, s32, u32, u32, u32, u32);
Gfx* FillRectBlack(Gfx*, s32, s32, s32, s32);
void dma_copy_base_729a30(u64*, size_t, void*);
void dma_copy_base_7fa3c0(u64*, size_t, void*);
void InitTexBuff(void);
void TexBuffLoad(MenuTexture*);
void* GetTBPointer(const void*);
void* GetVAddress(const void*);
void TexBuffLoadPw(MenuTexture*);
void TexBuffLoadP(MenuTexture*, s32);
void TexBuffLoadVS(MenuTexture*, s32, s32);
void InitChangeTexDataVS(void);
void SetChangeTexDataVS(MenuTexture*, s32, s32);
void DoChangeTexDataVS(void);
void InitChangeTexData(void);
void SetChangeTexData(MenuTexture*, s32);
void DoChangeTexData(void);
void ChangeTexData(MenuTexture*, s32);
void func_8009A2F0(struct_8018E0E8_entry*);
void InitSeqData(void);
s32 SetSeqData(MkAnimation*);
s32 SetSeqData2(MkAnimation*, s32);
void ChangeSeqData(s32, s32, MkAnimation*);
void ChangeSeqDataVS(s32, s32, s32, MkAnimation*);
void DoSeqData(void);
void DoSeqData2(s32, s32, s32, s32);
void DoSeqData2VS(s32, s32, s32, s32, s32);
MenuTexture* SelectTex(struct_8018DEE0_entry*);
MenuTexture* SelectTexVS(struct_8018DEE0_entry*, s32);
void GrayScaleTexBuff256(s32, s32, u32, s32);
void GrayScaleTexBuff(s32);
void GrayScaleTexBuff2(s32, s32);
void GrayScaleTexBuf3(s32, u32);
void GrayScaleTexBufRGB(s32, s32, s32, s32, s32);
u16* GetTexBuffPointer(u64*);
void InitKDL(void);
void KDLMaker1(MenuTexture*);
void KDLMaker2(void);
Gfx* KDLCall(Gfx*, MenuTexture*);
Gfx* DoTexList(Gfx*, MenuTexture*, s32, s32);
Gfx* DoTexList2(Gfx*, MenuTexture*, s32, s32, s32, s32);
Gfx* DoTexList4(Gfx*, MenuTexture*, f32, f32, s32, f32, f32);
Gfx* DoTexList5(Gfx*, MenuTexture*, s32, s32, s32);
Gfx* DoTexListW(Gfx*, struct_8018DEE0_entry*, s32, s32, s32);
Gfx* DoTexListVS(Gfx*, struct_8018DEE0_entry*, s32, s32, s32, s32);
void BeforeFadeMain(void);
void FadeMain(void);
void FadeMain2(s32);
void FadeInEx(s32, s32, s32);
void FadeIn(s32, s32);
void FadeInWhite(s32, s32);
void SettingSoundOutMode4Multiplayer(void);
void EndFadeOut(s32);
void FadeOutEx(s32, s32, s32);
void FadeOut(s32, s32);
void FadeOutWhite(s32, s32);
void FadeBlack(s32);
void FadeOutR(void);
void FadeOutM(void);
void SetFadeInEx(u32, u32);
void SetFadeIn(s32);
void SetFadeInWhite(s32);
void SetFadeOutEx(u32, u32);
void SetFadeOut(s32);
void SetFadeOutWhite(s32);
void SetFadeInScreen(s32, s32);
void SetFadeOutScreen(s32, s32);
void SetFadeOutM(s32);
void SetFadeOutF(void);
void SetFadeOutFw(void);
void SetFadeOutB(void);
void SetFadeOutD(void);
void SetFadeOutTaData(void);
void SetFadeOutOption(void);
void ThunderMain(s32);
void KThunder(s32);
void ThunderWorld(void);
void SetThunderEx(s32);
void InitObjBlock(void);
void SetObjBlock(s32, s32, s32, s8);
void DispOBJBlock(ObjBlock*);
void SetCpFont(u8, s32, s32);
s32 SetCpGnoteName(char*, s32, s32, s32);
void DispOBCpGnote(ObjBlock*);
void DispOBCpNotecursor(ObjBlock*);
void DispOBCpGnoteName(ObjBlock*);
void DispOBCpFree(void);
void DispOBCpCursor(ObjBlock*, s32);
void DispOBSubGsel1(ObjBlock*, s32);
void DispOBSubGselCaution(ObjBlock*);
void DispOBSubPselCursor(ObjBlock*, s32, s32);
void DispOBSubPsel1(ObjBlock*, MenuTexture*);
void DispOBSubPsel2(ObjBlock*);
void DispOBSubMsel1(ObjBlock*, s32);
void DispOBSubMselMap(ObjBlock*);
void DispOBSubTsel1(ObjBlock*);
void DispOBSubTselCursor(ObjBlock*);
void DispOBSubDselSnap(ObjBlock*);
void DispOBSubDselMes1(ObjBlock*);
void DispOBSubDselSel1(ObjBlock*);
void DispOBSubDselSel2(ObjBlock*);
void DispOBSubDselSel3(ObjBlock*);
void DispOBSubOptionMessages(ObjBlock*);
void CutUdSub(ObjBlock*);
void DispOBResultTime(ObjBlock*);
void ResultSubTime(s32, s32, s32, s32);
void DispOBResultPoint(ObjBlock*);
void SortDPRanking(s8*);
void ResultSubPoint(ObjBlock*, s32, s32, s32, s32, s8*);
void DispOBTimeattack1(ObjBlock*);
void DispOBTimeattack2(ObjBlock*);
void TimeAttack1Sub(s32, s32, s32);
void TimeAttack1Sub2(s32, s32, s32);
void DispOBTimeattackMessage1(ObjBlock*);
void DispOBPause(ObjBlock*);
void DispOBPauseTa(ObjBlock*);
void DispOBPauseVS(ObjBlock*);
void DispOBPauseGP(ObjBlock*);
void DispOBPauseBt(ObjBlock*);
void DispOBPauseCursor(void);
void DispOBTaReplay(ObjBlock*);
void DispOBInfo3P(ObjBlock*);
void DispOBGPLose(ObjBlock*);
void DispOBVSResult(ObjBlock*);
void DispOBVSIboxCursor(ObjBlock*, POSXY*);
void DispOBVSPoint2P(ObjBlock*);
void DispOBVSPoint3P(ObjBlock*);
void DispOBVSPoint4P(ObjBlock*);
void DispOBBtPoint(s32, s32, u8*);
void DispOBVSPoint123(s32, s32, u8*);
void DispOBTaLostghost(ObjBlock*);
void DispOBEnding1Halffade(ObjBlock*);
void DispOBEnding1Message1(ObjBlock*);
void DispOBEnding1Message2(ObjBlock*);
void DispOBEnding1Message3(ObjBlock*);
void DispOBEnding1Message4(ObjBlock*);
void DispOBStaffrollMessages(ObjBlock*);
void KawanoItoa(s32, char*);
void KawanoItoaNodata(char*);
void KawanoItoaM(s32, char*);
void KawanoItoaS(s32, char*);
void KawanoItoaC(s32, char*);
void KawanoItoa2(s32, char*);
void DoObjBlock(s32);
void DoOBJBlock1(void);
void DoOBJBlock2(void);
void GselSub1(s32, ObjBlock*);
void SelOKSub(ObjBlock*);
void MselBboxSub(ObjBlock*);
void MselBboxSub2(ObjBlock*);
void MselSub1(s32, ObjBlock*);
void MselSub2(ObjBlock*);
void MselSub4(ObjBlock*);
void MselBattleSub(ObjBlock*);
void DispOBSubMselCup(ObjBlock*);
void DispOBSubMselGhost(ObjBlock*);
void TselSub1(s32, ObjBlock*);
void DOBMoving1(ObjBlock*, s32, s32);
void DOBMoving1X(ObjBlock*, s32);
void DOBMoving1Y(ObjBlock*, s32);
void DOBMoving2X(ObjBlock*, s32);
void DOBMoving2Y(ObjBlock*, s32);
void DOBMoving3X(ObjBlock*, s32);
void DOBMoving4X(ObjBlock*, s32, s32);
void DOBCpCscolor(ObjBlock*);
void DOBCpMes2(ObjBlock*);
void DOBCpGnote(ObjBlock*);
void DOBSelOK(ObjBlock*);
void DOBGselNumber(ObjBlock*);
void DOBGselMode(ObjBlock*);
void DOBGselMove(ObjBlock*);
void DOBGselGP(ObjBlock*);
void DOBGselStart(ObjBlock*);
void DOBGselCaution(ObjBlock*);
void DOBPselAnimeMario(ObjBlock*, s8);
void DOBPSelAnimeKuppa(ObjBlock*);
void DOBPSelTurnOut(ObjBlock*);
void DOBPSelTurnIn(ObjBlock*);
void DOBPselCharacter1(ObjBlock*);
void DOBPselCursor1(ObjBlock*);
void DOBPselAnimePriority(ObjBlock*);
ObjBlock* GetGselCursorPtr(void);
ObjBlock* GetPselCharacterPtr(s32);
ObjBlock* GetOBJBlockPtr(s32);
ObjBlock* GetOBJBlockPtr2(s32);
s32 GetPselCharacterPattern(s32);
void GetPselCursorPosition(ObjBlock*, s32);
s32 GetPselCursorNumber(s32);
void DOBMselCup(ObjBlock*);
void DOBMselMap(ObjBlock*);
void DOBMselMove(ObjBlock*);
void DOBMselName(ObjBlock*);
void DOBMselAnimeMap2(ObjBlock*);
void DOBMselAnimeMapman(ObjBlock*);
void DOBMselBbox(ObjBlock*);
void DOBMselCupGsc(ObjBlock*);
void DOBTselCup(ObjBlock*);
void DOBTselCursor(ObjBlock*);
void DOBTselMap(ObjBlock*);
void DOBGameCup(ObjBlock*);
void DOBGameCupMode1(ObjBlock*);
void DOBGameCircuit(ObjBlock*);
void DOBGameCircuitMode1(ObjBlock*);
void DOBGameCircuitMode2(ObjBlock*);
void DOBMselNoise(ObjBlock*);
void DOBGameTime(ObjBlock*);
void DOBGamePoint(ObjBlock*);
void DOBInfo3P(ObjBlock*);
void DOBGPLose(ObjBlock*);
void DOBVSResult(ObjBlock*);
void DOBVSFace(ObjBlock*);
void DOBGameTa1(ObjBlock*);
void DOBGameTa2(ObjBlock*);
void DOBGamePause(ObjBlock*);
void DOBGameTaReplay(ObjBlock*);
void DOBGameTaMes1(ObjBlock*);
void DOBDselSnap(ObjBlock*);
void DOBDselSel1(ObjBlock*);
void DOBDselSel2(ObjBlock*);
void DOBDselSel3(ObjBlock*);
void DOBTaFanfare(ObjBlock*);
void DOBTaLostghost(ObjBlock*);
void DOBEnding1Halffade(ObjBlock*);
void DOBEnding1Message(ObjBlock*);
void DOBEnding1Face(ObjBlock*);
void DOBStaffrollMessages(ObjBlock*);
void DOBStaffrollType0(ObjBlock*);
void DOBStaffrollType1(ObjBlock*);

// This really, really shouldn't be in this header file, but I don't know where else to put it
void rmonPrintf(const char*, ...);
void decodeTKMK(u32*, u8*, u16*, s32);

/* File specific defines */

#define MENU_ITEMS_MAX 0x20
#define D_8018DEE0_SIZE 0x10
#define D_8018E060_SIZE 0x10
#define D_8018E0E8_SIZE 0x05
#define TEXTURE_MAP_MAX 0xC8
#define D_8018E768_SIZE 0x08
#define D_8018E7E8_SIZE 0x05
#define D_8018E810_SIZE 0x05

/* This is where I'd put my static data, if I had any */

extern s32 D_800DDB24;
extern s16 NameToNum[];

extern u16* TexBuff;
extern u32* MeltBuff;
extern u8* KWork;
extern u8* AdditionalPoint;
extern void* KdlBuff;
extern s8 PlayerGPpoints[8];
extern s8 DPointsRank[];
extern s8 KSkipFlg;
extern s8 KSkipStatus;
extern ObjBlock MenuItems[MENU_ITEMS_MAX];
extern struct_8018DEE0_entry SEQDataBlock[D_8018DEE0_SIZE];
extern struct_8018E060_entry ChangeTexBuffBlock[D_8018E060_SIZE];
extern struct_8018E0E8_entry ChangeTexBuffBlockVS[D_8018E0E8_SIZE];
extern s32 TexBuffNum;
extern TextureMap SMenuTextureMap[TEXTURE_MAP_MAX];
extern s32 AllTexBlockNum;
extern Gfx* KdlBuffPtr;
extern s32 AllKdlBlockNum;
extern struct_8018E768_entry KdlBuffBlock[D_8018E768_SIZE];
extern s32 KFlashCounter;
extern s8 FadingFlag[];
extern u32 FadeCounter[];
extern u32 FadeCounter2[];
extern s32 D_8018E7E0;
extern struct UnkStruct_8018E7E8 OldScreenPosition[D_8018E7E8_SIZE];
extern struct UnkStruct_8018E7E8 OldScreenSize[D_8018E810_SIZE];
extern s8 AfontColor;
extern u8 RankinFanfare;
extern u8 EndingMessageFlg;
extern s8 ThunderFlg[];
extern s32 ThunderCounter[];
extern s32 VcColorNum[];
extern s32 D_8018E854;
extern s32 VcColorStep[];
extern s32 D_8018E85C;

extern u8 _textures_0aSegmentRomStart[];
extern u8 _textures_0bSegmentRomStart[];

extern POSXY LgselDefaultPositions[];
extern POSXY GselPopupPositions[];
extern POSXY PlayerSelectPositions[][4];
extern POSXY MselCupboxPosition[];
extern POSXY MselMap4BoxPosition[];
extern POSXY LpselCharacterOKPosition[];
extern POSXY LmselCircuitPosition[][2];
extern POSXY MselBestboxDefaultPosition[];
extern POSXY MselBestboxTargetPosition[];
extern POSXY MselCupPositions[];
extern POSXY CpCursorPosition[];
extern POSXY VSFaceStartPosition2;
extern POSXY VSFaceTargetPosition[];
extern POSXY VSResultCursorPosition[];
extern POSXY VSResultWinlosePosition[];
extern POSXY TaResultCursorPosition[];
extern POSXY TaGhostbCursorPosition[];
extern POSXY TaGhostcCursorPosition[];
extern POSXY TarSelectCursorPosition[];
extern POSXY TarGhostbCursorPosition[];
extern POSXY TarGhostcCursorPosition[];
extern POSXY TselCupPositions[];
extern POSXY Ending1MessagesStartPosition[];
extern POSXY Ending1MessagesTargetPosition[];
extern RGBA16 PselCursorColor[];
extern RGBA16 TselCursorColor[];
extern RGBA16 TitlebackColor[];
extern const s16 FontWidth[];
extern char* CupName[];
extern char* CircuitNameSemilongJ[];
// Maps course IDs (as defined in the COURSES enum) to an Index in a given cup's track Order
extern const s8 Map2CircuitChart[]; // D_800EFD50
extern const s8 Driver2Pselset[];
extern s8 Map2CupChart[];
extern char* CupTypeNames[];
extern char* DriversName[];
extern char* DriversNameJ[];
extern char* ClassName[];
extern char* ClassNameJ[];
extern char* ScreenModeNames[];
extern char* SoundModeName[];
extern char* SoundModeNameJ[]; // D_800E7710
extern char* WinLoseJ[];
extern char* BestrecJ[];
extern char* LapTimeText;
extern char* LaptimeJ[];
extern char* RecordtimeJ[];
extern char* TaResultSelectJ[];
extern char* VSResultTitleJ[];
extern char TaRomghostMessageJ[];
extern char* D_800E77A0[];
extern char* MselBattleMessagesJ[];
extern char TselTitleJ[];
extern char D_800E77D8[];
extern char* DselLengthdataJ[];
extern char* DselSelect1J[];
extern char* DselSelect2J[];
extern char* DselSelect2MessagesJ[];
extern char* DselSelect3MessagesJ[];
extern char* OptionSelect1MessagesJ[];
extern char* OptionSelect3MessagesJ[];
extern char* OptionSelect3EMessagesJ[];
extern char* OptionCopyPak1ErrMessagesJ[];
extern char* OptionCopyPak2ErrMessagesJ[];
extern char* OptionCopyCopyerrMessagesJ[];
extern char* OptionCopyPakJ[];
extern char* OptionCopyPak2SelMessageJ[];
extern char* OptionCopySureMessagesJ[];
extern char* OptionCopySure2MessagesJ[];
extern char* OptionCopyCopyMessagesJ[];
extern char* GselCautionMessagesJ[];
extern char* D_800E7980[];
extern char* TaGhostaMessagesJ[];
extern char* TaLostGhostMessageJ[];
extern char* TaGhostbMessagesJ[];
extern char* D_800E7A44;
extern char* TaGhostb3MessagesJ[];
extern char* D_800E7A54[];
extern char* TaGhostcMessagesJ[];
extern char* TaGhostcSelectMessagesJ[];
extern char* TaGhostdMessagesJ[];
extern char* TaGhostd2MessagesJ[];
extern char* EndingMessage2[];
extern char* D_800E7A98;
extern char* EndingMessage3[];
extern char* EndingMessage4[];
extern const s8 AdditionalPointDefaults[];
extern const s8 ScreenMode2Num[];
extern const s8 ThunderOnoff[];
extern const s8 StartPauseSEQ[];
extern const s8 BottomPauseSEQ[];
extern RGBA16 KThunderColor[];
extern RGBA16 FadeColorBw[];
extern MenuTexture* D_800E7AF8[];
extern MenuTexture* D_800E7D0C[];
extern MkAnimation* D_800E7D34[];
extern MenuTexture* MenuTexturesBackground[];
extern MenuTexture* SelectNamePlateTable[];
extern MenuTexture* D_800E7D74[];
extern MenuTexture* D_800E7DC4[];
extern MkAnimation* D_800E7E14[];
extern MkAnimation* D_800E7E20[];
extern MkAnimation* D_800E7E34[];
extern MenuTexture* GlyphTextureLUT[];
extern MenuTexture* D_800E7FF0[];
extern MenuTexture* D_800E80A0[];
extern MenuTexture* D_800E8114[];
extern MenuTexture* D_800E8174[];
extern MenuTexture* D_800E817C[];
extern MenuTexture* D_800E81E4[];
extern MenuTexture* D_800E822C[];
extern MenuTexture* D_800E8234[];
extern MenuTexture* D_800E8254[];
extern MenuTexture* D_800E8274[];
extern MenuTexture* D_800E8294[];
extern MenuTexture* MenuTexturesBorderPlayer[];
extern MenuTexture* MenuTexturesTrackSelection[];
extern MenuTexture* D_800E82F4[];
extern MkAnimation* D_800E8320[];
extern MkAnimation* TBSeqPselReturn[];
extern MkAnimation* TBSeqPselP0[];
extern MkAnimation* TBSeqPselP1[];
extern MkAnimation* TBSeqPselP2[];
extern MkAnimation* TBSeqPselP3[];
extern MkAnimation* TBSeqPselP4[];
extern MkAnimation* TBSeqPselP5[];
extern MkAnimation* TBSeqPselP6[];
extern s32 P1PatNum[];
extern s32 P3PatNum[];
extern s32 P4PatNum[];
extern s32 P5PatNum[];
extern Vtx* D_800E84C0[];
extern Gfx* D_800E84CC[];
extern Gfx* D_800E84EC[];
extern Gfx* D_800E850C[];
extern s8 FirstTimeOnlyFlg;
extern f32 LogoDelaySize;
extern f32 LogoYangSpeed;
extern POSXY D_800E8538[];
extern POSXY D_800E8540[];
extern POSXY D_800E85C0[];
extern POSXY D_800E8600[];

extern s32 ControllerPak1NumPagesFree;
extern s32 ControllerPak1FileNote;
extern s32 ControllerPak2FileNote;

extern f32 D_8018ED98;
extern f32 D_8018ED9C;
extern f32 D_8018EDA0;

extern f32 D_8018EDA4;
extern f32 D_8018EDA8;
extern f32 D_8018EDAC;

#endif

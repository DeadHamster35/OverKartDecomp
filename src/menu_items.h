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
it last longer See `AutoTextureRect_noise` for the actual drawing of the static
*/

typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ s32 pattern;
    /* 0x08 */ s32 pattern2;
    /* 0x0C */ s32 disp_x;
    /* 0x10 */ s32 disp_y;
    /* 0x14 */ s8 pri; // higher is drawn on top; later gMenuItems win ties
    /* 0x15 */ bool8 disp_on;
    // Library hootanim/zuul_flag occupy this padding in OverKart; keep matching s16
    /* 0x16 */ s16 unused;
    /* 0x18 */ s32 sd_number;
    /* 0x1C */ s32 work1;
    /* 0x20 */ s32 work2;
    /* 0x24 */ f32 workf1;
} ObjBlock; // size = 0x28
typedef ObjBlock MenuItem;

typedef struct {
    /* 0x00 */ MkAnimation* textureSequence;
    /* 0x04 */ s32 sequenceIndex;    // Index in textureSequence that the animation is currently on
    /* 0x08 */ s32 frameCountDown;   // Frames left for the given animation part
    /* 0x0C */ u32 visible;          // visbile if 0x80000000, otherwise invisbile AND paused
    /* 0x10 */ s32 menuTextureIndex; // Don't know what sMenuTextureMap tracks
    /* 0x14 */ s32 unk14;            // Flip flops between 0 and 1, use unknown
} struct_8018DEE0_entry;             // size = 0x18

typedef struct {
    /* 0x0 */ MenuTexture* texture;
    /* 0x4 */ s32 texNum;
} struct_8018E060_entry; // size = 0x8

typedef struct {
    /* 0x0 */ MenuTexture* mk64Texture;
    /* 0x4 */ s16 unk4;
    /* 0x6 */ s16 unk6;
} struct_8018E0E8_entry; // size = 0x8

typedef struct {
    /* 0x00 */ u64* textureData; // This should be interpreted as a segmented address
                                 /**
                                  * Its hard to tell what exactly what this is meant to be,
                                  * but it appears to be used as some sort of offset/index from the address stored in tex_buff.
                                  * This value is (roughly) the sum of (width * height) of the
                                  * textures in all the previous entries in sMenuTextureMap
                                  */
    /* 0x04 */ s32 offset;
} TextureMap; // size = 0x08

typedef struct {
    /* 0x00 */ MenuTexture* textures;
    /* 0x04 */ Gfx* displayList;
} struct_8018E768_entry; // size = 0x08

struct UnkStruct_8018E7E8 {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 unk1;
    /* 0x6 */ s16 unk2;
}; // size = 0x8

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 dummy1;
    /* 0x06 */ s16 dummy2;
} POSXY; // size = 0x08
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
    // Force load image even if the data is loaded before
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
void init_kawano_game(void);
void SkipManager(void);
void SkipReset(void);
void SetCursorMessageColor(s32, s32, s32);
void SetCursorMessageColor4Option(s32, s32, s32);
void InitVC_kawano(void);
void VCAutoChanger(s32, s32*, s32*);
void DispResultTime(void);
void DispGPLose(void);
void DispInfo_3P(void);
void EraseInfo_3P(void);
void DispVSResult(void);
void DispMessageRomGhost(void);
void StartKawanoResult(void);
void StartKawanoEnding(void);
void StaffRoll_kawano1(void);
s32 GetFontNumber(char*);
s32 GetFontNumber_k(char*);
s32 GetFontNumber_h(char*);
s32 GetFontNumber_j(char*);
s32 GetWordLength(char*);
void set_text_color(s32);
void SetWord(s32, s32, char*);
void SetWord2(s32, s32, char*, s32, f32, f32, s32);
void SetWord2A(s32, s32, char*, s32, f32, f32);
void SetWord2AP(s32, s32, char*, s32, f32, f32);
void SetWord3(s32, s32, char*, s32, f32, f32, s32);
void SetWord3A(s32, s32, char*, s32, f32, f32);
void print_text1_center_mode_1(s32, s32, char*, s32, f32, f32);
void SetWord3AP(s32, s32, char*, s32, f32, f32);
void SetWord3ACP(s32, s32, char*, s32, f32, f32);
void print_text2(s32, s32, char*, s32, f32, f32, s32);
void SetWord4A(s32, s32, char*, s32, f32, f32);
void text_draw(s32, s32, char*, s32, f32, f32);
void kawanoDrawAfterSky(s32);
void kawanoDraw2(u32);
void kawanoDraw2_backup(u32);
void kawanoOnlyFade(s32);
void Randomf(void);
void testprog(void);
void kawanoDrawFinal_(s32);
void KawanoDrawFinal(void);
void kawanoDrawFinal4select(void);
void init_kawanoResult(void);
void kawanoDrawResult(void);
void kawanoDraw(s32);
void logo_proc(void);
void doNintenLogo(struct GfxPool*, s32);
void doCheckerFlag(struct GfxPool*, s32);
void doGameSelect(struct GfxPool*);
void TexDataChange(void);
void doGameSelect10(void);
Gfx* FlashRectangle2_(Gfx*, s32, s32, s32, s32, s32);
Gfx* FlashRectangle2(Gfx*, s32, s32, s32, s32);
Gfx* FlashRectangle2f(Gfx*, s32, s32, s32, s32);
Gfx* SetAfontVertex(Gfx*, Vtx*);
Gfx* Set_IAFont(Gfx*, u8*, f32, f32, u32, u32, f32, f32);
Gfx* AutoTextureRect(Gfx*, s8, s32, s32, s32, s32, s32, s32, s32, s32, u8*, u32, u32);
Gfx* AutoTextureRect_zoom(Gfx*, s8, s32, s32, f32, f32, s32, s32, s32, s32, s32, s32, u8*, u32, u32);
Gfx* AutoTextureRect_noise(Gfx* displayListHead, s32 xPos, s32 yPos, u32 width, u32 height);
Gfx* AutoTextureRect_2cyc(Gfx* displayListHead, s8 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9, u16* argA, u32 argB, u32 argC, s32 argD);
Gfx* AutoTextureRect_c(Gfx*, s8, s32, s32, s32, s32, s32, s32, u8*, u32, u32);
Gfx* AutoTextureRect64(Gfx*, s8, s32, s32, u8*, s32);
Gfx* AutoTextureRect_w(Gfx* displayListHead, s8 fmt, u32 arg2, u32 arg3, u32 arg4, u32 arg5, s32 arg6, s32 arg7,
                   u8* someTexture, u32 arg9, u32 argA, s32 width);
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
void TexBuffLoad_pw(MenuTexture*);
void TexBuffLoadP(MenuTexture*, s32);
void TexBuffLoad_vs(MenuTexture*, s32, s32);
void InitChangeTexData_vs(void);
void SetChangeTexData_vs(MenuTexture*, s32, s32);
void doChangeTexData_vs(void);
void InitChangeTexData(void);
void SetChangeTexData(MenuTexture*, s32);
void doChangeTexData(void);
void ChangeTexData(MenuTexture*, s32);
void func_8009A2F0(struct_8018E0E8_entry*);
void InitSeqData(void);
s32 SetSeqData(MkAnimation*);
s32 SetSeqData2(MkAnimation*, s32);
void ChangeSeqData(s32, s32, MkAnimation*);
void ChangeSeqData_vs(s32, s32, s32, MkAnimation*);
void doSeqData(void);
void doSeqData2(s32, s32, s32, s32);
void doSeqData2_vs(s32, s32, s32, s32, s32);
MenuTexture* SelectTex(struct_8018DEE0_entry*);
MenuTexture* SelectTex_vs(struct_8018DEE0_entry*, s32);
void GrayScale_tex_buff256(s32, s32, u32, s32);
void GrayScale_tex_buff(s32);
void GrayScale_tex_buff2(s32, s32);
void GrayScaleTexBuf3(s32, u32);
void GrayScaleTexBufRGB(s32, s32, s32, s32, s32);
u16* GetTexBuffPointer(u64*);
void InitKDL(void);
void KDL_maker1(MenuTexture*);
void KDL_maker2(void);
Gfx* KDL_call(Gfx*, MenuTexture*);
Gfx* doTexList(Gfx*, MenuTexture*, s32, s32);
Gfx* doTexList2(Gfx*, MenuTexture*, s32, s32, s32, s32);
Gfx* doTexList4(Gfx*, MenuTexture*, f32, f32, s32, f32, f32);
Gfx* doTexList5(Gfx*, MenuTexture*, s32, s32, s32);
Gfx* doTexList_w(Gfx*, struct_8018DEE0_entry*, s32, s32, s32);
Gfx* doTexList_vs(Gfx*, struct_8018DEE0_entry*, s32, s32, s32, s32);
void BeforeFadeMain(void);
void FadeMain(void);
void FadeMain2(s32);
void FadeIn_(s32, s32, s32);
void FadeIn(s32, s32);
void FadeInWhite(s32, s32);
void Setting_SoundOutMode4multiplayer(void);
void EndFadeOut(s32);
void FadeOut_(s32, s32, s32);
void FadeOut(s32, s32);
void FadeOutWhite(s32, s32);
void FadeBlack(s32);
void FadeOut_r(void);
void FadeOut_m(void);
void SetFadeIn_(u32, u32);
void SetFadeIn(s32);
void SetFadeInWhite(s32);
void SetFadeOut_(u32, u32);
void SetFadeOut(s32);
void SetFadeOutWhite(s32);
void SetFadeInScreen(s32, s32);
void SetFadeOutScreen(s32, s32);
void SetFadeOut_m(s32);
void SetFadeOut_f(void);
void SetFadeOut_fw(void);
void SetFadeOutB(void);
void SetFadeOut_d(void);
void SetFadeOutTaData(void);
void SetFadeOut_option(void);
void ThunderMain(s32);
void k_thunder(s32);
void ThunderWorld(void);
void SetThunder_(s32);
void InitObjBlock(void);
void SetObjBlock(s32, s32, s32, s8);
void DispOBJBlock(ObjBlock*);
void Set_cp_font(u8, s32, s32);
s32 Set_cp_gnote_name(char*, s32, s32, s32);
void DispOB_cp_gnote(ObjBlock*);
void DispOB_cp_notecursor(ObjBlock*);
void DispOB_cp_gnote_name(ObjBlock*);
void DispOB_cp_free(void);
void DispOB_cp_cursor(ObjBlock*, s32);
void DispOB_Sub_gsel1(ObjBlock*, s32);
void DispOB_Sub_gsel_caution(ObjBlock*);
void DispOB_Sub_psel_cursor(ObjBlock*, s32, s32);
void DispOB_Sub_psel1(ObjBlock*, MenuTexture*);
void DispOB_Sub_psel2(ObjBlock*);
void DispOB_Sub_msel1(ObjBlock*, s32);
void DispOB_Sub_msel_map(ObjBlock*);
void DispOB_Sub_tsel1(ObjBlock*);
void DispOB_Sub_tsel_cursor(ObjBlock*);
void DispOB_Sub_dsel_snap(ObjBlock*);
void DispOB_Sub_dsel_mes1(ObjBlock*);
void DispOB_Sub_dsel_sel1(ObjBlock*);
void DispOB_Sub_dsel_sel2(ObjBlock*);
void DispOB_Sub_dsel_sel3(ObjBlock*);
void DispOB_Sub_option_messages(ObjBlock*);
void Cut_ud_sub(ObjBlock*);
void DispOB_result_time(ObjBlock*);
void ResultSub_time(s32, s32, s32, s32);
void DispOB_result_point(ObjBlock*);
void SortDPRanking(s8*);
void ResultSub_point(ObjBlock*, s32, s32, s32, s32, s8*);
void DispOB_timeattack1(ObjBlock*);
void DispOB_timeattack2(ObjBlock*);
void TimeAttack1Sub(s32, s32, s32);
void TimeAttack1Sub2(s32, s32, s32);
void DispOB_timeattack_message1(ObjBlock*);
void DispOB_pause(ObjBlock*);
void DispOB_pause_ta(ObjBlock*);
void DispOB_pause_vs(ObjBlock*);
void DispOB_pause_gp(ObjBlock*);
void DispOB_pause_bt(ObjBlock*);
void DispOB_pause_cursor(void);
void DispOB_ta_replay(ObjBlock*);
void DispOB_info_3p(ObjBlock*);
void DispOB_gp_lose(ObjBlock*);
void DispOB_vs_result(ObjBlock*);
void DispOB_vs_ibox_cursor(ObjBlock*, POSXY*);
void DispOB_vs_point2P(ObjBlock*);
void DispOB_vs_point3P(ObjBlock*);
void DispOB_vs_point4P(ObjBlock*);
void DispOB_bt_point(s32, s32, u8*);
void DispOB_vs_point123(s32, s32, u8*);
void DispOB_ta_lostghost(ObjBlock*);
void DispOB_ending1_halffade(ObjBlock*);
void DispOB_ending1_message1(ObjBlock*);
void DispOB_ending1_message2(ObjBlock*);
void DispOB_ending1_message3(ObjBlock*);
void DispOB_ending1_message4(ObjBlock*);
void DispOB_staffroll_messages(ObjBlock*);
void kawano_itoa(s32, char*);
void kawano_itoa_nodata(char*);
void kawano_itoa_m(s32, char*);
void kawano_itoa_s(s32, char*);
void kawano_itoa_c(s32, char*);
void kawano_itoa2(s32, char*);
void DoObjBlock(s32);
void doOBJBlock1(void);
void doOBJBlock2(void);
void gsel_sub1(s32, ObjBlock*);
void sel_ok_sub(ObjBlock*);
void msel_bbox_sub(ObjBlock*);
void msel_bbox_sub2(ObjBlock*);
void msel_sub1(s32, ObjBlock*);
void msel_sub2(ObjBlock*);
void msel_sub4(ObjBlock*);
void msel_battle_sub(ObjBlock*);
void DispOB_Sub_msel_cup(ObjBlock*);
void DispOB_Sub_msel_ghost(ObjBlock*);
void tsel_sub1(s32, ObjBlock*);
void DOB_moving1(ObjBlock*, s32, s32);
void DOB_moving1_x(ObjBlock*, s32);
void DOB_moving1_y(ObjBlock*, s32);
void DOB_moving2_x(ObjBlock*, s32);
void DOB_moving2_y(ObjBlock*, s32);
void DOB_moving3_x(ObjBlock*, s32);
void DOB_moving4_x(ObjBlock*, s32, s32);
void DOB_cp_cscolor(ObjBlock*);
void DOB_cp_mes2(ObjBlock*);
void DOB_cp_gnote(ObjBlock*);
void DOB_sel_ok(ObjBlock*);
void DOB_gsel_number(ObjBlock*);
void DOB_gsel_mode(ObjBlock*);
void DOB_gsel_move(ObjBlock*);
void DOB_gsel_gp(ObjBlock*);
void DOB_gsel_start(ObjBlock*);
void DOB_gsel_caution(ObjBlock*);
void DOB_psel_anime_mario(ObjBlock*, s8);
void DOBPSelAnimeKuppa(ObjBlock*);
void DOBPSelTurnOut(ObjBlock*);
void DOBPSelTurnIn(ObjBlock*);
void DOB_psel_character1(ObjBlock*);
void DOB_psel_cursor1(ObjBlock*);
void DOB_psel_anime_priority(ObjBlock*);
ObjBlock* Get_gsel_cursor_ptr(void);
ObjBlock* Get_psel_character_ptr(s32);
ObjBlock* Get_OBJBlock_ptr(s32);
ObjBlock* Get_OBJBlock_ptr2(s32);
s32 Get_psel_character_pattern(s32);
void Get_psel_cursor_position(ObjBlock*, s32);
s32 Get_psel_cursor_number(s32);
void DOB_msel_cup(ObjBlock*);
void DOB_msel_map(ObjBlock*);
void DOB_msel_move(ObjBlock*);
void DOB_msel_name(ObjBlock*);
void DOB_msel_anime_map2(ObjBlock*);
void DOB_msel_anime_mapman(ObjBlock*);
void DOB_msel_bbox(ObjBlock*);
void DOB_msel_cup_gsc(ObjBlock*);
void DOB_tsel_cup(ObjBlock*);
void DOB_tsel_cursor(ObjBlock*);
void DOB_tsel_map(ObjBlock*);
void DOB_game_cup(ObjBlock*);
void DOB_game_cup_mode1(ObjBlock*);
void DOB_game_circuit(ObjBlock*);
void DOB_game_circuit_mode1(ObjBlock*);
void DOB_game_circuit_mode2(ObjBlock*);
void DOB_msel_noise(ObjBlock*);
void DOB_game_time(ObjBlock*);
void DOB_game_point(ObjBlock*);
void DOB_info_3p(ObjBlock*);
void DOB_gp_lose(ObjBlock*);
void DOB_vs_result(ObjBlock*);
void DOB_vs_face(ObjBlock*);
void DOB_game_ta1(ObjBlock*);
void DOB_game_ta2(ObjBlock*);
void DOB_game_pause(ObjBlock*);
void DOB_game_ta_replay(ObjBlock*);
void DOB_game_ta_mes1(ObjBlock*);
void DOB_dsel_snap(ObjBlock*);
void DOB_dsel_sel1(ObjBlock*);
void DOB_dsel_sel2(ObjBlock*);
void DOB_dsel_sel3(ObjBlock*);
void DOB_ta_fanfare(ObjBlock*);
void DOB_ta_lostghost(ObjBlock*);
void DOB_ending1_halffade(ObjBlock*);
void DOB_ending1_message(ObjBlock*);
void DOB_ending1_face(ObjBlock*);
void DOB_staffroll_messages(ObjBlock*);
void DOB_staffroll_type0(ObjBlock*);
void DOB_staffroll_type1(ObjBlock*);

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
extern s16 name_to_num[];

extern u16* tex_buff;
extern u32* melt_buff;
extern u8* k_work;
extern u8* additional_point;
extern void* kdl_buff;
extern s8 g_playerGPpoints[8];
extern s8 d_points_rank[];
extern s8 k_skip_flg;
extern s8 k_skip_status;
extern ObjBlock gMenuItems[MENU_ITEMS_MAX];
extern struct_8018DEE0_entry seq_data_block[D_8018DEE0_SIZE];
extern struct_8018E060_entry change_tex_buff_block[D_8018E060_SIZE];
extern struct_8018E0E8_entry change_tex_buff_block_vs[D_8018E0E8_SIZE];
extern s32 tex_buff_num;
extern TextureMap sMenuTextureMap[TEXTURE_MAP_MAX];
extern s32 all_tex_block_num;
extern Gfx* kdl_buff_ptr;
extern s32 all_kdl_block_num;
extern struct_8018E768_entry kdl_buff_block[D_8018E768_SIZE];
extern s32 k_flash_counter;
extern s8 g_FadingFlag[];
extern u32 g_FadeCounter[];
extern u32 g_FadeCounter2[];
extern s32 D_8018E7E0;
extern struct UnkStruct_8018E7E8 old_screen_position[D_8018E7E8_SIZE];
extern struct UnkStruct_8018E7E8 old_screen_size[D_8018E810_SIZE];
extern s8 afont_color;
extern u8 rankin_fanfare;
extern u8 ending_message_flg;
extern s8 thunder_flg[];
extern s32 thunder_counter[];
extern s32 vc_color_num[];
extern s32 D_8018E854;
extern s32 vc_color_step[];
extern s32 D_8018E85C;

extern u8 _textures_0aSegmentRomStart[];
extern u8 _textures_0bSegmentRomStart[];

extern POSXY lgsel_default_positions[];
extern POSXY gsel_popup_positions[];
extern POSXY PlayerSelectPositions[][4];
extern POSXY msel_cupbox_position[];
extern POSXY msel_map4box_position[];
extern POSXY lpsel_character_ok_position[];
extern POSXY lmsel_circuit_position[][2];
extern POSXY msel_bestbox_default_position[];
extern POSXY msel_bestbox_target_position[];
extern POSXY msel_cup_positions[];
extern POSXY cp_cursor_position[];
extern POSXY vs_face_start_position2;
extern POSXY vs_face_target_position[];
extern POSXY vs_result_cursor_position[];
extern POSXY vs_result_winlose_position[];
extern POSXY ta_result_cursor_position[];
extern POSXY ta_ghostb_cursor_position[];
extern POSXY ta_ghostc_cursor_position[];
extern POSXY tar_select_cursor_position[];
extern POSXY tar_ghostb_cursor_position[];
extern POSXY tar_ghostc_cursor_position[];
extern POSXY tsel_cup_positions[];
extern POSXY ending1_messages_start_position[];
extern POSXY ending1_messages_target_position[];
extern RGBA16 psel_cursor_color[];
extern RGBA16 tsel_cursor_color[];
extern RGBA16 titleback_color[];
extern const s16 font_width[];
extern char* cup_name[];
extern char* circuit_name_semilong_j[];
// Maps course IDs (as defined in the COURSES enum) to an index in a given cup's track order
extern const s8 map2circuit_chart[]; // D_800EFD50
extern const s8 driver2pselset[];
extern s8 map2cup_chart[];
extern char* cup_type_names[];
extern char* drivers_name[];
extern char* drivers_name_j[];
extern char* class_name[];
extern char* class_name_j[];
extern char* screen_mode_names[];
extern char* sound_mode_name[];
extern char* sound_mode_name_j[]; // D_800E7710
extern char* win_lose_j[];
extern char* bestrec_j[];
extern char* gLapTimeText;
extern char* laptime_j[];
extern char* recordtime_j[];
extern char* ta_result_select_j[];
extern char* vs_result_title_j[];
extern char ta_romghost_message_j[];
extern char* D_800E77A0[];
extern char* msel_battle_messages_j[];
extern char tsel_title_j[];
extern char D_800E77D8[];
extern char* dsel_lengthdata_j[];
extern char* dsel_select1_j[];
extern char* dsel_select2_j[];
extern char* dsel_select2_messages_j[];
extern char* dsel_select3_messages_j[];
extern char* option_select1_messages_j[];
extern char* option_select3_messages_j[];
extern char* option_select3e_messages_j[];
extern char* option_copy_pak1err_messages_j[];
extern char* option_copy_pak2err_messages_j[];
extern char* option_copy_copyerr_messages_j[];
extern char* option_copy_pak_j[];
extern char* option_copy_pak2sel_message_j[];
extern char* option_copy_sure_messages_j[];
extern char* option_copy_sure2_messages_j[];
extern char* option_copy_copy_messages_j[];
extern char* gsel_caution_messages_j[];
extern char* D_800E7980[];
extern char* ta_ghosta_messages_j[];
extern char* ta_lost_ghost_message_j[];
extern char* ta_ghostb_messages_j[];
extern char* D_800E7A44;
extern char* ta_ghostb3_messages_j[];
extern char* D_800E7A54[];
extern char* ta_ghostc_messages_j[];
extern char* ta_ghostc_select_messages_j[];
extern char* ta_ghostd_messages_j[];
extern char* ta_ghostd2_messages_j[];
extern char* ending_message2[];
extern char* D_800E7A98;
extern char* ending_message3[];
extern char* ending_message4[];
extern const s8 additional_point_defaults[];
extern const s8 screen_mode2num[];
extern const s8 thunder_onoff[];
extern const s8 start_pause_seq[];
extern const s8 bottom_pause_seq[];
extern RGBA16 k_thunder_color[];
extern RGBA16 fade_color_bw[];
extern MenuTexture* D_800E7AF8[];
extern MenuTexture* D_800E7D0C[];
extern MkAnimation* D_800E7D34[];
extern MenuTexture* gMenuTexturesBackground[];
extern MenuTexture* SelectNamePlateTable[];
extern MenuTexture* D_800E7D74[];
extern MenuTexture* D_800E7DC4[];
extern MkAnimation* D_800E7E14[];
extern MkAnimation* D_800E7E20[];
extern MkAnimation* D_800E7E34[];
extern MenuTexture* gGlyphTextureLUT[];
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
extern MenuTexture* gMenuTexturesBorderPlayer[];
extern MenuTexture* gMenuTexturesTrackSelection[];
extern MenuTexture* D_800E82F4[];
extern MkAnimation* D_800E8320[];
extern MkAnimation* TBSeq_psel_return[];
extern MkAnimation* TBSeq_psel_p0[];
extern MkAnimation* TBSeq_psel_p1[];
extern MkAnimation* TBSeq_psel_p2[];
extern MkAnimation* TBSeq_psel_p3[];
extern MkAnimation* TBSeq_psel_p4[];
extern MkAnimation* TBSeq_psel_p5[];
extern MkAnimation* TBSeq_psel_p6[];
extern s32 p1_pat_num[];
extern s32 p3_pat_num[];
extern s32 p4_pat_num[];
extern s32 p5_pat_num[];
extern Vtx* D_800E84C0[];
extern Gfx* D_800E84CC[];
extern Gfx* D_800E84EC[];
extern Gfx* D_800E850C[];
extern s8 first_time_only_flg;
extern f32 logo_delay_size;
extern f32 logo_yang_speed;
extern POSXY D_800E8538[];
extern POSXY D_800E8540[];
extern POSXY D_800E85C0[];
extern POSXY D_800E8600[];

extern s32 gControllerPak1NumPagesFree;
extern s32 gControllerPak1FileNote;
extern s32 gControllerPak2FileNote;

extern f32 D_8018ED98;
extern f32 D_8018ED9C;
extern f32 D_8018EDA0;

extern f32 D_8018EDA4;
extern f32 D_8018EDA8;
extern f32 D_8018EDAC;

#endif

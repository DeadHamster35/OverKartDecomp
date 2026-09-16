#ifndef CODE_80057C60_H
#define CODE_80057C60_H

#include <common_structs.h>
#include "objects.h"
#include "render_player.h"
#include "camera.h"

// code_80057C60

#define RENDER_SCREEN_MODE_1P_PLAYER_ONE PLAYER_ONE + SCREEN_MODE_1P
#define RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE PLAYER_ONE + SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL
#define RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO PLAYER_TWO + SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL
#define RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE PLAYER_ONE + SCREEN_MODE_2P_SPLITSCREEN_VERTICAL + 1
#define RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO PLAYER_TWO + SCREEN_MODE_2P_SPLITSCREEN_VERTICAL + 1
#define RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE PLAYER_ONE + SCREEN_MODE_3P_4P_SPLITSCREEN + 5
#define RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO PLAYER_TWO + SCREEN_MODE_3P_4P_SPLITSCREEN + 5
#define RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE PLAYER_THREE + SCREEN_MODE_3P_4P_SPLITSCREEN + 5
#define RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR PLAYER_FOUR + SCREEN_MODE_3P_4P_SPLITSCREEN + 5

#define RGB32(r, g, b) ((r << 16) | (g << 8) | (b))

typedef struct {
    char unk_00[0x4];
    Vec3f unk_04;
    char unk_10[0x4];
    s32 objectIndex;
    char unk_18[0x8];
} struct_D_8018CE10; // size = 0x20

/** @cond */

void flashcolor_table(s8, s16*, s16*, s16*);
void KWSetViewportFull(void);
void KWReturnViewport(void);
void kwdraw_safetyframe(void);
void kwdisplay_kumo(u32);

void kwdisplay3D(u32);
void kwdisplay3D_1p(void);
void kwdisplay3D_2p(void);
void kwdisplay3D_3p(void);
void kwdisplay3D_4p(void);
void kwdisplay3D_after(u32);
void kwdisplay3D_after_1p(void);
void kwdisplay3D_after_2p(void);
void kwdisplay3D_after_3p(void);
void kwdisplay3D_after_4p(void);
void KWDisplayEvent(s32);
void render_snowing_effect(s32);
void kwdisplay2D_init(void);
void KWDisplay2D(u32);
void KWDisplay2DAfter(u32);
void kwdisplay2D_1p(void);
void kwdisplay2D_1p_after(void);
void KWVideoFramesYori(void);

void kwdisplay1p_wipe(void);
void kwdisplay_orders(void);
void kwdisplay_ranks(void);
void kwdisplay_toppri(void);
void kwdisplay2D_2p_left(void);
void KWDisplay2D2PLeftAfter(void);
void kwdisplay2D_2p_right(void);
void kwdisplay2D_2p_right_after(void);
void kwdisplay2D_2p_sub(s32);
void kwdisplay2D_2p_up(void);
void kwdisplay2D_2p_up_after(void);
void kwdisplay2D_2p_down(void);
void kwdisplay2D_2p_down_after(void);
void KWDisplayAfter4PSub(s32);
void kwdisplay2D_4p_upleft(void);
void kwdisplay2D_4p_upleft_after(void);
void kwdisplay2D_4p_upright(void);
void kwdisplay2D_4p_upright_after(void);
void kwdisplay2D_4p_downleft(void);
void kwdisplay2D_4p_downleft_after(void);
void kwdisplay2D_4p_downright(void);
void kwdisplay2D_4p_downright_after(void);
void kwplayerparams(s32);
void kwcheck_randomcnt(s32);
void kw_resultscreen_start(void);
void kwcheck_ghostkart_item(void);
void kwsyori_sub(s32);
void kwpausesyori(void);
void kwgameframesyori_sub(void);

void kwgameframesyori(void);
void kwcheck_kaopanel(s32);
void kwset_kaopanel(void);
void kwcheck_command(void);
void KWGameEventCommon_VF(void);
void KWGameEventCommon(void);
void kwcheck_opening_start(void);
void kwchart_opening_start(void);
void kwchart_opening_next(void);
void kwchart_opening_jump(s32);
void kwchart_opening_finish(void);
void kwchart_opening_wait(s32);
void kwchart_opening_init(void);
void kwchart_opening_panel_start(void);
void kwchart_opening_anime(void);

void kwchart_goal_anime(void);

void kwchart_opening(void);

void convkartspeedtoangle(f32);
void kwdisplayon(s32*);
void kwdisplayoff(s32*);
void displayfinish(s32);
void kwget_color(s8, s16*, s16*, s16*);
void check_gftimer(void);
void kwcheck_rank(void);
void kwlapcntsub(s32, s32);

void kwlapcnt(s32);
void kwgoal_rank_scroll(void);
void kwcheck_nearbomb(s32);

void imageinit(void);
void effectinit(Player*);
void MakePos(Player*, Particle*, f32, f32, f32, s8, s8);
s32 MakeStartup(Particle*, s8, f32);
s32 MakeRDP(Particle*, s32, s16);
s32 set_particle_colour_randomly_varried(Particle*, s32, s16);
void initdrift_smoke_set(Player*, s16, s32, s8, s8);
void initdrift_smoke(Player*, s16, s32, s8, s8);
void set_init_smoke(void);
void set_up_smoke(Particle*, s16, s16, s16);
void InitRndSmoke(Player*, s16, s32, s8, s8);
void initrnd_spl(Player*, s16, s32, s8, s8);
void InitRapidSmoke(Player*, s16, s32, s8, s8);

void InitSpinSmoke(Player*, s16, s32, s8, s8);

void initengine_smoke(Player*, s16, s32, s8, s8);
void init_ring_set(Player*, s16, s32, s8, s8);
void init_ring(Player*, s16, s32, s8, s8);
void init_bubble(Player*, s16, s32, s8, s8);
void init_dobon(Player*, s16, s32, s8, s8);

void init_thunder(Player*, s16, s32, s8, s8);
void init_teresa_set(Player*, s16, s32, s8, s8);
void init_teresa(Player*, s16, s32, s8, s8);
void initstar_dust(Player*, s32, s32, s8, s8);
void initresult_star(Player*, s32, s32, s8, s8);
void initpowder_dust(Player*, s16, s32, s8, s8);
void initburn_dust_set(Player*, s16, s32, s32, s32);
void initburn_dust(Player*, s16, s32, s8, s8);
void initwater_dust(Player*, s16, s32, s8, s8);
void inittail_dust(Player*, s16, s32, s8, s8);
void init_slipstream(Player*, s16, s32, s8, s8);

void init_turbostream(Player*, s16, s32, s8, s8);
void make_land(Player*, Particle*, s32);
void init_land(Player*, s32, s32, s8, s8);
void initcrash_letter(Player*, s8, s8, s8);
void inithit_letter(Player*, s8, s8, s8);
void initboing_letter(Player*, s8, s8, s8);
void initpoomp_letter(Player*, s8, s8, s8);
void initspin_letter(Player*, s8, s8, s8);
void initonpu_letter(Player*, s8, s8, s8);
void make_smokepos(f32*, f32*, f32*, f32, f32, f32, u16, u16);
void engine_smoke(Player*, s16, s32, s32);
void random_bubble(Player*, s16, s8, s8);

void dobon_splash(Player*, s16, s8, s8);
void thunder_effect(Player*, s16, s8, s8);
void teresa_effect(Player*, s16, s8, s8);
void drift_smoke(Player*, s16, s8, s8);
void random_smoke(Player*, s16, s8, s8);
void rapid_smoke(Player*, s16, s8, s8);
void random_grass(Player*, s16, s8, s8);
void random_spl(Player*, s16, s8, s8);
void random_turbo(Player*, s16, s32, s32);

void random_ring(Player*, s16, s8, s8);
void spin_smoke(Player*, s16, s8, s8);
void star_dust(Player*, s16, s8, s8);
void result_star(Player*, s16, s8, s8);
void land_smoke(Player*, s16, s8, s8);
void burn_dust(Player*, s16, s8, s8);
void water_dust(Player*, s16, s8, s8);
void tail_dust(Player*, s16, s8, s8);
void powder_dust(Player*, s16, s8, s8);
void slip_stream(Player*, s16, s8, s8);
void crash_letter(Player*, s8, s8, s8);
void hit_letter(Player*, s8, s8, s8);
void boing_letter(Player*, s8, s8, s8);

void poomp_letter(Player*, s8, s8, s8);
void spin_letter(Player*, s8, s8, s8);
void onpu_letter(Player*, s8, s8, s8);
void common_mtx_calc(Vec3f, Vec3s, f32);
void set_gass_calc(Player*, s8, s16, s8);
void set_teresa_calc(Player*, s8, s16, s8);
void set_drift_calc(Player*, s8, s16, s8);
void set_smoke_calc(Player*, s8, s16, s8);

void set_spl_calc(Player*, s8, s16, s8);
void set_ring_calc(Player*, s8, s16, s8);
void set_thunder_calc(Player*, s8, s16, s8);

void set_slipstream_calc(Player*, s8, s16, s8);
void set_turbostream_calc(Player*, s8, s16, s8);
void set_spin_calc(Player*, s8, f32, s8, s8);
void set_fukidashi_calc(Player*, s8, u8*, s8, f32, s32);

void set_word_calc(Player*, s8, u8*, s8, f32, s32);
void set_crash_calc(Player*, s8, f32, s8, s8);
void set_hit_calc(Player*, s8, f32, s8, s8);
void set_boing_calc(Player*, s8, f32, s8, s8);
void set_poomp_calc(Player*, s8, f32, s8, s8);

void set_star_calc(Player*, s8, s16, s8);
void land_calc(Player*, s8, s16, s8);
void set_startail_calc(Player*, s8, s16, s8, f32);
void powder_calc(Player*, s8, s16, s8);
void burn_calc(Player*, s8, s16, s8);
void water_calc(Player*, s8, s16, s8);

void set_bubble_calc(Player*, s8, s16, s8);
void set_dobon_calc(Player*, s8, s16, s8);
void BalloonDisp(Player*, f32, f32, s8, s8, s16);
void balloon_strat(Player*, f32, f32, s8, s8);
void make_balloon_calc(Player*, s8, s16, s8);

void init_all_player_balloons(Player*, s8);
void allkill_balloon(Player*, s8);
void cut_balloon(Player*, s8);
void kill_balloon(s32, s8, s8);
void balloonset(Player*, s8);
void set_balloon_calc(Player*, s8, s8);
void draw_balloon(Vec3f, f32, s16, s16);

void draw_smoke(Vec3f, f32, s32, s16);
void draw_star(Vec3f, f32, s32, s16);
void draw_pachipachi(Vec3f, f32, s32, s16, s16);
void smokeset(Player*, s16, s8, s8);
void sparkset(Player*, s16, s8, s8);
void gassset(Player*, s16, s8, s8);

void letterset(Player*, s8, s8);
void OBJCalculation(Player*, s8, s8);
void exce_obj_calculation(Player*, s8, s8);
void obj_calculation1(Player*, s8, s8);

void kart_effect_strategy(void);
void effect_strategy(Player*, s8, s8);
void SmokeDisp(Player*, s8, s8);
void SmokeDisp2P(Player*, s8, s8);
void SmokeDisp3P(Player*, s8, s8);
void SmokeDisp4P(Player*, s8, s8);
void SmokeDispAfter(Player*, s8, s8);
void SmokeDispAfter2P(Player*, s8, s8);
void SmokeDispAfter3P(Player*, s8, s8);
void SmokeDispAfter4P(Player*, s8, s8);
void func_80075CA8(void);
void func_80085214();

extern s16 D_800E4730[];
extern u8** D_800E4770[];
extern u8** D_800E47A0[];
extern s32 NORM_SMOKE_IN[];
extern s32 NORM_SMOKE_OUT[];

extern f32 g_waterlevelPlayer[];

extern s32 KW16GFTimer;
extern s32 KW8GFTimer;
extern s32 KW4GFTimer;
extern s32 KW2GFTimer;
extern s32 KW64GFCount;
extern s32 KW32GFCount;
extern s32 KW16GFCount;
extern s32 KW8GFCount;
extern s32 D_801655C0;
extern s32 KW4GFCount;
extern s32 D_801655C8;
extern s32 KW2GFCount;
extern s32 D_801655D8;
extern s32 D_801655E8;
extern s32 D_801655F0;
extern s32 D_801655F8;

extern s32 KWFlash8;
extern s32 D_80165618;
extern s32 D_80165628;
extern u32 D_80165638;
extern u32 D_80165648;
extern u32 D_80165658[];
extern s32 D_80165678;
extern u16 D_801656B0;
extern u16 kwflash_r;
extern u16 kwflash_g;
extern u16 kwflash_b;
extern s16 D_801656F0;
extern s16 D_80165708;
extern s16 D_80165710;
// extern s16 D_80165730; -> objects.h
// extern s16 D_80165738; -> objects.h

extern s16 D_80165740;
extern s16 D_80165748;
extern s16 D_80165718;
extern s16 D_80165720;
extern s16 D_80165728;

// extern s16 gNumActiveThwomps; -> objects.h

extern s32 D_80165754;
// extern ThwompSpawn *gThowmpSpawnList; -> objects.h

extern Vec4s D_80165760;
extern s8 D_8016576A;
extern Vec4s D_80165770;
extern s16 D_8016578C;
extern Vec4s D_80165780;

extern s16 D_80165790;
extern s16 D_80165794;
extern s8 D_8016579C;
extern u16 D_8016579E;
extern u16 D_801657A2;
extern s8 D_801657AE;
extern s8 g_KWDBDispSW;
extern s8 D_801657B2;
extern s8 D_801657B4;
extern s8 D_801657B8[];
extern s8 g_cloudsToggle;
extern s8 D_801657D0[];
extern s8 g_KWDemoSW;
extern s8 D_801657E1;
extern s8 kwgoalpanelsw;
extern s8 D_801657E3;
extern s8 g_hudLapToggle;
extern s8 D_801657E5;
extern bool8 g_hudSpeedToggle2;
extern u8 D_801657E7;
extern bool8 g_hudMapToggle2;
extern bool8 g_blueLineRankToggle;
extern bool8 g_KWLapSW;
extern s32 D_801657FC;

extern s8 D_80165800[2];
extern s32 D_80165804;
extern s8 g_hudToggleFlag;
extern s32 D_8016580C;
extern bool8 g_hudSpeedToggle;
extern s32 D_80165814;
extern bool8 D_80165818;
extern s32 D_8016581C;
extern s8 D_80165820;
extern s8 D_80165828;
extern Vec3su D_8016582C;
extern s8 g_hudToggleFlagP2[2];
extern Vec3su D_80165834;
extern s8 D_80165840[];
extern s32 D_80165860;
extern s32 D_8016586C;
extern s32 D_80165878;
extern s32 D_8016587C;
extern u8* D_80165880;
extern s8 D_80165888;
extern s8 D_80165890;
extern s8 balloonsw;
extern s32 D_8016589C;
extern s8 D_801658A8;
extern s8 D_801658BC;
extern s8 D_801658C6;
extern s8 D_801658CE;
extern s8 D_801658D6;
extern s8 D_801658DC;
extern s8 D_801658E4;
extern s8 D_801658EC;
extern s8 D_801658F4;
extern u8 sRandomItemIndex;
extern s8 D_801658FE;
extern u8 gControllerRandom;

extern s16 D_80165900;
extern s8 D_80165908;

extern s8 D_80165A90;
// extern AnmObject g_DynamicObjects[]; -> objects.h

extern s32 objectListSize;
extern Mtx D_80183D60;
// extern s32 D_80183DA0; -> objects.h

extern f32 D_80183DA8[];
// extern s32 jugemuallocptr[]; -> objects.h

extern f32 D_80183DC8[];
// extern s32 bomballocptr[]; -> bomb_kart.h
// extern s32 gNextFreeObjectParticle1; -> objects.h

extern Vec3f D_80183E40;
// extern s32 gNextFreeObjectParticle2; -> objects.h

extern Vec3f D_80183E50;
// extern s32 gNextFreeObjectParticle3; -> objects.h
// extern s32 FireParticleCounter; -> objects.h

extern Vec3f D_80183E70;
// extern s32 gNextFreeLeafParticle; -> objects.h

extern Vec3su D_80183E80;
// extern s32 ItemBoxAllocPtr[]; -> objects.h

extern Vec3su D_80183E98;
// extern s32 objallocptr[]; -> objects.h
// extern s32 objallocptr2[]; -> objects.h

extern u8 D_80183FA8[4][0x2000];
// extern s32 objallocptr3[]; -> objects.h

extern u8* gLakituTexturePtr;
// extern s32 objallocptr4[]; -> objects.h
// extern Collision D_8018C0B0[]; -> objects.h
// extern s32 EffectAllocArray1[]; -> objects.h

extern Collision D_8018C3B0;
// extern s32 EffectAllocArray2[]; -> objects.h
// extern s32 EffectAllocArray3[]; -> objects.h

extern Collision D_8018C830;
// extern s32 FireParticleAllocArray[]; -> objects.h
// extern s32 leafallocptr[]; -> objects.h

extern Hud playerHUD[];
// extern s32 g_CloudAllocate[]; -> objects.h

extern struct_D_8018CE10 D_8018CE10[];
// extern s32 D_8018CF10; -> objects.h

extern Camera* D_8018CF14;
extern s16 D_8018CF18;
extern Player* D_8018CF1C;
extern s16 D_8018CF20;
extern Player* D_8018CF28[];
extern s16 D_8018CF48;
extern s16 D_8018CF50[];
extern s16 D_8018CF60;
extern s16 D_8018CF68[];
extern s16 D_8018CF78;
extern s16 gGPCurrentRaceCharacterIdByRank[];
extern f32 D_8018CFCC;
extern f32 D_8018CFD4;
extern s16 D_8018CF90;
extern s16 KWRank[];
extern s16 D_8018CFA8;
extern s16 D_8018CFB0;
extern s16 D_8018CFB8;
extern s16 D_8018CFC0;
extern s16 D_8018CFC8;
extern s16 D_8018CFD0;
extern s16 D_8018CFD8;
extern u8 D_8018CFAC[];
extern u8 D_8018CFB4[];
extern u8 D_8018CFBC[];
extern u8 D_8018CFC4[];
extern s16 D_8018CFE0;
extern f32 D_8018CFE4;
extern s16 D_8018CFE8;
extern f32 D_8018CFEC;
extern s16 D_8018CFF0;
extern f32 D_8018CFF4;
extern s16 D_8018CFF8;
extern s16 D_8018D000;
extern s16 D_8018D008;
extern f32 D_8018D00C;
extern s16 D_8018D010;
extern s16 D_8018D018;
extern f32 xOrientation;
extern s16 D_8018D020;
extern f32 panelx[];
extern s16 D_8018D048;
extern f32 g_hudCharpicRankY1[];
extern s16 D_8018D070;
extern f32 D_8018D078[];
extern s16 D_8018D098;
extern f32 D_8018D0A0[];
extern s16 D_8018D0C0;
extern f32 D_8018D0C8[];
extern s16 D_8018D0E8;
extern f32 D_8018D0F0[];
extern s16 D_8018D110;
extern s32 D_8018D114;
extern s32 modelingnum;
extern s32 D_8018D140;
extern s32 D_8018D150;
extern s32 kwkartnum;
extern s32 D_8018D160;
extern s32 D_8018D168;
extern s16 D_8018D16C;
extern s32 D_8018D170;
// extern s16 D_8018D174; -> objects.h

extern s32 D_8018D178;
// extern s16 D_8018D17C; -> objects.h

extern s32 D_8018D180;
extern s16 D_8018D184;
extern s32 g_KWScreenEnable;
extern s16 D_8018D18C;
extern s32 D_8018D190;
// extern s8 D_8018D198[]; -> objects.h

extern s32 D_8018D1A0;
// extern s8 D_8018D1A8[]; -> objects.h

extern s32 D_8018D1B4;
// extern s8 D_8018D1B8[]; -> objects.h

extern s32 D_8018D1C4;
// extern s32 D_8018D1C8; -> objects.h

extern s32 D_8018D1CC;
// extern s32 D_8018D1D0; -> objects.h

extern s32 D_8018D1D4;
// extern s32 D_8018D1D8; -> objects.h

extern s32 D_8018D1DC;
extern u8* logoptr;
extern f32 D_8018D1E8;
extern s32 D_8018D1EC;
// extern s32 CloudCount1P; -> objects.h
// extern s32 CloudCount2P; -> objects.h

extern s32 D_8018D1FC;
extern s16 D_8018D200;
extern s32 D_8018D204;
extern s16 D_8018D208;
extern s32 D_8018D20C;
extern s16 D_8018D210;
extern bool D_8018D214;
extern s16 D_8018D218;
extern s32 D_8018D21C;
extern u8 (*kumoptr)[1024];
extern s32 D_8018D224;
extern u8 kumotype;
extern s32 D_8018D22C;
// extern s8 kwhoshisw; -> objects.h

extern s32 g_mapTexture;
extern u8* D_8018D248[];
extern f32 g_mapScale;
extern s32 D_8018D2A4;
extern s32 D_8018D2AC;
extern s16 g_mapWidth;
extern s32 D_8018D2B4;
extern s16 g_mapHeight;
extern s32 D_8018D2BC;
extern s16 g_mapX[];
extern s32 D_8018D2C8[];
extern s16 g_mapY[];
extern s16 g_startX;
extern s16 g_startY;
extern s16 rzerox;
extern s16 rzeroy;
extern u16 g_mapR;
extern u16 g_mapG;
extern u16 g_mapB;
extern s32 D_8018D314;
extern u16 ralpha;
extern s32 kwgoalcnt;
extern s32 D_8018D380;
extern s32 D_8018D384;
extern s32 D_8018D388;
// extern s32 D_8018D3BC; -> objects.h
// extern s32 D_8018D3C0; -> objects.h
// extern s32 D_8018D3C4; -> objects.h

extern s32 kwr;
extern s32 kwg;
extern s32 kwb;
extern s32 kwa;
extern s32 wakur;
extern s32 wakug;
extern s32 wakub;
extern s32 wakua;
extern s32 wakuanmtmmax;
extern s32 wakuanmptr;
extern s32 kwvideoframetimer;
extern s32 kwgameframetimer;
extern s32 D_8018D40C;
extern s32 D_8018D410;
extern u8* drift_smoke_V;
extern u8* drift_smoke_E;
extern u8* D_8018D428;
extern u8* D_8018D42C;
extern u8* D_8018D430;
extern u8* D_8018D434;
extern u8* dobon1;
extern u8* dobon2;
extern u8* dobon3;
extern u8* dobon4;
extern u8* dobon5;
extern u8* dobon6;
extern u8* dobon7;
extern u8* dobon8;
extern u8* dobon9;
extern u8* dobon10;
extern u8* dobon11;
extern u8* dobon12;
extern u8* dobon13;
extern u8* dobon14;
extern u8* dobon15;
extern u8* kage_bitmap;
extern u8* D_8018D478;
extern u8* fukidashi2;
extern u8* onpu;
extern u8* hoshi;
extern u8* D_8018D48C;
extern u8* D_8018D490;
extern u8* smoke;
extern u8* grs;
extern u8* bubble;
extern u8* boing;
extern u8* poomp1;
extern u8* poomp2;
extern u8* whir1;
extern u8* whir2;
extern u8* crash1;
extern u8* crash2;
extern u8* balloon1;
extern u8* balloon2;
extern u8* thunderA;
extern u8* thunderB;
extern Vec3f gPlayerBalloonPosX[];
extern Vec3f gPlayerBalloonPosY[];
extern Vec3f gPlayerBalloonPosZ[];
extern u16 gPlayerBalloonStatus[8][3];
extern Vec3s D_8018D620[];
extern Vec3f D_8018D650[];
extern Vec3f D_8018D6B0[];

extern Vec3f D_8018D710[];
extern Vec3s D_8018D770[];
extern Vec3s D_8018D7A0[];
extern Vec3s D_8018D7D0[];

extern Vec3s D_8018D800[];
extern Vec3s D_8018D830[];
extern Vec3s gPlayerBalloonRotation[]; // Sprite rotation for each player balloon
extern Vec3s D_8018D890[];
extern s16 BalloonCount[]; // D_8018D8C0
extern Vec3s gPlayerBalloonDepartingTimer[];

/** @endcond */

#endif

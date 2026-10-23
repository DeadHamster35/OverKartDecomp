#include "../../MainInclude.h"

/* osTvType → osTvType */


extern long spriteKillA;
extern long spriteKillC;
extern short spriteKillB;
extern short spriteKillD;





/* BalloonDisp / SmokeDisp* / OBJCalculation: code_80057C60.h */

extern void CrashScreenDrawInfo(Ushort* Framebuffer, OSThread* Thread);
extern void decodeTKMK(int Input, int *temp, int Output, int transparent);


extern void LoadMap(int courseID);


extern void DMABuffer(void* Car, void* Camera, char kno, char place); //0x80020000
extern void DrawBuffer(void* Car, char kno, char place); //0x80021244


/* KWCheckRadiusXZ: math_util_2.h */
extern void DisplayShadow(Vec3f Position, Vec3s Angle, f32 Size);
extern void DisplayFlagGate(Camera* PlayerCamera);
/* MoveIWA → UpdateActorFallingRocks */
extern void DeleteObjectBuffer(Object *Object);
extern s16 AddObjectBuffer(Vec3f Position, Vec3s Angle, Vec3f Velocity, s16 objectID);

//float Radius,float hight,float Param
extern bool CollisionCylinder(Player *Car, Vec3f Position, f32 Radius, f32 Height, f32 Parameter); //0x8029EEB8
extern bool CollisionSphere(Player *Car, Object *Object); //0x8029FB80
extern void KillObject(Object* ObjectTarget);

/* RouletteStart / KWAnmStart: update_objects.h */

//math
extern void BrkProgram();
extern void SetFVector(Vector vct,float a,float b,float c);
extern Vector *SetVector(Vector vector, float vecx, float vecy, float vecz);
extern void InitialLight(short yaw,short Pitch,Vector light);
extern void CreateCameraAffineMtx(AffineMtx Matrix, SVector Position, SVector Angle);
extern void CreateCameraAffineMtx2(AffineMtx Matrix, SVector Position, SVector Angle);
extern void NomralizeVector(Vector vector);
extern void MakeTopMatrix(Matrix mf,float a, float x, float y, float z);
extern void MakeTopAlign(Matrix mf, float x, float y, float z);
extern float Atan2fx(float x, float y);
extern Ushort Atan2tx(float x, float y);
extern float Atanf(float x);
extern float Asinf(float x);
extern short Acost(float x);
extern short MakeDirection(float x1,float y1,float x2,float y2);

/* sinf / cosf → sinf / cosf */

extern Vtx* gBackgroundVtx;


extern void RotateLightMatrix(Uint lpointer,AffineMtx m,short yaw,short Pitch,int Count);
extern void SetUpVector(void *Car);
extern float SinTable[1024];
extern double Ipower(double x,int n);
extern double Power(double x,double y);
extern double Llog(double x);
extern double Lexp(double x);
extern double Lldexp(double x,int k);
extern double Ffrexp(double x,int *exp);
extern void SwapUint(Uint *a,Uint *b);


extern void SetPalette(int palette_number);
extern void SetWord(int x,int y,char *printText);
extern void SetWord2(int x,int y,char *printText);
extern void SetWord2A(int x,int y,char *printText,int interval,float mulx,float muly,int Type);
extern void SetWord2AP(int x,int y,char *printText,int interval,float mulx,float muly);
extern void SetWord3(int x,int y,char *printText,int interval,float mulx,float muly,int Type);
extern void SetWord3A(int x,int y,char *printText,int interval,float mulx,float muly);
extern void SetWord3AC(int x,int y,char *printText,int interval,float mulx,float muly);
extern void SetWord3AP(int x,int y,char *printText,int interval,float mulx,float muly);
extern void SetWord3ACP(int x,int y,char *printText,int interval,float mulx,float muly);
extern void SetWord4(int x,int y,char *printText,int interval,float mulx,float muly,int Type);
extern void SetWord4A(int x,int y,char *printText,int interval,float mulx,float muly);
extern void SetWord4AP(int x,int y,char *printText,int interval,float mulx,float muly);

extern void KawanoDrawFinal(void);


/* PrintNumber / PrintStringBinary / PrintStringHex: render_objects.h */
/* KWDisplay2D / KWDisplay2DAfter: code_80057C60.h */
extern void SetFadeOutB();
extern void SetObjBlock(int Kind, int x, int y, char pri);
extern void InitObjBlock();
extern void DoObjBlock(int pri_flag);
extern void DispObjBlock(void* Target);

/* MSelController / PSelController / GSelController / TitleController: menus.h */

extern void DOBPSelTurnIn(ObjBlock Target); //0x800AAB90
extern void DOBPSelTurnOut(ObjBlock Target); //0x800AAA9C

/* Camera.ScreenViewAngle is the member; global FOV array is CameraZoom */



extern void ThunderWorld();



extern void F_80090178(Player *Kart, char PlayerID, float *SpawnVector, float *FacingVector);
/* func_80090178 / SetFastOoB / func_80090868 → func_80090178 / func_80090868 */

extern void DoGameSelect10();
/* LakituIceBehavior: KwturiageJugemu+0x3c SMC; live C is OKLakituIce */





/* KWFlash8 / KW*GFTimer / KW*GFCount: code_80057C60.h */

/* Vtx_KTile* / KWTextureRGBAXLUAAZBBlInit: common_data.h (Vtx, not Vtx_t) */
extern Vtx VtxKTile8X8[];
extern Vtx VtxKTile8X16[];
extern Vtx VtxKTile16X16[];
extern Vtx VtxKTile16X16XFlip[];
extern Vtx VtxKTile32X32[];
extern Vtx VtxKTile32X32XFlip[];
extern Vtx VtxKTile40X32[];
extern Vtx VtxKTile64X64BL[];
extern Vtx Vtx_KTile64x64XFlipBL[];
extern Vtx Vtx_KTile64x96BL[];
extern Vtx Vtx_KTile96x16[];

extern Gfx KWTextureRGBAXLUAAZBBlInit[];

extern u8 CommonGrandPrixHumanItemCurve[][100];
extern u8 CommonGrandPrixCPUItemCurve[][100];
extern u8 CommonVersus2PlayerItemCurve[][100];
extern u8 CommonVersus3PlayerItemCurve[][100];
extern u8 CommonVersus4PlayerItemCurve[][100];
extern u8 CommonBattleItemCurve[][100];



/* CheckSplashJAL1/2/3: RunKart / SpinKart / ResultRunkart JALs; live C is CheckSplash */

/* CheckFinalLapFanfareJAL / CheckPlayStarBGMJAL: race_logic / effects JALs; live C is PlayFinalLapMusicHook / PlayStarMusicHook */

/* KWChartKumo: update_objects.h */
extern long CloudTypeMapCheck1;
extern long CloudTypeMapCheck2;
extern long CloudAmountMapCheck1;
extern long CloudAmountMapCheck2;
/* Snow3DAllocMapCheck1/2: KwallocEffectram1P CourseID LUI/ORI; live C is Snow3DCourseID */
/* Snow3DDisplayAfterMapCheck1/2: RenderSnowingEffect CourseID LUI/ORI; live C is Snow3DCourseID */


extern void DMABase729A30(int Input, int Length, long Output);
extern void TexBuffLoadP(void *texlist_ptr,int nocheck_flg);
extern void GrayScaleTexBuf3(Uint num, Uint step);
extern void GrayScaleTexBufRGB(Uint num, int Size, int r, int g, int b);
extern void FadeMain();
extern void FadeMain2(int i);
extern void SetFadeOut(int Fade);
extern short PutPylon(Vector pos,short Number);
extern s16 PutObject(Vec3f pos, s16 Category);

extern char FadingFlag[5];
extern int FadeCounter[5];
extern int FadeCounter2[5];


/* KWLookCamera / KWLookCameraPitch: math_util_2.h (third arg is Camera->CameraPos) */

extern void KWRectangle(int sx,int sy,int sizex,int sizey,int s ,int t,int Mode);
/* KWTexture2D* / KWTexture3DRGBA32AAZBBL: render_objects.h */

/* KWPrintLapTimeXLU: render_objects.h */
/* KWLoadTextureBlockRGBA16B: render_objects.h */
/* SprDrawClip → SprDrawClip */
extern void SprDrawClipST(int sx,int sy,int sizex,int sizey,int ss,int tt,int Mode);
/* CommonTextureHUDNormalDigit → CommonTextureHUDNormalDigit */

/* KWTexture*_Sub*: render_objects.h */

/* KWCreateAffine3D / IceKage / Board / Hole / Anm: math_util_2.h */

/* KWSetPrimColor / KWSetEnvColor / Sub*Mode / *Mode (except ToumeIMode / Color2Mode): render_objects.h */

extern void BumpObject(Object* InputObject);

/* InitializePlayer: spawn_players.h */
/* DrawLocalSkeletonShape: animation.h (Armature*, Animation**) */

extern long asm_itemJump1A;// 0x8007B084  //3C058016
extern long asm_itemJump1B;// 0x8007B098  //84A543BA

extern long asm_itemJump2A;// 0x8007AFC0  //3C058016
extern long asm_itemJump2B;// 0x8007AFD4  //84A543BA

/* sqrtf → sqrtf */

extern void colorFont(int color);

extern long g_SegmentA;
extern long gIntMesgQueue;
//
//


/* GameState / GameState → GameState */
extern long NextSequenceModeUnused;
extern long g_gameType;
extern short g_DebugBars;



/* KartVtx: render_player.h */


extern long antialiasToggle;
extern long antialiasToggleB;


/* ((s16 *) CupCourseOrder) / g_cup*Array* → CupCourseOrder */

/* g_DebugTextPalette → CommonTLUTDebugFont (SetFontColor) */

extern long g_bannerTexture;
extern long g_previewTexture;
/* MapTexture / MapX / MapY / MapR / MapG / MapB / MapScale / StartX / StartY / MapWidth / MapHeight: code_80057C60.h */
/* g_mapStartToggle: stock JAL vs NOP at map start; live C is InitialMapCode */
extern short g_map2X; //0x8018D2C0
extern short g_map2Y; //0x8018D2D8


extern long g_DEBUG;

/* g_CoursePreviewOffsets → GetTBPointer(Seg2MarioRacewayPreviewTexture) in CustomLevels.c */
/* r_CoursePreviewOffsets → _data_segment2SegmentRomStart + SEGMENT_OFFSET(that Table) */
/* g_CourseBannerOffsets / g_BattleBannerOffsets → TexBuff slots via D_800E7DC4[] + SMenuTextureMap */
/* g_cup*preview* were stock RAM copies of the same MenuTexture[2] rows */

extern long 	g_NintendoLogoOffset; //0x8019F88C
extern int	CheckContPackMenu();
extern long 	g_NintendoLogoBorder;

extern char g_lakituStatus; // 0x80165DCE


/* (Controllers[0].ButtonHeld) / (Controllers[0].ButtonHeld) / player1input* / (Controllers[0].ButtonHeld) → Controllers[0] */
extern char c_Input;

/* g_player1Location* / g_player1Speed* → GamePlayers[0] */
extern float g_player1LastLocationX;
extern float g_player1LastLocationY;
extern float g_player1LastLocationZ;
/* (GamePlayers[0].Direction[0]) → GamePlayers[0].Direction[0] */
/* (GamePlayers[0].Velocity[0])/Y/Z/A → GamePlayers[0] */
extern float g_player1SpeedU1;
extern float g_player1SpeedU2;
extern long g_playerStatus;

extern float g_player1CameraX;
extern float g_player1CameraY;
extern float g_player1CameraZ;
extern float g_playerSpriteSize;

extern float player2X;
extern float player2Y;
extern float player2Z;
extern float player3X;
extern float player3Y;
extern float player3Z;
extern float player4X;
extern float player4Y;
extern float player4Z;

extern long cpu2Speed;
extern long cpu3Speed;
extern long cpu4Speed;

extern char boost1;
extern char boost2;
extern char boost3;
extern char boost4;

extern void GULookAt(Mtx *m, float xEye, float yEye, float zEye,
	       float xAt,  float yAt,  float zAt,
	       float xUp,  float yUp,  float zUp);
extern void GUPerspective(Mtx *m, u16 *perspNorm, float fovy, float aspect, float near, float far, float Scale);

/* (GlobalScreen[0]->Width) / (D_8015F480[0].Flag) / (D_8015F480[0].CameraPoint) */
extern short g_player1ScreenHeight; //0x8015F4AE
extern short g_player1ScreenX; //0x8015F4B0
extern short g_player1ScreenY; //0x8015F4B2

extern short g_player2ScreenWidth; //0x8015F4AC
extern short g_player2ScreenHeight; //0x8015F4AE
extern short g_player2ScreenX; //0x8015F4B0
extern short g_player2ScreenY; //0x8015F4B2
extern short g_player2View; //0x8015F4B4
extern short g_player2Section; //0x8015F4B8

extern short g_player3ScreenWidth; //0x8015F4EC
extern short g_player3ScreenHeight; //0x8015F4EE
extern short g_player3ScreenX; //0x8015F4F0
extern short g_player3ScreenY; //0x8015F4F2
extern short g_player3View; //0x8015F4F4
extern short g_player3Section; //0x8015F4F8

extern short g_player4ScreenWidth; //0x8015F42C
extern short g_player4ScreenHeight; //0x8015F42E
extern short g_player4ScreenX; //0x8015F430
extern short g_player4ScreenY; //0x8015F432
extern short g_player4View; //0x8015F434
extern short g_player4Section; //0x8015F438


/* TimePlayerLastTouchedFinishLine → TimePlayerLastTouchedFinishLine */
extern float g_lap2Time;
extern float g_lap3Time;


/* NearestPathPointByPlayerId[0] → NearestPathPointByPlayerId[0] */


/* g_PlayerRankTable / GPCurrentRaceRankByPlayerId[0]-8 → GPCurrentRaceRankByPlayerId */

/* Gravity1: kart_attributes.h (f32[8]) */
extern float gravity_2;
extern float gravity_3;
extern float gravity_4;
extern float gravity_5;
extern float gravity_6;
extern float gravity_7;
extern float gravity_8;

/* MakeBalloonCalc prim/env; DrawBalloon B tables. Live BSS: LibraryVariables.c */
extern BalloonColor  BalloonColorArray[8];
extern BalloonColor  BalloonAdjustArray[8];
extern BalloonColor  BalloonColorArrayB[8];
extern BalloonColor  BalloonAdjustArrayB[8];

extern short surface_p0;

/* LapCountByPlayerId / g_gameLapPlayer* → LapCountByPlayerId */


/* RenderSnowingEffect → RenderSnowingEffect */



extern char itemBoolean; //0x80165F5F
extern char itemA;
extern char itemB;
extern char itemC;
extern char item2Boolean; //0x80165F5F
extern char item2A;
extern char item2B;
extern char item2C;

//extern long g_TimeLapTable; //0x8018CA70          !!! renamed to PlayerHUD !!!
//extern char g_lapCheckA;                          !!! removed inside HUD struct now: finlineAnim2 !!! U use this in MarioKartPractice.c
//extern char g_lapCheckB;                          !!! removed inside HUD struct now: lapCount !!! U use this in MarioKartPractice.c

/* HUDToggleFlag / HUDToggleFlagP2 / KWDBDispSW / HUDMapToggle2 / HUDSpeedToggle / HUDSpeedToggle2 / HUDLapToggle / BlueLineRankToggle / KWLapSW / KWDemoSW / HUDCharpicRankY1: code_80057C60.h */
extern short g_hudMapToggle; // 0x80165800
/* D_80165800 → D_80165800 */

//hud all players
extern char HUDToggle; // 0x800DC5B9 
extern char g_mapPlayers; // 0x8018D15B

//hud p1 only
extern char g_hudCharpicRankToggle; // 0x8018D2BF
extern char g_hudCharpicRankA; // 0x8018D3E3
extern float g_hudCharpicRankX1; // 0x8018D027 //float
extern float g_hudCharpicRankX2; // 0x8018D02B
extern float g_hudCharpicRankX3; // 0x8018D02F 
extern float g_hudCharpicRankX4; // 0x8018D033 
extern float g_hudCharpicRankY2; // 0x8018D054 
extern char g_hudCharpicRankY3; // 0x8018D058 
extern float g_hudCharpicRankY4; // 0x8018D05C 


extern short asm_DisableHUD; //0x80059D08


extern short g_GhostHUDID; //0x8018DAAA

extern long g_mlogoY;// 0x8018D9F0
/* (MenuItems[2].Kind) / (MenuItems[3].Kind) / (MenuItems[4].Kind) → MenuItems[].Kind */

/* KBGNumberNext / MenuScreenA/B/C / TitleDemo / CupSelect / CourseSelect / InGameTT / InGame: menus.h */

/* AsmCupCount: CheckFinish last-course immediate; live BSS is LibraryVariables.c */


/* songID / g_musicIDRaceways: StartRaceBGM NaSeqStart immediate; StartRaceBGMCustom */
/* asm_SongA / asm_SongB: StartRaceBGM / CheckPause 3P4P music guards; OKMusicIn3P4P */

extern Uint CullDL_Parameters;
extern long g_courseTable;
/* KeystockBuffer: staff ghost D_802BFB80.arraySize8[0][2][3]. KeystockCounter: sCourseGhostFramesRemaining */




//-------------- ACTION DATA --------------
#define OGA_ACT_NULL            0  // Null
#define OGA_ACT_DRIFT            1  // Drift
#define OGA_ACT_N_JUMP            2  // Jump
#define OGA_ACT_CENTER            3  // Road Center
#define OGA_ACT_LEFT            4  // Road Left
#define OGA_ACT_RIGHT            5  // Road Right
#define OGA_ACT_SPEED_FREE        6  // Speed Normal
#define OGA_ACT_SPEED_MAX        7  // Speed Accel
#define OGA_ACT_SPEED_MIN        8  // Speed Brake
#define OGA_ACT_TENUKI_KINSI        9  // Avoid Calm
#define OGA_ACT_TENUKI_KYOKA        10 // Avoid Hectic
#define OGA_ACT_TURBO            11 // Allow Boost

/* (CoursesCPUBehaviour[0]) → CoursesCPUBehaviour[0] */
extern Ushort ActionDataNumCustom[8];


/* CharacterGridIsSelected → CharacterGridIsSelected */
extern char player1OK; //
extern char player2OK; //
extern char player3OK; //
extern char player4OK; //

extern void textDrawPtr(int *x, int *y, const char *str, int spacing, float xScale, float yScale);
extern void textDraw(int x, int y, const char *str, int spacing, float xScale, float yScale);
extern void DrawText(int x, int y, const char *str, int spacing, float xScale, float yScale);
extern void SetFadeOutTaData();

extern int GetWordLength(const char *str);

/* CharacterSelections / Player1Character: menus.h */
extern char g_player2Character;
extern char g_player3Character;
extern char g_player4Character;

extern int GFXCounter;
extern int PKCounter;
extern void ExecuteItem(Player* PlayerID);

/* MenuMultiplayerSelection: menus.h */

extern char g_placedBadGPadvance;
extern short g_P1TopSpeed;


/* g_menuUpdateTimer* → SEQDataBlock[].FrameCountDown LSB */

/* g_menuPreviewValue* / g_BattlePreviewValue* → MenuItems[].Work1 LSB */

/* g_zoomFOV* → CameraZoom; g_zoomLevel* → KoteiGakakuMode */

extern char g_ReplayFlag;
/* CameraZoom → CameraZoom */

extern char g_sfxPause;

extern long g_RawAudio; //0x803B9260

#include <PR/libaudio.h>
extern ALSeqFile *MUSTablePointer;
extern ALSeqFile *AlCtlHeader;
extern ALSeqFile *AlTbl;
extern u16 SequenceCount;
/* (*(SequenceTable *) MUSTablePointer) is MUSTablePointer; (*(InstrumentTable *) AlCtlHeader) is AlCtlHeader */
/* (*(RawAudioTable *) AlTbl) is AlTbl; g_MUSBankMapTable is AlBankSets (audio heap, not named BSS) */
extern SFXTempPointerStruct SFXPointer;

extern short g_surfaceCheckP1;
/* PlayerSurfaceSoundID: audio/external.h */

/* GamePausedFlag comes from audio/external.h */

/* MakePos / MakeStartup / MakeRDP / InitRndSmoke / InitRapidSmoke / InitSpinSmoke: code_80057C60.h */
extern void MakeRandomRDP(Smoke *data,int col,short A);

/* MakeBodyColor / MakeBodyColorAdjust: render_player.h */

/* (((s8 *) D_800E9F7C)[0x14]) → D_800E9F7C[].unk_14 */

/* StarUseCounter: render_player.h */
/* CoursesCPUBehaviour → CoursesCPUBehaviour */
/* CharRadiusTbl is in kart_attributes.h */


extern PlayerTextureTable TBSeq_psel; //0x800E8340
extern PlayerTextureTable TBSeqPselReturn; //0x800E8340
extern PlayerTextureTable TBSeqPselP0; //0x800E8340
extern PlayerTextureTable TBSeqPselP1; //0x800E8340
extern PlayerTextureTable TBSeqPselP2; //0x800E8340
extern PlayerTextureTable TBSeqPselP3; //0x800E8340
extern PlayerTextureTable TBSeqPselP4; //0x800E8340
extern PlayerTextureTable TBSeqPselP5; //0x800E8340
extern PlayerTextureTable TBSeqPselP6; //0x800E8340

//menu
extern short menuExtra;

//title
extern short titleBackX;
extern short titleBackY;
extern char titleBackAnim;
extern int titleLogoX; //0x8018D9E0
extern int titleLogoY; //0x8018D9F2
extern int titleCopyToggle; //0x8018D9EC //F1
extern short titleCopyX; 
extern short titleCopyY; 
extern long titleCopyChange; //0x8018DA50 //long F0 darker 04 show message
extern int titlePushToggle; //0x8018DA64
extern short titlePushX; 
extern short titlePushY; 
extern int titlePushBlink; // long/int?

//sky & clouds
/* CloudsToggle: code_80057C60.h */
extern char g_WinKart;


extern long SkyColorTopTable;
extern long SkyColorBotTable;

/* g_SnowParticleTex / g_skySnow* / g_3DSnow*: LibraryVariables.c. Sky spawn/HitGoal were KWChartSnow SMC. */
extern long *g_MRCloudTexPtr; // Set of four I4 cloud images; //0x400 Length each.

/* KWChartSnow / KWChartIceBlockA: update_objects.h */
extern void KWChartIceBlockA(s32 ObjectIndex, s32 PlayerId);

//racer values
/* TimePlayerLastTouchedFinishLine → TimePlayerLastTouchedFinishLine */
/* NearestPathPointByPlayerId / g_PathPointPlayer* → NearestPathPointByPlayerId */

//fog
/* SkyToggle / FogRed/G/B → SkyToggle / FogRed/Green/Blue */
/* KBGNumber / KBGChange: menus.h */

//lightning flags
/* LightningFlag / LightningFlagPlayer1 come from audio/external.h */

//Offroad flags
/* OffroadFlagPlayer1[]: spawn_players.h */
extern short g_offroadFlagPlayer2; // 0x80165332 
extern short g_offroadFlagPlayer3; // 0x80165334 
extern short g_offroadFlagPlayer4; // 0x80165336 

//waterlevel checks

//wrongway flags 
extern short g_wrongwayFlagPlayer2; // 0x80163272 
extern short g_wrongwayFlagPlayer3; // 0x80163274 
extern short g_wrongwayFlagPlayer4; // 0x80163276 

//Player shadow flags
extern char g_ShadowflagPlayer0; //0x800F6B87
extern char g_ShadowflagPlayer1; //0x800F795F
extern char g_ShadowflagPlayer2;
extern char g_ShadowflagPlayer3;

//GP points
extern Uchar PlayerGPpoints[8]; //name to num: Mario, Luigi, Yoshi, Toad, D.K., Wario, Peach, Bowser
extern Uint OSMemSize;


//multiplayer points
extern Uchar g_2PRacePoints[2];
extern Uchar g_3PRacePoints[3];
extern Uchar g_4PRacePoints[4];
extern Uchar g_2PBattlePoints[2];
extern Uchar g_3PBattlePoints[3];
extern Uchar g_4PBattlePoints[4];


//course
extern short MonitorCounter;

//Player color Timers
/* ColorPlayer0R is u16[8] in render_player.c; 1R–7R */

//Sound and music
/* NAPlyTrgStart / NAPlyVoiceStart / NAEnmTrgStart / NaSeqStart / NaPlyLevel* / NaSceneLevel*: audio/external.h */
extern void playMusic(int MusicID);
extern void playMusic2(int musicID);
/* BattleSongID / g_musicIDBattle1: StartRaceBGM NaSeqStart immediate; StartRaceBGMCustom */

extern short g_musicIDRaceways; // 0x8028ECE6 
extern short g_musicIDToad; // 0x8028ECF6
extern short g_musicIDCountry; // 0x8028ED06
extern short g_musicIDBattle1; // 0x8028ED16
extern short g_musicIDKalamari; // 0x8028ED26
extern short g_musicIDKoopa; // 0x8028ED36
extern short g_musicIDBowser; // 0x8028ED46
extern short g_musicIDBanshee; // 0x8028ED56
extern short g_musicIDSnowy; // 0x8028ED66
extern short g_musicIDRainbow; // 0x8028ED76
extern short g_musicIDDK; // 0x8028ED86
extern short g_musicIDBattle2; // 0x8028ED96
/* g_musicTempo: audio BSS SeqPlayer tempo; live C is MusSeqTempoPercent / PlayFinalLapMusicHook */
extern Ushort g_fanfareTempo;
extern SeqPlayerStruct SeqPlayer[4];

/* NAIFxFlagEntry: audio/external.h */

//Cave Fire Particle Stuff
/* KWGetCaveFire: update_objects.h */
/* CaveFireColCheck: KWDisplayFireParticleSub switch bound SMC; DisplayFireParticleSub no longer pokes */
/* KWChaseSVal / KWChaseIVal / KWChaseFVal / KW2DMatrixInit: math_util_2.h */
/* SetColorRender → SetColorRender */
/* SprDrawSubCI8 / KWDisplayFireParticleSub: render_objects.h */

/* KWGetStar: update_objects.h */

extern Ushort RGBAFallingLeaf[];
extern Ushort RGBAQuestionMark[];
extern Ushort RGBALeaf[];
extern void InitialMapObject(void);
extern void SetTreeObject(struct ActorSpawnData *);
extern void SetPakkunObject(struct ActorSpawnData *);
/* LoadCourse → LoadCourse */
/* FallingRockGFX_U/L SMC → FallingRockGFX / FallingRockShadowGFX in actors.c */
extern int ShadowModel;
extern int HoleModel;
extern int ItemBoxModel;
extern void MoveFallingRock(Object *obj);

extern int CheckTriangleZXV(Bump *Bump,float Radius,float p1x,float p1y, float p1z, Ushort Pointer ,float lastx,float lasty,float lastz);
extern int CheckTriangleXYV(Bump *Bump,float Radius,float p1x, float p1y, float p1z ,Ushort Pointer ,float lastx,float lasty,float lastz);
extern int CheckTriangleYZV(Bump *Bump,float Radius,float p1x ,float p1y, float p1z, Ushort Pointer ,float lastx,float lasty,float lastz);

/* NaPlyLandStart / NaSeqFadeout: audio/external.h */

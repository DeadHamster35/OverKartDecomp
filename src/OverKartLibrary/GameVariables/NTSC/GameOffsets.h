#include "../../MainInclude.h"

/* SYSTEM_Region → osTvType in StockAliases.h */


extern long spriteKillA;
extern long spriteKillC;
extern short spriteKillB;
extern short spriteKillD;





/* BalloonDisp / SmokeDisp* / OBJCalculation: code_80057C60.h */

extern void CrashScreenDrawInfo(Ushort* Framebuffer, OSThread* Thread);
extern void decodeTKMK(int Input, int *temp, int Output, int transparent);

extern void GetFramebuffer(int PixelX,int PixelY,int Width,int Height,unsigned short *Source,unsigned short *Destination);

extern void LoadMap(int courseID);
extern void ClearFramebuffer();


extern void DMABuffer(void* Car, void* Camera, char kno, char place); //0x80020000
extern void DrawBuffer(void* Car, char kno, char place); //0x80021244


extern int KWCheckRadiusXZ(float x1,float y1,float x2,float y2,float Radius);
extern void DisplayKT1(Screen* Display);
extern void DisplayKT16(Screen* Display);
extern void DisplayShadow(Vector Position, SVector Angle, float Size);
extern void DisplayFlagGate(Camera* PlayerCamera);
/* MoveIWA → UpdateActorFallingRocks in StockAliases.h */
extern short DeleteObjectBuffer(void *Object);
extern short AddObjectBuffer(Vector Position, SVector Angle, Vector Velocity, short objectID);


//float Radius,float hight,float Param
extern int CollisionCylinder(void *Car, Vector Position, float Radius, float Height, float Parameter); //0x8029EEB8
extern int CollisionSphere(void *Car, void *Object); //0x8029FB80
extern void KillObject(Object* ObjectTarget);


extern void RouletteStart(int Player, int SpecialItem); //0x8007ABFC
extern void KWAnmStart(int Player, int SpecialItem); //0x8007ABFC

//math
extern int CalcDisplayPosition(Screen *screen,Vector origin,float x,float y);
extern void BrkProgram();
extern int SetMatrix(AffineMtx Matrix, int Mode);
extern float CalcDistance(Vector origin,Vector object);
extern Ushort CalcDirection(Vector origin,Vector object);
extern Ushort CalcDirectionS(Vector origin,SVector object);
extern void SetFVector(Vector vct,float a,float b,float c);
extern void SetSVector(SVector vct,short a,short b,short c);
extern Vector *CopyVector(Vector destination, Vector Source);
extern void CopySVector(SVector destination, SVector Source);
extern Vector *SetVector(Vector vector, float vecx, float vecy, float vecz);
extern void CopyAffine(AffineMtx Source,AffineMtx object);
extern void CopyLongArray(long *destination, long *Source, int Count);
extern void LoadIdentAffineMtx(AffineMtx Matrix);
extern void TranslateMatrix(AffineMtx Matrix,AffineMtx Matrix2,Vector vect);
extern void TranslateMatrix2(AffineMtx Matrix,AffineMtx Matrix2,Vector vect);
extern void CreateTransAffineMtx(AffineMtx Matrix,Vector Position);
extern void MakeGuPerspective(AffineMtx Matrix, Ushort *perspNorm, float fovy, float aspect, float near, float far, float Scale);
extern void MakeGuLookAt(AffineMtx Matrix, Vector eye, Vector look);
extern void CreateMtxRotateX(AffineMtx Matrix, short Angle);
extern void CreateMtxRotateY(AffineMtx Matrix, short Angle);
extern void CreateMtxRotateZ(AffineMtx Matrix, short Angle);
extern void RotateVector(Vector vector,SVector Angle);
extern void InitialLight(short yaw,short Pitch,Vector light);
extern void MakeMapLight(short yaw,short Pitch,int Count);
extern void MakeLight(Uint lpointer,short yaw,short Pitch,int Count);
extern void ScalingMatrix(AffineMtx Matrix, float Scale);
extern void CreateModelingMatrix(AffineMtx Matrix, Vector Position, SVector Angle);
extern void CreateCameraAffineMtx(AffineMtx Matrix, SVector Position, SVector Angle);
extern void CreateCameraAffineMtx2(AffineMtx Matrix, SVector Position, SVector Angle);
extern void NomralizeVector(Vector vector);
extern void MultipleMatrixByVector(Vector vector, Matrix Matrix);
extern void MultipleAffineMtxByVector(Vector vector, AffineMtx Matrix);
extern void MakeTopMatrix(Matrix mf,float a, float x, float y, float z);
extern void MakeAlignVector(Vector Vector,short OriginAngle);
extern void MakeAlignMatrix(Matrix mf, float x, float y, float z ,short roty);
extern void MakeTopAlign(Matrix mf, float x, float y, float z);
extern void MakeRotate(Matrix mf, short theta, float x, float y, float z);
extern void MakeAlign(Matrix mf, short theta, float x, float y, float z);
extern void CreateModelAffineMtx(AffineMtx Matrix, Vector Position, Vector Angle);
extern void MultiAffineMtx(AffineMtx mtx, AffineMtx ma, AffineMtx mb);
extern void AffineToMtx(void *Matrix, AffineMtx affine); 
extern Ushort GetAtanTable(float a, float b);
extern Ushort Atan2t(float y, float x);
extern float Atan2f(float x, float y);
extern float Atan2fx(float x, float y);
extern Ushort Atan2tx(float x, float y);
extern float Atanf(float x);
extern short Atant(float x);
extern float Asinf(float x);
extern short Asint(float x);
extern float Acosf(float x);
extern short Acost(float x);
extern Ushort MakeRandom(void);
extern Ushort MakeRandomLimmit(Ushort Limit);
extern short MakeDirection(float x1,float y1,float x2,float y2);
extern void MakeDirection3D(Vector Camera,Vector lookat,SVector ans);

extern float SinT(Ushort inputAngle);
extern float CosT(Ushort inputAngle);
/* sinF / cosF → sinf / cosf in StockAliases.h */

extern Vtx* gBackgroundVtx;


extern int CheckCone(Ushort Left,Ushort Right,Ushort Direction);
extern float CheckDisplayRange(Vector basepos, Vector markpos, Ushort CameraDirection, float Radius, float angle2, float limmit_distance);
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



extern long SetStar(void *Car, int PlayerIndex);
extern void ResetStar (void *Car, char PlayerIndex);
extern long SetTurbo(void *Car, char PlayerIndex);
extern long SetWing(void *Car, char PlayerIndex);
extern void ResetWing(void *car);
extern long SetStorm(void *Car, char PlayerIndex);
extern void ThunderWorld();
extern void VSGhost(void *Car,char PlayerID);
extern void SetVSGhost(void *Car,char PlayerID);
extern void ResetVSGhost(void *Car,char PlayerID);
extern void SetHeightJump(void *Car,char PlayerID);


extern void SetRollover(Player *Kart,char Place); //0x8008C528
extern void SetWheelspin(Player *Kart,char Place); //0x8008CDC0
extern void SetBroken(Player *Kart,char Place); //0x8008DABC
extern void SetThunder(Player *Kart,char Place); //0x8008DF98
extern void SetSpin(Player *Kart,char Place); //0x8008C73C
extern void SetBombThrowRollover(Player *Kart,char Place); //0x8008EAE0
extern void SetBombRollover(Player *Kart,char Place); //0x8008E6C0
extern void SetProWheelSpin(Player *Kart,char Place); //0x8008D0FC

extern void F_80090178(Player *Kart, char PlayerID, float *SpawnVector, float *FacingVector);
/* GetLakituSpawnPoint / SetFastOoB / CallLakitu → func_80090178 / func_80090868 */

extern void DoGameSelect10();
extern void SetLakitu(void *Car);
extern void LakituCheck(void *Car,char PlayerID);
extern void HangLakitu(void *Car,char PlayerID,char place);
/* LakituIceBehavior: KwturiageJugemu+0x3c SMC; live C is OKLakituIce */

extern void OGAContTrgChk(int CameraNumber);


extern void SlipCheck(Player *car,char kno);
extern void AddGravity(Player *car);
extern void ProStickAngle(Player *car, Controller *Cont, char Number);


/* KWFlash8 / KW*GFTimer / KW*GFCount: code_80057C60.h */

/* Vtx_KTile* / kwtexture_rgba_xlu_aa_zb_bl_init: common_data.h (Vtx, not Vtx_t) */
extern Vtx Vtx_KTile8x8[];
extern Vtx Vtx_KTile8x16[];
extern Vtx Vtx_KTile16x16[];
extern Vtx Vtx_KTile16x16XFlip[];
extern Vtx Vtx_KTile32x32[];
extern Vtx Vtx_KTile32x32XFlip[];
extern Vtx Vtx_KTile40x32[];
extern Vtx Vtx_KTile64x64BL[];
extern Vtx Vtx_KTile64x64XFlipBL[];
extern Vtx Vtx_KTile64x96BL[];
extern Vtx Vtx_KTile96x16[];

extern Gfx kwtexture_rgba_xlu_aa_zb_bl_init[];

extern u8 common_grand_prix_human_item_curve[][100];
extern u8 common_grand_prix_cpu_item_curve[][100];
extern u8 common_versus_2_player_item_curve[][100];
extern u8 common_versus_3_player_item_curve[][100];
extern u8 common_versus_4_player_item_curve[][100];
extern u8 common_battle_item_curve[][100];


extern void CheckSplash(void *Car,int PlayerIndex);

/* CheckSplashJAL1/2/3: RunKart / SpinKart / ResultRunkart JALs; live C is OKCheckSplashEnabled */

/* CheckFinalLapFanfareJAL / CheckPlayStarBGMJAL: race_logic / effects JALs; live C is PlayFinalLapMusicHook / PlayStarMusicHook */

extern void KWKumoAlloc(void);
extern void KWChartKumo(int screen_num);
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
extern short PutObject(Vector pos,int Category);

extern char FadingFlag[5];
extern int FadeCounter[5];
extern int FadeCounter2[5];

extern void DisplayBackground(Vtx_t *bg_vertex,Screen *screen,short screen_width,short screen_hight,float *ScreenViewAngle);

extern Ushort KWLookCamera(float x,float z,Camera *Camera);
extern Ushort KWLookCameraPitch(float y,float z,Camera *Camera);

extern void KWRectangle(int sx,int sy,int sizex,int sizey,int s ,int t,int Mode);
/* KWTexture2D* / KWTexture3DRGBA32AAZBBL: render_objects.h */

/* KWPrintLapTimeXLU: render_objects.h */
/* KWLoadTextureBlockRGBA16B: render_objects.h */
/* SPRDrawClip → SprDrawClip in StockAliases.h */
extern void SprDrawClipST(int sx,int sy,int sizex,int sizey,int ss,int tt,int Mode);
/* StockNumberSprites → common_texture_hud_normal_digit in StockAliases.h */

/* KWTexture*_Sub*: render_objects.h */

extern void	KWCreateAffine3D(Vector postion,USVector Angle,float Scale);
extern void	KWCreateAffine3DIceKage(Vector postion,USVector Angle,float Scale);
extern void	KWCreateAffine3DBoard(Vector Position,Vector Camera,float Scale);
extern void	KWCreateAffine3DHole(Vector Position,Vector Direction,float Scale);
extern void	KWCreateAffine3DAnm(int num);

/* KWSetPrimColor / KWSetEnvColor / Sub*Mode / *Mode (except ToumeIMode / Color2Mode): render_objects.h */


extern void InitializePlayer(int playerStructure, int characterID, float deltaX, float deltaZ, int characterID2, int unknown0xB000);
extern void DrawLocalSkeletonShape(Hierarchy* Skeleton,AnimePtr* Anime,short AnimeNumber,short CurrentFrame);

extern long asm_itemJump1A;// 0x8007B084  //3C058016
extern long asm_itemJump1B;// 0x8007B098  //84A543BA

extern long asm_itemJump2A;// 0x8007AFC0  //3C058016
extern long asm_itemJump2B;// 0x8007AFD4  //84A543BA

/* Sqrtf → sqrtf in StockAliases.h */

extern void colorFont(int color);

extern long g_SegmentA;
extern long gIntMesgQueue;
//
//


/* g_resetToggle / g_SequenceMode → GameState in StockAliases.h */
extern long NextSequenceModeUnused;
extern long g_gameType;
extern short CourseID;
extern short g_DebugBars;

extern Uint PathTable[21][4];
extern Uint PathTableB[21][4];

extern Ushort PathLengthTable[21][8];

/* KartVtx: render_player.h */


/* antialiasToggle / antialiasToggleB → __osViCurr/Next->features (sViContexts[].features) */


/* g_CupArray / g_cup*Array* → CupCourseOrder in StockAliases.h */

/* g_DebugTextPalette → common_tlut_debug_font (SetFontColor) */

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

/* g_NintendoLogoOffset (sMemoryPool+0x7fbc) / g_NintendoLogoBorder (func_800942D0+0x1dc): batch 6 overlay/SMC */
extern int	CheckContPackMenu();

extern char g_lakituStatus; // 0x80165DCE


extern short p_Input; //
extern char d_Input; //
extern char c_Input;

extern short player1inputX;
extern short player1inputY;
extern short p1Button;


extern float g_player1LocationX;
extern float g_player1LocationY;
extern float g_player1LocationZ;
extern float g_player1LastLocationX;
extern float g_player1LastLocationY;
extern float g_player1LastLocationZ;
extern long g_player1LocationA;
extern float g_player1SpeedX;
extern float g_player1SpeedY;
extern float g_player1SpeedZ;
extern float g_player1SpeedA;
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

extern void InitRDP();
extern void SetViewport(Screen *screen);
extern void ClearZBuffer();
extern void ClearZBuffer2(Screen *screen);
extern void InitialDrawCommon();
extern void GULookAt(Mtx *m, float xEye, float yEye, float zEye,
	       float xAt,  float yAt,  float zAt,
	       float xUp,  float yUp,  float zUp);
extern void GUPerspective(Mtx *m, u16 *perspNorm, float fovy, float aspect, float near, float far, float Scale);
extern float FarClip;

/* g_player1ScreenWidth → GlobalScreen[0]->Width in StockAliases.h */
extern short g_player1ScreenHeight; //0x8015F4AE
extern short g_player1ScreenX; //0x8015F4B0
extern short g_player1ScreenY; //0x8015F4B2
extern short g_player1View; //0x8015F4B4
extern short g_player1Section; //0x8015F4B8

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


/* g_TrialTime / g_lap2Time / g_lap3Time → TimePlayerLastTouchedFinishLine in StockAliases.h */


extern short g_progressValue;

extern long CourseObstacle; //0x8016359C

/* g_PlayerRankTable / g_playerPosition1–8 → GPCurrentRaceRankByPlayerId in StockAliases.h */

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

/* g_GameLapTable / g_gameLapPlayer* → LapCountByPlayerId in StockAliases.h */


/* KWDisplayEvent_After → RenderSnowingEffect in StockAliases.h */



extern char itemBoolean; //0x80165F5F
extern char itemA;
extern char itemB;
extern char itemC;
extern char item2Boolean; //0x80165F5F
extern char item2A;
extern char item2B;
extern char item2C;

//extern long g_TimeLapTable; //0x8018CA70          !!! renamed to g_hudStruct !!!
//extern char g_lapCheckA;                          !!! removed inside HUD struct now: finlineAnim2 !!! U use this in MarioKartPractice.c
//extern char g_lapCheckB;                          !!! removed inside HUD struct now: lapCount !!! U use this in MarioKartPractice.c

/* HUDToggleFlag / HUDToggleFlagP2 / KWDBDispSW / HUDMapToggle2 / HUDSpeedToggle / HUDSpeedToggle2 / HUDLapToggle / BlueLineRankToggle / KWLapSW / KWDemoSW / HUDCharpicRankY1: code_80057C60.h */
extern short g_hudMapToggle; // 0x80165800
/* RadarOn → D_80165800 in StockAliases.h */

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
extern long g_mflagID;// 0x8018DA30
extern long g_mpressstartID;// 0x8018DA58
extern long g_mracewayTime;// 0x8018DA80

/* KBGNumberNext / MenuScreenA/B/C / TitleDemo / CupSelect / CourseSelect / InGameTT / InGame: menus.h */

/* AsmCupCount: CheckFinish last-course immediate; live BSS is LibraryVariables.c */

extern void ResultsSequence();

/* songID / g_musicIDRaceways: StartRaceBGM NaSeqStart immediate; StartRaceBGMCustom */
/* asm_SongA / asm_SongB: StartRaceBGM / CheckPause 3P4P music guards; OKMusicIn3P4P */
/* CullDL_Parameters: packed decoder overlay; live C is Decodespculldisplaylist gsSPCullDisplayList(0, 7) */
extern long g_courseTable;
/* KeystockBuffer: staff ghost D_802BFB80.arraySize8[0][2][3]. KeystockCounter: sCourseGhostFramesRemaining */
extern SOUKOU_ZURE_TYPE LaneData[8];
extern float RoadWidth[21]; // MR default: 50.0f

extern void AreaBunkatuBP(int pathID);
extern void SidePointCalcBP(int pathID);
extern void CurveDataCalcBP(int pathID);	
extern void AngleDataCalcBP(int pathID);
extern void ShortcutDataCalcBP(int pathID);


extern void Func8001A588(int arg0, void *arg1, short *Arg2, char Arg3, int arg4);

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

extern void ActionStartCheck(int num, Player *Kart);
extern ActionData *ActPointPtr;
extern ActionData *ActData_Pointer;
extern Ushort ActionDataNumCustom[8];
extern short DriftFlg[10];
extern Ushort ActionFlg[10];


/* PlayerOK → CharacterGridIsSelected in StockAliases.h */
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

/* MenuMultiplayerSelection: menus.h */

extern char g_placedBadGPadvance;
extern short g_P1TopSpeed;


extern char g_menuUpdateTimer1;
extern char g_menuUpdateTimer2;
extern char g_menuUpdateTimer3;
extern char g_menuUpdateTimer4;

/* g_menuPreviewValue* / g_BattlePreviewValue* → MenuItems[].Work1 LSB in StockAliases.h */

/* g_zoomFOV* → CameraZoom; g_zoomLevel* → KoteiGakakuMode in StockAliases.h */

extern char g_ReplayFlag;
/* g_screenViewAngle → CameraZoom in StockAliases.h */

/* g_sfxPause is SequencePlayer overlay (gSequencePlayers+0x293); batch 6 */

/* g_MUSSequenceTable / g_MUSInstrumentTable / g_MUSRawAudioTable → StockAliases.h heap pointers */
/* g_MUSBankMapTable is AlBankSets (audio heap, not named BSS) */
/* MUSTablePointer is ALSeqFile* in audio/load.c, not AudioTablePointers */
extern SFXTempPointerStruct SFXPointer;

extern short g_surfaceCheckP1;
/* PlayerSurfaceSoundID: audio/external.h */

/* GamePausedFlag comes from audio/external.h */

/* MakePos / MakeStartup / MakeRDP / InitRndSmoke / InitRapidSmoke / InitSpinSmoke: code_80057C60.h */
extern void MakeRandomRDP(Smoke *data,int col,short A);

/* MakeBodyColor / MakeBodyColorAdjust: render_player.h */

extern void RunKart(void* Car, void* Camera, int place, int PlayerID); // 8002D268
extern void RunKartSimple(void* Car, void* Camera, int place, int PlayerID); // 8002F35C
/* g_playerEcho → D_800E9F7C[].unk_14 in StockAliases.h */

/* StarUseCounter: render_player.h */
extern long GhostUseCounter[8]; // 8018D950
extern long GhostUseTimer[8]; // 8018D970
/* ActionData_Pointer → CoursesCPUBehaviour in StockAliases.h */
extern short NoSimpleKartFlag[8]; // 801633F8
/* CharRadiusTbl is in kart_attributes.h */

extern long StringTableCourseGP[20]; // 800E7524
extern long StringTableCourse[20]; // 800E7574

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

extern int BombTable;

extern long SkyColorTopTable;
extern long SkyColorBotTable;

/* g_SnowParticleTex / g_skySnow* / g_3DSnow*: LibraryVariables.c. Sky spawn/HitGoal were KWChartSnow SMC. */
extern long *g_MRCloudTexPtr; // Set of four I4 cloud images; //0x400 Length each.

extern void KWChartSnow(void);
extern void KWChartIceBlock(void);

//racer values
/* g_timeLapChange → TimePlayerLastTouchedFinishLine in StockAliases.h */
/* g_playerPathPointTable / g_PathPointPlayer* → NearestPathPointByPlayerId */
extern short PlayerPathPointCopy[8]; // 80165320
extern short RivalOvertakeAllowFlag[10];
extern short EnemyTargetPlayer;
extern short RankUpdateFinishFlag;

//fog
/* g_fogToggleBanshee / g_fogR/G/B → SkyToggle / FogRed/Green/Blue in StockAliases.h */
/* KBGNumber / KBGChange: menus.h */

//lightning flags
/* LightningFlag / LightningFlagPlayer1 come from audio/external.h */

//Offroad flags
extern short OffroadFlagPlayer1; // 0x80165330 
extern short g_offroadFlagPlayer2; // 0x80165332 
extern short g_offroadFlagPlayer3; // 0x80165334 
extern short g_offroadFlagPlayer4; // 0x80165336 

//waterlevel checks

//wrongway flags 
extern short WrongwayFlagPlayer1; // 0x80163270 
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
extern void EtcEnemyDrive();
extern void CheckLapCount(int PlayerID, Player *car);
extern void WrapPathIndexAtFinish(float PosX, float PosY, float PosZ, short *wayPointIndex, int PathIndex);
extern short CalcOGAAreaSubBP(float mx, float my, float mz, Ushort t_group, int *b_num_ptr);
extern void InitCenterLine();
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
/* ColorPlayer0R is u16[8] in render_player.c; 1R–7R are StockAliases.h */

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
extern void KWGetCaveFire(int objnum);
/* CaveFireColCheck: KWDisplayFireParticleSub switch bound SMC; DisplayFireParticleSub no longer pokes */
extern int KWChaseSVal(short *var,short val,short step);
extern int KWChaseIVal(int   *var,int   val,int   step);
extern int KWChaseFVal(float *var,float val,float step);
extern void KW2DMatrixInit(void);
/* KWSet2Color → SetColorRender in StockAliases.h */
/* SprDrawSubCI8 / KWDisplayFireParticleSub: render_objects.h */

extern void KWGetStar(Vector Position,int Type);
extern void KWChartStar(void);

extern Ushort RGBAFallingLeaf[];
extern Ushort RGBAQuestionMark[];
extern void DisplayMap2();
extern void SearchListFile(Uint Addr);
extern void MakeCollision();
extern Ushort RGBALeaf[];
extern void InitialMap(void);
extern void InitialMapObject(void);
extern void SetTreeObject(Uint TargetAddress);  // 0x8029D584
extern void SetPakkunObject(Uint TargetAddress);  // 0x8029CC14
extern void SetItemBoxObject(Uint TargetAddress);  // 0x8029D830
/* LoadMapData → LoadCourse in StockAliases.h */
/* FallingRockGFX_U/L SMC → FallingRockGFX / FallingRockShadowGFX in actors.c */
extern void DisplayGroupmap(int Address, Screen* PlayerScreen);
extern int ShadowModel;
extern int HoleModel;
extern int ItemBoxModel;
extern void MoveFallingRock(Object *obj);

extern int CheckTriangleZXV(Bump *Bump,float Radius,float p1x,float p1y, float p1z, Ushort Pointer ,float lastx,float lasty,float lastz);
extern int CheckTriangleXYV(Bump *Bump,float Radius,float p1x, float p1y, float p1z ,Ushort Pointer ,float lastx,float lasty,float lastz);
extern int CheckTriangleYZV(Bump *Bump,float Radius,float p1x ,float p1y, float p1z, Ushort Pointer ,float lastx,float lasty,float lastz);

extern void SpinKart(Player* Car, Camera* Camera, char place, char kno);
extern void WeaponStatus(Player* Car, char kno, char place);
extern void SetStrategy(Player* Car, char kno, char place);
extern void CheckKartHit(Player* Car, char kno, char place);
extern float PowerCheck(Player* Car, char kno);
extern void DriftJump(Player* Car);
extern void CheckWall(Player* Car, char kno, Vector Velocity);
extern void AccelOn(Player* Car);
extern void AccelOff(Player* Car, float accelOff);
extern void SetRolloverFall(Player* Car, char kno);
extern void ResetRolloverFall(Player* Car, char kno);
extern void TirePosition(Player* Car, float new_x, float new_y, float new_z);
extern void EnemyTirePosition(Player* Car, float new_x, float new_y, float new_z);
extern void SetSlipAngle(Player* Car, char kno, float old_x, float old_z, float new_x, float new_z);
/* NaPlyLandStart / NaSeqFadeout: audio/external.h */

#include "../../MainInclude.h"

/* SYSTEM_Region → osTvType in StockAliases.h */


extern long spriteKillA;
extern long spriteKillC;
extern short spriteKillB;
extern short spriteKillD;


extern void DrawKart(); //0x0x800212B4
extern void DrawKart2P(); //0x0x800215DC
extern void DrawKart3P(); //0x0x8002186C
extern void DrawKart4P(); //0x0x800219BC

extern void DrawKartAfter(); //0x0x80021B0C
extern void DrawKart2PAfter(); //0x0x80021C78
extern void DrawKart3PAfter(); //0x0x80021D40
extern void DrawKart4PAfter(); //0x0x80021DA8


/* BalloonDisp / SmokeDisp* / OBJCalculation: code_80057C60.h */

extern void CrashScreenDrawInfo(ushort* Framebuffer, OSThread* Thread);
extern void DMAROMGhost();
extern void decodeTKMK(int input, int *temp, int output, int transparent);

extern void GetFramebuffer(int PixelX,int PixelY,int Width,int Height,unsigned short *Source,unsigned short *Destination);
extern void InitialBump(Bump *bump);
extern ushort CheckBump(Bump *bump,float Radius,float PositionX,float PositionY,float PositionZ);
extern ushort CheckBump2(Bump *bump,float Radius,float PositionX,float PositionY,float PositionZ,float LastX, float LastY,float LastZ);

extern void LoadMap(int courseID);
extern void KillDisplayList (uint Pointer);
extern void BumpVelocity(Vector Bump,float Distance ,Vector Velocity,float co);
extern void CalcBumpVelocity(Bump* InputBump, Vector Velocity);
extern void ScrollMapImage(int ObjectAddress,int ScrollS,int ScrollT);
extern void MakeWaterVertex(int ObjectAddress, char alpha, char red, char green, char blue);
extern void g_ClearFramebuffer();
extern short CheckArea(ushort pointer);
extern void CheckMapBG_ZX(Player *car,Vector normal,Vector velocity,Vector g_vector,float *dist,float *new_x,float *new_y,float *new_z);
extern void CheckMapBG_XY(Player *car,Vector normal,Vector velocity,float *dist,float *new_x,float *new_y,float *new_z);
extern void CheckMapBG_YZ(Player *car,Vector normal,Vector velocity,float *dist,float *new_x,float *new_y,float *new_z);


extern void DMABuffer(void* Car, void* Camera, char kno, char place); //0x80020000
extern void DrawBuffer(void* Car, char kno, char place); //0x80021244

extern void CheckDMA(); //0x80020524
extern void CheckDMA2P(); //0x8002088C
extern void CheckDMA3P(); //0x80020BF4
extern void CheckDMA4P(); //0x80020F1C

extern float CheckHight(float X_value, float Y_value, float Z_value);
extern int KWCheckRadiusXZ(float x1,float y1,float x2,float y2,float radius);
extern void DisplayKT1(Screen* Display);
extern void DisplayKT16(Screen* Display);
extern void DisplayShadow(Vector Position, SVector Angle, float Size);
extern void DisplayFlagGate(Camera* PlayerCamera);
/* MoveIWA → update_actor_falling_rocks in StockAliases.h */
extern short deleteObjectBuffer(void *Object);
extern short addObjectBuffer(Vector position, SVector angle, Vector velocity, short objectID);


//float radius,float hight,float param
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
extern ushort CalcDirection(Vector origin,Vector object);
extern ushort CalcDirectionS(Vector origin,SVector object);
extern void SetFVector(Vector vct,float a,float b,float c);
extern void SetSVector(SVector vct,short a,short b,short c);
extern Vector *CopyVector(Vector destination, Vector source);
extern void CopySVector(SVector destination, SVector source);
extern Vector *SetVector(Vector vector, float vecx, float vecy, float vecz);
extern void CopyAffine(AffineMtx source,AffineMtx object);
extern void CopyLongArray(long *destination, long *source, int count);
extern void LoadIdentAffineMtx(AffineMtx Matrix);
extern void TranslateMatrix(AffineMtx Matrix,AffineMtx Matrix2,Vector vect);
extern void TranslateMatrix2(AffineMtx Matrix,AffineMtx Matrix2,Vector vect);
extern void CreateTransAffineMtx(AffineMtx Matrix,Vector position);
extern void MakeGuPerspective(AffineMtx Matrix, ushort *perspNorm, float fovy, float aspect, float near, float far, float scale);
extern void MakeGuLookAt(AffineMtx Matrix, Vector eye, Vector look);
extern void CreateMtxRotateX(AffineMtx Matrix, short angle);
extern void CreateMtxRotateY(AffineMtx Matrix, short angle);
extern void CreateMtxRotateZ(AffineMtx Matrix, short angle);
extern void RotateVector(Vector vector,SVector angle);
extern void InitialLight(short yaw,short pitch,Vector light);
extern void MakeMapLight(short yaw,short pitch,int count);
extern void MakeLight(uint lpointer,short yaw,short pitch,int count);
extern void ScalingMatrix(AffineMtx Matrix, float scale);
extern void CreateModelingMatrix(AffineMtx Matrix, Vector Position, SVector Angle);
extern void CreateCameraAffineMtx(AffineMtx Matrix, SVector Position, SVector Angle);
extern void CreateCameraAffineMtx2(AffineMtx Matrix, SVector Position, SVector Angle);
extern void NomralizeVector(Vector vector);
extern void MultipleMatrixByVector(Vector vector, Matrix matrix);
extern void MultipleAffineMtxByVector(Vector vector, AffineMtx Matrix);
extern void MakeTopMatrix(Matrix mf,float a, float x, float y, float z);
extern void MakeAlignVector(Vector Vector,short OriginAngle);
extern void MakeAlignMatrix(Matrix mf, float x, float y, float z ,short roty);
extern void MakeTopAlign(Matrix mf, float x, float y, float z);
extern void MakeRotate(Matrix mf, short theta, float x, float y, float z);
extern void MakeAlign(Matrix mf, short theta, float x, float y, float z);
extern void CreateModelAffineMtx(AffineMtx matrix, Vector position, Vector angle);
extern void MultiAffineMtx(AffineMtx mtx, AffineMtx ma, AffineMtx mb);
extern void AffineToMtx(void *matrix, AffineMtx affine); 
extern ushort GetAtanTable(float a, float b);
extern ushort Atan2t(float y, float x);
extern float Atan2f(float x, float y);
extern float Atan2fx(float x, float y);
extern ushort Atan2tx(float x, float y);
extern float Atanf(float x);
extern short Atant(float x);
extern float Asinf(float x);
extern short Asint(float x);
extern float Acosf(float x);
extern short Acost(float x);
extern ushort MakeRandom(void);
extern ushort MakeRandomLimmit(ushort limit);
extern short MakeDirection(float x1,float y1,float x2,float y2);
extern void MakeDirection3D(Vector camera,Vector lookat,SVector ans);
extern int ChaseDir(short *org, short obj, short rate);

extern float sinT(ushort inputAngle);
extern float cosT(ushort inputAngle);
/* sinF / cosF → sinf / cosf in StockAliases.h */

extern Vtx* gBackgroundVtx;


extern int CheckCone(ushort left,ushort right,ushort direction);
extern float CheckDisplayRange(Vector basepos, Vector markpos, ushort camera_direction, float radius, float angle2, float limmit_distance);
extern void RotateLightMatrix(uint lpointer,AffineMtx m,short yaw,short pitch,int count);
extern void SetUpVector(void *Car);
extern float SinTable[1024];
extern float CosTable[4096];
extern short AtnTable[1024];	
extern double Ipower(double x,int n);
extern double Power(double x,double y);
extern double Llog(double x);
extern double Lexp(double x);
extern double Lldexp(double x,int k);
extern double Ffrexp(double x,int *exp);
extern void SwapUint(uint *a,uint *b);


extern void SetPalette(int palette_number);
extern void SetWord(int x,int y,char *printText);
extern void SetWord2(int x,int y,char *printText);
extern void SetWord2A(int x,int y,char *printText,int interval,float mulx,float muly,int type);
extern void SetWord2AP(int x,int y,char *printText,int interval,float mulx,float muly);
extern void SetWord3(int x,int y,char *printText,int interval,float mulx,float muly,int type);
extern void SetWord3A(int x,int y,char *printText,int interval,float mulx,float muly);
extern void SetWord3AC(int x,int y,char *printText,int interval,float mulx,float muly);
extern void SetWord3AP(int x,int y,char *printText,int interval,float mulx,float muly);
extern void SetWord3ACP(int x,int y,char *printText,int interval,float mulx,float muly);
extern void SetWord4(int x,int y,char *printText,int interval,float mulx,float muly,int type);
extern void SetWord4A(int x,int y,char *printText,int interval,float mulx,float muly);
extern void SetWord4AP(int x,int y,char *printText,int interval,float mulx,float muly);

extern void KawanoDrawFinal(void);

extern void KWSetViewportFull(); //0x80057C60
extern void KWReturnViewport(); //0x80057CE4

/* printNumber / printStringBinary / printStringHex: render_objects.h */
extern void printString(int xPosition, int yPosition, char *printText);
extern void printStringNumber(int xPosition, int yPosition, char *printText, int printValue);
extern void printStringUnsignedNumber(int xPosition, int yPosition, char *printText, uint printValue); //0x80057814
extern void printStringUnsignedHex(int xPosition, int yPosition, char *printText, uint printValue); //0x800578B0
extern void printStringUnsignedBinary(int xPosition, int yPosition, char *printText, uint printValue); //0x80057960
/* KWDisplay2D / KWDisplay2DAfter: code_80057C60.h */
extern void SetFadeOutB();
extern void SetObjBlock(int kind, int x, int y, char pri);
extern void InitObjBlock();
extern void DoObjBlock(int pri_flag);
extern void DispObjBlock(void* Target);

/* MSelController / PSelController / GSelController / TitleController: menus.h */

extern void DOBPSelTurnIn(ObjBlock Target); //0x800AAB90
extern void DOBPSelTurnOut(ObjBlock Target); //0x800AAA9C

/* ScreenViewAngle → gCameraZoom in StockAliases.h */

extern void BumpRoutin(Vector bu,float dist ,Vector velo ,float co);
extern long OoBCheck(ushort pointer);


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

extern void doGameSelect10();
extern void SetLakitu(void *Car);
extern void LakituCheck(void *Car,char PlayerID);
extern void HangLakitu(void *Car,char PlayerID,char place);
extern long LakituIceBehavior;

extern void OGAContTrgChk(int CameraNumber);


extern void SlipCheck(Player *car,char kno);
extern void AddGravity(Player *car);
extern void ProStickAngle(Player *car, Controller *cont, char number);


/* KWFlash8 / KW*GFTimer / KW*GFCount: code_80057C60.h */

extern Vtx_t Vtx_KTile8x16[];
extern Vtx_t Vtx_KTile16x16[];
extern Vtx_t Vtx_KTile16x16XFlip[];
extern Vtx_t Vtx_KTile40x32[];
/* Vtx_KTile32x32* → D_0D005AE0 / D_0D005B20 in StockAliases.h */
extern Vtx_t Vtx_KTile64x64BL[];
extern Vtx_t Vtx_KTile64x64XFlipBL[];
extern Vtx_t Vtx_KTile64x96BL[];
extern Vtx_t Vtx_KTile96x16[];

/* KWTexture*_AAZBBL_Init → D_0D0079C8 / D_0D007B00 in StockAliases.h */


extern float CheckWaterLevel(void *Car);
extern void CheckSplash(void *Car,int PlayerIndex);

extern long CheckSplashJAL1;
extern long CheckSplashJAL2;
extern long CheckSplashJAL3;

extern void LoadKeyStatus();
extern long CheckFinalLapFanfareJAL;
extern long CheckPlayStarBGMJAL;

extern void KWKumo_Alloc(void);
extern void KWChart_Kumo(int screen_num);
extern long CloudTypeMapCheck1;
extern long CloudTypeMapCheck2;
extern long CloudAmountMapCheck1;
extern long CloudAmountMapCheck2;
extern long Snow3DAllocMapCheck1;
extern long Snow3DAllocMapCheck2;
extern long Snow3DDisplayAfterMapCheck1;
extern long Snow3DDisplayAfterMapCheck2;
extern void KWDisplayJugemu(int Player);


extern void DMA_Base729A30(int input, int length, long output);
extern void TexBuffLoadP(void *texlist_ptr,int nocheck_flg);
extern void GrayScaleTexBuf3(uint num, uint step);
extern void GrayScaleTexBufRGB(uint num, int size, int r, int g, int b);
extern void FadeMain();
extern void FadeMain2(int i);
extern void SetFadeOut(int Fade);
extern short PutPylon(Vector pos,short number);
extern short PutObject(Vector pos,int category);

extern char g_FadingFlag[5];
extern int g_FadeCounter[5];
extern int g_FadeCounter2[5];

extern void DisplayBackground(Vtx_t *bg_vertex,Screen *screen,short screen_width,short screen_hight,float *screen_view_angle);

extern ushort KWLookCamera(float x,float z,Camera *camera);
extern ushort KWLookCameraPitch(float y,float z,Camera *camera);

extern void KWRectangle(int sx,int sy,int sizex,int sizey,int s ,int t,int mode);
/* KWTexture2D* / KWTexture3DRGBA32AAZBBL: render_objects.h */

extern void KWSpriteTile32B(short cx,short cy,uchar *addr,uint sizex,uint sizey);
extern void KWDisplayTotalTime(int Player);
/* KWPrintLapTimeXLU: render_objects.h */
extern void DrawLineHorizontal(short tx,short ty,short length,ushort r,ushort g,ushort b,ushort a);
extern void DrawLineVertical(short tx,short ty,short length,ushort r,ushort g,ushort b,ushort a);
extern void KWLoadTextureBlockI4b(uchar *texaddr,int cutx,int cuty);
/* KWLoadTextureBlockRGBA16B: render_objects.h */
/* SPRDrawClip → SprDrawClip in StockAliases.h */
extern void SprDrawClipST(int sx,int sy,int sizex,int sizey,int ss,int tt,int mode);
/* StockNumberSprites → common_texture_hud_normal_digit in StockAliases.h */

/* KWTexture*_Sub*: render_objects.h */
extern void	KWTextureRGBMA_SubBL(ushort *texaddr,uchar *alphaaddr,Vtx_t *vtxaddr,int sizex,int sizey,int cuty); // Multi Bit RGBA
extern void	KWTextureIA4_Sub(uchar *texaddr,Vtx_t *vtxaddr,int sizex,int sizey,int cutx,int cuty); //IA4
extern void	KWTextureA4_Sub(uchar *texaddr,Vtx_t *vtxaddr,int sizex,int sizey,int cutx,int cuty); //A4

extern void	KWCreateAffine3D(Vector postion,USVector angle,float scale);
extern void	KWCreateAffine3D_IceKage(Vector postion,USVector angle,float scale);
extern void	KWCreateAffine3D_Board(Vector position,Vector camera,float scale);
extern void	KWCreateAffine3D_Hole(Vector position,Vector direction,float scale);
extern void	KWCreateAffine3D_Anm(int num);

/* KWSetPrimColor / KWSetEnvColor / Sub*Mode / *Mode (except ToumeIMode / Color2Mode): render_objects.h */
extern void ToumeIMode(uint r,uint g,uint b,uint a);
extern void Color2Mode(uint prim_r,uint prim_g,uint prim_b,uint env_r,uint env_g,uint env_b,uint a);

extern void BumpObject(Object* InputObject);

extern void initializePlayer(int playerStructure, int characterID, float deltaX, float deltaZ, int characterID2, int unknown0xB000);
extern void DrawLocalSkeletonShape(Hierarchy* Skeleton,AnimePtr* Anime,short AnimeNumber,short CurrentFrame);

extern long asm_itemJump1A;// 0x8007B084  //3C058016
extern long asm_itemJump1B;// 0x8007B098  //84A543BA

extern long asm_itemJump2A;// 0x8007AFC0  //3C058016
extern long asm_itemJump2B;// 0x8007AFD4  //84A543BA

/* Sqrtf → sqrtf in StockAliases.h */

extern void colorFont(int color);
extern void loadFont();

extern long g_SegmentA;
extern long gIntMesgQueue;
//
//


/* g_resetToggle → gGamestate in StockAliases.h */
extern int g_DispTimeFlag;
extern long g_SequenceMode;
extern long g_NextSequenceMode;
extern long g_gameType;
extern short g_courseID;
extern short g_DebugBars;

extern uint PathTable[21][4];
extern uint PathTableB[21][4];

extern ushort PathLengthTable[21][8];

/* KartVtx: render_player.h */


extern long antialiasToggle;
extern long antialiasToggleB;


/* g_CupArray / g_cup*Array* → gCupCourseOrder in StockAliases.h */

extern uint64 g_DebugTextPalette;

extern long g_bannerTexture;
extern long g_previewTexture;
/* g_mapTexture / g_mapX / g_mapY / g_mapR / g_mapG / g_mapB / g_mapScale / g_startX / g_startY / g_mapWidth / g_mapHeight: code_80057C60.h */
extern long g_mapStartToggle;
extern short g_map2X; //0x8018D2C0
extern short g_map2Y; //0x8018D2D8


extern long g_DEBUG;

extern TexDataTable g_CoursePreviewOffsets[]; // 0x80199540
extern long r_CoursePreviewOffsets; // 0x12C750
extern long g_CourseBannerOffsets;
extern long g_BattleBannerOffsets;
extern long g_BattlePreviewOffsets; // 0x80199540
extern long g_cup0preview0; // 0x80199540
extern long g_cup0preview1; // 0x80199568
extern long g_cup0preview2; // 0x801994F0
extern long g_cup0preview3; // 0x801995B8


extern long g_cup1preview0; // 0x80199590
extern long g_cup1preview1; // 0x801994C8
extern long g_cup1preview2; // 0x80199428
extern long g_cup1preview3; // 0x80199400


extern long g_cup2preview0; // 0x80199630
extern long g_cup2preview1; // 0x801995E0
extern long g_cup2preview2; // 0x80199518
extern long g_cup2preview3; // 0x80199450


extern long g_cup3preview0; // 0x801996D0
extern long g_cup3preview1; // 0x801994A0
extern long g_cup3preview2; // 0x80199478
extern long g_cup3preview3; // 0x80199608

extern long 	g_NintendoLogoOffset; //0x8019F88C
extern int	CheckContPackMenu();
extern long 	g_NintendoLogoBorder;

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
extern void GUPerspective(Mtx *m, u16 *perspNorm, float fovy, float aspect, float near, float far, float scale);
extern float g_farClip;

extern short g_player1ScreenWidth; //0x8015F4AC
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


extern float g_TrialTime;
extern float g_lap2Time;
extern float g_lap3Time;


extern short g_progressValue;

extern long g_CourseObstacle; //0x8016359C

extern long g_PlayerRankTable[8];
extern long g_playerPosition1;// 801643B8
extern long g_playerPosition2;//, 801643BC
extern long g_playerPosition3;//, 801643C0
extern long g_playerPosition4;//, 801643C4
extern long g_playerPosition5;//, 801643C8
extern long g_playerPosition6;//, 801643CC
extern long g_playerPosition7;//, 801643D0
extern long g_playerPosition8;//, 801643D4

/* gravity_1: kart_attributes.h (f32[8]) */
extern float gravity_2;
extern float gravity_3;
extern float gravity_4;
extern float gravity_5;
extern float gravity_6;
extern float gravity_7;
extern float gravity_8;

extern BalloonColor  BalloonColorArray[8]; //0x800E4934
extern BalloonColor  BalloonAdjustArray[8]; //0x800E4954
extern BalloonColor  BalloonColorArrayB[8]; //0x800E4974
extern BalloonColor  BalloonAdjustArrayB[8]; //0x800E4994

extern short surface_p0;

/* g_GameLapTable / g_gameLapPlayer* → gLapCountByPlayerId in StockAliases.h */

extern void KWAnmNext(int num); //0x80086FD4

void KWDisplayEvent(int player);
/* KWDisplayEvent_After → render_snowing_effect in StockAliases.h */
void KWGameEventCommon_VF();
void KWGameEventCommon();

void KWDisplayIceBlock(int player);
void KWDisplayIceBlockShadow(int player);
void KWDisplayBombKartBT(int player);


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

/* g_hudToggleFlag / g_hudToggleFlagP2 / g_KWDBDispSW / g_hudMapToggle2 / g_hudSpeedToggle / g_hudSpeedToggle2 / g_hudLapToggle / g_blueLineRankToggle / g_KWLapSW / g_KWDemoSW / g_hudCharpicRankY1: code_80057C60.h */
extern short g_hudMapToggle; // 0x80165800
/* RadarOn → D_80165800 in StockAliases.h */

//hud all players
extern char g_hudToggle; // 0x800DC5B9 
extern char g_mapPlayers; // 0x8018D15B
extern int g_KWScreenEnable;

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
extern void KWDisplayAfter4PSub(int Player);
extern void KWDisplay2D2PLeftAfter();

extern void KWVideoFramesYori();

extern short g_GhostHUDID; //0x8018DAAA

extern long g_mlogoY;// 0x8018D9F0
extern long g_mflagID;// 0x8018DA30
extern long g_mpressstartID;// 0x8018DA58
extern long g_mracewayTime;// 0x8018DA80

/* KBGNumberNext / menuScreenA/B/C / titleDemo / g_cupSelect / g_courseSelect / g_InGameTT / g_InGame: menus.h */

extern void InitializeEndingSequence();
extern void EndingSequence();
extern void CheckFinish();
extern short asm_CupCount;

extern void ResultsSequence();

extern short songID; //
extern long asm_SongA;// 0x8028EC9C
extern long asm_SongB;// 0x8028F9C4

extern uint CullDL_Parameters;
extern long g_courseTable;
extern uint KeystockBuffer;
extern uint KeystockCounter;
extern SOUKOU_ZURE_TYPE LaneData[8];
extern float g_RoadWidth[21]; // MR default: 50.0f

extern void AreaBunkatuBP(int pathID);
extern void SidePointCalcBP(int pathID);
extern void CurveDataCalcBP(int pathID);	
extern void AngleDataCalcBP(int pathID);
extern void ShortcutDataCalcBP(int pathID);


extern void Func_8001A588(int arg0, void *arg1, short *arg2, char arg3, int arg4);

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

extern void ActionStartCheck(int num, Player *kart);
extern ActionData *ActPointPtr;
extern ActionData *ActData_Pointer;
extern ushort ActionDataNumCustom[8];
extern short g_driftFlg[10];
extern ushort g_actionFlg[10];


/* PlayerOK → gCharacterGridIsSelected in StockAliases.h */
extern char player1OK; //
extern char player2OK; //
extern char player3OK; //
extern char player4OK; //

extern void textDrawPtr(int *x, int *y, const char *str, int spacing, float xScale, float yScale);
extern void textDraw(int x, int y, const char *str, int spacing, float xScale, float yScale);
extern void DrawText(int x, int y, const char *str, int spacing, float xScale, float yScale);
extern void SetFadeOutTaData();

extern int GetWordLength(const char *str);

/* g_CharacterSelections / g_player1Character: menus.h */
extern char g_player2Character;
extern char g_player3Character;
extern char g_player4Character;

extern int g_GFXCounter;
extern int g_PKCounter;
extern unsigned short g_RNG;
extern void ExecuteItem(Player* PlayerID);

/* g_menuMultiplayerSelection: menus.h */

extern char g_placedBadGPadvance;
extern short g_P1TopSpeed;

extern short BalloonCount[4];

extern char g_menuUpdateTimer1;
extern char g_menuUpdateTimer2;
extern char g_menuUpdateTimer3;
extern char g_menuUpdateTimer4;

/* g_menuPreviewValue* / g_BattlePreviewValue* → gMenuItems[].work1 LSB in StockAliases.h */

/* g_zoomFOV* → gCameraZoom; g_zoomLevel* → kotei_gakaku_mode in StockAliases.h */

extern char g_ReplayFlag;
/* g_screenViewAngle → gCameraZoom in StockAliases.h */

extern char g_sfxPause;

extern long g_RawAudio; //0x803B9260

extern AudioTablePointers g_MUSTablePointer;

extern SequenceTable g_MUSSequenceTable;
extern InstrumentTable g_MUSInstrumentTable;
extern RawAudioTable g_MUSRawAudioTable;
extern BankMapTable g_MUSBankMapTable;
extern SFXTempPointerStruct g_sfxPointer;

extern short g_surfaceCheckP1;
/* g_PlayerSurfaceSoundID: audio/external.h */

/* g_gamePausedFlag comes from audio/external.h */
extern uint64 osClockRate;

/* MakePos / MakeStartup / MakeRDP / InitRndSmoke / InitRapidSmoke / InitSpinSmoke: code_80057C60.h */
extern void MakeRandomRDP(Smoke *data,int col,short A);

/* MakeBodyColor / MakeBodyColorAdjust: render_player.h */

extern void RunKart(void* Car, void* Camera, int place, int playerID); // 8002D268
extern void RunKartSimple(void* Car, void* Camera, int place, int playerID); // 8002F35C
/* g_playerEcho → D_800E9F7C[].unk_14 in StockAliases.h */

/* g_StarUseCounter: render_player.h */
extern long g_GhostUseCounter[8]; // 8018D950
extern long g_GhostUseTimer[8]; // 8018D970
/* ActionData_Pointer → gCoursesCPUBehaviour in StockAliases.h */
extern short g_noSimpleKartFlag[8]; // 801633F8
/* g_charRadiusTbl is in kart_attributes.h */

extern long g_StringTableCourseGP[20]; // 800E7524
extern long g_StringTableCourse[20]; // 800E7574

extern PlayerTextureTable TBSeq_psel; //0x800E8340
extern PlayerTextureTable TBSeq_psel_return; //0x800E8340
extern PlayerTextureTable TBSeq_psel_p0; //0x800E8340
extern PlayerTextureTable TBSeq_psel_p1; //0x800E8340
extern PlayerTextureTable TBSeq_psel_p2; //0x800E8340
extern PlayerTextureTable TBSeq_psel_p3; //0x800E8340
extern PlayerTextureTable TBSeq_psel_p4; //0x800E8340
extern PlayerTextureTable TBSeq_psel_p5; //0x800E8340
extern PlayerTextureTable TBSeq_psel_p6; //0x800E8340

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
/* g_cloudsToggle: code_80057C60.h */
extern char g_WinKart;

extern int g_BombTable;

extern long g_skyColorTopTable;
extern long g_skyColorBotTable;

extern long g_SnowParticleTex[36];
extern long *g_MRCloudTexPtr; // Set of four I4 cloud images; //0x400 length each.

extern float g_skySnowScale;
extern float g_skySnowVelocity;
extern long g_skySnowSpawnHeight;
extern long g_skySnowSpawnRadiusDensity;
extern long g_skySnowSpawnCenterOffset;
extern long g_skySnowHitGoal;

extern long g_3DSnowSpawnHeight;
extern long g_3DSnowSpawnDistanceMin;
extern long g_3DSnowSpawnDistanceMax;
extern long g_3DSnowSpawnCone;
extern long g_3DSnowSpawnRadius;
extern long g_3DSnowSwayVelocity;
extern long g_3DSnowSwayDistance;
extern float g_3DSnowSwayMovement;
extern float g_3DSnowScale;
extern float g_3DSnowVelocityUpLim;
extern float g_3DSnowVelocityLowLim;

extern void KWChartSnow(void);
extern void KWChartIceBlock(void);

//racer values
/* g_timeLapChange → gTimePlayerLastTouchedFinishLine in StockAliases.h */
/* g_playerPathPointTable / g_PathPointPlayer* → gNearestPathPointByPlayerId */
extern short g_playerPathPointCopy[8]; // 80165320
extern short g_rivalOvertakeAllowFlag[10];
extern short g_EnemyTargetPlayer;
extern short g_rankUpdateFinishFlag;

//fog
/* g_fogToggleBanshee / g_fogR/G/B → g_skyToggle / fog_red/green/blue in StockAliases.h */
/* KBGNumber / KBGChange: menus.h */

//lightning flags
/* g_lightningFlag / g_lightningFlagPlayer1 come from audio/external.h */

//offroad flags
extern short g_offroadFlagPlayer1; // 0x80165330 
extern short g_offroadFlagPlayer2; // 0x80165332 
extern short g_offroadFlagPlayer3; // 0x80165334 
extern short g_offroadFlagPlayer4; // 0x80165336 

//waterlevel checks
extern float g_waterlevelPlayer[8]; // 0x801652A0

//wrongway flags 
extern short g_wrongwayFlagPlayer1; // 0x80163270 
extern short g_wrongwayFlagPlayer2; // 0x80163272 
extern short g_wrongwayFlagPlayer3; // 0x80163274 
extern short g_wrongwayFlagPlayer4; // 0x80163276 

//player shadow flags
extern char g_ShadowflagPlayer0; //0x800F6B87
extern char g_ShadowflagPlayer1; //0x800F795F
extern char g_ShadowflagPlayer2;
extern char g_ShadowflagPlayer3;

//GP points
extern uchar g_playerGPpoints[8]; //name to num: Mario, Luigi, Yoshi, Toad, D.K., Wario, Peach, Bowser
extern void EtcEnemyDrive();
extern void CheckLapCount(int playerID, Player *car);
extern void WrapPathIndexAtFinish(float posX, float posY, float posZ, short *wayPointIndex, int pathIndex);
extern short CalcOGAAreaSubBP(float mx, float my, float mz, ushort t_group, int *b_num_ptr);
extern void InitCenterLine();
extern uint OSMemSize;


//multiplayer points
extern uchar g_2PRacePoints[2];
extern uchar g_3PRacePoints[3];
extern uchar g_4PRacePoints[4];
extern uchar g_2PBattlePoints[2];
extern uchar g_3PBattlePoints[3];
extern uchar g_4PBattlePoints[4];


//course
extern short g_monitorCounter;

//player color timers
/* g_colorPlayer0R is u16[8] in render_player.c; 1R–7R are StockAliases.h */

//sound and music
extern void playSound(int soundID);
extern void NAMusicVolume(unsigned char volume); //0-Mute 75-Half 127 Full
/* NAPlyTrgStart / NAPlyVoiceStart / NAEnmTrgStart / NaSeqStart / NaPlyLevel* / NaSceneLevel*: audio/external.h */
extern void NAEnmTrgStop(Vector ObjectPosition, int soundID);
extern void playMusic(int MusicID);
extern void playMusic2(int musicID);
extern short BattleSongID;


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
extern ushort g_musicTempo;
extern ushort g_fanfareTempo;
extern SeqPlayerStruct SeqPlayer[4];

extern void NAISeqFlagEntry(uint entry);
extern ushort NAIGetPlayingSeqFlag(uchar seq);
/* NAIFxFlagEntry: audio/external.h */

//Cave Fire Particle Stuff
extern void KWGetCaveFire(int objnum);
extern int CaveFireColCheck;
extern int KWChaseSVal(short *var,short val,short step);
extern int KWChaseIVal(int   *var,int   val,int   step);
extern int KWChaseFVal(float *var,float val,float step);
extern void KW2DMatrixInit(void);
/* KWSet2Color → set_color_render in StockAliases.h */
/* SprDrawSubCI8 / KWDisplayFireParticleSub: render_objects.h */
extern void KWDisplayRank(int Player);

extern short KWRank[8];
extern void KWGetStar(Vector position,int type);
extern void KWChartStar(void);
extern void KWDisplayStar(int player);

extern ushort RGBAFallingLeaf[];
extern ushort RGBAQuestionMark[];
extern void GoToGameSelect();
extern void DisplayMap2();
extern void SearchList(uint addr);
extern void SearchList2(uint addr, char Surface);
extern void SearchListFile(uint addr);
extern void MakeCollision();
extern ushort RGBALeaf[];
extern void InitialMap(void);
extern void InitialMapObject(void);
extern void SetTreeObject(uint TargetAddress);  // 0x8029D584
extern void SetPakkunObject(uint TargetAddress);  // 0x8029CC14
extern void SetItemBoxObject(uint TargetAddress);  // 0x8029D830
/* LoadMapData → load_course in StockAliases.h */
extern int FallingRockGFX_U; //default 0x3C0F0600
extern int FallingRockGFX_L; //default 0x25EF6FE0
extern int FallingRockShadowGFX_U; //default 0x3C0C0600
extern int FallingRockShadowGFX_L; //default 0x258C6F88
extern void DisplayGroupmap(int Address, Screen* PlayerScreen);
extern int ShadowModel;
extern int HoleModel;
extern int ItemBoxModel;
extern void MoveFallingRock(Object *obj);

extern int CheckTriangleZX(Bump *bump, float radius,float p1x,float p1y, float p1z, ushort pointer);
extern int CheckTriangleXY(Bump *bump, float radius,float p1x,float p1y, float p1z, ushort pointer);
extern int CheckTriangleYZ(Bump *bump, float radius,float p1x,float p1y, float p1z, ushort pointer);
extern int CheckTriangleZX_V(Bump *bump,float radius,float p1x,float p1y, float p1z, ushort pointer ,float lastx,float lasty,float lastz);
extern int CheckTriangleXY_V(Bump *bump,float radius,float p1x, float p1y, float p1z ,ushort pointer ,float lastx,float lasty,float lastz);
extern int CheckTriangleYZ_V(Bump *bump,float radius,float p1x ,float p1y, float p1z, ushort pointer ,float lastx,float lasty,float lastz);

extern void SpinKart(Player* Car, Camera* camera, char place, char kno);
extern void WeaponStatus(Player* Car, char kno, char place);
extern void SetStrategy(Player* Car, char kno, char place);
extern void CheckKartHit(Player* Car, char kno, char place);
extern float PowerCheck(Player* Car, char kno);
extern void DriftJump(Player* Car);
extern void CheckWall(Player* Car, char kno, Vector velocity);
extern void AccelOn(Player* Car);
extern void AccelOff(Player* Car, float accelOff);
extern short CheckSlope(ushort pointer);
extern void SetRolloverFall(Player* Car, char kno);
extern void ResetRolloverFall(Player* Car, char kno);
extern float CalcHeight(float px, float py, float pz, ushort pointer);
extern void TirePosition(Player* Car, float new_x, float new_y, float new_z);
extern void EnemyTirePosition(Player* Car, float new_x, float new_y, float new_z);
extern void SetSlipAngle(Player* Car, char kno, float old_x, float old_z, float new_x, float new_z);
/* NaPlyLandStart / NaSeqFadeout: audio/external.h */

#ifndef MAIN_H
#define MAIN_H

// Message IDs
#define MESG_SP_COMPLETE 100
#define MESG_DP_COMPLETE 101
#define MESG_VI_VBLANK 102
#define MESG_START_GFX_SPTASK 103
#define MESG_NMI_REQUEST 104

#define MTX_HUD_POOL_SIZE 800

#ifdef AVOID_UB
#define MTX_HUD_POOL_SIZE_MAX MTX_HUD_POOL_SIZE
#else
#define MTX_HUD_POOL_SIZE_MAX MTX_HUD_POOL_SIZE - 50
#endif

#define MTX_OBJECT_POOL_SIZE 128

//! @todo Verify with proper documentation
// functions called by MTXShadow multiply by 8
// 8 Kart AI Players * 4 real players in coop
#define MTX_SHADOW_POOL_SIZE 8 * 4

//! @todo Verify with proper documentation
// functions called by MTXKart multiply by 8
// 8 Kart AI Players * 4 real players in coop
#define MTX_KART_POOL_SIZE 8 * 4

#define MTX_EFFECT_POOL_SIZE 660

// SetIAFont sets an OOB Pool Size max check (760)
#ifdef AVOID_UB
#define MTX_EFFECT_POOL_SIZE_MAX MTX_EFFECT_POOL_SIZE
#else
#define MTX_EFFECT_POOL_SIZE_MAX MTX_EFFECT_POOL_SIZE + 100
#endif

#define GFX_POOL_SIZE 7500

struct GfxPool {
    /* 0x00000 */ Mtx MTXScreen;                       // Matrix for skybox and startup logo
    /* 0x00040 */ Mtx MTXPersp[4];                     // Matrix for perspective screen Modes
    /* 0x00140 */ Mtx MTXOrtho;                        // Matrix for ortho hud screen Modes
    /* 0x00180 */ Mtx MTXUnk;                          // Matrix Unused
    /* 0x001C0 */ Mtx MTXLookAt[4];                    // Matrix for lookat screen Modes
    /* 0x002C0 */ Mtx MTXHud[MTX_HUD_POOL_SIZE];       // Matrix hud elements and 2D related effects
    /* 0x0CAC0 */ Mtx MTXObject[MTX_OBJECT_POOL_SIZE]; // Matrix course objects
    /* 0x0EAC0 */ Mtx MTXShadow[MTX_SHADOW_POOL_SIZE]; // Matrix shadow characters
    /* 0x0F2C0 */ Mtx MTXKart[MTX_KART_POOL_SIZE];     // Matrix Kart characters
    /* 0x0FAC0 */ Mtx MTXEffect[MTX_EFFECT_POOL_SIZE]; // Matrix misc effects
    /* 0x19FC0 */ Mtx MTXArr[4];                       // Matrix Unused array
    /* 0x1A0C0 */ Gfx GFXPool[GFX_POOL_SIZE];
    /* 0x28B20 */ struct SPTask SPTask;
}; // Size = 0x28B70

typedef struct {
    u16 Triangle; // Index for CollisionTable which has indexes for FaceBuffer
    u16 NumTriangles;
} CollisionGrid;

void SysCreateThread(OSThread*, OSId, void (*entry)(void*), void*, void*, OSPri);
void MainFunc(void);
void Thread1Idle(void*);
void SetupMessagequeue(void);
void StartRSPTask(s32);
void BuildGraphicsTask(void);
void InitControllers(void);
void CheckController(s32);
void ReadControllers(void);
void AllocDepthBuffer(void);
void SetRSPTask(struct SPTask*);
void StartGraphicTask(struct SPTask*);
void BeginDrawing(void);
void EndDrawing(void);
void* SoftwareBlanking(s32);
void InitialGameframe(void);
void BeginGameframe(void);
void FlushDisplaylist(void);
void InitExcode(void);
void InitMkcode(void);
void InitOverkart(void);
void DMA(u8*, u8*, size_t);
void InitializeSystemWorks(void);
void BootSequence(void);
void RaceSequence(void);
void ExecuteSequence(void);
void YieldRSPTask(void);
void GetTaskMessage(void);
void SetClient(s32, struct VblankHandler*, OSMesgQueue*, OSMesg*);
void StartGFXTask(void);
void InterruptSequence(void);
void RDPDoneSequence(void);
void RSPDoneSequence(void);
void Mainproc(void*);
void InitialGameselectSequence(void);
void InitialKartselectSequence(void);
void InitialMapselectSequence(void);
void InitialTitleSequence(void);
void InitializeSequence(void);
void Gameproc(void*);
void Audioproc(void*);

extern struct VblankHandler* VblankHandler1;
extern struct VblankHandler* VblankHandler2;

extern struct SPTask* ActiveSPTask;
extern struct SPTask* SCurrentAudioSPTask;
extern struct SPTask* SCurrentDisplaySPTask;
extern struct SPTask* SNextAudioSPTask;
extern struct SPTask* SNextDisplaySPTask;

extern struct Controller Controllers[];
extern struct Controller* ControllerOne;
extern struct Controller* ControllerTwo;
extern struct Controller* ControllerThree;
extern struct Controller* ControllerFour;
extern struct Controller* ControllerFive;
extern struct Controller* ControllerSix;
extern struct Controller* ControllerSeven;
extern struct Controller* ControllerEight;

extern Player GamePlayers[];
extern Player* PlayerOne;
extern Player* PlayerTwo;
extern Player* PlayerThree;
extern Player* PlayerFour;
extern Player* PlayerFive;
extern Player* PlayerSix;
extern Player* PlayerSeven;
extern Player* PlayerEight;

extern Player* PlayerOneCopy;
extern Player* PlayerTwoCopy;

extern struct GfxPool GfxPools[];
extern struct GfxPool* DynamicP;

extern struct VblankHandler GameVblankHandler;
extern struct VblankHandler SSoundVblankHandler;
extern OSMesgQueue DmaMesgQueue, GameVblankQueue, GfxVblankQueue, UnusedGMsgQueue, IntrMesgQueue, gSPTaskMesgQueue;
extern OSMesgQueue SSoundMesgQueue;
extern OSMesg SSoundMesgBuf[1];
extern OSMesg DmaMesgBuf[1], GameMesgBuf;
extern OSMesg GfxMesgBuf[];
extern OSMesg IntrMesgBuf[16], gSPTaskMesgBuf[16];
extern OSMesg MainReceivedMesg;
extern OSIoMesg DmaIoMesg;
extern OSMesgQueue SIEventMesgQueue;
extern OSMesg SIEventMesgBuf[3];

extern OSContStatus ControllerStatuses[];

extern OSContPad ControllerPads[];
extern u8 ControllerBits;

extern CollisionGrid CollisionGridTable[];
extern u16 SimpleObjectCount;
extern u16 MatrixCount;
extern s32 Framerate;
extern f32 D_80150118;
extern u16 ResertType;
extern u16 TimeFlag;

extern s32 RaceClearFlag;
extern s32 NextSequenceMode;
extern f32 CameraZoom[];

extern f32 AspectRatio;

extern struct D_80150158 D80150158[];
extern uintptr_t SegmentTable[];
extern Gfx* DisplayListHead;
extern struct SPTask* GfxSPTask;
extern s32 D_801502A0;
extern s32 D_801502A4;
extern u16* CfbPtrs[];
extern uintptr_t PhysicalZBuffer;
extern Mat4 Affine;

extern s32 Padding[];

extern u16 CameraMode[];
extern u16 CameraPoint;

extern OSThread IdleThread;
extern u8 IdleThreadStack[];
extern OSThread VideoThread;
extern u8 VideoThreadStack[];
extern OSThread GameLoopThread;
extern u8 GameLoopThreadStack[];
extern OSThread AudioThread;
extern u8 AudioThreadStack[];

extern u8 GfxSPTaskYieldBuffer[];
extern u32 GfxSPTaskStack[];
extern OSMesg PIMesgBuf[];
extern OSMesgQueue PIMesgQueue;
void RaceSequence(void);
extern s32 GameState;
#ifndef gRaceState_AS_U16
// Prevent overlapping writes in gcc
// Whether StartingIndicator was intended to be a separate variable in Main.c from the rest of the game is Unknown
#ifdef GCC
extern u16 StartingIndicator;
#else
extern s32 StartingIndicator;
#endif
#endif

extern u16 DebugSection;
extern u16 DisplayFlag;
extern u16 DemoFlag;
extern u16 DebugMode;
extern s32 NewSequenceMode;
extern s32 ScreenSplitA;
extern s32 ScreenSplitB;
extern s32 PlayerCount;

extern s32 GameMode;
extern s32 CupLevel;
extern s32 CupNumber;
extern s32 RaceClass;
extern s32 FrameCounter;
extern u16 DispFrame;
extern u16 DrawFrame;
extern s32 D_800DC568;
extern s32 D_800DC56C[];
extern s16 Videoframe;
extern f32 Timer;
extern f32 GameTimer;

// end of definition of Main.c variables

#endif

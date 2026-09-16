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
// functions called by mtxShadow multiply by 8
// 8 Kart AI Players * 4 real players in coop
#define MTX_SHADOW_POOL_SIZE 8 * 4

//! @todo Verify with proper documentation
// functions called by mtxKart multiply by 8
// 8 Kart AI Players * 4 real players in coop
#define MTX_KART_POOL_SIZE 8 * 4

#define MTX_EFFECT_POOL_SIZE 660

// Set_IAFont sets an OOB pool size max check (760)
#ifdef AVOID_UB
#define MTX_EFFECT_POOL_SIZE_MAX MTX_EFFECT_POOL_SIZE
#else
#define MTX_EFFECT_POOL_SIZE_MAX MTX_EFFECT_POOL_SIZE + 100
#endif

#define GFX_POOL_SIZE 7500

struct GfxPool {
    /* 0x00000 */ Mtx mtxScreen;                       // Matrix for skybox and startup logo
    /* 0x00040 */ Mtx mtxPersp[4];                     // Matrix for perspective screen modes
    /* 0x00140 */ Mtx mtxOrtho;                        // Matrix for ortho hud screen modes
    /* 0x00180 */ Mtx mtxUnk;                          // Matrix unused
    /* 0x001C0 */ Mtx mtxLookAt[4];                    // Matrix for lookat screen modes
    /* 0x002C0 */ Mtx mtxHud[MTX_HUD_POOL_SIZE];       // Matrix hud elements and 2D related effects
    /* 0x0CAC0 */ Mtx mtxObject[MTX_OBJECT_POOL_SIZE]; // Matrix course objects
    /* 0x0EAC0 */ Mtx mtxShadow[MTX_SHADOW_POOL_SIZE]; // Matrix shadow characters
    /* 0x0F2C0 */ Mtx mtxKart[MTX_KART_POOL_SIZE];     // Matrix kart characters
    /* 0x0FAC0 */ Mtx mtxEffect[MTX_EFFECT_POOL_SIZE]; // Matrix misc effects
    /* 0x19FC0 */ Mtx mtxArr[4];                       // Matrix unused array
    /* 0x1A0C0 */ Gfx gfxPool[GFX_POOL_SIZE];
    /* 0x28B20 */ struct SPTask spTask;
}; // size = 0x28B70

typedef struct {
    u16 triangle; // Index for collision_table which has indexes for gFaceBuffer
    u16 numTriangles;
} CollisionGrid;

void SysCreateThread(OSThread*, OSId, void (*entry)(void*), void*, void*, OSPri);
void main_func(void);
void thread1_idle(void*);
void setup_messagequeue(void);
void start_RSP_task(s32);
void build_graphics_task(void);
void InitControllers(void);
void check_controller(s32);
void readControllers(void);
void AllocDepthBuffer(void);
void set_RSP_task(struct SPTask*);
void start_graphic_task(struct SPTask*);
void begin_drawing(void);
void end_drawing(void);
void* SoftwareBlanking(s32);
void initial_gameframe(void);
void begin_gameframe(void);
void flush_displaylist(void);
void init_excode(void);
void init_mkcode(void);
void init_overkart(void);
void DMA(u8*, u8*, size_t);
void InitializeSystemWorks(void);
void boot_sequence(void);
void race_sequence(void);
void execute_sequence(void);
void yield_RSP_task(void);
void get_task_message(void);
void set_client(s32, struct VblankHandler*, OSMesgQueue*, OSMesg*);
void start_gfx_task(void);
void interrupt_sequence(void);
void rdp_done_sequence(void);
void rsp_done_sequence(void);
void mainproc(void*);
void initial_gameselect_sequence(void);
void initial_kartselect_sequence(void);
void initial_mapselect_sequence(void);
void initial_title_sequence(void);
void initialize_sequence(void);
void gameproc(void*);
void audioproc(void*);

extern struct VblankHandler* gVblankHandler1;
extern struct VblankHandler* gVblankHandler2;

extern struct SPTask* gActiveSPTask;
extern struct SPTask* sCurrentAudioSPTask;
extern struct SPTask* sCurrentDisplaySPTask;
extern struct SPTask* sNextAudioSPTask;
extern struct SPTask* sNextDisplaySPTask;

extern struct Controller gControllers[];
extern struct Controller* gControllerOne;
extern struct Controller* gControllerTwo;
extern struct Controller* gControllerThree;
extern struct Controller* gControllerFour;
extern struct Controller* gControllerFive;
extern struct Controller* gControllerSix;
extern struct Controller* gControllerSeven;
extern struct Controller* gControllerEight;

extern Player gPlayers[];
extern Player* gPlayerOne;
extern Player* gPlayerTwo;
extern Player* gPlayerThree;
extern Player* gPlayerFour;
extern Player* gPlayerFive;
extern Player* gPlayerSix;
extern Player* gPlayerSeven;
extern Player* gPlayerEight;

extern Player* gPlayerOneCopy;
extern Player* gPlayerTwoCopy;

extern struct GfxPool gGfxPools[];
extern struct GfxPool* gDynamicP;

extern struct VblankHandler gGameVblankHandler;
extern struct VblankHandler sSoundVblankHandler;
extern OSMesgQueue gDmaMesgQueue, gGameVblankQueue, gGfxVblankQueue, unused_gMsgQueue, gIntrMesgQueue, gSPTaskMesgQueue;
extern OSMesgQueue sSoundMesgQueue;
extern OSMesg sSoundMesgBuf[1];
extern OSMesg gDmaMesgBuf[1], gGameMesgBuf;
extern OSMesg gGfxMesgBuf[];
extern OSMesg gIntrMesgBuf[16], gSPTaskMesgBuf[16];
extern OSMesg gMainReceivedMesg;
extern OSIoMesg gDmaIoMesg;
extern OSMesgQueue gSIEventMesgQueue;
extern OSMesg gSIEventMesgBuf[3];

extern OSContStatus gControllerStatuses[];

extern OSContPad gControllerPads[];
extern u8 gControllerBits;

extern CollisionGrid gCollisionGrid[];
extern u16 g_simpleObjectCount;
extern u16 gMatrixCount;
extern s32 framerate;
extern f32 D_80150118;
extern u16 resert_type;
extern u16 time_flag;

extern s32 race_clear_flag;
extern s32 next_sequence_mode;
extern f32 gCameraZoom[];

extern f32 g_aspectRatio;

extern struct D_80150158 gD_80150158[];
extern uintptr_t SegmentTable[];
extern Gfx* gDisplayListHead;
extern struct SPTask* gGfxSPTask;
extern s32 D_801502A0;
extern s32 D_801502A4;
extern u16* g_CfbPtrs[];
extern uintptr_t gPhysicalZBuffer;
extern Mat4 gAffine;

extern s32 padding[];

extern u16 camera_mode[];
extern u16 camera_point;

extern OSThread gIdleThread;
extern u8 gIdleThreadStack[];
extern OSThread gVideoThread;
extern u8 gVideoThreadStack[];
extern OSThread gGameLoopThread;
extern u8 gGameLoopThreadStack[];
extern OSThread gAudioThread;
extern u8 gAudioThreadStack[];

extern u8 gGfxSPTaskYieldBuffer[];
extern u32 gGfxSPTaskStack[];
extern OSMesg gPIMesgBuf[];
extern OSMesgQueue gPIMesgQueue;
void race_sequence(void);
extern s32 gGamestate;
#ifndef gRaceState_AS_U16
// Prevent overlapping writes in gcc
// Whether g_startingIndicator was intended to be a separate variable in main.c from the rest of the game is unknown
#ifdef GCC
extern u16 g_startingIndicator;
#else
extern s32 g_startingIndicator;
#endif
#endif

extern u16 g_DebugSection;
extern u16 g_DisplayFlag;
extern u16 g_DemoFlag;
extern u16 g_DebugMode;
extern s32 g_NewSequenceMode;
extern s32 g_ScreenSplitA;
extern s32 g_ScreenSplitB;
extern s32 g_playerCount;

extern s32 g_gameMode;
extern s32 cup_level;
extern s32 cup_number;
extern s32 g_raceClass;
extern s32 frame_counter;
extern u16 g_DispFrame;
extern u16 draw_frame;
extern s32 D_800DC568;
extern s32 D_800DC56C[];
extern s16 videoframe;
extern f32 timer;
extern f32 g_gameTimer;

// end of definition of main.c variables

#endif

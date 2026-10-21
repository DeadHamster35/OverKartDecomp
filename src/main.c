#ifndef GCC
#define gRaceState_AS_U16
#endif
#include <ultra64.h>
#include <PR/os.h>
#include <PR/ucode.h>
#include <macros.h>
#include <decode.h>
#include <mk64.h>
#include <course.h>

#include "profiler.h"
#include "main.h"
#include "racing/memory.h"
#include "menus.h"
#include <segments.h>
#include <common_structs.h>
#include <defines.h>
#include "buffers.h"
#include "camera.h"
#include "profiler.h"
#include "race_logic.h"
#include "skybox_and_splitscreen.h"
#include "render_objects.h"
#include "effects.h"
#include "code_80281780.h"
#include "audio/external.h"
#include "code_800029B0.h"
#include "code_80280000.h"
#include "podium_ceremony_actors.h"
#include "menu_items.h"
#include "code_80057C60.h"
#include "profiler.h"
#include "player_controller.h"
#include "render_player.h"
#include "render_courses.h"
#include "actors.h"
#include "replays.h"
#include <debug.h>
#include "crash_screen.h"
#include "buffers/gfx_output_buffer.h"
#include "OverKart5Hooks.h"
#include "save.h"

void InitMKCode(void);
void NasInitAudio(void);
void CreateDebugThread(void);
void StartDebugThread(void);
struct SPTask* NasAudioMain(void);

struct VblankHandler* VblankHandler1 = NULL;
struct VblankHandler* VblankHandler2 = NULL;

struct SPTask* ActiveSPTask = NULL;
struct SPTask* SCurrentAudioSPTask = NULL;
struct SPTask* SCurrentDisplaySPTask = NULL;
struct SPTask* SNextAudioSPTask = NULL;
struct SPTask* SNextDisplaySPTask = NULL;

struct Controller Controllers[NUM_PLAYERS];
struct Controller* ControllerOne = &Controllers[0];
struct Controller* ControllerTwo = &Controllers[1];
struct Controller* ControllerThree = &Controllers[2];
struct Controller* ControllerFour = &Controllers[3];
struct Controller* ControllerFive = &Controllers[4]; // All physical controllers combined.`
struct Controller* ControllerSix = &Controllers[5];
struct Controller* ControllerSeven = &Controllers[6];
struct Controller* ControllerEight = &Controllers[7];

Player GamePlayers[NUM_PLAYERS];
Player* PlayerOne = &GamePlayers[0];
Player* PlayerTwo = &GamePlayers[1];
Player* PlayerThree = &GamePlayers[2];
Player* PlayerFour = &GamePlayers[3];
Player* PlayerFive = &GamePlayers[4];
Player* PlayerSix = &GamePlayers[5];
Player* PlayerSeven = &GamePlayers[6];
Player* PlayerEight = &GamePlayers[7];

Player* PlayerOneCopy = &GamePlayers[0];
Player* PlayerTwoCopy = &GamePlayers[1];
UNUSED Player* PlayerThreeCopy = &GamePlayers[2];
UNUSED Player* PlayerFourCopy = &GamePlayers[3];

UNUSED s32 D_800FD850[3];
struct GfxPool GfxPools[2];
struct GfxPool* DynamicP;

UNUSED s32 GFXPoolPadding; // is this necessary?
struct VblankHandler GameVblankHandler;
struct VblankHandler SSoundVblankHandler;
OSMesgQueue DmaMesgQueue, GameVblankQueue, GfxVblankQueue, UnusedGMsgQueue, IntrMesgQueue, gSPTaskMesgQueue;
OSMesgQueue SSoundMesgQueue;
OSMesg SSoundMesgBuf[1];
OSMesg DmaMesgBuf[1], GameMesgBuf;
OSMesg GfxMesgBuf[1];
UNUSED OSMesg D_8014F010, D_8014F014;
OSMesg IntrMesgBuf[16], gSPTaskMesgBuf[16];
OSMesg MainReceivedMesg;
OSIoMesg DmaIoMesg;
OSMesgQueue SIEventMesgQueue;
OSMesg SIEventMesgBuf[3];

OSContStatus ControllerStatuses[4];
OSContPad ControllerPads[4];
u8 ControllerBits;
// Contains a 32x32 grid of indices into CollisionTable containing indices into FaceBuffer
CollisionGrid CollisionGridTable[1024];
u16 SimpleObjectCount;
u16 MatrixCount;
s32 Framerate;
f32 D_80150118;

u16 ResertType;
u16 TimeFlag;

s32 RaceClearFlag;
s32 NextSequenceMode;
UNUSED s32 D_80150128;
UNUSED s32 D_8015012C;
f32 CameraZoom[4]; // look like to be the fov of each character
UNUSED s32 D_80150140;
UNUSED s32 D_80150144;
f32 AspectRatio;
f32 FarClip;
f32 ScreenNearPoint;
UNUSED f32 D_80150154;

struct D_80150158 D80150158[16];
uintptr_t SegmentTable[16];
Gfx* DisplayListHead;

struct SPTask* GfxSPTask;
s32 D_801502A0;
s32 D_801502A4;
u16* CfbPtrs[3];
uintptr_t PhysicalZBuffer;
UNUSED u32 D_801502B8;
UNUSED u32 D_801502BC;
Mat4 Affine;

s32 Padding[2048];

u16 CameraMode[4];
u16 CameraPoint;

UNUSED OSThread PaddingThread;
OSThread IdleThread;
ALIGNED8 u8 IdleThreadStack[STACKSIZE]; // Based on sm64 and Padding between bss symbols.
OSThread VideoThread;
ALIGNED8 u8 VideoThreadStack[STACKSIZE];
UNUSED OSThread D_80156820;
UNUSED ALIGNED8 u8 D8015680Stack[STACKSIZE];
OSThread GameLoopThread;
ALIGNED8 u8 GameLoopThreadStack[STACKSIZE];
OSThread AudioThread;
ALIGNED8 u8 AudioThreadStack[STACKSIZE];
UNUSED OSThread D_8015CD30;
UNUSED ALIGNED8 u8 D8015CD30Stack[STACKSIZE / 2];

ALIGNED8 u8 GfxSPTaskYieldBuffer[4352];
ALIGNED8 u32 GfxSPTaskStack[256];
OSMesg PIMesgBuf[32];
OSMesgQueue PIMesgQueue;

s32 GameState = 0xFFFF;
// StartingIndicator is externed as an s32 in other files. DebugSection is only Used in Main.c, likely a developer mistake.
u16 StartingIndicator = RACE_NONE;
u16 DebugSection = 0;
u16 DisplayFlag = 0; // Renders the whole track. Displays red if Used in normal race Mode.
u16 DemoFlag = DEMO_MODE_INACTIVE;
u16 DebugMode = ENABLE_DEBUG_MODE;
s32 NewSequenceMode = 7; // = COURSE_DATA_MENU?;
UNUSED s32 NextSequenceModeUnused = 1;
s32 ScreenSplitA = SCREEN_MODE_1P;
s32 ScreenSplitB = SCREEN_MODE_1P;
UNUSED s32 D_800DC534 = 0;
s32 PlayerCount = 2;

s32 GameMode = GRAND_PRIX;
s32 CupLevel = 0;
s32 CupNumber = 0;
s32 RaceClass = CC_50;
s32 FrameCounter = 0;
UNUSED s32 D_800DC550 = 0;
UNUSED s32 D_800DC554 = 0;
UNUSED s32 D_800DC558 = 0;
// Framebuffer rendering values (max 3)
u16 DispFrame = 0;
u16 DrawFrame = 0;
UNUSED u16 D_800DC564 = 0;
s32 D_800DC568 = 0;
s32 D_800DC56C[8] = { 0 };
s16 Videoframe = 0;
UNUSED s16 D_800DC590 = 0;
f32 Timer = 0.0f;
f32 GameTimer = 0.0f;

void SysCreateThread(OSThread* thread, OSId id, void (*entry)(void*), void* arg, void* sp, OSPri pri) {
    thread->next = NULL;
    thread->queue = NULL;
    osCreateThread(thread, id, entry, arg, sp, pri);
}
void isPrintfInit(void);
void MainFunc(void) {
#ifdef VERSION_EU
    osTvType = TV_TYPE_PAL;
#endif
    osInitialize();
#ifdef DEBUG
    isPrintfInit(); // init osSyncPrintf
#endif
    SysCreateThread(&IdleThread, 1, &Thread1Idle, NULL, IdleThreadStack + ARRAY_COUNT(IdleThreadStack), 100);
    osStartThread(&IdleThread);
}

/**
 * Initialize hardware, Start Main thread, then idle.
 */
void Thread1Idle(void* arg) {
    osCreateViManager(OS_PRIORITY_VIMGR);
#ifdef VERSION_EU
    osViSetMode(&osViModeTable[OS_VI_PAL_LAN1]);
#else // VERSION_US
    if (osTvType == TV_TYPE_NTSC) {
        osViSetMode(&osViModeTable[OS_VI_NTSC_LAN1]);
    } else {
        osViSetMode(&osViModeTable[OS_VI_MPAL_LAN1]);
    }
#endif
    osViBlack(true);
    osViSetSpecialFeatures(OS_VI_GAMMA_OFF);
    osCreatePiManager(OS_PRIORITY_PIMGR, &PIMesgQueue, PIMesgBuf, ARRAY_COUNT(PIMesgBuf));
    ResertType = (s16) osResetType;
    CreateDebugThread();
    StartDebugThread();
    SysCreateThread(&VideoThread, 3, &Mainproc, arg, VideoThreadStack + ARRAY_COUNT(VideoThreadStack), 100);
    osStartThread(&VideoThread);
    osSetThreadPri(NULL, 0);

    // Halt
    while (true) {
        ;
    }
}

void SetupMessagequeue(void) {
    osCreateMesgQueue(&DmaMesgQueue, DmaMesgBuf, ARRAY_COUNT(DmaMesgBuf));
    osCreateMesgQueue(&gSPTaskMesgQueue, gSPTaskMesgBuf, ARRAY_COUNT(gSPTaskMesgBuf));
    osCreateMesgQueue(&IntrMesgQueue, IntrMesgBuf, ARRAY_COUNT(IntrMesgBuf));
    osViSetEvent(&IntrMesgQueue, (OSMesg) MESG_VI_VBLANK, 1);
    osSetEventMesg(OS_EVENT_SP, &IntrMesgQueue, (OSMesg) MESG_SP_COMPLETE);
    osSetEventMesg(OS_EVENT_DP, &IntrMesgQueue, (OSMesg) MESG_DP_COMPLETE);
}

void StartRSPTask(s32 taskType) {
    if (taskType == M_AUDTASK) {
        ActiveSPTask = SCurrentAudioSPTask;
    } else {
        ActiveSPTask = SCurrentDisplaySPTask;
    }
    osSpTaskLoad(&ActiveSPTask->Task);
    osSpTaskStartGo(&ActiveSPTask->Task);
    ActiveSPTask->State = SPTASK_STATE_RUNNING;
}

/**
 * Initializes the Fast3D OSTask structure.
 * Loads F3DEX or F3DLX based on the Number of players
 **/
void BuildGraphicsTask(void) {
    GfxSPTask->Msgqueue = &GfxVblankQueue;
    GfxSPTask->msg = (OSMesg) 2;
    GfxSPTask->Task.t.type = M_GFXTASK;
    GfxSPTask->Task.t.flags = OS_TASK_DP_WAIT;
    GfxSPTask->Task.t.ucode_boot = rspF3DBootStart;
    GfxSPTask->Task.t.ucode_boot_size = ((u8*) rspF3DBootEnd - (u8*) rspF3DBootStart);
    // The split-screen multiplayer racing State uses F3DLX which has a simple subpixel calculation.
    // Singleplayer race Mode and all other game states use F3DEX.
    // http://n64devkit.square7.ch/n64man/ucode/gspF3DEX.htm
    if (GameState != RACING || PlayerCount == 1) {
        GfxSPTask->Task.t.ucode = gspF3DEXTextStart;
        GfxSPTask->Task.t.ucode_data = gspF3DEXDataStart;
    } else {
        GfxSPTask->Task.t.ucode = gspF3DLXTextStart;
        GfxSPTask->Task.t.ucode_data = gspF3DLXDataStart;
    }
    GfxSPTask->Task.t.flags = 0;
    GfxSPTask->Task.t.flags = OS_TASK_DP_WAIT;
    GfxSPTask->Task.t.ucode_size = SP_UCODE_SIZE;
    GfxSPTask->Task.t.ucode_data_size = SP_UCODE_DATA_SIZE;
    GfxSPTask->Task.t.dram_stack = (u64*) &GfxSPTaskStack;
    GfxSPTask->Task.t.dram_stack_size = SP_DRAM_STACK_SIZE8;
    GfxSPTask->Task.t.output_buff = (u64*) &GfxSPTaskOutputBuffer;
    GfxSPTask->Task.t.output_buff_size = (u64*) ((u8*) GfxSPTaskOutputBuffer + sizeof(GfxSPTaskOutputBuffer));
    GfxSPTask->Task.t.data_ptr = (u64*) DynamicP->GFXPool;
    GfxSPTask->Task.t.data_size = (DisplayListHead - DynamicP->GFXPool) * sizeof(Gfx);
    KwdebugRcp();
    GfxSPTask->Task.t.yield_data_ptr = (u64*) &GfxSPTaskYieldBuffer;
    GfxSPTask->Task.t.yield_data_size = OS_YIELD_DATA_SIZE;
}

void InitControllers(void) {
    osCreateMesgQueue(&SIEventMesgQueue, &SIEventMesgBuf[0], ARRAY_COUNT(SIEventMesgBuf));
    osSetEventMesg(OS_EVENT_SI, &SIEventMesgQueue, (OSMesg) 0x33333333);
    osContInit(&SIEventMesgQueue, &ControllerBits, ControllerStatuses);
    if ((ControllerBits & 1) == 0) {
        SIsController1Unplugged = true;
    } else {
        SIsController1Unplugged = false;
    }
}

void CheckController(s32 Index) {
    struct Controller* controller = &Controllers[Index];
    u16 stick;

    if (SIsController1Unplugged) {
        return;
    }

    controller->AnalogX = ControllerPads[Index].stick_x;
    controller->AnalogY = ControllerPads[Index].stick_y;

    controller->ButtonPressed =
        ControllerPads[Index].button & (ControllerPads[Index].button ^ controller->ButtonHeld);
    controller->ButtonReleased = controller->ButtonHeld & (ControllerPads[Index].button ^ controller->ButtonHeld);
    controller->ButtonHeld = ControllerPads[Index].button;

    stick = 0;
    if (controller->AnalogX < -50) {
        stick |= L_JPAD;
    }
    if (controller->AnalogX > 50) {
        stick |= R_JPAD;
    }
    if (controller->AnalogY < -50) {
        stick |= D_JPAD;
    }
    if (controller->AnalogY > 50) {
        stick |= U_JPAD;
    }
    controller->AnalogPressed = stick & (stick ^ controller->AnalogHeld);
    controller->AnalogReleased = controller->AnalogHeld & (stick ^ controller->AnalogHeld);
    controller->AnalogHeld = stick;
}

void ReadControllers(void) {
    OSMesg msg;

    osContStartReadData(&SIEventMesgQueue);
    osRecvMesg(&SIEventMesgQueue, &msg, OS_MESG_BLOCK);
    osContGetReadData(ControllerPads);
    CheckController(0);
    CheckController(1);
    CheckController(2);
    CheckController(3);
    ControllerFive->ButtonHeld =
        (s16) (((ControllerOne->ButtonHeld | ControllerTwo->ButtonHeld) | ControllerThree->ButtonHeld) |
               ControllerFour->ButtonHeld);
    ControllerFive->ButtonPressed =
        (s16) (((ControllerOne->ButtonPressed | ControllerTwo->ButtonPressed) | ControllerThree->ButtonPressed) |
               ControllerFour->ButtonPressed);
    ControllerFive->ButtonReleased =
        (s16) (((ControllerOne->ButtonReleased | ControllerTwo->ButtonReleased) | ControllerThree->ButtonReleased) |
               ControllerFour->ButtonReleased);
    ControllerFive->AnalogHeld =
        (s16) (((ControllerOne->AnalogHeld | ControllerTwo->AnalogHeld) | ControllerThree->AnalogHeld) |
               ControllerFour->AnalogHeld);
    ControllerFive->AnalogPressed =
        (s16) (((ControllerOne->AnalogPressed | ControllerTwo->AnalogPressed) | ControllerThree->AnalogPressed) |
               ControllerFour->AnalogPressed);
    ControllerFive->AnalogReleased =
        (s16) (((ControllerOne->AnalogReleased | ControllerTwo->AnalogReleased) | ControllerThree->AnalogReleased) |
               ControllerFour->AnalogReleased);
    UpdateRumbleHelper();
    UpdateMotors();
}

void AllocDepthBuffer(void) {
    PhysicalZBuffer = VIRTUAL_TO_PHYSICAL(&ZBuffer);
}

void SetRSPTask(struct SPTask* SPTask) {
    osWritebackDCacheAll();
    osSendMesg(&gSPTaskMesgQueue, SPTask, OS_MESG_NOBLOCK);
}

void StartGraphicTask(struct SPTask* SPTask) {
    osWritebackDCacheAll();
    SPTask->State = SPTASK_STATE_NOT_STARTED;
    if (SCurrentDisplaySPTask == NULL) {
        SCurrentDisplaySPTask = SPTask;
        SNextDisplaySPTask = NULL;
        osSendMesg(&IntrMesgQueue, (OSMesg) MESG_START_GFX_SPTASK, OS_MESG_NOBLOCK);
    } else {
        SNextDisplaySPTask = SPTask;
    }
}

/**
 * Set default RCP (Reality Co-Processor) settings.
 */
void BeginDrawing(void) {
    StoreSegments();
    InitRDP();
    InitRSP();
    ClearFramebuffer();
    ClearZBuffer();
}

/**
 * End the master display list and initialize the graphics Task structure for the Next frame to be rendered.
 */
void EndDrawing(void) {
    gDPFullSync(DisplayListHead++);
    gSPEndDisplayList(DisplayListHead++);
    BuildGraphicsTask();
}

// clear_frame_buffer from SM64, with a few edits
//! @todo Why did void* work for matching
void* SoftwareBlanking(s32 color) {
    gDPPipeSync(DisplayListHead++);

    gDPSetRenderMode(DisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetCycleType(DisplayListHead++, G_CYC_FILL);

    gDPSetFillColor(DisplayListHead++, color);
    gDPFillRectangle(DisplayListHead++, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);

    gDPPipeSync(DisplayListHead++);

    gDPSetCycleType(DisplayListHead++, G_CYC_1CYCLE);
}

void InitialGameframe(void) {
    DynamicP = &GfxPools[0];
    SetSegment(1, DynamicP);
    GfxSPTask = &DynamicP->SPTask;
    DisplayListHead = DynamicP->GFXPool;
    BeginDrawing();
    SoftwareBlanking(0);
    EndDrawing();
    StartGraphicTask(&DynamicP->SPTask);
    DrawFrame++;
    FrameCounter++;
}

void BeginGameframe(void) {
    DynamicP = &GfxPools[FrameCounter & 1];
    SetSegment(1, DynamicP);
    DisplayListHead = DynamicP->GFXPool;
    GfxSPTask = &DynamicP->SPTask;
}

/**
 * Send Current master display list for rendering.
 * Tell the VI which colour framebuffer to display.
 * Yields to the VI Framerate twice, locking the game at 30 FPS.
 * Selects the Next framebuffer to be rendered and displayed.
 */
void FlushDisplaylist(void) {
    SetGraphCPUTime(BEFORE_DISPLAY_LISTS);
    osRecvMesg(&GfxVblankQueue, &MainReceivedMesg, OS_MESG_BLOCK);
    StartGraphicTask(&DynamicP->SPTask);
    SetGraphCPUTime(AFTER_DISPLAY_LISTS);
    osRecvMesg(&GameVblankQueue, &MainReceivedMesg, OS_MESG_BLOCK);
    osViSwapBuffer((void*) PHYSICAL_TO_VIRTUAL(CfbPtrs[DispFrame]));
    SetGraphCPUTime(THREAD5_END);
    osRecvMesg(&GameVblankQueue, &MainReceivedMesg, OS_MESG_BLOCK);
    SetFaultFrameBuffer(CfbPtrs[DispFrame]);

    if (++DispFrame == 3) {
        DispFrame = 0;
    }
    if (++DrawFrame == 3) {
        DrawFrame = 0;
    }
    FrameCounter++;
}

void InitExcode(void) {
    bzero((void*) SEG_ENDING, SEG_ENDING_SIZE);
    osWritebackDCacheAll();
    DMA((u8*) SEG_ENDING, (u8*) SEG_ENDING_ROM_START, SEG_ENDING_ROM_SIZE);
    osInvalICache((void*) SEG_ENDING, SEG_ENDING_SIZE);
    osInvalDCache((void*) SEG_ENDING, SEG_ENDING_SIZE);
}

void InitMkcode(void) {
    bzero((void*) SEG_RACING, SEG_RACING_SIZE);
    osWritebackDCacheAll();
    DMA((u8*) SEG_RACING, (u8*) SEG_RACING_ROM_START, SEG_RACING_ROM_SIZE);
    osInvalICache((void*) SEG_RACING, SEG_RACING_SIZE);
    osInvalDCache((void*) SEG_RACING, SEG_RACING_SIZE);
}

void InitOverkart(void) {
    bzero((void*) SEG_OVERKART, SEG_OVERKART_SIZE);
    osWritebackDCacheAll();
    DMA((u8*) SEG_OVERKART, (u8*) SEG_OVERKART_ROM_START, SEG_OVERKART_ROM_SIZE);
    osInvalICache((void*) SEG_OVERKART, SEG_OVERKART_SIZE);
    osInvalDCache((void*) SEG_OVERKART, SEG_OVERKART_SIZE);
    LoadBigFont();
}

void DMA(u8* dest, u8* romAddr, size_t Size) {

    osInvalDCache(dest, Size);
    while (Size > 0x100) {
        osPiStartDma(&DmaIoMesg, 0, 0, (uintptr_t) romAddr, dest, 0x100, &DmaMesgQueue);
        osRecvMesg(&DmaMesgQueue, &MainReceivedMesg, 1);
        Size -= 0x100;
        romAddr += 0x100;
        dest += 0x100;
    }
    if (Size != 0) {
        osPiStartDma(&DmaIoMesg, 0, 0, (uintptr_t) romAddr, dest, Size, &DmaMesgQueue);
        osRecvMesg(&DmaMesgQueue, &MainReceivedMesg, 1);
    }
}

/**
 * Setup Main segments and framebuffers.
 */
void InitializeSystemWorks(void) {
    UNUSED u32 pad[2];
    ptrdiff_t commonCourseDataSize; // Compressed mio0 Size
    uintptr_t textureSegSize;
    ptrdiff_t textureSegStart;
    uintptr_t allocatedMemory;
    UNUSED s32 unknown_padding;

    InitMkcode();
    InitOverkart();
    LastMemoryPointer = SEG_RACING;
    SetSegment(0, (void*) SEG_START);

    // Memory Pool Size of 0xAB630
    InitialFreeMemory(MEMORY_POOL_START, MEMORY_POOL_END);

    AllocDepthBuffer();

    // Initialize trig tables segment
    osInvalDCache((void*) TRIG_TABLES, TRIG_TABLES_SIZE);
    osPiStartDma(&DmaIoMesg, 0, 0, TRIG_TABLES_ROM_START, (void*) TRIG_TABLES, TRIG_TABLES_SIZE, &DmaMesgQueue);
    osRecvMesg(&DmaMesgQueue, &MainReceivedMesg, OS_MESG_BLOCK);

    SetSegment(2, (void*) LoadSegment(SEG_DATA_START, SEG_DATA_END));

    commonCourseDataSize = COMMON_TEXTURES_SIZE;
    commonCourseDataSize = ALIGN16(commonCourseDataSize);

#ifdef AVOID_UB
    textureSegStart = (ptrdiff_t) SEG_RACING - commonCourseDataSize;
#else
    textureSegStart = SEG_RACING - commonCourseDataSize;
#endif
    osPiStartDma(&DmaIoMesg, 0, 0, COMMON_TEXTURES_ROM_START, (void*) textureSegStart, commonCourseDataSize,
                 &DmaMesgQueue);
    osRecvMesg(&DmaMesgQueue, &MainReceivedMesg, OS_MESG_BLOCK);

    textureSegSize = *(uintptr_t*) (textureSegStart + 4);
    textureSegSize = ALIGN16(textureSegSize);
    allocatedMemory = FreeMemoryPointer;
    decodeMIO0((u8*) textureSegStart, (u8*) allocatedMemory);
    SetSegment(0xD, (void*) allocatedMemory);

    FreeMemoryPointer += textureSegSize;

    // Common course data does not get reloaded when the race State resets.
    // Therefore, only reset the memory ptr to after the common course data.
    StaticMemoryPointer = FreeMemoryPointer;
    VertexMemoryPointer = VERTEX_MEMORY_TOP;
    ObjectMemoryPointer = (uintptr_t) ok_ObjectPoolStart;
}

/**
 * @brief
 *
 */
void BootSequence(void) {
    NewSequenceMode = 0; // = START_MENU_FROM_QUIT?
    SoftwareBlanking(0);
}

void RaceSequence(void) {
    s16 i;
    u16 RotY;

    MatrixCount = 0;
    Effectcount = 0;
    if (PauseFlag != 0) {
        PauseSequence();
    }
    if (FadeOutFlag != 0) {
        FadeoutSequence();
        return;
    }

    StoreSegments();
    GameCodeDefault();

    if (Videoframe >= 6) {
        Videoframe = 5;
    }
    if (Videoframe < 0) {
        Videoframe = 1;
    }
    SetupPerspective();
    RunHotSwapScroll();

    switch (ScreenSplitA) {
        case SCREEN_MODE_1P:
            Framerate = 2;
            ApplyRaceTempo();
            KeyDataControl();
            if (PauseFlag == 0) {
                for (i = 0; i < Framerate; i++) {
                    if (TimeFlag) {
                        GameTimer += COURSE_TIMER_ITER;
                    }
                    CollisionKartToKart();
                    CollisionObjectToKart();
                    DriveStickControl();
                    CameraCheckFunc(PlayerOneCopy, Camera1, 0);
                    KartposControlFull();
                    EnemyControl();
                    KWVideoFramesYori();
                    ObjectStrategy();
                    MapStrategy();
                    RaceControl();
                }
                KenStartegy();
            }
            Kwgameframesyori();
            Videoframe = 0;
            SetGraphCPUTime(LEVEL_SCRIPT_EXECUTE);
            ScreenCounter = 0;
            DrawFullScreen();
            if (!DebugMode) {
                DebugSection = false;
            } else {
                if (DebugSection) {

                    if ((ControllerOne->ButtonPressed & R_TRIG) && (ControllerOne->ButtonHeld & A_BUTTON) &&
                        (ControllerOne->ButtonHeld & B_BUTTON)) {
                        DebugSection = false;
                    }

                    RotY = Camera1->CameraDirection[1];
                    DebugPathCount = D_800DC5EC->CameraPoint;
                    if (RotY < DEGREES(45)) {
                        Kwfprintud(40, 100, "SOUTH  ", DebugPathCount);
                    } else if (RotY < DEGREES(135)) {
                        Kwfprintud(40, 100, "EAST   ", DebugPathCount);
                    } else if (RotY < DEGREES(225)) {
                        Kwfprintud(40, 100, "NORTH  ", DebugPathCount);
                    } else if (RotY < DEGREES(315)) {
                        Kwfprintud(40, 100, "WEST   ", DebugPathCount);
                    } else {
                        Kwfprintud(40, 100, "SOUTH  ", DebugPathCount);
                    }

                } else {
                    if ((ControllerOne->ButtonPressed & L_TRIG) && (ControllerOne->ButtonHeld & A_BUTTON) &&
                        (ControllerOne->ButtonHeld & B_BUTTON)) {
                        DebugSection = true;
                    }
                }
            }
            break;

        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            if (CourseID == COURSE_DK_JUNGLE) {
                Framerate = 3;
            } else {
                Framerate = 2;
            }
            ApplyRaceTempo();
            if (PauseFlag == 0) {
                for (i = 0; i < Framerate; i++) {
                    if (TimeFlag != 0) {
                        GameTimer += COURSE_TIMER_ITER;
                    }
                    CollisionKartToKart();
                    CollisionObjectToKart();
                    DriveStickControl();
                    CameraCheckFunc(PlayerOneCopy, Camera1, 0);
                    KartposControlRu();
                    CameraCheckFunc(PlayerTwoCopy, Camera2, 1);
                    KartposControlLd();
                    EnemyControl();
                    KWVideoFramesYori();
                    ObjectStrategy();
                    MapStrategy();
                    RaceControl();
                }
                KenStartegy();
            }
            Kwgameframesyori();
            SetGraphCPUTime(LEVEL_SCRIPT_EXECUTE);
            Videoframe = 0;
            StoreSegments();
            InitRDP();
            if (ClearCFBFlag != 0) {
                ClearFramebuffer();
            }
            ScreenCounter = 0;
            if (WinKart == 0) {
                DrawRightScreen();
                DrawLeftScreen();
            } else {
                DrawLeftScreen();
                DrawRightScreen();
            }
            break;

        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:

            if (CourseID == COURSE_DK_JUNGLE) {
                Framerate = 3;
            } else {
                Framerate = 2;
            }
            ApplyRaceTempo();

            if (PauseFlag == 0) {
                for (i = 0; i < Framerate; i++) {
                    if (TimeFlag != 0) {
                        GameTimer += COURSE_TIMER_ITER;
                    }
                    CollisionKartToKart();
                    CollisionObjectToKart();
                    DriveStickControl();
                    CameraCheckFunc(PlayerOneCopy, Camera1, 0);
                    KartposControlRu();
                    CameraCheckFunc(PlayerTwoCopy, Camera2, 1);
                    KartposControlLd();
                    EnemyControl();
                    KWVideoFramesYori();
                    ObjectStrategy();
                    MapStrategy();
                    RaceControl();
                }
                KenStartegy();
            }
            SetGraphCPUTime(LEVEL_SCRIPT_EXECUTE);
            Videoframe = (u16) 0;
            Kwgameframesyori();
            StoreSegments();
            InitRDP();
            if (ClearCFBFlag != 0) {
                ClearFramebuffer();
            }
            ScreenCounter = 0;
            if (WinKart == 0) {
                DrawDownScreen();
                DrawUpScreen();
            } else {
                DrawUpScreen();
                DrawDownScreen();
            }

            break;

        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            if (PlayerCount == 3) {
                switch (CourseID) {
                    case COURSE_BOWSER_CASTLE:
                    case COURSE_MOO_MOO_FARM:
                    case COURSE_SKYSCRAPER:
                    case COURSE_DK_JUNGLE:
                        Framerate = 3;
                        break;
                    default:
                        Framerate = 2;
                        break;
                }
            } else {
                // Four players
                switch (CourseID) {
                    case COURSE_BLOCK_FORT:
                    case COURSE_DOUBLE_DECK:
                    case COURSE_BIG_DONUT:
                        Framerate = 2;
                        break;
                    case COURSE_DK_JUNGLE:
                        Framerate = 4;
                        break;
                    default:
                        Framerate = 3;
                        break;
                }
            }
            ApplyRaceTempo();
            if (PauseFlag == 0) {
                for (i = 0; i < Framerate; i++) {
                    if (TimeFlag != 0) {
                        GameTimer += COURSE_TIMER_ITER;
                    }
                    CollisionKartToKart();
                    CollisionObjectToKart();
                    DriveStickControl();
                    CameraCheckFunc(PlayerOneCopy, Camera1, 0);
                    KartpostControl1P();
                    CameraCheckFunc(PlayerTwo, Camera2, 1);
                    KartpostControl2P();
                    CameraCheckFunc(PlayerThree, Camera3, 2);
                    KartposControl3P();
                    CameraCheckFunc(PlayerFour, Camera4, 3);
                    KartposControl4P();
                    EnemyControl();
                    KWVideoFramesYori();
                    ObjectStrategy();
                    MapStrategy();
                    RaceControl();
                }
                KenStartegy();
            }
            Kwgameframesyori();
            Videoframe = 0;
            SetGraphCPUTime(LEVEL_SCRIPT_EXECUTE);
            StoreSegments();
            InitRDP();
            if (ClearCFBFlag != 0) {
                ClearFramebuffer();
            }
            ScreenCounter = 0;
            if (WinKart == 0) {
                DrawUpRightScreen();
                DrawDownLeftScreen();
                DrawDownRightScreen();
                DrawUpLeftScreen();
            } else if (WinKart == 1) {
                DrawUpLeftScreen();
                DrawDownLeftScreen();
                DrawDownRightScreen();
                DrawUpRightScreen();
            } else if (WinKart == 2) {
                DrawUpLeftScreen();
                DrawUpRightScreen();
                DrawDownRightScreen();
                DrawDownLeftScreen();
            } else {
                DrawUpLeftScreen();
                DrawUpRightScreen();
                DrawDownLeftScreen();
                DrawDownRightScreen();
            }
            break;
    }

    if (!DebugMode) {
        DispTimeFlag = 0;
    } else {
        if (DispTimeFlag) {
            DisplayProcTime();
            if ((!(ControllerOne->ButtonHeld & L_TRIG)) && (ControllerOne->ButtonHeld & R_TRIG) &&
                (ControllerOne->ButtonPressed & B_BUTTON)) {
                DispTimeFlag = 0;
            }
        } else {
            if ((!(ControllerOne->ButtonHeld & L_TRIG)) && (ControllerOne->ButtonHeld & R_TRIG) &&
                (ControllerOne->ButtonPressed & B_BUTTON)) {
                DispTimeFlag = 1;
            }
        }
    }
    MakeBorder();
    KwdisplayToppri();
    MiniMapDrawDefault();
#if DVDL
    display_dvdl();
#endif
    gDPFullSync(DisplayListHead++);
    gSPEndDisplayList(DisplayListHead++);
}

/**
 * mk64's game Loop depends on a series of states.
 * It runs a wide branching series of Code based on these states.
 * State 1) Clear framebuffer
 * State 2) Run menus
 * State 3) Process race related logic
 * State 4) Ending sequence
 * State 5) Credits
 *
 * Note that the State doesn't flip-flop at random but is permanent
 * until the State Changes (ie. Exit menus and Start a race).
 */

void ExecuteSequence(void) {
#if DVDL
    if ((ControllerOne->ButtonHeld & L_TRIG) && (ControllerOne->ButtonHeld & R_TRIG) &&
        (ControllerOne->ButtonHeld & Z_TRIG) && (ControllerOne->ButtonHeld & A_BUTTON)) {
        NewSequenceMode = CREDITS_SEQUENCE;
    } else if ((ControllerOne->ButtonHeld & L_TRIG) && (ControllerOne->ButtonHeld & R_TRIG) &&
               (ControllerOne->ButtonHeld & Z_TRIG) && (ControllerOne->ButtonHeld & B_BUTTON)) {
        NewSequenceMode = ENDING;
    }
#endif

    switch (GameState) {
        case 7:
            BootSequence();
            break;
        case START_MENU_FROM_QUIT:
        case MAIN_MENU_FROM_QUIT:
        case PLAYER_SELECT_MENU_FROM_QUIT:
        case COURSE_SELECT_MENU_FROM_QUIT:
            // Display black
            osViBlack(0);
            UpdateController();
            BeginDrawing();
            DoGameSelect(DynamicP);
#if DVDL
            display_dvdl();
#endif
            break;
        case RACING:
            RaceSequence();
            break;
        case ENDING:
            ResultsSequence();
            break;
        case CREDITS_SEQUENCE:
            EndingSequence();
            break;
    }
    PrintMenuFunctionDefault();
}

void YieldRSPTask(void) {
    if (ActiveSPTask->Task.t.type == M_GFXTASK) {
        ActiveSPTask->State = SPTASK_STATE_INTERRUPTED;
        osSpTaskYield();
    }
}

void GetTaskMessage(void) {
    UNUSED s32 pad;
    struct SPTask* SPTask;

    while (osRecvMesg(&gSPTaskMesgQueue, (OSMesg*) &SPTask, OS_MESG_NOBLOCK) != -1) {
        SPTask->State = SPTASK_STATE_NOT_STARTED;
        switch (SPTask->Task.t.type) {
            case 2:
                SNextAudioSPTask = SPTask;
                break;
            case 1:
                SNextDisplaySPTask = SPTask;
                break;
        }
    }

    if (SCurrentAudioSPTask == NULL && SNextAudioSPTask != NULL) {
        SCurrentAudioSPTask = SNextAudioSPTask;
        SNextAudioSPTask = NULL;
    }
    if (SCurrentDisplaySPTask == NULL && SNextDisplaySPTask != NULL) {
        SCurrentDisplaySPTask = SNextDisplaySPTask;
        SNextDisplaySPTask = NULL;
    }
}

void SetClient(s32 Index, struct VblankHandler* handler, OSMesgQueue* Queue, OSMesg* msg) {
    handler->Queue = Queue;
    handler->msg = msg;
    switch (Index) {
        case 1:
            VblankHandler1 = handler;
            break;
        case 2:
            VblankHandler2 = handler;
            break;
    }
}

void StartGFXTask(void) {
    if (ActiveSPTask == NULL && SCurrentDisplaySPTask != NULL &&
        SCurrentDisplaySPTask->State == SPTASK_STATE_NOT_STARTED) {
        SetGraphRCPTime(TASKS_QUEUED);
        StartRSPTask(M_GFXTASK);
    }
}

void InterruptSequence(void) {
    Timer += V_BlANK_TIMER_ITER;
    Videoframe++;

    GetTaskMessage();

    // First try to kick off an audio Task. If the gfx Task is currently
    // running, we need to asynchronously interrupt it -- RSPDoneSequence
    // will pick up on what we're doing and Start the audio Task for us.
    // If there is already an audio Task running, there is nothing to do.
    // If there is no audio Task available, try a gfx Task instead.
    if (SCurrentAudioSPTask != NULL) {
        if (ActiveSPTask != NULL) {
            YieldRSPTask();
        } else {
            SetAudioRCPTime();
            StartRSPTask(M_AUDTASK);
        }
    } else {
        if (ActiveSPTask == NULL && SCurrentDisplaySPTask != NULL &&
            SCurrentDisplaySPTask->State != SPTASK_STATE_FINISHED) {
            SetGraphRCPTime(TASKS_QUEUED);
            StartRSPTask(M_GFXTASK);
        }
    }

/* This is where I would put my rumble Code... If I had any. */
#if ENABLE_RUMBLE
    rumble_thread_update_vi();
#endif

    if (VblankHandler1 != NULL) {
        osSendMesg(VblankHandler1->Queue, VblankHandler1->msg, OS_MESG_NOBLOCK);
    }
    if (VblankHandler2 != NULL) {
        osSendMesg(VblankHandler2->Queue, VblankHandler2->msg, OS_MESG_NOBLOCK);
    }
}

void RDPDoneSequence(void) {
    // Gfx SP Task is completely done.
    if (SCurrentDisplaySPTask->Msgqueue != NULL) {
        osSendMesg(SCurrentDisplaySPTask->Msgqueue, SCurrentDisplaySPTask->msg, OS_MESG_NOBLOCK);
    }
    SetGraphRCPTime(RDP_COMPLETE);
    SCurrentDisplaySPTask->State = SPTASK_STATE_FINISHED_DP;
    SCurrentDisplaySPTask = NULL;
}

void RSPDoneSequence(void) {
    struct SPTask* curSPTask = ActiveSPTask;

    ActiveSPTask = NULL;

    if (curSPTask->State == SPTASK_STATE_INTERRUPTED) {
        // InterruptSequence tried to Start an audio Task while there was already a
        // gfx Task running, so it had to interrupt the gfx Task. That interruption
        // just Finished.
        if (osSpTaskYielded((OSTask*) curSPTask) == 0) {
            // The gfx Task completed before we had Time to interrupt it.
            // Mark it Finished, just like below.
            curSPTask->State = SPTASK_STATE_FINISHED;
            SetGraphRCPTime(RSP_COMPLETE);
        }
        // Start the audio Task, as expected by InterruptSequence.
        SetAudioRCPTime();
        StartRSPTask(M_AUDTASK);
    } else {
        curSPTask->State = SPTASK_STATE_FINISHED;
        if (curSPTask->Task.t.type == M_AUDTASK) {
            // After audio tasks come gfx tasks.
            SetAudioRCPTime();
            if (SCurrentDisplaySPTask != NULL) {
                if (SCurrentDisplaySPTask->State != SPTASK_STATE_FINISHED) {
                    if (SCurrentDisplaySPTask->State != SPTASK_STATE_INTERRUPTED) {
                        SetGraphRCPTime(TASKS_QUEUED);
                    }
                    StartRSPTask(M_GFXTASK);
                }
            }
            SCurrentAudioSPTask = NULL;
            if (curSPTask->Msgqueue != NULL) {
                osSendMesg(curSPTask->Msgqueue, curSPTask->msg, OS_MESG_NOBLOCK);
            }
        } else {
            // The SP process is done, but there is still a Display Processor notification
            // that needs to arrive before we can consider the Task completely Finished and
            // Null out SCurrentDisplaySPTask. That happens in RDPDoneSequence.
            SetGraphRCPTime(RSP_COMPLETE);
        }
    };
}

void Mainproc(UNUSED void* arg0) {
    s32 i;
    u64* framebuffer1;
    OSMesg msg;
    UNUSED s32 pad[4];

    CfbPtrs[0] = (u16*) &Framebuffer0;
    CfbPtrs[1] = (u16*) &Framebuffer1;
    CfbPtrs[2] = (u16*) &Framebuffer2;

    // Clear framebuffer.
    framebuffer1 = (u64*) &Framebuffer1;
    for (i = 0; i < 19200; i++) {
        framebuffer1[i] = 0;
    }
    SetupMessagequeue();
    InitializeSystemWorks();

    SysCreateThread(&AudioThread, 4, &Audioproc, 0, AudioThreadStack + ARRAY_COUNT(AudioThreadStack), 20);
    osStartThread(&AudioThread);

    SysCreateThread(&GameLoopThread, 5, &Gameproc, 0, GameLoopThreadStack + ARRAY_COUNT(GameLoopThreadStack),
                  10);
    osStartThread(&GameLoopThread);

    while (true) {
        osRecvMesg(&IntrMesgQueue, &msg, OS_MESG_BLOCK);
        switch ((u32) msg) {
            case MESG_VI_VBLANK:
                InterruptSequence();
                break;
            case MESG_SP_COMPLETE:
                RSPDoneSequence();
                break;
            case MESG_DP_COMPLETE:
                RDPDoneSequence();
                break;
            case MESG_START_GFX_SPTASK:
                StartGFXTask();
                break;
        }
    }
}

void InitialGameselectSequence(void) {
    InitMKCode();
    ScreenSplitA = SCREEN_MODE_1P;
    InititalPerspective();
}

void InitialKartselectSequence(void) {
    InitMKCode();
    ScreenSplitA = SCREEN_MODE_1P;
    InititalPerspective();
}

void InitialMapselectSequence(void) {
    InitMKCode();
    ScreenSplitA = SCREEN_MODE_1P;
    InititalPerspective();
}

void InitialTitleSequence(void) {
    InitMKCode();
    ScreenSplitA = SCREEN_MODE_1P;
    InititalPerspective();
}

/**
 * Sets courseId to NULL if
 *
 *
 */
void InitializeSequence(void) {
    switch (GameState) {
        case START_MENU_FROM_QUIT:
            InitialTitleSequence();
            LoadedcourseFlag = COURSE_NULL;
            break;
        case MAIN_MENU_FROM_QUIT:
            InitialGameselectSequence();
            LoadedcourseFlag = COURSE_NULL;
            break;
        case PLAYER_SELECT_MENU_FROM_QUIT:
            InitialKartselectSequence();
            LoadedcourseFlag = COURSE_NULL;
            break;
        case COURSE_SELECT_MENU_FROM_QUIT:
            InitialMapselectSequence();
            LoadedcourseFlag = COURSE_NULL;
            break;
        case RACING:
            InitialRaceSequence();
            break;
        case ENDING:
            LoadedcourseFlag = COURSE_NULL;
            InitExcode();
            InitialResultSequence();
            break;
        case CREDITS_SEQUENCE:
            LoadedcourseFlag = COURSE_NULL;
            InitExcode();
            InitializeEndingSequence();
            break;
    }
}

void Gameproc(UNUSED void* arg) {
    osCreateMesgQueue(&GfxVblankQueue, GfxMesgBuf, 1);
    osCreateMesgQueue(&GameVblankQueue, &GameMesgBuf, 1);
    InitControllers();
    if (!ResertType) {
        InitializeFirstOnce();
    }

    SetClient(2, &GameVblankHandler, &GameVblankQueue, (OSMesg) OS_EVENT_SW2);
    // These variables track stats such as Player wins.
    // In the event of a console reset, it remembers them.
    Wincount2P = &PAppNmiBuffer[0]; // 2  u8's, tracks Number of times Player 1/2 won a VS race
    Wincount3P =
        &PAppNmiBuffer[2]; // 9  u8's, 3x3, tracks Number of times Player 1/2/3   has placed in 1st/2nd/3rd in a VS race
    Wincount4P = &PAppNmiBuffer[11]; // 12 u8's, 4x3, tracks Number of times Player 1/2/3/4 has placed in 1st/2nd/3rd
                                       // in a VS race
    Battlewincount2P = &PAppNmiBuffer[23]; // 2  u8's, tracking Number of Battle Mode wins by Player 1/2
    Battlewincount3P = &PAppNmiBuffer[25]; // 3  u8's, tracking Number of Battle Mode wins by Player 1/2/3
    Battlewincount4P = &PAppNmiBuffer[28]; // 4  u8's, tracking Number of Battle Mode wins by Player 1/2/3/4
    InitialGameframe();
    ReadControllers();
    NaiInitInterface();

    while (true) {
        NasAudioInput();

        // Update the gamestate if it has changed (racing, menus, credits, etc.).
        if (NewSequenceMode != GameState) {
            GameState = NewSequenceMode;
            InitializeSequence();
        }
        SetGraphCPUTime(THREAD5_START);
        BeginGameframe();
        ReadControllers();
        ExecuteSequence();
        EndDrawing();
        FlushDisplaylist();
        AllRunDefault();
    }
}

/**
 * Sound processing thread. Runs at 50 or 60 FPS according to osTvType.
 */
void Audioproc(UNUSED void* arg) {
    UNUSED u32 Unused[3];
    NasInitAudio();
    osCreateMesgQueue(&SSoundMesgQueue, SSoundMesgBuf, ARRAY_COUNT(SSoundMesgBuf));
    SetClient(1, &SSoundVblankHandler, &SSoundMesgQueue, (OSMesg) 512);

    while (true) {
        OSMesg msg;
        struct SPTask* SPTask;

        osRecvMesg(&SSoundMesgQueue, &msg, OS_MESG_BLOCK);

        SetAudioCPUTime();

        SPTask = NasAudioMain();
        if (SPTask != NULL) {
            SetRSPTask(SPTask);
        }
        SetAudioCPUTime();
    }
}

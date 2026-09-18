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
#include "OverKartHooks.h"

void InitMKCode(void);
void Nas_InitAudio(void);
void CreateDebugThread(void);
void StartDebugThread(void);
struct SPTask* Nas_AudioMain(void);

struct VblankHandler* gVblankHandler1 = NULL;
struct VblankHandler* gVblankHandler2 = NULL;

struct SPTask* gActiveSPTask = NULL;
struct SPTask* sCurrentAudioSPTask = NULL;
struct SPTask* sCurrentDisplaySPTask = NULL;
struct SPTask* sNextAudioSPTask = NULL;
struct SPTask* sNextDisplaySPTask = NULL;

struct Controller gControllers[NUM_PLAYERS];
struct Controller* gControllerOne = &gControllers[0];
struct Controller* gControllerTwo = &gControllers[1];
struct Controller* gControllerThree = &gControllers[2];
struct Controller* gControllerFour = &gControllers[3];
struct Controller* gControllerFive = &gControllers[4]; // All physical controllers combined.`
struct Controller* gControllerSix = &gControllers[5];
struct Controller* gControllerSeven = &gControllers[6];
struct Controller* gControllerEight = &gControllers[7];

Player gPlayers[NUM_PLAYERS];
Player* gPlayerOne = &gPlayers[0];
Player* gPlayerTwo = &gPlayers[1];
Player* gPlayerThree = &gPlayers[2];
Player* gPlayerFour = &gPlayers[3];
Player* gPlayerFive = &gPlayers[4];
Player* gPlayerSix = &gPlayers[5];
Player* gPlayerSeven = &gPlayers[6];
Player* gPlayerEight = &gPlayers[7];

Player* gPlayerOneCopy = &gPlayers[0];
Player* gPlayerTwoCopy = &gPlayers[1];
UNUSED Player* gPlayerThreeCopy = &gPlayers[2];
UNUSED Player* gPlayerFourCopy = &gPlayers[3];

UNUSED s32 D_800FD850[3];
struct GfxPool gGfxPools[2];
struct GfxPool* gDynamicP;

UNUSED s32 gfxPool_padding; // is this necessary?
struct VblankHandler gGameVblankHandler;
struct VblankHandler sSoundVblankHandler;
OSMesgQueue gDmaMesgQueue, gGameVblankQueue, gGfxVblankQueue, unused_gMsgQueue, gIntrMesgQueue, gSPTaskMesgQueue;
OSMesgQueue sSoundMesgQueue;
OSMesg sSoundMesgBuf[1];
OSMesg gDmaMesgBuf[1], gGameMesgBuf;
OSMesg gGfxMesgBuf[1];
UNUSED OSMesg D_8014F010, D_8014F014;
OSMesg gIntrMesgBuf[16], gSPTaskMesgBuf[16];
OSMesg gMainReceivedMesg;
OSIoMesg gDmaIoMesg;
OSMesgQueue gSIEventMesgQueue;
OSMesg gSIEventMesgBuf[3];

OSContStatus gControllerStatuses[4];
OSContPad gControllerPads[4];
u8 gControllerBits;
// Contains a 32x32 grid of indices into collision_table containing indices into gFaceBuffer
CollisionGrid gCollisionGrid[1024];
u16 g_simpleObjectCount;
u16 gMatrixCount;
s32 framerate;
f32 D_80150118;

u16 resert_type;
u16 time_flag;

s32 race_clear_flag;
s32 next_sequence_mode;
UNUSED s32 D_80150128;
UNUSED s32 D_8015012C;
f32 gCameraZoom[4]; // look like to be the fov of each character
UNUSED s32 D_80150140;
UNUSED s32 D_80150144;
f32 g_aspectRatio;
f32 g_farClip;
f32 screen_near_point;
UNUSED f32 D_80150154;

struct D_80150158 gD_80150158[16];
uintptr_t SegmentTable[16];
Gfx* gDisplayListHead;

struct SPTask* gGfxSPTask;
s32 D_801502A0;
s32 D_801502A4;
u16* g_CfbPtrs[3];
uintptr_t gPhysicalZBuffer;
UNUSED u32 D_801502B8;
UNUSED u32 D_801502BC;
Mat4 gAffine;

s32 padding[2048];

u16 camera_mode[4];
u16 camera_point;

UNUSED OSThread paddingThread;
OSThread gIdleThread;
ALIGNED8 u8 gIdleThreadStack[STACKSIZE]; // Based on sm64 and padding between bss symbols.
OSThread gVideoThread;
ALIGNED8 u8 gVideoThreadStack[STACKSIZE];
UNUSED OSThread D_80156820;
UNUSED ALIGNED8 u8 D_8015680_Stack[STACKSIZE];
OSThread gGameLoopThread;
ALIGNED8 u8 gGameLoopThreadStack[STACKSIZE];
OSThread gAudioThread;
ALIGNED8 u8 gAudioThreadStack[STACKSIZE];
UNUSED OSThread D_8015CD30;
UNUSED ALIGNED8 u8 D_8015CD30_Stack[STACKSIZE / 2];

ALIGNED8 u8 gGfxSPTaskYieldBuffer[4352];
ALIGNED8 u32 gGfxSPTaskStack[256];
OSMesg gPIMesgBuf[32];
OSMesgQueue gPIMesgQueue;

s32 gGamestate = 0xFFFF;
// g_startingIndicator is externed as an s32 in other files. g_DebugSection is only used in main.c, likely a developer mistake.
u16 g_startingIndicator = RACE_NONE;
u16 g_DebugSection = 0;
u16 g_DisplayFlag = 0; // Renders the whole track. Displays red if used in normal race mode.
u16 g_DemoFlag = DEMO_MODE_INACTIVE;
u16 g_DebugMode = ENABLE_DEBUG_MODE;
s32 g_NewSequenceMode = 7; // = COURSE_DATA_MENU?;
UNUSED s32 g_NextSequenceMode = 1;
s32 g_ScreenSplitA = SCREEN_MODE_1P;
s32 g_ScreenSplitB = SCREEN_MODE_1P;
UNUSED s32 D_800DC534 = 0;
s32 g_playerCount = 2;

s32 g_gameMode = GRAND_PRIX;
s32 cup_level = 0;
s32 cup_number = 0;
s32 g_raceClass = CC_50;
s32 frame_counter = 0;
UNUSED s32 D_800DC550 = 0;
UNUSED s32 D_800DC554 = 0;
UNUSED s32 D_800DC558 = 0;
// Framebuffer rendering values (max 3)
u16 g_DispFrame = 0;
u16 draw_frame = 0;
UNUSED u16 D_800DC564 = 0;
s32 D_800DC568 = 0;
s32 D_800DC56C[8] = { 0 };
s16 videoframe = 0;
UNUSED s16 D_800DC590 = 0;
f32 timer = 0.0f;
f32 g_gameTimer = 0.0f;

void SysCreateThread(OSThread* thread, OSId id, void (*entry)(void*), void* arg, void* sp, OSPri pri) {
    thread->next = NULL;
    thread->queue = NULL;
    osCreateThread(thread, id, entry, arg, sp, pri);
}
void isPrintfInit(void);
void main_func(void) {
#ifdef VERSION_EU
    osTvType = TV_TYPE_PAL;
#endif
    osInitialize();
#ifdef DEBUG
    isPrintfInit(); // init osSyncPrintf
#endif
    SysCreateThread(&gIdleThread, 1, &thread1_idle, NULL, gIdleThreadStack + ARRAY_COUNT(gIdleThreadStack), 100);
    osStartThread(&gIdleThread);
}

/**
 * Initialize hardware, start main thread, then idle.
 */
void thread1_idle(void* arg) {
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
    osCreatePiManager(OS_PRIORITY_PIMGR, &gPIMesgQueue, gPIMesgBuf, ARRAY_COUNT(gPIMesgBuf));
    resert_type = (s16) osResetType;
    CreateDebugThread();
    StartDebugThread();
    SysCreateThread(&gVideoThread, 3, &mainproc, arg, gVideoThreadStack + ARRAY_COUNT(gVideoThreadStack), 100);
    osStartThread(&gVideoThread);
    osSetThreadPri(NULL, 0);

    // Halt
    while (true) {
        ;
    }
}

void setup_messagequeue(void) {
    osCreateMesgQueue(&gDmaMesgQueue, gDmaMesgBuf, ARRAY_COUNT(gDmaMesgBuf));
    osCreateMesgQueue(&gSPTaskMesgQueue, gSPTaskMesgBuf, ARRAY_COUNT(gSPTaskMesgBuf));
    osCreateMesgQueue(&gIntrMesgQueue, gIntrMesgBuf, ARRAY_COUNT(gIntrMesgBuf));
    osViSetEvent(&gIntrMesgQueue, (OSMesg) MESG_VI_VBLANK, 1);
    osSetEventMesg(OS_EVENT_SP, &gIntrMesgQueue, (OSMesg) MESG_SP_COMPLETE);
    osSetEventMesg(OS_EVENT_DP, &gIntrMesgQueue, (OSMesg) MESG_DP_COMPLETE);
}

void start_RSP_task(s32 taskType) {
    if (taskType == M_AUDTASK) {
        gActiveSPTask = sCurrentAudioSPTask;
    } else {
        gActiveSPTask = sCurrentDisplaySPTask;
    }
    osSpTaskLoad(&gActiveSPTask->task);
    osSpTaskStartGo(&gActiveSPTask->task);
    gActiveSPTask->state = SPTASK_STATE_RUNNING;
}

/**
 * Initializes the Fast3D OSTask structure.
 * Loads F3DEX or F3DLX based on the number of players
 **/
void build_graphics_task(void) {
    gGfxSPTask->msgqueue = &gGfxVblankQueue;
    gGfxSPTask->msg = (OSMesg) 2;
    gGfxSPTask->task.t.type = M_GFXTASK;
    gGfxSPTask->task.t.flags = OS_TASK_DP_WAIT;
    gGfxSPTask->task.t.ucode_boot = rspF3DBootStart;
    gGfxSPTask->task.t.ucode_boot_size = ((u8*) rspF3DBootEnd - (u8*) rspF3DBootStart);
    // The split-screen multiplayer racing state uses F3DLX which has a simple subpixel calculation.
    // Singleplayer race mode and all other game states use F3DEX.
    // http://n64devkit.square7.ch/n64man/ucode/gspF3DEX.htm
    if (gGamestate != RACING || g_playerCount == 1) {
        gGfxSPTask->task.t.ucode = gspF3DEXTextStart;
        gGfxSPTask->task.t.ucode_data = gspF3DEXDataStart;
    } else {
        gGfxSPTask->task.t.ucode = gspF3DLXTextStart;
        gGfxSPTask->task.t.ucode_data = gspF3DLXDataStart;
    }
    gGfxSPTask->task.t.flags = 0;
    gGfxSPTask->task.t.flags = OS_TASK_DP_WAIT;
    gGfxSPTask->task.t.ucode_size = SP_UCODE_SIZE;
    gGfxSPTask->task.t.ucode_data_size = SP_UCODE_DATA_SIZE;
    gGfxSPTask->task.t.dram_stack = (u64*) &gGfxSPTaskStack;
    gGfxSPTask->task.t.dram_stack_size = SP_DRAM_STACK_SIZE8;
    gGfxSPTask->task.t.output_buff = (u64*) &gGfxSPTaskOutputBuffer;
    gGfxSPTask->task.t.output_buff_size = (u64*) ((u8*) gGfxSPTaskOutputBuffer + sizeof(gGfxSPTaskOutputBuffer));
    gGfxSPTask->task.t.data_ptr = (u64*) gDynamicP->gfxPool;
    gGfxSPTask->task.t.data_size = (gDisplayListHead - gDynamicP->gfxPool) * sizeof(Gfx);
    kwdebug_rcp();
    gGfxSPTask->task.t.yield_data_ptr = (u64*) &gGfxSPTaskYieldBuffer;
    gGfxSPTask->task.t.yield_data_size = OS_YIELD_DATA_SIZE;
}

void InitControllers(void) {
    osCreateMesgQueue(&gSIEventMesgQueue, &gSIEventMesgBuf[0], ARRAY_COUNT(gSIEventMesgBuf));
    osSetEventMesg(OS_EVENT_SI, &gSIEventMesgQueue, (OSMesg) 0x33333333);
    osContInit(&gSIEventMesgQueue, &gControllerBits, gControllerStatuses);
    if ((gControllerBits & 1) == 0) {
        sIsController1Unplugged = true;
    } else {
        sIsController1Unplugged = false;
    }
}

void check_controller(s32 index) {
    struct Controller* controller = &gControllers[index];
    u16 stick;

    if (sIsController1Unplugged) {
        return;
    }

    controller->AnalogX = gControllerPads[index].stick_x;
    controller->AnalogY = gControllerPads[index].stick_y;

    controller->ButtonPressed =
        gControllerPads[index].button & (gControllerPads[index].button ^ controller->ButtonHeld);
    controller->ButtonReleased = controller->ButtonHeld & (gControllerPads[index].button ^ controller->ButtonHeld);
    controller->ButtonHeld = gControllerPads[index].button;

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

void readControllers(void) {
    OSMesg msg;

    osContStartReadData(&gSIEventMesgQueue);
    osRecvMesg(&gSIEventMesgQueue, &msg, OS_MESG_BLOCK);
    osContGetReadData(gControllerPads);
    check_controller(0);
    check_controller(1);
    check_controller(2);
    check_controller(3);
    gControllerFive->ButtonHeld =
        (s16) (((gControllerOne->ButtonHeld | gControllerTwo->ButtonHeld) | gControllerThree->ButtonHeld) |
               gControllerFour->ButtonHeld);
    gControllerFive->ButtonPressed =
        (s16) (((gControllerOne->ButtonPressed | gControllerTwo->ButtonPressed) | gControllerThree->ButtonPressed) |
               gControllerFour->ButtonPressed);
    gControllerFive->ButtonReleased =
        (s16) (((gControllerOne->ButtonReleased | gControllerTwo->ButtonReleased) | gControllerThree->ButtonReleased) |
               gControllerFour->ButtonReleased);
    gControllerFive->AnalogHeld =
        (s16) (((gControllerOne->AnalogHeld | gControllerTwo->AnalogHeld) | gControllerThree->AnalogHeld) |
               gControllerFour->AnalogHeld);
    gControllerFive->AnalogPressed =
        (s16) (((gControllerOne->AnalogPressed | gControllerTwo->AnalogPressed) | gControllerThree->AnalogPressed) |
               gControllerFour->AnalogPressed);
    gControllerFive->AnalogReleased =
        (s16) (((gControllerOne->AnalogReleased | gControllerTwo->AnalogReleased) | gControllerThree->AnalogReleased) |
               gControllerFour->AnalogReleased);
}

void AllocDepthBuffer(void) {
    gPhysicalZBuffer = VIRTUAL_TO_PHYSICAL(&gZBuffer);
}

void set_RSP_task(struct SPTask* spTask) {
    osWritebackDCacheAll();
    osSendMesg(&gSPTaskMesgQueue, spTask, OS_MESG_NOBLOCK);
}

void start_graphic_task(struct SPTask* spTask) {
    osWritebackDCacheAll();
    spTask->state = SPTASK_STATE_NOT_STARTED;
    if (sCurrentDisplaySPTask == NULL) {
        sCurrentDisplaySPTask = spTask;
        sNextDisplaySPTask = NULL;
        osSendMesg(&gIntrMesgQueue, (OSMesg) MESG_START_GFX_SPTASK, OS_MESG_NOBLOCK);
    } else {
        sNextDisplaySPTask = spTask;
    }
}

/**
 * Set default RCP (Reality Co-Processor) settings.
 */
void begin_drawing(void) {
    StoreSegments();
    InitRDP();
    InitRSP();
    g_ClearFramebuffer();
    ClearZBuffer();
}

/**
 * End the master display list and initialize the graphics task structure for the next frame to be rendered.
 */
void end_drawing(void) {
    gDPFullSync(gDisplayListHead++);
    gSPEndDisplayList(gDisplayListHead++);
    build_graphics_task();
}

// clear_frame_buffer from SM64, with a few edits
//! @todo Why did void* work for matching
void* SoftwareBlanking(s32 color) {
    gDPPipeSync(gDisplayListHead++);

    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);

    gDPSetFillColor(gDisplayListHead++, color);
    gDPFillRectangle(gDisplayListHead++, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);

    gDPPipeSync(gDisplayListHead++);

    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
}

void initial_gameframe(void) {
    gDynamicP = &gGfxPools[0];
    SetSegment(1, gDynamicP);
    gGfxSPTask = &gDynamicP->spTask;
    gDisplayListHead = gDynamicP->gfxPool;
    begin_drawing();
    SoftwareBlanking(0);
    end_drawing();
    start_graphic_task(&gDynamicP->spTask);
    draw_frame++;
    frame_counter++;
}

void begin_gameframe(void) {
    gDynamicP = &gGfxPools[frame_counter & 1];
    SetSegment(1, gDynamicP);
    gDisplayListHead = gDynamicP->gfxPool;
    gGfxSPTask = &gDynamicP->spTask;
}

/**
 * Send current master display list for rendering.
 * Tell the VI which colour framebuffer to display.
 * Yields to the VI framerate twice, locking the game at 30 FPS.
 * Selects the next framebuffer to be rendered and displayed.
 */
void flush_displaylist(void) {
    SetGraphCPUTime(BEFORE_DISPLAY_LISTS);
    osRecvMesg(&gGfxVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    start_graphic_task(&gDynamicP->spTask);
    SetGraphCPUTime(AFTER_DISPLAY_LISTS);
    osRecvMesg(&gGameVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    osViSwapBuffer((void*) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[g_DispFrame]));
    SetGraphCPUTime(THREAD5_END);
    osRecvMesg(&gGameVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    SetFaultFrameBuffer(g_CfbPtrs[g_DispFrame]);

    if (++g_DispFrame == 3) {
        g_DispFrame = 0;
    }
    if (++draw_frame == 3) {
        draw_frame = 0;
    }
    frame_counter++;
}

void init_excode(void) {
    bzero((void*) SEG_ENDING, SEG_ENDING_SIZE);
    osWritebackDCacheAll();
    DMA((u8*) SEG_ENDING, (u8*) SEG_ENDING_ROM_START, SEG_ENDING_ROM_SIZE);
    osInvalICache((void*) SEG_ENDING, SEG_ENDING_SIZE);
    osInvalDCache((void*) SEG_ENDING, SEG_ENDING_SIZE);
}

void init_mkcode(void) {
    bzero((void*) SEG_RACING, SEG_RACING_SIZE);
    osWritebackDCacheAll();
    DMA((u8*) SEG_RACING, (u8*) SEG_RACING_ROM_START, SEG_RACING_ROM_SIZE);
    osInvalICache((void*) SEG_RACING, SEG_RACING_SIZE);
    osInvalDCache((void*) SEG_RACING, SEG_RACING_SIZE);
}

void init_overkart(void) {
    bzero((void*) SEG_OVERKART, SEG_OVERKART_SIZE);
    osWritebackDCacheAll();
    DMA((u8*) SEG_OVERKART, (u8*) SEG_OVERKART_ROM_START, SEG_OVERKART_ROM_SIZE);
    osInvalICache((void*) SEG_OVERKART, SEG_OVERKART_SIZE);
    osInvalDCache((void*) SEG_OVERKART, SEG_OVERKART_SIZE);
    loadBigFont();
}

void DMA(u8* dest, u8* romAddr, size_t size) {

    osInvalDCache(dest, size);
    while (size > 0x100) {
        osPiStartDma(&gDmaIoMesg, 0, 0, (uintptr_t) romAddr, dest, 0x100, &gDmaMesgQueue);
        osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, 1);
        size -= 0x100;
        romAddr += 0x100;
        dest += 0x100;
    }
    if (size != 0) {
        osPiStartDma(&gDmaIoMesg, 0, 0, (uintptr_t) romAddr, dest, size, &gDmaMesgQueue);
        osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, 1);
    }
}

/**
 * Setup main segments and framebuffers.
 */
void InitializeSystemWorks(void) {
    UNUSED u32 pad[2];
    ptrdiff_t commonCourseDataSize; // Compressed mio0 size
    uintptr_t textureSegSize;
    ptrdiff_t textureSegStart;
    uintptr_t allocatedMemory;
    UNUSED s32 unknown_padding;

    init_mkcode();
    init_overkart();
    LastMemoryPointer = SEG_RACING;
    SetSegment(0, (void*) SEG_START);

    // Memory pool size of 0xAB630
    initial_free_memory(MEMORY_POOL_START, MEMORY_POOL_END);

    AllocDepthBuffer();

    // Initialize trig tables segment
    osInvalDCache((void*) TRIG_TABLES, TRIG_TABLES_SIZE);
    osPiStartDma(&gDmaIoMesg, 0, 0, TRIG_TABLES_ROM_START, (void*) TRIG_TABLES, TRIG_TABLES_SIZE, &gDmaMesgQueue);
    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);

    SetSegment(2, (void*) load_data(SEG_DATA_START, SEG_DATA_END));

    commonCourseDataSize = COMMON_TEXTURES_SIZE;
    commonCourseDataSize = ALIGN16(commonCourseDataSize);

#ifdef AVOID_UB
    textureSegStart = (ptrdiff_t) SEG_RACING - commonCourseDataSize;
#else
    textureSegStart = SEG_RACING - commonCourseDataSize;
#endif
    osPiStartDma(&gDmaIoMesg, 0, 0, COMMON_TEXTURES_ROM_START, (void*) textureSegStart, commonCourseDataSize,
                 &gDmaMesgQueue);
    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);

    textureSegSize = *(uintptr_t*) (textureSegStart + 4);
    textureSegSize = ALIGN16(textureSegSize);
    allocatedMemory = FreeMemoryPointer;
    decodeMIO0((u8*) textureSegStart, (u8*) allocatedMemory);
    SetSegment(0xD, (void*) allocatedMemory);

    FreeMemoryPointer += textureSegSize;

    // Common course data does not get reloaded when the race state resets.
    // Therefore, only reset the memory ptr to after the common course data.
    StaticMemoryPointer = FreeMemoryPointer;
    VertexMemoryPointer = VERTEX_MEMORY_TOP;
    ObjectMemoryPointer = (uintptr_t) ok_ObjectPoolStart;
}

/**
 * @brief
 *
 */
void boot_sequence(void) {
    g_NewSequenceMode = 0; // = START_MENU_FROM_QUIT?
    SoftwareBlanking(0);
}

void race_sequence(void) {
    s16 i;
    u16 rotY;

    gMatrixCount = 0;
    effectcount = 0;
    if (pause_flag != 0) {
        pause_sequence();
    }
    if (g_fadeOutFlag != 0) {
        fadeout_sequence();
        return;
    }

    gameCodeDefault();

    if (videoframe >= 6) {
        videoframe = 5;
    }
    if (videoframe < 0) {
        videoframe = 1;
    }
    setup_perspective();

    switch (g_ScreenSplitA) {
        case SCREEN_MODE_1P:
            framerate = 2;
            ApplyRaceTempo();
            key_data_control();
            if (pause_flag == 0) {
                for (i = 0; i < framerate; i++) {
                    if (time_flag) {
                        g_gameTimer += COURSE_TIMER_ITER;
                    }
                    collision_kart_to_kart();
                    collision_object_to_kart();
                    drive_stick_control();
                    CameraCheckFunc(gPlayerOneCopy, camera1, 0);
                    kartpos_control_full();
                    enemy_control();
                    KWVideoFramesYori();
                    object_strategy();
                    map_strategy();
                    race_control();
                }
                ken_startegy();
            }
            kwgameframesyori();
            videoframe = 0;
            SetGraphCPUTime(LEVEL_SCRIPT_EXECUTE);
            g_ScreenCounter = 0;
            draw_full_screen();
            if (!g_DebugMode) {
                g_DebugSection = false;
            } else {
                if (g_DebugSection) {

                    if ((gControllerOne->ButtonPressed & R_TRIG) && (gControllerOne->ButtonHeld & A_BUTTON) &&
                        (gControllerOne->ButtonHeld & B_BUTTON)) {
                        g_DebugSection = false;
                    }

                    rotY = camera1->camera_direction[1];
                    gDebugPathCount = D_800DC5EC->camera_point;
                    if (rotY < DEGREES(45)) {
                        kwfprintud(40, 100, "SOUTH  ", gDebugPathCount);
                    } else if (rotY < DEGREES(135)) {
                        kwfprintud(40, 100, "EAST   ", gDebugPathCount);
                    } else if (rotY < DEGREES(225)) {
                        kwfprintud(40, 100, "NORTH  ", gDebugPathCount);
                    } else if (rotY < DEGREES(315)) {
                        kwfprintud(40, 100, "WEST   ", gDebugPathCount);
                    } else {
                        kwfprintud(40, 100, "SOUTH  ", gDebugPathCount);
                    }

                } else {
                    if ((gControllerOne->ButtonPressed & L_TRIG) && (gControllerOne->ButtonHeld & A_BUTTON) &&
                        (gControllerOne->ButtonHeld & B_BUTTON)) {
                        g_DebugSection = true;
                    }
                }
            }
            break;

        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            if (g_courseID == COURSE_DK_JUNGLE) {
                framerate = 3;
            } else {
                framerate = 2;
            }
            ApplyRaceTempo();
            if (pause_flag == 0) {
                for (i = 0; i < framerate; i++) {
                    if (time_flag != 0) {
                        g_gameTimer += COURSE_TIMER_ITER;
                    }
                    collision_kart_to_kart();
                    collision_object_to_kart();
                    drive_stick_control();
                    CameraCheckFunc(gPlayerOneCopy, camera1, 0);
                    kartpos_control_ru();
                    CameraCheckFunc(gPlayerTwoCopy, camera2, 1);
                    kartpos_control_ld();
                    enemy_control();
                    KWVideoFramesYori();
                    object_strategy();
                    map_strategy();
                    race_control();
                }
                ken_startegy();
            }
            kwgameframesyori();
            SetGraphCPUTime(LEVEL_SCRIPT_EXECUTE);
            videoframe = 0;
            StoreSegments();
            InitRDP();
            if (g_ClearCFBFlag != 0) {
                g_ClearFramebuffer();
            }
            g_ScreenCounter = 0;
            if (win_kart == 0) {
                draw_right_screen();
                draw_left_screen();
            } else {
                draw_left_screen();
                draw_right_screen();
            }
            break;

        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:

            if (g_courseID == COURSE_DK_JUNGLE) {
                framerate = 3;
            } else {
                framerate = 2;
            }
            ApplyRaceTempo();

            if (pause_flag == 0) {
                for (i = 0; i < framerate; i++) {
                    if (time_flag != 0) {
                        g_gameTimer += COURSE_TIMER_ITER;
                    }
                    collision_kart_to_kart();
                    collision_object_to_kart();
                    drive_stick_control();
                    CameraCheckFunc(gPlayerOneCopy, camera1, 0);
                    kartpos_control_ru();
                    CameraCheckFunc(gPlayerTwoCopy, camera2, 1);
                    kartpos_control_ld();
                    enemy_control();
                    KWVideoFramesYori();
                    object_strategy();
                    map_strategy();
                    race_control();
                }
                ken_startegy();
            }
            SetGraphCPUTime(LEVEL_SCRIPT_EXECUTE);
            videoframe = (u16) 0;
            kwgameframesyori();
            StoreSegments();
            InitRDP();
            if (g_ClearCFBFlag != 0) {
                g_ClearFramebuffer();
            }
            g_ScreenCounter = 0;
            if (win_kart == 0) {
                draw_down_screen();
                draw_up_screen();
            } else {
                draw_up_screen();
                draw_down_screen();
            }

            break;

        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            if (g_playerCount == 3) {
                switch (g_courseID) {
                    case COURSE_BOWSER_CASTLE:
                    case COURSE_MOO_MOO_FARM:
                    case COURSE_SKYSCRAPER:
                    case COURSE_DK_JUNGLE:
                        framerate = 3;
                        break;
                    default:
                        framerate = 2;
                        break;
                }
            } else {
                // Four players
                switch (g_courseID) {
                    case COURSE_BLOCK_FORT:
                    case COURSE_DOUBLE_DECK:
                    case COURSE_BIG_DONUT:
                        framerate = 2;
                        break;
                    case COURSE_DK_JUNGLE:
                        framerate = 4;
                        break;
                    default:
                        framerate = 3;
                        break;
                }
            }
            ApplyRaceTempo();
            if (pause_flag == 0) {
                for (i = 0; i < framerate; i++) {
                    if (time_flag != 0) {
                        g_gameTimer += COURSE_TIMER_ITER;
                    }
                    collision_kart_to_kart();
                    collision_object_to_kart();
                    drive_stick_control();
                    CameraCheckFunc(gPlayerOneCopy, camera1, 0);
                    kartpost_control_1P();
                    CameraCheckFunc(gPlayerTwo, camera2, 1);
                    kartpost_control_2P();
                    CameraCheckFunc(gPlayerThree, camera3, 2);
                    kartpos_control_3P();
                    CameraCheckFunc(gPlayerFour, camera4, 3);
                    kartpos_control_4P();
                    enemy_control();
                    KWVideoFramesYori();
                    object_strategy();
                    map_strategy();
                    race_control();
                }
                ken_startegy();
            }
            kwgameframesyori();
            videoframe = 0;
            SetGraphCPUTime(LEVEL_SCRIPT_EXECUTE);
            StoreSegments();
            InitRDP();
            if (g_ClearCFBFlag != 0) {
                g_ClearFramebuffer();
            }
            g_ScreenCounter = 0;
            if (win_kart == 0) {
                draw_up_right_screen();
                draw_down_left_screen();
                draw_down_right_screen();
                draw_up_left_screen();
            } else if (win_kart == 1) {
                draw_up_left_screen();
                draw_down_left_screen();
                draw_down_right_screen();
                draw_up_right_screen();
            } else if (win_kart == 2) {
                draw_up_left_screen();
                draw_up_right_screen();
                draw_down_right_screen();
                draw_down_left_screen();
            } else {
                draw_up_left_screen();
                draw_up_right_screen();
                draw_down_left_screen();
                draw_down_right_screen();
            }
            break;
    }

    if (!g_DebugMode) {
        g_DispTimeFlag = 0;
    } else {
        if (g_DispTimeFlag) {
            DisplayProcTime();
            if ((!(gControllerOne->ButtonHeld & L_TRIG)) && (gControllerOne->ButtonHeld & R_TRIG) &&
                (gControllerOne->ButtonPressed & B_BUTTON)) {
                g_DispTimeFlag = 0;
            }
        } else {
            if ((!(gControllerOne->ButtonHeld & L_TRIG)) && (gControllerOne->ButtonHeld & R_TRIG) &&
                (gControllerOne->ButtonPressed & B_BUTTON)) {
                g_DispTimeFlag = 1;
            }
        }
    }
    make_border();
    kwdisplay_toppri();
    MiniMapDrawDefault();
#if DVDL
    display_dvdl();
#endif
    gDPFullSync(gDisplayListHead++);
    gSPEndDisplayList(gDisplayListHead++);
}

/**
 * mk64's game loop depends on a series of states.
 * It runs a wide branching series of code based on these states.
 * State 1) Clear framebuffer
 * State 2) Run menus
 * State 3) Process race related logic
 * State 4) Ending sequence
 * State 5) Credits
 *
 * Note that the state doesn't flip-flop at random but is permanent
 * until the state changes (ie. Exit menus and start a race).
 */

void execute_sequence(void) {
#if DVDL
    if ((gControllerOne->ButtonHeld & L_TRIG) && (gControllerOne->ButtonHeld & R_TRIG) &&
        (gControllerOne->ButtonHeld & Z_TRIG) && (gControllerOne->ButtonHeld & A_BUTTON)) {
        g_NewSequenceMode = CREDITS_SEQUENCE;
    } else if ((gControllerOne->ButtonHeld & L_TRIG) && (gControllerOne->ButtonHeld & R_TRIG) &&
               (gControllerOne->ButtonHeld & Z_TRIG) && (gControllerOne->ButtonHeld & B_BUTTON)) {
        g_NewSequenceMode = ENDING;
    }
#endif

    switch (gGamestate) {
        case 7:
            boot_sequence();
            break;
        case START_MENU_FROM_QUIT:
        case MAIN_MENU_FROM_QUIT:
        case PLAYER_SELECT_MENU_FROM_QUIT:
        case COURSE_SELECT_MENU_FROM_QUIT:
            // Display black
            osViBlack(0);
            UpdateController();
            begin_drawing();
            doGameSelect(gDynamicP);
#if DVDL
            display_dvdl();
#endif
            break;
        case RACING:
            race_sequence();
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

void yield_RSP_task(void) {
    if (gActiveSPTask->task.t.type == M_GFXTASK) {
        gActiveSPTask->state = SPTASK_STATE_INTERRUPTED;
        osSpTaskYield();
    }
}

void get_task_message(void) {
    UNUSED s32 pad;
    struct SPTask* spTask;

    while (osRecvMesg(&gSPTaskMesgQueue, (OSMesg*) &spTask, OS_MESG_NOBLOCK) != -1) {
        spTask->state = SPTASK_STATE_NOT_STARTED;
        switch (spTask->task.t.type) {
            case 2:
                sNextAudioSPTask = spTask;
                break;
            case 1:
                sNextDisplaySPTask = spTask;
                break;
        }
    }

    if (sCurrentAudioSPTask == NULL && sNextAudioSPTask != NULL) {
        sCurrentAudioSPTask = sNextAudioSPTask;
        sNextAudioSPTask = NULL;
    }
    if (sCurrentDisplaySPTask == NULL && sNextDisplaySPTask != NULL) {
        sCurrentDisplaySPTask = sNextDisplaySPTask;
        sNextDisplaySPTask = NULL;
    }
}

void set_client(s32 index, struct VblankHandler* handler, OSMesgQueue* queue, OSMesg* msg) {
    handler->queue = queue;
    handler->msg = msg;
    switch (index) {
        case 1:
            gVblankHandler1 = handler;
            break;
        case 2:
            gVblankHandler2 = handler;
            break;
    }
}

void start_gfx_task(void) {
    if (gActiveSPTask == NULL && sCurrentDisplaySPTask != NULL &&
        sCurrentDisplaySPTask->state == SPTASK_STATE_NOT_STARTED) {
        SetGraphRCPTime(TASKS_QUEUED);
        start_RSP_task(M_GFXTASK);
    }
}

void interrupt_sequence(void) {
    timer += V_BlANK_TIMER_ITER;
    videoframe++;

    get_task_message();

    // First try to kick off an audio task. If the gfx task is currently
    // running, we need to asynchronously interrupt it -- rsp_done_sequence
    // will pick up on what we're doing and start the audio task for us.
    // If there is already an audio task running, there is nothing to do.
    // If there is no audio task available, try a gfx task instead.
    if (sCurrentAudioSPTask != NULL) {
        if (gActiveSPTask != NULL) {
            yield_RSP_task();
        } else {
            SetAudioRCPTime();
            start_RSP_task(M_AUDTASK);
        }
    } else {
        if (gActiveSPTask == NULL && sCurrentDisplaySPTask != NULL &&
            sCurrentDisplaySPTask->state != SPTASK_STATE_FINISHED) {
            SetGraphRCPTime(TASKS_QUEUED);
            start_RSP_task(M_GFXTASK);
        }
    }

/* This is where I would put my rumble code... If I had any. */
#if ENABLE_RUMBLE
    rumble_thread_update_vi();
#endif

    if (gVblankHandler1 != NULL) {
        osSendMesg(gVblankHandler1->queue, gVblankHandler1->msg, OS_MESG_NOBLOCK);
    }
    if (gVblankHandler2 != NULL) {
        osSendMesg(gVblankHandler2->queue, gVblankHandler2->msg, OS_MESG_NOBLOCK);
    }
}

void rdp_done_sequence(void) {
    // Gfx SP task is completely done.
    if (sCurrentDisplaySPTask->msgqueue != NULL) {
        osSendMesg(sCurrentDisplaySPTask->msgqueue, sCurrentDisplaySPTask->msg, OS_MESG_NOBLOCK);
    }
    SetGraphRCPTime(RDP_COMPLETE);
    sCurrentDisplaySPTask->state = SPTASK_STATE_FINISHED_DP;
    sCurrentDisplaySPTask = NULL;
}

void rsp_done_sequence(void) {
    struct SPTask* curSPTask = gActiveSPTask;

    gActiveSPTask = NULL;

    if (curSPTask->state == SPTASK_STATE_INTERRUPTED) {
        // interrupt_sequence tried to start an audio task while there was already a
        // gfx task running, so it had to interrupt the gfx task. That interruption
        // just finished.
        if (osSpTaskYielded((OSTask*) curSPTask) == 0) {
            // The gfx task completed before we had time to interrupt it.
            // Mark it finished, just like below.
            curSPTask->state = SPTASK_STATE_FINISHED;
            SetGraphRCPTime(RSP_COMPLETE);
        }
        // Start the audio task, as expected by interrupt_sequence.
        SetAudioRCPTime();
        start_RSP_task(M_AUDTASK);
    } else {
        curSPTask->state = SPTASK_STATE_FINISHED;
        if (curSPTask->task.t.type == M_AUDTASK) {
            // After audio tasks come gfx tasks.
            SetAudioRCPTime();
            if (sCurrentDisplaySPTask != NULL) {
                if (sCurrentDisplaySPTask->state != SPTASK_STATE_FINISHED) {
                    if (sCurrentDisplaySPTask->state != SPTASK_STATE_INTERRUPTED) {
                        SetGraphRCPTime(TASKS_QUEUED);
                    }
                    start_RSP_task(M_GFXTASK);
                }
            }
            sCurrentAudioSPTask = NULL;
            if (curSPTask->msgqueue != NULL) {
                osSendMesg(curSPTask->msgqueue, curSPTask->msg, OS_MESG_NOBLOCK);
            }
        } else {
            // The SP process is done, but there is still a Display Processor notification
            // that needs to arrive before we can consider the task completely finished and
            // null out sCurrentDisplaySPTask. That happens in rdp_done_sequence.
            SetGraphRCPTime(RSP_COMPLETE);
        }
    };
}

void mainproc(UNUSED void* arg0) {
    s32 i;
    u64* framebuffer1;
    OSMesg msg;
    UNUSED s32 pad[4];

    g_CfbPtrs[0] = (u16*) &gFramebuffer0;
    g_CfbPtrs[1] = (u16*) &gFramebuffer1;
    g_CfbPtrs[2] = (u16*) &gFramebuffer2;

    // Clear framebuffer.
    framebuffer1 = (u64*) &gFramebuffer1;
    for (i = 0; i < 19200; i++) {
        framebuffer1[i] = 0;
    }
    setup_messagequeue();
    InitializeSystemWorks();

    SysCreateThread(&gAudioThread, 4, &audioproc, 0, gAudioThreadStack + ARRAY_COUNT(gAudioThreadStack), 20);
    osStartThread(&gAudioThread);

    SysCreateThread(&gGameLoopThread, 5, &gameproc, 0, gGameLoopThreadStack + ARRAY_COUNT(gGameLoopThreadStack),
                  10);
    osStartThread(&gGameLoopThread);

    while (true) {
        osRecvMesg(&gIntrMesgQueue, &msg, OS_MESG_BLOCK);
        switch ((u32) msg) {
            case MESG_VI_VBLANK:
                interrupt_sequence();
                break;
            case MESG_SP_COMPLETE:
                rsp_done_sequence();
                break;
            case MESG_DP_COMPLETE:
                rdp_done_sequence();
                break;
            case MESG_START_GFX_SPTASK:
                start_gfx_task();
                break;
        }
    }
}

void initial_gameselect_sequence(void) {
    InitMKCode();
    g_ScreenSplitA = SCREEN_MODE_1P;
    initital_perspective();
}

void initial_kartselect_sequence(void) {
    InitMKCode();
    g_ScreenSplitA = SCREEN_MODE_1P;
    initital_perspective();
}

void initial_mapselect_sequence(void) {
    InitMKCode();
    g_ScreenSplitA = SCREEN_MODE_1P;
    initital_perspective();
}

void initial_title_sequence(void) {
    InitMKCode();
    g_ScreenSplitA = SCREEN_MODE_1P;
    initital_perspective();
}

/**
 * Sets courseId to NULL if
 *
 *
 */
void initialize_sequence(void) {
    switch (gGamestate) {
        case START_MENU_FROM_QUIT:
            initial_title_sequence();
            g_loadedcourseFlag = COURSE_NULL;
            break;
        case MAIN_MENU_FROM_QUIT:
            initial_gameselect_sequence();
            g_loadedcourseFlag = COURSE_NULL;
            break;
        case PLAYER_SELECT_MENU_FROM_QUIT:
            initial_kartselect_sequence();
            g_loadedcourseFlag = COURSE_NULL;
            break;
        case COURSE_SELECT_MENU_FROM_QUIT:
            initial_mapselect_sequence();
            g_loadedcourseFlag = COURSE_NULL;
            break;
        case RACING:
            InitialRaceSequence();
            break;
        case ENDING:
            g_loadedcourseFlag = COURSE_NULL;
            init_excode();
            initial_result_sequence();
            break;
        case CREDITS_SEQUENCE:
            g_loadedcourseFlag = COURSE_NULL;
            init_excode();
            InitializeEndingSequence();
            break;
    }
}

void gameproc(UNUSED void* arg) {
    osCreateMesgQueue(&gGfxVblankQueue, gGfxMesgBuf, 1);
    osCreateMesgQueue(&gGameVblankQueue, &gGameMesgBuf, 1);
    InitControllers();
    if (!resert_type) {
        initialize_first_once();
    }

    set_client(2, &gGameVblankHandler, &gGameVblankQueue, (OSMesg) OS_EVENT_SW2);
    // These variables track stats such as player wins.
    // In the event of a console reset, it remembers them.
    wincount2p = &pAppNmiBuffer[0]; // 2  u8's, tracks number of times player 1/2 won a VS race
    wincount3p =
        &pAppNmiBuffer[2]; // 9  u8's, 3x3, tracks number of times player 1/2/3   has placed in 1st/2nd/3rd in a VS race
    wincount4p = &pAppNmiBuffer[11]; // 12 u8's, 4x3, tracks number of times player 1/2/3/4 has placed in 1st/2nd/3rd
                                       // in a VS race
    battlewincount2p = &pAppNmiBuffer[23]; // 2  u8's, tracking number of Battle mode wins by player 1/2
    battlewincount3p = &pAppNmiBuffer[25]; // 3  u8's, tracking number of Battle mode wins by player 1/2/3
    battlewincount4p = &pAppNmiBuffer[28]; // 4  u8's, tracking number of Battle mode wins by player 1/2/3/4
    initial_gameframe();
    readControllers();
    Nai_InitInterface();

    while (true) {
        Nas_AudioInput();

        // Update the gamestate if it has changed (racing, menus, credits, etc.).
        if (g_NewSequenceMode != gGamestate) {
            gGamestate = g_NewSequenceMode;
            initialize_sequence();
        }
        SetGraphCPUTime(THREAD5_START);
        begin_gameframe();
        readControllers();
        execute_sequence();
        end_drawing();
        flush_displaylist();
        allRunDefault();
    }
}

/**
 * Sound processing thread. Runs at 50 or 60 FPS according to osTvType.
 */
void audioproc(UNUSED void* arg) {
    UNUSED u32 unused[3];
    Nas_InitAudio();
    osCreateMesgQueue(&sSoundMesgQueue, sSoundMesgBuf, ARRAY_COUNT(sSoundMesgBuf));
    set_client(1, &sSoundVblankHandler, &sSoundMesgQueue, (OSMesg) 512);

    while (true) {
        OSMesg msg;
        struct SPTask* spTask;

        osRecvMesg(&sSoundMesgQueue, &msg, OS_MESG_BLOCK);

        SetAudioCPUTime();

        spTask = Nas_AudioMain();
        if (spTask != NULL) {
            set_RSP_task(spTask);
        }
        SetAudioCPUTime();
    }
}

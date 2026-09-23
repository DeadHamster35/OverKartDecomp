#include <defines.h>
#include <mk64.h>
#include <course.h>

#include "spawn_players.h"
#include "code_800029B0.h"
#include "kart_attributes.h"
#include "memory.h"
#include "path.h"
#include "buffers.h"
#include "kart_dma.h"
#include "camera.h"
#include "math_util.h"
#include "player_controller.h"
#include "code_80057C60.h"
#include "collision.h"
#include "render_courses.h"
#include "replays.h"
#include "cpu_vehicles_camera_path.h"
#include "render_player.h"
#include "podium_ceremony_actors.h"
#include "main.h"
#include "menus.h"
#include "render_player.h"
#include "menu_items.h"
#include "effects.h"
#include "decode.h"

f32 GridX[8];
f32 GridZ[8];
UNUSED f32 GridY[8];
s16 Kartrank[8];
f32 AcceleCount[8];
f32 WaterlevelPlayer[8];
s32 SterrCounter[8];
s32 GreenTimer[8];
s16 EchoFlag[8];
// Shadows values from CurrentPathID, but is an array
u16 JugemuLine[8];
// Shadows values from NearestPathPointByPlayerId, but is an array
s16 PlayerPathPointCopy[8];
s16 OffroadFlagPlayer1[8];
s16 TireHight;
UNUSED s32 D_80165348[29];
Player* HitPtr[8];

bool AcceleOnoffFlag[8];
s32 AcceleIncFlag[8];
s32 AcceleTime[8];
s32 AcceleFlag[8];
bool PowerBandAccele[8];
s32 PowerBandTimer[8];

bool BrakeOnoffFlag[8];
s32 BrakeIncFlag[8];
s32 BrakeTimer[8];
s32 BrakeFlag[8];
bool FullBrakeAccele[8];
s32 FullBrakeTimer[8];

s16 CPUChooseCharacters[7];

s16 D_8016556E;
s16 SelectDMAKart;
s16 SelectReardmaKart;
s16 SelectNondmaKart;
s16 SelectRearNondmaKart;
s16 SelectViewKart;
s16 SelectRearviewKart;
s16 SelectBumpKart;
s16 SelectRearbumpKart;
s16 SelectViewObj;
s16 SelectRearviewObj;

// arg4 is Height? Or something like that?
void InitializePlayer(Player* Player, s8 playerIndex, f32 startingRow, f32 StartingColumn, f32 arg4, f32 arg5,
                  u16 CharacterId, s16 playerType) {
    f32 ret;
    s8 idx;

    Player->Flag = PLAYER_INACTIVE;
    Player->Force = 0;
    Player->Kart = CharacterId;
    Player->Talk = 0;
    Player->Mass = gKartFrictionTable[Player->Kart];
    Player->Radius = CharRadiusTbl[Player->Kart];
    Player->Gravity = Gravity1[Player->Kart];

    switch (GameMode) {
        case GRAND_PRIX:
        case VERSUS:
            Player->FGrip = D_800E2400[RaceClass][Player->Kart];
            Player->RGrip = D_800E24B4[RaceClass][Player->Kart];
            Player->MaxPower = D_800E2568[RaceClass][Player->Kart];
            Player->AccMaxcount = TopSpeedTable[RaceClass][Player->Kart];
            break;

        // Uses 100CC values
        case TIME_TRIALS:
            Player->FGrip = D_800E2400[CC_100][Player->Kart];
            Player->RGrip = D_800E24B4[CC_100][Player->Kart];
            Player->MaxPower = D_800E2568[CC_100][Player->Kart];
            Player->AccMaxcount = TopSpeedTable[CC_100][Player->Kart];
            break;

        case BATTLE:
            Player->FGrip = D_800E2400[CC_BATTLE][Player->Kart];
            Player->RGrip = D_800E24B4[CC_BATTLE][Player->Kart];
            Player->MaxPower = D_800E2568[CC_BATTLE][Player->Kart];
            Player->AccMaxcount = TopSpeedTable[CC_BATTLE][Player->Kart];
            break;
    }

    Player->Position[0] = startingRow;
    ret = CheckHight(startingRow, arg4 + 50.0f, StartingColumn) + Player->Radius;
    Player->Position[2] = StartingColumn;
    Player->Position[1] = ret;
    Player->OldPosition[0] = startingRow;
    Player->OldPosition[1] = ret;

    EnemyTenukiY[playerIndex] = ret;

    Player->Direction[0] = 0;
    Player->OldPosition[2] = StartingColumn;
    Player->UpVector[1] = 1.0f;
    Player->UpVector[0] = 0.0f;
    Player->UpVector[2] = 0.0f;
    Player->Velocity[0] = 0.0f;
    Player->Velocity[1] = 0.0f;
    Player->Velocity[2] = 0.0f;
    Player->Direction[1] = arg5;
    Player->Direction[2] = 0;
    Player->Sterracc = 0;
    Player->Status = 0;

    Player->SlipFlag = 0;
    Player->Slipang = 0;
    Player->Nowsterr = 0;
    Player->ShakeFlag = 0;
    Player->Point = 0;
    Player->rap = -1;
    Player->Force = 0.0f;
    Player->Grip = 0.0f;
    Player->Speed = 0.0f;
    Player->Ground = 0.0f;
    Player->Flag = playerType;
    Player->JugemuFlag = 0;
    Player->WaterFlag = 0;
    Player->Hitcount = 0;
    Player->SlipstreamTimer = 0;
    Player->MaxaddPower = 0.0f;
    Player->PowerDown = 0.0f;
    Player->PowerCont = 0.0f;
    Player->Accelcount = 0.0f;
    Player->BrakeTime = 0.0f;
    Player->sus.JumpFirstspeed = 0.0f;
    Player->HandlingFlag = 0;
    Player->HitFlag = 0;
    Player->Weapon = 0;
    Player->Erase = ALPHA_MAX;

    Player->Kartroll = 0;
    Player->Kartpitch = 0;
    Player->sus.Flag = 0;
    Player->sus.Flag1 = 0;
    Player->sus.Broken = 0;
    Player->sus.JumpTimer = 0;
    Player->sus.GachonTimer = 0;
    Player->sus.BrokenTimer = 0;
    Player->sus.BoundTimer = 0;
    Player->sus.SwingTimer = 0;
    Player->sus.SwingFlag = 0;
    Player->sus.Bound = 0;
    Player->sus.Swing = 0;

    Player->KageAng = 0;
    Player->Sterrangle = 0;
    Player->Slipcount = 0;
    Player->SterrCount = 0;
    Player->OffsetsterrFlag = 0;
    Player->OldDirection = 0;
    Player->ThunderTimer = 0;
    Player->SpinTimer = 0;
    Player->OffsetsterrTimer = 0;
    Player->Slipang = 0;
    Player->Jumpcount = 0;
    Player->JugemuTimer = 0;
    Player->JugemuFlag = 0;
    Player->TurboTimer = 0;
    Player->WaterFlag = 0;
    Player->BombTimer = 0;
    Player->SlipstreamTimer = 0;
    Player->Hitcount = 0;
    Player->ChasespSterr = 0;
    Player->DriftCount = 0;
    Player->Chasepoint = 0;
    Player->StopsetrrCount = 0;
    Player->PowerstackCount = 0;
    Player->HitSterr = 0;
    Player->RolioverTime = 0;
    Player->BrokenTimer = 0;
    Player->DriftTurboTimer = 0;
    Player->TcolCount = 0;
    Player->Wallhitcount = 0;

    Player->Offsetsize = 1.0f;
    Player->sus.JumpFirstspeed = 1.0f;

    Player->GravityXz[0] = 0.0f;
    Player->GravityXz[1] = 0.0f;
    Player->GravityXz[2] = 0.0f;
    Player->TurboPower = 0.0f;
    Player->sus.Position = 0.0f;
    Player->sus.Accele = 0.0f;
    Player->sus.JumpL = 0.0f;
    Player->sus.JmpAccL = 0.0f;
    Player->sus.SwingFirstspeed = 0.0f;
    Player->sus.GachonFirstspeed = 0.0f;
    Player->sus.BoundFirstspeed = 0.0f;
    Player->sus.BrokenFirstspeed = 0.0f;
    Player->sus.YGrv = 0.0f;
    Player->FGrip = 0.0f;
    Player->RGrip = 0.0f;
    Player->Force = 0.0f;
    Player->Grip = 0.0f;
    Player->Speed = 0.0f;
    Player->Taco = 0.0f;
    Player->Accelcount = 0.0f;
    Player->PowerDown = 0.0f;
    Player->Power = 0.0f;
    Player->OffsetsterrSpeed = 0.0f;
    Player->SlipstreamPower = 0.0f;
    Player->MaxaddPower = 0.0f;
    Player->Jump = 0.0f;
    Player->JmpAcc = 0.0f;
    Player->JmpSpeed = 0.0f;
    Player->PowerCont = 0.0f;
    Player->Shake = 0.0f;
    Player->MidFront = 0.0f;
    Player->MidRear = 0.0f;
    Player->AllrearGrip = 0.0f;
    Player->BrakeTime = 0.0f;
    Player->MaxPower = 0.0f;
    Player->SparkX = 0.0f;
    Player->SparkZ = 0.0f;
    Player->OldSpeed = 0.0f;
    Player->MidLeft = 0.0f;
    Player->MidRight = 0.0f;

    idx = playerIndex;

    OldDir[0][idx] = 0;
    OldDir[1][idx] = 0;
    OldDir[2][idx] = 0;
    OldDir[3][idx] = 0;
    OldDeg[0][idx] = 0;
    OldDeg[1][idx] = 0;
    OldDeg[2][idx] = 0;
    OldDeg[3][idx] = 0;
    CompelFlag[0][idx] = 0;
    CompelFlag[1][idx] = 0;
    CompelFlag[2][idx] = 0;
    CompelFlag[3][idx] = 0;
    DrawNumber[0][idx] = 0;
    DrawNumber[1][idx] = 0;
    DrawNumber[2][idx] = 0;
    DrawNumber[3][idx] = 0;

    AcceleTime[idx] = 0;
    AcceleFlag[idx] = 0;
    PowerBandAccele[idx] = false;
    PowerBandTimer[idx] = 0;
    BrakeTimer[idx] = 0;
    BrakeFlag[idx] = 0;
    FullBrakeAccele[idx] = false;
    FullBrakeTimer[playerIndex] = 0;
    D_8018D900[0] = 0;

    GreenTimer[playerIndex] = 0;
    SterrCounter[playerIndex] = 0;
    D_80165020[playerIndex] = 0;
    OldVelocity[playerIndex][0] = 0.0f;
    OldVelocity[playerIndex][1] = 0.0f;
    OldVelocity[playerIndex][2] = 0.0f;
    AcceleCount[playerIndex] = 0.0f;
    WaterlevelPlayer[playerIndex] = 0.0f;
    AcceleOnoffFlag[playerIndex] = 0;
    AcceleIncFlag[playerIndex] = 0;
    BrakeOnoffFlag[playerIndex] = 0;
    BrakeIncFlag[playerIndex] = 0;
    TireHight = 0;

    Player->TireFL.Status = 0;
    Player->TireFR.Status = 0;
    Player->TireRL.Status = 0;
    Player->TireRR.Status = 0;

    Player->TireFL.LastAxis = 0;
    Player->TireFR.LastAxis = 0;
    Player->TireRL.LastAxis = 0;
    Player->TireRR.LastAxis = 0;

    Player->TireFL.LastPointer = 0;
    Player->TireFR.LastPointer = 0;
    Player->TireRL.LastPointer = 0;
    Player->TireRR.LastPointer = 0;

    Player->TireFR.Dummy = 0;
    Player->TireFL.Dummy = 0;
    Player->TireRL.Dummy = 0;
    Player->TireRR.Dummy = 0;

    Player->Bump.FlagXY = 0;
    Player->Bump.FlagYZ = 0;
    Player->Bump.FlagZX = 0;
    Player->Bump.LastXY = 0;
    Player->Bump.LastYZ = 0;
    Player->Bump.LastZX = 0;

    Player->TireFL.Position[0] = 0.0f;
    Player->TireFL.Position[1] = 0.0f;
    Player->TireFL.Position[2] = 0.0f;

    Player->TireFR.Position[0] = 0.0f;
    Player->TireFR.Position[1] = 0.0f;
    Player->TireFR.Position[2] = 0.0f;

    Player->TireRL.Position[0] = 0.0f;
    Player->TireRL.Position[1] = 0.0f;
    Player->TireRL.Position[2] = 0.0f;

    Player->TireRR.Position[0] = 0.0f;
    Player->TireRR.Position[1] = 0.0f;
    Player->TireRR.Position[2] = 0.0f;

    Player->TireFL.Height = 0.0f;
    Player->TireFR.Height = 0.0f;
    Player->TireRL.Height = 0.0f;
    Player->TireRR.Height = 0.0f;

    Player->Bump.DistanceXY = 0.0f;
    Player->Bump.DistanceYZ = 0.0f;
    Player->Bump.DistanceZX = 0.0f;
    Player->Bump.BumpXY[0] = 0.0f;
    Player->Bump.BumpXY[1] = 0.0f;
    Player->Bump.BumpXY[2] = 0.0f;
    Player->Bump.BumpYZ[0] = 0.0f;
    Player->Bump.BumpYZ[1] = 0.0f;
    Player->Bump.BumpYZ[2] = 0.0f;
    Player->Bump.BumpZX[0] = 0.0f;
    Player->Bump.BumpZX[1] = 0.0f;
    Player->Bump.BumpZX[2] = 0.0f;

    EchoFlag[playerIndex] = 0;
    D_8018CE10[playerIndex].unk_04[0] = 0.0f;
    D_8018CE10[playerIndex].unk_04[2] = 0.0f;
    InitialBumpKart(playerIndex);
    Effectinit(Player);
    AllkillBalloon(Player, playerIndex);
    if (GameMode == BATTLE) {
        InitAllPlayerBalloons(Player, playerIndex);
    }
    MakeAlignMatrix(Player->Align2, Player->UpVector[0], Player->UpVector[1], Player->UpVector[2],
                                 Player->Direction[1]);
    MakeAlignMatrix(Player->align, Player->UpVector[0], Player->UpVector[1], Player->UpVector[2],
                                 Player->Direction[1]);
}

void SetUpViewDistance(void) {
    switch (ScreenSplitA) {
        case SCREEN_MODE_1P:
            if (GameState == ENDING) {
                SelectViewKart = 0x898;
                SelectRearviewKart = 0;
                SelectBumpKart = 0x384;
                SelectRearbumpKart = 0;
                SelectNondmaKart = 0x384;
                SelectRearNondmaKart = 0;
                SelectDMAKart = 0x35C;
                SelectReardmaKart = 0;
                SelectViewObj = 0x1F4;
                SelectRearviewObj = 0;
            } else {
                SelectViewKart = 0x4B0;
                SelectRearviewKart = -0xA;
                SelectBumpKart = 0x384;
                SelectRearbumpKart = 0x32;
                SelectNondmaKart = 0x1F4;
                SelectRearNondmaKart = 0;
                SelectDMAKart = 0x15E;
                SelectReardmaKart = 0;
                SelectViewObj = 0xFA;
                SelectRearviewObj = 0;
            }
            break;

        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            if (GameMode == BATTLE) {
                SelectViewKart = 0x898;
                SelectRearviewKart = 0;
                SelectBumpKart = 0x320;
                SelectRearbumpKart = 0;
                SelectNondmaKart = 0x190;
                SelectRearNondmaKart = 0;
                SelectDMAKart = 0xC8;
                SelectReardmaKart = 0;
                SelectViewObj = 0xC8;
                SelectRearviewObj = 0;
            } else {
                SelectViewKart = 0x4B0;
                SelectRearviewKart = 0x32;
                SelectBumpKart = 0x320;
                SelectRearbumpKart = 0x32;
                SelectNondmaKart = 0x190;
                SelectRearNondmaKart = 0;
                SelectDMAKart = 0x96;
                SelectReardmaKart = 0;
                SelectViewObj = 0x96;
                SelectRearviewObj = 0;
            }
            break;

        default:
            if (GameMode == BATTLE) {
                SelectViewKart = 0x898;
                SelectRearviewKart = 0;
                SelectBumpKart = 0x320;
                SelectRearbumpKart = 0;
                SelectNondmaKart = 0x190;
                SelectRearNondmaKart = 0;
                SelectDMAKart = 0xC8;
                SelectReardmaKart = 0;
                SelectViewObj = 0xC8;
                SelectRearviewObj = 0;
            } else {
                SelectViewKart = 0x3E8;
                SelectRearviewKart = 0;
                SelectBumpKart = 0x258;
                SelectRearbumpKart = 0;
                SelectNondmaKart = 0x15E;
                SelectRearNondmaKart = 0;
                SelectDMAKart = 0x96;
                SelectReardmaKart = 0;
                SelectViewObj = 0x96;
                SelectRearviewObj = 0;
            }
            break;
    }
}

// typedef struct {
//     s32 unk00[8];
// } temp_80039DA4; // to be removed when data is compilable
// extern temp_80039DA4 D_800E4360;
// extern temp_80039DA4 D_800E4380;

void MakeRank(void) {
    s32 i;

    s32 sp2C[] = {
        7, 6, 5, 4, 3, 2, 1, 0,
    };

    s32 spC[] = {
        0, 1, 2, 3, 4, 5, 6, 7,
    };

    if (((CourseSelect == COURSE_ONE) && (D_8016556E == 0)) || (DemoFlag == 1) ||
        (TitleMode == DEBUG_MENU_OPTION_SELECTED)) {
        for (i = 0; i < NUM_PLAYERS; i++) {
            Kartrank[i] = sp2C[i];
        }
    } else {
        for (i = 0; i < NUM_PLAYERS; i++) {
            Kartrank[i] = spC[GPCurrentRaceRankByPlayerId[i]];
        }
    }
}

UNUSED f32 D_800E43A0 = 1.0f;
UNUSED s16 D_800E43A4 = 1;
UNUSED s16 D_800E43A8 = 0;

void MarioGP1P(f32* arg0, f32* arg1, f32 Arg2) {
    MakeRank();
    if (((CourseSelect == COURSE_ONE) && (D_8016556E == 0)) || (DemoFlag == 1) ||
        (TitleMode == DEBUG_MENU_OPTION_SELECTED)) {
        s16 rand;
        s16 i;

        do {
            rand = MakeRandomLimmit(7);
        } while (rand == CharacterSelections[0]);

        // Randomize PlayerTwo
        CPUChooseCharacters[0] = rand;

        // Chooses arr[0] as a fallback to prevent duplicating characters.
        // If it doesn't find the if, it will grab the final Index as a fallback.
        for (i = 1; i < 7; i++) {
            u16* arr = (u16*) CPUForPlayer[CharacterSelections[0]];
            if (rand == arr[i]) {
                CPUChooseCharacters[i] = arr[0];
            } else {
                CPUChooseCharacters[i] = arr[i];
            }
        }
    }

    D_8016556E = 0;
    if (DemoFlag == 1) {
        InitializePlayer(PlayerOneCopy, 0, arg0[Kartrank[0]], arg1[Kartrank[0]], Arg2, 32768.0f,
                     CharacterSelections[0], PLAYER_HUMAN_AND_CPU);
        InitializePlayer(PlayerTwo, 1, arg0[Kartrank[1]], arg1[Kartrank[1]], Arg2, 32768.0f, CPUChooseCharacters[0],
                     EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
        InitializePlayer(PlayerThree, 2, arg0[Kartrank[2]], arg1[Kartrank[2]], Arg2, 32768.0f, CPUChooseCharacters[1],
                     EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
        InitializePlayer(PlayerFour, 3, arg0[Kartrank[3]], arg1[Kartrank[3]], Arg2, 32768.0f, CPUChooseCharacters[2],
                     EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
        InitializePlayer(PlayerFive, 4, arg0[Kartrank[4]], arg1[Kartrank[4]], Arg2, 32768.0f, CPUChooseCharacters[3],
                     EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
        InitializePlayer(PlayerSix, 5, arg0[Kartrank[5]], arg1[Kartrank[5]], Arg2, 32768.0f, CPUChooseCharacters[4],
                     EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
        InitializePlayer(PlayerSeven, 6, arg0[Kartrank[6]], arg1[Kartrank[6]], Arg2, 32768.0f, CPUChooseCharacters[5],
                     EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
        InitializePlayer(PlayerEight, 7, arg0[Kartrank[7]], arg1[Kartrank[7]], Arg2, 32768.0f, CPUChooseCharacters[6],
                     EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
        OpeningFlag = 0;
    } else {
        InitializePlayer(PlayerOneCopy, 0, arg0[Kartrank[0]], arg1[Kartrank[0]] + 250.0f, Arg2, 32768.0f,
                     CharacterSelections[0], EXISTS | IS_GP_OPENING | NO_CONTROLS | IS_PLAYER);
        InitializePlayer(PlayerTwo, 1, arg0[Kartrank[1]], arg1[Kartrank[1]] + 250.0f, Arg2, 32768.0f,
                     CPUChooseCharacters[0], EXISTS | IS_GP_OPENING | NO_CONTROLS | IS_CPU_PLAYER);
        InitializePlayer(PlayerThree, 2, arg0[Kartrank[3]], arg1[Kartrank[2]] + 250.0f, Arg2, 32768.0f,
                     CPUChooseCharacters[1], EXISTS | IS_GP_OPENING | NO_CONTROLS | IS_CPU_PLAYER);
        InitializePlayer(PlayerFour, 3, arg0[Kartrank[2]], arg1[Kartrank[3]] + 250.0f, Arg2, 32768.0f,
                     CPUChooseCharacters[2], EXISTS | IS_GP_OPENING | NO_CONTROLS | IS_CPU_PLAYER);
        InitializePlayer(PlayerFive, 4, arg0[Kartrank[5]], arg1[Kartrank[4]] + 250.0f, Arg2, 32768.0f,
                     CPUChooseCharacters[3], EXISTS | IS_GP_OPENING | NO_CONTROLS | IS_CPU_PLAYER);
        InitializePlayer(PlayerSix, 5, arg0[Kartrank[4]], arg1[Kartrank[5]] + 250.0f, Arg2, 32768.0f,
                     CPUChooseCharacters[4], EXISTS | IS_GP_OPENING | NO_CONTROLS | IS_CPU_PLAYER);
        InitializePlayer(PlayerSeven, 6, arg0[Kartrank[7]], arg1[Kartrank[6]] + 250.0f, Arg2, 32768.0f,
                     CPUChooseCharacters[5], EXISTS | IS_GP_OPENING | NO_CONTROLS | IS_CPU_PLAYER);
        InitializePlayer(PlayerEight, 7, arg0[Kartrank[6]], arg1[Kartrank[7]] + 250.0f, Arg2, 32768.0f,
                     CPUChooseCharacters[6], EXISTS | IS_GP_OPENING | NO_CONTROLS | IS_CPU_PLAYER);
        OpeningFlag = 1;
    }
    SetUpViewDistance();
}

void TimeAttack(f32* arg0, f32* arg1, f32 Arg2) {
    InitializePlayer(PlayerFour, 3, arg0[2], arg1[2], Arg2, 32768.0f, CharacterSelections[0],
                 NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerFive, 4, arg0[3], arg1[3], Arg2, 32768.0f, CharacterSelections[0],
                 NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerSix, 5, arg0[4], arg1[4], Arg2, 32768.0f, CharacterSelections[0],
                 NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerSeven, 6, arg0[5], arg1[5], Arg2, 32768.0f, CharacterSelections[0],
                 NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerEight, 7, arg0[6], arg1[6], Arg2, 32768.0f, CharacterSelections[0],
                 NO_CONTROLS | IS_CPU_PLAYER);
    if (DemoFlag == 1) {
        InitializePlayer(PlayerOneCopy, 0, arg0[0], arg1[0], Arg2, 32768.0f, CharacterSelections[0],
                     PLAYER_HUMAN_AND_CPU);
        InitializePlayer(PlayerTwo, 1, arg0[0], arg1[0], Arg2, 32768.0f, CharacterSelections[0],
                     NO_CONTROLS | IS_CPU_PLAYER);
        InitializePlayer(PlayerThree, 2, arg0[1], arg1[1], Arg2, 32768.0f, CharacterSelections[0],
                     NO_CONTROLS | IS_CPU_PLAYER);
    } else if (ReplayFlag != 1) {
        InitializePlayer(PlayerOneCopy, 0, arg0[0], arg1[0], Arg2, 32768.0f, CharacterSelections[0],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        if (GhostStatus == 0) {
            InitializePlayer(PlayerTwo, 1, arg0[0], arg1[0], Arg2, 32768.0f, GhostKart,
                         EXISTS | IS_PLAYER | NO_CONTROLS | IS_GHOST);
        } else {
            InitializePlayer(PlayerTwo, 1, arg0[0], arg1[0], Arg2, 32768.0f, CharacterSelections[0],
                         NO_CONTROLS | IS_CPU_PLAYER);
        }
        if (RomghostStatus == 0) {
            InitializePlayer(PlayerThree, 2, arg0[0], arg1[0], Arg2, 32768.0f, RomghostKart,
                         EXISTS | IS_PLAYER | NO_CONTROLS | IS_GHOST);
        } else {
            InitializePlayer(PlayerThree, 2, arg0[1], arg1[1], Arg2, 32768.0f, CharacterSelections[0],
                         NO_CONTROLS | IS_CPU_PLAYER);
        }
    } else {
        InitializePlayer(PlayerOneCopy, 0, arg0[0], arg1[0], Arg2, 32768.0f, ReplayghostKart,
                     EXISTS | IS_PLAYER | NO_CONTROLS | IS_GHOST);
        if (ReplayghostStatus == 0) {
            InitializePlayer(PlayerTwo, 1, arg0[0], arg1[0], Arg2, 32768.0f, GhostKart,
                         EXISTS | IS_PLAYER | NO_CONTROLS | IS_GHOST);
        } else {
            InitializePlayer(PlayerTwo, 1, arg0[0], arg1[0], Arg2, 32768.0f, CharacterSelections[0],
                         NO_CONTROLS | IS_CPU_PLAYER);
        }
        if (RomghostStatus == 0) {
            InitializePlayer(PlayerThree, 2, arg0[0], arg1[0], Arg2, 32768.0f, RomghostKart,
                         EXISTS | IS_PLAYER | NO_CONTROLS | IS_GHOST);
        } else {
            InitializePlayer(PlayerThree, 2, arg0[1], arg1[1], Arg2, 32768.0f, CharacterSelections[0],
                         NO_CONTROLS | IS_CPU_PLAYER);
        }
    }
    OpeningFlag = 0;
    SetUpViewDistance();
}

void MarioGP2P(f32* arg0, f32* arg1, f32 Arg2) {
    MakeRank();
    if ((CourseSelect == COURSE_ONE) || (DemoFlag == 1) || (TitleMode == DEBUG_MENU_OPTION_SELECTED)) {
        s16 rand;
        s16 i;

        // @todo: this is a do-while Loop
    getRand:
        rand = MakeRandomLimmit(7);
        if (CharacterSelections[0] == rand) {
            goto getRand;
        }
        if (CharacterSelections[1] == rand) {
            goto getRand;
        }

        CPUChooseCharacters[0] = rand;

        for (i = 1; i < 6; i++) {
            u16* arr = (u16*) CPUForTwoPlayer[CharacterSelections[0]][CharacterSelections[1]];
            if (rand == arr[i]) {
                CPUChooseCharacters[i] = arr[0];
            } else {
                CPUChooseCharacters[i] = arr[i];
            }
        }
    }

    InitializePlayer(PlayerThree, 2, arg0[Kartrank[2]], arg1[Kartrank[2]], Arg2, 32768.0f, CPUChooseCharacters[0],
                 EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
    InitializePlayer(PlayerFour, 3, arg0[Kartrank[3]], arg1[Kartrank[3]], Arg2, 32768.0f, CPUChooseCharacters[1],
                 EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
    InitializePlayer(PlayerFive, 4, arg0[Kartrank[4]], arg1[Kartrank[4]], Arg2, 32768.0f, CPUChooseCharacters[2],
                 EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
    InitializePlayer(PlayerSix, 5, arg0[Kartrank[5]], arg1[Kartrank[5]], Arg2, 32768.0f, CPUChooseCharacters[3],
                 EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
    InitializePlayer(PlayerSeven, 6, arg0[Kartrank[6]], arg1[Kartrank[6]], Arg2, 32768.0f, CPUChooseCharacters[4],
                 EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
    InitializePlayer(PlayerEight, 7, arg0[Kartrank[7]], arg1[Kartrank[7]], Arg2, 32768.0f, CPUChooseCharacters[5],
                 EXISTS | IS_CPU_PLAYER | NO_CONTROLS);

    if (DemoFlag == 1) {
        InitializePlayer(PlayerOneCopy, 0, arg0[Kartrank[0]], arg1[Kartrank[0]], Arg2, 32768.0f,
                     CharacterSelections[0], PLAYER_HUMAN_AND_CPU);
    } else {
        InitializePlayer(PlayerOneCopy, 0, arg0[Kartrank[0]], arg1[Kartrank[0]], Arg2, 32768.0f,
                     CharacterSelections[0], EXISTS | NO_CONTROLS | IS_PLAYER);
    }
    if (DemoFlag == 1) {
        InitializePlayer(PlayerTwo, 1, arg0[Kartrank[1]], arg1[Kartrank[1]], Arg2, 32768.0f, CharacterSelections[1],
                     EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
    } else {
        InitializePlayer(PlayerTwo, 1, arg0[Kartrank[1]], arg1[Kartrank[1]], Arg2, 32768.0f, CharacterSelections[1],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
    }

    OpeningFlag = 0;
    SetUpViewDistance();
}

void VSRace2P(f32* arg0, f32* arg1, f32 Arg2) {
    InitializePlayer(PlayerThree, 2, arg0[1], arg1[1], Arg2, 32768.0f, CharacterSelections[0],
                 NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerFour, 3, arg0[2], arg1[2], Arg2, 32768.0f, CharacterSelections[0],
                 NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerFive, 4, arg0[3], arg1[3], Arg2, 32768.0f, CharacterSelections[0],
                 NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerSix, 5, arg0[4], arg1[4], Arg2, 32768.0f, CharacterSelections[0],
                 NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerSeven, 6, arg0[5], arg1[5], Arg2, 32768.0f, CharacterSelections[0],
                 NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerEight, 7, arg0[6], arg1[6], Arg2, 32768.0f, CharacterSelections[0],
                 NO_CONTROLS | IS_CPU_PLAYER);
    if (DemoFlag == 1) {
        InitializePlayer(PlayerOneCopy, 0, arg0[0], arg1[0], Arg2, 32768.0f, CharacterSelections[0],
                     PLAYER_HUMAN_AND_CPU);
    } else {
        InitializePlayer(PlayerOneCopy, 0, arg0[0], arg1[0], Arg2, 32768.0f, CharacterSelections[0],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
    }
    if (DemoFlag == 1) {
        InitializePlayer(PlayerTwo, 1, arg0[1], arg1[1], Arg2, 32768.0f, CharacterSelections[1], PLAYER_HUMAN_AND_CPU);
    } else {
        InitializePlayer(PlayerTwo, 1, arg0[1], arg1[1], Arg2, 32768.0f, CharacterSelections[1],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
    }
    OpeningFlag = 0;
    SetUpViewDistance();
}

void Battle2P(f32* arg0, f32* arg1, f32 Arg2) {
    if (CourseID == COURSE_BIG_DONUT) {
        InitializePlayer(PlayerOne, 0, arg0[0], arg1[0], Arg2, -16384.0f, CharacterSelections[0],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        InitializePlayer(PlayerTwo, 1, arg0[1], arg1[1], Arg2, 16384.0f, CharacterSelections[1],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
    } else {
        InitializePlayer(PlayerOne, 0, arg0[0], arg1[0], Arg2, 32768.0f, CharacterSelections[0],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        InitializePlayer(PlayerTwo, 1, arg0[1], arg1[1], Arg2, 0.0f, CharacterSelections[1],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
    }
    InitializePlayer(PlayerThree, 2, arg0[2], arg1[2], Arg2, 32768.0f, CharacterSelections[2],
                 NO_CONTROLS | IS_PLAYER);
    InitializePlayer(PlayerFour, 3, arg0[3], arg1[3], Arg2, 32768.0f, CharacterSelections[3],
                 NO_CONTROLS | IS_PLAYER);
    InitializePlayer(PlayerFive, 4, arg0[4], arg1[4], Arg2, 32768.0f, 4, NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerSix, 5, arg0[5], arg1[5], Arg2, 32768.0f, 5, NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerSeven, 6, arg0[6], arg1[6], Arg2, 32768.0f, 6, NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerEight, 7, arg0[0], arg1[0], Arg2, 32768.0f, 7, NO_CONTROLS | IS_CPU_PLAYER);
    OpeningFlag = 0;
    SetUpViewDistance();
}

void VSRace3P(f32* arg0, f32* arg1, f32 Arg2) {
    InitializePlayer(PlayerOne, 0, arg0[0], arg1[0], Arg2, 32768.0f, CharacterSelections[0],
                 EXISTS | NO_CONTROLS | IS_PLAYER);
    InitializePlayer(PlayerTwo, 1, arg0[1], arg1[1], Arg2, 32768.0f, CharacterSelections[1],
                 EXISTS | NO_CONTROLS | IS_PLAYER);
    InitializePlayer(PlayerThree, 2, arg0[2], arg1[2], Arg2, 32768.0f, CharacterSelections[2],
                 EXISTS | NO_CONTROLS | IS_PLAYER);
    if (DemoFlag == 1) {
        InitializePlayer(PlayerOne, 0, arg0[0], arg1[0], Arg2, 32768.0f, CharacterSelections[0], PLAYER_HUMAN_AND_CPU);
        InitializePlayer(PlayerTwo, 1, arg0[1], arg1[1], Arg2, 32768.0f, CharacterSelections[1], PLAYER_HUMAN_AND_CPU);
        InitializePlayer(PlayerThree, 2, arg0[2], arg1[2], Arg2, 32768.0f, CharacterSelections[2], PLAYER_HUMAN_AND_CPU);
    }

    InitializePlayer(PlayerFour, 3, arg0[3], arg1[3], Arg2, 32768.0f, 3, NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerFive, 4, arg0[4], arg1[4], Arg2, 32768.0f, 4, NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerSix, 5, arg0[5], arg1[5], Arg2, 32768.0f, 5, NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerSeven, 6, arg0[6], arg1[6], Arg2, 32768.0f, 6, NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerEight, 7, arg0[0], arg1[0], Arg2, 32768.0f, 7, NO_CONTROLS | IS_CPU_PLAYER);
    OpeningFlag = 0;
    SetUpViewDistance();
}

void Battle3P(f32* arg0, f32* arg1, f32 Arg2) {
    if (CourseID == COURSE_BIG_DONUT) {
        InitializePlayer(PlayerOne, 0, arg0[0], arg1[0], Arg2, -16384.0f, CharacterSelections[0],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        InitializePlayer(PlayerTwo, 1, arg0[1], arg1[1], Arg2, 16384.0f, CharacterSelections[1],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        InitializePlayer(PlayerThree, 2, arg0[2], arg1[2], Arg2, 0.0f, CharacterSelections[2],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
    } else {
        InitializePlayer(PlayerOne, 0, arg0[0], arg1[0], Arg2, 32768.0f, CharacterSelections[0],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        InitializePlayer(PlayerTwo, 1, arg0[1], arg1[1], Arg2, 0.0f, CharacterSelections[1],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        InitializePlayer(PlayerThree, 2, arg0[2], arg1[2], Arg2, -16384.0f, CharacterSelections[2],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
    }
    InitializePlayer(PlayerFour, 3, arg0[3], arg1[3], Arg2, 32768.0f, 3, NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerFive, 4, arg0[4], arg1[4], Arg2, 32768.0f, 4, NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerSix, 5, arg0[5], arg1[5], Arg2, 32768.0f, 5, NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerSeven, 6, arg0[6], arg1[6], Arg2, 32768.0f, 6, NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerEight, 7, arg0[0], arg1[0], Arg2, 32768.0f, 7, NO_CONTROLS | IS_CPU_PLAYER);
    OpeningFlag = 0;
    SetUpViewDistance();
}

void VSRace4P(f32* arg0, f32* arg1, f32 Arg2) {
    InitializePlayer(PlayerOne, 0, arg0[0], arg1[0], Arg2, 32768.0f, CharacterSelections[0],
                 EXISTS | NO_CONTROLS | IS_PLAYER);
    InitializePlayer(PlayerTwo, 1, arg0[1], arg1[1], Arg2, 32768.0f, CharacterSelections[1],
                 EXISTS | NO_CONTROLS | IS_PLAYER);
    InitializePlayer(PlayerThree, 2, arg0[2], arg1[2], Arg2, 32768.0f, CharacterSelections[2],
                 EXISTS | NO_CONTROLS | IS_PLAYER);
    InitializePlayer(PlayerFour, 3, arg0[3], arg1[3], Arg2, 32768.0f, CharacterSelections[3],
                 EXISTS | NO_CONTROLS | IS_PLAYER);
    if (DemoFlag == 1) {
        InitializePlayer(PlayerOne, 0, arg0[0], arg1[0], Arg2, 32768.0f, CharacterSelections[0], PLAYER_HUMAN_AND_CPU);
        InitializePlayer(PlayerTwo, 1, arg0[1], arg1[1], Arg2, 32768.0f, CharacterSelections[1], PLAYER_HUMAN_AND_CPU);
        InitializePlayer(PlayerThree, 2, arg0[2], arg1[2], Arg2, 32768.0f, CharacterSelections[2], PLAYER_HUMAN_AND_CPU);
        InitializePlayer(PlayerFour, 3, arg0[3], arg1[3], Arg2, 32768.0f, CharacterSelections[3], PLAYER_HUMAN_AND_CPU);
    }
    InitializePlayer(PlayerFive, 4, arg0[4], arg1[4], Arg2, 32768.0f, 4, NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerSix, 5, arg0[5], arg1[5], Arg2, 32768.0f, 5, NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerSeven, 6, arg0[6], arg1[6], Arg2, 32768.0f, 6, NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerEight, 7, arg0[0], arg1[0], Arg2, 32768.0f, 7, NO_CONTROLS | IS_CPU_PLAYER);
    OpeningFlag = 0;
    SetUpViewDistance();
}

void Battle4P(f32* arg0, f32* arg1, f32 Arg2) {
    if (CourseID == COURSE_BIG_DONUT) {
        InitializePlayer(PlayerOne, 0, arg0[0], arg1[0], Arg2, -16384.0f, CharacterSelections[0],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        InitializePlayer(PlayerTwo, 1, arg0[1], arg1[1], Arg2, 16384.0f, CharacterSelections[1],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        InitializePlayer(PlayerThree, 2, arg0[2], arg1[2], Arg2, 0.0f, CharacterSelections[2],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        InitializePlayer(PlayerFour, 3, arg0[3], arg1[3], Arg2, 32768.0f, CharacterSelections[3],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
    } else {
        InitializePlayer(PlayerOne, 0, arg0[0], arg1[0], Arg2, 32768.0f, CharacterSelections[0],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        InitializePlayer(PlayerTwo, 1, arg0[1], arg1[1], Arg2, 0.0f, CharacterSelections[1],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        InitializePlayer(PlayerThree, 2, arg0[2], arg1[2], Arg2, -16384.0f, CharacterSelections[2],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        InitializePlayer(PlayerFour, 3, arg0[3], arg1[3], Arg2, 16384.0f, CharacterSelections[3],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
    }
    InitializePlayer(PlayerFive, 4, arg0[4], arg1[4], Arg2, 32768.0f, 4, NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerSix, 5, arg0[5], arg1[5], Arg2, 32768.0f, 5, NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerSeven, 6, arg0[6], arg1[6], Arg2, 32768.0f, 6, NO_CONTROLS | IS_CPU_PLAYER);
    InitializePlayer(PlayerEight, 7, arg0[0], arg1[0], Arg2, 32768.0f, 7, NO_CONTROLS | IS_CPU_PLAYER);
    OpeningFlag = 0;
    SetUpViewDistance();
}

void ResultKartPos(void) {
    InitializePlayer(PlayerOne, 0, -2770.774f, -345.187f, -34.6f, 0.0f, DPointsRank[0],
                 EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
    InitializePlayer(PlayerTwo, 1, -3691.506f, -6.822f, -6.95f, (f32) (200 * DEGREES(1)), DPointsRank[1],
                 EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
    InitializePlayer(PlayerThree, 2, -3475.028f, -998.485f, -8.059f, (f32) (250 * DEGREES(1)), DPointsRank[2],
                 EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
    if (Resultstat.Rank >= 3) {
        InitializePlayer(PlayerFour, 3, -3025.772f, 110.039f, -23.224f, (f32) (155 * DEGREES(1)), Resultstat.Kart,
                     EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
    } else {
        InitializePlayer(PlayerFour, 3, -3025.772f, 110.039f, -23.224f, (f32) (155 * DEGREES(1)), DPointsRank[3],
                     EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
    }
    InitializePlayer(PlayerFive, 4, -2770.774f, -345.187f, -34.6f, 0.0f, 0, 0x7000);
    InitializePlayer(PlayerSix, 5, -3691.506f, -6.822f, -6.95f, (f32) (200 * DEGREES(1)), 0, 0x7000);
    InitializePlayer(PlayerSeven, 6, -3475.028f, -998.485f, -8.059f, (f32) (250 * DEGREES(1)), 0, 0x7000);
    InitializePlayer(PlayerEight, 7, -3025.772f, 110.039f, -23.224f, (f32) (155 * DEGREES(1)), 0, 0x7000);
    OpeningFlag = 0;
    SetUpViewDistance();
}

void ResetPos(void) {
    s16 sp5E;
    s16 sp5C;
    s16 sp5A;
    s32 temp;
    UNUSED s32 pad[4];
    switch (CourseID) {
        case COURSE_MARIO_RACEWAY:
        case COURSE_CHOCO_MOUNTAIN:
        case COURSE_BOWSER_CASTLE:
        case COURSE_BANSHEE_BOARDWALK:
        case COURSE_YOSHI_VALLEY:
        case COURSE_FRAPPE_SNOWLAND:
        case COURSE_KOOPA_BEACH:
        case COURSE_ROYAL_RACEWAY:
        case COURSE_LUIGI_RACEWAY:
        case COURSE_MOO_MOO_FARM:
        case COURSE_TOADS_TURNPIKE:
        case COURSE_KALAMARI_DESERT:
        case COURSE_SHERBET_LAND:
        case COURSE_RAINBOW_ROAD:
        case COURSE_WARIO_STADIUM:
        case COURSE_DK_JUNGLE:
            InitCenterLine();
            sp5E = (f32) CenterPathBP[0][0].Pointx;
            sp5C = (f32) CenterPathBP[0][0].Pointz;
            sp5A = (f32) CenterPathBP[0][0].Pointy;
            if (CourseID == COURSE_TOADS_TURNPIKE) {
                sp5E = 0;
            }
            break;

        case COURSE_BLOCK_FORT:
        case COURSE_SKYSCRAPER:
        case COURSE_DOUBLE_DECK:
        case COURSE_BIG_DONUT:
            KameHibasiraInit();
            break;
    }

    switch (CourseID) {
        case COURSE_MARIO_RACEWAY:
        case COURSE_CHOCO_MOUNTAIN:
        case COURSE_BOWSER_CASTLE:
        case COURSE_BANSHEE_BOARDWALK:
        case COURSE_YOSHI_VALLEY:
        case COURSE_FRAPPE_SNOWLAND:
        case COURSE_KOOPA_BEACH:
        case COURSE_ROYAL_RACEWAY:
        case COURSE_LUIGI_RACEWAY:
        case COURSE_MOO_MOO_FARM:
        case COURSE_TOADS_TURNPIKE:
        case COURSE_KALAMARI_DESERT:
        case COURSE_SHERBET_LAND:
        case COURSE_RAINBOW_ROAD:
        case COURSE_WARIO_STADIUM:
        case COURSE_DK_JUNGLE:
            switch (ScreenSplitA) {
                case SCREEN_MODE_1P:
                    switch (GameMode) {
                        case GRAND_PRIX:
                            GridX[0] = (GridX[2] = (GridX[4] = (GridX[6] = sp5E + 0x14)));
                            GridX[1] = (GridX[3] = (GridX[5] = (GridX[7] = sp5E - 0x14)));
                            GridZ[0] = sp5C + 0x1E;
                            GridZ[1] = sp5C + 0x32;
                            GridZ[2] = sp5C + 0x46;
                            GridZ[3] = sp5C + 0x5A;
                            GridZ[4] = sp5C + 0x6E;
                            GridZ[5] = sp5C + 0x82;
                            GridZ[6] = sp5C + 0x96;
                            GridZ[7] = sp5C + 0xAA;
                            MarioGP1P(GridX, GridZ, sp5A);
                            break;

                        case TIME_TRIALS:
                            GridX[0] = (GridX[2] = (GridX[4] = (GridX[6] = sp5E)));
                            GridX[1] = (GridX[3] = (GridX[5] = (GridX[7] = sp5E)));
                            GridZ[0] = sp5C + 0x1E;
                            GridZ[1] = sp5C + 0x1E;
                            GridZ[2] = sp5C + 0x1E;
                            GridZ[3] = sp5C + 0x1E;
                            GridZ[4] = sp5C + 0x1E;
                            GridZ[5] = sp5C + 0x1E;
                            GridZ[6] = sp5C + 0x1E;
                            GridZ[7] = sp5C + 0x1E;
                            TimeAttack(GridX, GridZ, sp5A);
                            break;
                    }
                    break;

                case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                    switch (GameMode) {
                        case GRAND_PRIX:
                            GridX[0] = (GridX[2] = (GridX[4] = (GridX[6] = sp5E + 0x14)));
                            GridX[1] = (GridX[3] = (GridX[5] = (GridX[7] = sp5E - 0x14)));
                            GridZ[0] = sp5C + 0x1E;
                            GridZ[1] = sp5C + 0x32;
                            GridZ[2] = sp5C + 0x46;
                            GridZ[3] = sp5C + 0x5A;
                            GridZ[4] = sp5C + 0x6E;
                            GridZ[5] = sp5C + 0x82;
                            GridZ[6] = sp5C + 0x96;
                            GridZ[7] = sp5C + 0xAA;
                            MarioGP2P(GridX, GridZ, sp5A);
                            break;

                        case VERSUS:
                            GridX[0] = (GridX[2] = (GridX[4] = (GridX[6] = sp5E + 0xA)));
                            GridX[1] = (GridX[3] = (GridX[5] = (GridX[7] = sp5E - 0xA)));
                            GridZ[0] = sp5C + 0x1E;
                            GridZ[1] = sp5C + 0x1E;
                            GridZ[2] = sp5C + 0x1E;
                            GridZ[3] = sp5C + 0x1E;
                            GridZ[4] = sp5C + 0x1E;
                            GridZ[5] = sp5C + 0x1E;
                            GridZ[6] = sp5C + 0x1E;
                            GridZ[7] = sp5C + 0x1E;
                            VSRace2P(GridX, GridZ, sp5A);
                            break;
                    }
                    break;

                case SCREEN_MODE_3P_4P_SPLITSCREEN:
                    switch (GameMode) {
                        case VERSUS:
                            GridX[0] = sp5E + 0x1E;
                            GridX[6] = sp5E - 0xA;
                            GridX[1] = sp5E + 0xA;
                            GridX[7] = sp5E - 0x1E;
                            GridX[4] = sp5E - 0xA;
                            GridX[2] = sp5E - 0xA;
                            GridX[5] = sp5E - 0x1E;
                            GridX[3] = sp5E - 0x1E;
                            GridZ[0] = sp5C + 0x1E;
                            GridZ[1] = sp5C + 0x1E;
                            GridZ[2] = sp5C + 0x1E;
                            GridZ[3] = sp5C + 0x1E;
                            GridZ[4] = sp5C + 0x1E;
                            GridZ[5] = sp5C + 0x1E;
                            GridZ[6] = sp5C + 0x1E;
                            GridZ[7] = sp5C + 0x1E;
                            if (PlayerCount == 4) {
                                VSRace4P(GridX, GridZ, sp5A);
                            } else {
                                VSRace3P(GridX, GridZ, sp5A);
                            }
                            break;
                    }
                    break;
            }
            break;

        case COURSE_BLOCK_FORT:
            switch (ScreenSplitA) {
                case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                    temp = 5;
                    if (1) {};
                    GridX[0] = 0;
                    GridX[1] = 0;
                    GridZ[1] = -200.0f;
                    GridZ[0] = 200.0f;
                    Battle2P(GridX, GridZ, temp);
                    break;

                case SCREEN_MODE_3P_4P_SPLITSCREEN:
                    temp = 5;
                    GridX[2] = -200.0f;
                    GridZ[1] = -200.0f;
                    GridX[0] = 0.0f;
                    GridX[1] = 0.0f;
                    GridZ[2] = 0.0f;
                    GridZ[3] = 0.0f;
                    GridX[3] = 200.0f;
                    GridZ[0] = 200.0f;
                    if (PlayerCount == 4) {
                        Battle4P(GridX, GridZ, temp);
                    } else {
                        Battle3P(GridX, GridZ, temp);
                    }
                    break;
            }
            break;

        case COURSE_SKYSCRAPER:
            switch (ScreenSplitA) {
                case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                    temp = 0x1E0;
                    if (1) {};
                    GridX[0] = 0.0f;
                    GridX[1] = 0.0f;
                    GridZ[1] = -400.0f;
                    GridZ[0] = 400.0f;
                    Battle2P(GridX, GridZ, temp);
                    break;

                case SCREEN_MODE_3P_4P_SPLITSCREEN:
                    temp = 0x1E0;
                    GridX[0] = 0.0f;
                    GridX[1] = 0.0f;
                    GridX[2] = -400.0f;
                    GridX[3] = 400.0f;
                    GridZ[0] = 400.0f;
                    GridZ[1] = -400.0f;
                    GridZ[2] = 0.0f;
                    GridZ[3] = 0.0f;
                    if (PlayerCount == 4) {
                        Battle4P(GridX, GridZ, temp);
                    } else {
                        Battle3P(GridX, GridZ, temp);
                    }
                    break;
            }
            break;

        case COURSE_DOUBLE_DECK:
            switch (ScreenSplitA) {
                case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                    temp = 0x37;
                    if (1) {};
                    GridX[0] = 0.0f;
                    GridX[1] = 0.0f;
                    GridZ[1] = -160.0f;
                    GridZ[0] = 160.0f;
                    Battle2P(GridX, GridZ, temp);
                    break;

                case 3:
                    temp = 0x37;
                    GridX[0] = 0.0f;
                    GridX[1] = 0.0f;
                    GridX[2] = -160.0f;
                    GridX[3] = 160.0f;
                    GridZ[0] = 160.0f;
                    GridZ[1] = -160.0f;
                    GridZ[2] = 0.0f;
                    GridZ[3] = 0.0f;
                    if (PlayerCount == 4) {
                        Battle4P(GridX, GridZ, temp);
                    } else {
                        Battle3P(GridX, GridZ, temp);
                    }
                    break;
            }
            break;

        case COURSE_BIG_DONUT:
            switch (ScreenSplitA) {
                case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                    temp = 0xC8;
                    if (1) {};
                    GridX[0] = 0.0f;
                    GridX[1] = 0.0f;
                    GridZ[1] = -575.0f;
                    GridZ[0] = 575.0f;
                    Battle2P(GridX, GridZ, temp);
                    break;

                case SCREEN_MODE_3P_4P_SPLITSCREEN:
                    temp = 0xC8;
                    GridX[0] = 0.0f;
                    GridX[1] = 0.0f;
                    GridX[2] = -575.0f;
                    GridX[3] = 575.0f;
                    GridZ[0] = 575.0f;
                    GridZ[1] = -575.0f;
                    GridZ[2] = 0.0f;
                    GridZ[3] = 0.0f;
                    if (PlayerCount == 4) {
                        Battle4P(GridX, GridZ, temp);
                    } else {
                        Battle3P(GridX, GridZ, temp);
                    }
                    break;
            }
            break;

        default:
            GridX[0] = (GridX[2] = (GridX[4] = (GridX[6] = 20.0f)));
            GridX[1] = (GridX[3] = (GridX[5] = (GridX[7] = -20.0f)));
            GridZ[0] = 30.0f;
            GridZ[1] = 50.0f;
            GridZ[2] = 70.0f;
            GridZ[3] = 90.0f;
            GridZ[4] = 110.0f;
            GridZ[5] = 130.0f;
            GridZ[6] = 150.0f;
            GridZ[7] = 170.0f;
            InitializePlayer(PlayerOneCopy, 0, GridX[0], GridZ[0], sp5A, 32768.0f, CharacterSelections[0],
                         EXISTS | NO_CONTROLS | IS_PLAYER);
            InitializePlayer(PlayerTwo, 1, GridX[1], GridZ[1], sp5A, 32768.0f, 1,
                         EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
            InitializePlayer(PlayerThree, 2, GridX[2], GridZ[2], sp5A, 32768.0f, 2,
                         EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
            InitializePlayer(PlayerFour, 3, GridX[3], GridZ[3], sp5A, 32768.0f, 3,
                         EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
            InitializePlayer(PlayerFive, 4, GridX[4], GridZ[4], sp5A, 32768.0f, 4,
                         EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
            InitializePlayer(PlayerSix, 5, GridX[5], GridZ[5], sp5A, 32768.0f, 5,
                         EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
            InitializePlayer(PlayerSeven, 6, GridX[6], GridZ[6], sp5A, 32768.0f, 6,
                         EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
            InitializePlayer(PlayerEight, 7, GridX[7], GridZ[7], sp5A, 32768.0f, 7,
                         EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
            OpeningFlag = 0;
            break;
    }

    if (GameMode != BATTLE) {
        InitEnemyKart();
    }
}

void ResultPos(void) {
    ResultKartPos();
}

void InitDraw(Player* Player, Camera* Camera, s8 PlayerId, s8 screenId) {
    if (Player->Flag & EXISTS) {
        if (screenId == 0) {
            RunKart(Player, Camera, screenId, PlayerId);
        }
        KartPosset(Player, Camera, screenId, PlayerId);
        if ((screenId == 0) || (screenId == 1)) {
            IinitcolDMA(Player, PlayerId, screenId, 0);
            IinitcolDMA(Player, PlayerId, screenId, 1);
            Sikartdma(Player, PlayerId, screenId, screenId, 0);
            decodeMIO0((u8*) &EncodedKartTexture[0][screenId][PlayerId],
                       (u8*) &D_802BFB80.arraySize8[0][screenId][PlayerId]);
        } else {
            IinitcolDMA(Player, PlayerId, screenId, 0);
            IinitcolDMA(Player, PlayerId, screenId, 1);
            Sikartdma(Player, (s8) (PlayerId + 4), screenId, (s8) (screenId - 2), 0);
            decodeMIO0((u8*) &EncodedKartTexture[0][screenId - 2][PlayerId + 4],
                       (u8*) &D_802BFB80.arraySize8[0][screenId - 2][PlayerId + 4]);
        }

        OldDir[screenId][PlayerId] = Player->dir[screenId];
        OldDeg[screenId][PlayerId] = Player->deg[screenId];
        OldSlipcount[screenId][PlayerId] = Player->Slipcount;
        DrawNumber[screenId][PlayerId] = 0;
        Doobj(Player, PlayerId, screenId);
    }
}

void InitialGame(void) {
    UNUSED s32 pad;
    Player* Player = &GamePlayers[0];

    Imageinit();
    if (GameState == ENDING) {
        ResultPos();
    } else {
        ResetPos();
    }
    if (!DemoFlag) {
        switch (ScreenSplitA) {
            case SCREEN_MODE_1P:
                switch (GameMode) {
                    case GRAND_PRIX:
                        if (CourseID == COURSE_TOADS_TURNPIKE) {
                            InitialCamera(0.0f, Player->Position[1], GridZ[7], Player->Direction[1], 8, 0);
                        } else {
                            InitialCamera((GridX[7] + GridX[6]) / 2, Player->Position[1], GridZ[7],
                                        Player->Direction[1], 8, 0);
                        }
                        break;

                    case TIME_TRIALS:
                        InitialCamera(Player->Position[0], Player->Position[1], Player->Position[2], Player->Direction[1], 1, 0);
                        break;

                    default:
                        InitialCamera(Player->Position[0], Player->Position[1], Player->Position[2], Player->Direction[1], 10, 0);
                        break;
                }
                break;

            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                switch (GameMode) {
                    case GRAND_PRIX:
                        InitialCamera(Player->Position[0], Player->Position[1], Player->Position[2], Player->Direction[1], 1, 0);
                        Player++;
                        InitialCamera(Player->Position[0], Player->Position[1], Player->Position[2], Player->Direction[1], 1, 1);
                        break;

                    case BATTLE:
                        InitialCamera(Player->Position[0], Player->Position[1], Player->Position[2], Player->Direction[1], 9, 0);
                        Player++;
                        InitialCamera(Player->Position[0], Player->Position[1], Player->Position[2], Player->Direction[1], 9, 1);
                        break;

                    default:
                        InitialCamera(Player->Position[0], Player->Position[1], Player->Position[2], Player->Direction[1], 1, 0);
                        Player++;
                        InitialCamera(Player->Position[0], Player->Position[1], Player->Position[2], Player->Direction[1], 1, 1);
                        break;
                }
                break;

            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                if (GameMode == BATTLE) {
                    InitialCamera(Player->Position[0], Player->Position[1], Player->Position[2], Player->Direction[1], 9, 0);
                    Player++;
                    InitialCamera(Player->Position[0], Player->Position[1], Player->Position[2], Player->Direction[1], 9, 1);
                    Player++;
                    InitialCamera(Player->Position[0], Player->Position[1], Player->Position[2], Player->Direction[1], 9, 2);
                    if (PlayerCount == 4) {
                        Player++;
                        InitialCamera(Player->Position[0], Player->Position[1], Player->Position[2], Player->Direction[1], 9, 3);
                    }
                } else {
                    InitialCamera(Player->Position[0], Player->Position[1], Player->Position[2], Player->Direction[1], 1, 0);
                    Player++;
                    InitialCamera(Player->Position[0], Player->Position[1], Player->Position[2], Player->Direction[1], 1, 1);
                    Player++;
                    InitialCamera(Player->Position[0], Player->Position[1], Player->Position[2], Player->Direction[1], 1, 2);
                    if (PlayerCount == 4) {
                        Player++;
                        InitialCamera(Player->Position[0], Player->Position[1], Player->Position[2], Player->Direction[1], 1, 3);
                    }
                }
                break;
        }
    } else {
        switch (ScreenSplitA) {
            case SCREEN_MODE_1P:
                InitialCamera(Player->Position[0], Player->Position[1], Player->Position[2], Player->Direction[1], 3, 0);
                break;

            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                InitialCamera(Player->Position[0], Player->Position[1], Player->Position[2], Player->Direction[1], 3, 0);
                Player++;
                InitialCamera(Player->Position[0], Player->Position[1], Player->Position[2], Player->Direction[1], 3, 1);
                break;

            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                InitialCamera(Player->Position[0], Player->Position[1], Player->Position[2], Player->Direction[1], 3, 0);
                Player++;
                InitialCamera(Player->Position[0], Player->Position[1], Player->Position[2], Player->Direction[1], 3, 1);
                Player++;
                InitialCamera(Player->Position[0], Player->Position[1], Player->Position[2], Player->Direction[1], 3, 2);
                Player++;
                InitialCamera(Player->Position[0], Player->Position[1], Player->Position[2], Player->Direction[1], 3, 3);
                break;
        }
    }

    switch (ScreenSplitA) {
        case SCREEN_MODE_1P:
            InitDraw(PlayerOneCopy, Camera1, 0, 0); // sic
            InitDraw(PlayerTwo, Camera1, 1, 0);
            InitDraw(PlayerThree, Camera1, 2, 0);
            InitDraw(PlayerFour, Camera1, 3, 0);
            InitDraw(PlayerFive, Camera1, 4, 0);
            InitDraw(PlayerSix, Camera1, 5, 0);
            InitDraw(PlayerSeven, Camera1, 6, 0);
            InitDraw(PlayerEight, Camera1, 7, 0);
            break;

        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            InitDraw(PlayerOneCopy, Camera1, 0, 0);
            InitDraw(PlayerTwo, Camera1, 1, 0);
            InitDraw(PlayerThree, Camera1, 2, 0);
            InitDraw(PlayerFour, Camera1, 3, 0);
            InitDraw(PlayerFive, Camera1, 4, 0);
            InitDraw(PlayerSix, Camera1, 5, 0);
            InitDraw(PlayerSeven, Camera1, 6, 0);
            InitDraw(PlayerEight, Camera1, 7, 0);
            InitDraw(PlayerOneCopy, Camera2, 0, 1);
            InitDraw(PlayerTwo, Camera2, 1, 1);
            InitDraw(PlayerThree, Camera2, 2, 1);
            InitDraw(PlayerFour, Camera2, 3, 1);
            InitDraw(PlayerFive, Camera2, 4, 1);
            InitDraw(PlayerSix, Camera2, 5, 1);
            InitDraw(PlayerSeven, Camera2, 6, 1);
            InitDraw(PlayerEight, Camera2, 7, 1);
            break;

        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            InitDraw(PlayerOneCopy, Camera1, 0, 0);
            InitDraw(PlayerTwo, Camera1, 1, 0);
            InitDraw(PlayerThree, Camera1, 2, 0);
            InitDraw(PlayerFour, Camera1, 3, 0);
            InitDraw(PlayerOneCopy, Camera2, 0, 1);
            InitDraw(PlayerTwo, Camera2, 1, 1);
            InitDraw(PlayerThree, Camera2, 2, 1);
            InitDraw(PlayerFour, Camera2, 3, 1);
            InitDraw(PlayerOneCopy, Camera3, 0, 2);
            InitDraw(PlayerTwo, Camera3, 1, 2);
            InitDraw(PlayerThree, Camera3, 2, 2);
            InitDraw(PlayerFour, Camera3, 3, 2);
            InitDraw(PlayerOneCopy, Camera4, 0, 3);
            InitDraw(PlayerTwo, Camera4, 1, 3);
            InitDraw(PlayerThree, Camera4, 2, 3);
            InitDraw(PlayerFour, Camera4, 3, 3);
            break;
    }
}

void InitResultKart(void) {
    Player* Player = PlayerOne;
    s32 PlayerId;

    InitialCamera(Player->Position[0], Player->Position[1], Player->Position[2], Player->Direction[1], 3, 0);
    InitialCamera(Player->Position[0], Player->Position[1], Player->Position[2], Player->Direction[1], 3, 1);

    for (PlayerId = 0; PlayerId < NUM_PLAYERS; PlayerId++, Player++) {
        IinitcolDMA(Player, PlayerId, 1, 0);
        IinitcolDMA(Player, PlayerId, 1, 1);
    }
}

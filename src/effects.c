#include <ultra64.h>
#include <macros.h>
#include <decode.h>
#include <mk64.h>
#include <defines.h>
#include <sounds.h>
#include "code_800029B0.h"
#include "math_util.h"
#include "kart_attributes.h"
#include "path.h"
#include "cpu_vehicles_camera_path.h"
#include "render_player.h"
#include "player_controller.h"
#include "render_objects.h"
#include "code_80057C60.h"
#include "effects.h"
#include "audio/external.h"
#include "spawn_players.h"
#include "menu_items.h"
#include <course.h>
#include "OverKartHooks.h"

s32 D_8018D900[8];
s16 D_8018D920[8];
s32 StarUseCounter[8];
s32 GhostUseCounter[8];
s32 GhostUseTimer[8]; // Used for the Alpha of the other screens in split-screen Mode
s32 D_8018D990[8];

UNUSED void KWGetTimeInit(void) {
}

s32 KWStartTime(UNUSED s32* arg0) {
}

void KWGetTime(UNUSED s32* arg0, UNUSED s32 arg1) {
    arg1 = 4;
}

UNUSED void KWAddTime(UNUSED s32 arg0) {
}

UNUSED void KWGetLine(UNUSED s32 arg0) {
}

UNUSED void KWSetTime(void) {
}

void Kwdebug(void) {
}

UNUSED void KwdebugItems(void) {
}

UNUSED void KwdebugRcp(void) {
}

UNUSED void DebugsyoriTest(void) {
}

UNUSED void KwdebugAnmbreak(void) {
}

UNUSED void KwdebugMtxkazu(void) {
}

UNUSED void KwdebugGptrkazu(void) {
}

UNUSED void KwdebugKumoram(void) {
}

UNUSED void KwdebugRmonprint1P(void) {
}
UNUSED void Debugsyori1P(void) {
}
UNUSED void Debugsyori2PLr(void) {
}
UNUSED void Debugsyori2PUd(void) {
}
UNUSED void Debugsyori4PKleft(void) {
}
UNUSED void Debugsyori4PKright(void) {
}
UNUSED void Debugsyori4PKup(void) {
}
UNUSED void Debugsyori4PKdown(void) {
}
UNUSED void Debugsyori4P(void) {
}
UNUSED void KwdisplayDebug(void) {
}
UNUSED void KwdebugPrintCommon(void) {
}
UNUSED void KwdebugprintPlayerSub(void) {
}
UNUSED void KwdebugprintOgawa(void) {
}

UNUSED void KwdebugprintPlayer(UNUSED s32 arg0, UNUSED s32 arg1) {
    arg1 = 4;
}

UNUSED void KwdebugprintSyori(void) {
}

UNUSED void KwdebugprintTest(void) {
}

UNUSED void Kwdebugprint1P(void) {
}

UNUSED void Kwdebugprint2PLeft(void) {
}

UNUSED void Kwdebugprint2PRight(void) {
}

UNUSED void Kwdebugprint2PUp(void) {
}

UNUSED void Kwdebugprint2PDown(void) {
}

UNUSED void Kwdebugprint4PUpleft(void) {
}

UNUSED void Kwdebugprint4PUpright(void) {
}

UNUSED void Kwdebugprint4PDownleft(void) {
}

UNUSED void Kwdebugprint4PDownright(void) {
}

void CheckHitLetter(Player* Player) {
    // The << 9 is a hacky way to check for HIT_BOMB
    if ((Player->Weapon & HIT_REDSHELL) || (Player->Weapon & HIT_GREENSHELL) ||
        ((Player->Weapon << 9) < 0) || (Player->Weapon & HIT_BOMB_THROW)) {
        Player->Talk = ((u16) Player->Talk | FLASH);
    }
}

UNUSED void StopStrat(void) {
}

void KillStrat(Player* Player, s8 playerIndex) {

    if ((Player->SlipFlag & ROLLOVER) == ROLLOVER) {
        ResetRollover(Player, playerIndex);
    }

    if (((Player->SlipFlag & SPIN_L) == SPIN_L) ||
        (Player->SlipFlag & SPIN_R) == SPIN_R) {
        ResetSpin(Player, playerIndex);
    }
    if ((Player->SlipFlag & WHEELSPIN) == WHEELSPIN) {
        ResetWheelspin(Player, playerIndex);
    }
    if ((Player->HandlingFlag & SPINOUT_SWERVE) != 0) {
        ResetProWheelspin(Player, playerIndex);
    }
    if ((Player->SlipFlag & TURBO) == TURBO) {
        ResetTurbo(Player);
    }
    if ((Player->SlipFlag & WING) == WING) {
        ResetWing(Player);
    }
    if ((Player->SlipFlag & STORM) == STORM) {
        ResetStorm(Player);
    }
    if ((Player->SlipFlag & EXPLODE) == EXPLODE) {
        ResetBomb(Player, playerIndex);
    }
    if ((Player->SlipFlag & THROW_EXPLODE) == THROW_EXPLODE) {
        ResetBombThrow(Player, playerIndex);
    }
    if ((Player->SlipFlag & DASH_JUMP) == DASH_JUMP) {
        ResetDashjump(Player);
    }
    if ((Player->SlipFlag & DASH_MIDJUMP) == DASH_MIDJUMP) {
        ResetDashMidjump(Player);
    }
    if ((Player->SlipFlag & RAPID_ACC) == RAPID_ACC) {
        ResetRapidacc(Player);
    }
    if ((Player->SlipFlag & ROLLOVER_FALL) == ROLLOVER_FALL) {
        ResetRolloverFall(Player, playerIndex);
    }
    if ((Player->SlipFlag & HEIGHT_JUMP) == HEIGHT_JUMP) {
        ResetHeightJump(Player, playerIndex);
    }
    Player->HandlingFlag = (s16) (Player->HandlingFlag & ~REVERSE_GEAR);
    Player->SlipFlag = (s32) (Player->SlipFlag & ~SPIN_TURN);
}

void SetRollover(Player* Player, s8 playerIndex) {
    UNUSED s32 sp24;
    s32 temp_v1;
    KillStrat(Player, playerIndex);
    CheckHitLetter(Player);
    temp_v1 = Player->Kart;
    Player->Jumpcount = 0;
    Player->JmpAcc = D_800E37B0[temp_v1];
    Player->JmpSpeed = 0.0f;

    Player->Jump = D_800E3790[temp_v1];
    Player->Slipcount = 0;
    Player->SlipFlag = Player->SlipFlag | ROLLOVER;
    Player->SlipFlag = Player->SlipFlag & ~DRIFT;
    Player->Slipang = 0;
    Player->RolioverTime = 2;
    Player->KageAng = 0;
    if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
        ((Player->Flag & IS_GHOST) != IS_GHOST)) {
        NAPlyVoiceStart(playerIndex, (temp_v1 * 0x10) + 0x29008005);
        NAPlyTrgStart(playerIndex, SOUND_ACTION_EXPLOSION);
    } else {
        OgaEnemyVoice(playerIndex, Player);
    }
    Player->Weapon = (s32) (Player->Weapon & ~HIT_GREENSHELL);
}

void Rollover(Player* Player, s8 playerIndex) {

    AccelOff(Player, 5.0f);
    Player->Slipcount += (s16) 0xA0;
    Player->KageAng += (s16) DEGREES(10);
    if (Player->Slipcount >= 0x2000) {
        Player->Slipcount = 0;
        Player->RolioverTime = (s16) (Player->RolioverTime - 1);
        if (Player->RolioverTime == 0) {
            Player->Slipcount = 0x2000;
            ResetRollover(Player, playerIndex);
            if (GameMode == BATTLE) {
                CutPlayerBalloon(Player, playerIndex);
            }
        }
    }
}

void ResetRollover(Player* Player, s8 playerIndex) {

    Player->Kartroll = 0;
    Player->Kartpitch = 0;
    Player->SlipFlag = (s32) (Player->SlipFlag & ~ROLLOVER);
    Player->Slipcount = 0;
    Player->Slipang = 0;
    Player->Nowsterr = 0;
    Player->RolioverTime = 0;
    Player->Sterrangle = 0;
    Player->Accelcount = 0.0f;
    CompelFlag[0][playerIndex] = 1;
    CompelFlag[1][playerIndex] = 1;
    CompelFlag[2][playerIndex] = 1;
    CompelFlag[3][playerIndex] = 1;
    Player->KageAng = 0;
}

void SetSpin(Player* Player, s8 playerIndex) {
    KillStrat(Player, playerIndex);
    if (((Player->SlipFlag & SPIN_L) != SPIN_L) &&
        ((Player->SlipFlag & SPIN_R) != SPIN_R)) {
        Player->SlipFlag &= ~DRIFT;

        if ((Player->Slipang / DEGREES(1)) >= 0) {
            Player->SlipFlag |= SPIN_R;
        } else {
            Player->SlipFlag |= SPIN_L;
        }

        Player->Talk |= WHIRRR;
        // clang-format off
        Player->Slipang = 0; Player->Nowsterr = 0; Player->Sterrangle = 0; Player->OldDirection = Player->Direction[1]; Player->SpinTimer = 2;
        // clang-format on
        CompelFlag[0][playerIndex] = 1;
        CompelFlag[1][playerIndex] = 1;
        CompelFlag[2][playerIndex] = 1;
        CompelFlag[3][playerIndex] = 1;
        AcceleCount[playerIndex] = Player->Accelcount;
        PowerBandTimer[playerIndex] = 0;
        PowerBandAccele[playerIndex] = false;
        AcceleFlag[playerIndex] = 0;
        AcceleTime[playerIndex] = 0;
        D_8018D920[playerIndex] = 0;

        if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
            ((Player->Flag & IS_GHOST) != IS_GHOST)) {
            NAPlyVoiceStart(playerIndex, (Player->Kart * 0x10) + 0x29008003);
        } else {
            OgaEnemyVoice(playerIndex, Player);
        }
    }
}

void ResetSpin(Player* Player, s8 PlayerId) {
    Player->SlipFlag &= ~SPIN_L;
    Player->SlipFlag &= ~SPIN_R;
    Player->Slipcount = 0;
    Player->Direction[1] = Player->OldDirection;
    Player->Nowsterr = 0;
    Player->Slipang = 0;
    Player->SlipFlag &= ~WHEELSPIN;

    CompelFlag[0][PlayerId] = 1;
    CompelFlag[1][PlayerId] = 1;
    CompelFlag[2][PlayerId] = 1;
    CompelFlag[3][PlayerId] = 1;

    Player->HitFlag &= ~INSTANT_SPINOUT;

    if ((PowerBandAccele[PlayerId] == true) && ((Player->Flag & IS_PLAYER) == IS_PLAYER)) {
        Player->Accelcount = (f32) (Player->Accelcount + 100.0f);
    }
    if ((GameMode == VERSUS) && ((Player->Flag & IS_CPU_PLAYER) == IS_CPU_PLAYER) && (!DemoFlag) &&
        ((Player->JugemuFlag & ON_LAKITU_ROD) == 0) && (GPCurrentRaceRankByPlayerId[PlayerId] != 0)) {
        Player->Weapon = (s32) (Player->Weapon | HIT_BOMB);
    }
}

void Spin(Player* Player, s8 playerIndex) {
    s16 stackPadding1;
    s16 stackPadding2;
    s16 sp30[5] = { DEGREES(6), DEGREES(6), DEGREES(12), DEGREES(9), DEGREES(10) };

    Player->Kartroll = 0;
    Player->Kartpitch = 0;
    if ((Player->HitFlag & INSTANT_SPINOUT) == INSTANT_SPINOUT) {
        AccelOff(Player, 100.0f);
    } else {
        if ((Player->Flag & IS_PLAYER) == IS_PLAYER) {
            AccelOff(Player, 1.0f);
        } else {
            AccelOff(Player, 4.0f);
        }
        if (!(Player->Flag & IS_PLAYER)) {
            AccelOff(Player, 30.0f);
        }
    }
    if ((Player->SlipFlag & SPIN_L) == SPIN_L) {
        Player->Direction[1] -= sp30[Player->SpinTimer];
        D_8018D920[playerIndex] -= sp30[Player->SpinTimer];
        stackPadding1 = (u16) D_8018D920[playerIndex] / (0x10000 / (0x168 / (sp30[Player->SpinTimer] / DEGREES(1))));
        if (stackPadding1 == 0) {
            Player->SpinTimer--;
            if (Player->SpinTimer <= 0) {
                if (GameMode == BATTLE) {
                    CutPlayerBalloon(Player, playerIndex);
                }
                ResetSpin(Player, playerIndex);
            }
        }
    } else {
        Player->Direction[1] += sp30[Player->SpinTimer];
        D_8018D920[playerIndex] -= sp30[Player->SpinTimer];
        stackPadding2 = (u16) D_8018D920[playerIndex] / (0x10000 / (0x168 / (sp30[Player->SpinTimer] / DEGREES(1))));
        if (stackPadding2 == 0) {
            Player->SpinTimer--;
            if (Player->SpinTimer <= 0) {
                ResetSpin(Player, playerIndex);
                if (GameMode == BATTLE) {
                    CutPlayerBalloon(Player, playerIndex);
                }
            }
        }
    }
    if ((PowerBandAccele[playerIndex] == true) && ((Player->Flag & IS_PLAYER) == IS_PLAYER)) {
        PowerBandTimer[playerIndex] = 0x00000078;
        if (Player->Accelcount <= 90.0f) {
            Player->Accelcount = 90.0f;
        }
    }
}

void SetWheelspin(Player* Player, s8 playerIndex) {
    KillStrat(Player, playerIndex);

    Player->Weapon &= ~HIT_BANANA;
    Player->OffsetsterrTimer = 0;
    Player->OffsetsterrSpeed = 3.0f;
    Player->OffsetsterrFlag = 1;
    Player->SlipFlag &= ~DRIFT;

    if (((Player->Nowsterr >> 16) >= 20) || ((Player->Nowsterr >> 16) <= -20) ||
        (((Player->Speed / 18.0f) * 216.0f) <= 30.0f) || ((Player->SlipFlag & N_JUMP) != 0) ||
        (((Player->Flag & IS_PLAYER) == 0) && ((Player->SlipFlag & CENTER_LINE) == 0))) {
        SetSpin(Player, playerIndex);
    } else {
        Player->SlipFlag |= WHEELSPIN;
    }
}

// almost identical to ProWheelspin, see there for more documentation
void Wheelspin(Player* Player, s8 playerIndex) {
    f32 swerve_accel_init;
    s16 swerve_timer;
    s16 swerve_direction;
    s16 swerve_velo_current;

    swerve_accel_init = Player->OffsetsterrSpeed;
    swerve_timer = Player->OffsetsterrTimer;
    swerve_direction = Player->OffsetsterrFlag;
    swerve_timer++;
    swerve_velo_current = (swerve_timer * swerve_accel_init) - (0.2 * (swerve_timer * swerve_timer));
    if ((swerve_timer != 0) && (swerve_velo_current < 0)) {
        swerve_timer = 0;
        swerve_direction = -swerve_direction;
        swerve_accel_init *= 0.8;
        // requires braking to recover. A driving spinout allows just Releasing gas as well
        if ((Player->SlipFlag & BREAKE) == BREAKE) {
            Player->SlipFlag |= DISABLE_SPIN;
        }
        if (swerve_accel_init <= 1.0f) {
            Player->SlipFlag &= ~WHEELSPIN;
            if ((Player->SlipFlag & DISABLE_SPIN) != DISABLE_SPIN) {
                SetSpin(Player, playerIndex);
                swerve_timer = 0;
            } else {
                Player->Talk |= MUSIC_NOTE;
                Player->SlipFlag &= ~DISABLE_SPIN;
                if ((Player->Flag & IS_PLAYER) == IS_PLAYER) {
                    NAPlyVoiceStart(playerIndex, (Player->Kart * 0x10) + 0x29008008);
                    swerve_timer = 0;
                }
            }
        }
    }
    swerve_velo_current *= swerve_direction;
    if ((swerve_velo_current <= 0) && (swerve_direction == 1)) {
        swerve_velo_current = 0;
    }
    if ((swerve_velo_current >= 0) && (swerve_direction == -1)) {
        swerve_velo_current = 0;
    }
    Player->Sterrangle += swerve_velo_current * 18;
    Player->OffsetsterrSpeed = swerve_accel_init;
    Player->OffsetsterrTimer = swerve_timer;
    Player->OffsetsterrFlag = swerve_direction;
    if (Player->SlipFlag & N_JUMP) {
        SetSpin(Player, playerIndex);
        Player->SlipFlag &= ~WHEELSPIN;
    }
}

void ResetWheelspin(Player* Player, UNUSED s8 playerIndex) {
    Player->SlipFlag &= ~WHEELSPIN;
}

void SetProWheelSpin(Player* Player, s8 playerIndex) {
    KillStrat(Player, playerIndex);

    Player->Weapon &= ~PRESS_SAVESPIN;
    Player->OffsetsterrTimer = 0;
    Player->OffsetsterrSpeed = 2.0f;
    Player->OffsetsterrFlag = 1;
    Player->SlipFlag &= ~DRIFT;
    Player->HandlingFlag |= SPINOUT_SWERVE;
}

void ProWheelspin(Player* Player, s8 playerIndex) {
    f32 swerve_accel_init;
    s16 swerve_timer;
    s16 swerve_direction;
    s16 swerve_velo_current;

    // These properties are only Used for swerving before spinouts
    swerve_accel_init = Player->OffsetsterrSpeed;
    swerve_timer = Player->OffsetsterrTimer;
    swerve_direction = Player->OffsetsterrFlag;
    swerve_timer++;

    // Standard physics formula: Velo_current = velo_init + (accel_init * Time) + (accel_jerk * Time**2) / 2
    swerve_velo_current = (swerve_accel_init * swerve_timer) - (0.1 * (swerve_timer * swerve_timer)); 

    // Once one swerve finishes, setup to Start a smaller one in the opposite Direction
    if ((swerve_timer != 0) && (swerve_velo_current < 0)) { // (10 * swerve_accel_init < swerve_timer))
        swerve_timer = 0;
        swerve_direction = -swerve_direction;
        swerve_accel_init *= 0.9;
        if (((Player->SlipFlag & BREAKE) == BREAKE) || !(Player->HandlingFlag & ACCELERATE)) {
            Player->SlipFlag |= DISABLE_SPIN;
        }
        // stop swerving once they are small enough
        if (swerve_accel_init <= 1.3) {
            Player->HandlingFlag &= ~SPINOUT_SWERVE;
            if ((Player->SlipFlag & DISABLE_SPIN) != DISABLE_SPIN) {
                SetSpin(Player, playerIndex);
                swerve_timer = 0;
            } else {
                Player->Talk |= MUSIC_NOTE;
                Player->SlipFlag &= ~DISABLE_SPIN;
                if ((Player->Flag & IS_PLAYER) == IS_PLAYER) {
                    NAPlyVoiceStart(playerIndex, (Player->Kart * 0x10) + 0x29008008);
                    swerve_timer = 0;
                }
            }
        }
    }
    swerve_velo_current *= swerve_direction;
    if ((swerve_velo_current <= 0) && (swerve_direction == 1)) {
        swerve_velo_current = 0;
    }
    if ((swerve_velo_current >= 0) && (swerve_direction == -1)) {
        swerve_velo_current = 0;
    }
    /* Unk078 contributes to rotational Velocity (Spin). It looks to be set each frame in steering Code
    (e.g. ProStickAngle), so it does not accumulate values from swerve_velo_current over multiple frames */
    Player->Sterrangle += swerve_velo_current * 20;
    Player->OffsetsterrSpeed = swerve_accel_init;
    Player->OffsetsterrTimer = swerve_timer;
    Player->OffsetsterrFlag = swerve_direction;
    if (Player->SlipFlag & N_JUMP) {
        SetSpin(Player, playerIndex);
        Player->HandlingFlag &= ~SPINOUT_SWERVE;
    }
}

void ResetProWheelspin(Player* Player, UNUSED s8 playerIndex) {
    Player->HandlingFlag &= ~SPINOUT_SWERVE;
}

void SetTurbo(Player* Player, s8 playerIndex) {

    KillStrat(Player, playerIndex);

    Player->SlipFlag |= TURBO;
    Player->Weapon &= ~USE_MUSHROOM;
    Player->sus.GachonTimer = 0;
    Player->sus.GachonFirstspeed = 8.0f;

    if (ReplayFlag != 1) {
        if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
            ((Player->Flag & IS_GHOST) != IS_GHOST)) {
            NaPlyDashStart(playerIndex);
            NAPlyTrgStart(playerIndex, 0x1900A40B);
        }
    } else {
        if (Player == PlayerOne) {
            NaPlyDashStart(playerIndex);
            NAPlyTrgStart(playerIndex, 0x1900A40B);
        }
    }

    Player->TurboTimer = 0x50;
}

void NowTurbo(Player* Player) {
    Player->Accelcount = (f32) Player->AccMaxcount;
    if (Player->TurboTimer > 0) {
        --Player->TurboTimer;
    }

    if (Player->TurboTimer != 0) {
        ChaseFnumber(&Player->TurboPower, 400.0f, 0.5f);
    } else {
        ChaseFnumber(&Player->TurboPower, 0.0f, 0.1f);
    }

    if (Player->TurboPower <= 1.0f) {
        Player->SlipFlag &= ~TURBO;
    }
}

void ResetTurbo(Player* Player) {
    Player->SlipFlag &= ~TURBO;
    Player->TurboPower = 0.0f;
}

void SetWing(Player* Player, s8 playerIndex) {
    KillStrat(Player, playerIndex);

    Player->OldDirection = Player->Direction[1];
    Player->SlipFlag |= WING;
    Player->SlipFlag &= ~DRIFT;
    Player->Weapon &= ~USE_FEATHER;
    Player->JmpAcc = D_800E3730[Player->Kart];
    Player->JmpSpeed = 0.0f;
    Player->Jump = (f32) D_800E3710[Player->Kart];
    Player->SpinTimer = 1;
    Player->Slipang = 0;
    Player->Nowsterr = 0;
    Player->Sterrangle = 0;
    D_8018D920[playerIndex] = 0;

    if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
        ((Player->Flag & IS_GHOST) != IS_GHOST)) {
        NAPlyTrgStart(playerIndex, 0x19008002);
    }

    if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
        ((Player->Flag & IS_GHOST) != IS_GHOST)) {
        NAPlyVoiceStart(playerIndex, (Player->Kart * 0x10) + 0x2900800C);
    }
}

void Wing(Player* Player, s8 playerIndex) {
    s16 temp;

    if (Player->SpinTimer == 0) {
        Player->Direction[1] = Player->OldDirection;
        temp = 0;
    } else {
        Player->Direction[1] -= DEGREES(10);
        D_8018D920[playerIndex] -= DEGREES(10);
        temp = ((u16) D_8018D920[playerIndex] / DEGREES(10));
    }
    if (temp == 0) {
        --Player->SpinTimer;
        if (Player->SpinTimer <= 0) {
            Player->SpinTimer = 0;
        }
        if ((Player->SpinTimer == 0) && ((Player->SlipFlag & N_JUMP) != N_JUMP)) {
            ResetWing(Player);
        }
    }
}

void ResetWing(Player* Player) {
    Player->Slipcount = 0;
    Player->Nowsterr = 0;
    Player->Slipang = 0;
    Player->Direction[1] = Player->OldDirection;
    Player->SlipFlag &= ~WING;
    Player->Gravity = Gravity1[Player->Kart];
    Player->Flag &= ~PLAYER_UNKNOWN_0x80;
}

void SetStorm(Player* Player, s8 playerIndex) {
    KillStrat(Player, playerIndex);

    Player->OldDirection = Player->Direction[1];
    Player->SlipFlag |= STORM;
    Player->SlipFlag &= ~DRIFT;
    Player->Weapon &= ~HIT_TORNADO;
    Player->JmpAcc = D_800E3770[Player->Kart];
    Player->JmpSpeed = 0.0f;
    Player->Jump = D_800E3750[Player->Kart];
    D_8018D920[playerIndex] = 0;
    Player->SpinTimer = 4;
    Player->Slipang = 0;
    Player->Nowsterr = 0;
    Player->Sterrangle = 0;

    if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
        ((Player->Flag & IS_GHOST) != IS_GHOST)) {
        NAPlyVoiceStart(playerIndex, (Player->Kart * 0x10) + 0x29008003);
    }
}

void Storm(Player* Player, s8 playerIndex) {
    s16 temp;

    if (Player->SpinTimer == 0) {
        Player->Direction[1] = Player->OldDirection;
        temp = 0;
    } else {
        Player->Direction[1] -= DEGREES(10);
        D_8018D920[playerIndex] -= DEGREES(10);
        temp = ((u16) (D_8018D920[playerIndex]) / DEGREES(10));
    }
    if (temp == 0) {
        --Player->SpinTimer;
        if (Player->SpinTimer <= 0) {
            Player->SpinTimer = 0;
        }
        if ((Player->SpinTimer == 0) && ((Player->SlipFlag & N_JUMP) != N_JUMP)) {
            ResetStorm(Player);
        }
    }
}

void ResetStorm(Player* Player) {
    Player->Slipcount = 0;
    Player->Nowsterr = 0;
    Player->Slipang = 0;
    Player->Direction[1] = Player->OldDirection;
    Player->SlipFlag &= ~STORM;
    Player->Gravity = Gravity1[Player->Kart];
}

void RollBrokenkart(Player* Player) {
    Player->sus.Accele += 8.0f;
    if (Player->sus.Accele >= 140.0f) {
        Player->sus.Accele = 140.0f;
    }

    if (Player->sus.Flag == 1) {
        Player->sus.Position += Player->sus.Accele;
        if (2002.0f <= Player->sus.Position) {
            Player->sus.Accele = 10.0f;
            Player->sus.Flag *= -1;
        }
    }
    if (Player->sus.Flag == -1) {
        Player->sus.Position -= Player->sus.Accele;
        if (Player->sus.Position <= -2002.0f) {
            Player->sus.Accele = 10.0f;
            Player->sus.Flag *= -1;
        }
    }
}

void SetBroken(Player* Player, s8 playerIndex) {
    KillStrat(Player, playerIndex);

    if ((Player->SlipFlag & BROKEN) == 0) {
        Player->sus.BrokenTimer = 0;
        Player->BrokenTimer = 0;
        Player->sus.BrokenFirstspeed = 4.5f;
        D_8018D990[playerIndex] = 0;
        Player->SlipFlag &= ~(RESTORE | DRIFT);
        CompelFlag[0][playerIndex] = 1;
        CompelFlag[1][playerIndex] = 1;
        CompelFlag[2][playerIndex] = 1;
        CompelFlag[3][playerIndex] = 1;
        Player->sus.Flag = 1;
        Player->sus.Position = 0.0f;
        Player->sus.Accele = 65.0f;

        if ((Player->Weapon & PRESS_SQUISH) != 0) {
            Player->HitFlag |= 0x80;
        }

        if (((Player->Flag & IS_PLAYER) != 0) && ((Player->SlipFlag & BROKEN) == 0)) {
            NAPlyVoiceStart(playerIndex, (Player->Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x05));
        }

        Player->SlipFlag |= BROKEN;
        if (((Player->Flag) & IS_CPU_PLAYER) != 0) {
            OgaEnemyVoice(playerIndex, Player);
        }
    }
}

void Broken(Player* Player, s8 playerIndex) {
    Player->Jumpcount = 0;
    Player->Slipcount = 0;
    Player->Nowsterr = 0;
    Player->Slipang = 0;
    Player->Force = 0.0f;
    Player->Accelcount = 0.0f;
    // clang-format off
    if ((Player->Bump.DistanceZX >= 600.0f) || ((Player->SlipFlag & CENTER_LINE) != 0)) { D_8018D990[playerIndex] = 3; } // placed block on same line to match
    // clang-format on

    switch (D_8018D990[playerIndex]) {
        case 0:
            Player->sus.BrokenFirstspeed = 4.5f;
            if (Player->BrokenTimer < 0x3D) {
                ++Player->BrokenTimer;
            }

            if ((Player->HitFlag & 0x80) != 0) {
                if ((Player->Weapon & PRESS_SQUISH) == 0) {
                    D_8018D990[playerIndex] = 1;
                    Player->BrokenTimer = 0;
                    if ((Player->Flag & IS_PLAYER) != 0) {
                        NAPlyTrgStart(playerIndex, 0x1901904B);
                        break;
                    }
                }
            } else {
                ++Player->BrokenTimer;
                if (Player->BrokenTimer >= 0x1E) {
                    D_8018D990[playerIndex] = 1;
                    Player->BrokenTimer = 0;
                    if ((Player->Flag & IS_PLAYER) != 0) {
                        NAPlyTrgStart(playerIndex, 0x1901904B);
                        break;
                    }
                }
                break;
            }

            break;
        case 1:
            Player->sus.BrokenFirstspeed = 4.5f;
            Player->Position[1] += 0.13;
            ++Player->BrokenTimer;

            if ((Player->HitFlag & 0x80) != 0) {
                if (Player->BrokenTimer >= 0x32) {
                    D_8018D990[playerIndex] = 2;
                    Player->BrokenTimer = 0;
                    Player->HitFlag &= 0xFF7F;
                }
            } else if (Player->BrokenTimer >= 0x50) {
                D_8018D990[playerIndex] = 2;
                Player->BrokenTimer = 0;
            }

            Player->sus.Accele += 6.0f;
            if (Player->sus.Accele >= 90.0f) {
                Player->sus.Accele = 90.0f;
            }

            Player->sus.Position += Player->sus.Accele;
            if (3458.0f <= Player->sus.Position) {
                Player->sus.Accele = 0.0f;
                break;
            }
            break;
        case 2:
            ++Player->BrokenTimer;
            if (Player->BrokenTimer >= 0x259) {
                D_8018D990[playerIndex] = 3;
                Player->BrokenTimer = 0;
            }

            if (Player->Bump.DistanceZX >= 600.0f) {
                D_8018D990[playerIndex] = 3;
            }

            Player->sus.BrokenFirstspeed = 4.5f;
            Player->Position[1] -= 0.085;

            if ((Player->SlipFlag & N_JUMP) != N_JUMP) {
                D_8018D990[playerIndex] = 3;
                Player->BrokenTimer = 0;
            }

            RollBrokenkart(Player);
            break;
        case 3:
            Player->sus.BrokenFirstspeed = 3.0f;
            Player->SlipFlag &= ~BROKEN;
            Player->sus.BrokenTimer = 0;
            Player->SlipFlag |= RESTORE;
            Player->Offsetsize = 1.0f;
            Player->Radius = CharRadiusTbl[Player->Kart];
            CompelFlag[0][playerIndex] = 1;
            CompelFlag[1][playerIndex] = 1;
            CompelFlag[2][playerIndex] = 1;
            CompelFlag[3][playerIndex] = 1;

            if ((Player->Flag & IS_PLAYER) != 0) {
                NAPlyVoiceStart(playerIndex, (Player->Kart * 0x10) + 0x29008008);
            }
            break;
    }
}

void SetThunder(Player* Player, s8 playerIndex) {
    KillStrat(Player, playerIndex);

    Player->Weapon &= ~HIT_LIGHTNING;
    Player->SlipFlag |= (THUNDER | THUNDER_SPIN);
    Player->SlipFlag &= ~DRIFT;
    Player->Force *= 0.6;
    Player->ThunderTimer = 0;
    Player->Offsetsize = 1.0f;
    StarUseCounter[playerIndex] = GameTimer;
    Player->OldDirection = Player->Direction[1];
    Player->SpinTimer = 2;
    Player->Slipang = 0;
    Player->Nowsterr = 0;
    Player->Sterrangle = 0;

    CompelFlag[0][playerIndex] = 1;
    CompelFlag[1][playerIndex] = 1;
    CompelFlag[2][playerIndex] = 1;
    CompelFlag[3][playerIndex] = 1;

    D_8018D920[playerIndex] = 0;

    if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
        ((Player->Flag & IS_GHOST) != IS_GHOST)) {
        NAPlyVoiceStart(playerIndex, (Player->Kart * 0x10) + 0x29008003);
    } else {
        OgaEnemyVoice(playerIndex, Player);
    }
    if (GameMode == BATTLE) {
        CutPlayerBalloon(Player, playerIndex);
    }
}

void Thunder(Player* Player, s8 playerIndex) {
    s16 test;
    if (((Player->SlipFlag & CARHIT) == CARHIT) &&
        ((Player->SlipFlag & BROKEN) != BROKEN)) {
        Player->SlipFlag &= ~THUNDER_SPIN;
        Player->Slipcount = 0;
        Player->Nowsterr = 0;
        Player->Slipang = 0;
        Player->Direction[1] = Player->OldDirection;
        ResetThunder(Player, playerIndex);
        CompelFlag[0][playerIndex] = 1;
        CompelFlag[1][playerIndex] = 1;
        CompelFlag[2][playerIndex] = 1;
        CompelFlag[3][playerIndex] = 1;
        SetBroken(Player, playerIndex);
    } else if ((Player->SlipFlag & THUNDER_SPIN) == THUNDER_SPIN) {
        Player->Direction[1] -= DEGREES(8);
        D_8018D920[playerIndex] -= DEGREES(8);
        test = (u16) D_8018D920[playerIndex] / DEGREES(8);
        if (test == 0) {
            Player->SpinTimer--;
            if (Player->SpinTimer <= 0) {
                Player->Slipcount = 0;
                Player->SlipFlag &= ~THUNDER_SPIN;
                Player->Nowsterr = 0;
                Player->Slipang = 0;
                Player->Direction[1] = Player->OldDirection;
                CompelFlag[0][playerIndex] = 1;
                CompelFlag[1][playerIndex] = 1;
                CompelFlag[2][playerIndex] = 1;
                CompelFlag[3][playerIndex] = 1;
            }
        }
        AccelOff(Player, 1.0f);
    } else {
        Player->ThunderTimer += 1;
        Player->Force = (f32) ((f64) Player->Force * 0.6);
        if ((Player->ThunderTimer == 1) && (Player->Flag & IS_PLAYER)) {
            NAPlyVoiceStart(playerIndex, (Player->Kart * 0x10) + 0x29008005);
        }
        if ((Player->ThunderTimer >= 0) && (Player->ThunderTimer < 0x1CC)) {
            ChaseFnumber(&Player->Offsetsize, 0.7f, 0.1f);
            ChaseFnumber(&Player->Radius,
                             (f32) ((f64) CharRadiusTbl[Player->Kart] * 0.9), 0.1f);
        } else {
            ResetThunder(Player, playerIndex);
            if (Player->Flag & IS_PLAYER) {
                NAPlyVoiceStart(playerIndex, (Player->Kart * 0x10) + 0x29008008);
            }
        }
    }
}

void ResetThunder(Player* Player, UNUSED s8 playerIndex) {
    ChaseFnumber(&Player->Offsetsize, 1.0f, 0.1f);
    ChaseFnumber(&Player->Radius, CharRadiusTbl[Player->Kart], 0.1f);

    Player->SlipFlag &= ~THUNDER;
    Player->Offsetsize = 1.0f;
    Player->Radius = CharRadiusTbl[Player->Kart];
    Player->sus.BrokenFirstspeed = 3.0f;
    Player->sus.BrokenTimer = 0;
    Player->SlipFlag |= RESTORE;

    if ((Player->SlipFlag & THUNDER_SPIN) == THUNDER_SPIN) {
        Player->Direction[1] = Player->OldDirection;
    }

    Player->SlipFlag &= ~THUNDER_SPIN;
}

void Bomb(Player* Player, s8 playerIndex) {
    Player->Kartroll = 0;
    Player->Kartpitch = 0;
    Player->Slipang = 0;
    Player->Nowsterr = 0;
    Player->Jumpcount = 0xF;
    Player->KageAng += DEGREES(15);
    Player->Force = 0.0f;
    Player->Accelcount = 0.0f;
    Player->Velocity[0] = 0.0f;
    Player->Velocity[2] = 0.0f;
    Player->SlipFlag &= ~(SPIN_L | SPIN_R);

    if ((Player->SlipFlag & N_JUMP) != N_JUMP) {
        ++Player->BombTimer;
    }

    if (Player->BombTimer == 3) {
        Player->SlipFlag &= ~EXPLODE;
        Player->Slipcount = 0;
        Player->RolioverTime = 0;
        CompelFlag[0][playerIndex] = 1;
        CompelFlag[1][playerIndex] = 1;
        CompelFlag[2][playerIndex] = 1;
        CompelFlag[3][playerIndex] = 1;
        Player->KageAng = 0;
        Player->Flag &= ~PLAYER_UNKNOWN_0x80;

        if ((PowerBandAccele[playerIndex] == true) && ((Player->Flag & IS_PLAYER) == IS_PLAYER)) {
            Player->Accelcount += 100.0f;
        }
        if (GameMode == BATTLE) {
            CutPlayerBalloon(Player, playerIndex);
        }
    } else {
        Player->Slipcount += 0x80;
        if (Player->Slipcount >= 0x2000) {
            Player->Slipcount = 0;
            --Player->RolioverTime;
            if (Player->RolioverTime == 0) {
                Player->SlipFlag &= ~EXPLODE;
                Player->RolioverTime = 0;
                CompelFlag[0][playerIndex] = 1;
                CompelFlag[1][playerIndex] = 1;
                CompelFlag[2][playerIndex] = 1;
                CompelFlag[3][playerIndex] = 1;
                Player->KageAng = 0;

                if (GameMode == BATTLE) {
                    CutPlayerBalloon(Player, playerIndex);
                }
                if ((PowerBandAccele[playerIndex] == true) &&
                    ((Player->Flag & IS_PLAYER) == IS_PLAYER)) {
                    Player->Accelcount += 100.0f;
                }

                Player->Flag &= ~PLAYER_UNKNOWN_0x80;
            }
        }
    }
}

void SetBombRollover(Player* Player, s8 playerIndex) {
    KillStrat(Player, playerIndex);
    CheckHitLetter(Player);

    Player->Slipcount = 0;
    Player->SlipFlag |= EXPLODE;
    Player->SlipFlag &= ~DRIFT;
    Player->JmpAcc = 0.0f;
    Player->JmpSpeed = 0.0f;
    Player->Jump = 0.0f;
    Player->JmpAcc = D_800E3730[Player->Kart];
    Player->Jump = D_800E3710[Player->Kart];
    Player->RolioverTime = 4;
    Player->KageAng = 0;
    Player->BombTimer = 0;

    if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
        ((Player->Flag & IS_GHOST) != IS_GHOST)) {
        if (((GameMode == VERSUS) && ((Player->Flag & IS_CPU_PLAYER) != 0)) && (!DemoFlag)) {
            NaSeMuteOff(playerIndex);
        }

        if (1) {}

        NAPlyVoiceStart(playerIndex, (Player->Kart * 0x10) + 0x29008005);
        if (((GameMode == VERSUS) && ((Player->Flag & IS_CPU_PLAYER) != 0)) && (!DemoFlag)) {
            NaSeMuteOff(playerIndex);
        }
        NAPlyTrgStart(playerIndex, SOUND_ACTION_EXPLOSION);
    } else {
        OgaEnemyVoice(playerIndex, Player);
    }

    Player->Weapon &= ~(HIT_BOMB | HIT_BOAT);
    Player->Talk |= CRASH;
    PowerBandTimer[playerIndex] = 0;
    PowerBandAccele[playerIndex] = false;
    AcceleFlag[playerIndex] = 0;
    AcceleTime[playerIndex] = 0;
}

void ResetBomb(Player* Player, s8 playerIndex) {
    Player->SlipFlag &= ~EXPLODE;
    Player->Slipcount = 0;
    Player->RolioverTime = 0;
    CompelFlag[0][playerIndex] = 1;
    CompelFlag[1][playerIndex] = 1;
    CompelFlag[2][playerIndex] = 1;
    CompelFlag[3][playerIndex] = 1;
    Player->KageAng = 0;
}

void BombThrow(Player* Player, s8 playerIndex) {
    Player->Kartroll = 0;
    Player->Kartpitch = 0;
    Player->Slipang = 0;
    Player->Nowsterr = 0;
    Player->Jumpcount = 0xF;
    Player->KageAng += DEGREES(15);
    Player->Force /= 2;
    Player->Accelcount = 0.0f;
    Player->SlipFlag &= ~(SPIN_L | SPIN_R);

    if ((Player->SlipFlag & N_JUMP) != N_JUMP) {
        ++Player->BombTimer;
    }

    if (Player->BombTimer == 4) {
        Player->SlipFlag &= ~THROW_EXPLODE;
        Player->Slipcount = 0;
        Player->RolioverTime = 0;
        CompelFlag[3][playerIndex] = 1;
        CompelFlag[0][playerIndex] = 1;
        CompelFlag[1][playerIndex] = 1;
        CompelFlag[2][playerIndex] = 1;
        Player->KageAng = 0;

        if ((PowerBandAccele[playerIndex] == true) && ((Player->Flag & IS_PLAYER) == IS_PLAYER)) {
            Player->Accelcount += 100.0f;
        }

        if (GameMode == BATTLE) {
            CutPlayerBalloon(Player, playerIndex);
        }
    } else {
        Player->Slipcount = (s16) (Player->Slipcount + 0x90);
        if (((s32) Player->Slipcount) >= 0x2000) {
            Player->Slipcount = 0;
            --Player->RolioverTime;
            if (Player->RolioverTime == 0) {
                Player->SlipFlag &= ~THROW_EXPLODE;
                Player->RolioverTime = 0;
                CompelFlag[0][playerIndex] = 1;
                CompelFlag[1][playerIndex] = 1;
                CompelFlag[2][playerIndex] = 1;
                CompelFlag[3][playerIndex] = 1;
                Player->KageAng = 0;
                if ((PowerBandAccele[playerIndex] == true) &&
                    ((Player->Flag & IS_PLAYER) == IS_PLAYER)) {
                    Player->Accelcount += 100.0f;
                }

                if (GameMode == BATTLE) {
                    CutPlayerBalloon(Player, playerIndex);
                }
            }
        }
    }
}

void SetBombThrowRollover(Player* Player, s8 playerIndex) {
    KillStrat(Player, playerIndex);
    CheckHitLetter(Player);

    Player->Slipcount = 0;
    Player->SlipFlag &= ~DRIFT;
    Player->JmpAcc = 0.0f;
    Player->JmpSpeed = 0.0f;
    Player->Jump = 0.0f;
    Player->JmpAcc = D_800E3730[Player->Kart];
    Player->Jump = D_800E3710[Player->Kart];
    Player->RolioverTime = 4;
    Player->KageAng = 0;
    Player->BombTimer = 0;

    if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
        ((Player->Flag & IS_GHOST) != IS_GHOST)) {
        NAPlyVoiceStart(playerIndex, (Player->Kart * 0x10) + 0x29008005);
        NAPlyTrgStart(playerIndex, SOUND_ACTION_EXPLOSION);
    } else {
        OgaEnemyVoice(playerIndex, Player);
    }

    Player->SlipFlag |= THROW_EXPLODE;
    Player->Talk |= CRASH;
    Player->Weapon &= ~(HIT_BOMB_THROW | HIT_REDSHELL);

    PowerBandTimer[playerIndex] = 0;
    PowerBandAccele[playerIndex] = false;
    AcceleFlag[playerIndex] = 0;
    AcceleTime[playerIndex] = 0;
}

void ResetBombThrow(Player* Player, s8 playerIndex) {
    Player->SlipFlag &= ~THROW_EXPLODE;
    Player->Slipcount = 0;
    Player->RolioverTime = 0;
    CompelFlag[0][playerIndex] = 1;
    CompelFlag[1][playerIndex] = 1;
    CompelFlag[2][playerIndex] = 1;
    CompelFlag[3][playerIndex] = 1;
    Player->KageAng = 0;
}

void SetDashjump(Player* Player, s8 PlayerId) {
    KillStrat(Player, PlayerId);

    Player->SlipFlag |= DASH_JUMP;
    Player->Weapon &= ~PRESS_BOOSTPAD;
    Player->sus.GachonTimer = 0;
    Player->sus.GachonFirstspeed = 8.0f;
    if (ReplayFlag != 1) {
        if (((Player->Flag & IS_PLAYER) == IS_PLAYER) && ((Player->Flag & IS_GHOST) == 0)) {
            NAPlyVoiceStart(PlayerId, (Player->Kart * 0x10) + 0x29008001);
            NAPlyTrgStart(PlayerId, 0x1900A40B);
        }
    } else if (Player == PlayerOne) {
        NAPlyVoiceStart(PlayerId, (Player->Kart * 0x10) + 0x29008001);
        NAPlyTrgStart(PlayerId, 0x1900A40B);
    }
    Player->HandlingFlag &= ~REVERSE_GEAR;
    Player->SlipFlag &= ~SPIN_TURN;
}

void NowDashjump(Player* Player) {
    f64 temp_f0;

    Player->Accelcount = Player->AccMaxcount;
    if ((u16) Player->Wallhitcount > 0) {
        Player->Accelcount = 0.0f;
    }
    if ((Player->BumpStatus != BOOST_RAMP_ASPHALT) && ((Player->SlipFlag & N_JUMP) != N_JUMP)) {
        ChaseFnumber(&Player->TurboPower, 0, 1.0f);
    } else {
        ChaseFnumber(&Player->TurboPower, 400.0f, 0.01f);
    }
    if (Player->TurboPower <= 1.0f) {
        Player->SlipFlag &= ~DASH_JUMP;
        Player->TurboPower = 0.0f;
        if (Player->Jumpcount >= 0x33) {
            temp_f0 = 0.7;
            Player->Accelcount = (Player->Accelcount * temp_f0);
            Player->Force = (Player->Force * temp_f0);
        }
    }
}

void ResetDashjump(Player* Player) {
    Player->SlipFlag &= ~DASH_JUMP;
    Player->TurboPower = 0.0f;
}

void SetDashMidjump(Player* Player, s8 PlayerId) {
    KillStrat(Player, PlayerId);

    Player->SlipFlag |= DASH_MIDJUMP;
    Player->Weapon &= ~PRESS_BOOSTJUMP;

    if (ReplayFlag != 1) {
        if (((Player->Flag & IS_PLAYER) == IS_PLAYER) && ((Player->Flag & IS_GHOST) == 0)) {
            NAPlyVoiceStart(PlayerId, (Player->Kart * 0x10) + 0x29008001);
            NAPlyTrgStart(PlayerId, 0x1900A40B);
        }
    } else if (Player == PlayerOne) {
        NAPlyVoiceStart(PlayerId, (Player->Kart * 0x10) + 0x29008001);
        NAPlyTrgStart(PlayerId, 0x1900A40B);
    }

    Player->HandlingFlag &= ~REVERSE_GEAR;
    Player->SlipFlag &= ~SPIN_TURN;
}

void NowDashMidjump(Player* Player) {
    Player->Accelcount = TopSpeedTable[0][Player->Kart];

    if ((Player->BumpStatus != BOOST_RAMP_WOOD) && ((Player->SlipFlag & N_JUMP) != N_JUMP)) {
        ChaseFnumber(&Player->TurboPower, 0, 1.0f);
    } else {
        ChaseFnumber(&Player->TurboPower, 300.0f, 0.1f);
    }

    if (Player->TurboPower <= 1.0f) {
        Player->SlipFlag &= ~DASH_MIDJUMP;
        Player->TurboPower = 0.0f;
        Player->Accelcount /= 2;
        Player->Force /= 2;
    }
}

void ResetDashMidjump(Player* Player) {
    Player->SlipFlag &= ~DASH_MIDJUMP;
    Player->TurboPower = 0.0f;
}

void SetRapidacc(Player* Player, s8 playerIndex) {
    KillStrat(Player, playerIndex);

    Player->OldDirection = Player->Direction[1];
    Player->SpinTimer = 2;
    Player->Slipang = 0;
    Player->Nowsterr = 0;
    Player->SlipFlag |= RAPID_ACC;
    Player->Sterrangle = 0;
    D_8018D920[playerIndex] = -0x8000;

    if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
        ((Player->Flag & IS_GHOST) != IS_GHOST)) {
        NAPlyVoiceStart(playerIndex, (Player->Kart * 0x10) + 0x29008003);
    }
}

void Rapidacc(Player* Player, s8 arg1) {
    s16 temp;

    Player->Force = (Player->MaxPower * 0.05);
    if (Player->SpinTimer < 0) {
        if ((Player->HandlingFlag & START_SPIN_LEFT) == START_SPIN_LEFT) {
            Player->Direction[1] += DEGREES(1);
            D_8018D920[arg1] += DEGREES(1);

            temp = ((u16) D_8018D920[arg1] / DEGREES(1));
            if (temp == 180) {
                Player->SlipFlag &= ~RAPID_ACC;
                Player->Flag &= ~PLAYER_UNKNOWN_0x80;
                Player->Accelcount /= 3.0f;
            }
        } else {

            Player->Direction[1] -= DEGREES(1);
            D_8018D920[arg1] -= DEGREES(1);
            temp = ((u16) D_8018D920[arg1] / DEGREES(1));
            if (temp == 180) {
                Player->SlipFlag &= ~RAPID_ACC;
                Player->Flag &= ~PLAYER_UNKNOWN_0x80;
                Player->Accelcount /= 3.0f;
            }
        }
    } else {
        if ((Player->SpinTimer & 1) != 0) {
            Player->Direction[1] -= DEGREES(2);
            D_8018D920[arg1] -= DEGREES(2);
            temp = ((u16) D_8018D920[arg1] / DEGREES(2));
            if (temp < 71) {
                --Player->SpinTimer;
            }
            Player->HandlingFlag |= START_SPIN_LEFT;
            Player->HandlingFlag &= ~START_SPIN_RIGHT;
            return;
        }
        Player->Direction[1] += DEGREES(2);
        D_8018D920[arg1] += DEGREES(2);
        temp = ((u16) D_8018D920[arg1] / DEGREES(2));
        if (temp >= 110) {
            --Player->SpinTimer;
        }
        Player->HandlingFlag |= START_SPIN_RIGHT;
        Player->HandlingFlag &= ~START_SPIN_LEFT;
    }
}

void ResetRapidacc(Player* Player) {
    Player->SlipFlag &= ~RAPID_ACC;
}

void RolloverFall(Player* Player, UNUSED s8 arg1) {
    Player->Slipcount += 0x80;
    Player->KageAng += DEGREES(10);
    Player->Nowsterr = 0;
    Player->Accelcount = 0.0f;
    Player->Force /= 2;
    if (Player->Slipcount >= 0x2000) {
        Player->Slipcount = 0;
        --Player->RolioverTime;
        if (Player->RolioverTime == 0) {
            Player->SlipFlag &= ~ROLLOVER_FALL;
            ExplorerSetJugemu(Player);
            ExplorerCallJugemu(Player);
        }
    }
}

void SetRolloverFall(Player* Player, s8 playerIndex) {
    if ((((Player->SlipFlag & SPIN_L) != 0) || ((Player->SlipFlag & SPIN_R) != 0) ||
         ((Player->SlipFlag & EXPLODE)) || ((Player->SlipFlag & THROW_EXPLODE)) ||
         ((Player->SlipFlag & ROLLOVER) != 0)) &&
        (GameMode == BATTLE)) {
        Player->HandlingFlag |= BALLOON_CUT;
    }

    KillStrat(Player, playerIndex);
    ResetStar(Player, playerIndex);

    Player->Slipcount = 0;
    Player->SlipFlag |= ROLLOVER_FALL;
    Player->SlipFlag &= ~DRIFT;
    Player->RolioverTime = 0x1E;
    Player->KageAng = 0;

    if (((Player->Flag & IS_PLAYER) != 0) && ((Player->Flag & IS_GHOST) == 0) &&
        ((Player->JugemuFlag & ON_LAKITU_ROD) == 0) && ((Player->WaterFlag & SUBMERGED) == 0) &&
        ((Player->WaterFlag & HALF_SUBMERGED) == 0)) {
        NAPlyVoiceStart(playerIndex, (Player->Kart * 0x10) + 0x29008004);
    }
}

void ResetRolloverFall(Player* Player, s8 playerIndex) {

    if ((Player->HandlingFlag & BALLOON_CUT) != 0) {
        CutPlayerBalloon(Player, playerIndex);
        Player->HandlingFlag &= ~BALLOON_CUT;
    }

    Player->Kartroll = 0;
    Player->Kartpitch = 0;
    Player->SlipFlag &= ~ROLLOVER_FALL;
    Player->Slipcount = 0;
    Player->Slipang = 0;
    Player->Nowsterr = 0;
    Player->RolioverTime = 0;
    Player->Sterrangle = 0;
    Player->Accelcount = 0.0f;

    CompelFlag[0][playerIndex] = 1;
    CompelFlag[1][playerIndex] = 1;
    CompelFlag[2][playerIndex] = 1;
    CompelFlag[3][playerIndex] = 1;
    Player->KageAng = 0;
}

void Star(Player* Player, s8 playerIndex) {
    if (((s32) GameTimer - StarUseCounter[playerIndex]) >= STAR_EFFECT_DURATION - 1) {
        D_8018D900[playerIndex] = 1;

        if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
            ((Player->Flag & IS_GHOST) != IS_GHOST)) {
            if (D_8018D900[playerIndex] == 1) {
                StopStarMusicHook(playerIndex);
                D_8018D900[playerIndex] = 0;
            }
        } else if (D_8018D900[playerIndex] == 1) {
            NaEnmMutekiStop((u8) playerIndex);
            D_8018D900[playerIndex] = 0;
        }
    }

    if (((s32) GameTimer - StarUseCounter[playerIndex]) >= STAR_EFFECT_DURATION) {
        Player->SlipFlag &= ~STAR;
    }
}

// Star Item
void SetStar(Player* Player, s8 playerIndex) {
    KillStrat(Player, playerIndex);

    Player->SlipFlag |= STAR;
    Player->Weapon &= ~USE_STAR;
    StarUseCounter[playerIndex] = GameTimer;
    D_8018D900[playerIndex] = 1;

    if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
        ((Player->Flag & IS_GHOST) != IS_GHOST)) {
        if (D_8018D900[playerIndex] == 1) {
            PlayStarMusicHook(playerIndex);
            D_8018D900[playerIndex] = 2;
        }
        // This may be in charge of ending the Star Sound. Still Unknown though.
    } else if (D_8018D900[playerIndex] == 1) {
        NaEnmMutekiStart(playerIndex);
        D_8018D900[playerIndex] = 2;
    }
}

void ResetStar(Player* Player, s8 playerIndex) {
    Player->SlipFlag &= ~STAR;
    if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
        ((Player->Flag & IS_GHOST) != IS_GHOST)) {
        StopStarMusicHook(playerIndex);
        return;
    }
    NaEnmMutekiStop(playerIndex);
}

void VSGhost(Player* Player, s8 playerIndex) {
    s32 time_elapsed;
    time_elapsed = ((s32) GameTimer) - GhostUseCounter[playerIndex];
    if (time_elapsed < BOO_EFFECT_DURATION) {
        Player->Erase -= 2;

        if (Player->Erase <= ALPHA_BOO_EFFECT) {
            Player->Erase = ALPHA_BOO_EFFECT;
        }
        // Player becomes invisible to other players
        GhostUseTimer[playerIndex] -= 2;
        if (GhostUseTimer[playerIndex] <= 0) {
            GhostUseTimer[playerIndex] = 0;
        }
    } else {
        // Player returns to normal visibility
        Player->Erase += 4;
        if (Player->Erase >= 0xF0) {
            Player->Erase = ALPHA_MAX;
            GhostUseTimer[playerIndex] = ALPHA_MAX;
            Player->SlipFlag &= ~TERESA;
            if ((Player->Flag & IS_PLAYER) != 0) {
                NaPlyObakeStop(playerIndex);
            }
        }

        GhostUseTimer[playerIndex] += 8;
        if (GhostUseTimer[playerIndex] >= 0xF0) {
            GhostUseTimer[playerIndex] = ALPHA_MAX;
            Player->Erase = ALPHA_MAX;
            Player->SlipFlag &= ~TERESA;
            if ((Player->Flag & IS_PLAYER) != 0) {
                NaPlyObakeStop(playerIndex);
            }
        }
    }
}

void SetVSGhost(Player* Player, s8 playerIndex) {
    s16 temp_v1;
    // become boo
    if ((Player->Flag & IS_PLAYER) != 0) {
        Player->HandlingFlag |= BOO_SMOKE_EFFECT;

        for (temp_v1 = 0; temp_v1 < 10; ++temp_v1) {
            Player->Gass[temp_v1].Flag = 0;
            Player->Gass[temp_v1].Timer = 0;
            Player->Gass[temp_v1].Number = 0;
        }
    }

    KillStrat(Player, playerIndex);

    Player->SlipFlag |= TERESA;
    Player->Weapon &= ~USE_BOO;
    GhostUseCounter[playerIndex] = GameTimer;
    GhostUseTimer[playerIndex] = ALPHA_MAX;

    if ((Player->Flag & IS_PLAYER) != 0) {
        NaPlyObakeStart(playerIndex);
    }
}

void ResetVSGhost(Player* Player, s8 playerIndex) {
    Player->Erase += 8;
    if (Player->Erase >= 0xF0) {
        Player->Erase = ALPHA_MAX;
        GhostUseTimer[playerIndex] = ALPHA_MAX;

        Player->SlipFlag &= ~TERESA;
        if ((Player->Flag & IS_PLAYER) != 0) {
            NaPlyObakeStop(playerIndex);
        }
    }

    GhostUseTimer[playerIndex] += 0x10;
    if (GhostUseTimer[playerIndex] >= 0xE0) {
        GhostUseTimer[playerIndex] = ALPHA_MAX;
        Player->Erase = ALPHA_MAX;
        Player->SlipFlag &= ~TERESA;
        if ((Player->Flag & IS_PLAYER) != 0) {
            NaPlyObakeStop(playerIndex);
        }
    }
}

void KillBombKart(Player* Player) {
    s32 playerIndex;

    if ((Player->Flag & IS_BOMB) != 0) {
        playerIndex = CheckKartNumber(Player);
        Player->Flag = (IS_PLAYER | NO_CONTROLS | IS_CPU_PLAYER);
        KwfinishBombkart(playerIndex);
    }
}

void ChangeBomb(Player* Player, s8 arg1) {
    Player->Erase -= 4;
    if (Player->Erase < 5) {
        Player->Erase = ALPHA_MIN;
        Player->Weapon &= ~PRESS_BECOME_BOMB;
        Player->Weapon |= PRESS_IS_BOMB;
        Player->Flag |= IS_BOMB;

        SetRealBomb(Player, arg1);
        KwstartBombkart(arg1);
    }
}

void RealBomb(Player* Player, s8 playerIndex) {
    Player->Erase += 2;
    if (Player->Erase >= 0xF0) {
        Player->Erase = ALPHA_MAX;
        Player->Weapon &= ~PRESS_IS_BOMB;
    }

    KwsetBombkartAlpha(playerIndex, (u32) Player->Erase);
}

void SetChangeBomb(Player* Player, UNUSED s8 arg1) {
    s16 temp_v0;

    Player->Weapon |= PRESS_BECOME_BOMB;
    Player->HandlingFlag |= BOO_SMOKE_EFFECT;

    for (temp_v0 = 0; temp_v0 < 10; ++temp_v0) {
        Player->Gass[temp_v0].Flag = 0;
        Player->Gass[temp_v0].Timer = 0;
        Player->Gass[temp_v0].Number = 0;
    }
}
// become Bomb
void SetRealBomb(Player* Player, UNUSED s8 arg1) {
    s16 temp_v0;
    Player->HandlingFlag |= BOO_SMOKE_EFFECT;
    for (temp_v0 = 0; temp_v0 < 10; ++temp_v0) {
        Player->Gass[temp_v0].Flag = 0;
        Player->Gass[temp_v0].Timer = 0;
        Player->Gass[temp_v0].Number = 0;
    }
}

void SetHeightJump(Player* Player, s8 playerIndex) {
    KillStrat(Player, playerIndex);

    Player->SlipFlag &= ~DRIFT;
    Player->JmpAcc = D_800E37F0[Player->Kart];
    Player->JmpSpeed = 0.0f;
    Player->Jump = D_800E37D0[Player->Kart];
    Player->Weapon &= ~UNUSED_TRIGGER_0x10000;
    Player->SlipFlag |= HEIGHT_JUMP;
}

void HeightJump(Player* Player, UNUSED s8 playerIndex) {
    Player->SlipFlag &= ~DRIFT;
    if ((Player->SlipFlag & N_JUMP) != N_JUMP) {
        Player->SlipFlag &= ~HEIGHT_JUMP;
        Player->Accelcount /= 2;
        Player->Force /= 2;
    }
}

void ResetHeightJump(Player* Player, UNUSED s8 playerIndex) {
    Player->SlipFlag &= ~HEIGHT_JUMP;
    Player->JmpAcc = 0.0f;
    Player->Jump = 0.0f;
    Player->JmpSpeed = 0.0f;
}

void MakeChasepoint(Player* Player, s8 PlayerId) {
    s16 pathPoint;

#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (CourseID) {
        case COURSE_BOWSER_CASTLE:
            pathPoint = NearestPathPointByPlayerId[PlayerId];
            if ((pathPoint >= 0x235) && (pathPoint < 0x247)) {
                Player->Chasepoint = 0x214;
            } else if ((pathPoint >= 0x267) && (pathPoint < 0x277)) {
                Player->Chasepoint = 0x25B;
            } else {
                Player->Chasepoint = NearestPathPointByPlayerId[PlayerId];
                if (Player->Chasepoint < 0) {
                    Player->Chasepoint = CourseTotalPathPoints[0] + Player->Chasepoint;
                }
            }
            break;
        case COURSE_BANSHEE_BOARDWALK:
            pathPoint = NearestPathPointByPlayerId[PlayerId];
            if ((pathPoint >= 0x12C) && (pathPoint < 0x13C)) {
                Player->Chasepoint = 0x12CU;
            } else {
                Player->Chasepoint = NearestPathPointByPlayerId[PlayerId];
                if (Player->Chasepoint < 0) {
                    Player->Chasepoint = CourseTotalPathPoints[0] + Player->Chasepoint;
                }
            }
            break;
        case COURSE_YOSHI_VALLEY:
        case COURSE_RAINBOW_ROAD:
            Player->Chasepoint = PlayerPathPointCopy[PlayerId];
            break;
        case COURSE_FRAPPE_SNOWLAND:
            pathPoint = NearestPathPointByPlayerId[PlayerId];
#ifdef VERSION_EU
            if (((pathPoint >= 0xF0) && (pathPoint < 0x11E)) || ((PlayerPathPointCopy[PlayerId] >= 0xF0) &&
                                                                 (PlayerPathPointCopy[PlayerId] < 0x11E)))
#else
            if ((pathPoint >= 0xF0) && (pathPoint < 0x105))
#endif
            {
                Player->Chasepoint = 0xF0U;
            } else {
                Player->Chasepoint = PlayerPathPointCopy[PlayerId];
                if (Player->Chasepoint < 0) {
                    Player->Chasepoint = CourseTotalPathPoints[0] + Player->Chasepoint;
                }
            }
            break;
        case COURSE_ROYAL_RACEWAY:
            pathPoint = NearestPathPointByPlayerId[PlayerId];
            if ((pathPoint >= 0x258) && (pathPoint < 0x2A4)) {
                Player->Chasepoint = 0x258U;
            } else {
                Player->Chasepoint = PlayerPathPointCopy[PlayerId];
                if (Player->Chasepoint < 0) {
                    Player->Chasepoint = CourseTotalPathPoints[0] + Player->Chasepoint;
                }
            }
            break;
        case COURSE_DK_JUNGLE:
            pathPoint = NearestPathPointByPlayerId[PlayerId];
            if ((pathPoint >= 0xB9) && (pathPoint < 0x119)) {
                Player->Chasepoint = 0xB9U;
            } else {
                Player->Chasepoint = NearestPathPointByPlayerId[PlayerId];
                if (Player->Chasepoint < 0) {
                    Player->Chasepoint = CourseTotalPathPoints[0] + Player->Chasepoint;
                }
            }
            break;
        case COURSE_BLOCK_FORT:
        case COURSE_SKYSCRAPER:
        case COURSE_DOUBLE_DECK:
        case COURSE_BIG_DONUT:
            Player->Chasepoint = 0U;
            break;
        default:
            Player->Chasepoint = NearestPathPointByPlayerId[PlayerId];
            if (Player->Chasepoint < 0) {
                Player->Chasepoint = CourseTotalPathPoints[0] + Player->Chasepoint;
            }
            break;
    }
#else

#endif
}

void func_80090178(Player* Player, s8 PlayerId, Vec3f Arg2, Vec3f Arg3) {
    u16 test;
    CenterPathStruct* temp_v1;
    f32 spF8[4] = { 0.0f, 0.0f, -700.0f, 700.0f };
    f32 spE8[4] = { 700.0f, -700.0f, 0.0f, 0.0f };
    f32 spD8[4] = { 0.0f, 0.0f, -650.0f, 650.0f };
    f32 spC8[4] = { 650.0f, -650.0f, 0.0f, 0.0f };
    f32 spB8[4] = { 0.0f, 0.0f, -400.0f, 400.0f };
    f32 spA8[4] = { 400.0f, -400.0f, 0.0f, 0.0f };
    f32 sp98[4] = { 0.0f, 0.0f, -350.0f, 350.0f };
    f32 sp88[4] = { 350.0f, -350.0f, 0.0f, 0.0f };
    f32 sp78[4] = { 0.0f, 0.0f, -675.0f, 675.0f };
    f32 sp68[4] = { 675.0f, -675.0f, 0.0f, 0.0f };
    f32 sp58[4] = { 0.0f, 0.0f, -550.0f, 550.0f };
    f32 sp48[4] = { 550.0f, -550.0f, 0.0f, 0.0f };
    f32 sp38[4] = { 0.0f, 0.0f, -575.0f, 575.0f };
    f32 sp28[4] = { 575.0f, -575.0f, 0.0f, 0.0f };
    f32 sp18[4] = { 10.0f, -10.0f, -575.0f, 575.0f };
    f32 sp08[4] = { 575.0f, -575.0f, 10.0f, -10.0f };

    switch (CourseID) {
        case COURSE_YOSHI_VALLEY:
            test = Player->Chasepoint;
            temp_v1 = &CenterPathBP[JugemuLine[PlayerId]][test];
            Arg2[0] = temp_v1->Pointx;
            Arg2[1] = temp_v1->Pointy;
            Arg2[2] = temp_v1->Pointz;
            temp_v1 = &CenterPathBP[JugemuLine[PlayerId]]
                                  [(Player->Chasepoint + 5) %
                                   (CourseTotalPathPoints[JugemuLine[PlayerId]] + 1)];
            Arg3[0] = temp_v1->Pointx;
            Arg3[1] = temp_v1->Pointy;
            Arg3[2] = temp_v1->Pointz;
            break;
        case COURSE_BLOCK_FORT:
            Arg2[0] = spF8[PlayerId];
            Arg2[1] = 0.0f;
            Arg2[2] = spE8[PlayerId];
            Arg3[0] = spD8[PlayerId];
            Arg3[1] = 0.0f;
            Arg3[2] = spC8[PlayerId];
            break;
        case COURSE_SKYSCRAPER:
            Arg2[0] = spB8[PlayerId];
            Arg2[1] = 480.0f;
            Arg2[2] = spA8[PlayerId];
            Arg3[0] = sp98[PlayerId];
            Arg3[1] = 480.0f;
            Arg3[2] = sp88[PlayerId];
            break;
        case COURSE_DOUBLE_DECK:
            Arg2[0] = sp78[PlayerId];
            Arg2[1] = 0.0f;
            Arg2[2] = sp68[PlayerId];
            Arg3[0] = sp58[PlayerId];
            Arg3[1] = 0.0f;
            Arg3[2] = sp48[PlayerId];
            break;
        case COURSE_BIG_DONUT:
            Arg2[0] = sp38[PlayerId];
            Arg2[1] = 200.0f;
            Arg2[2] = sp28[PlayerId];
            Arg3[0] = sp18[PlayerId];
            Arg3[1] = 200.0f;
            Arg3[2] = sp08[PlayerId];
            break;
        default:
            test = Player->Chasepoint;
            temp_v1 = &CenterPathBP[0][test];
            Arg2[0] = temp_v1->Pointx;
            Arg2[1] = temp_v1->Pointy;
            Arg2[2] = temp_v1->Pointz;
            temp_v1 = &CenterPathBP[0][(Player->Chasepoint + 5) % (CourseTotalPathPoints[0] + 1)];
            Arg3[0] = temp_v1->Pointx;
            Arg3[1] = temp_v1->Pointy;
            Arg3[2] = temp_v1->Pointz;
            break;
    }
}

void SetLakitu(Player* Player) {
    s32 playerIndex = CheckKartNumber(Player);

    Player->Sterrangle = 0;
    Player->Nowsterr = 0;
    Player->Slipang = 0;
    Player->JugemuFlag |= LAKITU_SCENE;
    Player->SlipFlag &= ~DRIFT;
    Player->Hangflag = 0;
    Player->Force = 0.0f;

    KillStrat(Player, playerIndex);
    ResetStar(Player, playerIndex);

    Player->sus.GachonTimer = 0;
    Player->Jumpcount = 0;
    Player->sus.GachonFirstspeed = 0.0f;
    if ((Player->SlipFlag & THUNDER) == THUNDER) {
        if ((Player->SlipFlag & THUNDER_SPIN) == THUNDER_SPIN) {
            Player->SlipFlag &= ~THUNDER_SPIN;
            Player->Slipcount = 0;
            Player->Nowsterr = 0;
            Player->Slipang = 0;
            Player->Direction[1] = Player->OldDirection;
        }
        ResetThunder(Player, playerIndex);
    }
    Player->SlipFlag &= ~SPIN_TURN;
}

void func_80090868(Player* Player) {
    s32 playerIndex;

    Player->Sterrangle = 0;
    Player->Nowsterr = 0;
    Player->Slipang = 0;
    Player->Force = 0.0f;
    playerIndex = CheckKartNumber(Player);

    if ((Player->JugemuFlag & ON_LAKITU_ROD) != ON_LAKITU_ROD) {
        Player->sus.Flag = 1;
        Player->sus.Position = 0.0f;
        Player->sus.Accele = 0.5f;
        MakeChasepoint(Player, playerIndex);
        Player->Hangflag = 0;
        Player->JugemuFlag |= ON_LAKITU_ROD;
        Player->JugemuTimer = 0;
        if ((Player->WaterFlag & SUBMERGED) == SUBMERGED) {
            if ((CourseID == COURSE_BOWSER_CASTLE) || (CourseID == COURSE_BIG_DONUT)) {
                Player->JugemuFlag |= LAVA_EFFECT;
            } else {
                Player->JugemuFlag |= WATER_EFFECT;
            }
            // removing the water effect for Sherbet Land makes sense. Perhaps rainbow road and skyscraper
            // had lava instead of an abyss initially?
            if ((CourseID == COURSE_SHERBET_LAND) || (CourseID == COURSE_SKYSCRAPER) ||
                (CourseID == COURSE_RAINBOW_ROAD)) {
                Player->JugemuFlag &= ~(LAVA_EFFECT | WATER_EFFECT);
            }
        }
    }
}

void HangLakitu(Player* Player, s8 PlayerId, s8 Arg2) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    Vec3f sp44;
    Vec3f sp38;
    CenterPathStruct* pathPoint;
    UNUSED s32 stackPadding2;
    UNUSED s32 stackPadding3;

    Player->Jumpcount = 0x000C;
    Player->Sterrangle = 0;
    Player->Nowsterr = 0;
    Player->Slipang = 0;
    Player->Force = 0.0f;
    KillStrat(Player, PlayerId);
    switch (Player->Hangflag) {
        case 0:
            if ((Player->JugemuFlag & IS_IN_WATER) == IS_IN_WATER) {
                if ((Player->JugemuTimer < 0x3C) || ((Player->JugemuFlag & ON_LAKITU_ROD) != ON_LAKITU_ROD)) {
                    Player->JugemuTimer++;
                    if (Player->JugemuTimer >= 0x3C) {
                        Player->JugemuTimer = 0x003C;
                    }
                } else {
                    ChaseFnumber(&Player->Position[1], WaterlevelPlayer[PlayerId] + 100.0f, 0.012f);
                    ChaseSnumber(&Player->Roll[Arg2], 0, 0.2f);
                    if ((WaterlevelPlayer[PlayerId] + 40.0f) <= Player->Position[1]) {
                        Player->Hangflag = 1;
                        Player->JugemuFlag |= IS_FADING_OUT;
                        Player->Erase = 0x00FF;
                    }
                }
            } else if ((Player->JugemuFlag & ON_LAKITU_ROD) == ON_LAKITU_ROD) {
                ChaseFnumber(&Player->Position[1], Player->Ground + 100.0f, 0.025f);
                ChaseSnumber(&Player->Roll[Arg2], 0, 0.2f);
                if ((Player->Ground + 40.0f) <= Player->Position[1]) {
                    Player->Hangflag = 1;
                    Player->JugemuFlag |= IS_FADING_OUT;
                    Player->Erase = 0x00FF;
                }
            }
            if ((Player->SlipFlag & TERESA) == TERESA) {
                ResetVSGhost(Player, PlayerId);
            }
            break;
        case 1:
            if (((Player->Flag & IS_PLAYER) == IS_PLAYER) && ((Player->Flag & IS_CPU_PLAYER) == 0)) {
                SetFadeOutScreen(PlayerId, 0xA);
            }
            if ((Player->JugemuFlag & IS_IN_WATER) == IS_IN_WATER) {
                ChaseFnumber(&Player->Position[1], WaterlevelPlayer[PlayerId] + 40.0f, 0.02f);
                Player->Erase -= 8;
                if (Player->Erase < 9) {
                    Player->Erase = 0;
                    Player->Hangflag = 2;
                    Player->JugemuFlag &= ~IS_IN_WATER;
                }
            } else {
                ChaseFnumber(&Player->Position[1], Player->OldPosition[1] + 40.0f, 0.02f);
                Player->Erase -= 8;
                if (Player->Erase < 9) {
                    Player->Erase = 0;
                    Player->Hangflag = 2;
                }
            }
            Player->JugemuFlag &= ~WATER_EFFECT;
            break;
        case 2:
            LakituSpawnBypass(Player, PlayerId, sp44, sp38);
            // Fakematch found by Verti, who knows what's going on here
            Player->Direction[1] = (u16) -CalcDirection(sp44, sp38) & 0xFFFF;
            Player->Position[0] = sp44[0];
            Player->Position[1] = sp44[1] + 40.0f;
            Player->Position[2] = sp44[2];
            Player->Hangflag = 3;
            break;
        case 3:
            OffroadFlagPlayer1[PlayerId] = 0;
            if (((Player->Flag & IS_PLAYER) == IS_PLAYER) && ((Player->Flag & IS_CPU_PLAYER) == 0)) {
                SetFadeInScreen(PlayerId, 0x14);
            }
            LakituSpawnBypass(Player, PlayerId, sp44, sp38);
            Player->Position[0] = sp44[0];
            Player->Position[1] = sp44[1] + 40.0f;
            Player->Position[2] = sp44[2];
            Player->Position[2] = Player->Position[2] + CosT((PlayerId * 0x1C70) - Player->Direction[1]) * -5.0f;
            Player->Position[0] = Player->Position[0] + SinT((PlayerId * 0x1C70) - Player->Direction[1]) * -5.0f;
            Player->Erase += 8;
            if (Player->Erase >= 0xF0) {
                Player->Erase = 0x00FF;
                Player->Hangflag = 4;
                Player->JugemuFlag &= ~IS_FADING_OUT;
                Player->JugemuTimer = 0;
            }
            break;
        case 4:
            if ((Player->JugemuTimer == 0x0096) || (Player->JugemuTimer == 0x00C8) || (Player->JugemuTimer == 0x00FA)) {
                Player->Position[2] = Player->Position[2] + CosT(-Player->Direction[1]) * -10.0f;
                Player->Position[0] = Player->Position[0] + SinT(-Player->Direction[1]) * -10.0f;
            }
            if (Player->JugemuTimer == 0x00FC) {
                pathPoint = CenterPathBP[0];
                Player->Position[0] = pathPoint->Pointx;
                Player->Position[1] = pathPoint->Pointy;
                Player->Position[2] = pathPoint->Pointz;
            }
            ChaseFnumber(&Player->Position[1], (Player->Ground + Player->Radius) - 2.0f, 0.04f);
            Player->JugemuTimer++;
            if (((Player->SlipFlag & N_JUMP) != N_JUMP) || (Player->SlipFlag & CARHIT)) {
                Player->JugemuFlag &= ~LAVA_EFFECT;
                if (Player->JugemuTimer >= 0x5B) {
                    if (Player->Flag & IS_PLAYER) {
                        NaPlyLevelStop(PlayerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x28));
                    }
                    if (GameMode == BATTLE) {
                        CutPlayerBalloon(Player, PlayerId);
                    }
                    Player->JugemuFlag &= ~ON_LAKITU_ROD;
                    Player->WaterFlag &= ~SPLASH_START;
                    if ((Player->JugemuFlag & HAS_ICE_CUBE) != HAS_ICE_CUBE) {
                        Player->JugemuFlag &= ~LAKITU_SCENE;
                        if ((Player->AccMaxcount * 0.9) <= Player->Accelcount) {
                            SetRapidacc(Player, PlayerId);
                        }
                    }
                }
            }
            break;
    }
    Player->sus.Accele += 8.0f;
    if (Player->sus.Accele >= 180.0f) {
        Player->sus.Accele = 180.0f;
    }
    if (Player->sus.Flag == 1) {
        Player->sus.Position += Player->sus.Accele;
        if (Player->sus.Position >= (f32) DEGREES(10)) {
            Player->sus.Accele = 0.0f;
            Player->sus.Flag *= -1;
        }
    }
    if (Player->sus.Flag == -1) {
        Player->sus.Position -= Player->sus.Accele;
        if (Player->sus.Position <= (f32) -DEGREES(10)) {
            Player->sus.Accele = 0.0f;
            Player->sus.Flag *= -1;
        }
    }
}

#define BLOCK_ITEM_USE_EFFECTS                                                                                        \
    THUNDER | HEIGHT_JUMP | RESTORE | BROKEN | THROW_EXPLODE |          \
        EXPLODE | STORM | DASH_JUMP | THUNDER_SPIN |      \
        ROLLOVER_FALL | RAPID_ACC | WHEELSPIN | ROLLOVER | \
        STAR | SPIN_L | SPIN_R | DASH_MIDJUMP

bool CheckItembutton(Player* Player) {
    s32 phi_v0 = 0;
    if ((((((Player->JugemuFlag & ON_LAKITU_ROD) == ON_LAKITU_ROD) ||
           ((Player->JugemuFlag & LAKITU_SCENE) == LAKITU_SCENE)) ||
          ((Player->Flag & IS_BOMB) != 0)) ||
         ((Player->Flag & IS_RACE_FINISH) != 0)) ||
        ((Player->Flag & EXISTS) == 0)) {
        return true;
    }

    switch (Player->Item) {
        case ITEM_MUSHROOM:
        case ITEM_DOUBLE_MUSHROOM:
        case ITEM_TRIPLE_MUSHROOM:
        case ITEM_SUPER_MUSHROOM:
            if ((Player->SlipFlag & N_JUMP) != 0) {
                return true;
            }
            phi_v0 = BLOCK_ITEM_USE_EFFECTS;
            goto prevent_item_use_label;
        case ITEM_STAR:
            phi_v0 = TERESA | BLOCK_ITEM_USE_EFFECTS;
        case ITEM_BOO:
            phi_v0 = phi_v0 | (TERESA | BLOCK_ITEM_USE_EFFECTS);
        prevent_item_use_label:
        default:
            if ((Player->SlipFlag & phi_v0) != 0) {
                return true;
            }
            return false;
    }
}

//UNUSED
void SetResultJump(Player* Player, s8 arg1) {
    s32 temp_v0;

    Player->OldDirection = Player->Direction[1];
    Player->HandlingFlag |= (TROPHYJUMP_STARS | TROPHYJUMP_END);
    Player->HandlingFlag &= ~TROPHYJUMP_START;
    Player->HandlingFlag |= TROPHYJUMP_KEEP;
    Player->JmpAcc = 0.002f;
    Player->JmpSpeed = 0.0f;
    Player->Jump = 2.6f;
    Player->SpinTimer = 2;
    Player->Slipang = 0;
    Player->Nowsterr = 0;
    Player->Sterrangle = 0;
    D_8018D920[arg1] = 0;

    Player->Spark[1].Number = 0;
    Player->Spark[1].Timer = 0;
    Player->Spark[1].Flag = 0;
    Player->Spark[0].Number = 0;
    Player->Spark[0].Timer = 0;
    Player->Spark[0].Flag = 0;

    // clang-format off
    temp_v0 = 2; do {
        // clang-format on
        Player->Spark[1 + temp_v0].Flag = 0;
        Player->Spark[1 + temp_v0].Timer = 0;
        Player->Spark[1 + temp_v0].Number = 0;
        Player->Spark[2 + temp_v0].Flag = 0;
        Player->Spark[2 + temp_v0].Timer = 0;
        Player->Spark[2 + temp_v0].Number = 0;
        Player->Spark[3 + temp_v0].Flag = 0;
        Player->Spark[3 + temp_v0].Timer = 0;
        Player->Spark[3 + temp_v0].Number = 0;

        temp_v0 += 4;
        Player->Letter[6 + temp_v0].Flag = 0;
        Player->Letter[6 + temp_v0].Timer = 0;
        Player->Letter[6 + temp_v0].Number = 0;
    } while (temp_v0 < 10);
}

// Unused
void ResultJump(Player* Player, s8 arg1) {
    s16 var_v1;
    UNUSED s32 stackPadding1;
    Vec3f spC = { 27.167f, 25.167f, 23.167f };

    Player->HandlingFlag |= TROPHYJUMP_KEEP;
    if (Player->SpinTimer == 0) {
        var_v1 = 0;
    } else {
        Player->Direction[1] -= DEGREES(20);
        D_8018D920[arg1] -= DEGREES(20);
        var_v1 = (u16) D_8018D920[arg1] / DEGREES(20);
    }
    if (((var_v1 == 9) && (Player->SpinTimer == 1)) || ((var_v1 == 0) && (Player->SpinTimer == 2)) ||
        (Player->SpinTimer == 0)) {
        Player->SpinTimer--;
        if (Player->SpinTimer <= 0) {
            Player->SpinTimer = 0;
        }
        if (Player->SpinTimer == 0) {
            if ((Player->Position[1] - (Player->Radius + 1.0f)) <= spC[arg1]) {
                Player->Position[1] = (f32) ((f64) (spC[arg1] + Player->Radius) + 1.08);
                Player->sus.BoundTimer = 0;
                Player->Slipcount = 0;
                Player->Nowsterr = 0;
                Player->Slipang = 0;
                Player->sus.BoundFirstspeed = 3.0f;
                Player->HandlingFlag &= ~TROPHYJUMP_END;
                Player->Gravity = Gravity1[Player->Kart];
                Player->Pitch[0] = 0;
                Player->Flag |= NO_CONTROLS;
                Player->Speed = 0.0f;
                Player->Force = 0.0f;
                Player->Accelcount = 0.0f;
                if (arg1 == 0) {
                    D_801658BC = 1;
                }
            }
        }
    }
}

void ResultKartJumpStart(s8 arg0) {
    if ((GamePlayers[arg0].HandlingFlag & TROPHYJUMP_END) == 0) {
        GamePlayers[arg0].HandlingFlag |= (TROPHYJUMP_KEEP | TROPHYJUMP_START);
        GamePlayers[arg0].Flag &= ~NO_CONTROLS;
    }
}

#include <ultra64.h>
#include <macros.h>
#include <common_structs.h>
#include <defines.h>
#include <sounds.h>
#include <mk64.h>
#include <course.h>

#include "camera.h"
#include "path.h"
#include "replays.h"
#include "main.h"
#include "code_800029B0.h"
#include "code_80057C60.h"
#include "update_objects.h"
#include "menu_items.h"
#include "cpu_vehicles_camera_path.h"
#include "spawn_players.h"
#include "audio/external.h"
#include "race_logic.h"
#include "skybox_and_splitscreen.h"
#include "math_util_2.h"
#include "effects.h"
#include "math.h"
#include "menus.h"
#include "seq_ids.h"
#include "OverKartHooks.h"
#include "OverKartStruct.h"

extern SaveData SaveGame;

#pragma intrinsic(sqrtf)

extern s16 BalloonCount[];
extern s16 RankUpdateFinishFlag;

extern s32 LapCountByPlayerId[];
extern u16 Check2PGP;

extern s32 D_8018D2AC;
extern s32 D_802B91E0;

u16 Kawanowait;
u16 SoundsetFlag;

float Screenzoom;

s32 FinishCount;
UNUSED s32 D_802BA03C;

s16 Savestatus[4];
u16 Check2PGP;

void SaveKeyStatus(void) {
    struct Controller* controllers = &Controllers[0];
    s32 i;
    for (i = 0; i < 4; i++) {
        Savestatus[i] = controllers->ButtonHeld;
        controllers++;
    }
}

void LoadKeyStatus(void) {
    struct Controller* controllers = &Controllers[0];
    s32 i;
    for (i = 0; i < 4; i++) {
        controllers->ButtonPressed = (controllers->ButtonHeld & (Savestatus[i] ^ controllers->ButtonHeld));
        controllers->ButtonReleased = (Savestatus[i] & (Savestatus[i] ^ controllers->ButtonHeld));
        controllers->ButtonHeld = Savestatus[i];
        controllers++;
    }
}

void BattleEnd(void) {

    switch (PlayerCount) {
        case 2:
            *(Battlewincount2P + WinKart) += 1;
            break;
        case 3:
            *(Battlewincount3P + WinKart) += 1;
            break;
        case 4:
            *(Battlewincount4P + WinKart) += 1;
            break;
    }
    NaSeMuteGoal((u8) WinKart);
    StartingIndicator = RACE_DONE;
    FinishCount = 10;
}

void CheckBattleStatus(void) {
    Player* Player;
    s32 playerIndex;
    s16 playersAlive[4];
    s16 playersDead[4];
    s16 aliveCounter = 0;
    s16 deadCounter = 0;

    for (playerIndex = 0; playerIndex < 4; playerIndex++) {
        Player = &GamePlayers[playerIndex];
        if (!(Player->Flag & EXISTS)) {
            continue;
        }
        if (Player->Flag & IS_RACE_FINISH) {
            continue;
        }
        // If Player has no balloons Left
        if (BalloonCount[playerIndex] < 0) {
            Player->Flag |= IS_RACE_FINISH;
            playersDead[deadCounter] = (s16) (Player - PlayerOne);
            deadCounter++;
            NaSeMuteGoal((u8) playerIndex); // play sad character Sound?
        } else {
            playersAlive[aliveCounter] = (s16) (Player - PlayerOne);
            aliveCounter++;
        }
    }
    if (aliveCounter == 1) {
        WinKart = (s32) playersAlive[0];
        BattleEnd();
    } else if (aliveCounter == 0) {
        WinKart = (s32) playersDead[0];
        BattleEnd();
    }
}

void CheckGoalinTime(void) {
    f32 temp_v0;
    s32 i;
    u16 temp_a2;

    for (i = 0; i < NUM_PLAYERS; i++) {

        if ((GamePlayers[i].Flag & IS_RACE_FINISH)) {
            continue;
        }
        temp_a2 = CurrentPathID[i];

        temp_v0 = ((2 - GamePlayers[i].rap) * CourseTotalPathPoints[temp_a2]);
        temp_v0 += CourseTotalPathPoints[temp_a2] * (1.0f - RateCountF[i]);
        temp_v0 /= 15.0f;

        TimePlayerLastTouchedFinishLine[i] = GameTimer + temp_v0;
    }
    RankUpdateFinishFlag = 1;
    SortGoalRank();
}

void CheckFinish(void) {

    /* GameCode skips fade frames, so EndRace never saw StartingIndicator 7 on a GP win. */
    EndRace();
    RaceStatus = 7;

    if (RaceClearFlag) {

        if (CourseSelect == AsmCupCount) {
            NextSequenceMode = ENDING;
        } else {
            CupNumber++;
            CourseSelect++;
            NextSequenceMode = RACING;
        }
    } else {
        CupNumber++;
        CourseSelect++;
        NextSequenceMode = RACING;
    }
}

void FinishVSZoom(void) {
    Screen* temp_v0 = &D_8015F480[WinKart];
    s32 phi_v1_4;

    ClearCFBFlag = 1;

    switch (FinishZoomFlag) {
        case 0:
            TwoDFlag = 0;
            FinishZoomFlag = 1;
            if (PlayerCount == 3) {
                EraseInfo3P();
            }
            break;
        case 1:
            if (temp_v0->Width < SCREEN_WIDTH) {
                temp_v0->Width += 2;
            }
            if (temp_v0->Height < SCREEN_HEIGHT) {
                temp_v0->Height += 2;
            }
            if (temp_v0->Posx < 160) {
                temp_v0->Posx += 1;

            } else if (temp_v0->Posx > 160) {
                temp_v0->Posx -= 1;
            }
            if (temp_v0->Posy < 120) {
                temp_v0->Posy += 1;
            } else if (temp_v0->Posy > 120) {
                temp_v0->Posy -= 1;
            }
            phi_v1_4 = 0;

            if (temp_v0->Height >= SCREEN_HEIGHT) {
                phi_v1_4++;
                temp_v0->Height = SCREEN_HEIGHT;
            }
            if (temp_v0->Width >= SCREEN_WIDTH) {
                temp_v0->Width = SCREEN_WIDTH;
                phi_v1_4++;
            }

            if (temp_v0->Posy == 120) {
                phi_v1_4++;
            }
            if (temp_v0->Posx == 160) {
                phi_v1_4++;
            }
            AspectRatio = (f32) ((f32) temp_v0->Width / (f32) temp_v0->Height);
            if (phi_v1_4 == 4) {
                FinishZoomFlag = 2;
                ScreenSplitA = SCREEN_MODE_1P;
                D_800DC5EC->Width = temp_v0->Width;
                D_800DC5EC->Height = temp_v0->Height;
                D_800DC5EC->Posx = temp_v0->Posx;
                D_800DC5EC->Posy = temp_v0->Posy;
                if (GameMode == BATTLE) {
                    DispVSResult();
                } else if (GameMode == VERSUS) {
                    DispVSResult();
                    OgaVSWinnerCameraInit();
                } else {
                    DispGPLose();
                    StartingIndicator = RACE_QUADRANT_RESULTS;
                }
            }
            break;
        case 2:
            break;
    }
}

void FinishZoom(void) {
    s32 phi_a0_10 = 0;

    ClearCFBFlag = 1;

    switch (FinishZoomFlag) {
        case 0:
            // Unused switch?
            switch (GameMode) {
                case GRAND_PRIX:
                case VERSUS:
                    break;
                case TIME_TRIALS:
                    break;
            }
            TwoDFlag = 0;
            switch (ScreenSplitB) {
                case SCREEN_MODE_1P:
                    FinishZoomFlag = 1;
                    break;
                case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                    FinishZoomFlag = 5;
                    break;
                case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                    FinishZoomFlag = 6;
                    break;
            }
            break;
        case 5:
            D_800DC5EC->Width -= 4;

            D_800DC5F0->Width -= 4;

            D_800DC5EC->Posx -= 2;

            D_800DC5F0->Posx += 2;

            if (D_800DC5EC->Width < 160) {
                D_800DC5EC->Width = 160;
                phi_a0_10++;
            }

            if (D_800DC5F0->Width < 160) {
                D_800DC5F0->Width = 160;
                phi_a0_10++;
            }

            if (D_800DC5EC->Posx < 80) {
                D_800DC5EC->Posx = 80;
                phi_a0_10++;
            }

            if (D_800DC5F0->Posx > SCREEN_HEIGHT) {
                D_800DC5F0->Posx = SCREEN_HEIGHT;
                phi_a0_10++;
            }

            AspectRatio = (f32) ((f32) D_800DC5EC->Width / (f32) D_800DC5EC->Height);
            if (phi_a0_10 == 4) {
                FinishZoomFlag = 3;
                DispResultTime();
                OgaDriversPointCameraInit();
                KwswitchDemo2Pgp();
            }
            break;
        case 6:
            D_800DC5EC->Height -= 4;
            D_800DC5F0->Height -= 4;
            D_800DC5EC->Posy -= 2;
            D_800DC5F0->Posy += 2;

            if (D_800DC5EC->Height < 120) {
                D_800DC5EC->Height = 120;
                phi_a0_10++;
            }

            if (D_800DC5F0->Height < 120) {
                D_800DC5F0->Height = 120;
                phi_a0_10++;
            }

            if (D_800DC5EC->Posy < 60) {
                D_800DC5EC->Posy = 60;
                phi_a0_10++;
            }

            if (D_800DC5F0->Posy > 180) {
                D_800DC5F0->Posy = 180;
                phi_a0_10++;
            }

            AspectRatio = (f32) ((f32) D_800DC5EC->Width / (f32) D_800DC5EC->Height);
            if (phi_a0_10 == 4) {
                FinishZoomFlag = 3;
                DispResultTime();
                OgaDriversPointCameraInit();
            }
            break;
        case 1:
            D_800DC5EC->Height -= 2;
            D_800DC5EC->Width = (D_800DC5EC->Height * SCREEN_WIDTH) / SCREEN_HEIGHT;

            if (D_800DC5EC->Height < 120) {

                D_800DC5EC->Height = 120;
                D_800DC5EC->Width = (D_800DC5EC->Height * SCREEN_WIDTH) / SCREEN_HEIGHT;
                FinishZoomFlag = 2;

                D_800DC5F0->Width = D_800DC5EC->Width;
                D_800DC5F0->Height = D_800DC5EC->Height;
                D_800DC5F0->Posx = D_800DC5EC->Posx;
                D_800DC5F0->Posy = D_800DC5EC->Posy;

                ScreenSplitA = SCREEN_MODE_2P_SPLITSCREEN_VERTICAL;
                AspectRatio = 1.33333337;
                PlayerCount = 2;
                InitResultKart();
                KWResultscreenStart();
            }
            break;
        case 2:
            D_800DC5EC->Posx -= 4;

            D_800DC5EC->Posy -= 2;

            if (D_800DC5EC->Posx < 80) {
                D_800DC5EC->Posx = 80;
                phi_a0_10++;
            }

            if (D_800DC5EC->Posy < 60) {
                D_800DC5EC->Posy = 60;
                phi_a0_10++;
            }
            D_800DC5F0->Posx += 4;
            D_800DC5F0->Posy += 2;

            if (D_800DC5F0->Posx > SCREEN_HEIGHT) {
                D_800DC5F0->Posx = SCREEN_HEIGHT;
                phi_a0_10++;
            }
            if (D_800DC5F0->Posy > 180) {
                D_800DC5F0->Posy = 180;
                phi_a0_10++;
            }
            if (phi_a0_10 == 4) {
                FinishZoomFlag = 7;
                Kawanowait = 3;
            }
            break;
        case 7:
            Kawanowait--;
            if (Kawanowait == 0) {
                FinishZoomFlag = 3;
                DispResultTime();
                if (GameMode == GRAND_PRIX) {
                    OgaDriversPointCameraInit();
                } else {
                    OgaTimeattackCameraInit();
                }
            }
            break;
        case 4:
            FadeOutFlag = 1;
            FadeOutCounter = 5;
            StartingIndicator = RACE_QUADRANT_RESULTS;
            CheckFinish();
            break;
    }
}

UNUSED void CallNextSequence(s32 arg0) {
    NextSequenceMode = arg0;
    StartingIndicator = RACE_QUITTING;
    NaSeqFadeout(25);
    NaSEFadeout(25);
    BackgroundFlag = 1;
    ClearCFBFlag = 1;
    TwoDFlag = 0;
    FinishCount = 5;
}

void StartRaceBGM(s32 track) {

    if ((OKMusicIn3P4P == 0) && (ScreenSplitB == SCREEN_MODE_3P_4P_SPLITSCREEN)) {
        return; // If 3P/4P splitscreen Mode is currently on, don't play the music for the Current track.
    }

    SetMusicVolume();

    if (StartRaceBGMCustom() != 0) {
        return;
    }

    switch (track) {
        case COURSE_MARIO_RACEWAY: // Raceways, Wario Stadium
        case COURSE_ROYAL_RACEWAY:
        case COURSE_LUIGI_RACEWAY:
        case COURSE_WARIO_STADIUM:
            NaSeqStart(SEQ_TRACK_RACEWAY);
            break;

        case COURSE_TOADS_TURNPIKE: // Toad's Turnpike
            NaSeqStart(SEQ_TRACK_TURNPIKE);
            break;

        case COURSE_YOSHI_VALLEY: // Yoshi Valley, Moo Moo Farm
        case COURSE_MOO_MOO_FARM:
            NaSeqStart(SEQ_TRACK_FARM);
            break;

        case COURSE_CHOCO_MOUNTAIN: // Choco Mountain, Various Battle Stages
        case COURSE_BLOCK_FORT:
        case COURSE_DOUBLE_DECK:
            NaSeqStart(SEQ_TRACK_MOUNTAIN);
            break;

        case COURSE_KALAMARI_DESERT: // Kalimari Desert
            NaSeqStart(SEQ_TRACK_DESERT);
            break;

        case COURSE_KOOPA_BEACH: // Koopa Troopa Beach
            NaSeqStart(SEQ_TRACK_BEACH);
            break;

        case COURSE_BOWSER_CASTLE: // Bowser Castle
            NaSeqStart(SEQ_TRACK_CASTLE);
            break;

        case COURSE_BANSHEE_BOARDWALK: // Banshee Boardwalk
            NaSeqStart(SEQ_TRACK_SCARY);
            break;

        case COURSE_FRAPPE_SNOWLAND: // Frappe Snowland, Sherbet Land
        case COURSE_SHERBET_LAND:
            NaSeqStart(SEQ_TRACK_SNOW);
            break;

        case COURSE_RAINBOW_ROAD: // Rainbow Road
            NaSeqStart(SEQ_TRACK_RAINBOW);
            break;

        case COURSE_DK_JUNGLE: // DK's Jungle Parkway
            NaSeqStart(SEQ_TRACK_JUNGLE);
            break;

        case COURSE_SKYSCRAPER: // Other Battle Stages
        case COURSE_BIG_DONUT:
            NaSeqStart(SEQ_TRACK_BATTLE);
            break;
		
#ifdef AVOID_UB
		default: //! @BUG: No default case. Enable AVOID_UB for custom tracks.
		    NaSeqStart(SEQ_TRACK_RACEWAY);
			break;
#endif
    }
}

void StartRacing(void) {
    s32 i;

    TimeFlag = -1;
    if (!DemoFlag) { // If we're not in the title screen demo, play the music for the Current track.
        StartRaceBGM(CourseID);
    }

    if (StartingIndicator == RACE_STAGING) {
        StartingIndicator = RACE_IN_PROGRESS;
    }

    for (i = 0; i < NUM_PLAYERS; i++) {

        if ((GamePlayers[i].Flag & EXISTS) == 0) {
            continue;
        }

        // Sets Player to human.
        if (GamePlayers[i].Flag & NO_CONTROLS) {
            GamePlayers[i].Flag ^= NO_CONTROLS;
        }
    }
}

f32 GetLaptime(s32 arg0) {
    f32 temp_v0 = GamePlayers[arg0].Position[2];
    f32 temp_v1 = GamePlayers[arg0].OldPosition[2];
    f32 temp_f14 = GoalBannerPos[2] - temp_v0;
    f32 temp_f16 = temp_v1 - GoalBannerPos[2];
    return GameTimer - ((COURSE_TIMER_ITER_f * temp_f14) / (temp_f14 + temp_f16));
}

void GoalinSubroutin(s32 i) {
    GamePlayers[i].Flag |= IS_RACE_FINISH;
}

void CheckKartRap(void) {
    s16 currentPosition;
    s32 PlayerId;

    for (PlayerId = 0; PlayerId < NUM_PLAYERS; PlayerId++) {
        Player* Player = &GamePlayers[PlayerId];

        if ((Player->Flag & EXISTS) == 0) {
            continue;
        }

        if (LapCountByPlayerId[PlayerId] < Player->rap) {
            Player->rap--;
        } else if (LapCountByPlayerId[PlayerId] > Player->rap) {
            Player->rap++;
            if ((Player->Flag & IS_PLAYER) != 0) {
                // When Player finishes the race
                if (Player->rap == 3) {
                    GoalinSubroutin(PlayerId);

                    currentPosition = Player->Rank;
                    Player->Flag |= IS_CPU_PLAYER;

                    if ((currentPosition < 4) || (SaveGame.GameSettings.GPMode == 1)) {
                        RaceClearFlag = 1;
                    }

                    NaSeMuteGoal((u8) PlayerId);
                    if ((SoundsetFlag & EXISTS) == 0) {
                        SoundsetFlag |= EXISTS;
                    }

                    if (GameMode == GRAND_PRIX && PlayerCount == 2 && Check2PGP == 0) {
                        Check2PGP = 1;
                    }
                    if ((Player->Flag & IS_GHOST) == 0) {
                        StartingIndicator = RACE_HUMAN_FINISHED;
                    }
                    if (GameMode == TIME_TRIALS) {
                        KillGhost(Player);
                    }

                    if (GameMode == VERSUS) {
                        FinishCount = 180;
                        if (currentPosition == 0) {
                            WinKart = PlayerId;
                        }
                        switch (PlayerCount) {
                            case 2:
                                if (currentPosition == 0) {
                                    *(Wincount2P + PlayerId) += 1;
                                }
                                if (*(Wincount2P + PlayerId) > 99) {
                                    *(Wincount2P + PlayerId) = 99;
                                }
                                StartingIndicator = RACE_DONE;
                                PlayerId = RankBuffer[1];
                                GamePlayers[PlayerId].Weapon |= HIT_CRITTER;
                                GamePlayers[PlayerId].Flag |= IS_CPU_PLAYER;
                                NaSeMuteGoal((u8) PlayerId);
                                break;
                            case 3:
                                if (currentPosition < 3) {
                                    *(Wincount3P + PlayerId * 3 + currentPosition) += 1;
                                }
                                if (*(Wincount3P + PlayerId * 3 + currentPosition) > 99) {
                                    *(Wincount3P + PlayerId * 3 + currentPosition) = 99;
                                }
                                /* Because the last Player may not finish, their score must be updated when the 2nd
                                   to last racer finishes. */
                                if (currentPosition == 1) {
                                    StartingIndicator = RACE_DONE; // triggers results screen

                                 /* This messes with the Loop Index by setting it to the Index of the last Player. 
                                    But, because versus always gives the Player with the lower slot/port Number
                                    the advantage if 2 players finish at the same Time,  it can only skip Finished
                                    players who do not need more processing. It can run the same Index twice, but
                                    any Player who Finished this frame already had their lap Count updated, so 
                                    nothing will happen */
                                    PlayerId = RankBuffer[2];
                                    *(Wincount3P + PlayerId * 3 + 2) += 1;
                                    if (*(Wincount3P + PlayerId * 3 + 2) > 99) {
                                        *(Wincount3P + PlayerId * 3 + 2) = 99;
                                    }
                                    GamePlayers[PlayerId].Weapon |= HIT_CRITTER;
                                    GamePlayers[PlayerId].Flag |= IS_CPU_PLAYER;
                                    NaSeMuteGoal((u8) PlayerId);
                                }
                                break;
                            case 4:
                                if (currentPosition < 3) {
                                    *(Wincount4P + PlayerId * 3 + currentPosition) += 1;
                                }
                                if (*(Wincount4P + PlayerId * 3 + currentPosition) > 99) {
                                    *(Wincount4P + PlayerId * 3 + currentPosition) = 99;
                                }
                                // if second to last, race is over
                                if (currentPosition == 2) {
                                    StartingIndicator = RACE_DONE;
                                    PlayerId = RankBuffer[3];
                                    GamePlayers[PlayerId].Weapon |= HIT_CRITTER;
                                    GamePlayers[PlayerId].Flag |= IS_CPU_PLAYER;
                                    NaSeMuteGoal((u8) PlayerId);
                                }
                                break;
                        }
                    }

                } else if (Player->rap == 2) {
                    if ((Player->Flag & 0x100) != 0) {
                        return;
                    }
                    if ((SoundsetFlag & 0x4000) == 0) {
                        SoundsetFlag |= 0x4000;
                        PlayFinalLapMusicHook((u8) PlayerId);
                    }
                }
            } else if (Player->rap == 3) {
                GoalinSubroutin(PlayerId);
                if (GameMode == TIME_TRIALS) {
                    KillGhost(Player);
                }
            }
        }
    }
    if ((Check2PGP != 0) && (Check2PGP != 100)) {
        Check2PGP = 100;
        SortGoalRank2PGP();
    }
}

void func_8028F3E8(void) {
}

// func_8028F3F0
void CheckKartRank(void) {
    s16 PlayerId;
    s16 Position;

    for (PlayerId = 0; PlayerId < NUM_PLAYERS; PlayerId++) {
        if (((GamePlayers[PlayerId].Flag & EXISTS) != 0) && ((GamePlayers[PlayerId].Flag & IS_RACE_FINISH) == 0) &&
            ((GamePlayers[PlayerId].Flag & IS_GHOST) == 0)) {
            Position = GPCurrentRaceRankByPlayerId[PlayerId];
            GamePlayers[PlayerId].Rank = Position;
            RankBuffer[Position] = PlayerId;
        }
    }
}

void EnemyControl(void) {
    s32 i;

    switch (StartingIndicator) {
        case RACE_IN_PROGRESS:
        case RACE_HUMAN_FINISHED:
        case RACE_DONE:
        case RACE_QUADRANT_RESULTS:
            for (i = 0; i < NUM_PLAYERS; i++) {
                DriveEnemyKart(i);
            }
        case RACE_PREP:
        case RACE_STAGING:
            EtcEnemyDrive();
            break;
    }
}

void RaceResetCheck(void) {
    if (DebugMode) {
        if (((ControllerFive->ButtonHeld & R_TRIG) != 0) && ((ControllerFive->ButtonHeld & L_TRIG) != 0) &&
            ((ControllerFive->ButtonHeld & A_BUTTON) != 0) && ((ControllerFive->ButtonHeld & B_BUTTON) != 0)) {

            NaSeqFadeout(0x19);
            NaSEFadeout(0x19);
            NextSequenceMode = START_MENU_FROM_QUIT;
            StartingIndicator = RACE_QUITTING;
            BackgroundFlag = 1;
            ClearCFBFlag = 1;
            TwoDFlag = 0;
            FinishCount = 5;
        }
    }
}

void SetVpScale(void) {
    s16 screenWidth;

    switch (ScreenSplitA) { /* irregular */
        case SCREEN_MODE_1P:
            screenWidth = (s16) (s32) (320.0f * Screenzoom);
            if (screenWidth < 0) {
                screenWidth = 1;
            }
            D_800DC5EC->Width = screenWidth;
            screenWidth = (s16) (s32) (240.0f * Screenzoom);
            if (screenWidth < 0) {
                screenWidth = 1;
            }
            D_800DC5EC->Height = screenWidth;
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            screenWidth = (s16) (s32) (160.0f * Screenzoom);
            if (screenWidth <= 0) {
                screenWidth = 1;
            } else if (screenWidth >= 0x140) {
                screenWidth = 0x013C;
            }
            D_800DC5EC->Width = screenWidth;
            D_800DC5F0->Width = screenWidth;
            screenWidth = (s16) (s32) (240.0f * Screenzoom);
            if (screenWidth <= 0) {
                screenWidth = 1;
            } else if (screenWidth >= 0x1E0) {
                screenWidth = 0x01DC;
            }
            D_800DC5EC->Height = screenWidth;
            D_800DC5F0->Height = screenWidth;
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            screenWidth = (s16) (s32) (320.0f * Screenzoom);
            if (screenWidth <= 0) {
                screenWidth = 1;
            } else if (screenWidth >= 0x280) {
                screenWidth = 0x027C;
            }
            D_800DC5EC->Width = screenWidth;
            D_800DC5F0->Width = screenWidth;
            screenWidth = (s16) (s32) (120.0f * Screenzoom);
            if (screenWidth <= 0) {
                screenWidth = 1;
            } else if (screenWidth >= 0xF0) {
                screenWidth = 0x00EC;
            }
            D_800DC5EC->Height = screenWidth;
            D_800DC5F0->Height = screenWidth;
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            screenWidth = (s16) (s32) (160.0f * Screenzoom);
            if (screenWidth <= 0) {
                screenWidth = 1;
            } else if (screenWidth >= 0x140) {
                screenWidth = 0x013C;
            }
            D_800DC5EC->Width = screenWidth;
            D_800DC5F0->Width = screenWidth;
            D_800DC5F4->Width = screenWidth;
            D_800DC5F8->Width = screenWidth;
            screenWidth = (s16) (s32) (120.0f * Screenzoom);
            if (screenWidth <= 0) {
                screenWidth = 1;
            } else if (screenWidth >= 0xF0) {
                screenWidth = 0x00EC;
            }
            D_800DC5EC->Height = screenWidth;
            D_800DC5F0->Height = screenWidth;
            D_800DC5F4->Height = screenWidth;
            D_800DC5F8->Height = screenWidth;
            break;
    }
}

void ZoomOutScreen(void) {
    Screenzoom = (f32) (Screenzoom - 0.017f);
    if (Screenzoom < 0.0f) {
        Screenzoom = 0.0f;
    }
    SetVpScale();
}

void ZoomInScreen(void) {
    Screenzoom = (f32) (Screenzoom + 0.028f);
    if (Screenzoom > 1.0f) {
        Screenzoom = 1.0f;
    }
    SetVpScale();
}

void CheckPause(void) {
    s32 i;

    if (ReplayFlag) {
        return;
    }

    //! @todo increasing players past four would require increase this Loop iterator.
    for (i = 0; i < 4; i++) {

        Player* Player = &GamePlayers[i];
        struct Controller* controller = &Controllers[i];

        if (!(Player->Flag & IS_PLAYER)) {
            continue;
        }
        if (Player->Flag & IS_CPU_PLAYER) {
            continue;
        }

        if ((OKMusicIn3P4P != 0) || (ScreenSplitA != SCREEN_MODE_3P_4P_SPLITSCREEN)) {
            if ((controller->ButtonPressed & L_TRIG) && !(controller->ButtonHeld & R_TRIG)) {
                controller->ButtonPressed &= ~L_TRIG;

                MusicUserVolumeFlag++;
                if (MusicUserVolumeFlag >= 3) {
                    MusicUserVolumeFlag = 0;
                }
                PlaySound(SOUND_ACTION_PING);
                SetMusicVolume();
            }
        }
        if ((controller->ButtonPressed & START_BUTTON) && (!(controller->ButtonHeld & R_TRIG)) &&
            (!(controller->ButtonHeld & L_TRIG))) {
            SaveKeyStatus();
            PauseFlag = (controller - ControllerOne) + 1;
            controller->ButtonPressed = 0;
            NaPauseSet(1);
            GhostWrite = 1;
            if (GameMode == TIME_TRIALS) {
                if (PlayerOne->Flag & (EXISTS | IS_GHOST)) {
                    KillGhost(PlayerOne);
                }
                if (PlayerTwo->Flag & (EXISTS | IS_GHOST)) {
                    KillGhost(PlayerTwo);
                }
                if (PlayerThree->Flag & (EXISTS | IS_GHOST)) {
                    KillGhost(PlayerThree);
                }
            }
            return;
        }
    }

    if (DebugMode) {
        if (GameMode == BATTLE) {
            // do stuff?
        } else {
            if (ControllerOne->ButtonPressed & U_JPAD) {
                LapCountByPlayerId[0] = 2;
            }
            if (ControllerOne->ButtonPressed & R_JPAD) {
                LapCountByPlayerId[0] = 2;
                LapCountByPlayerId[1] = 2;
            }
            if (ControllerOne->ButtonPressed & D_JPAD) {
                LapCountByPlayerId[0] = 2;
                LapCountByPlayerId[1] = 2;
                LapCountByPlayerId[2] = 2;
                LapCountByPlayerId[3] = 2;
                LapCountByPlayerId[4] = 2;
                LapCountByPlayerId[5] = 2;
                LapCountByPlayerId[6] = 2;
                LapCountByPlayerId[7] = 2;
            }
        }
    }
}

void StartFadeout(void) {
    NextSequenceMode = START_MENU_FROM_QUIT;
    StartingIndicator = RACE_QUITTING;
    NaSeqFadeout(25);
    NaSEFadeout(25);
    BackgroundFlag = 1;
    ClearCFBFlag = 1;
    TwoDFlag = 0;
    FinishCount = 5;
}

#ifdef VERSION_EU
#define gDemoTimer_SIZE 1600
#else
#define gDemoTimer_SIZE 1920
#endif

void CheckTitleDemo(void) {
    if (FinishCount < 0) {
        FinishCount = gDemoTimer_SIZE;
        return;
    }
    FinishCount--;
    if (ControllerFive->ButtonPressed != 0) {
        StartFadeout();
        KBGNumber = START_MENU;
        return;
    }
    if (FinishCount == 0) {
        StartFadeout();
        KBGNumber = LOGO_INTRO_MENU;
    }
}

void RaceControl(void) {
    Player* ply = &GamePlayers[0];
    s32 i;
    u32 phi_v0_4;

    if (DemoPlayFlag) {
        CheckTitleDemo();
    }
    switch (StartingIndicator) {
        case RACE_NONE:
            if (!DemoFlag) { // If we're not in the demo Mode, play the starting fanfare for the Current Mode (Grand Prix / Time Trials / VS / Battle)
                if (GameMode == GRAND_PRIX) {
                    NaFanStart(SEQ_EVENT_RACE_STARTING);
                    PlaySound(SOUND_ACTION_REV_ENGINE);
                    PlaySound(SOUND_ACTION_REV_ENGINE_2);
                } else {
                    NaFanStart(SEQ_EVENT_RACE_STARTING_VS);
                }
            }
            DelayInitialMap();
            StartingIndicator = RACE_PREP;
            D_80150118 = 3.0f;
            DisplayFlag = 0;
            SoundsetFlag = 0;
            TimeFlag = 0;
            GameTimer = 0.0f;
            Timer = 0.0f;
            ClearCFBFlag = 1;
            BackgroundFlag = 1;
            Screenzoom = 0.008f;
            FinishZoomFlag = 0;
            if (ScreenSplitB != SCREEN_MODE_1P) {
                Kwdisplayon(&D_8018D2AC);
            }
            for (i = 0; i < NUM_PLAYERS; i++) {
                if ((ply->Flag & EXISTS) == 0) {
                    continue;
                }
                ply->Flag |= NO_CONTROLS;
                ply++;
            }
            TwoDFlag = 1;
            break;
        case RACE_PREP:
            ZoomInScreen();
            if (Screenzoom == 1.0f) {
                if (ScreenSplitA != SCREEN_MODE_1P) {
                    if (CourseID == COURSE_LUIGI_RACEWAY) {
                        TakeSnapshot9();
                    } else if (CourseID == COURSE_WARIO_STADIUM) {
                        TakeSnapshot15();
                    }
                }
                StartingIndicator = RACE_STAGING;
                ClearCFBFlag = 0;
                TwoDFlag = 1;
                Kwjugemustart();
                if ((GameMode == TIME_TRIALS) && (RomghostStatus == 0)) {
                    phi_v0_4 = 0x1;
                    for (i = 0; i < CourseID; i++) {
                        phi_v0_4 <<= 1;
                    }
                    if ((ReplayFlag == 0) && (!(D_800DC5AC & phi_v0_4))) {
                        DispMessageRomGhost();
                        D_800DC5AC |= phi_v0_4;
                    }
                }
                if (PlayerCount == 3) {
                    DispInfo3P();
                }
            }
            RaceResetCheck();
            break;
        case RACE_STAGING:
            if (DemoFlag) {
                StartRacing();
            }
            if ((DebugMode) && (ControllerFive->ButtonPressed & Z_TRIG)) {
                StartRacing();
            }
            RaceResetCheck();
            break;
        case RACE_IN_PROGRESS:
            if (GameMode == BATTLE) {
                CheckBattleStatus();
            } else {
                CheckKartRank();
                CheckKartRap();
            }
            RaceResetCheck();
            CheckPause();
            break;
        case RACE_HUMAN_FINISHED:

            switch (GameMode) {
                case GRAND_PRIX:
                    RaceResetCheck();
                    CheckKartRank();
                    CheckKartRap();
                    CheckPause();

                    switch (ScreenSplitB) {
                        case SCREEN_MODE_1P:
                            FinishCount = 690;
                            StartingIndicator = RACE_DONE;
                            CheckGoalinTime();
                            break;
                        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                            if (((PlayerOne->Flag & IS_RACE_FINISH) != 0) &&
                                ((PlayerTwo->Flag & IS_RACE_FINISH) != 0)) {

                                if (PlayerOne->Rank < PlayerTwo->Rank) {
                                    WinKart = 1;
                                } else {
                                    WinKart = 0;
                                }

                                CheckGoalinTime();
                                FinishCount = 600;
                                StartingIndicator = RACE_DONE;
                            }
                            break;
                    }
                    break;
                case VERSUS:
                    RaceResetCheck();
                    CheckKartRank();
                    CheckKartRap();
                    CheckPause();
                    break;
                case TIME_TRIALS:
                    FinishCount = 360;
                    if (ReplayFlag != 0) {
                        StartingIndicator = RACE_QUADRANT_RESULTS;
                    } else {
                        StartingIndicator = RACE_DONE;
                    }
                    break;
            }
            break;
        case RACE_DONE:
            if (FinishCount != 0) {
                FinishCount--;
            } else {
                switch (GameMode) {
                    case GRAND_PRIX:
                        if (RaceClearFlag != 0) {
                            FinishZoom();
                        } else if (ScreenSplitB == SCREEN_MODE_1P) {
                            DispGPLose();
                            StartingIndicator = RACE_QUADRANT_RESULTS;
                        } else {
                            FinishVSZoom();
                        }
                        break;
                    case TIME_TRIALS:
                        FinishZoom();
                        break;
                    case VERSUS:
                    case BATTLE:
                        FinishVSZoom();
                        break;
                }
            }
            RaceResetCheck();
            break;
        case RACE_QUITTING:
            ZoomOutScreen();
            if (Screenzoom <= 0) {
                FadeOutFlag = 1;
                FadeOutCounter = 5;
            }
            break;
        case RACE_QUADRANT_RESULTS:
            break;
    }
}

UNUSED void GotoTitle(void) {
    FadeOutFlag = 1;
    FadeOutCounter = 5;
    NextSequenceMode = START_MENU_FROM_QUIT;
}

void GotoGameselect(void) {
    FadeOutFlag = 1;
    FadeOutCounter = 5;
    NextSequenceMode = MAIN_MENU_FROM_QUIT;
}

void GoToGameSelect(void) {
    FadeOutFlag = 1;
    FadeOutCounter = 5;
    NextSequenceMode = PLAYER_SELECT_MENU_FROM_QUIT;
}

void GotoMapselect(void) {
    FadeOutFlag = 1;
    FadeOutCounter = 5;
    NextSequenceMode = COURSE_SELECT_MENU_FROM_QUIT;
}

void GotoRacing(void) {
    FadeOutFlag = 1;
    FadeOutCounter = 5;
    NextSequenceMode = RACING;
}

void CheckCol(Player* playerOne, Player* playerTwo) {
    f32 sp70 = (playerOne->Radius + playerTwo->Radius) - 5.0f;
    f32 temp_f0;
    f32 sp74;
    Vec3f sp60;
    Vec3f sp54;
    f32 temp_f0_2;
    f32 temp_f16;
    f32 temp_f2;

    f32 gFloatArray802B8790[] = { 1.2, 1.0, 0.9, 0.7, 2.0, 1.8, 0.9, 2.3 };

    f32 sp24 = gFloatArray802B8790[playerOne->Kart];
    f32 sp20 = gFloatArray802B8790[playerTwo->Kart];

    sp60[0] = playerOne->Position[0] - playerTwo->Position[0];
    sp60[1] = (playerOne->Position[1] - playerOne->Radius) - (playerTwo->Position[1] - playerTwo->Radius);
    sp60[2] = playerOne->Position[2] - playerTwo->Position[2];

    sp54[0] = playerTwo->Velocity[0] - playerOne->Velocity[0];
    sp54[1] = playerTwo->Velocity[1] - playerOne->Velocity[1];
    sp54[2] = playerTwo->Velocity[2] - playerOne->Velocity[2];

    temp_f0 = sqrtf((sp60[0] * sp60[0]) + (sp60[1] * sp60[1]) + (sp60[2] * sp60[2]));

    if (temp_f0 < 0.1f) {
        return;
    }

    sp74 = temp_f0 - sp70;
    if (sp74 > 0) {
        return;
    }

    if (playerOne->Flag & IS_BOMB) {
        if (playerTwo->Flag & IS_BOMB) {
            KillBombKart(playerOne);
            KillBombKart(playerTwo);
            NAPlyTrgStart((playerTwo - PlayerOne), 0x19008001U);
            return;
        } else {
            playerTwo->Weapon |= HIT_BOMB;
            KillBombKart(playerOne);
            NAPlyTrgStart((playerTwo - PlayerOne), 0x19008001U);
        }
    } else if (playerTwo->Flag & IS_BOMB) {
        playerOne->Weapon |= HIT_BOMB;
        KillBombKart(playerTwo);
        NAPlyTrgStart(playerOne - PlayerOne, 0x19008001U);
        return;
    }
    if (playerOne->SlipFlag & STAR) {
        if (!(playerTwo->SlipFlag & STAR)) {
            playerTwo->Weapon |= HIT_BOMB_THROW;
        }
    } else if (playerTwo->SlipFlag & STAR) {
        playerOne->Weapon |= HIT_BOMB_THROW;
    } else {
        playerOne->SlipFlag |= CARHIT;
        playerTwo->SlipFlag |= CARHIT;
    }
    temp_f0_2 = sqrtf((sp54[0] * sp54[0]) + (sp54[1] * sp54[1]) + (sp54[2] * sp54[2]));
    sp60[0] /= temp_f0;
    sp60[1] /= temp_f0;
    sp60[2] /= temp_f0;
    if (temp_f0_2 < 0.2f) {
        temp_f0 = (playerOne->Radius + playerTwo->Radius) * 0.55f;
        playerOne->Position[0] = playerTwo->Position[0] + (sp60[0] * temp_f0);
        playerOne->Position[1] = playerTwo->Position[1] + (sp60[1] * temp_f0);
        playerOne->Position[2] = playerTwo->Position[2] + (sp60[2] * temp_f0);
        playerTwo->Position[0] -= temp_f0 * sp60[0];
        playerTwo->Position[1] -= temp_f0 * sp60[1];
        playerTwo->Position[2] -= temp_f0 * sp60[2];
        return;
    } else {
        temp_f16 = ((sp60[0] * sp54[0]) + (sp60[1] * sp54[1]) + (sp60[2] * sp54[2])) / temp_f0_2;
    }
    temp_f0_2 = temp_f0_2 * temp_f16 * 0.85;
    if ((playerOne->SlipFlag & STAR) != STAR) {
        temp_f2 = (temp_f0_2 * sp20) / sp24;
        playerOne->Velocity[0] += sp60[0] * temp_f2;
        playerOne->Velocity[1] += sp60[1] * temp_f2;
        playerOne->Velocity[2] += sp60[2] * temp_f2;
        playerOne->Position[0] -= sp60[0] * sp74 * 0.5f;
        playerOne->Position[1] -= sp60[1] * sp74 * 0.5f;
        playerOne->Position[2] -= sp60[2] * sp74 * 0.5f;
    }
    if ((playerTwo->SlipFlag & STAR) != STAR) {
        temp_f2 = (temp_f0_2 * sp24) / sp20;
        playerTwo->Velocity[0] -= sp60[0] * temp_f2;
        playerTwo->Velocity[1] -= sp60[1] * temp_f2;
        playerTwo->Velocity[2] -= sp60[2] * temp_f2;
        playerTwo->Position[0] += sp60[0] * sp74 * 0.5f;
        playerTwo->Position[1] += sp60[1] * sp74 * 0.5f;
        playerTwo->Position[2] += sp60[2] * sp74 * 0.5f;
    }
    if (playerOne->Flag & IS_PLAYER) {
        NAPlyTrgStart((playerOne - PlayerOne), 0x19008001U);
        return;
    }
    if (playerTwo->Flag & IS_PLAYER) {
        NAPlyTrgStart((playerTwo - PlayerOne), 0x19008001U);
    }
}

void CollisionKartToKart(void) {
    Player* ply;
    Player* ply2;
    s32 i;
    s32 k;

    for (i = 0; i < 7; i++) {
        ply = &GamePlayers[i];

        if ((ply->Flag & EXISTS) && (!(ply->SlipFlag & TERESA)) &&
            (!(ply->Flag & IS_GHOST)) && (!(ply->SlipFlag & BROKEN))) {

            for (k = i + 1; k < NUM_PLAYERS; k++) {
                ply2 = &GamePlayers[k];

                if ((ply2->Flag & EXISTS) && (!(ply2->SlipFlag & TERESA)) &&
                    (!(ply2->Flag & IS_GHOST)) && (!(ply2->SlipFlag & BROKEN))) {

                    CheckCol(ply, ply2);
                }
            }
        }
    }
}

void PauseSequence(void) {

    Kwpausesyori();

    switch (ScreenSplitA) {
        case SCREEN_MODE_1P:
            CameraCheckFunc(PlayerOneCopy, Camera1, 0);
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            CameraCheckFunc(PlayerOneCopy, Camera1, 0);
            CameraCheckFunc(PlayerTwoCopy, Camera2, 1);
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            CameraCheckFunc(PlayerOneCopy, Camera1, 0);
            CameraCheckFunc(PlayerTwo, Camera2, 1);
            CameraCheckFunc(PlayerThree, Camera3, 2);
            CameraCheckFunc(PlayerFour, Camera4, 3);
            break;
    }
}

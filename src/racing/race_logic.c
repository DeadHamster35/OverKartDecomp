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

#pragma intrinsic(sqrtf)

extern s16 BalloonCount[];
extern s16 g_rankUpdateFinishFlag;

extern s32 gLapCountByPlayerId[];
extern u16 check_2p_GP;

extern s32 D_8018D2AC;
extern s32 D_802B91E0;

u16 kawanowait;
u16 soundset_flag;

float screenzoom;

s32 finish_count;
UNUSED s32 D_802BA03C;

s16 savestatus[4];
u16 check_2p_GP;

void save_key_status(void) {
    struct Controller* controllers = &gControllers[0];
    s32 i;
    for (i = 0; i < 4; i++) {
        savestatus[i] = controllers->ButtonHeld;
        controllers++;
    }
}

void LoadKeyStatus(void) {
    struct Controller* controllers = &gControllers[0];
    s32 i;
    for (i = 0; i < 4; i++) {
        controllers->ButtonPressed = (controllers->ButtonHeld & (savestatus[i] ^ controllers->ButtonHeld));
        controllers->ButtonReleased = (savestatus[i] & (savestatus[i] ^ controllers->ButtonHeld));
        controllers->ButtonHeld = savestatus[i];
        controllers++;
    }
}

void battle_end(void) {

    switch (g_playerCount) {
        case 2:
            *(battlewincount2p + win_kart) += 1;
            break;
        case 3:
            *(battlewincount3p + win_kart) += 1;
            break;
        case 4:
            *(battlewincount4p + win_kart) += 1;
            break;
    }
    Na_SeMute_Goal((u8) win_kart);
    g_startingIndicator = RACE_DONE;
    finish_count = 10;
}

void check_battle_status(void) {
    Player* player;
    s32 playerIndex;
    s16 playersAlive[4];
    s16 playersDead[4];
    s16 aliveCounter = 0;
    s16 deadCounter = 0;

    for (playerIndex = 0; playerIndex < 4; playerIndex++) {
        player = &gPlayers[playerIndex];
        if (!(player->flag & EXISTS)) {
            continue;
        }
        if (player->flag & IS_RACE_FINISH) {
            continue;
        }
        // If player has no balloons left
        if (BalloonCount[playerIndex] < 0) {
            player->flag |= IS_RACE_FINISH;
            playersDead[deadCounter] = (s16) (player - gPlayerOne);
            deadCounter++;
            Na_SeMute_Goal((u8) playerIndex); // play sad character sound?
        } else {
            playersAlive[aliveCounter] = (s16) (player - gPlayerOne);
            aliveCounter++;
        }
    }
    if (aliveCounter == 1) {
        win_kart = (s32) playersAlive[0];
        battle_end();
    } else if (aliveCounter == 0) {
        win_kart = (s32) playersDead[0];
        battle_end();
    }
}

void check_goalin_time(void) {
    f32 temp_v0;
    s32 i;
    u16 temp_a2;

    for (i = 0; i < NUM_PLAYERS; i++) {

        if ((gPlayers[i].flag & IS_RACE_FINISH)) {
            continue;
        }
        temp_a2 = CurrentPathID[i];

        temp_v0 = ((2 - gPlayers[i].rap) * g_courseTotalPathPoints[temp_a2]);
        temp_v0 += g_courseTotalPathPoints[temp_a2] * (1.0f - rate_count_F[i]);
        temp_v0 /= 15.0f;

        gTimePlayerLastTouchedFinishLine[i] = g_gameTimer + temp_v0;
    }
    g_rankUpdateFinishFlag = 1;
    sort_goal_rank();
}

void CheckFinish(void) {

    if (race_clear_flag) {

        if (g_courseSelect == COURSE_FOUR) {
            next_sequence_mode = ENDING;
        } else {
            cup_number++;
            g_courseSelect++;
            next_sequence_mode = RACING;
        }
    } else {
        cup_number++;
        g_courseSelect++;
        next_sequence_mode = RACING;
    }
}

void finish_vs_zoom(void) {
    Screen* temp_v0 = &D_8015F480[win_kart];
    s32 phi_v1_4;

    g_ClearCFBFlag = 1;

    switch (finish_zoom_flag) {
        case 0:
            two_d_flag = 0;
            finish_zoom_flag = 1;
            if (g_playerCount == 3) {
                EraseInfo_3P();
            }
            break;
        case 1:
            if (temp_v0->width < SCREEN_WIDTH) {
                temp_v0->width += 2;
            }
            if (temp_v0->height < SCREEN_HEIGHT) {
                temp_v0->height += 2;
            }
            if (temp_v0->posx < 160) {
                temp_v0->posx += 1;

            } else if (temp_v0->posx > 160) {
                temp_v0->posx -= 1;
            }
            if (temp_v0->posy < 120) {
                temp_v0->posy += 1;
            } else if (temp_v0->posy > 120) {
                temp_v0->posy -= 1;
            }
            phi_v1_4 = 0;

            if (temp_v0->height >= SCREEN_HEIGHT) {
                phi_v1_4++;
                temp_v0->height = SCREEN_HEIGHT;
            }
            if (temp_v0->width >= SCREEN_WIDTH) {
                temp_v0->width = SCREEN_WIDTH;
                phi_v1_4++;
            }

            if (temp_v0->posy == 120) {
                phi_v1_4++;
            }
            if (temp_v0->posx == 160) {
                phi_v1_4++;
            }
            g_aspectRatio = (f32) ((f32) temp_v0->width / (f32) temp_v0->height);
            if (phi_v1_4 == 4) {
                finish_zoom_flag = 2;
                g_ScreenSplitA = SCREEN_MODE_1P;
                D_800DC5EC->width = temp_v0->width;
                D_800DC5EC->height = temp_v0->height;
                D_800DC5EC->posx = temp_v0->posx;
                D_800DC5EC->posy = temp_v0->posy;
                if (g_gameMode == BATTLE) {
                    DispVSResult();
                } else if (g_gameMode == VERSUS) {
                    DispVSResult();
                    oga_vs_winner_camera_init();
                } else {
                    DispGPLose();
                    g_startingIndicator = RACE_QUADRANT_RESULTS;
                }
            }
            break;
        case 2:
            break;
    }
}

void finish_zoom(void) {
    s32 phi_a0_10 = 0;

    g_ClearCFBFlag = 1;

    switch (finish_zoom_flag) {
        case 0:
            // Unused switch?
            switch (g_gameMode) {
                case GRAND_PRIX:
                case VERSUS:
                    break;
                case TIME_TRIALS:
                    break;
            }
            two_d_flag = 0;
            switch (g_ScreenSplitB) {
                case SCREEN_MODE_1P:
                    finish_zoom_flag = 1;
                    break;
                case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                    finish_zoom_flag = 5;
                    break;
                case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                    finish_zoom_flag = 6;
                    break;
            }
            break;
        case 5:
            D_800DC5EC->width -= 4;

            D_800DC5F0->width -= 4;

            D_800DC5EC->posx -= 2;

            D_800DC5F0->posx += 2;

            if (D_800DC5EC->width < 160) {
                D_800DC5EC->width = 160;
                phi_a0_10++;
            }

            if (D_800DC5F0->width < 160) {
                D_800DC5F0->width = 160;
                phi_a0_10++;
            }

            if (D_800DC5EC->posx < 80) {
                D_800DC5EC->posx = 80;
                phi_a0_10++;
            }

            if (D_800DC5F0->posx > SCREEN_HEIGHT) {
                D_800DC5F0->posx = SCREEN_HEIGHT;
                phi_a0_10++;
            }

            g_aspectRatio = (f32) ((f32) D_800DC5EC->width / (f32) D_800DC5EC->height);
            if (phi_a0_10 == 4) {
                finish_zoom_flag = 3;
                DispResultTime();
                oga_drivers_point_camera_init();
                kwswitch_demo2pgp();
            }
            break;
        case 6:
            D_800DC5EC->height -= 4;
            D_800DC5F0->height -= 4;
            D_800DC5EC->posy -= 2;
            D_800DC5F0->posy += 2;

            if (D_800DC5EC->height < 120) {
                D_800DC5EC->height = 120;
                phi_a0_10++;
            }

            if (D_800DC5F0->height < 120) {
                D_800DC5F0->height = 120;
                phi_a0_10++;
            }

            if (D_800DC5EC->posy < 60) {
                D_800DC5EC->posy = 60;
                phi_a0_10++;
            }

            if (D_800DC5F0->posy > 180) {
                D_800DC5F0->posy = 180;
                phi_a0_10++;
            }

            g_aspectRatio = (f32) ((f32) D_800DC5EC->width / (f32) D_800DC5EC->height);
            if (phi_a0_10 == 4) {
                finish_zoom_flag = 3;
                DispResultTime();
                oga_drivers_point_camera_init();
            }
            break;
        case 1:
            D_800DC5EC->height -= 2;
            D_800DC5EC->width = (D_800DC5EC->height * SCREEN_WIDTH) / SCREEN_HEIGHT;

            if (D_800DC5EC->height < 120) {

                D_800DC5EC->height = 120;
                D_800DC5EC->width = (D_800DC5EC->height * SCREEN_WIDTH) / SCREEN_HEIGHT;
                finish_zoom_flag = 2;

                D_800DC5F0->width = D_800DC5EC->width;
                D_800DC5F0->height = D_800DC5EC->height;
                D_800DC5F0->posx = D_800DC5EC->posx;
                D_800DC5F0->posy = D_800DC5EC->posy;

                g_ScreenSplitA = SCREEN_MODE_2P_SPLITSCREEN_VERTICAL;
                g_aspectRatio = 1.33333337;
                g_playerCount = 2;
                init_result_kart();
                kw_resultscreen_start();
            }
            break;
        case 2:
            D_800DC5EC->posx -= 4;

            D_800DC5EC->posy -= 2;

            if (D_800DC5EC->posx < 80) {
                D_800DC5EC->posx = 80;
                phi_a0_10++;
            }

            if (D_800DC5EC->posy < 60) {
                D_800DC5EC->posy = 60;
                phi_a0_10++;
            }
            D_800DC5F0->posx += 4;
            D_800DC5F0->posy += 2;

            if (D_800DC5F0->posx > SCREEN_HEIGHT) {
                D_800DC5F0->posx = SCREEN_HEIGHT;
                phi_a0_10++;
            }
            if (D_800DC5F0->posy > 180) {
                D_800DC5F0->posy = 180;
                phi_a0_10++;
            }
            if (phi_a0_10 == 4) {
                finish_zoom_flag = 7;
                kawanowait = 3;
            }
            break;
        case 7:
            kawanowait--;
            if (kawanowait == 0) {
                finish_zoom_flag = 3;
                DispResultTime();
                if (g_gameMode == GRAND_PRIX) {
                    oga_drivers_point_camera_init();
                } else {
                    oga_timeattack_camera_init();
                }
            }
            break;
        case 4:
            g_fadeOutFlag = 1;
            g_fadeOutCounter = 5;
            g_startingIndicator = RACE_QUADRANT_RESULTS;
            CheckFinish();
            break;
    }
}

UNUSED void call_next_sequence(s32 arg0) {
    next_sequence_mode = arg0;
    g_startingIndicator = RACE_QUITTING;
    NaSeqFadeout(25);
    Na_SE_Fadeout(25);
    gBackgroundFlag = 1;
    g_ClearCFBFlag = 1;
    two_d_flag = 0;
    finish_count = 5;
}

void start_race_BGM(s32 track) {

    if (g_ScreenSplitB == SCREEN_MODE_3P_4P_SPLITSCREEN) {
        return; // If 3P/4P splitscreen mode is currently on, don't play the music for the current track.
    }

    set_music_volume();

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

void start_racing(void) {
    s32 i;

    time_flag = -1;
    if (!g_DemoFlag) { // If we're not in the title screen demo, play the music for the current track.
        start_race_BGM(g_courseID);
    }

    if (g_startingIndicator == RACE_STAGING) {
        g_startingIndicator = RACE_IN_PROGRESS;
    }

    for (i = 0; i < NUM_PLAYERS; i++) {

        if ((gPlayers[i].flag & EXISTS) == 0) {
            continue;
        }

        // Sets player to human.
        if (gPlayers[i].flag & NO_CONTROLS) {
            gPlayers[i].flag ^= NO_CONTROLS;
        }
    }
}

f32 get_laptime(s32 arg0) {
    f32 temp_v0 = gPlayers[arg0].position[2];
    f32 temp_v1 = gPlayers[arg0].old_position[2];
    f32 temp_f14 = g_goalBannerPos[2] - temp_v0;
    f32 temp_f16 = temp_v1 - g_goalBannerPos[2];
    return g_gameTimer - ((COURSE_TIMER_ITER_f * temp_f14) / (temp_f14 + temp_f16));
}

void goalin_subroutin(s32 i) {
    gPlayers[i].flag |= IS_RACE_FINISH;
}

void check_kart_rap(void) {
    s16 currentPosition;
    s32 playerId;

    for (playerId = 0; playerId < NUM_PLAYERS; playerId++) {
        Player* player = &gPlayers[playerId];

        if ((player->flag & EXISTS) == 0) {
            continue;
        }

        if (gLapCountByPlayerId[playerId] < player->rap) {
            player->rap--;
        } else if (gLapCountByPlayerId[playerId] > player->rap) {
            player->rap++;
            if ((player->flag & IS_PLAYER) != 0) {
                // When player finishes the race
                if (player->rap == 3) {
                    goalin_subroutin(playerId);

                    currentPosition = player->rank;
                    player->flag |= IS_CPU_PLAYER;

                    if (currentPosition < 4) {
                        race_clear_flag = 1;
                    }

                    Na_SeMute_Goal((u8) playerId);
                    if ((soundset_flag & EXISTS) == 0) {
                        soundset_flag |= EXISTS;
                    }

                    if (g_gameMode == GRAND_PRIX && g_playerCount == 2 && check_2p_GP == 0) {
                        check_2p_GP = 1;
                    }
                    if ((player->flag & IS_GHOST) == 0) {
                        g_startingIndicator = RACE_HUMAN_FINISHED;
                    }
                    if (g_gameMode == TIME_TRIALS) {
                        kill_ghost(player);
                    }

                    if (g_gameMode == VERSUS) {
                        finish_count = 180;
                        if (currentPosition == 0) {
                            win_kart = playerId;
                        }
                        switch (g_playerCount) {
                            case 2:
                                if (currentPosition == 0) {
                                    *(wincount2p + playerId) += 1;
                                }
                                if (*(wincount2p + playerId) > 99) {
                                    *(wincount2p + playerId) = 99;
                                }
                                g_startingIndicator = RACE_DONE;
                                playerId = rank_buffer[1];
                                gPlayers[playerId].weapon |= HIT_CRITTER;
                                gPlayers[playerId].flag |= IS_CPU_PLAYER;
                                Na_SeMute_Goal((u8) playerId);
                                break;
                            case 3:
                                if (currentPosition < 3) {
                                    *(wincount3p + playerId * 3 + currentPosition) += 1;
                                }
                                if (*(wincount3p + playerId * 3 + currentPosition) > 99) {
                                    *(wincount3p + playerId * 3 + currentPosition) = 99;
                                }
                                /* Because the last player may not finish, their score must be updated when the 2nd
                                   to last racer finishes. */
                                if (currentPosition == 1) {
                                    g_startingIndicator = RACE_DONE; // triggers results screen

                                 /* This messes with the loop index by setting it to the index of the last player. 
                                    But, because versus always gives the player with the lower slot/port number
                                    the advantage if 2 players finish at the same time,  it can only skip finished
                                    players who do not need more processing. It can run the same index twice, but
                                    any player who finished this frame already had their lap count updated, so 
                                    nothing will happen */
                                    playerId = rank_buffer[2];
                                    *(wincount3p + playerId * 3 + 2) += 1;
                                    if (*(wincount3p + playerId * 3 + 2) > 99) {
                                        *(wincount3p + playerId * 3 + 2) = 99;
                                    }
                                    gPlayers[playerId].weapon |= HIT_CRITTER;
                                    gPlayers[playerId].flag |= IS_CPU_PLAYER;
                                    Na_SeMute_Goal((u8) playerId);
                                }
                                break;
                            case 4:
                                if (currentPosition < 3) {
                                    *(wincount4p + playerId * 3 + currentPosition) += 1;
                                }
                                if (*(wincount4p + playerId * 3 + currentPosition) > 99) {
                                    *(wincount4p + playerId * 3 + currentPosition) = 99;
                                }
                                // if second to last, race is over
                                if (currentPosition == 2) {
                                    g_startingIndicator = RACE_DONE;
                                    playerId = rank_buffer[3];
                                    gPlayers[playerId].weapon |= HIT_CRITTER;
                                    gPlayers[playerId].flag |= IS_CPU_PLAYER;
                                    Na_SeMute_Goal((u8) playerId);
                                }
                                break;
                        }
                    }

                } else if (player->rap == 2) {
                    if ((player->flag & 0x100) != 0) {
                        return;
                    }
                    if ((soundset_flag & 0x4000) == 0) {
                        soundset_flag |= 0x4000;
                        PlayFinalLapMusicHook((u8) playerId);
                    }
                }
            } else if (player->rap == 3) {
                goalin_subroutin(playerId);
                if (g_gameMode == TIME_TRIALS) {
                    kill_ghost(player);
                }
            }
        }
    }
    if ((check_2p_GP != 0) && (check_2p_GP != 100)) {
        check_2p_GP = 100;
        sort_goal_rank_2PGP();
    }
}

void func_8028F3E8(void) {
}

// func_8028F3F0
void check_kart_rank(void) {
    s16 playerId;
    s16 position;

    for (playerId = 0; playerId < NUM_PLAYERS; playerId++) {
        if (((gPlayers[playerId].flag & EXISTS) != 0) && ((gPlayers[playerId].flag & IS_RACE_FINISH) == 0) &&
            ((gPlayers[playerId].flag & IS_GHOST) == 0)) {
            position = gGPCurrentRaceRankByPlayerId[playerId];
            gPlayers[playerId].rank = position;
            rank_buffer[position] = playerId;
        }
    }
}

void enemy_control(void) {
    s32 i;

    switch (g_startingIndicator) {
        case RACE_IN_PROGRESS:
        case RACE_HUMAN_FINISHED:
        case RACE_DONE:
        case RACE_QUADRANT_RESULTS:
            for (i = 0; i < NUM_PLAYERS; i++) {
                drive_enemy_kart(i);
            }
        case RACE_PREP:
        case RACE_STAGING:
            EtcEnemyDrive();
            break;
    }
}

void race_reset_check(void) {
    if (g_DebugMode) {
        if (((gControllerFive->ButtonHeld & R_TRIG) != 0) && ((gControllerFive->ButtonHeld & L_TRIG) != 0) &&
            ((gControllerFive->ButtonHeld & A_BUTTON) != 0) && ((gControllerFive->ButtonHeld & B_BUTTON) != 0)) {

            NaSeqFadeout(0x19);
            Na_SE_Fadeout(0x19);
            next_sequence_mode = START_MENU_FROM_QUIT;
            g_startingIndicator = RACE_QUITTING;
            gBackgroundFlag = 1;
            g_ClearCFBFlag = 1;
            two_d_flag = 0;
            finish_count = 5;
        }
    }
}

void set_vp_scale(void) {
    s16 screenWidth;

    switch (g_ScreenSplitA) { /* irregular */
        case SCREEN_MODE_1P:
            screenWidth = (s16) (s32) (320.0f * screenzoom);
            if (screenWidth < 0) {
                screenWidth = 1;
            }
            D_800DC5EC->width = screenWidth;
            screenWidth = (s16) (s32) (240.0f * screenzoom);
            if (screenWidth < 0) {
                screenWidth = 1;
            }
            D_800DC5EC->height = screenWidth;
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            screenWidth = (s16) (s32) (160.0f * screenzoom);
            if (screenWidth <= 0) {
                screenWidth = 1;
            } else if (screenWidth >= 0x140) {
                screenWidth = 0x013C;
            }
            D_800DC5EC->width = screenWidth;
            D_800DC5F0->width = screenWidth;
            screenWidth = (s16) (s32) (240.0f * screenzoom);
            if (screenWidth <= 0) {
                screenWidth = 1;
            } else if (screenWidth >= 0x1E0) {
                screenWidth = 0x01DC;
            }
            D_800DC5EC->height = screenWidth;
            D_800DC5F0->height = screenWidth;
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            screenWidth = (s16) (s32) (320.0f * screenzoom);
            if (screenWidth <= 0) {
                screenWidth = 1;
            } else if (screenWidth >= 0x280) {
                screenWidth = 0x027C;
            }
            D_800DC5EC->width = screenWidth;
            D_800DC5F0->width = screenWidth;
            screenWidth = (s16) (s32) (120.0f * screenzoom);
            if (screenWidth <= 0) {
                screenWidth = 1;
            } else if (screenWidth >= 0xF0) {
                screenWidth = 0x00EC;
            }
            D_800DC5EC->height = screenWidth;
            D_800DC5F0->height = screenWidth;
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            screenWidth = (s16) (s32) (160.0f * screenzoom);
            if (screenWidth <= 0) {
                screenWidth = 1;
            } else if (screenWidth >= 0x140) {
                screenWidth = 0x013C;
            }
            D_800DC5EC->width = screenWidth;
            D_800DC5F0->width = screenWidth;
            D_800DC5F4->width = screenWidth;
            D_800DC5F8->width = screenWidth;
            screenWidth = (s16) (s32) (120.0f * screenzoom);
            if (screenWidth <= 0) {
                screenWidth = 1;
            } else if (screenWidth >= 0xF0) {
                screenWidth = 0x00EC;
            }
            D_800DC5EC->height = screenWidth;
            D_800DC5F0->height = screenWidth;
            D_800DC5F4->height = screenWidth;
            D_800DC5F8->height = screenWidth;
            break;
    }
}

void zoom_out_screen(void) {
    screenzoom = (f32) (screenzoom - 0.017f);
    if (screenzoom < 0.0f) {
        screenzoom = 0.0f;
    }
    set_vp_scale();
}

void zoom_in_screen(void) {
    screenzoom = (f32) (screenzoom + 0.028f);
    if (screenzoom > 1.0f) {
        screenzoom = 1.0f;
    }
    set_vp_scale();
}

void check_pause(void) {
    s32 i;

    if (replay_flag) {
        return;
    }

    //! @todo increasing players past four would require increase this loop iterator.
    for (i = 0; i < 4; i++) {

        Player* player = &gPlayers[i];
        struct Controller* controller = &gControllers[i];

        if (!(player->flag & IS_PLAYER)) {
            continue;
        }
        if (player->flag & IS_CPU_PLAYER) {
            continue;
        }

        if (g_ScreenSplitA != SCREEN_MODE_3P_4P_SPLITSCREEN) {
            if ((controller->ButtonPressed & L_TRIG) && !(controller->ButtonHeld & R_TRIG)) {
                controller->ButtonPressed &= ~L_TRIG;

                g_musicUserVolumeFlag++;
                if (g_musicUserVolumeFlag >= 3) {
                    g_musicUserVolumeFlag = 0;
                }
                playSound(SOUND_ACTION_PING);
                set_music_volume();
            }
        }
        if ((controller->ButtonPressed & START_BUTTON) && (!(controller->ButtonHeld & R_TRIG)) &&
            (!(controller->ButtonHeld & L_TRIG))) {
            save_key_status();
            pause_flag = (controller - gControllerOne) + 1;
            controller->ButtonPressed = 0;
            Na_PauseSet(1);
            ghost_write = 1;
            if (g_gameMode == TIME_TRIALS) {
                if (gPlayerOne->flag & (EXISTS | IS_GHOST)) {
                    kill_ghost(gPlayerOne);
                }
                if (gPlayerTwo->flag & (EXISTS | IS_GHOST)) {
                    kill_ghost(gPlayerTwo);
                }
                if (gPlayerThree->flag & (EXISTS | IS_GHOST)) {
                    kill_ghost(gPlayerThree);
                }
            }
            return;
        }
    }

    if (g_DebugMode) {
        if (g_gameMode == BATTLE) {
            // do stuff?
        } else {
            if (gControllerOne->ButtonPressed & U_JPAD) {
                gLapCountByPlayerId[0] = 2;
            }
            if (gControllerOne->ButtonPressed & R_JPAD) {
                gLapCountByPlayerId[0] = 2;
                gLapCountByPlayerId[1] = 2;
            }
            if (gControllerOne->ButtonPressed & D_JPAD) {
                gLapCountByPlayerId[0] = 2;
                gLapCountByPlayerId[1] = 2;
                gLapCountByPlayerId[2] = 2;
                gLapCountByPlayerId[3] = 2;
                gLapCountByPlayerId[4] = 2;
                gLapCountByPlayerId[5] = 2;
                gLapCountByPlayerId[6] = 2;
                gLapCountByPlayerId[7] = 2;
            }
        }
    }
}

void start_fadeout(void) {
    next_sequence_mode = START_MENU_FROM_QUIT;
    g_startingIndicator = RACE_QUITTING;
    NaSeqFadeout(25);
    Na_SE_Fadeout(25);
    gBackgroundFlag = 1;
    g_ClearCFBFlag = 1;
    two_d_flag = 0;
    finish_count = 5;
}

#ifdef VERSION_EU
#define gDemoTimer_SIZE 1600
#else
#define gDemoTimer_SIZE 1920
#endif

void check_title_demo(void) {
    if (finish_count < 0) {
        finish_count = gDemoTimer_SIZE;
        return;
    }
    finish_count--;
    if (gControllerFive->ButtonPressed != 0) {
        start_fadeout();
        KBGNumber = START_MENU;
        return;
    }
    if (finish_count == 0) {
        start_fadeout();
        KBGNumber = LOGO_INTRO_MENU;
    }
}

void race_control(void) {
    Player* ply = &gPlayers[0];
    s32 i;
    u32 phi_v0_4;

    if (demo_play_flag) {
        check_title_demo();
    }
    switch (g_startingIndicator) {
        case RACE_NONE:
            if (!g_DemoFlag) { // If we're not in the demo mode, play the starting fanfare for the current mode (Grand Prix / Time Trials / VS / Battle)
                if (g_gameMode == GRAND_PRIX) {
                    NaFanStart(SEQ_EVENT_RACE_STARTING);
                    playSound(SOUND_ACTION_REV_ENGINE);
                    playSound(SOUND_ACTION_REV_ENGINE_2);
                } else {
                    NaFanStart(SEQ_EVENT_RACE_STARTING_VS);
                }
            }
            DelayInitialMap();
            g_startingIndicator = RACE_PREP;
            D_80150118 = 3.0f;
            g_DisplayFlag = 0;
            soundset_flag = 0;
            time_flag = 0;
            g_gameTimer = 0.0f;
            timer = 0.0f;
            g_ClearCFBFlag = 1;
            gBackgroundFlag = 1;
            screenzoom = 0.008f;
            finish_zoom_flag = 0;
            if (g_ScreenSplitB != SCREEN_MODE_1P) {
                kwdisplayon(&D_8018D2AC);
            }
            for (i = 0; i < NUM_PLAYERS; i++) {
                if ((ply->flag & EXISTS) == 0) {
                    continue;
                }
                ply->flag |= NO_CONTROLS;
                ply++;
            }
            two_d_flag = 1;
            break;
        case RACE_PREP:
            zoom_in_screen();
            if (screenzoom == 1.0f) {
                if (g_ScreenSplitA != SCREEN_MODE_1P) {
                    if (g_courseID == COURSE_LUIGI_RACEWAY) {
                        take_snapshot9();
                    } else if (g_courseID == COURSE_WARIO_STADIUM) {
                        take_snapshot15();
                    }
                }
                g_startingIndicator = RACE_STAGING;
                g_ClearCFBFlag = 0;
                two_d_flag = 1;
                kwjugemustart();
                if ((g_gameMode == TIME_TRIALS) && (romghost_status == 0)) {
                    phi_v0_4 = 0x1;
                    for (i = 0; i < g_courseID; i++) {
                        phi_v0_4 <<= 1;
                    }
                    if ((replay_flag == 0) && (!(D_800DC5AC & phi_v0_4))) {
                        DispMessageRomGhost();
                        D_800DC5AC |= phi_v0_4;
                    }
                }
                if (g_playerCount == 3) {
                    DispInfo_3P();
                }
            }
            race_reset_check();
            break;
        case RACE_STAGING:
            if (g_DemoFlag) {
                start_racing();
            }
            if ((g_DebugMode) && (gControllerFive->ButtonPressed & Z_TRIG)) {
                start_racing();
            }
            race_reset_check();
            break;
        case RACE_IN_PROGRESS:
            if (g_gameMode == BATTLE) {
                check_battle_status();
            } else {
                check_kart_rank();
                check_kart_rap();
            }
            race_reset_check();
            check_pause();
            break;
        case RACE_HUMAN_FINISHED:

            switch (g_gameMode) {
                case GRAND_PRIX:
                    race_reset_check();
                    check_kart_rank();
                    check_kart_rap();
                    check_pause();

                    switch (g_ScreenSplitB) {
                        case SCREEN_MODE_1P:
                            finish_count = 690;
                            g_startingIndicator = RACE_DONE;
                            check_goalin_time();
                            break;
                        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                            if (((gPlayerOne->flag & IS_RACE_FINISH) != 0) &&
                                ((gPlayerTwo->flag & IS_RACE_FINISH) != 0)) {

                                if (gPlayerOne->rank < gPlayerTwo->rank) {
                                    win_kart = 1;
                                } else {
                                    win_kart = 0;
                                }

                                check_goalin_time();
                                finish_count = 600;
                                g_startingIndicator = RACE_DONE;
                            }
                            break;
                    }
                    break;
                case VERSUS:
                    race_reset_check();
                    check_kart_rank();
                    check_kart_rap();
                    check_pause();
                    break;
                case TIME_TRIALS:
                    finish_count = 360;
                    if (replay_flag != 0) {
                        g_startingIndicator = RACE_QUADRANT_RESULTS;
                    } else {
                        g_startingIndicator = RACE_DONE;
                    }
                    break;
            }
            break;
        case RACE_DONE:
            if (finish_count != 0) {
                finish_count--;
            } else {
                switch (g_gameMode) {
                    case GRAND_PRIX:
                        if (race_clear_flag != 0) {
                            finish_zoom();
                        } else if (g_ScreenSplitB == SCREEN_MODE_1P) {
                            DispGPLose();
                            g_startingIndicator = RACE_QUADRANT_RESULTS;
                        } else {
                            finish_vs_zoom();
                        }
                        break;
                    case TIME_TRIALS:
                        finish_zoom();
                        break;
                    case VERSUS:
                    case BATTLE:
                        finish_vs_zoom();
                        break;
                }
            }
            race_reset_check();
            break;
        case RACE_QUITTING:
            zoom_out_screen();
            if (screenzoom <= 0) {
                g_fadeOutFlag = 1;
                g_fadeOutCounter = 5;
            }
            break;
        case RACE_QUADRANT_RESULTS:
            break;
    }
}

UNUSED void goto_title(void) {
    g_fadeOutFlag = 1;
    g_fadeOutCounter = 5;
    next_sequence_mode = START_MENU_FROM_QUIT;
}

void goto_gameselect(void) {
    g_fadeOutFlag = 1;
    g_fadeOutCounter = 5;
    next_sequence_mode = MAIN_MENU_FROM_QUIT;
}

void GoToGameSelect(void) {
    g_fadeOutFlag = 1;
    g_fadeOutCounter = 5;
    next_sequence_mode = PLAYER_SELECT_MENU_FROM_QUIT;
}

void goto_mapselect(void) {
    g_fadeOutFlag = 1;
    g_fadeOutCounter = 5;
    next_sequence_mode = COURSE_SELECT_MENU_FROM_QUIT;
}

void goto_racing(void) {
    g_fadeOutFlag = 1;
    g_fadeOutCounter = 5;
    next_sequence_mode = RACING;
}

void check_col(Player* playerOne, Player* playerTwo) {
    f32 sp70 = (playerOne->radius + playerTwo->radius) - 5.0f;
    f32 temp_f0;
    f32 sp74;
    Vec3f sp60;
    Vec3f sp54;
    f32 temp_f0_2;
    f32 temp_f16;
    f32 temp_f2;

    f32 gFloatArray802B8790[] = { 1.2, 1.0, 0.9, 0.7, 2.0, 1.8, 0.9, 2.3 };

    f32 sp24 = gFloatArray802B8790[playerOne->kart];
    f32 sp20 = gFloatArray802B8790[playerTwo->kart];

    sp60[0] = playerOne->position[0] - playerTwo->position[0];
    sp60[1] = (playerOne->position[1] - playerOne->radius) - (playerTwo->position[1] - playerTwo->radius);
    sp60[2] = playerOne->position[2] - playerTwo->position[2];

    sp54[0] = playerTwo->velocity[0] - playerOne->velocity[0];
    sp54[1] = playerTwo->velocity[1] - playerOne->velocity[1];
    sp54[2] = playerTwo->velocity[2] - playerOne->velocity[2];

    temp_f0 = sqrtf((sp60[0] * sp60[0]) + (sp60[1] * sp60[1]) + (sp60[2] * sp60[2]));

    if (temp_f0 < 0.1f) {
        return;
    }

    sp74 = temp_f0 - sp70;
    if (sp74 > 0) {
        return;
    }

    if (playerOne->flag & IS_BOMB) {
        if (playerTwo->flag & IS_BOMB) {
            kill_bomb_kart(playerOne);
            kill_bomb_kart(playerTwo);
            NAPlyTrgStart((playerTwo - gPlayerOne), 0x19008001U);
            return;
        } else {
            playerTwo->weapon |= HIT_BOMB;
            kill_bomb_kart(playerOne);
            NAPlyTrgStart((playerTwo - gPlayerOne), 0x19008001U);
        }
    } else if (playerTwo->flag & IS_BOMB) {
        playerOne->weapon |= HIT_BOMB;
        kill_bomb_kart(playerTwo);
        NAPlyTrgStart(playerOne - gPlayerOne, 0x19008001U);
        return;
    }
    if (playerOne->slip_flag & STAR) {
        if (!(playerTwo->slip_flag & STAR)) {
            playerTwo->weapon |= HIT_BOMB_THROW;
        }
    } else if (playerTwo->slip_flag & STAR) {
        playerOne->weapon |= HIT_BOMB_THROW;
    } else {
        playerOne->slip_flag |= CARHIT;
        playerTwo->slip_flag |= CARHIT;
    }
    temp_f0_2 = sqrtf((sp54[0] * sp54[0]) + (sp54[1] * sp54[1]) + (sp54[2] * sp54[2]));
    sp60[0] /= temp_f0;
    sp60[1] /= temp_f0;
    sp60[2] /= temp_f0;
    if (temp_f0_2 < 0.2f) {
        temp_f0 = (playerOne->radius + playerTwo->radius) * 0.55f;
        playerOne->position[0] = playerTwo->position[0] + (sp60[0] * temp_f0);
        playerOne->position[1] = playerTwo->position[1] + (sp60[1] * temp_f0);
        playerOne->position[2] = playerTwo->position[2] + (sp60[2] * temp_f0);
        playerTwo->position[0] -= temp_f0 * sp60[0];
        playerTwo->position[1] -= temp_f0 * sp60[1];
        playerTwo->position[2] -= temp_f0 * sp60[2];
        return;
    } else {
        temp_f16 = ((sp60[0] * sp54[0]) + (sp60[1] * sp54[1]) + (sp60[2] * sp54[2])) / temp_f0_2;
    }
    temp_f0_2 = temp_f0_2 * temp_f16 * 0.85;
    if ((playerOne->slip_flag & STAR) != STAR) {
        temp_f2 = (temp_f0_2 * sp20) / sp24;
        playerOne->velocity[0] += sp60[0] * temp_f2;
        playerOne->velocity[1] += sp60[1] * temp_f2;
        playerOne->velocity[2] += sp60[2] * temp_f2;
        playerOne->position[0] -= sp60[0] * sp74 * 0.5f;
        playerOne->position[1] -= sp60[1] * sp74 * 0.5f;
        playerOne->position[2] -= sp60[2] * sp74 * 0.5f;
    }
    if ((playerTwo->slip_flag & STAR) != STAR) {
        temp_f2 = (temp_f0_2 * sp24) / sp20;
        playerTwo->velocity[0] -= sp60[0] * temp_f2;
        playerTwo->velocity[1] -= sp60[1] * temp_f2;
        playerTwo->velocity[2] -= sp60[2] * temp_f2;
        playerTwo->position[0] += sp60[0] * sp74 * 0.5f;
        playerTwo->position[1] += sp60[1] * sp74 * 0.5f;
        playerTwo->position[2] += sp60[2] * sp74 * 0.5f;
    }
    if (playerOne->flag & IS_PLAYER) {
        NAPlyTrgStart((playerOne - gPlayerOne), 0x19008001U);
        return;
    }
    if (playerTwo->flag & IS_PLAYER) {
        NAPlyTrgStart((playerTwo - gPlayerOne), 0x19008001U);
    }
}

void collision_kart_to_kart(void) {
    Player* ply;
    Player* ply2;
    s32 i;
    s32 k;

    for (i = 0; i < 7; i++) {
        ply = &gPlayers[i];

        if ((ply->flag & EXISTS) && (!(ply->slip_flag & TERESA)) &&
            (!(ply->flag & IS_GHOST)) && (!(ply->slip_flag & BROKEN))) {

            for (k = i + 1; k < NUM_PLAYERS; k++) {
                ply2 = &gPlayers[k];

                if ((ply2->flag & EXISTS) && (!(ply2->slip_flag & TERESA)) &&
                    (!(ply2->flag & IS_GHOST)) && (!(ply2->slip_flag & BROKEN))) {

                    check_col(ply, ply2);
                }
            }
        }
    }
}

void pause_sequence(void) {

    kwpausesyori();

    switch (g_ScreenSplitA) {
        case SCREEN_MODE_1P:
            CameraControl(gPlayerOneCopy, camera1, 0);
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            CameraControl(gPlayerOneCopy, camera1, 0);
            CameraControl(gPlayerTwoCopy, camera2, 1);
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            CameraControl(gPlayerOneCopy, camera1, 0);
            CameraControl(gPlayerTwo, camera2, 1);
            CameraControl(gPlayerThree, camera3, 2);
            CameraControl(gPlayerFour, camera4, 3);
            break;
    }
}

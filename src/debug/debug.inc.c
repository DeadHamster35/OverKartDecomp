#include <ultra64.h>
#include <debug.h>
#include "all_variables.h"

#if DVDL

extern s32 frame_counter;

/**
 * Edit this to edit what displays on the screen while the DVDL is active.
 * The Size of the structure array is calculated at compile time.
 */
variableWatchAttributes gMainVariableWatchList[] = {
    { "Global Timer: ", &frame_counter, sizeof(frame_counter), DISPLAY_DECIMAL_NUMBER | DISPLAY_SIGNED_NUMBER, 0, 0 },
    { "Actors: ", &g_simpleObjectCount, sizeof(g_simpleObjectCount), DISPLAY_DECIMAL_NUMBER, 0, 0 },
    { "Player Type: ", &gPlayers[0].flag, sizeof(gPlayerOne->flag), DISPLAY_HEXIDECIMAL_NUMBER, 0, 0 },
    { "X ", &gPlayers[0].position[0], sizeof(gPlayerOne->position[0]), DISPLAY_FLOAT_NUMBER, 0, 0 },
    { "Y ", &gPlayers[0].position[1], sizeof(gPlayerOne->position[1]), DISPLAY_FLOAT_NUMBER, 0, 0 },
    { "Z ", &gPlayers[0].position[2], sizeof(gPlayerOne->position[2]), DISPLAY_FLOAT_NUMBER, 0, 0 },
    { "Torque 9C: ", &gPlayers[0].accelcount, sizeof(gPlayerOne->accelcount), DISPLAY_FLOAT_NUMBER, 0, 0 },
    { "Top Speed 214: ", &gPlayers[0].acc_maxcount, sizeof(gPlayerOne->acc_maxcount), DISPLAY_FLOAT_NUMBER, 0, 0 },
    { "Accel Offset C4: ", &gPlayers[0].kartpitch, sizeof(gPlayerOne->kartpitch), DISPLAY_SIGNED_NUMBER, 0, 0 },

};

#endif

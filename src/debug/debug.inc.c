#include <ultra64.h>
#include <debug.h>
#include "all_variables.h"

#if DVDL

extern s32 FrameCounter;

/**
 * Edit this to edit what displays on the screen while the DVDL is Active.
 * The Size of the structure array is calculated at compile Time.
 */
VariableWatchAttributes gMainVariableWatchList[] = {
    { "Global Timer: ", &FrameCounter, sizeof(FrameCounter), DISPLAY_DECIMAL_NUMBER | DISPLAY_SIGNED_NUMBER, 0, 0 },
    { "Actors: ", &SimpleObjectCount, sizeof(SimpleObjectCount), DISPLAY_DECIMAL_NUMBER, 0, 0 },
    { "Player Type: ", &GamePlayers[0].Flag, sizeof(PlayerOne->Flag), DISPLAY_HEXIDECIMAL_NUMBER, 0, 0 },
    { "X ", &GamePlayers[0].Position[0], sizeof(PlayerOne->Position[0]), DISPLAY_FLOAT_NUMBER, 0, 0 },
    { "Y ", &GamePlayers[0].Position[1], sizeof(PlayerOne->Position[1]), DISPLAY_FLOAT_NUMBER, 0, 0 },
    { "Z ", &GamePlayers[0].Position[2], sizeof(PlayerOne->Position[2]), DISPLAY_FLOAT_NUMBER, 0, 0 },
    { "Torque 9C: ", &GamePlayers[0].Accelcount, sizeof(PlayerOne->Accelcount), DISPLAY_FLOAT_NUMBER, 0, 0 },
    { "Top Speed 214: ", &GamePlayers[0].AccMaxcount, sizeof(PlayerOne->AccMaxcount), DISPLAY_FLOAT_NUMBER, 0, 0 },
    { "Accel Offset C4: ", &GamePlayers[0].Kartpitch, sizeof(PlayerOne->Kartpitch), DISPLAY_SIGNED_NUMBER, 0, 0 },

};

#endif

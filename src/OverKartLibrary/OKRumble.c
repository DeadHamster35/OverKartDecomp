#include "MainInclude.h"
#include "save.h"

s32 RumbleActive[MAXCONTROLLERS];
s32 RumbleTimer[MAXCONTROLLERS];
s32 RumbleFramesOn[MAXCONTROLLERS];
s32 RumbleFramesOff[MAXCONTROLLERS];
static s32 RumblePhase[MAXCONTROLLERS];

void SetRumble(char PlayerID, s32 framesOn, s32 framesOff, s32 duration)
{
	s32 player;
	s32 oldPeriod;
	s32 newPeriod;

	player = (s32) PlayerID;
	if ((player < 0) || (player >= MAXCONTROLLERS))
	{
		return;
	}
	if ((framesOn <= 0) || (duration <= 0))
	{
		return;
	}
	if (framesOff < 0)
	{
		framesOff = 0;
	}
	if (RumbleActive[player] == 0)
	{
		RumbleActive[player] = 1;
		RumbleFramesOn[player] = framesOn;
		RumbleFramesOff[player] = framesOff;
		RumbleTimer[player] = duration;
		RumblePhase[player] = 0;
		return;
	}
	oldPeriod = RumbleFramesOn[player] + RumbleFramesOff[player];
	newPeriod = framesOn + framesOff;
	if ((oldPeriod <= 0) || ((framesOn * oldPeriod) > (RumbleFramesOn[player] * newPeriod)))
	{
		RumbleFramesOn[player] = framesOn;
		RumbleFramesOff[player] = framesOff;
		RumblePhase[player] = 0;
	}
	if (duration > RumbleTimer[player])
	{
		RumbleTimer[player] = duration;
	}
}

void ClearRumble(char PlayerID)
{
	s32 player;

	player = (s32) PlayerID;
	if ((player < 0) || (player >= MAXCONTROLLERS))
	{
		return;
	}
	RumbleActive[player] = 0;
	RumbleTimer[player] = 0;
	RumbleFramesOn[player] = 0;
	RumbleFramesOff[player] = 0;
	RumblePhase[player] = 0;
	MotorActive[player] = 0;
}

void UpdateRumbleHelper(void)
{
	s32 i;
	s32 period;

	for (i = 0; i < MAXCONTROLLERS; i++)
	{
		if (RumbleActive[i] == 0)
		{
			MotorActive[i] = 0;
			continue;
		}

		period = RumbleFramesOn[i] + RumbleFramesOff[i];
		if (period <= 0)
		{
			RumbleActive[i] = 0;
			RumbleFramesOn[i] = 0;
			RumbleFramesOff[i] = 0;
			RumblePhase[i] = 0;
			MotorActive[i] = 0;
			continue;
		}

		if (RumblePhase[i] < RumbleFramesOn[i])
		{
			MotorActive[i] = 1;
		}
		else
		{
			MotorActive[i] = 0;
		}

		RumblePhase[i]++;
		if (RumblePhase[i] >= period)
		{
			RumblePhase[i] = 0;
		}

		RumbleTimer[i]--;
		if (RumbleTimer[i] <= 0)
		{
			RumbleActive[i] = 0;
			RumbleFramesOn[i] = 0;
			RumbleFramesOff[i] = 0;
			RumblePhase[i] = 0;
		}
	}
}

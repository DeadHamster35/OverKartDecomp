#include "../MainInclude.h"





// VISUALS //

void SetAnimMusicNote(char PlayerID)
{
	GamePlayers[(int)PlayerID].Talk |= MUSIC_NOTE;
}

void SetAnimCrash(char PlayerID)
{
	GamePlayers[(int)PlayerID].Talk |= CRASH;
}

void SetAnimPoomp(char PlayerID)
{
	GamePlayers[(int)PlayerID].Talk |= POOMP;
}

void SetAnimBoing(char PlayerID)
{
	GamePlayers[(int)PlayerID].Talk |= BOING;
}

void SetAnimExplosion(char PlayerID)
{
	GamePlayers[(int)PlayerID].Talk |= FLASH;
}

void SetAnimBonkStars(char PlayerID)
{
	GamePlayers[(int)PlayerID].HandlingFlag |= TROPHYJUMP_STARS;
}

void SetAnimLandingDust(char PlayerID)
{
	GamePlayers[(int)PlayerID].HandlingFlag |= LANDING_DUST;
}

void SetAnimBooSmoke(char PlayerID)
{
	GamePlayers[(int)PlayerID].HandlingFlag |= BOO_SMOKE_EFFECT;
}

void SetAnimWaterDrip(char PlayerID, bool Active)
{
	if (Active)
	{
		GamePlayers[(int)PlayerID].JugemuFlag |= WATER_EFFECT;
		return;
	}
	if ((!Active) && ((GamePlayers[(int)PlayerID].JugemuFlag & WATER_EFFECT) != 0))
	{
		GamePlayers[(int)PlayerID].JugemuFlag ^= WATER_EFFECT;
		return;
	}
}

void SetAnimSmoking(char PlayerID, bool Active)
{
	if (Active)
	{
		GamePlayers[(int)PlayerID].JugemuFlag |= LAVA_EFFECT;
		return;
	}
	if ((!Active) && ((GamePlayers[(int)PlayerID].JugemuFlag & LAVA_EFFECT) != 0))
	{
		GamePlayers[(int)PlayerID].JugemuFlag ^= LAVA_EFFECT;
		return;
	}
}

void DisableOutline(char PlayerID) // Fixes custom characters black outlines
{
	if ((unsigned char)GamePlayers[(int)PlayerID].Erase == 0xFF)
	{
		GamePlayers[(int)PlayerID].Erase = 0x30;
	}
}



// GAMEPLAY // 

void SetMapObjectHit(char PlayerID)
{
	GamePlayers[(int)PlayerID].Weapon |= HIT_BOMB;
}

void SetMushroomBoost(char PlayerID)
{
	GamePlayers[(int)PlayerID].Weapon |= USE_MUSHROOM;
}

void SetLightningHit(char PlayerID)
{
	GamePlayers[(int)PlayerID].Weapon |= HIT_LIGHTNING;
}

void SetShrunken(char PlayerID, bool Active)
{
	if (Active)
	{
		GamePlayers[(int)PlayerID].SlipFlag |= IS_LIGHTNING_HIT;
		GamePlayers[(int)PlayerID].ThunderTimer = 0x00FF;
	}
	if (!Active)
	{
		GamePlayers[(int)PlayerID].SlipFlag |= IS_UNSQUISHED;
		GamePlayers[(int)PlayerID].ThunderTimer = 0x01CC;
	}
}

void SetStarMan(char PlayerID, bool Active)
{
	if (Active)
	{
		if ((GamePlayers[(int)PlayerID].SlipFlag & IS_STAR) == 0)
		{
			NAEnmTrgStart(GamePlayers[(int)PlayerID].Position,GamePlayers[(int)PlayerID].Velocity,0x0100ff2c);
		}
		GamePlayers[(int)PlayerID].SlipFlag |= IS_STAR;
		StarUseCounter[(int)PlayerID]  = 0xFFFF;
	}
	if (!Active)
	{
		StarUseCounter[(int)PlayerID]  = 0;
		ResetStar(&GamePlayers[(int)PlayerID], PlayerID);
		NAEnmTrgStop(GamePlayers[(int)PlayerID].Position,0x0100ff2c);
	}

}

void SetGhostEffect(char PlayerID, bool Active, int UseCount)
{

	if (Active)
	{
		
		GhostUseCounter[(int)PlayerID]  = UseCount;
		GhostUseTimer[(int)PlayerID] = 30;
		if ((GamePlayers[(int)PlayerID].SlipFlag & IS_BOO) == 0)
		{
			SetVSGhost((void*)&GamePlayers[(int)PlayerID], PlayerID);
		}
	}
	if ((!Active))
	{
		GhostUseCounter[(int)PlayerID]  = 0;
		GhostUseTimer[(int)PlayerID]  = -1;
		ResetVSGhost((void*)&GamePlayers[(int)PlayerID], PlayerID);
	}

}

void SetGhostEffectSimple(char PlayerID, bool Active)
{
	SetGhostEffect(PlayerID, Active, Active ? 0xFFFF : 0);
}

void SetBooTranslucent(char PlayerID)
{
	GamePlayers[(int)PlayerID].Weapon |= USE_BOO;
}

void SetBecomeBomb(char PlayerID, bool Active)
{
	if (Active)
	{
		GamePlayers[(int)PlayerID].Weapon |= PRESS_BECOME_BOMB;
	}
	if ((!Active) && ((GamePlayers[(int)PlayerID].Flag & IS_BOMB) != 0))
	{
		GamePlayers[(int)PlayerID].Flag ^= IS_BOMB;
	}
}



void SetFlattened(char PlayerID, bool Active)
{
	if (Active)
	{
		GamePlayers[(int)PlayerID].Weapon |= PRESS_SQUISH;
	}
	if ((!Active) && ((GamePlayers[(int)PlayerID].Weapon & PRESS_SQUISH) != 0))
	{
		GamePlayers[(int)PlayerID].Weapon ^= PRESS_SQUISH;
	}
}

void SetSpinOutSaveable(char PlayerID)
{
	GamePlayers[(int)PlayerID].Weapon |= HIT_BANANA;
}

void SetSpinOut(char PlayerID)
{
	GamePlayers[(int)PlayerID].Weapon |= HIT_CRITTER;
}

void SetFailedStart(char PlayerID)
{
	GamePlayers[(int)PlayerID].Weapon |= PRESS_STARTSPIN;
}

void SetGreenShellHit(char PlayerID)
{
	GamePlayers[(int)PlayerID].Weapon |= HIT_GREENSHELL;
}

void SetRedShellHit(char PlayerID)
{
	GamePlayers[(int)PlayerID].Weapon |= HIT_REDSHELL;
}

void SetBonk(char PlayerID)
{
	GamePlayers[(int)PlayerID].SlipFlag |= IS_BONKING;
}



void ChangeMaxSpeed(char PlayerID, float SpeedGain)
{
	GamePlayers[(int)PlayerID].AccMaxcount += (float)SpeedGain;
}


void SetCamShiftUp(char PlayerID, float shift)
{
	if (PlayerID > 3)
	{
		return;
	}
	if (!(GamePlayers[(int)PlayerID].Flag & IS_PLAYER))
	{
		return;
	}
	GlobalCamera[(int)PlayerID]->CameraVector[1] = 9 + shift;
}

void ChangePlayerSize(char PlayerID, float ScaleFactor)
{
	if(((GamePlayers[(int)PlayerID].Kart) == 5) || ((GamePlayers[(int)PlayerID].Kart) == 7))
	{
		GamePlayers[(int)PlayerID].Radius = 6 * (float)ScaleFactor;
	}
	else
	{
		GamePlayers[(int)PlayerID].Radius = 5.5 * (float)ScaleFactor;
	}
	GamePlayers[(int)PlayerID].Offsetsize = (float)ScaleFactor;

	if ((PlayerID <= 3) && (GamePlayers[(int)PlayerID].Flag&IS_PLAYER))
	{
		GlobalCamera[(int)PlayerID]->CameraVector[1] = 9 * (float)ScaleFactor;
	}
}

void SetPlayerColor(char PlayerID, int Colors, int AdjColor, float Speed)
{
	MakeBodyColor((void*)&GamePlayers[(int)PlayerID], PlayerID, Colors, Speed);
	MakeBodyColorAdjust((void*)&GamePlayers[(int)PlayerID], PlayerID, AdjColor, Speed);
}

void SetPlayerEcho(char PlayerID, char echo)
{
	if (!(GamePlayers[(int)PlayerID].Flag&IS_PLAYER))
	{
		return;
	}	
	GlobalAddressA = (long)(&(((s8 *) D_800E9F7C)[0x14])) + (0x3C * PlayerID);
	*(char*)GlobalAddressA = echo;
}

void PlayrandmCharacterSFX(char PlayerID)
{
	if ((RNG <= 0x3333))
	{
    	NAPlyVoiceStart(PlayerID, 0x29008008 + (GamePlayers[(int)PlayerID].Kart * 0x10));  //voice char lucky sfx
    	return;
    }
	if (RNG <= 0x7777)
	{
    	NAPlyVoiceStart(PlayerID, 0x2900800D + (GamePlayers[(int)PlayerID].Kart * 0x10));   //voice char yahho sfx
    	return;
	}
	if (RNG <= 0x9999)
	{
    	NAPlyVoiceStart(PlayerID, 0x29008001 + (GamePlayers[(int)PlayerID].Kart * 0x10));   //voice char gogo sfx
    	return;
	}
	NAPlyVoiceStart(PlayerID, 0x2900800C + (GamePlayers[(int)PlayerID].Kart * 0x10));   //voice char Jump sfx    
}


void ProStickAngleHook(Player *car, Controller *Cont, char Number)
{
    if(car->Talk&0x2 && (!(car->SlipFlag&IS_DRIFTING) || ((car->SlipFlag&IS_JUMPING) && (car->SlipFlag&IS_DRIFTING))))
    {
        ProStickAngle(car,Cont,Number);
    }
    else
    {
        if (!(car->SlipFlag&IS_IN_AIR))
        {
            ProStickAngle(car,Cont,Number);
        }
        else if(car->SlipFlag&IS_JUMPING && car->Bump.DistanceZX <= 5)
        {
            ProStickAngle(car,Cont,Number);
        }
    }
}




void MasterEffect(int PlayerID, short EffectID)
{
	switch (EffectID)
	{
		case StateAnimMusicNote:
		{
			SetAnimMusicNote(PlayerID);
			break;
		}
		case StateAnimCrash:
		{
			SetAnimCrash(PlayerID);
			break;
		}
		case StateAnimPoomp:
		{
			SetAnimPoomp(PlayerID);
			break;
		}
		case StateAnimBoing:
		{
			SetAnimBoing(PlayerID);
			break;
		}
		case StateAnimExplosion:
		{
			SetAnimExplosion(PlayerID);
			break;
		}
		case StateAnimBonkStars:
		{
			SetAnimBonkStars(PlayerID);
			break;
		}
		case StateAnimLandingDust:
		{
			SetAnimLandingDust(PlayerID);
			break;
		}
	}

}


void  MasterStatus(int PlayerID, short StatusID)
{
	switch (StatusID)
	{
		case StateMapObjectHit:
		{
			SetMapObjectHit(PlayerID);
			break;
		}
		case StateLightningHit:
		{
			SetLightningHit(PlayerID);
			break;
		}
		case StateBooTranslucent:
		{
			SetBooTranslucent(PlayerID);
			break;
		}
		case StateBecomeBombOn:
		{
			SetBecomeBomb(PlayerID, true);
			break;			
		}
		case StateBecomeBombOff:
		{
			SetBecomeBomb(PlayerID, false);
			break;			
		}
		case StateFlattenedOn:
		{
			//SetFlattened(PlayerID, true);	
			SetBroken((Player*)&GamePlayers[PlayerID], PlayerID);
			break;
		}
		case StateFlattenedOff:
		{
			SetFlattened(PlayerID, false);			
			break;
		}
		case StateMushroomBoost:
		{
			SetMushroomBoost(PlayerID);
			break;
		}
		case StateSpinOutSaveable:
		{
			SetSpinOutSaveable(PlayerID);
			break;
		}
		case StateSpinOut:
		{
			SetSpinOut(PlayerID);
			break;
		}
		case StateGreenShellHit:
		{
			SetGreenShellHit(PlayerID);
			break;
		}
		case StateRedShellHit:
		{
			SetRedShellHit(PlayerID);
			break;
		}
		case StateBonk:
		{
			SetBonk(PlayerID);
			break;
		}
		case StateStarOn:
		{
			SetStarMan(PlayerID,true);
			break;
		}
		case StateStarOff:
		{
			SetStarMan(PlayerID,false);
			break;
		}
		case StateGhostOn:
		{
			//SetGhostEffect(PlayerID,true, 1);			
			SetVSGhost((Player*)&GamePlayers[(int)PlayerID], (char)PlayerID);
			break;
		}
		case StateGhostOff:
		{
			SetGhostEffect(PlayerID,false, 0);
			break;
		}
		case StateLoseUnusedItem:
		{
			if (GamePlayers[PlayerID].Flag&IS_PLAYER)
				DynamicObjects[ItemBoxAllocPtr[PlayerID]].Output = 0;
			break;
		}

		
	}
}

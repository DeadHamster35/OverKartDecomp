#include "../MainInclude.h"





// VISUALS //

void SetAnimMusicNote(char PlayerID)
{
	GlobalPlayer[(int)PlayerID].Talk |= MUSIC_NOTE;
}

void SetAnimCrash(char PlayerID)
{
	GlobalPlayer[(int)PlayerID].Talk |= CRASH;
}

void SetAnimPoomp(char PlayerID)
{
	GlobalPlayer[(int)PlayerID].Talk |= POOMP;
}

void SetAnimBoing(char PlayerID)
{
	GlobalPlayer[(int)PlayerID].Talk |= BOING;
}

void SetAnimExplosion(char PlayerID)
{
	GlobalPlayer[(int)PlayerID].Talk |= FLASH;
}

void SetAnimBonkStars(char PlayerID)
{
	GlobalPlayer[(int)PlayerID].HandlingFlag |= TROPHYJUMP_STARS;
}

void SetAnimLandingDust(char PlayerID)
{
	GlobalPlayer[(int)PlayerID].HandlingFlag |= LANDING_DUST;
}

void SetAnimBooSmoke(char PlayerID)
{
	GlobalPlayer[(int)PlayerID].HandlingFlag |= BOO_SMOKE_EFFECT;
}

void SetAnimWaterDrip(char PlayerID, bool Active)
{
	if (Active)
	{
		GlobalPlayer[(int)PlayerID].JugemuFlag |= WATER_EFFECT;
		return;
	}
	if ((!Active) && ((GlobalPlayer[(int)PlayerID].JugemuFlag & WATER_EFFECT) != 0))
	{
		GlobalPlayer[(int)PlayerID].JugemuFlag ^= WATER_EFFECT;
		return;
	}
}

void SetAnimSmoking(char PlayerID, bool Active)
{
	if (Active)
	{
		GlobalPlayer[(int)PlayerID].JugemuFlag |= LAVA_EFFECT;
		return;
	}
	if ((!Active) && ((GlobalPlayer[(int)PlayerID].JugemuFlag & LAVA_EFFECT) != 0))
	{
		GlobalPlayer[(int)PlayerID].JugemuFlag ^= LAVA_EFFECT;
		return;
	}
}

void DisableOutline(char PlayerID) // Fixes custom characters black outlines
{
	if ((unsigned char)GlobalPlayer[(int)PlayerID].Erase == 0xFF)
	{
		GlobalPlayer[(int)PlayerID].Erase = 0x30;
	}
}



// GAMEPLAY // 

void SetMapObjectHit(char PlayerID)
{
	GlobalPlayer[(int)PlayerID].Weapon |= HIT_BOMB;
}

void SetMushroomBoost(char PlayerID)
{
	GlobalPlayer[(int)PlayerID].Weapon |= USE_MUSHROOM;
}

void SetLightningHit(char PlayerID)
{
	GlobalPlayer[(int)PlayerID].Weapon |= HIT_LIGHTNING;
}

void SetShrunken(char PlayerID, bool Active)
{
	if (Active)
	{
		GlobalPlayer[(int)PlayerID].SlipFlag |= IS_LIGHTNING_HIT;
		GlobalPlayer[(int)PlayerID].ThunderTimer = 0x00FF;
	}
	if (!Active)
	{
		GlobalPlayer[(int)PlayerID].SlipFlag |= IS_UNSQUISHED;
		GlobalPlayer[(int)PlayerID].ThunderTimer = 0x01CC;
	}
}

void SetStarMan(char PlayerID, bool Active)
{
	if (Active)
	{
		if ((GlobalPlayer[(int)PlayerID].SlipFlag & IS_STAR) == 0)
		{
			NAEnmTrgStart(GlobalPlayer[(int)PlayerID].Position,GlobalPlayer[(int)PlayerID].Velocity,0x0100ff2c);
		}
		GlobalPlayer[(int)PlayerID].SlipFlag |= IS_STAR;
		StarUseCounter[(int)PlayerID]  = 0xFFFF;
	}
	if (!Active)
	{
		StarUseCounter[(int)PlayerID]  = 0;
		ResetStar((void*)&GlobalPlayer[(int)PlayerID], PlayerID);
		NAEnmTrgStop(GlobalPlayer[(int)PlayerID].Position,0x0100ff2c);
	}

}

void SetGhostEffect(char PlayerID, bool Active, int UseCount)
{

	if (Active)
	{
		
		GhostUseCounter[(int)PlayerID]  = UseCount;
		GhostUseTimer[(int)PlayerID] = 30;
		if ((GlobalPlayer[(int)PlayerID].SlipFlag & IS_BOO) == 0)
		{
			SetVSGhost((void*)&GlobalPlayer[(int)PlayerID], PlayerID);
		}
	}
	if ((!Active))
	{
		GhostUseCounter[(int)PlayerID]  = 0;
		GhostUseTimer[(int)PlayerID]  = -1;
		ResetVSGhost((void*)&GlobalPlayer[(int)PlayerID], PlayerID);
	}

}

void SetBooTranslucent(char PlayerID)
{
	GlobalPlayer[(int)PlayerID].Weapon |= USE_BOO;
}

void SetBecomeBomb(char PlayerID, bool Active)
{
	if (Active)
	{
		GlobalPlayer[(int)PlayerID].Weapon |= PRESS_BECOME_BOMB;
	}
	if ((!Active) && ((GlobalPlayer[(int)PlayerID].Flag & IS_BOMB) != 0))
	{
		GlobalPlayer[(int)PlayerID].Flag ^= IS_BOMB;
	}
}



void SetFlattened(char PlayerID, bool Active)
{
	if (Active)
	{
		GlobalPlayer[(int)PlayerID].Weapon |= PRESS_SQUISH;
	}
	if ((!Active) && ((GlobalPlayer[(int)PlayerID].Weapon & PRESS_SQUISH) != 0))
	{
		GlobalPlayer[(int)PlayerID].Weapon ^= PRESS_SQUISH;
	}
}

void SetSpinOutSaveable(char PlayerID)
{
	GlobalPlayer[(int)PlayerID].Weapon |= HIT_BANANA;
}

void SetSpinOut(char PlayerID)
{
	GlobalPlayer[(int)PlayerID].Weapon |= HIT_CRITTER;
}

void SetFailedStart(char PlayerID)
{
	GlobalPlayer[(int)PlayerID].Weapon |= PRESS_STARTSPIN;
}

void SetGreenShellHit(char PlayerID)
{
	GlobalPlayer[(int)PlayerID].Weapon |= HIT_GREENSHELL;
}

void SetRedShellHit(char PlayerID)
{
	GlobalPlayer[(int)PlayerID].Weapon |= HIT_REDSHELL;
}

void SetBonk(char PlayerID)
{
	GlobalPlayer[(int)PlayerID].SlipFlag |= IS_BONKING;
}



void ChangeMaxSpeed(char PlayerID, float SpeedGain)
{
	GlobalPlayer[(int)PlayerID].AccMaxcount += (float)SpeedGain;
}


void SetCamShiftUp(char PlayerID, float shift)
{
	if (!(GlobalPlayer[(int)PlayerID].Flag & IS_PLAYER))
	{
		return;
	}
	GlobalCamera[(int)PlayerID]->CameraVector[1] = 9 + shift;
}

void ChangePlayerSize(char PlayerID, float ScaleFactor)
{
	if(((GlobalPlayer[(int)PlayerID].Kart) == 5) || ((GlobalPlayer[(int)PlayerID].Kart) == 7))
	{
		GlobalPlayer[(int)PlayerID].Radius = 6 * (float)ScaleFactor;
	}
	else
	{
		GlobalPlayer[(int)PlayerID].Radius = 5.5 * (float)ScaleFactor;
	}
	GlobalPlayer[(int)PlayerID].Offsetsize = (float)ScaleFactor;

	if (GlobalPlayer[(int)PlayerID].Flag&IS_PLAYER)
	{
		GlobalCamera[(int)PlayerID]->CameraVector[1] = 9 * (float)ScaleFactor;
	}
}

void SetPlayerColor(char PlayerID, int Colors, int AdjColor, float Speed)
{
	MakeBodyColor((void*)&GlobalPlayer[(int)PlayerID], PlayerID, Colors, Speed);
	MakeBodyColorAdjust((void*)&GlobalPlayer[(int)PlayerID], PlayerID, AdjColor, Speed);
}

void SetPlayerEcho(char PlayerID, char echo)
{
	if (!(GlobalPlayer[(int)PlayerID].Flag&IS_PLAYER))
	{
		return;
	}	
	GlobalAddressA = (long)(&g_playerEcho) + (0x3C * PlayerID);
	*(char*)GlobalAddressA = echo;
}

void PlayrandmCharacterSFX(char PlayerID)
{
	if ((RNG <= 0x3333))
	{
    	NAPlyVoiceStart(PlayerID, 0x29008008 + (GlobalPlayer[(int)PlayerID].Kart * 0x10));  //voice char lucky sfx
    	return;
    }
	if (RNG <= 0x7777)
	{
    	NAPlyVoiceStart(PlayerID, 0x2900800D + (GlobalPlayer[(int)PlayerID].Kart * 0x10));   //voice char yahho sfx
    	return;
	}
	if (RNG <= 0x9999)
	{
    	NAPlyVoiceStart(PlayerID, 0x29008001 + (GlobalPlayer[(int)PlayerID].Kart * 0x10));   //voice char gogo sfx
    	return;
	}
	NAPlyVoiceStart(PlayerID, 0x2900800C + (GlobalPlayer[(int)PlayerID].Kart * 0x10));   //voice char Jump sfx    
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
			SetBroken((Player*)&GlobalPlayer[PlayerID], PlayerID);
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
			SetVSGhost((Player*)&GlobalPlayer[(int)PlayerID], (char)PlayerID);
			break;
		}
		case StateGhostOff:
		{
			SetGhostEffect(PlayerID,false, 0);
			break;
		}
		case StateLoseUnusedItem:
		{
			if (GlobalPlayer[PlayerID].Flag&IS_PLAYER)
				DynamicObjects[ItemBoxAllocPtr[PlayerID]].Output = 0;
			break;
		}

		
	}
}

#include "../MainInclude.h"





//bool IceSoundPlayed[8];

short GetCourseLapMax(void)
{
    if ((HotSwapID > 0) && (OverKartHeader.LapCount == SPRINT_LAPCOUNT))
    {
        return 1;
    }
    if (HotSwapID > 0)
    {
        if ((OverKartHeader.LapCount > 9) || (OverKartHeader.LapCount < 1))
        {
            return 3;
        }
        if (GameMode == GAMEMODE_TT)
        {
            return 3;
        }
        return OverKartHeader.LapCount;
    }
    return 3;
}

#define SprintFinishWindow  20

float SprintFinishPlane;
float SprintFinishDirection;
float SprintLastZ[8];
bool SprintFinishArmed;
bool SprintNearFinish[8];
Vector SprintStartBanner;

void WrapPathIndexAtFinishCheck(float PosX, float PosY, float PosZ, short *wayPointIndex, int PathIndex)
{
    if ((HotSwapID > 0) && (OverKartHeader.LapCount == SPRINT_LAPCOUNT))
    {
        return;
    }
    WrapPathIndexAtFinish(PosX, PosY, PosZ, wayPointIndex, PathIndex);
}

void SetSprintFinish(void)
{
    Marker *PathArray;
    short LastMarker;

    SprintFinishArmed = false;
    SprintFinishPlane = 0.0f;
    SprintFinishDirection = 0.0f;
    SprintStartBanner[0] = 0.0f;
    SprintStartBanner[1] = 0.0f;
    SprintStartBanner[2] = 0.0f;

    {
        int ThisPlayer;
    for (ThisPlayer = 0; ThisPlayer < 8; ThisPlayer++)
    {
        SprintLastZ[ThisPlayer] = GlobalPlayer[ThisPlayer].Position[2];
        SprintNearFinish[ThisPlayer] = false;
    }
    }

    if ((HotSwapID == 0) || (OverKartHeader.LapCount != SPRINT_LAPCOUNT))
    {
        return;
    }
    if (OverKartHeader.PathLength[0] < 2)
    {
        return;
    }

    PathArray = (Marker *)GetRealAddress(PathTable[0][0]);
    LastMarker = OverKartHeader.PathLength[0] - 1;

    SprintStartBanner[0] = GoalBannerPos[0];
    SprintStartBanner[1] = GoalBannerPos[1];
    SprintStartBanner[2] = GoalBannerPos[2];

    GoalBannerPos[0] = (float)PathArray[LastMarker].Position[0];
    GoalBannerPos[1] = (float)PathArray[LastMarker].Position[1] - 15.0f;
    GoalBannerPos[2] = (float)PathArray[LastMarker].Position[2];
    FinishLineZ = (float)PathArray[LastMarker].Position[2];
    SprintFinishPlane = FinishLineZ;

    // Stock crossing math assumes the racing line runs -Z through the plane.
    // If the designer laid the final marker out the other way, mirror it.
    if (PathArray[LastMarker].Position[2] <= PathArray[LastMarker - 1].Position[2])
    {
        SprintFinishDirection = 1.0f;
    }
    else
    {
        SprintFinishDirection = -1.0f;
    }

    SprintFinishArmed = true;
}

void SprintLapCheck(int PlayerID, Player *car)
{
    float CurrentZ;
    float PreviousZ;
    float Overshoot;
    float Approach;

    if (!SprintFinishArmed)
    {
        CheckLapCount(PlayerID, car);
        return;
    }

    CurrentZ = car->Position[2];
    PreviousZ = SprintLastZ[PlayerID];
    SprintLastZ[PlayerID] = CurrentZ;

    if (g_playerPathPointTable[PlayerID] >= (OverKartHeader.PathLength[0] - SprintFinishWindow))
    {
        SprintNearFinish[PlayerID] = true;
    }

    if (StartingIndicator < 3)
    {
        return;
    }
    if (*GlobalLap[PlayerID] >= 3)
    {
        return;
    }
    if (!SprintNearFinish[PlayerID])
    {
        return;
    }

    Overshoot = SprintFinishDirection * (SprintFinishPlane - CurrentZ);
    Approach = SprintFinishDirection * (PreviousZ - SprintFinishPlane);

    if ((Overshoot < 0.0f) || (Approach <= 0.0f))
    {
        return;
    }

    *GlobalLap[PlayerID] = 3;
    g_timeLapChange[PlayerID] = GameTimer - ((0.01666666f * Overshoot) / (Overshoot + Approach));
}

void SetLapIndex(void)
{
    short LapMax = GetCourseLapMax();
    int Players;

    if ((HotSwapID > 0) && (OverKartHeader.LapCount != SPRINT_LAPCOUNT))
    {
        if ((OverKartHeader.LapCount > 9) || (OverKartHeader.LapCount < 1) || (GameMode == GAMEMODE_TT))
        {
            OverKartHeader.LapCount = 3;
        }
    }
    else if ((HotSwapID == 0) && (GameMode == GAMEMODE_TT))
    {
        OverKartHeader.LapCount = 3;
    }

    Players = PlayerCount;
    if (GameMode == GAMEMODE_GP)
    {
        Players = 8;
    }
    {
        int ThisPlayer;
    for (ThisPlayer = 0; ThisPlayer < Players; ThisPlayer++)
    {
        if ((HotSwapID > 0) && (OverKartHeader.LapCount == SPRINT_LAPCOUNT))
        {
            *GlobalLap[ThisPlayer] = 2;
        }
        else
        {
            *GlobalLap[ThisPlayer] = 2 - LapMax;
        }
    }
    }

    SetSprintFinish();
}

short GetCourseLapIndex(int Player)
{
    short LapMax = GetCourseLapMax();
    int LapIndex;

    if ((HotSwapID > 0) && (OverKartHeader.LapCount == SPRINT_LAPCOUNT))
    {
        return 1;
    }

    LapIndex = *GlobalLap[Player] + LapMax - 2;
    if (LapIndex < 0)
    {
        return 0;
    }
    if (LapIndex > LapMax)
    {
        return LapMax;
    }
    return LapIndex;
}

void CheckSplashRepl(char WaterType)
{	
	char PlayerID;
	for (PlayerID = 0; PlayerID < 8; PlayerID++)
	{
		if (((GlobalPlayer[(int)PlayerID].Flag & EXISTS)) && (GamePausedFlag == 0x00))
		{
			if(!CustomWaterHeight[(int)PlayerID])
			{
				WaterlevelPlayer[(int)PlayerID] = CheckWaterLevel((void*)&GlobalPlayer[(int)PlayerID]);
			}
			if(WaterType != ZVOID || (HotSwapID == 0))
			{
				if(WaterlevelPlayer[(int)PlayerID] >= (GlobalPlayer[(int)PlayerID].Position[1]))
				{
					SET_FLAG(GlobalPlayer[(int)PlayerID].WaterFlag,HALF_SUBMERGED);
				}

				else
				{
					CLR_FLAG(GlobalPlayer[(int)PlayerID].WaterFlag,HALF_SUBMERGED);
				}

				if((WaterlevelPlayer[(int)PlayerID]-(GlobalPlayer[(int)PlayerID].Position[1])) > (GlobalPlayer[(int)PlayerID].Radius))
				{
					SET_FLAG(GlobalPlayer[(int)PlayerID].WaterFlag,SUBMERGED);
					CLR_FLAG(GlobalPlayer[(int)PlayerID].WaterFlag,HALF_SUBMERGED);
				}

				else
				{
					CLR_FLAG(GlobalPlayer[(int)PlayerID].WaterFlag,SUBMERGED);
				}

				if((WaterlevelPlayer[(int)PlayerID] - (GlobalPlayer[(int)PlayerID].Position[1])) > (GlobalPlayer[(int)PlayerID].Radius))
				{
					if(!(GlobalPlayer[(int)PlayerID].WaterFlag&SPLASH_START))
					{
						SET_FLAG(GlobalPlayer[(int)PlayerID].WaterFlag,SPLASH_DIVE);
						SET_FLAG(GlobalPlayer[(int)PlayerID].WaterFlag,SPLASH_START);

						if((CourseID != 6) && (CourseID != 16) && (CourseID != 13) && ((GlobalPlayer[(int)PlayerID].Flag&IS_PLAYER)))
						{
							if(((CourseID == 2) && (CourseID == 19)) || WaterType == ZLAVA)
							{
								NAPlyTrgStart(PlayerID, 0x1900801c);
							}
							else if (WaterType != ZVOID)
							{
								NAPlyTrgStart(PlayerID, 0x19008008);
							}
							if(HotSwapID > 0 && ((GlobalPlayer[(int)PlayerID].Flag&IS_PLAYER)))
							{
								NAPlyTrgStart(PlayerID, 0x29008004 + (GlobalPlayer[(int)PlayerID].Kart * 0x10));
							}
						}
					}
				}

				if((CourseID == 6) || ((CourseID == 16) && (CourseID == 13)))
				{
					CLR_FLAG(GlobalPlayer[(int)PlayerID].WaterFlag,SPLASH_DIVE);
					CLR_FLAG(GlobalPlayer[(int)PlayerID].WaterFlag,SPLASH_START);
				}
			}

			if((WaterlevelPlayer[(int)PlayerID] - (GlobalPlayer[(int)PlayerID].Position[1])) > (GlobalPlayer[(int)PlayerID].Radius) && ((GlobalPlayer[(int)PlayerID].Bump.DistanceZX) >= 600))
			{	
				SET_FLAG(GlobalPlayer[(int)PlayerID].JugemuFlag,IS_IN_WATER);
			}

			if(GlobalPlayer[(int)PlayerID].Bump.DistanceZX >= 600)
			{
				SET_FLAG(GlobalPlayer[(int)PlayerID].JugemuFlag,LAKITU_CAMERA);
			}

			else if(!(GlobalPlayer[(int)PlayerID].SlipFlag&IS_IN_AIR))
			{
				CLR_FLAG(GlobalPlayer[(int)PlayerID].JugemuFlag,LAKITU_CAMERA);
			}

			if((GlobalPlayer[(int)PlayerID].Flag&IS_CPU_PLAYER) && (OoBCheck(GlobalPlayer[(int)PlayerID].Bump.LastZX) || (GlobalPlayer[(int)PlayerID].JugemuFlag&IS_IN_WATER)) && !(GlobalPlayer[(int)PlayerID].JugemuFlag&ON_LAKITU_ROD) && !(GlobalPlayer[(int)PlayerID].JugemuFlag&OUT_OF_BOUNDS) && !(GlobalPlayer[(int)PlayerID].SlipFlag&ON_CENTER_LINE))
			{
				SetLakitu((void*)&GlobalPlayer[(int)PlayerID]);
				CallLakitu((void*)&GlobalPlayer[(int)PlayerID]);
			}

			if((GlobalPlayer[(int)PlayerID].Flag&IS_CPU_PLAYER) && GlobalPlayer[(int)PlayerID].BumpStatus == 253 && !(GlobalPlayer[(int)PlayerID].SlipFlag&IS_IN_AIR))
			{
				SetLakitu((void*)&GlobalPlayer[(int)PlayerID]);
				CallLakitu((void*)&GlobalPlayer[(int)PlayerID]);
			}

			LakituCheck((void*)&GlobalPlayer[(int)PlayerID], PlayerID);
		}
	}
}

void SetWaterType(char WaterType)
{
	char PlayerID;

	if (WaterType == ZICE && HotSwapID != 0)
	{
		OKLakituIce = 1;
	}
	else
	{
		OKLakituIce = 0;
	}
	
	if (StartingIndicator >= 0x01 && StartingIndicator <= 0x06)
	{
		//CheckSplashRepl(WaterType);

		if(WaterType != 0 && HotSwapID != 0)
		{
			for (PlayerID = 0; PlayerID < 8; PlayerID++)						// Loop for each racer
			{
				if (((GlobalPlayer[(int)PlayerID].Flag & EXISTS) != 0) && (GamePausedFlag == 0x00))
				{
					switch (WaterType)
					{
						case ZVOID:
						{	
							if((WaterlevelPlayer[(int)PlayerID] - GlobalPlayer[(int)PlayerID].Position[1]) > (GlobalPlayer[(int)PlayerID].Radius))
							{
								{
								    int i;
								for (i = 0; i < 10; i++)
								{
									GlobalPlayer[(int)PlayerID].Gass[i].Flag = 0;
									GlobalPlayer[(int)PlayerID].Letter[i].Flag = 0;
									GlobalPlayer[(int)PlayerID].smoke[i].Flag = 0;
									GlobalPlayer[(int)PlayerID].Spark[i].Flag = 0;
								}
								}
							}
							if((GlobalPlayer[0].JugemuFlag & ON_LAKITU_ROD) != 0)
							{
								SetAnimWaterDrip(0,0);
							}
							break;
						}

						case ZLAVA:
						{
							if(((GlobalPlayer[(int)PlayerID].JugemuFlag & LAKITU_CAMERA) != 0))
							{
								if(((GlobalPlayer[(int)PlayerID].JugemuFlag & IS_IN_WATER) == 0) && ((GlobalPlayer[(int)PlayerID].JugemuFlag & ON_LAKITU_ROD) == 0) && ((GlobalPlayer[(int)PlayerID].SlipFlag & IS_STAR) == 0))
								{
									SetPlayerColor(PlayerID,0x00340000,0x00004040,1.5);
								}
							}
							if(((GlobalPlayer[(int)PlayerID].WaterFlag & SUBMERGED) != 0) || ((GlobalPlayer[(int)PlayerID].WaterFlag & HALF_SUBMERGED) != 0))
							{
								SetAnimSmoking(PlayerID,1);
							}
							if((GlobalPlayer[(int)PlayerID].WaterFlag & SPLASH_START) != 0)
							{
								{
								    int i;
								for (i = 0; i < 10; i++)
								{
									if(GlobalPlayer[(int)PlayerID].Gass[i].Number == 5)
									{
										GlobalPlayer[(int)PlayerID].Gass[i].Swork1 = 0xFF;
										GlobalPlayer[(int)PlayerID].Gass[i].Swork2 = 0;
										GlobalPlayer[(int)PlayerID].Gass[i].Swork3 = 0;
										GlobalPlayer[(int)PlayerID].Gass[i].Swork4 = 0xFF;
									}
									if(GlobalPlayer[(int)PlayerID].Gass[i].Number == 3)
									{
										GlobalPlayer[(int)PlayerID].Gass[i].Swork1 = 0xFF;
										GlobalPlayer[(int)PlayerID].Gass[i].Swork2 = 0x3F;
										GlobalPlayer[(int)PlayerID].Gass[i].Swork3 = 0x3F;
										GlobalPlayer[(int)PlayerID].Gass[i].Swork4 = 0xFF;
									}
								}
								}
							}
						break;
						}
/* Using stock function instead
						case Water_Ice:
						{
							if(((GlobalPlayer[(int)PlayerID].WaterFlag & SUBMERGED) != 0) && ((GlobalPlayer[(int)PlayerID].JugemuFlag & ON_LAKITU_ROD) != 0))
							{
								GlobalPlayer[(int)PlayerID].JugemuFlag |= IS_FROZEN;
								SetAnimWaterDrip(PlayerID,0);
							}

							if(((GlobalPlayer[(int)PlayerID].JugemuFlag & ON_LAKITU_ROD) != 0) && ((GlobalPlayer[(int)PlayerID].WaterFlag & SUBMERGED) != 0) && !IceSoundPlayed[(int)PlayerID])
							{
								IceSoundPlayed[(int)PlayerID] = true;
								GlobalPlayer[(int)PlayerID].JugemuFlag |= HAS_ICE_CUBE;
								NAPlyTrgStart(PlayerID, 0x1900a055);
							}

							if((GlobalPlayer[(int)PlayerID].JugemuTimer >= 125) && ((GlobalPlayer[(int)PlayerID].JugemuFlag & OUT_OF_BOUNDS) != 0) && ((GlobalPlayer[(int)PlayerID].JugemuFlag & HAS_ICE_CUBE) != 0))
							{
								NAPlyTrgStart(PlayerID, 0x1900a056);

								GlobalPlayer[(int)PlayerID].JugemuFlag ^= OUT_OF_BOUNDS;
								GlobalPlayer[(int)PlayerID].JugemuFlag ^= HAS_ICE_CUBE;
								IceSoundPlayed[(int)PlayerID] = false;
							}

							if(((GlobalPlayer[(int)PlayerID].JugemuFlag & IS_FROZEN) != 0) && ((GlobalPlayer[(int)PlayerID].Accelcount) > 270))
							{
								GlobalPlayer[(int)PlayerID].JugemuFlag ^= IS_FROZEN;
							}
						break;
						}
*/
						case ZHOTSTUFF:
						{
							if((WaterlevelPlayer[(int)PlayerID] - GlobalPlayer[(int)PlayerID].Position[1]) > 0 - (GlobalPlayer[(int)PlayerID].Radius))
							{
								GlobalPlayer[(int)PlayerID].Position[1] = WaterlevelPlayer[(int)PlayerID] + (GlobalPlayer[(int)PlayerID].Radius) + 1;
								GlobalPlayer[(int)PlayerID].JugemuFlag |= LAVA_EFFECT;
								LavaFloorBumpCount[(int)PlayerID]++;
								LavaFloorRecoilRequired[(int)PlayerID] = true;
							}
							
							if (LavaFloorBumpCount[(int)PlayerID] >= 4)
							{
								CallLakitu(&GlobalPlayer[(int)PlayerID]);
							}

							
							//LavaFloorBumpCount[PlayerIndex] += 1;



							break;
						}
						default:
						{				
							break;
						}
					}	
				}
			}
		}
	}
}

void NopSplashCheckCode(void) //Run at custom Code init
{
	OKCheckSplashEnabled = 0;
}

bool EffectStarBGM[4];
bool EffectFinalLapBGM[4];
bool FinalLapActive;
bool FinalLapAnim[4];

void EffectBGMReplace(void)
{
	char PlayerID;

	if (StartingIndicator < 0x03)
	{
		{
		    int EfL;
		for (EfL = 0; EfL < 4; EfL++)
		{
			EffectStarBGM[EfL] = false;
			EffectFinalLapBGM[EfL] = false;
			FinalLapActive = false;
			FinalLapAnim[EfL] = false;
		}
		}
	}

	if (StartingIndicator == 0x03)
	{
		if (EffectStarBGM[0] || EffectStarBGM[1] || EffectStarBGM[2] || EffectStarBGM[3] || FinalLapAnim[0] || FinalLapAnim[1] || FinalLapAnim[2] || FinalLapAnim[3])
		{
			switch (MusicUserVolumeFlag)
			{
				case 0:
				case 1:
				{
					NAMusicVolume(75);
					break;
				}
				case 2:
				{
					NAMusicVolume(0);
					break;
				}
				default:
				{				
					break;
				}	
			}
		}
		else if (!EffectStarBGM[0] || !EffectStarBGM[1] || !EffectStarBGM[2] || !EffectStarBGM[3] || !FinalLapAnim[0] || !FinalLapAnim[1] || !FinalLapAnim[2] || !FinalLapAnim[3])
		{
			switch (MusicUserVolumeFlag)
			{
				case 0:
				{
					NAMusicVolume(127);
					break;
				}
				case 1:
				{
					NAMusicVolume(75);
					break;
				}
				case 2:
				{
					NAMusicVolume(0);
					break;
				}
				default:
				{				
					break;
				}	
			}
		}

		for (PlayerID = 0; PlayerID < PlayerCount; PlayerID++)
		{
			if (((GlobalPlayer[(int)PlayerID].Flag & IS_PLAYER) != 0) && ((GlobalPlayer[(int)PlayerID].Flag & IS_GHOST) == 0) && (GamePausedFlag == 0x00))
			{
				if ((g_GameLapTable[(int)PlayerID] == 2) && (!FinalLapActive))
				{
					if (!((HotSwapID > 0) && (OverKartHeader.LapCount == SPRINT_LAPCOUNT)))
					{
						FinalLapActive = true;
					}
				}
				if ((AnimatedLakituStruct[(int)PlayerID].EventFlag == LAKITU_LAPFINAL))
				{
					FinalLapAnim[(int)PlayerID] = true;
				}
				if ((AnimatedLakituStruct[(int)PlayerID].EventFlag != LAKITU_LAPFINAL))
				{
					FinalLapAnim[(int)PlayerID] = false;
				}
			}
		}

		for (PlayerID = 0; PlayerID < 4; PlayerID++)
		{
			if (((GlobalPlayer[(int)PlayerID].Flag & IS_PLAYER) != 0) && (GamePausedFlag == 0x00))
			{
				if ((GlobalPlayer[(int)PlayerID].SlipFlag & IS_STAR) != 0)
				{
					if (!EffectStarBGM[(int)PlayerID])
					{
						EffectStarBGM[(int)PlayerID] = true;
						NaPlyLevelStart(PlayerID,0x0100ff2c);
					}
					continue;
				}
				if (((GlobalPlayer[(int)PlayerID].SlipFlag & IS_STAR) == 0) && EffectStarBGM[(int)PlayerID])
				{
					EffectStarBGM[(int)PlayerID] = false;
					NaPlyLevelStop(PlayerID,0x0100ff2c);
					NAPlyVoiceStart(PlayerID, 0x29008008 + (GlobalPlayer[(int)PlayerID].Kart * 0x10));
					continue;
				}
			}

			if (((GlobalPlayer[(int)PlayerID].Flag & IS_PLAYER) != 0) && ((GlobalPlayer[(int)PlayerID].Flag & IS_GHOST) == 0) && (GamePausedFlag == 0x00))
			{
				if ((g_GameLapTable[(int)PlayerID] == 2))
				{
					if (!((HotSwapID > 0) && (OverKartHeader.LapCount == SPRINT_LAPCOUNT)))
					{
						if (!EffectFinalLapBGM[(int)PlayerID])
						{
							EffectFinalLapBGM[(int)PlayerID] = true;
							NaPlyLevelStart(PlayerID,0x1900ff3a);
						}
						continue;
					}
				}
			}
		}
	}
	if (GameMode == GAMEMODE_BATTLE && PlayerCount >= 3 && StartingIndicator == 5)
	{
		NAMusicVolume(0);
	}	
}

void NopPlayEffectBGMCode(void) //Run at custom Code init
{
}

void CheckJugemuMarker(void)
{
    OKPathStruct* PathValues;

    //this sucks.
	//timing issue.
	//duplicate loops
    if (HotSwapID == 0)
    {
        return;
    }

	if (OverKartRAMHeader.EchoOffset != 0)
	{
		GlobalIntA = *(int*)OverKartRAMHeader.EchoOffset;
		if (GlobalIntA != 0)
		{
			GlobalAddressA = OverKartRAMHeader.EchoOffset + 4;
			PathValues = (OKPathStruct*)GlobalAddressA;	
			{
			    int PlayerID;
			for (PlayerID = 0; PlayerID < 8; PlayerID++)					// Loop for each racer		
			{
				if (!(GlobalPlayer[(int)PlayerID].Flag&EXISTS))					// Only run for existing racers
				{
					continue;
				}
				if (GlobalPlayer[0].JugemuFlag == 0)
				{
					continue;
				}

				{
				    int ThisValue;
				for (ThisValue = 0; ThisValue < GlobalIntA; ThisValue++)
				{
					if (PathValues[ThisValue].Type == PATH_JUMP)
					{	
						if ((g_playerPathPointTable[(int)PlayerID] >= PathValues[ThisValue].PathStart) && (g_playerPathPointTable[(int)PlayerID] <= PathValues[ThisValue].PathStop))		// Path range check
						{		
							g_playerPathPointTable[PlayerID] = PathValues[ThisValue].PathStart;					
						}
					}
				}
				}
			}
			}
	
		}
	}	
}

short LastAIPath = 0;
void CheckPaths(void)
{	
	OKPathStruct* PathValues;

	Toggle3DSnow = 1; // set to 0 by default.

	if (OverKartRAMHeader.EchoOffset != 0)
	{
		GlobalIntA = *(int*)OverKartRAMHeader.EchoOffset;
		if (GlobalIntA != 0)
		{
			GlobalAddressA = OverKartRAMHeader.EchoOffset + 4;
			PathValues = (OKPathStruct*)GlobalAddressA;		
			{
			    int PlayerID;
			for (PlayerID = 0; PlayerID < 8; PlayerID++)					// Loop for each racer		
			{	
				if (!(GlobalPlayer[(int)PlayerID].Flag&EXISTS))					// Only run for existing racers
				{
					continue;
				}
						
				SetPlayerEcho(PlayerID, 0);
				SetCamShiftUp(PlayerID, 0);
				NoSimpleKartFlag[(int)PlayerID] = 0;
				GlobalPlayer[(int)PlayerID].Talk &= ~0x2; // Unused Talk Flag
				{
				    int ThisValue;
				for (ThisValue = 0; ThisValue < GlobalIntA; ThisValue++)
				{				
					if ((g_playerPathPointTable[(int)PlayerID] >= PathValues[ThisValue].PathStart) && (g_playerPathPointTable[(int)PlayerID] <= PathValues[ThisValue].PathStop))		// Path range check
					{		
						switch (PathValues[ThisValue].Type)
						{
							case (PATH_ECHO):
							{	
								SetPlayerEcho(PlayerID, PathValues[ThisValue].Power);
								break;
							}
							case (PATH_COLOR):
							{	
								Uint BodyColor = GetRGB32(PathValues[ThisValue].ColorR,PathValues[ThisValue].ColorG,PathValues[ThisValue].ColorB);
								Uint AdjColor = GetRGB32(PathValues[ThisValue].AdjColorR,PathValues[ThisValue].AdjColorG,PathValues[ThisValue].AdjColorB);
								MakeBodyColor((void*)&GlobalPlayer[(int)PlayerID], PlayerID, BodyColor , 1);
								MakeBodyColorAdjust((void*)&GlobalPlayer[(int)PlayerID], PlayerID, AdjColor, 1);
								break;
							}
							case (PATH_CAMERA):
							{
								SetCamShiftUp(PlayerID, PathValues[ThisValue].Power);
								break;
							}
							case (PATH_NOSIMPLE):
							{							
								NoSimpleKartFlag[(int)PlayerID] = 1;
								break;
							}
							case (PATH_AIRCONTROL):
							{							
								GlobalPlayer[(int)PlayerID].Talk |= 0x2; // Unused Talk Flag
								break;
							}
							case (PATH_WEATHER):
							{
								if (PlayerID == 0)
								{
									Toggle3DSnow = 1;
								}
								
								break;
							}
						}
					}
				}
				}	
			}
			}
		}
	}

	
	if (OverKartHeader.PathCount > 1)
	{
		//Check Player Lap
		{
		    int ThisPlayer;
		for (ThisPlayer = 0; ThisPlayer < 8; ThisPlayer++)
		{
			if (GlobalPlayer[ThisPlayer].Flag & IS_CPU_PLAYER)
			{	
				if (CPUPaths[ThisPlayer].LastLap != GlobalPlayer[ThisPlayer].rap)
				{
					CPUPaths[ThisPlayer].LastLap = GlobalPlayer[ThisPlayer].rap;
					CPUPaths[ThisPlayer].LastPath = CPUPaths[ThisPlayer].CurrentPath;
					GlobalBoolA = false;

					//Loop until valid path. Max 100 checks.
					{
					    int This;
					for (This = 0; This < 100; This++)
					{
						GlobalShortA = MakeRandomLimmit(OverKartHeader.PathCount);

						if (GlobalShortA > OverKartHeader.PathCount)
						{
							GlobalShortA = OverKartHeader.PathCount;
						}

						if (OverKartHeader.PathSplit != 1)
						{
							break;
						}
						
						if (GlobalShortA != LastAIPath)
						{
							LastAIPath = GlobalShortA;
							break;
						}
					}
					}
					//Assign new path. 
					CPUPaths[ThisPlayer].CurrentPath = GlobalShortA;
					
				}
				if (CPUPaths[ThisPlayer].CurrentPath == -1)
				{
					CPUPaths[ThisPlayer].CurrentPath = 0;
				}
				CurrentPathID[ThisPlayer] = CPUPaths[ThisPlayer].CurrentPath;
			}
			else
			{
				//human Player
				//check surface and assign path.
				if (CPUPaths[ThisPlayer].LastLap != GlobalPlayer[ThisPlayer].rap)
				{
					CPUPaths[ThisPlayer].LastLap = GlobalPlayer[ThisPlayer].rap;
					CurrentPathID[ThisPlayer] = 0;
				}
			}
		}
		}
	}

}


short CalcOGAAreaSubBPWrapper(float mx, float my, float mz, Ushort t_group, int *b_num_ptr)
{
    short       ClosestMarker, BestMarker;
    float       BestDistance;
    float       ClosestDistance;
    short       ClosestPath, BestPath;
    float       Work;
    short LocalPathCount;
    short* PathLengths;

    if (HotSwapID == 0)
    {
        return CalcOGAAreaSubBP(mx, my, mz, t_group, b_num_ptr);
    }

    ClosestMarker = -1;
    BestMarker = -1;
    BestDistance = 9999999.0f;
    ClosestDistance = 9999999.0f;
    ClosestPath = 0;
    BestPath = 0;
    Work = 0.0f;

    LocalPathCount = OverKartHeader.PathCount;        
    PathLengths = (short*)&PathLengthTable[0];
    

    {
        int ThisPath;
    for (ThisPath = 0; ThisPath < LocalPathCount; ThisPath++)
    {
        Marker* Point = (Marker*)GetRealAddress(PathTable[0][ThisPath]);
        {
            int ThisMark;
        for (ThisMark = 0; ThisMark < PathLengths[ThisPath]; ThisMark++)
        {
            Work = 
            (
                (mx - Point[ThisMark].Position[0]) * (mx - Point[ThisMark].Position[0]) + 
                (my - Point[ThisMark].Position[1]) * (my - Point[ThisMark].Position[1]) + 
                (mz - Point[ThisMark].Position[2]) * (mz - Point[ThisMark].Position[2])
            );
            if (Work < ClosestDistance)
            {
                ClosestDistance = Work;
                ClosestPath = ThisPath;
                ClosestMarker = ThisMark;
            }
            
            if ((Point[ThisMark].Group == t_group) && (Work < BestDistance))
            {
                BestDistance = Work;
                BestPath = ThisPath;
                BestMarker = ThisMark;
            }
        }
        }
    }
    }

    if (BestMarker == -1)
    {
        *b_num_ptr = ClosestPath;
        return ClosestMarker;
    }
    *b_num_ptr = BestPath;
    return BestMarker;
}


void SetBalloonColor(int PlayerIndex, int R, int G, int B, int AdjR, int AdjG, int AdjB)
{
	
	BalloonColorArray[GlobalPlayer[PlayerIndex].Kart].R = R;
	BalloonColorArray[GlobalPlayer[PlayerIndex].Kart].G = G;
	BalloonColorArray[GlobalPlayer[PlayerIndex].Kart].B = B;
	BalloonAdjustArray[GlobalPlayer[PlayerIndex].Kart].R = AdjR;
	BalloonAdjustArray[GlobalPlayer[PlayerIndex].Kart].G = AdjG;
	BalloonAdjustArray[GlobalPlayer[PlayerIndex].Kart].B = AdjB;

	BalloonColorArrayB[GlobalPlayer[PlayerIndex].Kart].R = R;
	BalloonColorArrayB[GlobalPlayer[PlayerIndex].Kart].G = G;
	BalloonColorArrayB[GlobalPlayer[PlayerIndex].Kart].B = B;
	BalloonAdjustArrayB[GlobalPlayer[PlayerIndex].Kart].R = AdjR;
	BalloonAdjustArrayB[GlobalPlayer[PlayerIndex].Kart].G = AdjG;
	BalloonAdjustArrayB[GlobalPlayer[PlayerIndex].Kart].B = AdjB;

}


void SetBalloonDefaults(void)
{
	
	{
	    int ThisPlayer;
	for (ThisPlayer = 0; ThisPlayer < 8; ThisPlayer++)
	{
		BalloonColorArray[ThisPlayer].R = StockBalloonColors[ThisPlayer].R;
		BalloonColorArray[ThisPlayer].G = StockBalloonColors[ThisPlayer].G;
		BalloonColorArray[ThisPlayer].B = StockBalloonColors[ThisPlayer].B;
		BalloonAdjustArray[ThisPlayer].R = StockAdjustColors[ThisPlayer].R;
		BalloonAdjustArray[ThisPlayer].G = StockAdjustColors[ThisPlayer].G;
		BalloonAdjustArray[ThisPlayer].B = StockAdjustColors[ThisPlayer].B;

		BalloonColorArrayB[ThisPlayer].R = StockBalloonColors[ThisPlayer].R;
		BalloonColorArrayB[ThisPlayer].G = StockBalloonColors[ThisPlayer].G;
		BalloonColorArrayB[ThisPlayer].B = StockBalloonColors[ThisPlayer].B;
		BalloonAdjustArrayB[ThisPlayer].R = StockAdjustColors[ThisPlayer].R;
		BalloonAdjustArrayB[ThisPlayer].G = StockAdjustColors[ThisPlayer].G;
		BalloonAdjustArrayB[ThisPlayer].B = StockAdjustColors[ThisPlayer].B;
	}
	}

}


void SetBalloonTeams(void)
{
	if (TeamMode == 0)
	{
		SetBalloonDefaults();
		return;
	}
	{
	    int ThisPlayer;
	for (ThisPlayer = 0; ThisPlayer < PlayerCount; ThisPlayer++)
	{
		if (Objectives[ThisPlayer].TeamIndex == 0)
		{
			//Red
			SetBalloonColor(ThisPlayer, 225, 0, 0, 30, 0, 0);
		}
		else
		{
			//Blue
			SetBalloonColor(ThisPlayer, 0, 0, 225, 0, 0, 30);
		}

	}
	}
}

void LakituSpawnBypass(Player *Kart, char PlayerID, float *SpawnVector, float *FacingVector)
{
	if ((GameMode != GAMEMODE_BATTLE) || (HotSwapID == 0))
	{
		GetLakituSpawnPoint(Kart, PlayerID, SpawnVector, FacingVector);
	}
	else
	{
		FacingVector[0] = 0;
		FacingVector[1] = 0;
		FacingVector[2] = 0;

		SpawnVector[0] = SpawnPoint[(int)PlayerID][0];
		SpawnVector[1] = SpawnPoint[(int)PlayerID][1];
		SpawnVector[2] = SpawnPoint[(int)PlayerID][2];
	}
	
}

#include "MainInclude.h"
#include "OKInclude.h"

//OverKart6

extern unsigned short Net[];

int DPRTester;
int RandomDPR;

void LoadLogo()
{

	
	
	//804CCB9C
	SetSegment(0x8, (void*) &ok_Logo);
	
	*SourceAddress = (int)(&LogoROM);
	*TargetAddress = (int)(&ok_FreeSpace);
	DataLength = 0x4000;
	RunDMA();
	*SourceAddress = (int)(&ok_FreeSpace);
	*TargetAddress = (int)(&ok_Logo);
	RunMIO();

	/* batch6: g_NintendoLogoOffset / g_NintendoLogoBorder are Pool overlay + mid-function SMC */
	
}


void SetMenuPanels(int SwitchMode)
{

	

	
	GameOKMenu.PanelAddress = (OKPanel*)(Uint)&RacePanel;
	GameOKMenu.PanelAddress[0].NameAddress = (Uint)MenuNames[0];
	GameOKMenu.PanelAddress[1].NameAddress = (Uint)MenuNames[1];
	GameOKMenu.PanelAddress[2].NameAddress = (Uint)MenuNames[2];
	GameOKMenu.PanelAddress[0].NameLength = MenuChar[0];
	GameOKMenu.PanelAddress[1].NameLength = MenuChar[1];
	GameOKMenu.PanelAddress[2].NameLength = MenuChar[2];
	
	GameOKMenu.PanelAddress[1].OptionCount = PageLimit[1];
	GameOKMenu.PanelAddress[2].OptionCount = PageLimit[2];
	
	GameOKMenu.PanelAddress[1].ParameterToggles = (char*)&RenderMode[0];
	GameOKMenu.PanelAddress[2].ParameterToggles = (char*)&LevelSettingsMode[0];
	GameOKMenu.PanelAddress[0].Options = (OKOption*)&OKGameOptions;
	GameOKMenu.PanelAddress[1].Options = (OKOption*)&OKRenderOptions;
	GameOKMenu.PanelAddress[2].Options = (OKOption*)&OKMapOptions;

	
	switch(SwitchMode)
	{
		case RACESWITCH:		
		{
			GameOKMenu.PanelAddress[0].OptionCount = PageLimit[0];
			GameOKMenu.PanelAddress[0].ParameterToggles = (char*)&GameType[0];
			{ int ThisLoop; for (ThisLoop = 0; ThisLoop < PageLimit[0]; ThisLoop++)
			{
				GameOKMenu.PanelAddress[0].Options[ThisLoop].ParameterCount = GameLimits[ThisLoop];
				GameOKMenu.PanelAddress[0].Options[ThisLoop].ParameterNames = (Uint*)&GameParameters[ThisLoop][0];
				GameOKMenu.PanelAddress[0].Options[ThisLoop].ParameterLengths = (int*)&GameChar[ThisLoop];
				GameOKMenu.PanelAddress[0].Options[ThisLoop].OptionName = (Uint)GameOptionNames[ThisLoop];
			} }
			break;
		}
		case BATTLESWITCH:
		{
			GameOKMenu.PanelAddress[0].OptionCount = PageLimit[4];
			GameOKMenu.PanelAddress[0].ParameterToggles = (char*)&BattleMode[0];

			{ int ThisLoop; for (ThisLoop = 0; ThisLoop < PageLimit[4]; ThisLoop++)
			{
				GameOKMenu.PanelAddress[0].Options[ThisLoop].ParameterCount = BattleLimits[ThisLoop];
				GameOKMenu.PanelAddress[0].Options[ThisLoop].ParameterNames = (Uint*)&BattleParameters[ThisLoop][0];
				GameOKMenu.PanelAddress[0].Options[ThisLoop].ParameterLengths = (int*)&BattleChar[ThisLoop];
				GameOKMenu.PanelAddress[0].Options[ThisLoop].OptionName = (Uint)BattleOptionNames[ThisLoop];
			} }
			break;
		}
	}
	{ int ThisLoop; for (ThisLoop = 0; ThisLoop < PageLimit[1]; ThisLoop++)
	{
		GameOKMenu.PanelAddress[1].Options[ThisLoop].ParameterCount = RenderLimits[ThisLoop];
		GameOKMenu.PanelAddress[1].Options[ThisLoop].ParameterNames = (Uint*)&RenderParameters[ThisLoop];
		GameOKMenu.PanelAddress[1].Options[ThisLoop].ParameterLengths = (int*)RenderChar[ThisLoop];
		GameOKMenu.PanelAddress[1].Options[ThisLoop].OptionName = (Uint)RenderOptionNames[ThisLoop];
	} }

	{ int ThisLoop; for (ThisLoop = 0; ThisLoop < PageLimit[2]; ThisLoop++)
	{
		GameOKMenu.PanelAddress[2].Options[ThisLoop].ParameterCount = MapLimits[ThisLoop];
		GameOKMenu.PanelAddress[2].Options[ThisLoop].ParameterNames = (Uint*)&MapParameters[ThisLoop];
		GameOKMenu.PanelAddress[2].Options[ThisLoop].ParameterLengths = (int*)MapChar[ThisLoop];
		GameOKMenu.PanelAddress[2].Options[ThisLoop].OptionName = (Uint)MapOptions[ThisLoop];
	} }
}
void OKSetup(void)
{
	LoadLogo();
	LoadArrows();
	LoadHeaderOffsets();
	LoadHudButtons();
	SetupFontF3D();
	
		
	
	
	LoadEEPROM((Uint)&SaveGame);
	
	CheckContPackMenu();
	
	#if OverKartBuild
		CheatSetup();
	#endif

	BackupStockAudioTables();

	*SourceAddress = (int)&BombTable;
	*TargetAddress = (int)&ok_Bomb;	
	DataLength = 0xA8;
	RunRAM();
	
	
	LoadBigFont();
	
	NopSplashCheckCode();
	//FlyCamInit();

	HotSwapID = 0;
	CopyCourseTable(1);
    CopyCourseTable(0);
	RaceStatus = 0xFF;
	OKMusicIn3P4P = 1;
	
	SetFontColor(255,255,255,75,75,75);
	
	/* batch6: g_sfxPause is SequencePlayer overlay (gSequencePlayers+0x293), not a named BSS */
	HotSwapGP = 0;
	MapModeCheck = 0;
	PrintCount = 0;
	//FlyCamSpeed = 5;
	//ok_Knowledge = (long)(&ok_Target);

	YFLIP = 0;
	ZFLIP = 0;
	
	//PlayerSelectSetup();
	
	/* FreeSpaceAddress was ok_Storage; dest is ok5_MenuStorage */
	SaveGame.TENNES = true;

	GameOKMenu.PanelCount = 3;
	
	SetMenuPanels(RACESWITCH);
	StartupSwitch = 1;
}
bool CheckEndGame()
{
	if (PlayerCount == 0x01)
	{
		if (g_playerPosition1 <= 3)
		{
			return true;
		}
		else if (SaveGame.GameSettings.GPMode == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if ((g_playerPosition1 <= 3) || (g_playerPosition2 <= 3))
		{
			return true;
		}
		else if (SaveGame.GameSettings.GPMode == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
}



void StartRace()
{
	MapIsCurrentlyLoading = true;
	
	

	#if OverKartBuild
	{
		LoadCheatData();
	}				
	#endif

	SetZoomLevel(1); // (All races Start zoomed out)
	LoadedcourseFlag = 0xF0;
	InitialCustomParticleBuffer();
    SetLapIndex();    
    SetSky();
    SetWater();
    LoadMinimap();	
    
	
	
	if ((SaveGame.GameSettings.StatsMode == 1) || (GameMode == GAMEMODE_TT))
	{
		CheckStats(1);
	}
	else
	{
		CheckStats(0);
	}

	if ((GameMode == GAMEMODE_GP) || ((GameMode == GAMEMODE_VS) && (SaveGame.GameSettings.AIMode > 0)) )
	{
		SetAIDifficulty(RaceClass);
	}

	
	if (HotSwapID > 0)
	{
		if ((GameMode != GAMEMODE_TT) && (GameMode != GAMEMODE_BATTLE))
		{
			PlaceSIBox(GetRealAddress(0x06000668));
		}
		if (GameMode != GAMEMODE_BATTLE)
		{
			EmptyActionData();
		}
		
		
		
		if (GameMode != GAMEMODE_BATTLE)
		{	
			if ((SaveGame.GameSettings.GameType == 1))  
			{     
				//Red Coin Challenge   	
				RedCoinChallenge(GetRealAddress(0x06000620));
				CoinCount[0] = 0;
				CoinCount[1] = 0;
				CoinCount[2] = 0;
				CoinCount[3] = 0;
			}
			
			if ((SaveGame.GameSettings.GameType == 2))  
			{      
				//Gold Coin Challenge  	
				
				
				GlobalIntA = (int)(8 * PlayerCount); // CoinCount
				
				if (GlobalIntA < 10)
				{
					GlobalIntA = 10;
				}
				GoldCoinChallenge(GetRealAddress(pathOffset), GlobalIntA);
				CoinCount[0] = 0;
				CoinCount[1] = 0;
				CoinCount[2] = 0;
				CoinCount[3] = 0;
				CoinCount[4] = 0;
				CoinCount[5] = 0;
				CoinCount[6] = 0;
				CoinCount[7] = 0;
			}
		}
		if (VersionNumber > 4)
		{
			{ int This; for (This = 0; This < 100; This++)
			{
				ClearOKObject(This);
			} }
			LoadOKObjects();
			SetOKObjects();	
		}
	}
	else
	{
		if ((SaveGame.GameSettings.GameType == 2))  
		{      
			//Gold Coin Challenge  	
			
			
			GlobalIntA = (int)(8 * PlayerCount); // CoinCount
			
			if (GlobalIntA < 10)
			{
				GlobalIntA = 10;
			}


			if (CourseID != 4)
			{
				GoldCoinChallenge(GetRealAddress((int) PathTable[CourseID][0]), GlobalIntA);
			}
			else
			{
				GlobalIntA = (int)(GlobalIntA * 0.3f);
				GoldCoinChallenge(GetRealAddress((int) PathTable[CourseID][0]), GlobalIntA);
				GoldCoinChallenge(GetRealAddress((int) PathTable[CourseID][1]), GlobalIntA);
				GoldCoinChallenge(GetRealAddress((int) PathTable[CourseID][2]), GlobalIntA);
				GoldCoinChallenge(GetRealAddress((int) PathTable[CourseID][3]), GlobalIntA);
			}
			
			CoinCount[0] = 0;
			CoinCount[1] = 0;
			CoinCount[2] = 0;
			CoinCount[3] = 0;
			CoinCount[4] = 0;
			CoinCount[5] = 0;
			CoinCount[6] = 0;
			CoinCount[7] = 0;
		}
	}
}

void EndRace()
{
	ScrollLock = false;
}

void CheckIFrames()
{
	{ int ThisPlayer; for (ThisPlayer = 0; ThisPlayer < 8; ThisPlayer++)
	{
		if (IFrames[ThisPlayer] != 0)
		{
			IFrames[ThisPlayer]--;
		}
	} }
}




void ExecuteItemHook(Player* Car)
{
	if (GameMode != GAMEMODE_BATTLE)
	{
		ExecuteItem(Car);
	}
	else
	{
		switch (SaveGame.BattleSettings.GameType)
		{
			case BTL_BATTLE:
			{
				ExecuteItem(Car);
				break;
			}
			case BTL_CTF:
			{
				int PlayerID = (int)(Car - GamePlayers);
				
				//if (GlobalController[PlayerID]->ButtonPressed & BTN_Z)				
				{
					if (CheckExecuteItem(PlayerID))
					{
						ExecuteItem(Car);
					}
				}
				break;
			}
			case BTL_SOCCER:
			{
				int PlayerID = (int)(Car - GamePlayers);
				
				if (GlobalController[PlayerID]->ButtonPressed & BTN_Z)
				{
					if (CheckExecuteItem(PlayerID))
					{
						ExecuteItem(Car);
					}
				}
				break;
			}
		}
	}
}


void CheckBattleCDown()
{
	if (GameMode == GAMEMODE_BATTLE)
    {
        switch (SaveGame.BattleSettings.GameType)
        {
            case BTL_BATTLE:
            {
                { int ThisPlayer; for (ThisPlayer = 0; ThisPlayer < PlayerCount; ThisPlayer++)
                {
                    if (GlobalController[ThisPlayer]->ButtonPressed & BTN_CDOWN)				
                    {
                        ExecuteItem((Player*)&GlobalPlayer[ThisPlayer]);
                    }
                } }
                break;
            }
            case BTL_CTF:
            {
                { int ThisPlayer; for (ThisPlayer = 0; ThisPlayer < PlayerCount; ThisPlayer++)
                {
                    if (GlobalController[ThisPlayer]->ButtonPressed & BTN_CDOWN)				
                    {
                        DropFlag(ThisPlayer);
                    }
                } }
                break;
            }
            case BTL_SOCCER:
            {
                { int ThisPlayer; for (ThisPlayer = 0; ThisPlayer < PlayerCount; ThisPlayer++)
                {
                } }
                break;
            }
        }
    }
    else
    {
        { int ThisPlayer; for (ThisPlayer = 0; ThisPlayer < PlayerCount; ThisPlayer++)
        {
            if (GlobalController[ThisPlayer]->ButtonPressed & BTN_CDOWN)				
            {
                ExecuteItem((Player*)&GlobalPlayer[ThisPlayer]);
            }
        } }
    }
	
	
}


short HighCamAngle, HighCamDistance = 35;
void QuickCamCode(Camera* LocalCamera, Player* LocalPlayer)
{
	return;
	LocalPlayer->Status = LocalPlayer->Status|(0x000A);
	ObjectPosition[0] = 0;
	ObjectPosition[1] = 0;
	ObjectPosition[2] = HighCamDistance;

	MakeAlignVector(ObjectPosition, HighCamAngle);

	if (GlobalController[0]->ButtonHeld & BTN_DLEFT)
	{
		HighCamAngle+=DEG1;
	}
	if (GlobalController[0]->ButtonHeld & BTN_DRIGHT)
	{
		HighCamAngle-=DEG1;
	}
	if (GlobalController[0]->ButtonHeld & BTN_DUP)
	{
		HighCamDistance--;
	}
	if (GlobalController[0]->ButtonHeld & BTN_DDOWN)
	{
		HighCamDistance++;
	}

	{ int ThisVector; for (ThisVector = 0; ThisVector < 3; ThisVector++)
	{
		LocalCamera->CameraPos[ThisVector] = LocalPlayer->Position[ThisVector] + ObjectPosition[ThisVector];	
		LocalCamera->LookatPos[ThisVector] = LocalPlayer->Position[ThisVector];			            
	} }
	LocalCamera->CameraPos[1] += 20;
}

#if !OverKartBuild
void CameraCheckFunc(Player* Player, Camera* Camera, s8 Index)
{
	CameraControl(Player, Camera, Index);
	QuickCamCode(Camera, Player);
}

void KartCheckFuncA(void)
{
	DrawKart();
}

void KartCheckFuncB(void)
{
	DrawKart2P();
}

void KartCheckFuncC(void)
{
	DrawKart3P();
}

void KartCheckFuncD(void)
{
	DrawKart4P();
}

void KartCheckFuncA2(void)
{
	DrawKartAfter();
}

void KartCheckFuncB2(void)
{
	DrawKart2PAfter();
}

void KartCheckFuncC2(void)
{
	DrawKart3PAfter();
}

void KartCheckFuncD2(void)
{
	DrawKart4PAfter();
}

void ResultsCheck(void)
{
}
#endif


void SetBattle()
{
	if (GameMode == GAMEMODE_BATTLE)
	{
		TeamMode = SaveGame.BattleSettings.TeamBattle;
		
		switch (SaveGame.BattleSettings.GameType)
		{
			case BTL_BATTLE:
			{
				SlayerInit();
				SetBalloonTeams();
				break;
			}
			case BTL_CTF:
			{
				PlaceFlags((Uint)&BattleFlag_Red_GFX, FlagModels, (Uint)&BattleMushroom_Red_GFX, MushroomModels, 1);
				SetBalloonTeams();
				break;
			}
			case BTL_SOCCER:
			{
				//PlaceBalls(SoccerBall, RedMushroom, MushroomModels, 1);  //lol
			}
		}
	}
}
void GameCode(void)
{	
	#if OverKartBuild
	{
		ApplyCheats();
	}
	#endif

    
	#if(DEBUGBUILD)
	{
		if (GlobalController[0]->ButtonHeld & BTN_DLEFT)
		{
			*GlobalLap[0] = 2;
		}
	}
	#endif
	CheckIFrames();
	
	if (SaveGame.RenderSettings.Platform == 0)
	{
		//Console
		OKStaticTempoOnly = 0;
		if (GameMode != GAMEMODE_TT)
		{
			DynamicTempo();
		}
		else
		{
			if (HotSwapID > 0)
			{
				StaticTempo(OverKartHeader.ManualTempo);
			}
			else
			{
				StaticTempo(2);
			}
		}
		
	}
	else
	{
		//emulator
		OKStaticTempoOnly = 1;
		StaticTempo(2);
	}
	
	CheckBattleCDown();

	if (GameMode == GAMEMODE_BATTLE)
	{
		switch(SaveGame.BattleSettings.GameType)
		{
			case BTL_BATTLE:
			{
				break;
			}
			case BTL_CTF:
			{							
				CaptureFlag();
				DisplayGameScore();
				break;
			}
			case BTL_SOCCER:
			{
				CaptureBalls();
				DisplayGameScore();
				break;
			}
		}
		
	}

	if (MapModeCheck || (SaveGame.GameSettings.GameType == 3))
	{
		PracticeHack();		
	}
	
	if (GameMode != GAMEMODE_BATTLE)
	{
		if (SaveGame.GameSettings.FlycamMode == 1)
		{
			RunFlyCam();
		}
	}

	
	if (GameMode == GAMEMODE_BATTLE)
	{
		if (SaveGame.BattleSettings.Flycam == 1)
		{
			RunFlyCam();
		}
	}
	
	
	SurfaceExplorerMode = SaveGame.GameSettings.ExploreMode;

	if (SaveGame.RenderSettings.InputMode > 0x00)
	{
		DrawInputDisplay();
	}

	if (SaveGame.RenderSettings.DetailMode > 0x00)
	{
		PrintDetails();
	}

	
	if (HotSwapID > 0)   //Version 4 Texture Scroll Function
	{			
		if ((GamePausedFlag == 0) && (ScrollLock))
		{
			RunTextureScroll();
			CheckOKObjects();
			RunDisplayScreen();

			//Dynamic WaveRace Water 
			RunWaterVertex();
			
			LavaFloorRecoil();
			CheckPaths();
		}	
	}
	

	if ((HotSwapID > 0) || (SaveGame.RenderSettings.DrawMode == 1))
	{
		FarClip = 20000;
	}
	else
	{
		FarClip = 6800;
	}


	if (StartingIndicator == 1)
	{
			// Version 5 and Above.
		// Adds Scrolling Textures.
		// Adds Tranlucent Textures.

		if (RaceStatus != 1)
		{
			SetBattle();
			RaceStatus = 1;
			StartRace();
			HsLabel = -1;
			MenuChanged = -1;
		}

		
		GlobalShortD = 0;
		
	}
	if (StartingIndicator == 2)
	{
		if (RaceStatus != 2)
		{
			RaceStatus = 2;			
		}
		
		ScrollLock = true;
		
		if (GlobalShortD < 60)
		{	
			PrintAnticheat();		
			if  (HotSwapID > 0)
			{
				LoadFont();
				GlobalIntA = *(long*)(&ok_Credits);
				if (GlobalIntA != 0)
				{		
							
					PrintString( (140 - (GlobalIntA * 4)), 150, (char*)(&ok_Credits + 1));
				}
				GlobalIntA = *(long*)(&ok_SerialKey);
				if ((GameMode == GAMEMODE_TT) && (GlobalIntA != 0))
				{
					PrintString( (140 - (GlobalIntA * 4)), 160, (char*)(&ok_SerialKey + 1));
					PrintStringNumber(76,170,"Base Version -",OverKartHeader.Version);
				}			
				if (GameMode == GAMEMODE_GP)
				{
					PrintGPTime(GPTotalTime[0],0);			
				}
			}
			

			GlobalShortD += 1;
		}


		
	}
	if (StartingIndicator == 3)
	{
		RaceStatus = 3;
		if ((SaveGame.GameSettings.GameType == 2) && (HotSwapID > 0) && (MenuMultiplayerSelection == 1))
		{
			DisplayCoinSprite();			
		}
		
	}
	
	if (StartingIndicator == 5)
	{
		if (RaceStatus != 5)
		{
			GPTotalTime[0] += g_TrialTime[0];
			RaceStatus = 5;
		}
		if (GameMode == GAMEMODE_GP)
		{
			PrintGPTime(GPTotalTime[0],30);
		}
	}

	if (StartingIndicator == 6)
	{	
		RaceStatus = 6;
	}

	if (StartingIndicator == 7)
	{
		
		if (RaceStatus != 7)
		{
			RaceStatus = 7;	
			
			EndRace();
		}
		
	}
	
	if(SaveGame.RenderSettings.DisplayFPS == 1)
	{
		DrawFPS(220,10);	
	}

}
void ResetMap()
{
	GPTotalTime[0] = 0;
	GPTotalTime[1] = 0;
	GPCourseIndex = 0;
}


Ushort PiracyCountdown = 0;



void AllRun()
{


    
	//Emulator Only ForceHack
	//SaveGame.RenderSettings.Platform = 1;
	//SaveGame.RenderSettings.CullMode = 1;
	//Emulator ForceHack

	
	if (SaveGame.RenderSettings.CullMode == 1)
	{
		//Emulator
		/* batch6: CullDL_Parameters poke; decoder uses local gsSPCullDisplayList(0, 7) */
	}
	else
	{
		//Console
		/* batch6: CullDL_Parameters poke; decoder uses local gsSPCullDisplayList(0, 7) */
	}

	BattleGametype = SaveGame.BattleSettings.GameType;


	ClockCycle[0] = osGetCount();
	CycleCount[0] = (ClockCycle[0] - OldCycle[0]);
	OldCycle[0] = ClockCycle[0];
	GlobalFrameCount++;

	BackupStockAudioTables();

	switch (StartupSwitch)
	{
		case 0:
		OKSetup();
		break;

		case 1:
		
		if (((GlobalController[0]->ButtonHeld & BTN_L) == BTN_L) && ((GlobalController[0]->ButtonPressed & BTN_DDOWN) == BTN_DDOWN))
		{
			MapModeCheck = 1;	
		}
		break;
		
		case 2:
		ModCheck();
		break;
	}
	
	
	//MatrixCount = 0;
	
	
	if ((HotSwapID == 0) || (OverKartHeader.BackgroundToggle == 1))
	{
		BackgroundFlag = 1;
	}
	else
	{
		BackgroundFlag = 0;
	}
	

	if (SaveGame.GameSettings.CupMode > 0x00)
	{

		AsmCupCount = 15;

	}
	else
	{
		#if(DEBUGBUILD)
		{
			AsmCupCount = 0;
		}
		#else
		{
			AsmCupCount = 3;
		}
		#endif
		
	}

    //winter 2026
    AsmCupCount = 3;
	
	SetWeather3D(OverKartHeader.SkyType == 3);


	SetWaterType((char)OverKartHeader.WaterType);
	
	if (HotSwapID > 0)
	{
		if (GameMode != GAMEMODE_BATTLE)
		{
			CourseID = 0;
		}
		else
		{
			CourseID = 15;
		}
	}

	if (CourseID == 0x14)
	{
		GlobalScreen[0]->Width = 0x0240;
	}
	
	switch(KBGNumber)
	{
		#if OverKartBuild
			case 99:
			{
				if (MenuChanged != 99)
				{
					LastMenuID = MenuChanged;
					MenuChanged = 99;
				}

				
				CheatMenuHandler();

				
				break;
			}

			case 98:
			{
				if (MenuChanged != 98)
				{
					*SourceAddress = (int)(&Pirate);
					*TargetAddress = (int)(&ok_FreeSpace);
					DataLength = (int)&PirateEnd - (int)&Pirate;
					RunDMA();
					*SourceAddress = (int)(&ok_FreeSpace);
					*TargetAddress = (int)(&ok5_MenuStorage);
					RunMIO();
					PiracyCountdown = 60;

					MenuChanged = 98;
				}

				

				if (PiracyCountdown <= 0)
				{
					
					if (GlobalController[4]->ButtonPressed)
					{	
						SpriteBtnStart(200,120,0.5f,true);
						KBGNumber = 10;
						KBGNumberNext = 10;
						KBGChange = 1;
						SetFadeOutTaData();
					}
					else
					{
						SpriteBtnStart(200,120,0.5f,false);
					}
				}
				else
				{
					PiracyCountdown--;
				}
				break;

			}
		#endif
		case 6:
		{
			if (MenuChanged != 6)
			{
				LastMenuID = MenuChanged;
				MenuChanged = 6;
			}
			GameOptionsHandler(4);
			break;
		}
		case 8:
		{
			if (MenuChanged != 8)
			{
				LastMenuID = MenuChanged;
				MenuChanged = 8;
			}
			break;
		}
		case 10:
		{			
			#if OverKartBuild
			if (SaveGame.TENNES == true)
			{
				SaveGame.TENNES = false;
				KBGNumber = 98;
				KBGNumberNext = 98;
				KBGChange = 1;
				SetFadeOutTaData();
				break;
			}
			#endif
			if (MenuChanged != 10)
			{	
				//MenuToggle = 0;
				LastMenuID = MenuChanged;
				MenuChanged = 10;
				
				MenuIndex = 0;
				LoadCoinSprite();
				LoadArrows();
				LoadNumberSprites();
				LoadCoin();
				
				
				//SetFontColor(26,26,29,12,12,15);ok_sto
				

				#if OverKartBuild
				MenuAngle[2] = -20;
				MenuAngle[3] = 0;

				*SourceAddress = (int)(&Splash3D);
				*TargetAddress = (int)(&ok_FreeSpace);
				DataLength = (int)&Splash3DEnd - (int)&Splash3D;
				RunDMA();
				*SourceAddress = (int)(&ok_FreeSpace);
				*TargetAddress = (int)(&ok5_MenuStorage);
				Splash3DRAM = *TargetAddress;
				GlobalAddressA = RunMIO();
				
				SetSegment(0x8, (void*) Splash3DRAM);

				*SourceAddress = (int)(&MenuIconsROM);
				*TargetAddress = (int)(&ok_FreeSpace);
				DataLength = (int)&MenuIconsEnd - (int)&MenuIconsROM;
				RunDMA();
				*SourceAddress = (int)(&ok_FreeSpace);
				*TargetAddress = GlobalAddressA;
				MenuIconsRAM = GlobalAddressA;
				GlobalAddressA = RunMIO();
				
				#endif

				
				StartupSwitch = 2;



				if ((SaveGame.SaveVersion != 8) && (SaveGame.SaveVersion != 99))
				{
					SaveGame.SaveVersion = 8;	
					SaveGame.Initial = 0;
					{ int This; for (This = 0; This < 8; This++)
					{
						RenderMode[This] = 0;
						GameType[This] = 0;
						LevelSettingsMode[This] = 0;
						BattleMode[This] = 0;
					} }
					{ int This; for (This = 8; This < 16; This++)
					{
						RenderMode[This]  = 0;
					} }
					SaveGame.RenderSettings.AliasMode = 1;
					SaveGame.RenderSettings.Platform = 1;
					SaveGame.RenderSettings.CullMode = 1;
					
					SaveGame.LevelSettings.ScaleXMode = 2;
					SaveGame.LevelSettings.ScaleYMode = 2;
					SaveGame.LevelSettings.ScaleZMode = 2;

					#if ProtectMode
						//SaveGame.TENNES = DPR();
						//DPRSave();
					#endif
					
				}

				ScrollLock = false;
				StartingIndicator = 0;
				break;
			}
			break;
		}
		
		case 11:
		{
			if (MenuChanged != 11)
			{
				NaSeqStart(2);
				MenuScreenB = 3; //resets Mode selection to Start screen.
				LastMenuID = MenuChanged;
				MenuChanged = 11;
				SaveGame.Initial = 1;
				//MenuToggle = 0;
				
				/*
				*SourceAddress = (int)(&MenuIconsROM);
				*TargetAddress = (int)(&ok_FreeSpace);
				DataLength = (int)&MenuIconsEnd - (int)&MenuIconsROM;
				RunDMA();
				*SourceAddress = (int)(&ok_FreeSpace);
				*TargetAddress = (int)(&ok5_MenuStorage);
				MenuIconsRAM = (int)(&ok5_MenuStorage);
				GlobalAddressA = RunMIO();
				*/
				*SourceAddress = (int)(&ROptionROM);
				*TargetAddress = (int)(&ok_FreeSpace);
				DataLength = (int)&ROptionEnd - (int)&ROptionROM;
				RunDMA();
				*SourceAddress = (int)(&ok_FreeSpace);
				*TargetAddress = (int)(&ok5_MenuStorage);
				MenuIconsRAM = (int)(&ok5_MenuStorage);
				GlobalAddressA = RunMIO();

				ScrollLock = false;
				StartingIndicator = 0;
				//HotSwapID = 0;
				SaveEEPROM((Uint)&SaveGame);
			}
			
			CopyCourseTable(0);
			break;
		}
		case 12:
		{
			
			if (MenuChanged != 12)
			{					
				//MenuToggle = 0;
				if (GameMode == GAMEMODE_BATTLE)
				{
					SetMenuPanels(BATTLESWITCH);
				}
				else
				{
					SetMenuPanels(RACESWITCH);
				}

				//PlayerSelectMenuBefore();
				MenuProgress[0] = 0;
				MenuProgress[1] = 0;
				MenuProgress[2] = 0;
				MenuProgress[3] = 0;

				LastMenuID = MenuChanged;
				MenuChanged = 12;   
				//MenuScreenC = 0;    
					
				ScrollLock = false;
				HotSwapID = 0;
				CopyCourseTable(0);   
			}
			break;
		}			
		case 13:
		{
			if ((MapIsCurrentlyLoading) || (GameState == RACING))
			{
				break;
			}
			
			if (MenuChanged != 13)
			{
				//MenuToggle = 0;

				LastMenuID = MenuChanged;
				MenuChanged = 13;
				
				
				//PlayerSelectMenuAfter();

				
				//
				//
				//reload the R options graphic 
				*SourceAddress = (int)(&ROptionROM);
				*TargetAddress = (int)(&ok_FreeSpace);
				DataLength = (int)&ROptionEnd - (int)&ROptionROM;
				RunDMA();
				*SourceAddress = (int)(&ok_FreeSpace);
				*TargetAddress = (int)(&ok5_MenuStorage);
				MenuIconsRAM = (int)(&ok5_MenuStorage);
				GlobalAddressA = RunMIO();
				// finish loading Press R graphic
				//
				//

				MenuIndex = 0;
				ResetMap();
				SetAlwaysAdvance();				
                HotSwapID = 0;
                CopyCourseTable(0);
                SetPreviews();
                PreviewRefresh();
                SetBanners();
                HsLabel = -1;
                CourseValue = -1;

				ScrollLock = false;
				StartingIndicator = 0;
				
			}				
		


			
			break;
			
		}
		default:
		{
			HsLabel = -1;
			MenuChanged = -1;
			break;
			
		}
		
	}

	MapIsCurrentlyLoading = false;
}


void PrintMenuFunction()
{
	ClockCycle[1] = osGetCount();
	CycleCount[1] = (ClockCycle[1] - OldCycle[1]);     
	OldCycle[1] = ClockCycle[1];

    
    

	if(SaveGame.RenderSettings.DisplayFPS == 1)
	{
		DrawFPS(220,10);	
	}
	
	switch (MenuChanged)
	{
		#if OverKartBuild
		case 99:
		{
			//Draw Cheats Menu
			DrawCheatMenu();
			break;
		}
		case 98:
		{
			KWSpriteDiv(256,120,(Ushort*)&ok5_MenuStorage,512,240,4);
			break;
		}
		#endif
		case 6:
		{
			//Draw OptionsMenu
			
			if (MenuIndex < GameOKMenu.PanelCount)
			{
				ModularMenu((OKMenu*)&GameOKMenu);
				
				if (MenuIndex == 2)  //filthy hack
				{
					KWSprite(240,22,16,16,(Ushort*)&lit_arrowsprite_r);
				}
			}
			else
			{
				CourseMenu(175);
			}
			break;
		}
		case 8:
		{
			//logo
			
			break;
		}
		case 10:
		{
			//title screen
			
			
			
			DrawBox(102,150,120,38, 0, 0, 0, 200);
			
			if (SaveGame.RenderSettings.CullMode == 0)
			{
				PrintBigText(128, 151, 0.6f, "LLE MODE" );
			}
			else
			{
				PrintBigText(128, 151, 0.6f, "HLE MODE" );
			}
			if (SaveGame.RenderSettings.Platform == 0)
			{
				PrintBigText(115, 169, 0.6f, "N64 CONSOLE" );
			}
			else
			{
				PrintBigText(128, 169, 0.6f, "EMULATOR" );
			}
				
			
			if (TitleSwitch == 0)
			{
				GlobalIntA = 160;
			}
			else
			{
				GlobalIntA = 178;
			}
			KWSprite(235,GlobalIntA,16,16,(Ushort*)&lit_arrowsprite_r);
			KWSprite(95,GlobalIntA,16,16,(Ushort*)&lit_arrowsprite_l);

				
			
		
			#if OverKartBuild
     		KWTexture2DRGBA32PT(160,70,0,0.95f,(Uchar*)MenuIconsRAM,(void*)(&V256x12832B), 256, 128, 256, 4);
            #endif
			
			
			break;

		}
		case 11:
		{
			DrawGameSelect();
			break;
			if (!MenuToggle)
			{
				//Press R for Options Message
				GlobalAddressA = (Uint)(StartLogoRAM) + 8192;
				DrawBox(65,200,190,16,0,0,0,175);
				KWSpriteDiv(160,210,(Ushort*)(GlobalAddressA),256,16,8);
			}
			break;
		}
		case 12:
		{
			DrawPlayerSelect(SaveGame.GameSettings.StatsMode);
			break;
			if (!MenuToggle)
			{
				//Press R for Options Message
				GlobalAddressA = (Uint)(StartLogoRAM) + 8192;
				DrawBox(65,200,190,16,0,0,0,175);
				KWSpriteDiv(160,210,(Ushort*)(GlobalAddressA),256,16,8);
			}
			break;

		}
		case 13:
		{
			DrawMapSelect();
			break;

		}
	}
	FadeMain();
}



#include "MainInclude.h"
#include "OKInclude.h"
#include "data/ModelData/ModelData.h"

#if OverKartBuild
extern void DrawCheat3D(int CurrentPlayer);
#endif


void DrawPerScreen(Camera* LocalCamera)
{
	if (ScrollLock)
	{
		#if OverKartBuild
			int CurrentPlayer = LocalCamera - Cameras;
			DrawCheat3D(CurrentPlayer);
		#endif

		if(HotSwapID > 0)
		{
			DrawOKObjects(LocalCamera, SaveGame.GameSettings.FlycamMode);
		}


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
					DrawGameFlags(LocalCamera);
					DrawGameBase(LocalCamera);

					break;
				}
				case BTL_SOCCER:
				{
					DrawGameFlags(LocalCamera);
					break;
				}
			}
		}
		else
		{

		}

	}

}




void LoadCoin(void)
{

	*SourceAddress = (int)(&RCSpriteROM);
	*TargetAddress = (int)(&ok_FreeSpace);
	DataLength = 0x204;
	RunDMA();
	*SourceAddress = (int)(&ok_FreeSpace);
	*TargetAddress = (int)(&ok_RedCoinSprite);
	RunMIO();

}

void DropCoins(int PlayerIndex)
{
	int ThisCoin;

	IFrames[PlayerIndex] = 90;
	ObjectPosition[0] = GamePlayers[PlayerIndex].Position[0];
	ObjectPosition[1] = GamePlayers[PlayerIndex].Position[1];
	ObjectPosition[2] = GamePlayers[PlayerIndex].Position[2];

	for (ThisCoin = 0; ThisCoin < CoinCount[PlayerIndex]; ThisCoin++)
	{
		ObjectVelocity[0] = -2 + (MakeRandomLimmit(4));
		ObjectVelocity[1] = 4;
		ObjectVelocity[2] = -2 + (MakeRandomLimmit(4));
		MakeAlignVector(ObjectVelocity,(GamePlayers[PlayerIndex].Direction[1]));
		MasterCreateObject(ObjectPosition, ObjectAngle, ObjectVelocity, 48, 3.0); //motion coin
		ChangeMaxSpeed(PlayerIndex, -3);
	}
	CoinCount[PlayerIndex] = 0;
}



void CheckHit(int PlayerIndex, int HitType)
{
	if (SaveGame.GameSettings.GameType == 2)
	{
		DropCoins(PlayerIndex);
	}
	if (GameMode == GAMEMODE_BATTLE)
	{
		DropFlag(PlayerIndex);
	}
}




char BalancedItemSetA[] =
{
	ib_banana,
	ib_banana,
	ib_banana,
	ib_banana,
	ib_bananas,
	ib_kinoko1,
	ib_kinoko1,
	ib_kinoko2,
	ib_gkame,
	ib_gkame,
	ib_imbox,
	ib_imbox,
	ib_imbox,
	ib_imbox,
	ib_tellesa,
	ib_tellesa,
};

char BalancedItemSetB[] =
{
	ib_banana,
	ib_banana,
	ib_bananas,
	ib_bananas,
	ib_gskame,
	ib_gskame,
	ib_rkame,
	ib_rkame,
	ib_rskame,
	ib_kinoko2,
	ib_kinoko2,
	ib_kinoko3,
	ib_gkame,
	ib_imbox,
	ib_tellesa,
	ib_tellesa,
};

char BalancedItemSetC[] =
{
	ib_gskame,
	ib_gskame,
	ib_gskame,
	ib_rskame,
	ib_rskame,
	ib_rskame,
	ib_kinoko3,
	ib_kinoko3,
	ib_kinokoc,
	ib_kinokoc,
	ib_star,
	ib_thunder,
	ib_tkame,
	ib_tkame,
	ib_tkame,
	ib_tkame,
};


char BalancedItemSetD[] =
{
	ib_rskame,
	ib_rskame,
	ib_rskame,
	ib_rskame,
	ib_star,
	ib_star,
	ib_star,
	ib_star,
	ib_star,
	ib_star,
	ib_star,
	ib_star,
	ib_thunder,
	ib_thunder,
	ib_kinokoc,
	ib_kinokoc,
};




void ItemboxCollideCheck(Player* Car, Object* Target)
{
	int PlayerID = Car - GamePlayers;
	if (CollisionSphere(Car, Target))
	{
		Target->Sparam = 3;
		Target->Flag = EXISTOBJ;
		Target->Counter = 0;

		if (Car->Flag & IS_PLAYER)
		{
			if (Target->Bump.Dummy != 0)
			{
				RouletteStart(PlayerID, Target->Bump.Dummy);
			}
			else
			{
				switch (SaveGame.GameSettings.ItemMode)
				{
					case IM_DEFAULT:
					{
						RouletteStart(PlayerID, 0);
						break;
					}

					case IM_RANDOM:
					{
						GlobalShortA = MakeRandomLimmit(14);
						if (GlobalShortA < 14)
						{
							GlobalShortA += 1;
						}
						RouletteStart(PlayerID, GlobalShortA);
						break;
					}

					case IM_BALANCE:
					{
						switch (Car->Rank)
						{
							case 0:
							{
								GlobalShortA = 	BalancedItemSetA[MakeRandomLimmit(16)];
								break;
							}
							case 1:
							case 2:
							{
								GlobalShortA = 	BalancedItemSetB[MakeRandomLimmit(16)];
								break;
							}
							case 3:
							case 4:
							case 5:
							{
								GlobalShortA = 	BalancedItemSetC[MakeRandomLimmit(16)];
								break;
							}
							case 6:
							case 7:
							{
								GlobalShortA = 	BalancedItemSetD[MakeRandomLimmit(16)];
								break;
							}
						}
						RouletteStart(PlayerID, GlobalShortA);
						break;
					}

				}
			}

		}
	}
	else
	{
		if (Target->Sparam == 0)
		{
			Target->Sparam = 1;
			Target->Flag = EXISTOBJ;
		}
	}

}



void Draw3DRacer(Uint ModelAddress, Uint Player)
{

	if (SaveGame.RenderSettings.SplitMode == 0)
	{
		/* spriteKillA/B/C/D were SMC of InitRenderPlayer / TryRenderingPlayer. Batch 6. */
	}
	else
	{
		ObjectPosition[0] = GamePlayers[Player].Direction[0];
		ObjectPosition[1] =  GamePlayers[Player].Direction[1] +  GamePlayers[Player].Sterrangle;
		ObjectPosition[2] = GamePlayers[Player].Direction[2];
		ObjectAngle[2] = BaseTurn + AddTurn;
		ObjectAngle[0] = 0x3FFF - GamePlayers[0].Kartroll * 2;
		ObjectAngle[1] = GamePlayers[0].AngleZ[0] * -2;

		DrawGeometryScale(GamePlayers[Player].Position,ObjectAngle,ModelAddress, 0.08);
	}


}


void DisplayObject(void *Car, Object *InputObject)
{

	switch (InputObject->Category)
	{
		case (47):
		{

			GlobalAddressB = (long)RedCoin_GFX;
			ObjectPosition[0] = InputObject->Position[0];
			ObjectPosition[1] = InputObject->Position[1];
			ObjectPosition[2] = InputObject->Position[2];


			InputObject->Angle[1] += DEG1 * 3;
			ObjectAngle[0] = InputObject->Angle[0];
			ObjectAngle[1] = InputObject->Angle[1];
			ObjectAngle[2] = InputObject->Angle[2];


			DrawRedCoin(ObjectPosition,ObjectAngle,GlobalAddressB, 0.10f);
			break;
		}
		case 48:
		{
			GlobalAddressB = (long)GoldCoin_GFX;
			UpdateObjectGravity(InputObject);
			UpdateObjectVelocity(InputObject);

			UpdateObjectFrictionScale(InputObject,0.5);
			UpdateObjectBump(InputObject);

			if(InputObject->Bump.DistanceZX < 0)
			{
				InputObject->Velocity[1] = 0;
			}

			ObjectPosition[0] = InputObject->Position[0];
			ObjectPosition[1] = InputObject->Position[1] - 5.0f;
			ObjectPosition[2] = InputObject->Position[2];


			InputObject->Angle[1] += DEG1 * 3;
			ObjectAngle[0] = InputObject->Angle[0];
			ObjectAngle[1] = InputObject->Angle[1];
			ObjectAngle[2] = InputObject->Angle[2];


			DrawGeometryScale(ObjectPosition,ObjectAngle,GlobalAddressB, 0.125f);
			break;
		}
		case 49:
		{
			GlobalAddressB = (long)GoldCoin_GFX;
			ObjectPosition[0] = InputObject->Position[0];
			ObjectPosition[1] = InputObject->Position[1] - 5.0f;
			ObjectPosition[2] = InputObject->Position[2];


			InputObject->Angle[1] += DEG1 * 3;
			ObjectAngle[0] = InputObject->Angle[0];
			ObjectAngle[1] = InputObject->Angle[1];
			ObjectAngle[2] = InputObject->Angle[2];


			DrawGeometryScale(ObjectPosition,ObjectAngle,GlobalAddressB, 0.125f);
			break;
		}
	}

}

int RedCoinCollide(Player *Car, Object *Coin)
{
	int PlayerID = Car - GamePlayers;
	if (TestCollideSphere(Car->Position,Car->Radius,Coin->Position,6.0))
	{
		DeleteObjectBuffer(Coin);
		NAPlyTrgStart(PlayerID, 0x49008017);  //coin sfx
		CoinCount[PlayerID]++;
		if (CoinCount[PlayerID] != 8)
		{
			PlayrandmCharacterSFX(PlayerID);
		}
		SetAnimMusicNote(PlayerID);
	}
	if (CoinCount[PlayerID] == 8)
	{
		SetStar(Car,PlayerID);
		NAPlyTrgStart(PlayerID, 0x1900F103);  //Goal in sfx
		CoinCount[PlayerID]++;
	}

	return(0);
}


int GoldCoinCollide(Player *Car, Object *Coin)
{
	int PlayerID = Car - GamePlayers;
	if (IFrames[PlayerID] == 0)
	{
		if (CoinCount[PlayerID] < 10)
		{
			if (TestCollideSphere(Car->Position,Car->Radius,Coin->Position,7.0))
			{
				DeleteObjectBuffer(Coin);
				NAPlyTrgStart(PlayerID, 0x49008017);  //coin sfx
				CoinCount[PlayerID]++;
				PlayrandmCharacterSFX(PlayerID);

				SetAnimMusicNote(PlayerID);
				ChangeMaxSpeed(PlayerID, 3);
			}
		}
	}
	return(0);
}


void CollideObject(Player *Car, Object *Target)
{
	switch (Target->Category)
	{
		case 12:
		{
			ItemboxCollideCheck(Car,Target);
			break;
		}
		case 47:
		{
			RedCoinCollide(Car,Target);

			break;
		}
		case 48:
		case 49:
		{
			GoldCoinCollide(Car,Target);

			break;
		}
		default:
		{
			return;
			break;
		}
	}

}
void RedCoinChallenge(long CoinOffset)
{
	int currentCoin;

	GlobalShortD = 1;
	if (ScreenFlip == 1)
	{
		GlobalShortD = -1;
	}
	for (currentCoin = 0; currentCoin < 8; currentCoin++)
	{
		ObjectPosition[0] = (float)*(short*)(CoinOffset);
		ObjectPosition[1] = (float)*(short*)(CoinOffset + 2);
		ObjectPosition[2] = (float)*(short*)(CoinOffset + 4);

		ObjectPosition[0] = ObjectPosition[0] * GlobalShortD;
		CreateObject(ObjectPosition,47);
		CoinOffset += 8;
	}
}


void PlaceSIBox(long BoxOffset)
{
	int CurrentBox;
	Marker *BoxArray = (Marker*)(BoxOffset);

	GlobalShortD = 1;
	if (ScreenFlip == 1)
	{
		GlobalShortD = -1;
	}
	for (CurrentBox = 0; CurrentBox < 8; CurrentBox++)
	{
		if (BoxArray[CurrentBox].Position[0] == -32768)
		{
			return;
		}
		ObjectPosition[0] = (float)BoxArray[CurrentBox].Position[0];
		ObjectPosition[1] = (float)BoxArray[CurrentBox].Position[1];
		ObjectPosition[2] = (float)BoxArray[CurrentBox].Position[2];

		ObjectPosition[0] = ObjectPosition[0] * GlobalShortD;
		CreateObject(ObjectPosition,43);
	}
}


void GoldCoinChallenge(Uint PathOffset, int CoinCount)
{
	int currentCoin;
	Marker* Path = (Marker*)(PathOffset);

	GlobalShortD = 1;
	if (ScreenFlip == 1)
	{
		GlobalShortD = -1;
	}
	GlobalIntB = (g_pathLength / CoinCount);
	ObjectAngle[0] = 0;
	ObjectAngle[1] = 0;
	ObjectAngle[2] = 0;
	for (currentCoin = 0; currentCoin < CoinCount; currentCoin++)
	{
		ObjectPosition[0] = (float)Path[currentCoin * GlobalIntB].Position[0];
		ObjectPosition[1] = (float)Path[currentCoin * GlobalIntB].Position[1] + 5;
		ObjectPosition[2] = (float)Path[currentCoin * GlobalIntB].Position[2];
		MasterCreateObject(ObjectPosition, ObjectAngle, ObjectVelocity, 49, 2.0);  //static coin
	}
}

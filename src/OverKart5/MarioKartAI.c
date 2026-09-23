#include "MainInclude.h"
#include "OKInclude.h"

/*
char GlobalCharA; CPU STY
char GlobalCharB; FORCE CPU
char MenuOverflow; START BOOST
*/


void FastAI()
{
     switch(RaceClass)
     {
          case 0x00:
          {
               GlobalFloatA = 498.0;
               break;
          }
          case 0x01:
          {
               GlobalFloatA = 518.0;
               break;
          }
          case 0x02:
          {
               GlobalFloatA = 530.0;
               break;
          }
     }

     if ((StartingIndicator < 0x03) && (GlobalCharC == 0x00))
     {
          GlobalCharC = 0x01;
     }
     if ((StartingIndicator == 0x03) && (GlobalCharC == 0x01))
     {
          GlobalCharC = 0x00;
          switch(PlayerCount)
          {
               case 0x01 :
               {
                    GlobalPlayer[1].Weapon |= USE_MUSHROOM;
                    GlobalPlayer[2].Weapon |= USE_MUSHROOM;
                    GlobalPlayer[3].Weapon |= USE_MUSHROOM;
                    break;
               }
               case 0x02 :
               {
                    GlobalPlayer[1].Weapon |= USE_MUSHROOM;
                    break;
               }
               case 0x03 :
               {
                    GlobalPlayer[1].Weapon |= USE_MUSHROOM;
                    GlobalPlayer[2].Weapon |= USE_MUSHROOM;
                    break;
               }
               case 0x04 :
               {
                    GlobalPlayer[1].Weapon |= USE_MUSHROOM;
                    GlobalPlayer[2].Weapon |= USE_MUSHROOM;
                    GlobalPlayer[3].Weapon |= USE_MUSHROOM;
                    break;
               }
          }
     }
     if (GameTimer > 5.0)
     {
          switch(PlayerCount)
          {
               case 0x01 :
               {
                    GlobalPlayer[1].AccMaxcount = GlobalFloatA;
                    GlobalPlayer[2].AccMaxcount = GlobalFloatA;
                    GlobalPlayer[3].AccMaxcount = GlobalFloatA;

                    break;
               }
               case 0x02 :
               {
                    GlobalPlayer[1].AccMaxcount = GlobalFloatA;

                    break;
               }
               case 0x03 :
               {
                    GlobalPlayer[1].AccMaxcount = GlobalFloatA;
                    GlobalPlayer[2].AccMaxcount = GlobalFloatA;

                    break;
               }
               case 0x04 :
               {
                    GlobalPlayer[1].AccMaxcount = GlobalFloatA;
                    GlobalPlayer[2].AccMaxcount = GlobalFloatA;
                    GlobalPlayer[3].AccMaxcount = GlobalFloatA;

                    break;
               }
          }

     }
}


short RubberCheck[4] = {20, 30, 40, 35};
short SpeedMin[4] = {25, 35, 45, 40};

char RubberChar[8];
void RubberBandMan(int EnemyID, Player* EnemyKart)
{
     if (ScrollLock)
     {
          short ThisIndex = (short)g_playerPathPointTable[EnemyID];
          short TargetIndex = (short)g_playerPathPointTable[EnemyTargetPlayer];

          if (EnemyKart->Speed < SPEED_KM(SpeedMin[RaceClass]))
          {
               AccelOn(EnemyKart);
               RubberChar[EnemyID] = 0;
               return;
          }

          if (g_GameLapTable[EnemyID] < g_GameLapTable[EnemyTargetPlayer])
          {
               AccelOn(EnemyKart);
               RubberChar[EnemyID] = 1;
               return;
          }

          if (g_GameLapTable[EnemyID] == g_GameLapTable[EnemyTargetPlayer])
          {
               if (ThisIndex < TargetIndex + RubberCheck[RaceClass])
               {
                    AccelOn(EnemyKart);
                    RubberChar[EnemyID] = 2;
                    return;
               }
               else
               {
                    AccelOff(EnemyKart, 1);
                    RubberChar[EnemyID] = 3;
                    return;
               }
          }

          if (g_GameLapTable[EnemyID] > g_GameLapTable[EnemyTargetPlayer])
          {
               if (ThisIndex < RubberCheck[RaceClass])
               {
                    AccelOn(EnemyKart);
                    RubberChar[EnemyID] = 4;
                    return;
               }
               else
               {
                    AccelOff(EnemyKart, 1);
                    RubberChar[EnemyID] = 5;
                    return;
               }
          }
     }
}

short DifficultyCurve[4] = {0,5,10,8};
void SetAIDifficulty(int Difficulty)
{
     int ThisPlayer;
     for (ThisPlayer = PlayerCount; ThisPlayer < 8; ThisPlayer++)
     {
          GlobalPlayer[ThisPlayer].AccMaxcount += DifficultyCurve[Difficulty];
     }
}


void AISetup()
{
    int ThisPlayer;
    switch (SaveGame.GameSettings.AIMode)
	{
        case 0:
        {
            return;
            break;
        }
        case 1:
        {
            PlayerOK[1] = 0x01;
            PlayerOK[2] = 0x01;
            PlayerOK[3] = 0x01;
            break;
        }
        case 2:
        {
            PlayerOK[2] = 0x01;
            PlayerOK[3] = 0x01;
            break;
        }
        case 3:
        {
            PlayerOK[3] = 0x01;
            break;
        }

	}
    if ((StartingIndicator >= 3) && (StartingIndicator < 7))
    {
        for (ThisPlayer = SaveGame.GameSettings.AIMode; ThisPlayer < 4; ThisPlayer++)
        {
            if (*GlobalLap[ThisPlayer] < 3)
            {
                GlobalPlayer[ThisPlayer].Flag |= EXISTS;
                GlobalPlayer[ThisPlayer].Flag |= IS_CPU_PLAYER;
            }
        }




        EnemyTargetPlayer = 0;



    }

}

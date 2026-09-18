#include "MainInclude.h"
#include "OKInclude.h"

/*
char GlobalCharA; CPU STY
char GlobalCharB; FORCE CPU
char MenuOverflow; START BOOST
*/


void fastAI()
{
     switch(g_raceClass)
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

     if ((g_startingIndicator < 0x03) && (GlobalCharC == 0x00))
     {
          GlobalCharC = 0x01;
     }
     if ((g_startingIndicator == 0x03) && (GlobalCharC == 0x01))
     {
          GlobalCharC = 0x00;
          switch(g_playerCount)
          {
               case 0x01 :
               {
                    GlobalPlayer[1].weapon |= USE_MUSHROOM;
                    GlobalPlayer[2].weapon |= USE_MUSHROOM;
                    GlobalPlayer[3].weapon |= USE_MUSHROOM;
                    break;
               }
               case 0x02 :
               {
                    GlobalPlayer[1].weapon |= USE_MUSHROOM;
                    break;
               }
               case 0x03 :
               {
                    GlobalPlayer[1].weapon |= USE_MUSHROOM;
                    GlobalPlayer[2].weapon |= USE_MUSHROOM;
                    break;
               }
               case 0x04 :
               {
                    GlobalPlayer[1].weapon |= USE_MUSHROOM;
                    GlobalPlayer[2].weapon |= USE_MUSHROOM;
                    GlobalPlayer[3].weapon |= USE_MUSHROOM;
                    break;
               }
          }
     }
     if (g_gameTimer > 5.0)
     {
          switch(g_playerCount)
          {
               case 0x01 :
               {
                    GlobalPlayer[1].acc_maxcount = GlobalFloatA;
                    GlobalPlayer[2].acc_maxcount = GlobalFloatA;
                    GlobalPlayer[3].acc_maxcount = GlobalFloatA;

                    break;
               }
               case 0x02 :
               {
                    GlobalPlayer[1].acc_maxcount = GlobalFloatA;

                    break;
               }
               case 0x03 :
               {
                    GlobalPlayer[1].acc_maxcount = GlobalFloatA;
                    GlobalPlayer[2].acc_maxcount = GlobalFloatA;

                    break;
               }
               case 0x04 :
               {
                    GlobalPlayer[1].acc_maxcount = GlobalFloatA;
                    GlobalPlayer[2].acc_maxcount = GlobalFloatA;
                    GlobalPlayer[3].acc_maxcount = GlobalFloatA;

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
     if (scrollLock)
     {
          short ThisIndex = (short)g_playerPathPointTable[EnemyID];
          short TargetIndex = (short)g_playerPathPointTable[g_EnemyTargetPlayer];

          if (EnemyKart->speed < SPEED_KM(SpeedMin[g_raceClass]))
          {
               AccelOn(EnemyKart);
               RubberChar[EnemyID] = 0;
               return;
          }

          if (g_GameLapTable[EnemyID] < g_GameLapTable[g_EnemyTargetPlayer])
          {
               AccelOn(EnemyKart);
               RubberChar[EnemyID] = 1;
               return;
          }

          if (g_GameLapTable[EnemyID] == g_GameLapTable[g_EnemyTargetPlayer])
          {
               if (ThisIndex < TargetIndex + RubberCheck[g_raceClass])
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

          if (g_GameLapTable[EnemyID] > g_GameLapTable[g_EnemyTargetPlayer])
          {
               if (ThisIndex < RubberCheck[g_raceClass])
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
     for (ThisPlayer = g_playerCount; ThisPlayer < 8; ThisPlayer++)
     {
          GlobalPlayer[ThisPlayer].acc_maxcount += DifficultyCurve[Difficulty];
     }
}


void aiSetup()
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
    if ((g_startingIndicator >= 3) && (g_startingIndicator < 7))
    {
        for (ThisPlayer = SaveGame.GameSettings.AIMode; ThisPlayer < 4; ThisPlayer++)
        {
            if (*GlobalLap[ThisPlayer] < 3)
            {
                GlobalPlayer[ThisPlayer].flag |= EXISTS;
                GlobalPlayer[ThisPlayer].flag |= IS_CPU_PLAYER;
            }
        }




        g_EnemyTargetPlayer = 0;



    }

}

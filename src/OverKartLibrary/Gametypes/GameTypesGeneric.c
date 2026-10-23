#include "../MainInclude.h"



int ObjectiveMapID = 0;



void DisplayGameScore(void)
{
     if (TeamMode)
     {
          PrintBigTextNumber(142, 90, 0.5f, "", TeamScore[0]);
          PrintBigTextNumber(142, 130, 0.5f, "", TeamScore[1]);
          return;
     }

     switch(PlayerCount)
     {
          case 2:
          {
               PrintBigTextNumber(142, 90, 0.5f, "", Objectives[0].Score);
               PrintBigTextNumber(142, 130, 0.5f, "", Objectives[1].Score);
               break;
          }
          case 3:
          {
               PrintBigTextNumber(70, 90, 0.4f, "", Objectives[0].Score);
               PrintBigTextNumber(230, 90, 0.4f, "", Objectives[1].Score);
               PrintBigTextNumber(70, 130, 0.4f, "", Objectives[2].Score);
               break;
          }
          case 4:
          {
               PrintBigTextNumber(70, 90, 0.4f, "", Objectives[0].Score);
               PrintBigTextNumber(230, 90, 0.4f, "", Objectives[1].Score);
               PrintBigTextNumber(70, 130, 0.4f, "", Objectives[2].Score);
               PrintBigTextNumber(230, 130, 0.4f, "", Objectives[3].Score);
               break;
          }
     }
}

void BalloonCheck(Player* Player, f32 arg1, f32 Arg2, s8 playerIndex, s8 balloonIndex, s16 Rotation)
{
     if (BattleGametype == BTL_CTF)
     {
          return;
     }
     if (BattleGametype == BTL_SOCCER)
     {
          return;
     }

     BalloonDisp(Player, arg1, Arg2, playerIndex, balloonIndex, Rotation);
}

void ResetFlag(int ThisFlag)
{
     if (GameFlag[ThisFlag].PlayerHolding != -1)
     {
          Objectives[GameFlag[ThisFlag].PlayerHolding].FlagHeld = -1;
          Objectives[GameFlag[ThisFlag].PlayerHolding].FlagTimer = 0;
          ChangeMaxSpeed(GameFlag[ThisFlag].PlayerHolding, 60);
     }
     


     GameFlag[ThisFlag].Position[0] = (float)GameFlag[ThisFlag].Origin[0];
     GameFlag[ThisFlag].Position[1] = (float)GameFlag[ThisFlag].Origin[1];
     GameFlag[ThisFlag].Position[2] = (float)GameFlag[ThisFlag].Origin[2];

     GameFlag[ThisFlag].Velocity[0] = 0;
     GameFlag[ThisFlag].Velocity[1] = 0;
     GameFlag[ThisFlag].Velocity[2] = 0;

     GameFlag[ThisFlag].PlayerHolding = -1;
     GameFlag[ThisFlag].IFrames = 0;
     GameFlag[ThisFlag].RespawnTimer = 0;
     GameFlag[ThisFlag].Angle[0] = 0;
     GameFlag[ThisFlag].Angle[1] = 0;
     GameFlag[ThisFlag].Angle[2] = 0;

     CheckBump2(
          (Bump*)&GameFlag[ThisFlag].BumpData, 
          4.0, 
          GameFlag[ThisFlag].Position[0], 
          GameFlag[ThisFlag].Position[1], 
          GameFlag[ThisFlag].Position[2], 
          GameFlag[ThisFlag].Position[0] - GameFlag[ThisFlag].Velocity[0], 
          GameFlag[ThisFlag].Position[1] - GameFlag[ThisFlag].Velocity[1], 
          GameFlag[ThisFlag].Position[2] - GameFlag[ThisFlag].Velocity[2] 
     );
}


void PlacePlayerSpawn(const SVector Position, int PlayerID)
{
    
    SpawnPoint[PlayerID][0] = (float)Position[0];
    SpawnPoint[PlayerID][1] = (float)Position[1] + 5.0f;
    SpawnPoint[PlayerID][2] = (float)Position[2];
    
    GamePlayers[PlayerID].Position[0] = (float)Position[0];
    GamePlayers[PlayerID].Position[1] = (float)Position[1] + 5.0f;
    GamePlayers[PlayerID].Position[2] = (float)Position[2];

    GamePlayers[PlayerID].OldPosition[0] = (float)Position[0];
    GamePlayers[PlayerID].OldPosition[1] = (float)Position[1] + 5.0f;
    GamePlayers[PlayerID].OldPosition[2] = (float)Position[2];

    GamePlayers[PlayerID].Ground = (float)Position[1];

    GlobalCamera[PlayerID]->CameraPos[0] = (float)Position[0];
    GlobalCamera[PlayerID]->CameraPos[1] = (float)Position[1] + 5.0f;
    GlobalCamera[PlayerID]->CameraPos[2] = (float)Position[2];

    GlobalCamera[PlayerID]->LookatPos[0] = 0;
    GlobalCamera[PlayerID]->LookatPos[1] = 0;
    GlobalCamera[PlayerID]->LookatPos[2] = 0;

    

     GlobalCamera[PlayerID]->LookatPos[0] = 0;
     GlobalCamera[PlayerID]->LookatPos[1] = 0;
     GlobalCamera[PlayerID]->LookatPos[2] = 0;

     


    GamePlayers[PlayerID].Direction[1] = (short)(CalcDirection(GamePlayers[PlayerID].Position, Origin) * -1);
    GlobalCamera[PlayerID]->CameraDirection[1] = GamePlayers[PlayerID].Direction[1];
    GlobalCamera[PlayerID]->ChaseDirection = GamePlayers[PlayerID].Direction[1];
    CheckBump2
    (    
        (Bump*)&GamePlayers[PlayerID].Bump, 
        5.0, 
        GamePlayers[PlayerID].Position[0], GamePlayers[PlayerID].Position[1], GamePlayers[PlayerID].Position[2], 
        GamePlayers[PlayerID].OldPosition[0], GamePlayers[PlayerID].OldPosition[1], GamePlayers[PlayerID].OldPosition[2] 
    );
    

    ManualBump((Bump*)&GamePlayers[PlayerID].Bump, GamePlayers[PlayerID].Position);     
}

void PlaceFlagSpawn(const SVector Position, int FlagID)
{
     GameFlag[FlagID].Origin[0] = Position[0];
     GameFlag[FlagID].Origin[1] = Position[1];
     GameFlag[FlagID].Origin[2] = Position[2];
     ResetFlag(FlagID);
}
void PlaceBaseSpawn(const SVector Position, int BaseID)
{
     GameBase[BaseID].Position[0] = (float)Position[0];
     GameBase[BaseID].Position[1] = (float)Position[1];
     GameBase[BaseID].Position[2] = (float)Position[2];     
     GameBase[BaseID].Origin[0] = Position[0];
     GameBase[BaseID].Origin[1] = Position[1];
     GameBase[BaseID].Origin[2] = Position[2];     
     GameBase[BaseID].PlayerHolding = BaseID;
}


void DrawGameBase(Camera* LocalCamera)
{    
     //Draw GameFlags
     ObjectAngle[0] = 0;    
     ObjectAngle[1] = 0; 
     ObjectAngle[2] = 0;
     {
         int ThisFlag;
     for (ThisFlag = 0; ThisFlag < FlagCount; ThisFlag++)
     {    
          Uint *GArray = (Uint*)(GameBase[ThisFlag].F3D);
          gSPDisplayList(DisplayListHead++, GArray[0]);
          DrawGeometryScale(GameBase[ThisFlag].Position, ObjectAngle, GArray[1], 0.35);
     }
     }
}

void DrawGameFlags(Camera* LocalCamera)
{
     FaceStruct *Surface = (FaceStruct*)(FaceBuffer);
     //Draw GameFlags
     {
         int ThisFlag;
     for (ThisFlag = 0; ThisFlag < FlagCount; ThisFlag++)
     {
          Uint *GArray;

          GlobalIntA = (int)GameFlag[ThisFlag].PlayerHolding;
          GameFlag[ThisFlag].Angle[0] += (DEG1 * GameFlag[ThisFlag].AngularVel[0]);
          GameFlag[ThisFlag].Angle[1] += (DEG1 * GameFlag[ThisFlag].AngularVel[1]);
          GameFlag[ThisFlag].Angle[2] += (DEG1 * GameFlag[ThisFlag].AngularVel[2]);
          if (GlobalIntA != -1)
          {
               GameFlag[ThisFlag].Position[0] = GamePlayers[GlobalIntA].Position[0];
               GameFlag[ThisFlag].Position[1] = GamePlayers[GlobalIntA].Position[1] + 5;
               GameFlag[ThisFlag].Position[2] = GamePlayers[GlobalIntA].Position[2];              
               GameFlag[ThisFlag].Velocity[0] = 0;
               GameFlag[ThisFlag].Velocity[1] = 0;
               GameFlag[ThisFlag].Velocity[2] = 0;
          }
          else
          {
               GameFlag[ThisFlag].Position[0] += GameFlag[ThisFlag].Velocity[0];
               GameFlag[ThisFlag].Position[1] += GameFlag[ThisFlag].Velocity[1];
               GameFlag[ThisFlag].Position[2] += GameFlag[ThisFlag].Velocity[2];

               
               GlobalFloatA = 0;
               {
                   int ThisVector;
               for (ThisVector = 0; ThisVector < 3; ThisVector++)
               {
                    GlobalFloatA += GameFlag[ThisFlag].Velocity[ThisVector] * GameFlag[ThisFlag].Velocity[ThisVector];
               }
               }

               if (GlobalFloatA > 0.0f)
               {
                    CheckBump2((Bump*)&GameFlag[ThisFlag].BumpData, 4.0, GameFlag[ThisFlag].Position[0], GameFlag[ThisFlag].Position[1], GameFlag[ThisFlag].Position[2], GameFlag[ThisFlag].Position[0] - GameFlag[ThisFlag].Velocity[0], GameFlag[ThisFlag].Position[1] - GameFlag[ThisFlag].Velocity[1], GameFlag[ThisFlag].Position[2] - GameFlag[ThisFlag].Velocity[2] );
                    ManualBump((Bump*)&GameFlag[ThisFlag].BumpData, GameFlag[ThisFlag].Position);     
                    
                    if (GameFlag[ThisFlag].BumpData.DistanceXY < 0)               
                    {
                         ManualBounce(GameFlag[ThisFlag].BumpData.BumpXY, GameFlag[ThisFlag].Velocity);                    
                    }
                    if (GameFlag[ThisFlag].BumpData.DistanceYZ < 0)
                    {
                         ManualBounce(GameFlag[ThisFlag].BumpData.BumpYZ, GameFlag[ThisFlag].Velocity);
                    }
                    if(GameFlag[ThisFlag].BumpData.DistanceZX < 0)
                    {
                         GameFlag[ThisFlag].Velocity[0] *= (1.0 -  (float)((float)GameFlag[ThisFlag].Friction / 30000.0));
                         if (GameFlag[ThisFlag].Velocity[1] < ((float)GameFlag[ThisFlag].Gravity / -250.0))
                         {
                              GameFlag[ThisFlag].Velocity[1] *= (-1.0 + (float)((float)GameFlag[ThisFlag].Bounce / 1000.0));
                         }
                         else
                         {
                              GameFlag[ThisFlag].Velocity[1] = 0;
                         }
                         
                         GameFlag[ThisFlag].Velocity[2] *= (1.0 -  (float)((float)GameFlag[ThisFlag].Friction / 30000.0));

                       
                    }
                    else
                    {
                         GameFlag[ThisFlag].Velocity[1] -= (float)((float)GameFlag[ThisFlag].Gravity / 1000.0);
                    }
               }
               
          }
          

          if (GameFlag[ThisFlag].Position[1] < WaterHeight)
          {
               ResetFlag(ThisFlag);               
          }
            
          
          
          if ((int)(Surface[GameFlag[ThisFlag].BumpData.LastZX].Status & 0xFF) == JUMPMARK) //LavaFloor
          {
               ResetFlag(ThisFlag);
          }
          

          GArray = (Uint*)(GameFlag[ThisFlag].F3D);
          gSPDisplayList(DisplayListHead++, GArray[0]);
          DrawGeometryScale(GameFlag[ThisFlag].Position, GameFlag[ThisFlag].Angle, GArray[1], ((float)(GameFlag[ThisFlag].Scale / 1000.0f)));
     }
     }
}


void DropFlag(int PlayerIndex)
{
     if (Objectives[PlayerIndex].FlagHeld != -1)
     {
          GlobalIntA = (int)Objectives[PlayerIndex].FlagHeld;
          GameFlag[GlobalIntA].Position[0] = GamePlayers[PlayerIndex].Position[0];
          GameFlag[GlobalIntA].Position[1] = GamePlayers[PlayerIndex].Position[1] + 5;
          GameFlag[GlobalIntA].Position[2] = GamePlayers[PlayerIndex].Position[2];

          
          GameFlag[GlobalIntA].Velocity[0] = -2 + (MakeRandomLimmit(4));
          GameFlag[GlobalIntA].Velocity[1] = (float)(GameFlag[GlobalIntA].Lift) / 1000.0;
          GameFlag[GlobalIntA].Velocity[2] = -3 + (MakeRandomLimmit(6));

          MakeAlignVector(ObjectVelocity,(GamePlayers[PlayerIndex].Direction[1]));
          
          ChangeMaxSpeed((char)PlayerIndex, 60);

          GameFlag[(int)Objectives[PlayerIndex].FlagHeld].PlayerHolding = -1;
          GameFlag[(int)Objectives[PlayerIndex].FlagHeld].IFrames = 30;
          GameFlag[(int)Objectives[PlayerIndex].FlagHeld].RespawnTimer = 120;
          Objectives[PlayerIndex].FlagHeld= -1;
          Objectives[PlayerIndex].IFrames = 100;
     }       
	
}

bool CheckExecuteItem(int PlayerIndex)
{    
    if (Objectives[PlayerIndex].FlagHeld == -1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

#include "../MainInclude.h"


short ObjectSubBehaviorTurnTarget(float InputPosition[3], short InputAngle, float TargetPosition[3], short ToleranceAngle)
{
	GlobalShortA = (short)(MakeDirection(InputPosition[0],InputPosition[2], TargetPosition[0], TargetPosition[2]) * -1);
	GlobalShortA -= InputAngle;
	if (GlobalShortA > (DEG1 * ToleranceAngle))
	{
		return 1;
	}
	else if (GlobalShortA < -(DEG1 * ToleranceAngle))
	{
		return -1;
	}
	return 0;
}



float ObjectSubBehaviorLookTarget(OKObject* InputObject, float TargetPosition[3], short Sight, short Viewcone)
{	
	GlobalFloatA = (float)Sight;	
	GlobalFloatB = (InputObject->ObjectData.Position[0] - TargetPosition[0]);
	GlobalFloatC = (InputObject->ObjectData.Position[2] - TargetPosition[2]);
	GlobalFloatD = (((GlobalFloatB * GlobalFloatB) + (GlobalFloatC * GlobalFloatC)) / (GlobalFloatA * GlobalFloatA));

	GlobalShortA = (short)(CalcDirection(InputObject->ObjectData.Position, TargetPosition) * -1);
	GlobalShortA -= InputObject->ObjectData.Angle[1];
	
	
	if ((GlobalShortA < (DEG1 * (Viewcone/2))) && (GlobalShortA > (DEG1 * (Viewcone/-2))))
	{	
		return GlobalFloatD;
		
	}
	
	return 35000;
	
	
}

float ObjectSubBehaviorLookedAt(OKObject* InputObject, int PlayerIndex, short LookAngle, short Range)
{
	Player ThisPlayer = GamePlayers[PlayerIndex];
	GlobalFloatA = (float)Range;	
	GlobalFloatB = (ThisPlayer.Position[0] - InputObject->ObjectData.Position[0]);
	GlobalFloatC = (ThisPlayer.Position[2] - InputObject->ObjectData.Position[2]);
	GlobalFloatD = (((GlobalFloatB * GlobalFloatB) + (GlobalFloatC * GlobalFloatC)) / (GlobalFloatA * GlobalFloatA));

	GlobalShortA = (short)(CalcDirection(ThisPlayer.Position, InputObject->ObjectData.Position) * -1);
	GlobalShortA -= ThisPlayer.Direction[1];
	
	
	if ((GlobalShortA < (DEG1 * (LookAngle/2))) && (GlobalShortA > (DEG1 * (LookAngle/-2))))
	{	
		return GlobalFloatD;
	}
	
	return 35000;
	
	
}


void ObjectBehaviorExist(OKObject* InputObject)
{
	OKObjectType *ThisType = (OKObjectType*)&(OverKartRAMHeader.ObjectTypeList[InputObject->TypeIndex]);
	if (ThisType->GravityToggle == 1)
	{
		UpdateObjectGravity((Object*)&InputObject->ObjectData);
	}

	UpdateObjectVelocity((Object*)&InputObject->ObjectData);	

	if (ThisType->BumpRadius != 0)
	{
		UpdateObjectBump((Object*)&InputObject->ObjectData);		
	
		if(InputObject->ObjectData.Bump.DistanceZX < 0)
		{
			InputObject->ObjectData.Velocity[1] = 0;
		}
	}
}

void ObjectSubBehaviorWalk(OKObject* InputObject, float Speed)
{
	ObjectVelocity[0] = 0;
	ObjectVelocity[1] = InputObject->ObjectData.Velocity[1];
	ObjectVelocity[2] = Speed;
	MakeAlignVector(ObjectVelocity, InputObject->ObjectData.Angle[1]);
	InputObject->ObjectData.Velocity[0] = ObjectVelocity[0];
	InputObject->ObjectData.Velocity[1] = ObjectVelocity[1];
	InputObject->ObjectData.Velocity[2] = ObjectVelocity[2];

	ObjectBehaviorExist(InputObject);	
}

void ObjectBehaviorStrafe(OKObject* InputObject)
{
	OKObjectType *ThisType = (OKObjectType*)&(OverKartRAMHeader.ObjectTypeList[InputObject->TypeIndex]);
	OKObjectList *ThisList = (OKObjectList*)&(OverKartRAMHeader.ObjectList[InputObject->ListIndex]);
    short* Parameters = (short*)GetRealAddress(ThisType->ObjectBehavior);
    /*
        <Parameter_0_Name>Range</Parameter_0_Name>
        <Parameter_1_Name>Speed</Parameter_1_Name>
    */
    short Range = Parameters[0];
    short Speed = Parameters[1];

	GlobalShortA = 1;
	if (ThisList->Flag != 0)
	{
		GlobalShortA = -1;
	}
	
	switch (InputObject->Status[1])
	{
		case 0:
		{
			InputObject->ObjectData.Velocity[0] = (float)(Speed * 0.01f) * GlobalShortA;
			InputObject->ObjectData.Velocity[1] = 0;
			InputObject->ObjectData.Velocity[2] = 0;

			InputObject->Counter[0] += InputObject->ObjectData.Velocity[0] * GlobalShortA;
			if (InputObject->Counter[0] > Range)
			{
				InputObject->Status[1] = 1;
			}
            InputObject->TargetDistance = InputObject->ObjectData.Velocity[0];
			break;
		}
		case 1:
		{
            InputObject->TargetDistance -= (float)(Speed * 0.0003f) * GlobalShortA;
			InputObject->ObjectData.Velocity[0] = InputObject->TargetDistance;
			InputObject->ObjectData.Velocity[1] = 0;
			InputObject->ObjectData.Velocity[2] = 0;

			
			if (InputObject->ObjectData.Velocity[0] <= (float)(Speed * -0.01f))
			{
				InputObject->Status[1] = 2;
			}
            break;
		}
		case 2:
		{
			InputObject->ObjectData.Velocity[0] = (float)(Speed * -0.01f) * GlobalShortA;
			InputObject->ObjectData.Velocity[1] = 0;
			InputObject->ObjectData.Velocity[2] = 0;

			InputObject->Counter[0] += InputObject->ObjectData.Velocity[0] * GlobalShortA;
			if (InputObject->Counter[0]<= 0)
			{
				InputObject->Status[1] = 3;
			}
			break;
		}
		case 3:
		{
			InputObject->ObjectData.Velocity[0] = (float)(Speed * -0.01f) * GlobalShortA;
			InputObject->ObjectData.Velocity[1] = 0;
			InputObject->ObjectData.Velocity[2] = 0;

			InputObject->Counter[0] += InputObject->ObjectData.Velocity[0] * GlobalShortA;
			if (InputObject->Counter[0] < (Range * -1))
			{
				InputObject->Status[1] = 4;
			}
            InputObject->TargetDistance = InputObject->ObjectData.Velocity[0];
			break;
		}
		case 4:
		{
			InputObject->TargetDistance += (float)(Speed * 0.0003f) * GlobalShortA;
			InputObject->ObjectData.Velocity[0] = InputObject->TargetDistance;
			InputObject->ObjectData.Velocity[1] = 0;
			InputObject->ObjectData.Velocity[2] = 0;

			
			if (InputObject->ObjectData.Velocity[0] >= (float)(Speed * 0.01f))
			{
				InputObject->Status[1] = 5;
			}
            break;
		}
		case 5:
		{	
			InputObject->ObjectData.Velocity[0] = (float)(Speed * 0.01f) * GlobalShortA;
			InputObject->ObjectData.Velocity[1] = 0;
			InputObject->ObjectData.Velocity[2] = 0;

			InputObject->Counter[0] += InputObject->ObjectData.Velocity[0] * GlobalShortA;
			if (InputObject->Counter[0]>= 0)
			{
				InputObject->Status[1] = 0;
			}
			break;
		}
	}
		
	MakeAlignVector(InputObject->ObjectData.Velocity, InputObject->ObjectData.Angle[1]);
	ObjectBehaviorExist(InputObject);
	
}

void ObjectSubBehaviorWander(OKObject* InputObject, short Range, short Speed)
{
    OKObjectList *ThisList = (OKObjectList*)&(OverKartRAMHeader.ObjectList[InputObject->ListIndex]);


	GlobalFloatA = (float)Range;
	GlobalFloatB = GlobalFloatA * 0.6;
	
	GlobalIntA = (InputObject->ObjectData.Position[0] - ThisList->OriginPosition[0]);
	GlobalIntB = (InputObject->ObjectData.Position[2] - ThisList->OriginPosition[2]);
	

	if ((GlobalIntA * GlobalIntA) + (GlobalIntB * GlobalIntB) < (GlobalFloatB * GlobalFloatB))
	{
		 InputObject->Status[1] = 0;
	}
	
	
	if (InputObject->Status[1] >= 1)
	{		
		ObjectPosition[0] = (float)ThisList->OriginPosition[0];
		ObjectPosition[1] = (float)ThisList->OriginPosition[1];
		ObjectPosition[2] = (float)ThisList->OriginPosition[2];

		InputObject->ObjectData.Angle[1] += (DEG1 * 2 * ObjectSubBehaviorTurnTarget(InputObject->ObjectData.Position, InputObject->ObjectData.Angle[1], ObjectPosition, 3));
	}
	else
	{
		if ((GlobalIntA * GlobalIntA) + (GlobalIntB * GlobalIntB) > (GlobalFloatA * GlobalFloatA))
		{
			InputObject->Status[1] = 1;
		}
		else
		{
			if (InputObject->Counter[0] > 0)
			{
				InputObject->Counter[0]--;
				
				switch (InputObject->Status[0])
				{
					case 0: 
					{
						break;				
					}
					case 1: 
					{
						InputObject->ObjectData.Angle[1] += DEG1 * 2;
						break;					
					}
					case 2: 
					{
						InputObject->ObjectData.Angle[1] -= DEG1 * 2;
						break;				
					}
					case 3: 
					{
						break;		
					}
				}
			}
			else
			{
				InputObject->Status[0] = MakeRandomLimmit(4);
				InputObject->Counter[0] = MakeRandomLimmit(60) + 15;
			}
		}

	}
    //Speed
	ObjectSubBehaviorWalk(InputObject, (Speed * 0.01f));
	
}




void ObjectBehaviorFlee(OKObject* InputObject)
{	
    OKObjectType *ThisType = (OKObjectType*)&(OverKartRAMHeader.ObjectTypeList[InputObject->TypeIndex]);    
    short* Parameters = (short*)GetRealAddress(ThisType->ObjectBehavior);
    /*
        
        <Parameter_0_Name>Range</Parameter_0_Name>
        <Parameter_1_Name>Speed</Parameter_1_Name>
        <Parameter_2_Name>Sight</Parameter_2_Name>
        <Parameter_3_Name>Viewcone</Parameter_3_Name>
    */
    short Range = Parameters[0];
    short Speed = Parameters[1];
    short Sight = Parameters[2];
    short Viewcone = Parameters[3];

	switch (InputObject->SubBehaviorClass)
	{
		case(SUBBEHAVIOR_DOCILE):
		{			
			InputObject->TargetDistance = 800;
			if (GameMode == 00)
			{
				GlobalIntA = 8;			
			}
			else
			{
				GlobalIntA = PlayerCount;
			}
			{
			    int CurrentPlayer;
			for (CurrentPlayer = 0; CurrentPlayer < GlobalIntA; CurrentPlayer++)
			{
				GlobalFloatD = ObjectSubBehaviorLookedAt(InputObject, CurrentPlayer, Viewcone, Range);
				if ((GlobalFloatD < 1) && (GlobalFloatD < InputObject->TargetDistance))
				{

					InputObject->SubBehaviorClass = SUBBEHAVIOR_SURPRISE;
					InputObject->TargetDistance = GlobalFloatD;
					GlobalFloatD = 2.01 - GlobalFloatD;
					InputObject->PlayerTarget = (short)CurrentPlayer;
					InputObject->Counter[1] = 15 * GlobalFloatD; 
					
					InputObject->ObjectData.Velocity[0] = 0;
					InputObject->ObjectData.Velocity[1] += 2 * GlobalFloatD;
					InputObject->ObjectData.Velocity[2] = 0;
				}
			}
			}
			if (InputObject->SubBehaviorClass == SUBBEHAVIOR_DOCILE)
			{
				ObjectSubBehaviorWander(InputObject, Range, Speed);
			}
			break;
		}
		case(SUBBEHAVIOR_SURPRISE):
		{
			
			
			if (InputObject->Counter[1] <= 0)
			{
				InputObject->SubBehaviorClass = SUBBEHAVIOR_CHASE;
			}
			else
			{
				InputObject->Counter[1]--;
			}
			
			ObjectBehaviorExist(InputObject);
			break;
		}
		case(SUBBEHAVIOR_CHASE):
		{
			
			GlobalFloatD = ObjectSubBehaviorLookTarget(InputObject, GamePlayers[InputObject->PlayerTarget].Position, Sight, Viewcone);
			if (GlobalFloatD < 2)
			{
				GlobalShortC = ObjectSubBehaviorTurnTarget(InputObject->ObjectData.Position, InputObject->ObjectData.Angle[1], GamePlayers[InputObject->PlayerTarget].Position, 4);
				InputObject->ObjectData.Angle[1] += (DEG1 * -4 * GlobalShortC);
				GlobalFloatA = (InputObject->ObjectData.Velocity[0] * InputObject->ObjectData.Velocity[0]) + (InputObject->ObjectData.Velocity[2] * InputObject->ObjectData.Velocity[2]);
                GlobalFloatB = (Speed * 0.01f);
				if (GlobalFloatA < (Speed * Speed))
				{
					if (GlobalFloatA == 0)
					{
						GlobalFloatA = 0.25f;
					}
					else
					{
						GlobalFloatA = sqrtf(GlobalFloatA) + 0.25f;
					}
				}
				else
				{
					GlobalFloatA = 2;
				}


				if (GlobalShortC != 0)
				{
					GlobalFloatB = GlobalFloatA / 1.25f;
					ObjectSubBehaviorWalk(InputObject,GlobalFloatB);
				}
				else
				{
					ObjectSubBehaviorWalk(InputObject,GlobalFloatA);
				}
				

			}
			else
			{
				InputObject->SubBehaviorClass = SUBBEHAVIOR_DOCILE;
				InputObject->Counter[1] = 0;
				ObjectBehaviorExist(InputObject);
			}
			
			break;
		}
	}
}

void ObjectBehaviorSearch(OKObject* InputObject)
{	
	OKObjectType *ThisType = (OKObjectType*)&(OverKartRAMHeader.ObjectTypeList[InputObject->TypeIndex]);
    short* Parameters = (short*)GetRealAddress(ThisType->ObjectBehavior);
    /*
        
        <Parameter_0_Name>Range</Parameter_0_Name>
        <Parameter_1_Name>Speed</Parameter_1_Name>
        <Parameter_2_Name>Sight</Parameter_2_Name>
        <Parameter_3_Name>Viewcone</Parameter_3_Name>

    */

    short Range = Parameters[0];
    short Speed = Parameters[1];
    short Sight = Parameters[2];
    short Viewcone = Parameters[3];

	switch (InputObject->SubBehaviorClass)
	{
		case(SUBBEHAVIOR_DOCILE):
		{			
			InputObject->TargetDistance = 800;
			if (GameMode == 00)
			{
				GlobalIntA = 8;			
			}
			else
			{
				GlobalIntA = PlayerCount;
			}
			{
			    int CurrentPlayer;
			for (CurrentPlayer = 0; CurrentPlayer < GlobalIntA; CurrentPlayer++)
			{
				GlobalFloatD = ObjectSubBehaviorLookTarget(InputObject, GamePlayers[CurrentPlayer].Position, Sight, Viewcone);					
				if ((GlobalFloatD < 1) && (GlobalFloatD < InputObject->TargetDistance))
				{

					InputObject->SubBehaviorClass = SUBBEHAVIOR_SURPRISE;
					InputObject->TargetDistance = GlobalFloatD;
					GlobalFloatD = 2.01 - GlobalFloatD;
					InputObject->PlayerTarget = (short)CurrentPlayer;
					InputObject->Counter[1] = 15 * GlobalFloatD; 
					
					InputObject->ObjectData.Velocity[0] = 0;
					InputObject->ObjectData.Velocity[1] += 2 * GlobalFloatD;
					InputObject->ObjectData.Velocity[2] = 0;
				}
			}
			}
			if (InputObject->SubBehaviorClass == SUBBEHAVIOR_DOCILE)
			{
				ObjectSubBehaviorWander(InputObject, Range, Speed);
			}
			break;
		}
		case(SUBBEHAVIOR_SURPRISE):
		{
			
			
			if (InputObject->Counter[1] <= 0)
			{
				InputObject->SubBehaviorClass = SUBBEHAVIOR_CHASE;
			}
			else
			{
				InputObject->Counter[1]--;
			}
			
			ObjectBehaviorExist(InputObject);
			break;
		}
		case(SUBBEHAVIOR_CHASE):
		{
			
			GlobalFloatD = ObjectSubBehaviorLookTarget(InputObject, GamePlayers[InputObject->PlayerTarget].Position, Sight, Viewcone);
			if (GlobalFloatD < 2)
			{
				GlobalShortC = ObjectSubBehaviorTurnTarget(InputObject->ObjectData.Position, InputObject->ObjectData.Angle[1], GamePlayers[InputObject->PlayerTarget].Position, 4);
				InputObject->ObjectData.Angle[1] += (DEG1 * 4 * GlobalShortC);
				GlobalFloatA = (InputObject->ObjectData.Velocity[0] * InputObject->ObjectData.Velocity[0]) + (InputObject->ObjectData.Velocity[2] * InputObject->ObjectData.Velocity[2]);
				GlobalFloatB = (Speed * 0.01f);

				//get Speed from max Speed and not just the Value 2.0 wtf was that
				if (GlobalFloatA < (GlobalFloatB * GlobalFloatB))
				{
					if (GlobalFloatA == 0)
					{
						GlobalFloatA = 0.25;
					}
					else
					{
						GlobalFloatA = sqrtf(GlobalFloatA) + 0.25;
					}
				}
				else
				{
					GlobalFloatA = GlobalFloatB;
				}


				if (GlobalShortC != 0)
				{
					ObjectSubBehaviorWalk(InputObject,GlobalFloatA * 0.75f);
					//slow on turns
				}
				else
				{
					ObjectSubBehaviorWalk(InputObject,GlobalFloatA);
				}
				

			}
			else
			{
				InputObject->SubBehaviorClass = SUBBEHAVIOR_DOCILE;
				InputObject->Counter[1] = 0;
				ObjectBehaviorExist(InputObject);
			}
			
			break;
		}
	}
}

short ObjectSearchClosestMarker(float ObjectPostion[], Marker* PathData)
{
	GlobalShortB = 0;
	GlobalFloatB = 35000;
	{
	    int CurrentMarker;
	for (CurrentMarker = 0; ; CurrentMarker++)
	{		
		if (PathData[CurrentMarker].Position[0] == (short)0x8000)
		{
			return GlobalShortB;
		}
		else
		{
			GlobalFloatA = (
				(ObjectPostion[0] - (float)PathData[CurrentMarker].Position[0]) * (ObjectPostion[0] - (float)PathData[CurrentMarker].Position[0]) +
				(ObjectPostion[2] - (float)PathData[CurrentMarker].Position[2]) * (ObjectPostion[2] - (float)PathData[CurrentMarker].Position[2])
			);
			

			if(GlobalFloatA < GlobalFloatB)
			{
				GlobalShortB = (short)CurrentMarker;
				GlobalFloatB = GlobalFloatA;
			}
		}
	}
	}
	
	
}


void ObjectBehaviorFollowPath(OKObject* InputObject)
{
	Uint* PathOffsets = (Uint*)&pathOffset; 
	OKObjectType *ThisType = (OKObjectType*)&(OverKartRAMHeader.ObjectTypeList[InputObject->TypeIndex]);

    short* Parameters = (short*)GetRealAddress(ThisType->ObjectBehavior);
    /*
        <Parameter_0_Name>Path Index</Parameter_0_Name>
        <Parameter_1_Name>Speed</Parameter_1_Name>
    */

	Marker* PathData = (Marker*)(GetRealAddress(PathOffsets[Parameters[0]]));
	short ThisPoint = InputObject->PathTarget;
	short NextPoint = ThisPoint + 1;
	Vector Origin;
	Vector Rear;
	Vector Front;

	
	if (InputObject->PathTarget == -1)
	{
		InputObject->PathTarget = 1 + ObjectSearchClosestMarker(InputObject->ObjectData.Position,PathData);

		if (PathData[InputObject->PathTarget].Position[0] == (short)0x8000)
		{
			InputObject->PathTarget  = 0;
		}

		ObjectPosition[0] = (float)PathData[InputObject->PathTarget].Position[0];
		ObjectPosition[1] = (float)PathData[InputObject->PathTarget].Position[1];
		ObjectPosition[2] = (float)PathData[InputObject->PathTarget].Position[2];
		InputObject->ObjectData.Angle[1] = (-1 * CalcDirection(InputObject->ObjectData.Position, ObjectPosition));

		if (InputObject->PathTarget > 0)
		{
			InputObject->PlayerTarget = InputObject->PathTarget - 1;
		}
		else
		{
			InputObject->PlayerTarget = 0;
		}

		
		ObjectPosition[0] = (float)PathData[InputObject->PlayerTarget].Position[0];
		ObjectPosition[1] = (float)PathData[InputObject->PlayerTarget].Position[1];
		ObjectPosition[2] = (float)PathData[InputObject->PlayerTarget].Position[2];

		InputObject->ObjectData.Position[0] = ObjectPosition[0];
		InputObject->ObjectData.Position[1] = ObjectPosition[1];
		InputObject->ObjectData.Position[2] = ObjectPosition[2];
	}
	else if (InputObject->PathTarget >= 0)
	{
		if (PathData[NextPoint].Position[0] == (short)0x8000)
		{
			NextPoint = 0;
		}

		ObjectPosition[0] = (float)PathData[NextPoint].Position[0];
		ObjectPosition[1] = (float)PathData[NextPoint].Position[1];
		ObjectPosition[2] = (float)PathData[NextPoint].Position[2];
		
		GlobalFloatA = (ObjectPosition[0] - InputObject->ObjectData.Position[0]) + (ObjectPosition[2] - InputObject->ObjectData.Position[2]);


		
		
		ObjectPosition[0] = (float)PathData[ThisPoint].Position[0];
		ObjectPosition[2] = (float)PathData[ThisPoint].Position[2];
		GlobalFloatB = (ObjectPosition[0] - InputObject->ObjectData.Position[0]) + (ObjectPosition[2] - InputObject->ObjectData.Position[2]);
		
		GlobalFloatA *= GlobalFloatA;
		GlobalFloatB *= GlobalFloatB;

		if (GlobalFloatA < GlobalFloatB)
		{
			InputObject->PathTarget = NextPoint + 1;
			InputObject->PlayerTarget = NextPoint; //use PlayerTarget for Last Point.

			Origin[0] = InputObject->ObjectData.Position[0];
			Origin[1] = InputObject->ObjectData.Position[1];
			Origin[2] = InputObject->ObjectData.Position[2];
			MakeAlignVector(Origin,InputObject->ObjectData.Angle[1]);

			Rear[0] = Origin[0];
			Rear[1] = Origin[1] + 10;
			Rear[2] = Origin[2] - 30;

			Front[0] = Origin[0];
			Front[1] = Origin[1] + 10;
			Front[2] = Origin[2] + 30;
			Rear[1] = CalcHeight(Rear[0], Rear[1], Rear[2], InputObject->ObjectData.Bump.LastZX);
			Front[1] = CalcHeight(Front[0], Front[1], Front[2], InputObject->ObjectData.Bump.LastZX);
			InputObject->ObjectData.Angle[0] = (DEG1 * -90) + CalcVerticalDirection(Rear, Front);
		}
		else
		{

			ChaseDir(&InputObject->ObjectData.Angle[1],(-1 * MakeDirection(InputObject->ObjectData.Position[0],InputObject->ObjectData.Position[2],ObjectPosition[0],ObjectPosition[2])), (DEG1 * 5));
			ObjectSubBehaviorWalk(InputObject, (float)(Parameters[1] * 0.01f));

			if (TestCollideSphere(InputObject->ObjectData.Position,25,ObjectPosition, 25))
			{
				InputObject->PathTarget = NextPoint;
				InputObject->PlayerTarget = ThisPoint; //use PlayerTarget for Last Point.

				Origin[0] = InputObject->ObjectData.Position[0];
				Origin[1] = InputObject->ObjectData.Position[1];
				Origin[2] = InputObject->ObjectData.Position[2];
				MakeAlignVector(Origin,InputObject->ObjectData.Angle[1]);

				Rear[0] = Origin[0];
				Rear[1] = Origin[1] + 10;
				Rear[2] = Origin[2] - 30;

				Front[0] = Origin[0];
				Front[1] = Origin[1] + 10;
				Front[2] = Origin[2] + 30;
				Rear[1] = CalcHeight(Rear[0], Rear[1], Rear[2], InputObject->ObjectData.Bump.LastZX);
				Front[1] = CalcHeight(Front[0], Front[1], Front[2], InputObject->ObjectData.Bump.LastZX);
				InputObject->ObjectData.Angle[0] = (DEG1 * -90) + CalcVerticalDirection(Rear, Front);
			}
		}

		
	}
	
}

void ObjectBehaviorBounce(OKObject* InputObject)
{
	UpdateActorFallingRocks((struct FallingRock *) ((Object*)&InputObject->ObjectData));
}



void Misbehave(OKObject* InputObject)
{
    OKObjectType *ThisType = (OKObjectType*)&(OverKartRAMHeader.ObjectTypeList[InputObject->TypeIndex]);
    short* Parameters = (short*)GetRealAddress(ThisType->ObjectBehavior);


	switch (OverKartRAMHeader.ObjectTypeList[InputObject->TypeIndex].BehaviorClass)
	{
		case BEHAVIOR_STATIC:
		{
			Vector FAngle;
			SVector SAngle;
			FAngle[0] = (float)InputObject->AngularVelocity[0];
			FAngle[1] = (float)InputObject->AngularVelocity[1];
			FAngle[2] = (float)InputObject->AngularVelocity[2];
			MakeAlignVector(FAngle, OverKartRAMHeader.ObjectList[InputObject->ListIndex].OriginAngle[1]);
			SAngle[0] = (short)FAngle[0];
			SAngle[1] = (short)FAngle[1];
			SAngle[2] = (short)FAngle[2];
			UpdateObjectAngle((Object*)(&InputObject->ObjectData), SAngle);
			ObjectBehaviorExist(InputObject);
			break;
		}
		case BEHAVIOR_PATH:
		{	
			ObjectBehaviorFollowPath(InputObject);
		}
		case BEHAVIOR_WANDER:
		{
			ObjectSubBehaviorWander(InputObject, Parameters[0], Parameters[1]);
			break;
		}
		case BEHAVIOR_SEARCH:
		{
			
			ObjectBehaviorSearch(InputObject);
			break;
		}
		case BEHAVIOR_BOUNCE:
		{
			
			ObjectBehaviorBounce(InputObject);
			break;
		}
        
		case BEHAVIOR_FLEE:
		{
			
			ObjectBehaviorFlee(InputObject);
			break;
		}
		case BEHAVIOR_STRAFE:
		{

			ObjectBehaviorStrafe(InputObject);
			break;
		}
	}

	if (InputObject->ObjectData.Position[1] < WaterHeight)
	{
		InputObject->SubBehaviorClass = SUBBEHAVIOR_DEAD;
	}
	
}

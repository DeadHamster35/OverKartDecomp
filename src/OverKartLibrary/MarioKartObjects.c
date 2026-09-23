#include "MainInclude.h"


unsigned short CheckBump2Simple(Object *InputObject)
{
	return CheckBump2((Bump*)&InputObject->Bump, InputObject->Radius, InputObject->Position[0], InputObject->Position[1], InputObject->Position[2], InputObject->Position[0] - InputObject->Velocity[0], InputObject->Position[1] - InputObject->Velocity[1], InputObject->Position[2] - InputObject->Velocity[2] );
}


unsigned short CheckBump2Simple2(Uint BumpAddress, float Radius, Vector InputPosition)
{
	return CheckBump2((Bump*)BumpAddress, Radius, InputPosition[0], InputPosition[1], InputPosition[2],  InputPosition[0], InputPosition[1], InputPosition[2] );
}
short MasterCreateObject(float localPosition[], short localRotation[], float localVelocity[], short localID, float Radius)
{
	GlobalShortA = AddObjectBuffer(localPosition,localRotation,ObjectVelocity,localID);
	
	GlobalAddressA = (long)(&SimpleObjectArray) + (GlobalShortA * 0x70);
	SimpleObjectArray[GlobalShortA].Flag = 0xC000;
	SimpleObjectArray[GlobalShortA].Radius = Radius;
	return GlobalShortA;
}




short CreateObjectRotation(float localPosition[], short localRotation[], short localID)
{
	ObjectVelocity[0] = 0;
	ObjectVelocity[1] = 0;
	ObjectVelocity[2] = 0;

	GlobalShortA = MasterCreateObject(localPosition,localRotation,ObjectVelocity,localID, 5.0);
	
	return GlobalShortA;
}

short CreateObjectRadius(float localPosition[], short localRotation[], short localID, float localRadius)
{
	ObjectVelocity[0] = 0;
	ObjectVelocity[1] = 0;
	ObjectVelocity[2] = 0;
	GlobalShortA = MasterCreateObject(localPosition,localRotation,ObjectVelocity,localID, localRadius);
	
	return GlobalShortA;
}

short CreateObject(float localPosition[], short localID)
{
	ObjectAngle[0] = 0;
	ObjectAngle[1] = 0;
	ObjectAngle[2] = 0;

	ObjectVelocity[0] = 0;
	ObjectVelocity[1] = 0;
	ObjectVelocity[2] = 0;

	return MasterCreateObject(localPosition,ObjectAngle,ObjectVelocity,localID, 5.0);
}

short CreateObjectSimple(int XPosition, int YPosition, int ZPosition, int localRotation, int localID)
{
	
	ObjectPosition[0] = (float)XPosition;
	ObjectPosition[1] = (float)YPosition;
	ObjectPosition[2] = (float)ZPosition;
	ObjectAngle[0] = 0;
	ObjectAngle[1] = (short)localRotation;
	ObjectAngle[2] = 0;
	ObjectVelocity[0] = 0;
	ObjectVelocity[1] = 0;
	ObjectVelocity[2] = 0;

	return MasterCreateObject(ObjectPosition,ObjectAngle,ObjectVelocity,localID, 5.0);
}

void UpdateObjectVelocity(Object* InputObject)
{
	InputObject->Position[0] += InputObject->Velocity[0];
	InputObject->Position[1] += InputObject->Velocity[1];
	InputObject->Position[2] += InputObject->Velocity[2];
}
void UpdateObjectAngle(Object* InputObject, short InputAngle[])
{
	InputObject->Angle[0] += InputAngle[0];
	InputObject->Angle[1] += InputAngle[1];
	InputObject->Angle[2] += InputAngle[2];
}



void UpdateObjectGravity(Object* InputObject)
{
	InputObject->Velocity[1] -= 0.5;
	/*
	if (InputObject->Velocity[1] > -2)
	{
		InputObject->Velocity[1] -= 0.5;
	}
	else
	{
		InputObject->Velocity[1] = -2;
	}
	*/
}


void ManualBump(Bump* BumpData, Vector Position )
{
	float dist;
	dist = BumpData->DistanceZX;
	if ((BumpData->DistanceZX < 0) && (BumpData->FlagZX == TRUE))     
	{
		Position[0] -= BumpData->BumpZX[0] * dist;
		Position[1] -= BumpData->BumpZX[1] * dist;
		Position[2] -= BumpData->BumpZX[2] * dist;
	}
	
	dist = BumpData->DistanceXY;
	if ((BumpData->DistanceXY < 0) && (BumpData->FlagXY == TRUE))
	{
		Position[0] -= BumpData->BumpXY[0] * dist;
		Position[1] -= BumpData->BumpXY[1] * dist;
		Position[2] -= BumpData->BumpXY[2] * dist;
	}
	
	dist = BumpData->DistanceYZ;
	if ((BumpData->DistanceYZ < 0) && (BumpData->FlagYZ == TRUE))
	{
		Position[0] -= BumpData->BumpYZ[0] * dist;
		Position[1] -= BumpData->BumpYZ[1] * dist;
		Position[2] -= BumpData->BumpYZ[2] * dist;
	}
}
void ManualBounce(Vector BumpDistance, Vector Velocity)
{
     float fx, fy, fz;
     float vx, vy, vz;
     float m, LocalVelo, vv, LocalVelo2;

     vx = Velocity[0];
     vy = Velocity[1];
     vz = Velocity[2];
     LocalVelo = sqrtf(vx * vx + vy * vy + vz * vz);

     m = BumpDistance[0] * vx + BumpDistance[1] * vy + BumpDistance[2] * vz;
     fx = vx - m * BumpDistance[0];
     fy = vy - m * BumpDistance[1];
     fz = vz - m * BumpDistance[2];
     vx = fx - m * BumpDistance[0];
     vy = fy - m * BumpDistance[1];
     vz = fz - m * BumpDistance[2];

     LocalVelo2 = sqrtf(vx * vx + vy * vy + vz * vz);
     vv = 1 / LocalVelo2 * LocalVelo;
     Velocity[0] = vx * vv;
     Velocity[1] = vy * vv;
     Velocity[2] = vz * vv;
}


void UpdateObjectBump(Object* InputObject)
{
	CheckBump2Simple(InputObject);
	BumpObject(InputObject);
}
	
void UpdateObjectFriction(Object* InputObject, float Friction)
{
	InputObject->Velocity[0] -= Friction;
	InputObject->Velocity[1] -= Friction;
	InputObject->Velocity[2] -= Friction;
}

void UpdateObjectFrictionScale(Object* InputObject, float FrictionScale)
{
	InputObject->Velocity[0] *= (1.0 - (FrictionScale / 30));
	InputObject->Velocity[1] *= (1.0 - (FrictionScale / 30));
	InputObject->Velocity[2] *= (1.0 - (FrictionScale / 30));
}

void PakkunStrategyOverride(Object* PiranhaPlant)
{
    PakkunObject *Pakkun = (PakkunObject*)(PiranhaPlant);
    if(PiranhaPlant->Flag&HIDEOBJ)
    {
        return;
    }
    if(PiranhaPlant->Flag&FLYINGOBJ)
    {
        PiranhaPlant->Position[1]+=4.0f;
        if(PiranhaPlant->Position[1]>800.0f)
        {
            PiranhaPlant->Flag|=HIDEOBJ;
            return;
        }
    }

	Pakkun->Timers[0] ++;
	if (Pakkun->Timers[0] > 60)
	{
		Pakkun->Timers[0] = 0;
	}
	
}

/*

set_itembox_object(Uint Address)
{
      Uint Number= SEGMENT_NUMBER(Address);
      Uint Offset= SEGMENT_OFFSET(Address);
      ITEMBOXOBJ *itembox =(ITEMBOXOBJ *)OS_PHYSICAL_TO_K0(segmentTable[Number]+Offset);
      OBJECT *obj;
      FVector Position;
      FVector Velocity;
      SVector Angle;
      short   Count;

      if(game_mode==TIMEATTACK)        return;
      if(!item_set_flag)        return;

      set_fvector(Velocity,0,0,0);
      //set_svector(Angle,0,0,0);
      while(itembox->Position[0]!=-32768)
      {
 //     if(screen_flip)	 Position[0]*=-1.0f;

      Position[0]=((float)itembox->Position[0])*flip_value;
      Position[1]=(float)itembox->Position[1];
      Position[2]=(float)itembox->Position[2];
  //    Position[1]=check_hight(Position[0],Position[1],Position[2]);

      Angle[0]=make_random();
      Angle[1]=make_random();
      Angle[2]=make_random();
      Count=add_object_buffer(Position,Angle,Velocity,IBOX);
      object_buffer[Count].Fparam=check_hight(Position[0],Position[1]+10,Position[2]);
      object_buffer[Count].Velocity[0]=Position[1];
      object_buffer[Count].Position[1]=object_buffer[Count].Fparam-20.0f;
      ++itembox;
      }
}
*/

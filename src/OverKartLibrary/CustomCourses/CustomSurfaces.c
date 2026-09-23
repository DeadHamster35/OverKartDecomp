#include "../MainInclude.h"

#define RESULT_JUMP				0x0800
#define MOVE_CAMERA				0
#define STOP_CAMERA				1
#define LAND					0x0100
#define WALLHIT	   				0x0010
#define WALLHIT_JOIN			0x0020

int LavaFloorBumpCount[8];
bool LavaFloorRecoilRequired[8];
bool LavaFloorRecoiling[8];

float AbsFloat(float Value)
{
	int x = *(int*)&Value;
	x &= 0x7FFFFFFF;
	return *(float*)&x;
}

void SurfaceSFX(Player *car, int SFX_ID, float min_Speed)
{
	short car_number = car - &GlobalPlayer[0];

	if (SPEEDMETER(car->Speed) >= min_Speed && car->Flag&IS_PLAYER)
	{
		if (PlayerSurfaceSoundID[car_number] == 0)
		{
			PlayerSurfaceSoundID[car_number] = SFX_ID;
		}
	}	
}


short SurfaceStorage[8];



void AddGravityEdit(Player *car)
{
	short car_number = car - &GlobalPlayer[0];

	short cont_number = car_number;
	if (car->Flag&IS_GHOST)
	{
		cont_number = car_number + 4;
		if (car_number == 0)
		{
			cont_number = 7;
		}
	}

	AddGravity(car);

	if (StartingIndicator < 3 || StartingIndicator >= 6 || FadingFlag[4] != 0)
	{
		car->MaxPower = 0;
		CustomWaterHeight[car_number] = false;
		SurfaceStorage[car_number] = 0;
		return;
	}

	// Keep using //
	switch (car->BumpStatus)
	{
	case GetItem:
		if (car->Jumpcount == 0 && car->Wallhitcount == 0)
		{
			RouletteStart(car_number,0);
		}
		break;
	case Shrunken:
		if (car->Jumpcount == 0 && car->Wallhitcount == 0)
		{
			SetShrunken(car_number, true);
		}
		break;
	case StarMan:
		if (car->Jumpcount == 0 && car->Wallhitcount == 0 && !(car->SlipFlag&IS_BROKEN))
		{
			SetStar(car, car_number);
		}
		break;
	case FastOoB:
		if (car->Jumpcount == 0 && car->Wallhitcount == 0)
		{
			SetFastOoB(car, car_number);
		}
		break;
	case BooSurface:
		if (car->Jumpcount == 0 && car->Wallhitcount == 0  && !(car->SlipFlag&IS_BROKEN))
		{
			SetGhostEffect(car_number, true, 0xFFFF);
		}
		break;
	case MushroomBoost:
		if (car->Accelcount < 300.0f)
		{
			car->MaxPower = 0;
		}				
		car->TurboTimer = 80;
		break;
	case Mud:
		if (car->Jumpcount == 0 && car->Wallhitcount == 0 && !(car->SlipFlag&(IS_STAR|IS_BRAKING)))
		{
			SurfaceSFX(car,SFX_WET_SAND_CENTER,10.0f);
			if (car->Flag&IS_CPU_PLAYER && car->HandlingFlag&CPU_SIMPLE_KART)
			{
				car->BumpStatus = 1;
				break;
			}
			car->PowerCont = 0.65f;
		}
		break;
	}

	// Storage Flag routine //
	
	if (car->SlipFlag&IS_BROKEN || car->JugemuFlag&ON_LAKITU_ROD)
	{
		SurfaceStorage[car_number] = STORE_NONE;
		car->MaxPower = 0;
	}

	if (SurfaceStorage[car_number]&STORE_TRICK)
	{
		if (car->Bump.DistanceZX >= 0.2f)
		{
			car->Gravity = TRICK_GRAVITY;
		}
		if (car->Jumpcount >= 40)
		{
			car->SlipFlag &= ~IS_FEATHER_JUMPING;
			car->OldDirection = car->Direction[1];
		}
		if (car->Jumpcount == 0)
		{
			ResetWing(car);
		}
		if (!(car->Flag&0x80))
		{
			SetAnimMusicNote(car_number);
			if (car->SlipFlag&IS_TURBO_BOOSTING)
			{
				car->TurboTimer += 15;
			}
			else
			{
				if (!(car->SlipFlag&IS_BROKEN))
				{
					SetTurbo(car, car_number);
					car->TurboTimer = 3;
				}
			}
			SurfaceStorage[car_number] = STORE_NONE;
			car->Velocity[1] = 0;
			MakeBodyColor(car,car_number,0x00500050,2.0f);
		}
	}



	if (SurfaceStorage[car_number]&STORE_GAP)
	{
		if (car->Bump.DistanceZX >= 0.2f)
		{
			float TestGravity = 10.0f * (256 - GlobalPlayer[car_number].Bump.Dummy);
			if (TestGravity != 0)
			{
				car->Gravity = TestGravity;
			}
	}
		if (car->Jumpcount >= 40)
		{
			car->SlipFlag &= ~IS_FEATHER_JUMPING;
			car->OldDirection = car->Direction[1];
		}
		if (car->Jumpcount == 0)
		{
			ResetWing(car);
		}
		if (!(car->Flag&0x80))
		{
			SetAnimMusicNote(car_number);
			if (car->SlipFlag&IS_TURBO_BOOSTING)
			{
				car->TurboTimer += 15;
			}
			else
			{
				if (!(car->SlipFlag&IS_BROKEN))
				{
					SetTurbo(car, car_number);
					car->TurboTimer = 3;
				}
			}
			SurfaceStorage[car_number] = STORE_NONE;
			car->Velocity[1] = 0;
			MakeBodyColor(car,car_number,0x00500050,2.0f);
		}
	}



	
	switch ((int)car->MaxPower)
	{
		case TrickJump:
		{
			if (car->Jumpcount <= 10 && GlobalController[cont_number]->ButtonPressed&BTN_R && SPEEDMETER(car->Speed) >= TRICK_TRIGGER_SPEED_MIN && !(car->SlipFlag&IS_BROKEN) && !(car->SlipFlag&IS_FEATHER_JUMPING) && !(SurfaceStorage[car_number]&STORE_TRICK))
			{
				car->Flag |= 0x80;
				SetAnimBonkStars(car_number);
				if (car->SlipFlag&IS_TURBO_BOOSTING)
				{
					short turbo_store = car->TurboTimer;
					SetWing(car, car_number);
					car->SlipFlag |= IS_TURBO_BOOSTING;
					car->TurboTimer = turbo_store;
				}
				else
				{
					SetWing(car, car_number);
				}
				car->Jumpcount = 0;
				if (car->MaxPower != car->BumpStatus)
				{
					car->MaxPower = car->BumpStatus;
				}
				SurfaceStorage[car_number] = STORE_TRICK;
			}
			else if (car->Flag & IS_CPU_PLAYER)
			{
				if (car->TireFL.Status != TrickJump)
				{
					car->Flag |= 0x80;
					SetAnimBonkStars(car_number);
					SetWing(car, car_number);
					car->Jumpcount = 0;
					if (car->MaxPower != car->BumpStatus)
					{
						car->MaxPower = car->BumpStatus;
					}
					SurfaceStorage[car_number] = STORE_TRICK;
				}
			}
			break;
		}

		case GapJump:
		{
			FaceStruct *SurfaceBuffer = (FaceStruct*)(FaceBuffer);
			Vtx *TargetVert = (Vtx*)SurfaceBuffer[GlobalPlayer[car_number].Bump.LastZX].p1;
			//Target Blue-Channel of SurfaceMap Vertex Color
			if (TargetVert->v.cn[2] != 255)
			{
				GlobalPlayer[car_number].Bump.Dummy = TargetVert->v.cn[2];  //use.Bump Dummy; goes Unused by gamecode		
			}
			
			if (car->Jumpcount <= 10 && GlobalController[cont_number]->ButtonPressed&BTN_R && SPEEDMETER(car->Speed) >= TRICK_TRIGGER_SPEED_MIN && !(car->SlipFlag&IS_BROKEN) && !(car->SlipFlag&IS_FEATHER_JUMPING) && !(SurfaceStorage[car_number]&STORE_GAP))
			{
				car->Flag |= 0x80;
				SetAnimBonkStars(car_number);
				if (car->SlipFlag&IS_TURBO_BOOSTING)
				{
					
					
					short turbo_store = car->TurboTimer;
					SetWing(car, car_number);
					car->SlipFlag |= IS_TURBO_BOOSTING;
					car->TurboTimer = turbo_store;
				}
				else
				{
					SetWing(car, car_number);
				}
				car->Jumpcount = 0;
				if (car->MaxPower != car->BumpStatus)
				{
					car->MaxPower = car->BumpStatus;		
				}
				
				
				SurfaceStorage[car_number] = STORE_GAP;
			}
			else if (car->Flag & IS_CPU_PLAYER)
			{
				if (car->TireFL.Status == GapJump)
				{
					FaceStruct *SurfaceBuffer = (FaceStruct*)(FaceBuffer);
					Vtx *TargetVert = (Vtx*)SurfaceBuffer[GlobalPlayer[car_number].Bump.LastZX].p1;
					//Target Blue-Channel of SurfaceMap Vertex Color
					GlobalPlayer[car_number].Bump.Dummy = TargetVert->v.cn[2];  //use.Bump Dummy; goes Unused by gamecode
				}
				else
				{
					car->Flag |= 0x80;
					SetAnimBonkStars(car_number);
					SetWing(car, car_number);
					car->Jumpcount = 0;
					if (car->MaxPower != car->BumpStatus)
					{
						car->MaxPower = car->BumpStatus;
					}
					SurfaceStorage[car_number] = STORE_GAP;
				}
			}
			break;
		}


		case ForceJump:
		{
			if (car->TireFL.Status == ForceJump)
			{
				FaceStruct *SurfaceBuffer = (FaceStruct*)(FaceBuffer);
				Vtx *TargetVert = (Vtx*)SurfaceBuffer[GlobalPlayer[car_number].Bump.LastZX].p1;
				//Target Blue-Channel of SurfaceMap Vertex Color
                if (TargetVert->v.cn[2] != 255)
			    {
				    GlobalPlayer[car_number].Bump.Dummy = TargetVert->v.cn[2];  //use.Bump Dummy; goes Unused by gamecode
                }
			}
			else
			{
				car->Flag |= 0x80;
				SetAnimBonkStars(car_number);
				SetWing(car, car_number);
				car->Jumpcount = 0;
				if (car->MaxPower != car->BumpStatus)
				{
					car->MaxPower = car->BumpStatus;
				}
				SurfaceStorage[car_number] = STORE_GAP;
			}
			break;
		}
	}
	
	// One Time uses //

	if (car->MaxPower == car->BumpStatus)
	{
		return;
	}
	if (car->Jumpcount != 0)
	{
		return;
	}

	switch (car->BumpStatus)
	{
	case ObjectHit:
		if (car->Jumpcount == 0 && car->Wallhitcount == 0 && !(car->SlipFlag&IS_BROKEN))
		{
			SetMapObjectHit(car_number);
			if (car->MaxPower != car->BumpStatus)
			{
				car->MaxPower = car->BumpStatus;
			}
		}
		break;
	case RedShellHit:
		if (car->Jumpcount == 0 && car->Wallhitcount == 0 && !(car->SlipFlag&IS_BROKEN))
		{
			SetRedShellHit(car_number);
			if (car->MaxPower != car->BumpStatus)
			{
				car->MaxPower = car->BumpStatus;
			}
		}
		break;
	case GreenShellHit:
		if (car->Jumpcount == 0 && car->Wallhitcount == 0 && !(car->SlipFlag&IS_BROKEN))
		{
			SetGreenShellHit(car_number);
			if (car->MaxPower != car->BumpStatus)
			{
				car->MaxPower = car->BumpStatus;
			}
		}
		break;
	case FailedStart:
		if (car->Jumpcount == 0 && car->Wallhitcount == 0 && !(car->SlipFlag&IS_BROKEN))
		{
			SetFailedStart(car_number);
			if (car->MaxPower != car->BumpStatus)
			{
				car->MaxPower = car->BumpStatus;
			}
		}
		break;
	case SpinOut:
		if (car->Jumpcount == 0 && car->Wallhitcount == 0 && !(car->SlipFlag&IS_BROKEN))
		{
			SetSpinOut(car_number);
			if (car->MaxPower != car->BumpStatus)
			{
				car->MaxPower = car->BumpStatus;
			}
		}
		break;
	case SpinOutSaveable:
		if (car->Jumpcount == 0 && car->Wallhitcount == 0 && !(car->SlipFlag&IS_BROKEN))
		{
			SetSpinOutSaveable(car_number);
			if (car->MaxPower != car->BumpStatus)
			{
				car->MaxPower = car->BumpStatus;
			}
		}
		break;
	case FeatherJump:
		if (car->Jumpcount == 0 && car->Wallhitcount == 0 && !(car->SlipFlag&IS_BROKEN))
		{
			SetWing(car, car_number);
			if (car->MaxPower != car->BumpStatus)
			{
				car->MaxPower = car->BumpStatus;
			}
		}
		break;
	case TornadoJump:
		if (car->Jumpcount == 0 && car->Wallhitcount == 0 && !(car->SlipFlag&IS_BROKEN))
		{
			SetStorm(car, car_number);
			if (car->MaxPower != car->BumpStatus)
			{
				car->MaxPower = car->BumpStatus;
			}
		}
		break;
	case MushroomBoost:
		if (car->Jumpcount == 0 && car->Wallhitcount == 0 && !(car->SlipFlag&IS_BROKEN))
		{
			SetTurbo(car, car_number);
			if (car->MaxPower != car->BumpStatus)
			{
				car->MaxPower = car->BumpStatus;
			}
		}
		break;

	case GapJump:
	{
		if (car->MaxPower != car->BumpStatus)
		{
			car->MaxPower = car->BumpStatus;
		}
		
		break;
	}
	default:
		if (car->MaxPower != car->BumpStatus)
		{
			car->MaxPower = car->BumpStatus;
		}
		break;
	}

	// Resets //

	switch ((int)car->MaxPower)
	{
	case Shrunken:
		if (car->BumpStatus != Shrunken)
		{
			SetShrunken(car_number, false);
		}
		break;
	case StarMan:
		if (car->BumpStatus != StarMan)
		{
			ResetStar(car, car_number);
		}
		break;
	case BooSurface:
		if (car->BumpStatus != BooSurface)
		{
			SetGhostEffect(car_number, false, 0);
		}
		break;
	default:
		break;
	}
}

//Water Surface Check

void CheckMapBGZXHook(Player *car, Vector normal, Vector Velocity, Vector g_vector, float *dist, float *new_x, float *new_y, float *new_z)
{
	if (HotSwapID > 0)
	{
		short car_number = car - &GlobalPlayer[0];
		if (car->BumpStatus == Water)
		{
			CustomWaterHeight[car_number] = TRUE;
			WaterlevelPlayer[car_number] = car->Position[1] - car->Offsetsize - car->Radius - car->Bump.DistanceZX;
		}
		else
		{
			CustomWaterHeight[car_number] = FALSE;
			CheckMapBGZX(car,normal,Velocity,g_vector,dist,new_x,new_y,new_z);
		}
	}
	else
	{
		CheckMapBGZX(car,normal,Velocity,g_vector,dist,new_x,new_y,new_z);
	}
}



void ExplorerSetJugemu(Player* Target)
{
	CheckJugemuMarker();
	if ((SurfaceExplorerMode == true) && (Target->JugemuFlag == 0))
	{
		return;
	}
	else
	{
		SetLakitu(Target);
	}
}

void ExplorerKWDisplayJugemu(int Player)
{
	CheckJugemuMarker();
	if ((StartingIndicator >= 3 && StartingIndicator < 5) && (SurfaceExplorerMode == true) && (GlobalPlayer[Player].JugemuFlag == 0))
	{
		return;
	}
	else
	{
		KWDisplayJugemu(Player);
	}
}

void ExplorerCallJugemu(Player* Target)
{
	CheckJugemuMarker();
	if ((SurfaceExplorerMode == true) && (Target->JugemuFlag == 0))
	{
		return;
	}
	else
	{
		CallLakitu(Target);
	}
}


void ExplorerHangJugemu(Player* Target, char Index, char place)
{
	CheckJugemuMarker();
	if ((SurfaceExplorerMode == true) && (Target->JugemuFlag == 0))
	{
		return;
	}
	else
	{
		HangLakitu(Target, Index, place);
	}
}


void CheckDirectionFunc(int Null, Screen *display)
{
	Ushort ang= display->Camera->CameraDirection[1];
	if(ScreenFlip)
    {
		if (ZFLIP)
		{	
			if(ang<DEG1 * 45)     display->CameraDirection=0;          
			else if(ang<DEG1 * 135)     display->CameraDirection=3;          
			else if(ang<DEG1 * 225)     display->CameraDirection=2;          
			else if(ang<DEG1 * 315)     display->CameraDirection=1;          
			else      display->CameraDirection=0;    
		}
		else
		{
			if(ang< DEG1 * 45)     display->CameraDirection=2;          
			else if(ang< DEG1 * 135)     display->CameraDirection=3;          
			else if(ang< DEG1 * 225)     display->CameraDirection=0;          
			else if(ang< DEG1 * 315)     display->CameraDirection=1;          
			else      display->CameraDirection=2;  
		}
           
    }
    else
    {
		if (ZFLIP)
		{	
			if(ang< (DEG1 * 45))     
			{
				display->CameraDirection=0;          
			}
			else 
			{
				if(ang< (DEG1 * 135))     
				{
					display->CameraDirection=1;  
				}
				else
				{
					if(ang< (DEG1 * 225))
					{
						display->CameraDirection=2;
					}
					else
					{
						if(ang< (DEG1 * 315))
						{
							display->CameraDirection=3;
						}
						else
						{
							display->CameraDirection=0;
						}
					}   
				}
			}          
		}
		else
		{
			if(ang<DEG1 * 45)     display->CameraDirection=2;          
			else if(ang<DEG1 * 135)     display->CameraDirection=1;          
			else if(ang<DEG1 * 225)     display->CameraDirection=0;          
			else if(ang<DEG1 * 315)     display->CameraDirection=3;          
			else      display->CameraDirection=2;    
		}
           
    }
}


//Useful types
#define SMOKE_DRIFTLETTER	1
#define SMOKE_RANDOM		2
#define SMOKE_GRASS			3
#define SMOKE_RAPID			5

//Smoke dither
#define SMOKE_DITHERNONE	0
#define SMOKE_DITHER		1

void InitCustomSmoke(Player *car, Vector pos_R, Vector pos_F, short Count, int kk, int surface, int LR_flag)
{
	int min_speed;
	int color;
	char Alpha;
	short car_number = car-&GlobalPlayer[0];

	if (HotSwapID == 0)
	{
		return;
	}

	switch (surface)
	{
	case Mud:
		min_speed = 5;
		color =  CHARS2INT(0,16,0,0);
		Alpha = 100;

		if(Count == 0 && (car->smoke[kk].Timer >= 1 || car->smoke[kk].Flag == 0))
		{
			if(SPEEDMETER(car->Speed) >= min_speed)
			{
				MakePos(car, &car->smoke[Count], pos_R[0], pos_R[1], pos_R[2], surface, LR_flag);
				MakeStartup(&car->smoke[Count], SMOKE_RANDOM, 0.75f);
				MakeRDP(&car->smoke[Count], color, Alpha);
				car->smoke[Count].Swork5 = SMOKE_DITHER;
				ParticleCreateLocal(PTCLGROUNDMUD, car_number, 0, -car->Direction[1], pos_R, 0.3f, SPEEDMETER(car->Speed)/200, 0, 0, 1, KWMODE_BL, FRAME_1, 1, LR_flag, CHARS2INT(230,140,40,175));
			}
		} 
		else
		{
			if(car->smoke[kk].Timer >= 1)
			{
				if(SPEEDMETER(car->Speed) >= min_speed)
				{
					MakePos(car, &car->smoke[Count], pos_R[0], pos_R[1], pos_R[2], surface, LR_flag);
					MakeStartup(&car->smoke[Count], SMOKE_RAPID, 1.0f);
					MakeRDP(&car->smoke[Count], color, Alpha);
					car->smoke[Count].Swork5 = SMOKE_DITHER;
					ParticleCreateLocal(PTCLGROUNDMUD, car_number, 0, -car->Direction[1], pos_R, 0.3f, SPEEDMETER(car->Speed)/200, 0, 0, 1, KWMODE_BL, FRAME_1, 1, LR_flag, CHARS2INT(230,140,40,175));
				}
			}
		}
		break;
	}
}

void InitRndSmokeHook(Player *car,short Count,int kk,char kno,char place)
{
	Ushort rndom;
	int surface, Flag;
	Vector pos_R;
	Vector pos_F;

	InitRndSmoke(car,Count,kk,kno,place);

	rndom = MakeRandomLimmit(10);
	surface = 0xff;
	pos_R[0] = 0;
	pos_R[1] = 0;
	pos_R[2] = 0;
	pos_F[0] = 0;
	pos_F[1] = 0;
	pos_F[2] = 0;

	if(rndom==0 || rndom==8)
	{
		pos_R[0] = car->TireRL.Position[0];
		pos_R[1] = car->TireRL.Height+2;
		pos_R[2] = car->TireRL.Position[2];
		pos_F[0] = car->TireFL.Position[0];
		pos_F[1] = car->TireFL.Height+2;
		pos_F[2] = car->TireFL.Position[2];
		Flag = 1; //LEFT
		surface = car->TireRL.Status;
	}
	if(rndom==2 || rndom==6)
	{
		pos_R[0] = car->TireRR.Position[0];
		pos_R[1] = car->TireRR.Height+2;
		pos_R[2] = car->TireRR.Position[2];
		pos_F[0] = car->TireFR.Position[0];
		pos_F[1] = car->TireFR.Height+2;
		pos_F[2] = car->TireFR.Position[2];
		Flag = 0; //RIGHT
		surface = car->TireRR.Status;
	}

	InitCustomSmoke(car, pos_R, pos_F, Count, kk, surface, Flag);
}

void InitRapidSmokeHook(Player *car,short Count,int kk,char kno,char place)
{
	Ushort rndom;
	int surface, Flag;
	Vector pos_R;
	Vector pos_F;

	InitRapidSmoke(car,Count,kk,kno,place);

	rndom = MakeRandomLimmit(10);
	surface = 0xff;
	pos_R[0] = 0;
	pos_R[1] = 0;
	pos_R[2] = 0;
	pos_F[0] = 0;
	pos_F[1] = 0;
	pos_F[2] = 0;

	if(rndom==0 || rndom==8)
	{
		pos_R[0] = car->TireRL.Position[0];
		pos_R[1] = car->TireRL.Height+2;
		pos_R[2] = car->TireRL.Position[2];
		pos_F[0] = car->TireFL.Position[0];
		pos_F[1] = car->TireFL.Height+2;
		pos_F[2] = car->TireFL.Position[2];
		Flag = 1; //LEFT
		surface = car->TireRL.Status;
	}
	if(rndom==2 || rndom==6)
	{
		pos_R[0] = car->TireRR.Position[0];
		pos_R[1] = car->TireRR.Height+2;
		pos_R[2] = car->TireRR.Position[2];
		pos_F[0] = car->TireFR.Position[0];
		pos_F[1] = car->TireFR.Height+2;
		pos_F[2] = car->TireFR.Position[2];
		Flag = 0; //RIGHT
		surface = car->TireRR.Status;
	}

	InitCustomSmoke(car, pos_R, pos_F, Count, kk, surface, Flag);
}

void InitSpinSmokeHook(Player *car,short Count,int kk,char kno,char place)
{
	Ushort rndom;
	int surface, Flag;
	Vector pos_R;
	Vector pos_F;

	InitSpinSmoke(car,Count,kk,kno,place);

	rndom = MakeRandomLimmit(10);
	surface = 0xff;
	pos_R[0] = 0;
	pos_R[1] = 0;
	pos_R[2] = 0;
	pos_F[0] = 0;
	pos_F[1] = 0;
	pos_F[2] = 0;

	if(rndom==0 || rndom==8)
	{
		pos_R[0] = car->TireRL.Position[0];
		pos_R[1] = car->TireRL.Height+2;
		pos_R[2] = car->TireRL.Position[2];
		pos_F[0] = car->TireFL.Position[0];
		pos_F[1] = car->TireFL.Height+2;
		pos_F[2] = car->TireFL.Position[2];
		Flag = 1; //LEFT
		surface = car->TireRL.Status;
	}
	if(rndom==2 || rndom==6)
	{
		pos_R[0] = car->TireRR.Position[0];
		pos_R[1] = car->TireRR.Height+2;
		pos_R[2] = car->TireRR.Position[2];
		pos_F[0] = car->TireFR.Position[0];
		pos_F[1] = car->TireFR.Height+2;
		pos_F[2] = car->TireFR.Position[2];
		Flag = 0; //RIGHT
		surface = car->TireRR.Status;
	}

	InitCustomSmoke(car, pos_R, pos_F, Count, kk, surface, Flag);
}

void LavaFloorRecoil(void)
{
	float launchAngle;
	float distZX, distY;
	float vel;
	int players = GameMode == 00 ? 8 : PlayerCount;
	{
	    int i;
	for (i = 0; i < players; i++)
	{
		short pathPoint;
		short Direction;
		float launchAngleRadians;
		float vZX;
		float vY;

		if (LavaFloorRecoilRequired[i])
		{
			LavaFloorRecoiling[i] = true;
			pathPoint = g_playerPathPointTable[i];

			NAPlyTrgStart(i, 0x1900a40b);
			GlobalPlayer[i].SlipFlag = 0;
			GlobalPlayer[i].SlipFlag |= IS_TUMBLING;
			GlobalPlayer[i].Speed = 0;

			Direction = (short)(MakeDirection(GlobalPlayer[i].Position[0], GlobalPlayer[i].Position[2], PathPointPointer[pathPoint].Pointx, PathPointPointer[pathPoint].Pointz) * -1);
			GlobalPlayer[i].Direction[1] = Direction;

			distZX = Sqrtf(
				(GlobalPlayer[i].Position[0] - PathPointPointer[pathPoint].Pointx) * (GlobalPlayer[i].Position[0] - PathPointPointer[pathPoint].Pointx)
				+
				(GlobalPlayer[i].Position[2] - PathPointPointer[pathPoint].Pointz) * (GlobalPlayer[i].Position[2] - PathPointPointer[pathPoint].Pointz));
			distY = PathPointPointer[pathPoint].Pointy - GlobalPlayer[i].Position[1];

			//Similar Height
			if (AbsFloat(distY < 30))
			{
				launchAngle = (-.135 * distZX) + 107.5;
				if (launchAngle < 55) launchAngle = 55;
				if (launchAngle > 80) launchAngle = 80;
				vel = 70;
				if (distZX < 500) { vel = 22; }
				if (distZX < 450) { vel = 20; }
				if (distZX < 400) { vel = 15; }
				if (distZX < 350) { vel = 16; }
				if (distZX < 300) { vel = 16; }
				if (distZX < 250) { vel = 16.8; }
				if (distZX < 200) { vel = 18; }
				if (distZX < 150) { vel = 16.5; }
				if (distZX < 125) { vel = 12; }
			}

			//Lava below course
			if (distY >= 30)
			{
				launchAngle = (-.135 * distZX) + 105.5;
				if (launchAngle < 60) launchAngle = 60;
				if (launchAngle > 80) launchAngle = 80;
				vel = 70;
				if (distZX < 500) { vel = 23; }
				if (distZX < 450) { vel = 21; }
				if (distZX < 400) { vel = 16; }
				if (distZX < 350) { vel = 17; }
				if (distZX < 300) { vel = 17; }
				if (distZX < 250) { vel = 17.8; }
				if (distZX < 200) { vel = 19; }
				if (distZX < 150) { vel = 17.5; }
				if (distZX < 100) { vel = 13; }
			}

			//Lava far below course
			if (distY >= 60)
			{
				launchAngle = (-.135 * distZX) + 107.5;
				if (launchAngle < 65) launchAngle = 65;
				if (launchAngle > 82) launchAngle = 82;
				vel = 70;
				if (distZX < 500) { vel = 26; }
				if (distZX < 450) { vel = 24; }
				if (distZX < 400) { vel = 19; }
				if (distZX < 350) { vel = 20; }
				if (distZX < 300) { vel = 20; }
				if (distZX < 250) { vel = 20.8; }
				if (distZX < 200) { vel = 22; }
				if (distZX < 150) { vel = 20.5; }
				if (distZX < 100) { vel = 16; }
			}

			if (distY <= -30) //Lava above course
			{
				launchAngle = (-.135 * distZX) + 107.5;
				if (launchAngle < 55) launchAngle = 55;
				if (launchAngle > 80) launchAngle = 80;
				vel = 70;
				if (distZX < 500) { vel = 20; }
				if (distZX < 450) { vel = 18; }
				if (distZX < 400) { vel = 13; }
				if (distZX < 350) { vel = 14; }
				if (distZX < 300) { vel = 14; }
				if (distZX < 250) { vel = 14.8; }
				if (distZX < 200) { vel = 16; }
				if (distZX < 150) { vel = 14.5; }
				if (distZX < 125) { vel = 10; }
			}

			if (distY <= -59) //Lava far above course
			{
				launchAngle = (-.135 * distZX) + 107.5;
				if (launchAngle < 55) launchAngle = 55;
				if (launchAngle > 80) launchAngle = 80;
				vel = 70;
				if (distZX < 500) { vel = 21; }
				if (distZX < 450) { vel = 19; }
				if (distZX < 400) { vel = 14; }
				if (distZX < 350) { vel = 14.5; }
				if (distZX < 300) { vel = 15; }
				if (distZX < 250) { vel = 15.8; }
				if (distZX < 200) { vel = 17; }
				if (distZX < 150) { vel = 15.5; }
				if (distZX < 125) { vel = 12; }
			}

			if (distY <= -89) //Lava really far above course
			{
				launchAngle = (-.135 * distZX) + 107.5;
				if (launchAngle < 55) launchAngle = 55;
				if (launchAngle > 80) launchAngle = 80;
				vel = 70;
				if (distZX < 500) { vel = 22; }
				if (distZX < 450) { vel = 14; }
				if (distZX < 400) { vel = 13; }
				if (distZX < 350) { vel = 13; }
				if (distZX < 300) { vel = 16; }
				if (distZX < 250) { vel = 16.8; }
				if (distZX < 200) { vel = 18; }
				if (distZX < 150) { vel = 16.5; }
				if (distZX < 125) { vel = 12; }
			}

			launchAngleRadians = launchAngle * 3.1415 / 180.0f;
			vZX = cosF(launchAngleRadians) * vel;
			vY = sinF(launchAngleRadians) * vel;

			ObjectVelocity[0] = 0;
			ObjectVelocity[1] = vY;
			ObjectVelocity[2] = vZX;

			MakeAlignVector(ObjectVelocity, GlobalPlayer[i].Direction[1]);
			GlobalPlayer[i].Gravity = 0;
			GlobalPlayer[i].GravityXz[0] = 0;
			GlobalPlayer[i].GravityXz[1] = 0;
			GlobalPlayer[i].GravityXz[2] = 0;
			GlobalPlayer[i].Velocity[0] = ObjectVelocity[0];
			GlobalPlayer[i].Velocity[1] = ObjectVelocity[1];
			GlobalPlayer[i].Velocity[2] = ObjectVelocity[2];
			LavaFloorRecoilRequired[i] = false;
		}

		if (LavaFloorRecoiling[i] && !LavaFloorRecoilRequired[i])
		{
			pathPoint = g_playerPathPointTable[i];
			if (pathPoint + 2 <= CourseTotalPathPoints[0]) pathPoint += 2;
			Direction = (short)(MakeDirection(GlobalPlayer[i].Position[0], GlobalPlayer[i].Position[2], PathPointPointer[pathPoint].Pointx, PathPointPointer[pathPoint].Pointz) * -1);
			GlobalPlayer[i].Direction[1] = Direction;
		}
	}
	}
}



void InteractLavaFloor(Bump* Bump, Ushort Pointer, int FlagAxis)
{
	int PlayerIndex = (*(long*)&Bump - (long)&g_PlayerStructTable - 0x110) / 0xDD8;
	FaceStruct* face = (FaceStruct*)FaceBuffer + Pointer;

	if ((face->Status & 0x00FF) == LavaSurface) //LavaFloor
	{
		GlobalPlayer[PlayerIndex].JugemuFlag |= LAVA_EFFECT;
		LavaFloorRecoilRequired[PlayerIndex] = true;
		LavaFloorBumpCount[PlayerIndex] += 1;
	}
	else
	{
		if ((FlagAxis & YAXIS_FACE) == YAXIS_FACE)
		{
			if (LavaFloorRecoiling[PlayerIndex])
			{
				ResetRolloverFall((Player*)&GlobalPlayer[PlayerIndex], PlayerIndex);
				if ((GlobalPlayer[PlayerIndex].SlipFlag & IS_TUMBLING) == IS_TUMBLING)
					GlobalPlayer[PlayerIndex].SlipFlag ^= IS_TUMBLING;

				if ((GlobalPlayer[PlayerIndex].JugemuFlag & LAVA_EFFECT) == LAVA_EFFECT)
					GlobalPlayer[PlayerIndex].JugemuFlag ^= LAVA_EFFECT;

				LavaFloorRecoilRequired[PlayerIndex] = false;
				LavaFloorRecoiling[PlayerIndex] = false;
				LavaFloorBumpCount[PlayerIndex] = 0;
			}
		}
		
	}

	if (LavaFloorBumpCount[PlayerIndex] >= 4)
	{
		CallLakitu(&GlobalPlayer[PlayerIndex]);
	}
}

void CustomLavaSpinKart(Player* car, Camera* Camera, char place, char kno)
{
	Vector power_vector = { 0.0f,0.0f,1.0f };
	Vector g_vector = { 0,0,0 };
	Vector power_vec = { 0,0,0 };

	float new_x, new_y, new_z;
	float old_x, old_y, old_z;
	float nx;
	float grav_x, grav_y, grav_z;
	float dist, Speed;
	Vector Velocity;
	Vector normal;
	short carmera_flag;

	/////// WEAPON
	WeaponStatus(car, kno, place);

	/////// STRATEGY
	if ((car->SlipFlag & SPIN_L) == SPIN_L || (car->SlipFlag & SPIN_R) == SPIN_R
		|| (car->SlipFlag & RAPID_ACC) == RAPID_ACC || (car->SlipFlag & WING) == WING
		|| (car->SlipFlag & STORM) == STORM || (car->SlipFlag & THUNDER_SPIN) == THUNDER_SPIN
		|| (car->HandlingFlag & RESULT_JUMP)) carmera_flag = STOP_CAMERA;
	else carmera_flag = MOVE_CAMERA;

	SetStrategy(car, kno, place);

	////// FORCE
	//AddGravity(car);
	//Instead of AddGravity, just set the Gravity to 1000 and zx Gravity to 0.
	car->Gravity = 1000;
	car->GravityXz[0] = 0;
	car->GravityXz[2] = 0;

	//Don't care about slip stream here
	//SlipCheck(car,kno);

	if ((car->SlipFlag & SPIN_L) == SPIN_L || (car->SlipFlag & SPIN_R) == SPIN_R || (car->SlipFlag & THUNDER_SPIN) == THUNDER_SPIN)
	{
		grav_x = (-1 * (car->GravityXz[0])) + ((-car->Bump.BumpZX[0] * car->Gravity) * .1);
		grav_y = (-car->Bump.BumpZX[1] * car->Gravity);
		grav_z = (-1 * (car->GravityXz[2])) + ((-car->Bump.BumpZX[2] * car->Gravity) * .1);
	}
	else
	{
		grav_x = -1 * car->GravityXz[0];
		grav_y = -1 * (car->Gravity);
		grav_z = -1 * car->GravityXz[2];
	}
	//////// CAR & CAR
	CheckKartHit(car, kno, place);

	///////// VELOCITY (X,Y,Z)
	if (carmera_flag == STOP_CAMERA) {
		MakeAlignMatrix(car->align, car->UpVector[0], car->UpVector[1], car->UpVector[2], car->Direction[1]);
		MakeAlignMatrix(car->Align2, car->UpVector[0], car->UpVector[1], car->UpVector[2], car->OldDirection);
	}
	else {
		MakeAlignMatrix(car->align, car->UpVector[0], car->UpVector[1], car->UpVector[2], car->Direction[1]);
	}

	// Go away Power vector, you make math hard to do.
	//power_vector[2] = PowerCheck(car,kno); 
	//if(carmera_flag==STOP_CAMERA) 
	//	MultipleMatrixByVector(power_vector,car->Align2);
	//else 
	//	MultipleMatrixByVector(power_vector,car->align);

	Velocity[0] = car->Velocity[0];
	Velocity[1] = car->Velocity[1];
	Velocity[2] = car->Velocity[2];

	if (car->Hitcount <= 2 && car->Wallhitcount <= 2
		&& (car->SlipFlag & ROLLOVER) != ROLLOVER
		&& (car->SlipFlag & EXPLODE) != EXPLODE && (car->SlipFlag & THROW_EXPLODE) != THROW_EXPLODE) {
		Velocity[0] += ((((power_vector[0] + grav_x) + power_vec[0]) - (Velocity[0] * (0.12 * car->Mass))) / 6000) / ((car->BrakeTime * 5) + 1);
		Velocity[2] += ((((power_vector[2] + grav_z) + power_vec[2]) - (Velocity[2] * (0.12 * car->Mass))) / 6000) / ((car->BrakeTime * 5) + 1);
	}
	else {
		Velocity[0] += ((((power_vector[0] + grav_x) + power_vec[0]) - (Velocity[0] * (0.2 * car->Mass))) / 6000) * 0.08;
		Velocity[2] += ((((power_vector[2] + grav_z) + power_vec[2]) - (Velocity[2] * (0.2 * car->Mass))) / 6000) * 0.08;
	}

	Velocity[1] += ((((power_vector[1] + grav_y) + power_vec[1]) - (Velocity[1] * (0.12 * car->Mass))) / 6000); // /car->sus.JumpFirstspeed;

	if ((car->JugemuFlag & ON_LAKITU_ROD) == ON_LAKITU_ROD || (car->JugemuFlag & OUT_OF_BOUNDS) == OUT_OF_BOUNDS) {
		Velocity[0] = 0;
		Velocity[1] = 0;
		Velocity[2] = 0;
	}

	////// CAR POSITION
	old_x = car->Position[0];
	old_y = car->Position[1];
	old_z = car->Position[2];
	car->OldPosition[0] = car->Position[0];
	car->OldPosition[1] = car->Position[1];
	car->OldPosition[2] = car->Position[2];

	new_x = old_x + car->Velocity[0]; // WAS new_x = old_x+car->Velocity[0]+kwcar[kno].Boundvel[0];
	new_y = old_y + car->Velocity[1];
	new_z = old_z + car->Velocity[2]; // WAS old_z+car->Velocity[2]+kwcar[kno].Boundvel[2];


	//DriftJump(car);
	//new_y +=car->Jump;

	//////  CHECK BUMP 
	CustomCheckBump2((Bump*)&car->Bump, car->Radius, new_x, new_y, new_z, car->OldPosition[0], car->OldPosition[1], car->OldPosition[2]);
	car->UpVector[0] = 0;
	car->UpVector[1] = 1;
	car->UpVector[2] = 0;

	/////JUMP FLAG
	car->SlipFlag = car->SlipFlag | N_JUMP;
	car->Jumpcount += 1;
	if ((dist = car->Bump.DistanceZX) <= 0) {
		car->SlipFlag = car->SlipFlag & ~(D_JUMP);
		car->SlipFlag = car->SlipFlag & ~(N_JUMP);
		if ((car->SlipFlag & ROLLOVER) != ROLLOVER && (car->SlipFlag & EXPLODE) != EXPLODE
			&& (car->SlipFlag & THROW_EXPLODE) != THROW_EXPLODE) {
			if (car->Jumpcount >= 28) {  ///////BIG PURUPURU
				if (car->Jumpcount >= 50) car->Jumpcount = 50;
				car->sus.BoundFirstspeed = 3;
				car->sus.BoundTimer = 0;
				car->Talk = car->Talk | POOMP;
				if ((car->Flag & IS_PLAYER) == IS_PLAYER && (car->Flag & IS_GHOST) != IS_GHOST)NaPlyLandStart(kno, (float)(car->Jumpcount) / 50);
				if ((car->Flag & IS_PLAYER) == IS_PLAYER) car->HandlingFlag |= LAND;
			}
			if ((car->Jumpcount <= 27 && car->Jumpcount >= 10) && SPEEDMETER(car->Speed) >= 20) {   ///////SMALL PURUPURU 
				car->sus.BoundFirstspeed = 2;
				car->sus.BoundTimer = 0;
				if ((car->Flag & IS_PLAYER) == IS_PLAYER && (car->Flag & IS_GHOST) != IS_GHOST)NaPlyLandStart(kno, (float)(car->Jumpcount) / 50);
				if ((car->Flag & IS_PLAYER) == IS_PLAYER) car->HandlingFlag |= LAND;
			}
			car->Jumpcount = 0;    ///// JUMP STOP
		}
		else { ///////////ROLLOVER
			if (car->Jumpcount >= 10) {
				if (car->Jumpcount >= 50) car->Jumpcount = 50;
				if ((car->Flag & IS_PLAYER) == IS_PLAYER && (car->Flag & IS_GHOST) != IS_GHOST)NaPlyLandStart(kno, (float)(car->Jumpcount) / 20);

				if (car->Jumpcount >= 40) car->Jumpcount = 20;
				if ((car->SlipFlag & ROLLOVER) == ROLLOVER) {
					car->Jumpcount /= 6.5;
					car->JmpAcc = 0.06;
					car->JmpSpeed = 0.0;
				}
				else {////,EXPLODE
					car->Jumpcount /= 7.5;
					car->JmpAcc = 0.06;
					car->JmpSpeed = 0.0;
					if ((car->Flag & IS_PLAYER) == IS_PLAYER) car->HandlingFlag |= LAND;
				}
			}
			else car->Jumpcount = 0;
		}
		car->Jump = car->Jumpcount;
	}

	/////// CHECK HIT
	if ((dist = car->Bump.DistanceZX) <= 0) {
		CheckMapBGZX(car, normal, Velocity, g_vector, &dist, &new_x, &new_y, &new_z);
	}
	if ((dist = car->Bump.DistanceXY) < 0) {
		CheckMapBGXY(car, normal, Velocity, &dist, &new_x, &new_y, &new_z);
		CheckWall(car, kno, Velocity);
		AccelOff(car, 6);
	}
	if ((dist = car->Bump.DistanceYZ) < 0) {
		CheckMapBGYZ(car, normal, Velocity, &dist, &new_x, &new_y, &new_z);
		CheckWall(car, kno, Velocity);
		AccelOff(car, 6);
	}
	if ((dist = car->Bump.DistanceXY) >= 0 && (dist = car->Bump.DistanceYZ) >= 0) {
		car->HitFlag &= ~WALLHIT_JOIN;
		if (car->Wallhitcount != 0) {
			car->Wallhitcount += 1;
			if (car->Wallhitcount >= 10)car->Wallhitcount = 0;
		}
	}

	///// CHECK ROLOVER_FALL
	if (CheckSlope(car->Bump.LastZX) != 0 && (car->SlipFlag & ROLLOVER_FALL) != ROLLOVER_FALL) {
		SetRolloverFall(car, kno);
	}
	else {
		if (!(car->SlipFlag & N_JUMP) && CheckSlope(car->Bump.LastZX) == 0 && (car->SlipFlag & ROLLOVER_FALL))
			ResetRolloverFall(car, kno);
	}

	/////check basic Player,s hight
	car->Ground = CalcHeight(new_x, new_y, new_z, car->Bump.LastZX);

	//////CHECK TIRE POSITION 
	if ((car->Flag & IS_PLAYER) == IS_PLAYER && (ScreenSplitA == 0 || ScreenSplitA == 1 || ScreenSplitA == 2))
		TirePosition(car, new_x, new_y, new_z);
	else
		EnemyTirePosition(car, new_x, new_y, new_z);

	////// SLIPANGLE
	SetSlipAngle(car, kno, old_x, old_z, new_x, new_z);

	////// NEW POSITION
	Speed = Velocity[0] * Velocity[0] + Velocity[2] * Velocity[2];
	car->OldSpeed = car->Speed;
	car->Speed = Sqrtf(Speed);

	if (((car->SlipFlag & ROLLOVER) != ROLLOVER && car->Force <= 0 && car->Speed < 0.13) || ((car->SlipFlag & ROLLOVER) != ROLLOVER && car->Force <= 0 && car->Speed < 0.20 && (car->SlipFlag & BREAKE) == BREAKE)) {
		Velocity[0] = Velocity[0] + (-1 * Velocity[0]);
		Velocity[2] = Velocity[2] + (-1 * Velocity[2]);
	}
	else {
		car->Position[0] = new_x;
		car->Position[2] = new_z;
	}
	car->Position[1] = new_y;
	car->GravityXz[0] = g_vector[0];
	car->GravityXz[2] = g_vector[2];

	car->Velocity[0] = Velocity[0];
	car->Velocity[1] = Velocity[1];
	car->Velocity[2] = Velocity[2];


	if ((car->Flag & IS_PLAYER) == IS_PLAYER) {
		if (car->Speed > 9) // WAS if(car->Speed>MaxSpeed[car->Kart])
		{
			nx = 9 / car->Speed; // WAS nx=MaxSpeed[car->Kart]/car->Speed;
			car->Velocity[0] = car->Velocity[0] * nx;
			car->Velocity[1] = car->Velocity[1] * nx;
			car->Velocity[2] = car->Velocity[2] * nx;
			car->Speed = 9; // WAS car->Speed = MaxSpeed[car->Kart];
		}
	}

	////////CHECK SPLASH
	CheckSplash(car, kno);
}

void CustomSpinKart(Player* Car, Camera* Camera, char place, char kno)
{
	int PlayerID = (*(long*)&Car - (long)&g_PlayerStructTable) / 0xDD8;
	if (LavaFloorRecoiling[PlayerID])
	{
		CustomLavaSpinKart(Car, Camera, place, kno);
	}
	else
	{
		SpinKart(Car, Camera, place, kno);
	}
}

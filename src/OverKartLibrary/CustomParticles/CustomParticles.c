#include "../MainInclude.h"

Ushort CParticleCount;

CustomParticle CustomParticleBuffer[MAX_CUSTOM_PARTICLES];

//Call on Race Start
void InitialCustomParticleBuffer(void)
{
	CParticleCount=0;
	{
	    int i;
	for (i=0; i<MAX_CUSTOM_PARTICLES; i++)
	{
		//Default values on race init
		CustomParticleBuffer[i].Flag=0;
		CustomParticleBuffer[i].Category=0;
		CustomParticleBuffer[i].Counter=0; 
		CustomParticleBuffer[i].cparam=0; 
		CustomParticleBuffer[i].Fparam=0; 
		CustomParticleBuffer[i].Renderdistance=0;
		CustomParticleBuffer[i].Drawmode=0;
		CustomParticleBuffer[i].movemode=0;
		CustomParticleBuffer[i].Scale=0;
		CustomParticleBuffer[i].Position[0]=0;
		CustomParticleBuffer[i].Position[1]=0;
		CustomParticleBuffer[i].Position[2]=0;
		CustomParticleBuffer[i].Velocity[0]=0;
		CustomParticleBuffer[i].Velocity[1]=0;
		CustomParticleBuffer[i].Velocity[2]=0;
		CustomParticleBuffer[i].Angle[0]=0;
		CustomParticleBuffer[i].Angle[1]=0;
		CustomParticleBuffer[i].Angle[2]=0;
		CustomParticleBuffer[i].ColR=0;
		CustomParticleBuffer[i].colG=0;
		CustomParticleBuffer[i].colB=0;
		CustomParticleBuffer[i].colA=0;
		CustomParticleBuffer[i].Onscreen=0;
		CustomParticleBuffer[i].anim_speed=1;
		CustomParticleBuffer[i].anim_counter=0;
		CustomParticleBuffer[i].anim_maxframe=1;
		CustomParticleBuffer[i].draw_counter=0;
		CustomParticleBuffer[i].Loopcount=0;
		CustomParticleBuffer[i].loopcountmax=1;
		CustomParticleBuffer[i].Sparam1=0;
	}
	}
}

void DeleteCustomParticleBuffer(CustomParticle *particle)
{
	particle->Flag=0;
	particle->Category=0;
	--CParticleCount;
}

void SetCustomParticleBuffer(CustomParticle *particle,Vector pos,SVector Angle,Vector velo,float Scale,short Category)
{
	CopyVector(particle->Position,pos);
	CopyVector(particle->Velocity,velo);
	particle->Angle[0] = Angle[0];
	particle->Angle[1] = Angle[1];
	particle->Angle[2] = Angle[2];	



 	particle->Category=Category;
	SET_FLAG(particle->Flag,EXISTPTCL);
	particle->Counter=0;
	particle->cparam=0;
	particle->Fparam=0;
	particle->Renderdistance=500;
	particle->Scale=Scale;
	particle->Loopcount=0;
	particle->loopcountmax=1;
}

short AddCustomParticleBuffer(Vector pos,SVector Angle,Vector velo,float Scale,short Category,char filtermode)
{
	CustomParticle *particle;
	if(CParticleCount>=MAX_CUSTOM_PARTICLES)
	{
		return(-1);
	}
 	{
 	    int i;
 	for (i=0;i<MAX_CUSTOM_PARTICLES;i++)
	{
		if(CustomParticleBuffer[i].Flag==0)	 
		{
			++CParticleCount;
			particle=(CustomParticle *)&CustomParticleBuffer[i];
			SetCustomParticleBuffer(particle,pos,Angle,velo,Scale,Category);
			CustomParticleBuffer[i].Drawmode = filtermode;
			return(i);
		}
	}
 	}
	return(-1);		     //NO SPACE!!!!
}


////Color Mode////


void ParticleColormode(CustomParticle *particle)
{
	switch (particle->Drawmode)
	{
	case KWMODE_BL_FLASH:
	case KWMODE_PT_FLASH:
		ToumeIMode(MakeRandomLimmit(200)+40,MakeRandomLimmit(200)+40,MakeRandomLimmit(200)+40,particle->colA);
		break;
	default:
		ToumeIMode(particle->ColR,particle->colG,particle->colB,particle->colA);
		break;
	}
}

////Filter////


void ParticleFiltermode(CustomParticle *particle)
{
	switch (particle->Drawmode)
	{
	case KWMODE_BL:
	case KWMODE_BL_FLASH:
		gSPSetOtherMode(DisplayListHead++,G_SETOTHERMODE_H, G_MDSFT_TEXTFILT, 2, G_TF_BILERP);
		break;
	case KWMODE_PT:
	case KWMODE_PT_FLASH:
		gSPSetOtherMode(DisplayListHead++,G_SETOTHERMODE_H, G_MDSFT_TEXTFILT, 2, G_TF_POINT);
		break;
	default:
		break;
	}
}

////On screen////


void ParticleOnscreen(CustomParticle *particle, int Player, bool Onscreen)
{
	if(Onscreen)
	{
		switch (Player)
		{
		case 0:
			SET_FLAG(particle->Onscreen,ONSCREEN1);
			break;
		case 1:
			SET_FLAG(particle->Onscreen,ONSCREEN2);
			break;
		case 2:
			SET_FLAG(particle->Onscreen,ONSCREEN3);
			break;
		case 3:
			SET_FLAG(particle->Onscreen,ONSCREEN4);
			break;		
		default:
			break;
		}
	}
	if(!Onscreen)
	{
		switch (Player)
		{
		case 0:
			CLR_FLAG(particle->Onscreen,ONSCREEN1);
			break;
		case 1:
			CLR_FLAG(particle->Onscreen,ONSCREEN2);
			break;
		case 2:
			CLR_FLAG(particle->Onscreen,ONSCREEN3);
			break;
		case 3:
			CLR_FLAG(particle->Onscreen,ONSCREEN4);
			break;		
		default:
			break;
		}
	}
}

////Common Move Routines////

void ParticleUpdateVelMove(CustomParticle *particle)
{

	char frame;
	Uchar subframemode = 0;

	switch (particle->anim_speed)
	{
	default:
	case FRAME_1:
		frame = 0;
		break;
	case FRAME_2:
		frame = KW2GFTimer;
		break;
	case FRAME_4:
		frame = KW4GFTimer;
		break;
	case FRAME_8:
		frame = KW8GFTimer;
		break;
	case FRAME_16:
		frame = KW16GFTimer;
		break;

	case FRAME_SUB_2:
		subframemode = 1;
		frame = 2;
		break;
	case FRAME_SUB_3:
		subframemode = 1;
		frame = 3;
		break;
	case FRAME_SUB_4:
		subframemode = 1;
		frame = 4;
		break;
	case FRAME_SUB_5:
		subframemode = 1;
		frame = 5;
		break;
	case FRAME_SUB_6:
		subframemode = 1;
		frame = 6;
		break;
	}

	if (!InGame)
	{
		switch (particle->anim_speed)
		{
		default:
		case FRAME_1:
			frame = 0;
			break;
		case FRAME_2:
			frame = GlobalFrameChar/10;
			break;
		case FRAME_4:
			frame = GlobalFrameChar/6;
			break;
		case FRAME_8:
			frame = GlobalFrameChar/3;
			break;
		case FRAME_16:
			frame = GlobalFrameChar/2;
			break;
		}
	}
	

	switch (subframemode)
	{
	default:
	case 0:
		if (particle->anim_counter < particle->anim_maxframe && frame == 0)
		{
			particle->anim_counter++;
		}
		if (particle->anim_counter >= particle->anim_maxframe)
		{
			particle->anim_counter = 0;
			particle->Loopcount++;
		}
		break;

	case 1:
		if (particle->anim_counter < particle->anim_maxframe)
		{
			if (particle->anim_counter + frame >= particle->anim_maxframe)
			{
				particle->anim_counter = particle->anim_maxframe;
			}
			else
			{
				particle->anim_counter += frame;
			}
		}
		particle->Loopcount++;
		break;
	}

	if (particle->Loopcount >= particle->loopcountmax)
	{
		DeleteCustomParticleBuffer(particle);
	}	


	particle->Position[0] += particle->Velocity[0];
	particle->Position[1] += particle->Velocity[1];
	particle->Position[2] += particle->Velocity[2];
}




////-------Create Local Particle-------//
// Category = particle visual
// Player = Camera screen
// screen_share = show to every Player
// rndoffset = random spawn Offset from Position
// filtermode = bilinear/Point KWMODE_BL/KWMODE_PT or add color flashing: _FLASH
// anim_speed = FRAME_1/FRAME_2/FRAME_4/FRAME_8/FRAME_16 or _SUB_ for skip
// Loopcount = sprite sheet Loop until destruction
// Xflip = mirror sprite
// RGBA = int 0xRRGGBBAA or CHARS2INT(r,g,b,a)
short ParticleCreateLocal(short Category, int Player, bool screen_share, short Angle, Vector pos, float Scale, float speed_forward, float speed_up, short rotationadd, Ushort rndoffset, char filtermode, Uchar anim_speed, Uchar Loopcount, Uchar Xflip, Uint RGBA)
{
	CustomParticle *particle;	
	short particleID = -1;
	float rndX;
	float rndY;
	float rndZ;

	Vector vel;
	SVector ang;

	vel[0] = 0;
	vel[1] = speed_up;
	vel[2] = speed_forward;
	ang[0] = 0;
	ang[1] = 0;
	ang[2] = deg(180);

	MakeAlignVector(vel,Angle);

	particleID = AddCustomParticleBuffer(pos,ang,vel,Scale,Category,filtermode);
	if (particleID < 0)
	{
		return -1;
	}

	particle=&CustomParticleBuffer[particleID];

	particle->Owner = (char)Player;
	particle->anim_speed = anim_speed;
	particle->anim_counter = 0;
	particle->Xflip = Xflip;
	particle->loopcountmax = Loopcount;
	particle->Sparam1 = rotationadd;


	particle->ColR = (RGBA>>24) & 0xFF;
	particle->colG = (RGBA>>16) & 0xFF;
	particle->colB = (RGBA>>8) & 0xFF;
	particle->colA = RGBA & 0xFF;

	rndX = MakeRandomLimmit(rndoffset);
	rndY = MakeRandomLimmit(rndoffset);
	rndZ = MakeRandomLimmit(rndoffset);
	if (MakeRandomLimmit(2) == 0)
	{
		particle->Position[0] += rndX;
		particle->Position[1] += rndY;
		particle->Position[2] += rndZ;
	}
	else
	{
		particle->Position[0] -= rndX;
		particle->Position[1] -= rndY;
		particle->Position[2] -= rndZ;		
	}

	if (screen_share)
	{
		SET_FLAG(particle->Flag,SCRNSHREPTCL);
	}

	switch (particle->Category)
	{
	default:
		particle->anim_maxframe = 6;
		break;
	case PTCLSMOKEPUFF:
	case PTCLBUBBLEPOP:
	case PTCLEXPLOSION:
		particle->anim_maxframe = 7;
		break;
	case PTCLSPLATTER:
		particle->anim_maxframe = 4;
		break;
	case PTCLDIRT:
		particle->anim_maxframe = 3;
		break;
	case PTCLPEBBLE:
		particle->Renderdistance = 3000;
		particle->anim_maxframe = 1;
		break;
	case PTCLBUBBLE:
		particle->anim_maxframe = 1;
		break;
	}

	return particleID;
}


////Common 32x32 Particle////

void ParticleDrawCommon32X32(CustomParticle *particle, int Player, bool RGBAMode)
{
	float Distance;
	int rdist;
	Vtx_t *VTXTile;
	Gfx *GFXTile;
	Uchar SpriteMode;
	Ushort *sprite_sheet;

	if (!(particle->Flag&SCRNSHREPTCL) && particle->Owner != Player)
	{
		return;
	}

	rdist= particle->Renderdistance;

	Distance=CheckDisplayRange(GlobalCamera[Player]->CameraPos,particle->Position,GlobalCamera[Player]->CameraDirection[1],20*20,CameraZoom[Player],rdist*rdist);
	
	if(Distance<0 && InGame == 1) // Not on screen
	{
		return;
	}

	// Setup //
	GFXTile = D_0D007B00;
	SpriteMode = 0;

	if (RGBAMode)
	{
		GFXTile = KWTextureRGBAXLUAAZBBlInit;
		SpriteMode = 1;
	}

	VTXTile = (Vtx_t *) VtxKTile32X32;

	if (particle->Xflip)
	{
		VTXTile = (Vtx_t *) VtxKTile32X32XFlip;
	}

	// Sheets //
	switch (particle->Category)
	{
	default:
	case PTCLSMOKEPUFF:
		sprite_sheet = ParticleSmokePuffIA16;
		break;
	case PTCLPUFF:
		sprite_sheet = ParticlePuffIA16;
		break;
	case PTCLPOP:
		sprite_sheet = ParticlePopIA16;
		break;
	case PTCLSPARKLE:
		sprite_sheet = ParticleSparkleIA16;
		break;
	case PTCLGROUNDMUD:
		sprite_sheet = ParticleMudSplatIA16;
		break;
	case PTCLBUBBLEPOP:
		sprite_sheet = ParticleBubblePopIA16;
		break;
	case PTCLPEBBLE:
		sprite_sheet = ParticlePebbleIA16;
		break;
	case PTCLBUBBLE:
		sprite_sheet = ParticleBubbleIA16;
		break;
	case PTCLEXPLOSION:
		sprite_sheet = ParticleExplosionRGBA16;
		break;
	case PTCLSPLATTER:
		sprite_sheet = ParticleSplatterIA16;
		break;
	case PTCLDIRT:
		sprite_sheet = ParticleDirtIA16;
		break;
	}

	if (particle->anim_maxframe > 1)
	{
		sprite_sheet += (0x400*particle->anim_counter);
	}	


	// Make sprite //
	if (InGame)
	{
	    particle->Angle[2] = particle->Angle[2] + particle->Sparam1;
	    particle->Angle[1] = KWLookCamera(particle->Position[0],particle->Position[2],GlobalCamera[Player]->CameraPos);
	}
	

	KWCreateAffine3D(particle->Position,particle->Angle,particle->Scale);

	switch (SpriteMode)
	{
	default:
	case 0:
		gSPDisplayList(DisplayListHead++, GFXTile);
		ParticleColormode(particle);
		ParticleFiltermode(particle);
		KWTextureIA16Sub(sprite_sheet, (Vtx *) VTXTile,32,32,32,32);
		break;

	case 1:
		gSPDisplayList(DisplayListHead++, GFXTile);
		ParticleColormode(particle);
		ParticleFiltermode(particle);
		KWTextureRGBASubBL(sprite_sheet, (Vtx *) VTXTile,32,32,32);
		break;
	}


}




////Particle////

void DrawCustomParticle(int Player)
{
	CustomParticle *particle;

	{
	    int i;
	for (i=0;i<MAX_CUSTOM_PARTICLES;i++)
	{
		if(!(CustomParticleBuffer[i].Flag&EXISTPTCL))
		{
			continue;
		}
		if(CustomParticleBuffer[i].Flag&HIDEPTCL)
		{
			continue;
		}

		particle =(CustomParticle *)&CustomParticleBuffer[i];

		switch (particle->Category)
		{
			case PTCLSPARKLE:
			case PTCLPUFF:
			case PTCLSMOKEPUFF:
			case PTCLPOP:
			case PTCLGROUNDMUD:
			case PTCLBUBBLEPOP:
			case PTCLPEBBLE:
			case PTCLBUBBLE:
			case PTCLSPLATTER:
			case PTCLDIRT:
				ParticleDrawCommon32X32(particle,Player,0);
				break;
			case PTCLEXPLOSION:
				ParticleDrawCommon32X32(particle,Player,1);
				break;
			default:
				break;
		}
	}
	}
}

void MoveCustomParticle(void)
{
	CustomParticle *particle;

	if (GamePausedFlag != 0)
	{
		return;
	}

	{
	    int i;
	for (i=0;i<MAX_CUSTOM_PARTICLES;i++)
	{
		if(!(CustomParticleBuffer[i].Flag&EXISTPTCL))
		{
			continue;
		}
		if(CustomParticleBuffer[i].Flag&HIDEPTCL)
		{
			continue;
		}

		particle =(CustomParticle *)&CustomParticleBuffer[i];

		ParticleUpdateVelMove(particle);

	}
	}
}

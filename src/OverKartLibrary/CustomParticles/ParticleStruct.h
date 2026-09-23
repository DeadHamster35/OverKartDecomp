#ifndef PARTICLESTRUCT_H_
#define PARTICLESTRUCT_H_

#include "../MainInclude.h"


#define MAX_CUSTOM_PARTICLES		200


// Custom Particle IDs
#define PTCLSPARKLE			1
#define PTCLPUFF			2
#define PTCLSMOKEPUFF		3
#define PTCLPOP				4
#define PTCLBUBBLEPOP		5
#define PTCLEXPLOSION		6
#define PTCLPEBBLE			7
#define PTCLBUBBLE			8
#define PTCLSPLATTER		9
#define PTCLGROUNDMUD		10
#define PTCLDIRT			11


// Particle KWMode flags
#define KWMODE_BL		0
#define KWMODE_PT		1
#define KWMODE_BL_FLASH	2
#define KWMODE_PT_FLASH	3

// Particle frame Modes
#define FRAME_1			0
#define FRAME_2			1
#define FRAME_4			2
#define FRAME_8			3
#define FRAME_16		4
#define FRAME_SUB_2		5
#define FRAME_SUB_3		6
#define FRAME_SUB_4		7
#define FRAME_SUB_5		8
#define FRAME_SUB_6		9

// Particle flags
#define EXISTPTCL   0x8000
#define HITPTCL     0x4000
#define MOVEPTCL    0x2000
#define MYPTCL      0x1000
#define HIDEPTCL	 0x800
#define SCRNSHREPTCL 0x400
#define FALLINGPTCL  0x200
#define SHADOWPTCL	 0x100

#define STATE4PTCL	  0x80
#define STATE3PTCL	  0x40
#define STATE2PTCL	  0x20
#define STATE1PTCL	  0x10



typedef struct CustomParticle{

	short		Flag;	
	Uchar		Category, Counter;

	Ushort 		Renderdistance;
	Uchar		Xflip, Owner;

	float   	Fparam;
	float 		Scale;

	Vector 		Position;
	Vector		Velocity;
	Uchar		Loopcount, loopcountmax;

	USVector	Angle;

	Uchar		Drawmode, movemode, draw_counter, cparam;
	Uchar		ColR, colG, colB, colA;
	Uchar		Onscreen, anim_speed, anim_counter, anim_maxframe;

	Uchar		Param1, param2;
	short		Sparam1;

} CustomParticle;


// Externals for Particle Files
extern Ushort ParticlePopIA16[];
extern Ushort ParticleMudSplatIA16[];
extern Ushort ParticleSparkleIA16[];
extern Ushort ParticleSmokePuffIA16[];
extern Ushort ParticlePuffIA16[];
extern Ushort ParticleExplosionRGBA16[];
extern Ushort ParticleApplePieceRGBA16[];
extern Ushort ParticleBubblePopIA16[];
extern Ushort ParticlePebbleIA16[];
extern Ushort ParticleBubbleIA16[];
extern Ushort ParticleSplatterIA16[];
extern Ushort ParticleDirtIA16[];







#endif

#ifndef STRUCT_H_
#define STRUCT_H_

/*
 * Vanilla Player / Camera / Hud / AnmObject / Object / Screen / Controller
 * live in decomp headers. This file keeps OverKart flags, macros, and custom types.
 */

#define CPU2SEC 	0x2CB5E16
//LakituRaceSequence
#define LAKITU_RACE_START 		1
#define LAKITU_RACE_APPROACH 		2
#define LAKITU_RACE_RACING		3
#define LAKITU_RACE_FINISH		4
#define LAKITU_RACE_VICTORYLAP	5

//Speedmeter conversion
#define SPEEDMETER(a)  (a/18)*216   ///18 = 1 METER, 1 frame = 60->60*60*60 = 216000/1000 = 216
#define SPEED_KM(a)	(a*18.0)/216.0 ///18=1METER ,1frame=60->60*60*60=216000/1000=216

// Character Names
#define CHAR_MARIO		0
#define CHAR_LUIGI		1
#define CHAR_YOSHI		2
#define CHAR_TOAD		3
#define CHAR_DONKEY		4
#define CHAR_WARIO		5
#define CHAR_PEACH		6
#define CHAR_BOWSER		7


#define   XAXIS_FACE      0x8000
#define   YAXIS_FACE      0x4000
#define   ZAXIS_FACE      0x2000


#define ib_nothing	0
#define ib_banana	1	
#define ib_bananas	2     
#define ib_gkame	3     
#define ib_gskame	4     
#define ib_rkame	5     
#define ib_rskame	6
#define ib_tkame	7
#define ib_thunder	8	
#define ib_imbox	9
#define ib_star		10
#define ib_tellesa	11     
#define ib_kinoko1	12    
#define ib_kinoko2	13
#define ib_kinoko3	14
#define ib_kinokoc	15

#define GAMEMODE_GP			0
#define GAMEMODE_TT			1
#define GAMEMODE_VS			2
#define GAMEMODE_BATTLE		3

#define MAX_OBJECT 100

//Object Flag
#define EXISTOBJ   0x8000
#define HITOBJ     0x4000
#define MOVEOBJ    0x2000
#define MYOBJ      0x1000
#define HIDEOBJ     0x800
#define FLYINGOBJ   0x400
#define SEBANANAOBJ  0x200
#define SEKAMEHITOBJ  0x100
#define SEKAMEOUTOBJ   0x80
#define ONSCREEN1     0x1
#define ONSCREEN2     0x2
#define ONSCREEN3     0x4
#define ONSCREEN4     0x8

// Player / weapon / water / handling / talk / Kimura slip flags live in defines.h.
// P1 sprite bits also live there. P2–P4 are OverKart-only.

//Player Status Flag
#define P2_DMA_SPRITE		0x0010
#define P2_DRAW_SPRITE		0x0020
#define P2_FLIP_SPRITE		0x0040
#define P2_DRAW_PARTICLES	0x0080
#define P3_DMA_SPRITE		0x0100
#define P3_DRAW_SPRITE		0x0200
#define P3_FLIP_SPRITE		0x0400
#define P3_DRAW_PARTICLES	0x0800
#define P4_DMA_SPRITE		0x1000
#define P4_DRAW_SPRITE		0x2000
#define P4_FLIP_SPRITE		0x4000
#define P4_DRAW_PARTICLES	0x8000

#ifndef G_TX_MIRROR
#define	G_TX_MIRROR			0x1
#endif
#ifndef G_TX_CLAMP
#define	G_TX_CLAMP			0x2
#endif
#ifndef G_TX_NOMASK
#define	G_TX_NOMASK	0
#endif
#ifndef G_TX_NOLOD
#define	G_TX_NOLOD	0
#endif


#define DOSHIN   1	    //Kawaguti's Work
#define TREE1    2
#define TREE2    3
#define TREE3    4
#define IWA      5
#define BANANA   6
#define GSHELL	 7
#define RSHELL	 8
#define EGG	 9	    //KT5 yoshy's valley
#define PAKKUN	 10
#define PUKUPUKU 11
#define IBOX   	 12
#define FAKEIBOX 13
#define SBANANA  14
#define SL1      15
#define SL2      16
#define SL3      17
#define COW      18
#define TREE4    19
#define PYLON    20	    //for example!!!!
#define SGSHELL    21
#define SRSHELL    22
#define MKANBAN    23
#define FIRE       24
#define PALMTREE   25
#define TREE5      26
#define TREE6      27
#define TREE7      28
#define SNOWTREE   29
#define SABOTEN1   30
#define SABOTEN2   31
#define SABOTEN3   32
#define TREE8      33
#define BLUEFIRE   34
#define WKANBAN    35
#define FIREPILLAR 36
#define TRUCK1     37
#define SHIP       38
#define FUMIKIRI   39
#define BUS        40
#define TANKLORRY  41
#define TSHELL	   42
#define SIBOX	   43
#define RV	   44
#define SEDAN	   44
#define NUTS	   45

//Player Slip Flag (OverKart names; same bits as defines.h BREAKE / DRIFT / …)
#define IS_BRAKING	 		0x00000001
#define IS_JUMPING 			0x00000002
#define IS_BOOSTJUMP_ZONE	0x00000004
#define IS_IN_AIR 			0x00000008
#define IS_DRIFTING			0x00000010
#define IS_AB_SPINNING		0x00000020
#define SPINOUT_RIGHT 		0x00000040
#define SPINOUT_LEFT 		0x00000080
#define IS_DRIFT_BOOSTING	0x00000100   
#define IS_STAR 			0x00000200    
#define IS_TUMBLING			0x00000400
#define IS_SPINNING_OUT		0x00000800  
#define ON_CENTER_LINE		0x00001000 
#define IS_TURBO_BOOSTING	0x00002000
#define IS_START_SPINNING	0x00004000  
#define IS_BONKING      	0x00008000
#define IS_WALL_TUMBLING  	0x00010000   
#define LIGHTNING_SPIN		0x00020000
#define IS_FEATHER_JUMPING	0x00080000
#define IS_BOOST_JUMPING	0x00100000
#define IS_SLIP_STREAMING	0x00200000
#define IS_WRONG_WAY		0x00400000    
#define IS_TORNADO_JUMPING	0x00800000
#define IS_VERTICAL_HIT		0x01000000  
#define IS_MOMENTUM_HIT		0x02000000
#define IS_SQUISHED			0x04000000
#define IS_UNSQUISHED		0x08000000
#define IS_HEIGHT_JUMPING	0x10000000		
#define IS_DRIFT_STEERING	0x20000000
#define IS_LIGHTNING_HIT	0x40000000
#define IS_BOO				0x80000000

// OverKart jugemu names not in defines.h (OUT_OF_BOUNDS is the same bit as LAKITU_SCENE).
#define OUT_OF_BOUNDS  		0x0008
#define IS_ALL_MELTING		0x0040

//Bomb Types
#define BOMBNULL		0
#define BOMBROTATEL		1
#define BOMBROTATER		2
#define BOMBSTOP		3


//Water Types
#define ZWATER		0
#define ZVOID		1
#define ZLAVA		2
#define ZICE		3
#define ZHOTSTUFF	4


#define JR		0x03E00008


//Custom bit flags example

//MARK: - Player


typedef struct EnemyItem //SIZE: 8* 0x10 = 0x80 | LOCATION: 801642D8 | 801642E8 | 801642F8 | 80164308 | 80164318 | 80164328 | 80164338 | 80164348
{   
	short	item_num;		//0
	short	obj_num;		//2
	short	time;			//4
	short	limit;			//6
	short	count1;			//8
	short	count2;			//A
	short	count3;			//C
	short	count4;			//E
}EnemyItem;


typedef struct BalloonColor{
	char Blank;
	char R,G,B;
} BalloonColor;

typedef struct GpResult //size: 0x20 | location: 0x802874D8
{
	short mode;			//00 802874D8 result mode 
	//empty short
	long  timer;		//04 802874DC result_timer 
	long  total_timer;	//08 802874E0 result_timer 
	long  timeend;		//0C 802874E4 timeend 		
	long  pos[3];		//10 802874E8
	char  carmove;		//1C 802874F4 move along trophy path
	char  rank;			//1D 802874F5 result rank 0,1,2,3 			| equals player num in trophy	   
	uchar  kart;		//1E 802874F6 result rank 0,1,2,3,4,5,6,7 
	// empty char
} GpResult;


typedef unsigned long Hierarchy;

typedef struct AnimeRecord{
	short			attr;
	short			code;
	short			start;
	short			loop;
	short			anime_frame;
	short			skeleton_total;
	short		   	*param;
	unsigned short 	*table;
} AnimeRecord,*AnimePtr;


typedef struct Lakitu{
		char		unknown;
		char		activeFlag; //busy
		short		unknown3;
		long		unknown4;
		long		unknown5;
		char      	pickupFlag;
		char		unknownFlag;
		char		reverseFlag;
		//much more
} Lakitu;

//Lakitu Animation Bit Flag
#define ANM_PLAYING			0x00002000
#define ANM_REVERSE			0x00004000

//Lakitu Alpha Bit Flag
#define ANM_ALPHA_VISIBLE	0x00000001
#define ANM_ALPHA_DITHER	0x00000002

//Lakitu Event Flag
#define LAKITU_START		1
#define LAKITU_GOAL			2
#define LAKITU_RESCUE		3
#define LAKITU_LAP2			4
#define LAKITU_LAPFINAL		5
#define LAKITU_REVERSE		6
#define LAKITU_RESCUE_ICE	7

typedef struct AnmObject_Lakitu{
		float		scale;
		float		pos[3];
		float		pos_offset[3];
		float		unknown[3];
		float		pos_anm_offset[3];
		float		unknown2[3];
		float		unknown3[4];
		unsigned long	anm_start_counter;
		long      	anm_bit_flag;
		long      	alpha_bit_flag;
		long		unknown4;
		long		colormode_copy;
		long		image_address_ptr;
		long		colormode;
		long		image_address_offset;
		long		unknown5;
		long		render_mode;
		long		unknown6;
		long		unknown_ptr;
		long		unknown_ptr2;
		float		unknown7[4];
		long      	status_flag;
		unsigned short	movement_counter;
		short      	movement_offset_counter;
		long		unknown8;
		ushort		alpha_dither;
		short		unknown9;
		long      	anm_progress_counter;
		long      	unknown_counter;
		short      	unknown10;
		short      	movement_offset_flag;
		short      	unknown11;
		short      	unknown12;
		short      	angle;
		short      	unknown13;
		float		unknown14[4];
		long      	active_flag;
		char      	anm_state_countdown;
		short      	unknown15;
		short      	unknown16;
		char      	anm_frame_count;
		char      	anm_frame_count_copy;
		short      	unknown17;
		short      	unknown18;
		char      	event_flag;
		char      	anm_frame_offsetX;
		char      	anm_frame_offsetY;
		long      	unknown_state_flag;
} AnmObject_Lakitu;


typedef struct SkyColor{
		short	RedHi, GreenHi, BlueHi;
		short	RedLo, GreenLo, BlueLo;
} SkyColor;


typedef struct Playercolor{
		short	R;
        short   dummy[7];
        short   G;
        short   dummy2[7];
        short   B;
        short   dummy3[7];
        short	D;
        short   dummy4[7];
        short   P;
        short   dummy5[7];
        short   Y;
} Playercolor;


typedef struct Stats{

	float 	FGrip[5][8];
	float 	*FGripPointer[5];
	float 	RGrip[5][8];
	float	*RGripPointer[5];
	float 	MaxPower[5][8];
	float	*MaxPowerPointer[5];
	float 	AccelerationCount[5][8];
	float	*AccelerationCountPointer[5];

	float 	Mass[8];
	float	Gravity[8];
	float	Friction[8];
	float	MaxSpeed[8];
	float	Radius[8];

	float 	SlipDownFT[8][15];
	float 	SlipDownRT[8][15];
	float	*SlipDownRTPointer[8];
	float	*SlipDownFTPointer[8];

	float 	PowerDownFT[8][15];
	float 	PowerDownRT[8][15];
	float	*PowerDownRTPointer[8];
	float	*PowerDownFTPointer[8];

	float	Acceleration[8][10];
	float	*AccelerationPointer[8];

	float	Brake[8][15];
	float	*BrakePointer[8];

	float	SlipAngle[8][15];
	float	*SlipAnglePointer[8];

	float	SteerSpeed[8][15];
	float	*SteerSpeedPointer[8];

	float	ProOffsetAngle[8];
	float	SteerSpeedReturn[8];
	float 	KartSteerLoss[8];
	float	PowerRecover[8];
	float	PowerBandAcceleration[8];

	float	DriftKartFSpeed[8];
	float	DriftKartAcceleration[8];
	float	WingKartFSpeed[8];
	float	WingKartAcceleration[8];
	float	StormKartFSpeed[8];
	float	StormKartAcceleration[8];
	float	RollKartFSpeed[8];
	float	RollKartAcceleration[8];
	float	HeightJumpKartFSpeed[8];
	float	HeightJumpKartAcceleration[8];

	int AccelerationOnOffFlag[8];
	int AccelerationIncFlag[8];
	int AccelerationTime[8];
	int AccelerationFlag[8];
	int PowerBandAccelerationInt[8];
	int PowerBandTimer[8];

	int BrakeOnOffFlag[8];
	int BrakeIncFlag[8];
	int BrakeTimer[8];
	int BrakeFlag[8];
	int FullBrakeAccelerationInt[8];
	int FullBrakeTimer[8];

	
} Stats;

typedef struct POSXY{
    short x,y;
    short dummy1,dummy2;
} POSXY;

typedef struct POS_XY_NoBS{
    short x,y;
} POS_XY_NoBS;

typedef struct PlayerTextureTable{
	uint		TextureOffset[8];
} PlayerTextureTable;

#define OBJ_BLOCK_MAX 32

typedef struct ObjBlock //size: 0x28 * 32 = 0x500 | location: 0x8018D9E0
{
    int 		kind;				//00 
    int 		pattern,pattern2;	//04 08
    int 		disp_x,disp_y;		//0C 10
    char 		pri,disp_on;		//14 15
	char 		hootanim;		//custom align
	char		zuul_flag;		//custom align	
    int 		sd_number;			//18 seq_data_number
    int 		work1,work2;		//1C 20
    float 		workf1;				//24
} ObjBlock;

typedef struct SOUKOU_ZURE_TYPE{        // 128 bit  (16)byte
    float lane;                            // 32 bit    (4)
    float lane_target;                    // 32 bit    (4)
    float speed;                        // 32 bit    (4)
    float lane_free_drive;                // 32 bit    (4)
}SOUKOU_ZURE_TYPE;

#define CHANNEL_MUTE		0x10000000
#define CHANNEL_PAUSE		0x20000000
#define CHANNEL_IDLE		0x40000000
#define CHANNEL_PLAYING		0x80000000

//SFX Structs
typedef struct SFXStruct{
    ushort                    flag1;
    ushort                    flag2;
    long                    rawSFXROMpointer;
    long                    loopdataRAMpointer;
    long                    predictordataRAMpointer;
    long                    length;
    long                    empty1;
    long                    empty2;
    long                    emtpy3;
} SFXStruct;

typedef struct SFXSampleStruct{
    ushort                    flag1;
    ushort                    flag2;
    long                    *unknown1;
    long                    empty1;
    long                    emtpy2;
    SFXStruct                *predictors;
    long                    unknown2;
    long                    empty3;
    long                    emtpy4;
} SFXSampleStruct;

// The below structs have been guesswork on the Seq0 sounds, but they would need to be extended for every sequence if you want to access those.
typedef struct SFXSampleIndexStruct{
    SFXSampleStruct            *SFX2pointer[127];
} SFXSampleIndexStruct;

typedef struct SFXTempPointerStruct{
    ushort                InstrumentChannelAmount;
    uchar                DrumChannelAmount,pad;
    SFXSampleIndexStruct            *InstSampleindex;
    SFXSampleIndexStruct            *DrumSampleindex2;
} SFXTempPointerStruct;


typedef struct TablePointer{
    int address;
    int length;        
} TablePointer;

typedef struct SequenceTable{
    short category;        
    short index;        
    TablePointer pointer[30];        
} SequenceTable;

typedef struct InstrumentTable{
    short category;        
    short index;        
    TablePointer pointer[21];        
} InstrumentTable;

typedef struct RawAudioTable{
    short category;        
    short index;        
    TablePointer pointer[21];        
} RawAudioTable;

typedef struct BankMap{
    uchar tableID;
    uchar instBankID;        
} BankMap;

typedef struct BankMapTable{
    short offset[30];        
    BankMap assignment[30];        
} BankMapTable;

typedef struct AudioTablePointers{
    SequenceTable SequenceTable_p;
    InstrumentTable InstrumentTable_p;
    RawAudioTable RawAudioTable_p;
    BankMapTable BankMapTable_p;
    short SongAmount;
    short free;
    SFXTempPointerStruct SFXAlloc_p;
} AudioTablePointers;


typedef struct SeqChannelStruct{
		int Playflag; 
		char unknown1; 
		char Priority; 
		short unknown3;
		char unknown4; 
		char PanNote; 
		char Pan;
		char unknown6;
		int unknown7;
		int unknown8;
		int unknown9;
		int Loopcount;
		short Instrument;
		short unknown11;
		float Volume;
		float VolumeMultiplicator;
		short unknown12;
		short Looppan;
		float VolumeTarget;
		float Pitch;
		int *unknown13;
		int *Audiotask1note;
		int *Audiotask2note;
		int *unknown14;
		int *Sequencepointer;
		int *Audiotask1loop;
		int *Audiotask2loop;
		int *unknown15;
		int unknown16;
		int unknown17;
		int unknown18;
		int *Currenttracknote;
		int unknown19;
		int unknown20;
		int unknown21;
		int unknown22;
		int unknown23;
		int unknown24;
		uchar Echo;
		uchar Release;
		short unknown25;
		int *unknown26;
		int *unknownselfpointer1;
		int *unknownselfpointer2;
		int unknown27;
		int *unknownselfpointer3;
		int *unknownselfpointer4;
		int *unknownselfpointer5;
		int unknown28;
		int *unknownselfpointer6;
		int *unknownselfpointer7;
		int *unknownselfpointer8;
		int unknown29;
		int *unknownselfpointer9;
		int *unknownselfpointer10;
		int *unknownselfpointer11;
		int unknown30;
		int *unknownselfpointer12;
} SeqChannelStruct;

typedef struct SeqPlayerStruct{
		int Playflag; 
		uchar CurrentID; 
		uchar SubID; 
		short unknown1;
		ushort Tempo;
		short unknown2;
		int Loopcount;
		int unknown3;
		int *RawSequence;
		float VolumeFull;
		float unknown5;
		int unknown6;
		float Volumedodge;
		float VolumeMultiplicator;
		float VolumeTarget;
		SeqChannelStruct *Channel[16];
		int *LoopBranch;
		int unknown7;
		int unknown8;
		int unknown9;
		int unknown10;
		int unknown11;
		int unknown12;
		int *unknown13;
		int *unknown14;
		int *unknownselfpointer1;
		int *unknownselfpointer2;
		int unknown15;
		int *unknownselfpointer3;
		int *unknownselfpointer4;
		int *unknownselfpointer5;
		int unknown16;
		int *unknownselfpointer6;
		int *unknownselfpointer7;
		int *unknownselfpointer8;
		int unknown17;
		int *unknownselfpointer9;
		int *unknownselfpointer10;
		int *unknownselfpointer11;
		int unknown18;
		int *unknownselfpointer912;
		int Empty[29];
} SeqPlayerStruct;

typedef struct CourseHeader {
	uint Seg6Start;      //(0x00) ROM address at which segment 6 file begins
	uint Seg6End;       //(0x04) ROM address at which segment 6 file ends
	uint Seg4Start;      //(0x08) ROM address at which segment 4 file begins
	uint Seg7End;       //(0x0C) ROM address at which segment 7 (not 4) file ends
	uint Seg9Start;      //(0x10) ROM address at which segment 9 file begins
	uint Seg9End;       //(0x14) ROM address at which segment 9 file ends
	uint Seg47Buffer;      //(0x18) RSP address of compressed segments 4 and 7
	uint NumberVerts;        //(0x1C) number of vertices in the vertex file
	uint Segment7Pointer;       //(0x20) RSP address at which segment 7 data begins
	uint Segment7Size;      //(0x24) Size of segment 7 data after decompression, minus 8 bytes for some reason
	uint TextureRSP;   //(0x28) RSP address of texture list
	ushort flag,unused;    //(0x2E) Padding
} CourseHeader;


typedef struct TexDataTable{
	uint null;
	short* DataPointer;
	short Width, Height;
	uint null2;
	short DMASize, pad1;
	uint padding[5]; //padding
} TexDataTable;

#endif

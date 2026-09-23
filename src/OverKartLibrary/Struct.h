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

// Player / Weapon / water / handling / Talk / Kimura slip flags live in defines.h.
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

//Player Slip Flag (OverKart Names; same bits as defines.h BREAKE / DRIFT / …)
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

// OverKart jugemu Names not in defines.h (OUT_OF_BOUNDS is the same bit as LAKITU_SCENE).
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
	short	ItemNum;		//0
	short	ObjNum;		//2
	short	Time;			//4
	short	Limit;			//6
	short	Count1;			//8
	short	Count2;			//A
	short	Count3;			//C
	short	Count4;			//E
}EnemyItem;


typedef struct BalloonColor{
	char Blank;
	char R,G,B;
} BalloonColor;

typedef struct GpResult //Size: 0x20 | location: 0x802874D8
{
	short Mode;			//00 802874D8 result Mode 
	//empty short
	long  Timer;		//04 802874DC result_timer 
	long  TotalTimer;	//08 802874E0 result_timer 
	long  Timeend;		//0C 802874E4 Timeend 		
	long  pos[3];		//10 802874E8
	char  Carmove;		//1C 802874F4 move along trophy path
	char  Rank;			//1D 802874F5 result Rank 0,1,2,3 			| equals Player num in trophy	   
	Uchar  Kart;		//1E 802874F6 result Rank 0,1,2,3,4,5,6,7 
	// empty char
} GpResult;


typedef unsigned long Hierarchy;

typedef struct AnimeRecord{
	short			Attr;
	short			Code;
	short			Start;
	short			Loop;
	short			AnimeFrame;
	short			SkeletonTotal;
	short		   	*Param;
	unsigned short 	*Table;
} AnimeRecord,*AnimePtr;


typedef struct Lakitu{
		char		Unknown;
		char		ActiveFlag; //busy
		short		Unknown3;
		long		Unknown4;
		long		Unknown5;
		char      	PickupFlag;
		char		UnknownFlag;
		char		ReverseFlag;
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

typedef struct AnmObjectLakitu{
		float		Scale;
		float		pos[3];
		float		PosOffset[3];
		float		Unknown[3];
		float		PosAnmOffset[3];
		float		Unknown2[3];
		float		Unknown3[4];
		unsigned long	AnmStartCounter;
		long      	AnmBitFlag;
		long      	AlphaBitFlag;
		long		Unknown4;
		long		ColormodeCopy;
		long		ImageAddressPtr;
		long		Colormode;
		long		ImageAddressOffset;
		long		Unknown5;
		long		RenderMode;
		long		Unknown6;
		long		UnknownPtr;
		long		UnknownPtr2;
		float		Unknown7[4];
		long      	StatusFlag;
		unsigned short	MovementCounter;
		short      	MovementOffsetCounter;
		long		Unknown8;
		Ushort		AlphaDither;
		short		Unknown9;
		long      	AnmProgressCounter;
		long      	UnknownCounter;
		short      	Unknown10;
		short      	MovementOffsetFlag;
		short      	Unknown11;
		short      	Unknown12;
		short      	Angle;
		short      	Unknown13;
		float		Unknown14[4];
		long      	ActiveFlag;
		char      	AnmStateCountdown;
		short      	Unknown15;
		short      	Unknown16;
		char      	AnmFrameCount;
		char      	AnmFrameCountCopy;
		short      	Unknown17;
		short      	Unknown18;
		char      	EventFlag;
		char      	AnmFrameOffsetX;
		char      	AnmFrameOffsetY;
		long      	UnknownStateFlag;
} AnmObjectLakitu;


typedef struct SkyColor{
		short	RedHi, GreenHi, BlueHi;
		short	RedLo, GreenLo, BlueLo;
} SkyColor;


typedef struct Playercolor{
		short	R;
        short   Dummy[7];
        short   G;
        short   Dummy2[7];
        short   B;
        short   Dummy3[7];
        short	D;
        short   Dummy4[7];
        short   P;
        short   Dummy5[7];
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
    short Dummy1,Dummy2;
} POSXY;

typedef struct POSXYNoBS{
    short x,y;
} POSXYNoBS;

typedef struct PlayerTextureTable{
	Uint		TextureOffset[8];
} PlayerTextureTable;

#define OBJ_BLOCK_MAX 32

typedef struct ObjBlock //Size: 0x28 * 32 = 0x500 | location: 0x8018D9E0
{
    int 		Kind;				//00 
    int 		Pattern,Pattern2;	//04 08
    int 		DispX,DispY;		//0C 10
    char 		pri,DispOn;		//14 15
	char 		Hootanim;		//custom align
	char		ZuulFlag;		//custom align	
    int 		SdNumber;			//18 seq_data_number
    int 		Work1,Work2;		//1C 20
    float 		Workf1;				//24
} ObjBlock;

typedef struct SOUKOU_ZURE_TYPE{        // 128 bit  (16)byte
    float Lane;                            // 32 bit    (4)
    float LaneTarget;                    // 32 bit    (4)
    float Speed;                        // 32 bit    (4)
    float LaneFreeDrive;                // 32 bit    (4)
}SOUKOU_ZURE_TYPE;

#define CHANNEL_MUTE		0x10000000
#define CHANNEL_PAUSE		0x20000000
#define CHANNEL_IDLE		0x40000000
#define CHANNEL_PLAYING		0x80000000

//SFX Structs
typedef struct SFXStruct{
    Ushort                    Flag1;
    Ushort                    Flag2;
    long                    RawSFXROMpointer;
    long                    LoopdataRAMpointer;
    long                    PredictordataRAMpointer;
    long                    Length;
    long                    Empty1;
    long                    Empty2;
    long                    Emtpy3;
} SFXStruct;

typedef struct SFXSampleStruct{
    Ushort                    Flag1;
    Ushort                    Flag2;
    long                    *Unknown1;
    long                    Empty1;
    long                    Emtpy2;
    SFXStruct                *Predictors;
    long                    Unknown2;
    long                    Empty3;
    long                    Emtpy4;
} SFXSampleStruct;

// The below structs have been guesswork on the Seq0 sounds, but they would need to be extended for every sequence if you want to access those.
typedef struct SFXSampleIndexStruct{
    SFXSampleStruct            *SFX2pointer[127];
} SFXSampleIndexStruct;

typedef struct SFXTempPointerStruct{
    Ushort                InstrumentChannelAmount;
    Uchar                DrumChannelAmount,pad;
    SFXSampleIndexStruct            *InstSampleindex;
    SFXSampleIndexStruct            *DrumSampleindex2;
} SFXTempPointerStruct;


typedef struct TablePointer{
    int Address;
    int Length;        
} TablePointer;

typedef struct SequenceTable{
    short Category;        
    short Index;        
    TablePointer Pointer[30];        
} SequenceTable;

typedef struct InstrumentTable{
    short Category;        
    short Index;        
    TablePointer Pointer[21];        
} InstrumentTable;

typedef struct RawAudioTable{
    short Category;        
    short Index;        
    TablePointer Pointer[21];        
} RawAudioTable;

typedef struct BankMap{
    Uchar TableID;
    Uchar InstBankID;        
} BankMap;

typedef struct BankMapTable{
    short Offset[30];        
    BankMap Assignment[30];        
} BankMapTable;

typedef struct AudioTablePointers{
    SequenceTable SequenceTableP;
    InstrumentTable InstrumentTableP;
    RawAudioTable RawAudioTableP;
    BankMapTable BankMapTableP;
    short SongAmount;
    short Free;
    SFXTempPointerStruct SFXAllocP;
} AudioTablePointers;


typedef struct SeqChannelStruct{
		int Playflag; 
		char Unknown1; 
		char Priority; 
		short Unknown3;
		char Unknown4; 
		char PanNote; 
		char Pan;
		char Unknown6;
		int Unknown7;
		int Unknown8;
		int Unknown9;
		int Loopcount;
		short Instrument;
		short Unknown11;
		float Volume;
		float VolumeMultiplicator;
		short Unknown12;
		short Looppan;
		float VolumeTarget;
		float Pitch;
		int *Unknown13;
		int *Audiotask1note;
		int *Audiotask2note;
		int *Unknown14;
		int *Sequencepointer;
		int *Audiotask1loop;
		int *Audiotask2loop;
		int *Unknown15;
		int Unknown16;
		int Unknown17;
		int Unknown18;
		int *Currenttracknote;
		int Unknown19;
		int Unknown20;
		int Unknown21;
		int Unknown22;
		int Unknown23;
		int Unknown24;
		Uchar Echo;
		Uchar Release;
		short Unknown25;
		int *Unknown26;
		int *Unknownselfpointer1;
		int *Unknownselfpointer2;
		int Unknown27;
		int *Unknownselfpointer3;
		int *Unknownselfpointer4;
		int *Unknownselfpointer5;
		int Unknown28;
		int *Unknownselfpointer6;
		int *Unknownselfpointer7;
		int *Unknownselfpointer8;
		int Unknown29;
		int *Unknownselfpointer9;
		int *Unknownselfpointer10;
		int *Unknownselfpointer11;
		int Unknown30;
		int *Unknownselfpointer12;
} SeqChannelStruct;

typedef struct SeqPlayerStruct{
		int Playflag; 
		Uchar CurrentID; 
		Uchar SubID; 
		short Unknown1;
		Ushort Tempo;
		short Unknown2;
		int Loopcount;
		int Unknown3;
		int *RawSequence;
		float VolumeFull;
		float Unknown5;
		int Unknown6;
		float Volumedodge;
		float VolumeMultiplicator;
		float VolumeTarget;
		SeqChannelStruct *Channel[16];
		int *LoopBranch;
		int Unknown7;
		int Unknown8;
		int Unknown9;
		int Unknown10;
		int Unknown11;
		int Unknown12;
		int *Unknown13;
		int *Unknown14;
		int *Unknownselfpointer1;
		int *Unknownselfpointer2;
		int Unknown15;
		int *Unknownselfpointer3;
		int *Unknownselfpointer4;
		int *Unknownselfpointer5;
		int Unknown16;
		int *Unknownselfpointer6;
		int *Unknownselfpointer7;
		int *Unknownselfpointer8;
		int Unknown17;
		int *Unknownselfpointer9;
		int *Unknownselfpointer10;
		int *Unknownselfpointer11;
		int Unknown18;
		int *Unknownselfpointer912;
		int Empty[29];
} SeqPlayerStruct;

typedef struct CourseHeader {
	Uint Seg6Start;      //(0x00) ROM Address at which segment 6 file begins
	Uint Seg6End;       //(0x04) ROM Address at which segment 6 file ends
	Uint Seg4Start;      //(0x08) ROM Address at which segment 4 file begins
	Uint Seg7End;       //(0x0C) ROM Address at which segment 7 (not 4) file ends
	Uint Seg9Start;      //(0x10) ROM Address at which segment 9 file begins
	Uint Seg9End;       //(0x14) ROM Address at which segment 9 file ends
	Uint Seg47Buffer;      //(0x18) RSP Address of compressed segments 4 and 7
	Uint NumberVerts;        //(0x1C) Number of vertices in the vertex file
	Uint Segment7Pointer;       //(0x20) RSP Address at which segment 7 data begins
	Uint Segment7Size;      //(0x24) Size of segment 7 data after decompression, minus 8 bytes for some reason
	Uint TextureRSP;   //(0x28) RSP Address of Texture list
	Ushort Flag,Unused;    //(0x2E) Padding
} CourseHeader;


typedef struct TexDataTable{
	Uint Null;
	short* DataPointer;
	short Width, Height;
	Uint Null2;
	short DMASize, pad1;
	Uint Padding[5]; //Padding
} TexDataTable;

#endif

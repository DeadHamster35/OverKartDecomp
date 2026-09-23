#include "MainInclude.h"
#include <assets/common_data.h>

void DisplayFlagGateCheck(Camera* LocalCamera)
{
	if (HotSwapID == 0)
    {
        DisplayFlagGate(LocalCamera);
        return;
    }
	if ( ((HotSwapID > 0) && (OverKartHeader.GoalBannerToggle != 0)) )
    {	
        
        if (OverKartHeader.FogStart > 0)
		{
			g_fogToggleBanshee = 1;
			g_fogR = (Uint)OverKartHeader.FogRGBA[0];
			g_fogG = (Uint)OverKartHeader.FogRGBA[1];
			g_fogB = (Uint)OverKartHeader.FogRGBA[2];
			gDPSetCycleType(GraphPtrOffset++, G_CYC_2CYCLE);
			gDPSetFogColor(GraphPtrOffset++, (Uint)OverKartHeader.FogRGBA[0], (Uint)OverKartHeader.FogRGBA[1], (Uint)OverKartHeader.FogRGBA[2], (Uint)OverKartHeader.FogRGBA[3]);
			gSPFogPosition(GraphPtrOffset++, OverKartHeader.FogStart, OverKartHeader.FogStop);
			gDPSetRenderMode(GraphPtrOffset++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE);
			gSPSetGeometryMode(GraphPtrOffset++, (G_FOG | G_SHADING_SMOOTH));
		}
		DisplayFlagGate(LocalCamera);
		if (SprintFinishArmed)
		{
			float SavedBanner[3];
			SavedBanner[0] = GoalBannerPos[0];
			SavedBanner[1] = GoalBannerPos[1];
			SavedBanner[2] = GoalBannerPos[2];
			GoalBannerPos[0] = SprintStartBanner[0];
			GoalBannerPos[1] = SprintStartBanner[1];
			GoalBannerPos[2] = SprintStartBanner[2];
			DisplayFlagGate(LocalCamera);
			GoalBannerPos[0] = SavedBanner[0];
			GoalBannerPos[1] = SavedBanner[1];
			GoalBannerPos[2] = SavedBanner[2];
		}
        gSPClearGeometryMode(GraphPtrOffset++, (G_FOG));
        
	}
}

#define	PHYS_TO_K1(x)	((u32)(x)|0xA0000000)	/* physical to kseg1 */
#define IO_READ(Addr)       (*(vu32 *)PHYS_TO_K1(Addr))
void DetectEmulator(void)
{
	Uint buff;


	if (IO_READ(0xA4100010) == 0)
	{
		EmulatorPlatform = 1;
		CartridgeStatus = 1;
		return;
	} // DPC_CLOCK_REG in Libultra
		

	// Fallback, harder emulator check.
	// The VI has an interesting quirk where its values are mirrored every 0x40 bytes
	// It's unlikely that emulators handle this, so we'll write to the VI_TEST_ADDR register and readback 0x40 bytes from its Address
	// If they don't match, we probably have an emulator
	buff = (*(Uint*)0xA4400038);
	(*(Uint*)0xA4400038) = 0x6ABCDEF9;
	if ((*(Uint*)0xA4400038) != (*(u32*)0xA4400078))
	{
		(*(Uint*)0xA4400038) = buff;
		EmulatorPlatform = 1;
		CartridgeStatus = 2;
		return;
	}

	(*(Uint*)0xA4400038) = buff;
	EmulatorPlatform = 0;
	CartridgeStatus = 3;
	return;
}


void RunDMA(void)
{
	DMA((u8 *) (uintptr_t) *TargetAddress, (u8 *) (uintptr_t) *SourceAddress, (size_t) DataLength);
}

void DMABase729A30Bypass(int Input, int Length, long Output)
{
	

	DMABase729A30(Input, Length, Output);
	return;
	
	// To-Do
	// This doesn't work
	// figure out how to make it work
	// there's other functions using 729A30 magic Number
	// get out your wizard kit potter.


	//Used for loading preview Textures.
	if (HotSwapID == 0)
	{
		if ((Input & 0x0A000000) == 0x0A000000)
		{
			DMABase729A30(Input, Length, Output);
		}
	}
	else
	{
		if ((Input & 0x0A000000) == 0x0A000000)
		{
			DMABase729A30(Input, Length, Output);
		}
		else
		{
			DMA((u8 *) (uintptr_t) Output, (u8 *) (uintptr_t) Input, (size_t) Length);
		}
	}
}

void RunRAM(void)
{
	ramCopy(*TargetAddress, *SourceAddress, DataLength);
}
int RunMIO(void)
{
	return (int) decodeMIO0((u8 *) (uintptr_t) *SourceAddress, (u8 *) (uintptr_t) *TargetAddress);
}
void RunTKM(void)
{
	decodeTKMK(*SourceAddress, TkmPoint, *TargetAddress, 0xBE);
}

void LoadEEPROM(Uint Destination)
{
	osEepromLongRead(&SIEventMesgQueue, 0, (u8 *) (uintptr_t) Destination, 512);
}
void SaveEEPROM(Uint Source)
{
	osEepromLongWrite(&SIEventMesgQueue, 0, (u8 *) (uintptr_t) Source, 512);
}



Ushort GetRGBA16(int R, int G, int B, int A)
{
	return (R & 0x1F)<<11 | (G & 0x1F)<<6 | (B & 0x1F)<<1 | (A & 0x01);
}

Uint GetRGB32(int R, int G, int B)
{
	return ( (R << 16) | (G << 8) | (B));
}

void SetFontColor(int FontR, int FontG, int FontB, int ShadowR, int ShadowG, int ShadowB)
{
	Ushort *ColorValues = (Ushort*)&g_DebugTextPalette;
	ColorValues[0] = 0; //0000 transparent
	ColorValues[1] = GetRGBA16(FontR,FontG,FontB,1);
	ColorValues[2] = GetRGBA16(ShadowR,ShadowG,ShadowB,1);
	ColorValues[3] = 1; //0001 black
}

void SetFontColorPalette(Uint Address, int FontR, int FontG, int FontB, int ShadowR, int ShadowG, int ShadowB)
{
	Ushort *ColorValues = (Ushort*)Address;
	ColorValues[0] = 0; //0000 transparent
	ColorValues[1] = GetRGBA16(FontR,FontG,FontB,1);
	ColorValues[2] = GetRGBA16(ShadowR,ShadowG,ShadowB,1);
	ColorValues[3] = 1; //0001 black
}

void LoadFontF3D(Uint Address)
{
	gSPDisplayList(GraphPtrOffset++, 0x0D008108);
	gSPDisplayList(GraphPtrOffset++, Address);
	gDPSetAlphaCompare	(GraphPtrOffset++ ,G_AC_THRESHOLD);
}

void SetupFontF3D(void)
{
	SetFontColorPalette((Uint)&RedTextPalette, 31, 11, 11, 14, 5, 5);
	SetFontColorPalette((Uint)&BlueTextPalette, 11, 11, 31, 5, 5, 14);
	SetFontColorPalette((Uint)&GreenTextPalette, 11, 31, 11, 5, 14, 5);
	SetFontColorPalette((Uint)&WhiteTextPalette, 30, 30, 30, 14, 14, 14);

	*SourceAddress = GetRealAddress((int) (uintptr_t) D_0D008080);
	DataLength = 0xB8;

	*TargetAddress = (Uint)&RedPaletteF3D;
	RunRAM();
	*TargetAddress = (Uint)&BluePaletteF3D;
	RunRAM();
	*TargetAddress = (Uint)&GreenPaletteF3D;
	RunRAM();
	*TargetAddress = (Uint)&WhitePaletteF3D;
	RunRAM();

	*(Uint *)((Uint)&RedPaletteF3D + 0x14) = (Uint)&RedTextPalette;
	*(Uint *)((Uint)&BluePaletteF3D + 0x14) = (Uint)&BlueTextPalette;
	*(Uint *)((Uint)&GreenPaletteF3D + 0x14) = (Uint)&GreenTextPalette;
	*(Uint *)((Uint)&WhitePaletteF3D + 0x14) = (Uint)&WhiteTextPalette;
}


int LoadOKData (Uint SourceInput, Uint SizeData)
{
	*SourceAddress = SourceInput;
	*TargetAddress = ObjectMemoryPointer;
	DataLength = SizeData;
	GlobalShortA = 16 - (SizeData % 16);
	ObjectMemoryPointer = ObjectMemoryPointer + SizeData + GlobalShortA;
	RunDMA();
	CheckCourseMemoryHighWater();
	return *TargetAddress;
}

int LoadDataBypass(Uint RomStart, Uint RomEnd)
{
	if (GameMode == GAMEMODE_GP)
	{
		if ((GPCourseIndex > AsmCupCount) && (HotSwapID > 0))
		{
			HotSwapGP = HotSwapID;
			HotSwapID = 0;
		}
	}
	
	*SourceAddress = RomStart;	
	DataLength = RomEnd - RomStart  ;
	GlobalShortA = 64 - ((RomEnd - RomStart) % 64);
	DataLength += GlobalShortA;
	LastMemoryPointer -= DataLength;
	*TargetAddress = LastMemoryPointer;
	RunDMA();
	return LastMemoryPointer;
}

int LoadPressDataBypass(Uint RomStart, Uint RomEnd)
{
	if (GameMode == GAMEMODE_GP)
	{
		if ((GPCourseIndex > AsmCupCount) && (HotSwapID > 0))
		{
			HotSwapGP = HotSwapID;
			HotSwapID = 0;
		}
	}
	*SourceAddress = RomStart;	
	DataLength = RomEnd - RomStart;		
	GlobalShortA = 64 - (DataLength % 64);
	DataLength += GlobalShortA;
	*TargetAddress = FreeMemoryPointer;
	RunDMA();

	
	DataLength = *(Uint*)(*TargetAddress + 4);  //Size of decompressed data stored in MIO0 header.	
	GlobalShortA = 64 - (DataLength % 64);
	DataLength += GlobalShortA;
	LastMemoryPointer -= DataLength;

	*SourceAddress = FreeMemoryPointer;
	*TargetAddress = LastMemoryPointer;	
	RunMIO();


	GlobalShortA = 64 - ((RomEnd - RomStart) % 64);
	FreeMemoryPointer += (RomEnd - RomStart) + GlobalShortA;
	return LastMemoryPointer;
}

void MakeVertex(Vtx *vtx, int n, short x, short y, short z, short tx, short ty, Uchar r, Uchar g, Uchar b, Uchar a)
{
	vtx[n].v.ob[0] = x;
	vtx[n].v.ob[1] = y;
	vtx[n].v.ob[2] = z;

	vtx[n].v.flag = 0;

	vtx[n].v.tc[0] = tx;
	vtx[n].v.tc[1] = ty;

	vtx[n].v.cn[0] = r;
	vtx[n].v.cn[1] = g;
	vtx[n].v.cn[2] = b;
	vtx[n].v.cn[3] = a;
}

/* Get Status Timer */

Uint GetStarTimer(Uchar PlayerID)
{
    return (int)GameTimer-StarUseCounter[PlayerID];
}

Uint GetBooTimer(Uchar PlayerID)
{
    return (int)GameTimer-GhostUseCounter[PlayerID];
}

//Returns custom course Number
short CustomLevelID(void)
{
    if (HotSwapID == 0)
    {
        return -1;
    }
    else
    {
		switch (CupSelect)
		{
		case 0:
			return (CourseSelect + (16 * (HotSwapID-1)));
			break;
		case 1:
			return ((CourseSelect+4) + (16 * (HotSwapID-1)));
			break;
		case 2:
			return ((CourseSelect+8) + (16 * (HotSwapID-1)));
			break;
		case 3:
			return ((CourseSelect+12) + (16 * (HotSwapID-1)));
			break;
		default:
			return -1;
			break;
		}
        
    }
}




void LoadBigFont(void)
{
#if AMPEDUP_FONT == 1
	*SourceAddress = (int)(&AmpedUpFont);
	DataLength = (int)(&AmpedUpFontEnd) - (int)(&AmpedUpFont);
#else
	*SourceAddress = (int)(&LibraryFont);
	DataLength = (int)(&LibraryFontEnd) - (int)(&LibraryFont);
#endif
	DataLength = (DataLength + 15) & ~15;
	*TargetAddress = (int)(&ok_FreeSpace);
	RunDMA();
	*SourceAddress = (int)(&ok_FreeSpace);
	*TargetAddress = (int)(&nicefont);
	RunMIO();
}

void ShiftVertColor(Uint Address,Uint Counter,Uint v_buff,char Alpha,char red,char Green,char Blue)
{
	Uint Number= SegmentNumber(Address);
	Uint Offset= SegmentOffset(Address);
	Vtx_t  *vt=(Vtx_t *)GetRealAddress(SegmentTable[Number]+Offset);
	Uint i;
	for(i=0;i<Counter;i++)
	{
		vt->cn[0]+=red;
		vt->cn[1]+=Green;
		vt->cn[2]+=Blue;		
		vt->cn[3]+=Alpha;
		++vt;
	}	
}

int GetRealAddress(int RSPAddress)
{
	return (int) SegmentToVirtual((const void *) (uintptr_t) RSPAddress);
}


void DrawBox(int X, int Y, int SizeX, int SizeY, int R, int G, int B, int A)
{
	GraphPtr = FillRect1ColorF(GraphPtr, X, Y, X + SizeX, Y + SizeY, R, G, B, A);
}



int GetIntLength(int Input)
{
	if (Input < 0)
	{
		Input *= -1;
	}
	if (Input < 10) return 1;
	if (Input < 100) return 2;
	if (Input < 1000) return 3;
	if (Input < 10000) return 4;
	if (Input < 100000) return 5;
	if (Input < 1000000) return 6;
	if (Input < 10000000) return 7;
	if (Input < 100000000) return 8;
	if (Input < 1000000000) return 9;
	return 0;
}
void PrintDecimal(int X, int Y, float Value, int Length)
{
	int PrintOffset = 0;
	int PrintLength = 10;
	{
	    int Loop;
	for (Loop = 1; Loop < Length; Loop++)
	{
		PrintLength *= 10;	
	}
	}
	

	WholeNumber = (int) Value;
	DecimalNumber = (int) ((Value - WholeNumber) * PrintLength);
	if (DecimalNumber > (float)((Value - WholeNumber) * PrintLength))
	{
		DecimalNumber--;
	}

	PrintOffset = 8 * GetIntLength(WholeNumber);

	if (Value < 0)
	{
     	X -= 8;
		PrintOffset += 8;
		DecimalNumber *= -1;
	}
	PrintStringNumber(X,Y,"",WholeNumber);
	PrintString((X + PrintOffset + 10), Y, ".");		
	PrintOffset += 8;
	if ((DecimalNumber < (PrintLength / 10)) && (DecimalNumber != 0))
	{
		PrintStringNumber((X+PrintOffset),Y,"",0);
		PrintOffset += 8;
	}
	PrintStringNumber((X+PrintOffset),Y,"",DecimalNumber);
     
}




#define Rad2Deg(Rad) (Rad * 180 / M_PI)
Ushort CalcVerticalDirection(Vector Origin,Vector Target)
{
	float ux,uy,uz,dist;
	

	ux =	Target[0] - Origin[0];
	uy = 	Target[1] - Origin[1];
	uz =	Target[2] - Origin[2];

	dist = Sqrtf((ux*ux)+(uz*uz));
	
	return ((Atan2t(dist,uy)));
}

void MakeAlignVectorX(Vector Input, short RotX)
{
    float sine  = SinT(RotX);
       float cosine = CosT(RotX);
    float v0=Input[0];
    float v1=Input[1];
    float v2=Input[2];
    
     Input[0]=v0;
    Input[1]=sine*v2+cosine*v1;
    Input[2]=cosine*v2-sine*v1;
}


void MakeAlignVectorZ(Vector Input, short RotZ)
{
    float sine  = SinT(RotZ);
       float cosine = CosT(RotZ);
    float v0=Input[0];
    float v1=Input[1];
    float v2=Input[2];
    
     Input[0]=cosine*v0+sine*v1;
    Input[1]=-sine*v0+cosine*v1;
    Input[2]=v2;
}

void PrintFloat(int X, int Y, float Value)
{
	PrintDecimal(X,Y,Value,2);	
}

void ResetObject(void)
{
	/*
	{
	    int thisLoop;
	for (thisLoop = 0; thisLoop < 4; thisLoop++)
	{
		{
		    int subLoop;
		for (subLoop = 0; subLoop < 4; subLoop++)
		{
			AffineMatrix[thisLoop][subLoop] = 0;
		}
		}
	}
	}
	*/
	{
	    int thisLoop;
	for (thisLoop = 0; thisLoop < 3; thisLoop++)
	{
		ObjectPosition[thisLoop] = 0;
		ObjectAngle[thisLoop] = 0;
		ObjectVelocity[thisLoop] = 0;
	}
	}
}




char* PrintHex(char *buf, int num, int nDigits) {
    //print Hex Number into Buffer.
    //will zero-pad to specified Number of digits.
    //will truncate numbers larger than specified Length.
    //returns Pointer to Null terminator.
    char *bufEnd = &buf[nDigits];
    *bufEnd = 0;
    while(nDigits--) {
        buf[nDigits] = Hex[num & 0xF];
        num >>= 4;
    }
    return bufEnd;
}

					    	//x,0,1,2,3,4,5,6,7
char CharacterConvert[9] = 	{-1,0,1,6,3,2,4,5,7};
char CharacterUnconvert[9] = 	{-1,0,1,4,3,5,6,2,7};

//WaveDir = Address to a stored Ushort for the calculation
//WaveSpeed = Additional Value (degree)
void RunWaveSpeed(Ushort *WaveDir,short WaveSpeed)
{
	*(Ushort*)WaveDir += WaveSpeed;
}

//WaveXScale = Width of the wave - horizontally
//WaveYThin = Higher Value means less Distance between waves (degree) - vertically
//WaveDir = Variable that gets calculated by RunWaveSpeed
void SpriteDrawWave(int cx,int cy,Ushort *Addr,int sizex,int sizey,float WaveXScale,short WaveYThin,Ushort WaveDir)
{
	short Angle;
	short angstep = WaveYThin;
	int intsx = cx - sizex / 2;
	int sy = cy - sizey / 2;

	int	sx;
	Ushort *tmpaddr;

	Angle = WaveDir;
	tmpaddr = Addr;

	{
	    int i;
	for (i = 0; i < sizey; i++)
	{
		KWLoadTextureBlockRGBA16B(tmpaddr+sizex*i,sizex,1);
		sx = (int)(intsx + (WaveXScale) * SinT(Angle+angstep*i));
		SPRDrawClip(sx,sy,sizex,1,1);
		sy++;
	}
	}
}

Ushort CustomCheckBump2(Bump* Bump, float Radius, float px, float py, float pz, float lastx, float lasty, float lastz)
{
	Ushort tmp = CheckBump2(Bump, Radius, px, py, pz, lastx, lasty, lastz);
	
	if (tmp != 0)
	{
		InteractLavaFloor(Bump, Bump->LastZX, tmp);
	}
	return tmp;
}

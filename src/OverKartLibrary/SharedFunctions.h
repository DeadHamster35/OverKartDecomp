#ifndef SharedFunctionsH
#define SharedFunctionsH
#include "MainInclude.h"


#define SIZE_OF_ARRAY(array) (sizeof(array)/sizeof((array)[0]))

#define min(a, b) ((a) <= (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#define sqr(x) ((x) * (x))
#define PythagoreanTheorem(x1, x2, z1, z2) (sqr((x1)-(x2)) + sqr((z1)-(z2)))


#define SET_FLAG(n, f) ((n) |= (f)) 
#define CLR_FLAG(n, f) ((n) &= ~(f)) 
#define TGL_FLAG(n, f) ((n) ^= (f)) 
#define CHK_FLAG(n, f) ((n) & (f))
#define GetRGBA16Inline(R,G,B,A) (short)((R & 0x1F)<<11 | (G & 0x1F)<<6 | (B & 0x1F)<<1 | (A & 0x01))
#define CHARS2SHORT(charA, charB) 				(charA & 0xFF)<<8 | (charB & 0xFF)
#define CHARS2INT(charA, charB, charC, charD) 	(charA & 0xFF)<<24 | (charB & 0xFF)<<16 | (charC & 0xFF)<<8 | (charD & 0xFF)
#define SHORTS2INT(shortA, shortB) 				(shortA & 0xFF)<<16 | (shortB & 0xFF)

#define PythagoreanTheorem(x1, x2, z1, z2) (sqr((x1)-(x2)) + sqr((z1)-(z2)))
#define  deg(n)       ((short)(65536/360*n))






extern void DisplayFlagGateCheck(Camera* LocalCamera);


extern void DisplayObject(void* car, Object* inputObject);
extern void CollideObject(Player* Car, Object* Target);
extern void ItemboxCollideCheck(Player* Car, Object* Target);
extern void DrawPerScreen(Camera* LocalCamera);
extern void TitleMenu(void);
extern void MiniMapDraw(void);
extern void GameCode(void);
extern void AllRun(void);
extern void PrintMenuFunction(void);
extern void ExecuteItemHook(Player* Car);


extern int LoadOKData(Uint SourceInput, Uint SizeData);
extern int LoadDataBypass(Uint RomStart, Uint RomEnd);
extern int LoadPressDataBypass(Uint RomStart, Uint RomEnd);


extern void DetectEmulator(void);

extern void RunDMA(void);
extern void RunRAM(void);
extern void RunTKM(void);
extern int RunMIO(void);
extern void LoadEEPROM(Uint Destination);
extern void SaveEEPROM(Uint Source);
extern Uint GetStarTimer(Uchar PlayerID);
extern Uint GetBooTimer(Uchar PlayerID);

extern short CustomLevelID(void);

extern Ushort GetRGBA16(int R, int G, int B, int A);
extern Uint GetRGB32(int R, int G, int B);

extern void SetFontColor(int FontR, int FontG, int FontB, int ShadowR, int ShadowG, int ShadowB);
extern void LoadFontF3D(Uint Address);
extern void SetupFontF3D(void);
extern void PrintDecimal(int X, int Y, float Value, int Length);
extern void PrintFloat(int X, int Y, float Value);
extern int GetRealAddress(int RSPAddress);

extern void LoadBigFont(void);

extern Ushort CalcVerticalDirection(Vector origin,Vector object);
void MakeAlignVectorX(Vector Input, short RotX);
void MakeAlignVectorZ(Vector Input, short RotX);

extern void ResetObject(void);
extern void DrawBox(int X, int Y, int SizeX, int SizeY, int R, int G, int B, int A);
extern char* PrintHex(char *buf, int num, int nDigits);
extern void PrintNumberSprite(int X, int Y, int Value);
extern void ShiftVertColor(Uint Address,Uint Counter,Uint v_buff,char Alpha,char red,char Green,char Blue);
extern void MakeVertex(Vtx *vtx, int n, short x, short y, short z, short tx, short ty, Uchar r, Uchar g, Uchar b, Uchar a);

extern int ReturnStringLength(char *stringAddress);
extern char CharacterConvert[];
extern char CharacterUnconvert[];

extern void RunWaveSpeed(Ushort *WaveDir,short WaveSpeed);
extern void SpriteDrawWave(int cx,int cy,Ushort *Addr,int sizex,int sizey,float WaveScale,short WaveAngleStep,Ushort WaveDirAddr);
extern Ushort CustomCheckBump2(Bump* Bump, float Radius, float px, float py, float pz, float lastx, float lasty, float lastz);
#endif

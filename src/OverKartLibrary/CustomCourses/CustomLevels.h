#ifndef CustomLebelsH
#define CustomLevelsH
#include "../MainInclude.h"

extern unsigned char ScaleXMode, ScaleYMode, ScaleZMode, ScalePad;
extern float LevelScales[7];
extern short ZFLIP;
extern short YFLIP;
extern short BattleSongID;
extern void FPS_Check(void);
extern void DynamicTempo(void);
extern void StaticTempo(int Tempo);
extern void stockASM(void);
extern void overkartASM(void);
extern void runTextureScroll(void);
extern void runWaterVertex(void);

extern void DecodeSP1Triangle_OK(Gfx *gfx,unsigned char *pp,char code );
extern void DecodeSP2Triangle_OK(Gfx *gfx,unsigned char *pp,char code );
extern void DecodeSP1Quadrangle_OK(Gfx *gfx,unsigned char *pp,char code );
extern void DecodeVertex2_OK(char *ramaddress, uint number);

extern void CheckFinishFix(void);
extern void runDisplayScreen(void);
extern void runKillDisplayObjects(void);
extern void loadTextureScrollTranslucent(void);
extern void copyCourseTable(int copyMode);
extern void hsTableSet(void);
extern void stockTableSet(void);
extern void setSong(void);
extern void setPath(void);
extern void setEcho(void);
extern void setSky(void);
extern void setWater(void);
extern void loadHeaderOffsets(void);
extern void LoadCustomHeader(int inputID);
extern void SetCustomData(void);
extern void loadMinimap(void);
extern void loadOKObjects(void);
extern void setOKObjects(void);
extern void SetGhostData(void);
extern void SnowCustomCheck(int SnowIndex);
extern void setBanners(void);
extern void setPreviews(void);
extern void swapHS(int direction);
extern void previewRefresh(void);
extern void SetCourseNames(bool custom);
extern void initFireParticles(long PathOffset);
extern void DisplayFireParticleSub(int num,uchar color,void* Camera);
extern void SetWeatherType(char WeatherType);
extern void SetCloudType(char CloudType);
extern void SetWeather3D(bool Weather3DEnable);
extern void Snow3DCameralook(int num, void* Camera);
extern void EventDisplay(int player);
extern void EventDisplay_After(int player);
extern void CommonGameEventChart(void);
extern void EmptyActionData(void);
extern Gfx* IceKageGfx;
extern void LoadIceKage(void);
extern void LoadBomb(void);
extern void MapStartupDefault(short InputID);
extern void InitialMapCode(void);
extern void InitialMapObjectCode(void);
extern int LoadPressDataBypass(unsigned int RomStart, unsigned int RomEnd);
extern int LoadDataBypass(unsigned int RomStart, unsigned int RomEnd);
extern void SearchListHook(unsigned int Addr);
extern void SearchListFileHook(unsigned int addr);
extern void SearchList2Hook(unsigned int addr, char Surface);
extern void KWKumo_Alloc_Hook_Default(void);
extern void KWChart_Kumo_Hook_Default(int screen_num);
extern void DisplayKT1Hook(Screen* Display);
extern void DisplayKT16Hook(Screen* Display);
extern void XLUDisplay(Screen* Display);
extern short RadarLineX, RadarLineY;
extern void RadarFinishLine(uint ulx, uint uly, ushort *addr);
extern float HijackScaleMinimapX(float x);
extern float HijackScaleMinimapY(float z);
#endif

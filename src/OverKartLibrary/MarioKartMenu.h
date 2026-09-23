#ifndef MarioKartMenuH
#define MarioKartMenuH
#include "MainInclude.h"
extern void PrintMap(int devParameter);
extern void PrintDetails(void);
extern void PrintGPTime(float printTime, int OffsetY);
extern void printTime(float printTime);
extern void PrintMenuTest(void);
extern void MapSelectMenuDefault(void);
extern void DrawMapSelectDefault(void);


extern void LoadHudButtons(void);
extern void SpriteBtnA(int Posx, int Posy, float Scale, bool pressed);
extern void SpriteBtnB(int Posx, int Posy, float Scale, bool pressed);
extern void SpriteBtnStart(int Posx, int Posy, float Scale, bool pressed);
extern void SpriteBtnCUp(int Posx, int Posy, float Scale, bool pressed);
extern void SpriteBtnCDown(int Posx, int Posy, float Scale, bool pressed);
extern void SpriteBtnCLeft(int Posx, int Posy, float Scale, bool pressed);
extern void SpriteBtnCRight(int Posx, int Posy, float Scale, bool pressed);
extern void SpriteBtnL(int Posx, int Posy, float Scale, bool pressed);
extern void SpriteBtnR(int Posx, int Posy, float Scale, bool pressed);
extern void SpriteBtnZ(int Posx, int Posy, float Scale, bool pressed);
extern void SpriteBtnDUp(int Posx, int Posy, float Scale, bool pressed);
extern void SpriteBtnDDown(int Posx, int Posy, float Scale, bool pressed);
extern void SpriteBtnDLeft(int Posx, int Posy, float Scale, bool pressed);
extern void SpriteBtnDRight(int Posx, int Posy, float Scale, bool pressed);
extern void SpriteBtnDMid(int Posx, int Posy, float Scale);
extern void SpriteAnalogStick(int Posx, int Posy, float Scale, char Direction);
extern void ModularMenu(OKMenu* OptionsMenu);
extern void CourseMenu(int Alpha);

extern void PrintNiceText(int Posx, int Posy, float Scale, char *text);
extern void PrintBigText(int Posx, int Posy, float Scale, char *text);
extern void PrintNiceTextNumber(int Posx, int Posy, float Scale, char *text, int Value);
extern void PrintBigTextNumber(int Posx, int Posy, float Scale, char *text, int Value);
extern void PrintBigTextNumberNoGap(int Posx, int Posy, float Scale, char *text, int Value);
extern void DrawFPS(int X, int Y);
extern void DrawLapCounter(void);
extern void KWDisplayInit(void);
#endif

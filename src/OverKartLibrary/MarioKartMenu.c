#include "MainInclude.h"








void PrintMenuTest(void)
{

}

void PrintGPTime(float printTime, int OffsetY)
{




     WholeNumber = (int) printTime;
     DecimalNumber = (int) ((printTime - WholeNumber) * 100);



     if (DecimalNumber < 0)
     {
          DecimalNumber = DecimalNumber * -1;
     }

     if (WholeNumber > 60)
     {
          Minutes = WholeNumber / 60;
          Seconds = WholeNumber - (Minutes * 60);
     }
     else
     {
          Seconds = WholeNumber;
     }

     if (Minutes >= 10)
     {
          if (Minutes >= 100)
          {
               if (Minutes >= 1000)
               {

                    PrintOffsetA = 40;
               }
               else
               {
                    PrintOffsetA = 32;
               }
          }
          else
          {
               PrintOffsetA = 24;
          }
     }
     else
     {
          PrintOffsetA = 16;
     }

     if (Seconds >= 10)
     {
          if (Seconds >= 100)
          {
               if (Seconds >= 1000)
               {

                    PrintOffsetB = 40;
               }
               else
               {
                    PrintOffsetB = 32;
               }
          }
          else
          {
               PrintOffsetB = 24;
          }
     }
     else
     {
          PrintOffsetB = 16;
     }



     PrintOffsetB = PrintOffsetB + PrintOffsetA;


     LoadFont();
     MenuPosition[1] = 170+(int)OffsetY;
     MenuPosition[0] = 55;

     PrintString(MenuPosition[0],MenuPosition[1],"Total Time:");

     MenuPosition[0] = 145;


     PrintString(MenuPosition[0] + PrintOffsetA, MenuPosition[1], "'");
     if (Seconds < 10)
     {
          PrintStringNumber(MenuPosition[0] + PrintOffsetA,MenuPosition[1],"",0);
          PrintOffsetA = PrintOffsetA + 8;
          PrintOffsetB = PrintOffsetB + 8;
     }

     PrintString(MenuPosition[0] + PrintOffsetB, MenuPosition[1], "\"");
     if (DecimalNumber < 10)
     {
          PrintStringNumber(MenuPosition[0]+PrintOffsetB,MenuPosition[1],"",0);
          PrintOffsetB = PrintOffsetB + 8;
     }




     PrintStringNumber(MenuPosition[0],MenuPosition[1],"",Minutes);
     PrintStringNumber(MenuPosition[0] + PrintOffsetA,MenuPosition[1],"",Seconds);
     PrintStringNumber(MenuPosition[0]+PrintOffsetB,MenuPosition[1],"",DecimalNumber);



}



void PrintMap(int devParameter)
{
     
     MenuPosition[0] = 25;
     MenuPosition[1] = 150;
     GraphPtr = FillRect1ColorF(GraphPtr, MenuPosition[0] + 18, MenuPosition[1] + 18, MenuPosition[0] + 185, MenuPosition[1] + 68, 0, 0, 0, 175);
     GraphPtr = FillRect1ColorF(GraphPtr, MenuPosition[0] + 18, MenuPosition[1] + 18 + (devParameter * 10), MenuPosition[0] + 74, MenuPosition[1] + 28 + (devParameter * 10), 200, 0, 0, 175);
     LoadFont();


     PrintStringNumber(MenuPosition[0],MenuPosition[1],"  Map XY:",MapX[0]);
     PrintStringNumber(MenuPosition[0]+112,MenuPosition[1],"",MapY[0]);
     PrintStringNumber(MenuPosition[0],MenuPosition[1]+10,"Start XY:",StartX);
     PrintStringNumber(MenuPosition[0]+112,MenuPosition[1]+10,"",StartY);
     PrintString(MenuPosition[0],MenuPosition[1]+20,"   Scale:");               
     PrintFloat(MenuPosition[0] + 72,MenuPosition[1]+20,(MapScale * 100.0f));
     PrintStringNumber(MenuPosition[0],MenuPosition[1]+30,"Line XY:",RadarLineX);
     PrintStringNumber(MenuPosition[0]+112,MenuPosition[1]+30,"",RadarLineY);
               

     

}


void CourseMenu(int Alpha)
{
     short *l_courseID;
     
     DrawBox(50,10,220,121,0,0,0,Alpha);
     
     DrawBox(48,8,2,124,255,0,0,255);
     DrawBox(270,8,2,124,255,0,0,255);
     DrawBox(50,8,220,2,255,0,0,255);
     DrawBox(50,130,220,2,255,0,0,255);
     DrawBox(60,32,200,1,0,0,0,255);
     KWSprite(80,22,16,16,(Ushort*)&lit_arrowsprite_l);
     
     

     if (ParameterIndex == 0) //currentParameter
     {
          MenuPosition[0] = 157 -(10 * 4);
          GraphPtr = FillRect1ColorF(GraphPtr, MenuPosition[0], 19, MenuPosition[0] + (10 * 8), 29, 200, 0, 0, 200);
     }
     else if (ParameterIndex == 1) //currentParameter
     {
          MenuPosition[0] = 155 - ((CupChar[MenuCup]) * 4);
          GraphPtr = FillRect1ColorF(GraphPtr, MenuPosition[0], 41, MenuPosition[0] + (CupChar[MenuCup] * 8), 51, 0, 200, 0, 200);
     }
     else
     {
          if (SYSTEM_Region == 0x00)
          {
               GlobalAddressA = (cup_PAL + (MenuCup * 8) + ((ParameterIndex - 2) * 2));
          }
          else
          {
               GlobalAddressA = (cup_NTSC + (MenuCup * 8) + ((ParameterIndex - 2) * 2));
          }
          l_courseID = (short *)GlobalAddressA;
          MenuPosition[0] = 157 - ((CourseChar[(long)*l_courseID]) * 4);
          MenuPosition[1] = ((ParameterIndex - 1) * 14) + 44;
          GraphPtr = FillRect1ColorF(GraphPtr, MenuPosition[0], MenuPosition[1], MenuPosition[0] + (CourseChar[(long)*l_courseID] * 8), MenuPosition[1]+11, 0, 0, 200, 200);
     }
     LoopValue = 0;
     LoadFont();
     PrintString(98,0,"Cup Editor");
     MenuPosition[0] = 135 - (CupChar[MenuCup] * 4);
     PrintString(MenuPosition[0],22,CupNames[MenuCup]);
     MenuPosition[1] = 40;
     do{
          if (SYSTEM_Region == 0x00)
          {
               GlobalAddressA = (cup_PAL + (MenuCup * 8) + LoopValue * 2);
          }
          else
          {
               GlobalAddressA = (cup_NTSC + (MenuCup * 8) + LoopValue * 2);
          }
          l_courseID = (short *)GlobalAddressA;
          MenuPosition[0] = 138 - (CourseChar[(long)*l_courseID] * 4);
          PrintString(MenuPosition[0],MenuPosition[1],CourseNames[(long)*l_courseID]);
          MenuPosition[1] = MenuPosition[1] + 14;
          LoopValue++;
     } while (LoopValue < 4);
}

float FPSA, FPSB;
void DrawFPS(int X, int Y)
{     
    if (GlobalFrameCount % 5 == 0)
    {
        FPSA = (1 * CPU2SEC) / CycleCount[0];
        FPSB = (1 * CPU2SEC) / CycleCount[1];
    }
    LoadFont();
    PrintDecimal(X,Y,FPSA, 2);         
    PrintDecimal(X,Y + 10,FPSB, 2); 
}




void ModularMenu(OKMenu* OptionsMenu)
{
     if (MenuBlink > 30)
     {
          MenuBlink = 0;
     }
     else
     {
          MenuBlink++;
     }
     
     DrawBox(30,10,260,200,0,0,0,200);
     
     DrawBox(28,8,2,203,255,0,0,255);
     DrawBox(290,8,2,203,255,0,0,255);
     DrawBox(30,8,260,2,255,0,0,255);
     DrawBox(30,209,260,2,255,0,0,255);
     DrawBox(40,32,240,1,0,0,0,255);

     
     if (ParameterIndex == 0) //currentParameter
     {
          MenuPosition[0] = 158 - ((OptionsMenu->PanelAddress[MenuIndex].NameLength) * 4);
          GraphPtr = FillRect1ColorF(GraphPtr, MenuPosition[0], 19, MenuPosition[0] + ((OptionsMenu->PanelAddress[MenuIndex].NameLength) * 8), 29, 200, 0, 0, 200);
     }
     else
     {
          MenuPosition[1] = ParameterIndex * 19 + 32;          
          if (MenuBlink < 29)  
          {
               KWSprite(38,MenuPosition[1]+2,16,16,(Ushort*)&lit_red_selecter);
          }
     }

     LoopValue = 0;
     MenuPosition[0] = 138 - ((OptionsMenu->PanelAddress[MenuIndex].NameLength) * 4);
     
     
     
     LoadFont();
     SetFontColor(255,255,255,75,75,75);
     
     PrintString(MenuPosition[0],0,(char*)OptionsMenu->PanelAddress[MenuIndex].NameAddress);
     
     MenuPosition[1] = 30;
     
     if (OptionsMenu->PanelAddress[MenuIndex].OptionCount > 7)
     {
          GlobalShortB = 7;
     }
     else
     {
          GlobalShortB = OptionsMenu->PanelAddress[MenuIndex].OptionCount;
     }
     for (LoopValue = 0; LoopValue < GlobalShortB; LoopValue++)
     {
          OKOption* ThisOption = (OKOption*)(&OptionsMenu->PanelAddress[MenuIndex].Options[LoopValue + (long)MenuOverflow]);          
          PrintString(30,MenuPosition[1],(char*)ThisOption->OptionName);
          GlobalShortA = (int)OptionsMenu->PanelAddress[MenuIndex].ParameterToggles[LoopValue + (long)MenuOverflow];
          MenuPosition[0] = 220 - (ThisOption->ParameterLengths[GlobalShortA] * 4);          
          PrintString(MenuPosition[0],MenuPosition[1],(char*)ThisOption->ParameterNames[GlobalShortA]);
          MenuPosition[1] = MenuPosition[1] + 19;     
     } 
     
     if ((MenuOverflow + 7) < OptionsMenu->PanelAddress[MenuIndex].OptionCount)  //menuOverflowIndex
     {
          if (MenuBlink < 15)  //Used for blinking down arrow
          {
               KWSprite(161,190,16,16,(Ushort*)&lit_arrowsprite_d);
          }
     }
     if (MenuOverflow > 0)  //menuOverflowIndex
     {
          if (MenuBlink < 15)  //Used for blinking down arrow
          {
               KWSprite(161,40,16,16,(Ushort*)&lit_arrowsprite_u);
          }
     }
     if (MenuIndex > 0)  //Used for Left arrow
     {
          KWSprite(80,22,16,16,(Ushort*)&lit_arrowsprite_l);
     }
     if (MenuIndex < (int)OptionsMenu->PanelCount)  //Used for Right arrow
     {
          KWSprite(240,22,16,16,(Ushort*)&lit_arrowsprite_r);
     }
}






int NumPlaces (int n) 
{
    if (n < 0) return 0;
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    if (n < 10000) return 4;
    if (n < 100000) return 5;
    if (n < 1000000) return 6;
    if (n < 10000000) return 7;
    if (n < 100000000) return 8;
    return 9;
}

void DrawNumberSprite(int x, int y, int Number)
{
	SprDrawClipST(x,y,8,16,Number*8,0,1);
}

//Prints a Value with Right-handed Number sprites
void PrintNumberSprite(int X, int Y, int Value)
{
	char negativeVal;
	int digit[9];
	int Index;

	KWSprite(1000,1000,104,16,StockNumberSprites);

	negativeVal = 0;

	if (Value < 0)
	{
		Value = Value*-1;
		negativeVal = 1;
	}

	digit[0] = ((Value%10));
	digit[1] = ((Value%100)/10);
	digit[2] = ((Value%1000)/100);
	digit[3] = ((Value%10000)/1000);
	digit[4] = ((Value%100000)/10000);
	digit[5] = ((Value%1000000)/100000);
	digit[6] = ((Value%10000000)/1000000);
	digit[7] = ((Value%100000000)/10000000);
	digit[8] = ((Value%1000000000)/100000000);

	for (Index = 0; Index < NumPlaces(Value); Index++)
	{
		DrawNumberSprite(X-9*Index, Y, digit[Index]);
		if (negativeVal == 1)
		{
			DrawNumberSprite(X-9*(NumPlaces(Value)), Y+4, 11);
			DrawNumberSprite(X-9*(NumPlaces(Value)), Y+4, 10);
			negativeVal = 0;
		}
	}
}

int ReturnStringLength(char *stringAddress)
{
	GlobalIntA = 0;
	GlobalCharA = *(char*)stringAddress;
	if(GlobalCharA != 0)
	{
		do
		{
			++GlobalIntA;
			GlobalCharA = (*(char*)(stringAddress + GlobalIntA));
		}
		while (GlobalCharA != 0);
	}
	return(GlobalIntA);
}



///////////////HUD Buttons///////////////

void LoadHudButtons(void)
{
     *SourceAddress = (int)(&HudButtonsROM);
     *TargetAddress = (int)(&ok_FreeSpace);
     DataLength = 0x4400;
     RunDMA();
     *SourceAddress = (int)(&ok_FreeSpace);
     *TargetAddress = (int)(&hud_buttons);
     RunMIO();
}


void SpriteBtnA(int Posx, int Posy, float Scale, bool pressed)
{
     switch (pressed)
     {
     case true:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x100));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x100),16,16);
          }
          break;
     
     default:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,hud_buttons);
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,hud_buttons,16,16);
          }
          break;
     }
}

void SpriteBtnB(int Posx, int Posy, float Scale, bool pressed)
{
     switch (pressed)
     {
     case true:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x300));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x300),16,16);
          }
          break;
     
     default:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x200));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x200),16,16);
          }
          break;
     }
}

void SpriteBtnStart(int Posx, int Posy, float Scale, bool pressed)
{
     switch (pressed)
     {
     case true:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x500));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x500),16,16);
          }
          break;
     
     default:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x400));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x400),16,16);
          }
          break;
     }
}

void SpriteBtnCUp(int Posx, int Posy, float Scale, bool pressed)
{
     switch (pressed)
     {
     case true:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x700));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x700),16,16);
          }
          break;
     
     default:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x600));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x600),16,16);
          }
          break;
     }
}

void SpriteBtnCDown(int Posx, int Posy, float Scale, bool pressed)
{
     switch (pressed)
     {
     case true:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x900));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x900),16,16);
          }
          break;
     
     default:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x800));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x800),16,16);
          }
          break;
     }
}

void SpriteBtnCLeft(int Posx, int Posy, float Scale, bool pressed)
{
     switch (pressed)
     {
     case true:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0xB00));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0xB00),16,16);
          }
          break;
     
     default:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0xA00));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0xA00),16,16);
          }
          break;
     }
}

void SpriteBtnCRight(int Posx, int Posy, float Scale, bool pressed)
{
     switch (pressed)
     {
     case true:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0xD00));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0xD00),16,16);
          }
          break;
     
     default:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0xC00));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0xC00),16,16);
          }
          break;
     }
}

void SpriteBtnL(int Posx, int Posy, float Scale, bool pressed)
{
     switch (pressed)
     {
     case true:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0xF00));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0xF00),16,16);
          }
          break;
     
     default:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0xE00));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0xE00),16,16);
          }
          break;
     }
}

void SpriteBtnR(int Posx, int Posy, float Scale, bool pressed)
{
     switch (pressed)
     {
     case true:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x1100));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x1100),16,16);
          }
          break;
     
     default:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x1000));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x1000),16,16);
          }
          break;
     }
}

void SpriteBtnZ(int Posx, int Posy, float Scale, bool pressed)
{
     switch (pressed)
     {
     case true:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x1300));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x1300),16,16);
          }
          break;
     
     default:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x1200));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x1200),16,16);
          }
          break;
     }
}


void SpriteBtnDUp(int Posx, int Posy, float Scale, bool pressed)
{
     switch (pressed)
     {
     case true:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x1500));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x1500),16,16);
          }
          break;
     
     default:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x1400));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x1400),16,16);
          }
          break;
     }
}

void SpriteBtnDDown(int Posx, int Posy, float Scale, bool pressed)
{
     switch (pressed)
     {
     case true:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x1700));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x1700),16,16);
          }
          break;
     
     default:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x1600));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x1600),16,16);
          }
          break;
     }
}

void SpriteBtnDLeft(int Posx, int Posy, float Scale, bool pressed)
{
     switch (pressed)
     {
     case true:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x1900));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x1900),16,16);
          }
          break;
     
     default:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x1800));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x1800),16,16);
          }
          break;
     }
}

void SpriteBtnDRight(int Posx, int Posy, float Scale, bool pressed)
{
     switch (pressed)
     {
     case true:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x1B00));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x1B00),16,16);
          }
          break;
     
     default:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x1A00));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x1A00),16,16);
          }
          break;
     }
}

void SpriteBtnDMid(int Posx, int Posy, float Scale)
{

     if (Scale == 1 || Scale == 0)
     {
          KWSprite(Posx,Posy,16,16,(hud_buttons+0x1C00));
     }
     else
     {
          KWSprite(0,0,0,0,hud_buttons);
          KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x1C00),16,16);
     }
}

//Direction: 0-Mid; 1-Up; 2-Down; 3-Left; 4-Right
void SpriteAnalogStick(int Posx, int Posy, float Scale, char Direction)
{
     switch (Direction)
     {
     case 1: //UP
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x1E00));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x1E00),16,16);
          }
          break;
     case 2: //DOWN
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x1F00));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x1F00),16,16);
          }
          break;
     case 3: //LEFT
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x2000));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x2000),16,16);
          }
          break;
     case 4: //RIGHT
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x2100));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x2100),16,16);
          }
          break;	
     default:
          if (Scale == 1 || Scale == 0)
          {
               KWSprite(Posx,Posy,16,16,(hud_buttons+0x1D00));
          }
          else
          {
               KWSprite(0,0,0,0,hud_buttons);
               KWSpriteScale(Posx,Posy,Scale,(hud_buttons+0x1D00),16,16);
          }
          break;
     }
}

void PrintNiceText(int Posx, int Posy, float Scale, char *text)
{
	char *FontString = text;


	{
	    int i;
	    unsigned char ch;
	for (i = 0; i < ReturnStringLength(text); i++)
	{
		ch = (unsigned char)text[i];
		if (ch < 33)
		{
			continue;
		}

		if (ch > 127)
		{
			continue;
		}
		if (ch >= 97 && ch <= 122)
		{
			if (Scale == 1 || Scale == 0)
			{
				KWSprite(Posx+i*7,Posy,8,16,(nicefont+0x80*(FontString[i]-64)-0x80));
			}
			else
			{
				KWSprite(0,0,0,0,nicefont);
				KWSpriteScale(Posx+i*8*Scale,Posy,Scale,(nicefont+0x80*(FontString[i]-64)-0x80),8,16);
			}
			continue;
		}
		if (ch >= 123 && ch <= 127)
		{
			if (Scale == 1 || Scale == 0)
			{
				KWSprite(Posx+i*7,Posy,8,16,(nicefont+0x80*(FontString[i]-58)-0x80));
			}
			else
			{
				KWSprite(0,0,0,0,nicefont);
				KWSpriteScale(Posx+i*8*Scale,Posy,Scale,(nicefont+0x80*(FontString[i]-58)-0x80),8,16);
			}
			continue;
		}			
		else
		{
			if (Scale == 1 || Scale == 0)
			{
				KWSprite(Posx+i*7,Posy,8,16,(nicefont+0x80*(FontString[i]-32)-0x80));
			}
			else
			{
				KWSprite(0,0,0,0,nicefont);
				KWSpriteScale(Posx+i*8*Scale,Posy,Scale,(nicefont+0x80*(FontString[i]-32)-0x80),8,16);
			}
		}
	}
	}
}


void PrintBigText(int Posx, int Posy, float Scale, char *text)
{
	char *FontString = text;


	{
	    int i;
	    unsigned char ch;
	for (i = 0; i < ReturnStringLength(text); i++)
	{
		ch = (unsigned char)text[i];
		if (ch < 33)
		{
			continue;
		}

		if (ch > 127)
		{
			continue;
		}
		if (ch >= 97 && ch <= 122)
		{
			GlobalAddressA = (Uint)((&nicefont[0])+(0x200*(FontString[i]-64)-0x200));
               KWTexture2DRGBA(Posx+(Scale*i*16),Posy,0,Scale,(u16*)GlobalAddressA,(Vtx*)(&V1632), 16, 32, 16, 32);
			continue;
		}
		if (ch >= 123 && ch <= 127)
		{
			GlobalAddressA = (Uint)((&nicefont[0])+(0x200*(FontString[i]-58)-0x200));
               KWTexture2DRGBA(Posx+(Scale*i*16),Posy,0,Scale,(u16*)GlobalAddressA,(Vtx*)(&V1632), 16, 32, 16, 32);
			continue;
		}			
		else
		{
			GlobalAddressA = (Uint)((&nicefont[0])+(0x200*(FontString[i]-32)-0x200));
               KWTexture2DRGBA(Posx+(Scale*i*16),Posy,0,Scale,(u16*)GlobalAddressA,(Vtx*)(&V1632), 16, 32, 16, 32);
		}
	}
	}
}


void PrintBigTextNumber(int Posx, int Posy, float Scale, char *text, int Value)
{
	char negativeVal;
	int digit[9];
	char valstring[50];
	int a;
	int i;

	PrintBigText(Posx, Posy, Scale, text);
     
	negativeVal = 0;

	if (Value < 0)
	{
		Value = Value*-1;
		negativeVal = 1;
	}

	digit[0] = ((Value%10));
	digit[1] = ((Value%100)/10);
	digit[2] = ((Value%1000)/100);
	digit[3] = ((Value%10000)/1000);
	digit[4] = ((Value%100000)/10000);
	digit[5] = ((Value%1000000)/100000);
	digit[6] = ((Value%10000000)/1000000);
	digit[7] = ((Value%100000000)/10000000);
	digit[8] = ((Value%1000000000)/100000000);

	for (a = 0; a < 50; a++)
	{
		valstring[a] = 32;
	}
	
	for (i = 0; i < NumPlaces(Value); i++)
	{
		if (i > 9)
		{
			continue;
		}
		valstring[i] = (digit[NumPlaces(Value)-1-i] + 48);
	}

	PrintBigText((Scale*32)+Posx+(ReturnStringLength(text))*15*Scale, Posy, Scale, valstring);		

	if (negativeVal == 1)
	{
		PrintBigText((Scale*15)+Posx+(ReturnStringLength(text))*15*Scale, Posy, Scale, "-");
		negativeVal = 0;
	}
}



void PrintBigTextNumberNoGap(int Posx, int Posy, float Scale, char *text, int Value)
{
	char negativeVal;
	int digit[9];
	char valstring[50];
	int a;
	int i;

	PrintBigText(Posx, Posy, Scale, text);
     
	negativeVal = 0;

	if (Value < 0)
	{
		Value = Value*-1;
		negativeVal = 1;
	}

	digit[0] = ((Value%10));
	digit[1] = ((Value%100)/10);
	digit[2] = ((Value%1000)/100);
	digit[3] = ((Value%10000)/1000);
	digit[4] = ((Value%100000)/10000);
	digit[5] = ((Value%1000000)/100000);
	digit[6] = ((Value%10000000)/1000000);
	digit[7] = ((Value%100000000)/10000000);
	digit[8] = ((Value%1000000000)/100000000);

	for (a = 0; a < 50; a++)
	{
		valstring[a] = 32;
	}
	
	for (i = 0; i < NumPlaces(Value); i++)
	{
		if (i > 9)
		{
			continue;
		}
		valstring[i] = (digit[NumPlaces(Value)-1-i] + 48);
	}

	PrintBigText((Scale*32)+Posx+(ReturnStringLength(text) -1)*15*Scale, Posy, Scale, valstring);		

	if (negativeVal == 1)
	{
		PrintBigText((Scale*15)+Posx+(ReturnStringLength(text) -1)*15*Scale, Posy, Scale, "-");
		negativeVal = 0;
	}
}

void PrintNiceTextNumber(int Posx, int Posy, float Scale, char *text, int Value)
{
	char negativeVal;
	int digit[9];
	char valstring[50];
	int a;
	int i;

	PrintNiceText(Posx, Posy, Scale, text);

	negativeVal = 0;

	if (Value < 0)
	{
		Value = Value*-1;
		negativeVal = 1;
	}

	digit[0] = ((Value%10));
	digit[1] = ((Value%100)/10);
	digit[2] = ((Value%1000)/100);
	digit[3] = ((Value%10000)/1000);
	digit[4] = ((Value%100000)/10000);
	digit[5] = ((Value%1000000)/100000);
	digit[6] = ((Value%10000000)/1000000);
	digit[7] = ((Value%100000000)/10000000);
	digit[8] = ((Value%1000000000)/100000000);

	for (a = 0; a < 50; a++)
	{
		valstring[a] = 32;
	}
	
	for (i = 0; i < NumPlaces(Value); i++)
	{
		if (i > 9)
		{
			continue;
		}
		valstring[i] = (digit[NumPlaces(Value)-1-i] + 48);
	}

	PrintNiceText((Scale*14)+Posx+(ReturnStringLength(text))*7*Scale, Posy, Scale, valstring);		

	if (negativeVal == 1)
	{
		PrintNiceText((Scale*7)+Posx+(ReturnStringLength(text))*7*Scale, Posy, Scale, "-");
		negativeVal = 0;
	}
}


void DrawButtonInputDisplay(bool FullMenu)
{
	DrawButtonInputDisplayDefault(FullMenu);
}


short AnchorPoint[][4]  = 
{
     {85,23,95,20}, 
     {120,90,130,85}, {180,210,190,205},
     {120,90,130,85}, {180,210,190,205},
};

void Zanzou2(int Player)
{
    Uint Time = 0x0D00C158;
    short LapMax = GetCourseLapMax();
    int LocalLapY;
    int LocalLapX;
    int LapIndex;


    //Time
	if (KWLap[Player].Tptr2) 
    {
		KWSpriteXLU(KWLap[Player].Lap1CompletionTimeX-19,KWLap[Player].ty+8,128,(Ushort*)Time,32,16,32,16);
        KWPrintLapTimeXLU(KWLap[Player].Lap1CompletionTimeX,KWLap[Player].ty,128,KWLap[Player].Totaltime);
	}
	if (KWLap[Player].Tptr3) 
    {
        KWSpriteXLU(KWLap[Player].Lap2CompletionTimeX-19,KWLap[Player].ty+8,80,(Ushort*)Time,32,16,32,16);
        KWPrintLapTimeXLU(KWLap[Player].Lap2CompletionTimeX,KWLap[Player].ty,80,KWLap[Player].Totaltime);
	}

    LocalLapY = KWLap[Player].cy + 3;
    LocalLapX = KWLap[Player].cx2;

    LocalLapX -= 16;
    LocalLapY -= 4;
    
    //Setup the RCP Color Combiner
            gDPPipeSync(GraphPtrOffset++);
            gDPSetCycleType(GraphPtrOffset++, G_CYC_1CYCLE);
            gSPClearGeometryMode(GraphPtrOffset++,G_ZBUFFER);

            gSPSetGeometryMode(GraphPtrOffset++,G_SHADE | G_SHADING_SMOOTH);
            gDPSetPrimColor(GraphPtrOffset++, 0, 0, 255, 255, 255, 255);
            gDPSetCombineLERP(GraphPtrOffset++,0, 0, 0, TEXEL0,
                TEXEL0, 0, PRIMITIVE, 0,
                0, 0, 0, TEXEL0,
                TEXEL0, 0, PRIMITIVE, 0);
            gDPSetTexturePersp(GraphPtrOffset++,G_TP_NONE);
            gDPSetTextureFilter(GraphPtrOffset++,G_TF_POINT);
            gDPSetTextureConvert(GraphPtrOffset++,G_TC_FILT);
            gDPSetTextureLOD(GraphPtrOffset++,G_TL_TILE);
            gDPSetTextureDetail(GraphPtrOffset++,G_TD_CLAMP);
            gDPSetTextureLUT(GraphPtrOffset++,G_TT_NONE);
            gDPSetRenderMode(GraphPtrOffset++,  G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    //

    
    gDPSetTextureLUT(GraphPtrOffset++, G_TT_RGBA16);
	gSPTexture(GraphPtrOffset++, 65535, 65535, 0, 0, 1);
	gDPLoadTLUT_pal16(GraphPtrOffset++, 0, (Uint)&LapCounterTextures + laptext_PaletteOffset);
	gDPLoadTextureBlock_4b(GraphPtrOffset++, (Uint)&LapCounterTextures, G_IM_FMT_CI,32,8,0,G_TX_CLAMP,G_TX_CLAMP,5,3,0,0);
    
    
    gDPTileSync(GraphPtrOffset++);
    KWRectangle(LocalLapX,LocalLapY,32,8,0,0,1);
    
  	if (KWLap[Player].Cptr2)
    {
        LocalLapX =  KWLap[Player].cx2;
        gDPSetPrimColor(GraphPtrOffset++, 0, 0, 255, 255, 255, 128);
        KWRectangle(LocalLapX,LocalLapY,32,8,0,0,1);
	}
    if (KWLap[Player].Cptr3)
    {
        LocalLapX =  KWLap[Player].cx3;
        gDPSetPrimColor(GraphPtrOffset++, 0, 0, 255, 255, 255, 80);
        KWRectangle(LocalLapX,LocalLapY,32,8,0,0,1);
	}


    gDPPipeSync(GraphPtrOffset++);
    gDPTileSync(GraphPtrOffset++);

    gDPSetPrimColor(GraphPtrOffset++, 0, 0, 255, 255, 255, 255);

    LocalLapX = KWLap[Player].cx + 10;
    LocalLapY = KWLap[Player].cy;

    
    LocalLapY -= 8;

    LapIndex = GetCourseLapIndex(Player);
    
    //1 Player large font.
    gDPLoadTLUT_pal16(GraphPtrOffset++, 0, (Uint)&LapCounterTextures + LargeLapNumbers_PaletteOffset);

    gDPLoadTextureBlock_4b(GraphPtrOffset++, (Uint)&LapCounterTextures + LargeLapNumbers_Offset + (128 * LapIndex), G_IM_FMT_CI,16,16,0,G_TX_CLAMP,G_TX_CLAMP,4,4,0,0);
    KWRectangle(LocalLapX,LocalLapY,16,16,0,0,1);

    LocalLapX += 10;
    gDPLoadTextureBlock_4b(GraphPtrOffset++, (Uint)&LapCounterTextures + LargeLapNumbers_Offset + (128 * 10), G_IM_FMT_CI,16,16,0,G_TX_CLAMP,G_TX_CLAMP,4,4,0,0);
    KWRectangle(LocalLapX,LocalLapY,16,16,0,0,1);

    LocalLapX += 9;
    gDPLoadTextureBlock_4b(GraphPtrOffset++, (Uint)&LapCounterTextures + LargeLapNumbers_Offset + (128 * LapMax), G_IM_FMT_CI,16,16,0,G_TX_CLAMP,G_TX_CLAMP,4,4,0,0);
    KWRectangle(LocalLapX,LocalLapY,16,16,0,0,1);
    
    


}


void KWDisplayUDLapWrapper(int ThisPlayer)
{
    short LapMax;
    int LocalLapX;
    int LocalLapY;
    int LapIndex;
    
    
    
    
    if (KWDBDispSW != 0)
    {
        KWDisplayItembox(ThisPlayer);
        return;
    }
    if (RadarOn[ThisPlayer] == 1)
    {
        KWDisplayItembox(ThisPlayer);
        return;
    }
    if (KWScreenEnable == 0)
    {
        KWDisplayItembox(ThisPlayer);
        return;
    }


    KWDisplayItembox(ThisPlayer);
    KWDisplayTotalTime(ThisPlayer);

    
    KWDisplayInit();
    LapMax = GetCourseLapMax();
    
    gDPLoadTLUT_pal16(GraphPtrOffset++, 0, (Uint)&LapCounterTextures + laptext_PaletteOffset);
    gDPLoadTextureBlock_4b(GraphPtrOffset++, (Uint)&LapCounterTextures, G_IM_FMT_CI,32,8,0,G_TX_CLAMP,G_TX_CLAMP,5,3,0,0);

    LocalLapX = KWLap[ThisPlayer].cx - 16;
    LocalLapY = KWLap[ThisPlayer].cy -1;
    KWRectangle(LocalLapX,LocalLapY,32,8,0,0,1);
    
    if (KWLap[ThisPlayer].Goal != LapMax)
    {
        LapIndex = GetCourseLapIndex(ThisPlayer);

        //1 Player large font.
        gDPLoadTLUT_pal16(GraphPtrOffset++, 0, (Uint)&LapCounterTextures + LargeLapNumbers_PaletteOffset);
        LocalLapX += 26;
        LocalLapY -= 7;
        gDPLoadTextureBlock_4b(GraphPtrOffset++, (Uint)&LapCounterTextures + LargeLapNumbers_Offset + (128 * LapIndex), G_IM_FMT_CI,16,16,0,G_TX_CLAMP,G_TX_CLAMP,4,4,0,0);
        KWRectangle(LocalLapX,LocalLapY,16,16,0,0,1);

        LocalLapX += 10;
        gDPLoadTextureBlock_4b(GraphPtrOffset++, (Uint)&LapCounterTextures + LargeLapNumbers_Offset + (128 * 10), G_IM_FMT_CI,16,16,0,G_TX_CLAMP,G_TX_CLAMP,4,4,0,0);
        KWRectangle(LocalLapX,LocalLapY,16,16,0,0,1);

        LocalLapX += 9;
        gDPLoadTextureBlock_4b(GraphPtrOffset++, (Uint)&LapCounterTextures + LargeLapNumbers_Offset + (128 * LapMax), G_IM_FMT_CI,16,16,0,G_TX_CLAMP,G_TX_CLAMP,4,4,0,0);
        KWRectangle(LocalLapX,LocalLapY,16,16,0,0,1);
    }

    
    gDPSetRenderMode(GraphPtrOffset++,  G_RM_OPA_SURF, G_RM_OPA_SURF2);
}

void KWDisplay4LapWrapper(int ThisPlayer)
{
    int LocalLapX, LocalLapY;
    short LapMax;
    int LapIndex;

    KWDisplayTotalTime(ThisPlayer);

    if ((KWScreenEnable == 0) || (KWLapSW == 0) )
    {
    
        if (KWLap[ThisPlayer].Bomb)
        {
            KWTexture2DCI8BL(
                KWLap[ThisPlayer].Bombx, 
                KWLap[ThisPlayer].Bomby, 
                0, 1.0f, 
                (u8*)0x0D01B4D8, 
                (u8*)0x0D01D6D8,
                (Vtx*)0x0D005AE0,
                32,32,32,32
            );
        }
        KWDisplayItemboxs(ThisPlayer);
        return;
    }

    KWDisplayItemboxs(ThisPlayer);

    KWDisplayInit();
    LapMax = GetCourseLapMax();

    gDPLoadTLUT_pal16(GraphPtrOffset++, 0, (Uint)&LapCounterTextures + laptext_PaletteOffset);
    gDPLoadTextureBlock_4b(GraphPtrOffset++, (Uint)&LapCounterTextures, G_IM_FMT_CI,32,8,0,G_TX_CLAMP,G_TX_CLAMP,5,3,0,0);

    
    
    if (KWLap[ThisPlayer].Goal != LapMax)
    {
        LocalLapX = KWLap[ThisPlayer].cx - 16;
        LocalLapY = KWLap[ThisPlayer].cy - 4;
        KWRectangle(LocalLapX,LocalLapY,32,8,0,0,1);
    }

    gDPLoadTLUT_pal16(GraphPtrOffset++, 0, (Uint)&LapCounterTextures + SmallLapNumbers_PaletteOffset);
    gDPLoadTextureBlock_4b(GraphPtrOffset++, ((Uint)&LapCounterTextures + SmallLapNumbers_Offset), G_IM_FMT_CI,16,128,0,G_TX_CLAMP,G_TX_CLAMP,4,7,0,0);

    if (KWLap[ThisPlayer].Goal != LapMax)
    {
        LocalLapX = KWLap[ThisPlayer].cx - 26;
        LocalLapY = KWLap[ThisPlayer].cy + 4;

        LapIndex = GetCourseLapIndex(ThisPlayer);

        KWRectangle(LocalLapX+13,LocalLapY,16,8,0,(8*LapIndex),1);
        KWRectangle(LocalLapX+21,LocalLapY,8,8,0,(8*10),1);
        KWRectangle(LocalLapX+29,LocalLapY,16,8,0,(8*LapMax),1);
    }
    
    
    if (KWLap[ThisPlayer].Bomb)
    {
        KWTexture2DCI8BL(
            KWLap[ThisPlayer].Bombx, 
            KWLap[ThisPlayer].Bomby, 
            0, 1.0f, 
            (u8*)0x0D01B4D8, 
            (u8*)0x0D01D6D8,
            (Vtx*)0x0D005AE0,
            32,32,32,32
        );
    }
    
    gDPSetRenderMode(GraphPtrOffset++,  G_RM_OPA_SURF, G_RM_OPA_SURF2);
}
     
void KWDisplayLRLapWrapper(int ThisPlayer)
{
    int LocalLapX, LocalLapY;
    short LapMax;
    int LapIndex;

    KWDisplayTotalTime(ThisPlayer);
    
    

    if ((KWScreenEnable == 0) || (KWLapSW == 0) )
    {
        KWDisplayItembox(ThisPlayer);
        return;
    }

    KWDisplayItembox(ThisPlayer);

    KWDisplayInit();
    LapMax = GetCourseLapMax();
    gDPLoadTLUT_pal16(GraphPtrOffset++, 0, (Uint)&LapCounterTextures + laptext_PaletteOffset);
    gDPLoadTextureBlock_4b(GraphPtrOffset++, (Uint)&LapCounterTextures, G_IM_FMT_CI,32,8,0,G_TX_CLAMP,G_TX_CLAMP,5,3,0,0);

    
    
    if (KWLap[ThisPlayer].Goal != LapMax)
    {
        LocalLapX = KWLap[ThisPlayer].cx - 16;
        LocalLapY = KWLap[ThisPlayer].cy - 4;
        KWRectangle(LocalLapX,LocalLapY,32,8,0,0,1);
    }

    gDPLoadTLUT_pal16(GraphPtrOffset++, 0, (Uint)&LapCounterTextures + SmallLapNumbers_PaletteOffset);
    gDPLoadTextureBlock_4b(GraphPtrOffset++, ((Uint)&LapCounterTextures + SmallLapNumbers_Offset), G_IM_FMT_CI,16,128,0,G_TX_CLAMP,G_TX_CLAMP,4,7,0,0);

    if (KWLap[ThisPlayer].Goal != LapMax)
    {
        LocalLapX = KWLap[ThisPlayer].cx;
        LocalLapY = KWLap[ThisPlayer].cy - 4;

        LapIndex = GetCourseLapIndex(ThisPlayer);

        KWRectangle(LocalLapX+13,LocalLapY,16,8,0,(8*LapIndex),1);
        KWRectangle(LocalLapX+21,LocalLapY,8,8,0,(8*10),1);
        KWRectangle(LocalLapX+29,LocalLapY,16,8,0,(8*LapMax),1);
    }
    gDPSetRenderMode(GraphPtrOffset++,  G_RM_OPA_SURF, G_RM_OPA_SURF2);

}
     
void KWDisplayInit(void)
{
    gDPPipeSync(GraphPtrOffset++);
    
    gDPSetCycleType(GraphPtrOffset++, G_CYC_1CYCLE);
    

    gSPClearGeometryMode(GraphPtrOffset++, G_ZBUFFER);
    gSPSetGeometryMode(GraphPtrOffset++, G_SHADE | G_SHADING_SMOOTH);
    
    gDPSetPrimColor(GraphPtrOffset++, 0, 0, 255, 255, 255, 255);
    gDPSetCombineLERP(GraphPtrOffset++,0, 0, 0, TEXEL0,
        TEXEL0, 0, PRIMITIVE, 0,
        0, 0, 0, TEXEL0,
        TEXEL0, 0, PRIMITIVE, 0);
    gDPSetTexturePersp(GraphPtrOffset++,G_TP_NONE);
    gDPSetTextureFilter(GraphPtrOffset++,G_TF_POINT);
    gDPSetTextureConvert(GraphPtrOffset++,G_TC_FILT);
    gDPSetTextureLOD(GraphPtrOffset++,G_TL_TILE);
    gDPSetTextureDetail(GraphPtrOffset++,G_TD_CLAMP);
    gDPSetTextureLUT(GraphPtrOffset++,G_TT_NONE);
    gDPSetRenderMode(GraphPtrOffset++,  G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPPipeSync(GraphPtrOffset++);
    

    gDPSetTextureLUT(GraphPtrOffset++, G_TT_RGBA16);
    gSPTexture(GraphPtrOffset++, 65535, 65535, 0, 0, 1);
    
}

void KWDisplay2P1LR()
{
    KWDisplayLRLapWrapper(0);
}
void KWDisplay2P2LR()
{
    KWDisplayLRLapWrapper(1);
}


void KWDisplay4P1()
{
    KWDisplay4LapWrapper(0);
}
void KWDisplay4P2()
{
    KW2DMatrixInit();
    KWDisplay4LapWrapper(1);
}
void KWDisplay4P3()
{
    KWDisplay4LapWrapper(2);
}
void KWDisplay4P4()
{
    KWDisplay4LapWrapper(3);
}


void KWDisplay2P1UD()
{
    KWDisplayUDLapWrapper(0);
}
void KWDisplay2P2UD()
{
    KWDisplayUDLapWrapper(1);
}

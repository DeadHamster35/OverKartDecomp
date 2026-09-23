#include "MainInclude.h"
#include "OKInclude.h"


Uint SetLimit = 5;

void LoadPosition()
{
     switch(PlayerCount)
     {
          case 0x01 :
          {
               MenuPosition[0] = 5;
               MenuPosition[1] = 15;
               break;
          }
          case 0x02 :
          {
               MenuPosition[0] = 90;
               MenuPosition[1] = 97;
               break;
          }
          case 0x03 :
          {
               MenuPosition[0] = 170;
               MenuPosition[1] = 200;
               break;
          }
          case 0x04 :
          {
               MenuPosition[0] = 90;
               MenuPosition[1] = 97;
               break;
          }
     }
}

void PrintDetails()
{
     if (PlayerCount == 1)
     {
          MenuPosition[0] = 58;
          MenuPosition[1] = 175;
          GraphPtr = FillRect1ColorF(GraphPtr, MenuPosition[0] + 18, MenuPosition[1] + 18, MenuPosition[0] + 185, MenuPosition[1] + 58, 0, 0, 0, 175);
          LoadFont();


          PrintString(MenuPosition[0],MenuPosition[1],"X:");
          PrintString(MenuPosition[0],MenuPosition[1]+10,"Y:");
          PrintString(MenuPosition[0],MenuPosition[1]+20,"Z:");
          PrintString(MenuPosition[0],MenuPosition[1]+30,"A:");

          if (SaveGame.RenderSettings.DetailMode == 1)
          {
               PrintString(MenuPosition[0]+87,MenuPosition[1]," SX:");
               PrintString(MenuPosition[0]+87,MenuPosition[1]+10," SY:");
               PrintString(MenuPosition[0]+87,MenuPosition[1]+20," SZ:");
          }
          else
          {
               PrintString(MenuPosition[0]+87,MenuPosition[1]," LA:");
               PrintString(MenuPosition[0]+87,MenuPosition[1]+10," LS:");
               PrintString(MenuPosition[0]+87,MenuPosition[1]+20," PG:");
          }


          PrintString(MenuPosition[0]+87,MenuPosition[1]+30," SA:");


          { int Loop; for (Loop = 0; Loop < 4; Loop++)
          {
               switch  (Loop)
               {
                    case 0x00:
                    {
                         WholeNumber = (int) GlobalPlayer[0].Position[0];
                         DecimalNumber = (int) ((GlobalPlayer[0].Position[0] - WholeNumber) * 1000);
                         break;
                    }
                    case 0x01:
                    {
                         WholeNumber = (int) GlobalPlayer[0].Position[1];
                         DecimalNumber = (int) ((GlobalPlayer[0].Position[1] - WholeNumber) * 1000);
                         break;
                    }
                    case 0x02:
                    {
                         WholeNumber = (int) GlobalPlayer[0].Position[2];
                         DecimalNumber = (int) ((GlobalPlayer[0].Position[2] - WholeNumber) * 1000);
                         break;
                    }
                    case 0x03:
                    {
                         float playerAngle = (((float)GlobalPlayer[0].Direction[0] / 65536) * 360);
                         WholeNumber = playerAngle;
                         DecimalNumber = (int) ((playerAngle - WholeNumber) * 1000);
                         break;
                    }

               }
               if (DecimalNumber < 0)
               {
                    DecimalNumber = DecimalNumber * -1;
               }

               if (WholeNumber > 0)
               {
                    PrintOffsetD = WholeNumber;
               }
               else
               {
                    PrintOffsetD = WholeNumber * -1;
               }
               if (PrintOffsetD >= 10)
               {
                    if (PrintOffsetD >= 100)
                    {
                         if (PrintOffsetD >= 1000)
                         {

                              PrintOffsetB = 8;
                         }
                         else
                         {
                         PrintOffsetB = 16;
                         }
                    }
                    else
                    {
                    PrintOffsetB = 24;
                    }
               }
               else
               {
                    PrintOffsetB = 32;
               }
               if (WholeNumber >= 0)
               {
                    PrintOffsetB = PrintOffsetB + 8;
               }

               PrintOffsetA = 56;

               if (DecimalNumber < 100)
               {
                    PrintStringNumber(MenuPosition[0]+PrintOffsetA-4,MenuPosition[1],"",0);
                    PrintOffsetC = 8;
               }
               else
               {
                    PrintOffsetC = 0;
               }



               PrintStringNumber(MenuPosition[0]+PrintOffsetB,MenuPosition[1],"",WholeNumber);
               PrintString(MenuPosition[0]+PrintOffsetA,MenuPosition[1],".");
               PrintStringNumber(MenuPosition[0]+PrintOffsetA-4+PrintOffsetC,MenuPosition[1],"",DecimalNumber);
               MenuPosition[1] = MenuPosition[1] + 10;
               //
               //
          } }


          MenuPosition[0] = MenuPosition[0] + 87;
          MenuPosition[1] = 175;
          { int Loop; for (Loop = 0; Loop < 4; Loop++)
          {
               switch  (Loop)
               {
                    case 0x00:
                    {
                         if (SaveGame.RenderSettings.DetailMode == 1)
                         {
                              WholeNumber = (int) GlobalPlayer[0].Velocity[0];
                              DecimalNumber = (int) ((GlobalPlayer[0].Velocity[0] - WholeNumber) * 1000);
                         }
                         else
                         {
                              WholeNumber = (int) LakituSurface[0];
                         }

                         break;
                    }
                    case 0x01:
                    {
                         if (SaveGame.RenderSettings.DetailMode == 1)
                         {
                              WholeNumber = (int) GlobalPlayer[0].Velocity[1];
                              DecimalNumber = (int) ((GlobalPlayer[0].Velocity[1] - WholeNumber) * 1000);
                         }
                         else
                         {
                              WholeNumber = (int) GlobalPlayer[0].JugemuFlag;
                         }

                         break;
                    }
                    case 0x02:
                    {
                         if (SaveGame.RenderSettings.DetailMode == 1)
                         {
                              WholeNumber = (int) GlobalPlayer[0].Velocity[2];
                              DecimalNumber = (int) ((GlobalPlayer[0].Velocity[2] - WholeNumber) * 1000);
                         }
                         else
                         {
                              WholeNumber = (int) NearestPathPointByPlayerId[0];
                         }

                         break;
                    }
                    case 0x03:
                    {
                         WholeNumber = (int) GlobalPlayer[0].Speed;
                         DecimalNumber = (int) ((GlobalPlayer[0].Speed - WholeNumber) * 1000);
                         break;
                    }
               }
               if (DecimalNumber < 0)
               {
                    DecimalNumber = DecimalNumber * -1;
               }


               if (WholeNumber > 0)
               {
                    PrintOffsetD = WholeNumber;
               }
               else
               {
                    PrintOffsetD = WholeNumber * -1;
               }
               if (PrintOffsetD >= 10)
               {
                    if (PrintOffsetD >= 100)
                    {
                         PrintOffsetB = 8;
                    }
                    else
                    {
                    PrintOffsetB = 16;
                    }
               }
               else
               {
                    PrintOffsetB = 24;
               }
               if (WholeNumber >= 0)
               {
                    PrintOffsetB = PrintOffsetB + 8;
               }
               if ((Loop == 2) && (SaveGame.RenderSettings.DetailMode == 2))
               {
                    PrintOffsetB = PrintOffsetB + 8;
               }
               PrintOffsetA = 48;

               if ((SaveGame.RenderSettings.DetailMode == 1) | (Loop == 3))
               {
                    if (DecimalNumber < 100)
                    {
                         PrintStringNumber(MenuPosition[0]+PrintOffsetA-4,MenuPosition[1],"",0);
                         PrintOffsetC = 8;
                    }
                    else
                    {
                         PrintOffsetC = 0;
                    }
               }

               PrintStringNumber(MenuPosition[0]+PrintOffsetB,MenuPosition[1],"",WholeNumber);
               if ((SaveGame.RenderSettings.DetailMode == 1) | (Loop == 3))
               {
                    PrintString(MenuPosition[0]+PrintOffsetA,MenuPosition[1],".");
                    PrintStringNumber(MenuPosition[0]+PrintOffsetA-4+PrintOffsetC,MenuPosition[1],"",DecimalNumber);
               }
               MenuPosition[1] = MenuPosition[1] + 10;
               //
               //
          } }


     }

}

void PrintAnticheat()
{
     LoadPosition();
     
     if (SaveGame.GameSettings.GameType == 3)
     {
          GraphPtr = FillRect1ColorF(GraphPtr, MenuPosition[0] + 18, MenuPosition[1] + 18, MenuPosition[0] + (12 * 8) + 19, MenuPosition[1] + 28, 0, 0,0, 175);
          LoadFont();

          PrintString(MenuPosition[0],MenuPosition[1], "Practice  ON");
          MenuPosition[1] +=10;
     }     
     
     if (SaveGame.GameSettings.ItemMode > 0)
     {
          GraphPtr = FillRect1ColorF(GraphPtr, MenuPosition[0] + 18, MenuPosition[1] + 18, MenuPosition[0] + (11 * 8) + 19, MenuPosition[1] + 28, 0, 0,0, 175);
          LoadFont();

          PrintString(MenuPosition[0],MenuPosition[1], "Force Items");
          MenuPosition[1] +=10;
     }
     else if (SaveGame.GameSettings.AIMode > 0x00)
     {

          switch (SaveGame.GameSettings.AIMode)
          {
               // PRACTICE BUILD overlay text.
               case 0x01 :
               {
                    GraphPtr = FillRect1ColorF(GraphPtr, MenuPosition[0] + 18, MenuPosition[1] + 18, MenuPosition[0] + (10 * 8) + 19, MenuPosition[1] + 28, 0, 0,0, 175);
                    LoadFont();
                    PrintString(MenuPosition[0],MenuPosition[1], "Versus CPU");
                    break;
               }
               case 0x02 :
               {
                    GraphPtr = FillRect1ColorF(GraphPtr, MenuPosition[0] + 18, MenuPosition[1] + 18, MenuPosition[0] + (9 * 8) + 19, MenuPosition[1] + 28, 0, 0,0, 175);
                    LoadFont();
                    PrintString(MenuPosition[0],MenuPosition[1], "Debug CPU");
                    break;
               }
          }
          MenuPosition[1] +=10;
     }
     else if ((SaveGame.GameSettings.StatsMode == 1) && (HotSwapID == 0) )
     {
          GraphPtr = FillRect1ColorF(GraphPtr, MenuPosition[0] + 18, MenuPosition[1] + 18, MenuPosition[0] + (11 * 8) + 19, MenuPosition[1] + 28, 0, 0,0, 175);
          LoadFont();
          PrintString(MenuPosition[0],MenuPosition[1], "Force Stats");
          MenuPosition[1] +=10;
     }



}


void CheckMenuButton (short PlayerIndex)
{
     if (ButtonHolding[PlayerIndex] == true)
     {
          if ((GlobalController[PlayerIndex]->ButtonHeld == 0) && (GlobalController[PlayerIndex]->AnalogHeld == 0))
          {
               MenuButtonHeld[PlayerIndex] = 0;
               ButtonHolding[PlayerIndex] = false;
               ButtonTimer[PlayerIndex] = 0;
               
          }
          else
          {
               ButtonTimer[PlayerIndex] = ButtonTimer[PlayerIndex] + 1;  //Button Held Timer          
               if (ButtonTimer[PlayerIndex] >= 15)
               {                      
                    ButtonHolding[PlayerIndex] = false;
                    ButtonTimer[PlayerIndex] = 0;  //Button Held Timer               
               }
          }
     }
     else
     {
          if (GlobalController[PlayerIndex]->ButtonHeld > 0)
          {
               MenuButtonHeld[PlayerIndex] = GlobalController[PlayerIndex]->ButtonHeld;
               if (GlobalController[PlayerIndex]->ButtonPressed == 0)
               {
                    ButtonHolding[PlayerIndex] = true;
               }
          }
          else
          {
               if (GlobalController[PlayerIndex]->AnalogHeld > 0)
               {
                    MenuButtonHeld[PlayerIndex] = GlobalController[PlayerIndex]->AnalogHeld;
                    if (GlobalController[PlayerIndex]->AnalogPressed == 0)
                    {
                         ButtonHolding[PlayerIndex] = true;
                    }
               }
               else
               {          
                    MenuButtonHeld[PlayerIndex] = 0;
                    ButtonHolding[PlayerIndex] = false;
                    ButtonTimer[PlayerIndex] = 0;  //Button Held Timer
               }
          }
     }
}

void SwapParameter(OKMenu OptionsMenu, int directionIndex)
{
     
     //if directionIndex == 0, swap down;
     //if directionIndex == 1, swap up;

     if(directionIndex == 0) 
     {
          if(OptionsMenu.PanelAddress[MenuIndex].ParameterToggles[ParameterIndex - 1 + MenuOverflow] > 0)
          {
               OptionsMenu.PanelAddress[MenuIndex].ParameterToggles[ParameterIndex - 1 + MenuOverflow]--;
               PlaySound(0x49008000);
               return;
          }          
     }
     else if(OptionsMenu.PanelAddress[MenuIndex].ParameterToggles[ParameterIndex - 1+ MenuOverflow] < OptionsMenu.PanelAddress[MenuIndex].Options[ParameterIndex - 1+ MenuOverflow].ParameterCount - 1) 
     {
          OptionsMenu.PanelAddress[MenuIndex].ParameterToggles[ParameterIndex - 1 + MenuOverflow]++;
          PlaySound(0x49008000);
          return;
     }
     
     
     PlaySound(0x49008002);
}

void GameOptionsHandler(short PlayerIndex)
{
     CheckMenuButton(PlayerIndex);
          
     //MenuButtonHeld is set to 0x01 when a Button is held down.
     if(ButtonHolding[PlayerIndex] == false)
     {
          
          // Uses the Control Stick or Dpad to switch through the menu.
          switch(MenuButtonHeld[PlayerIndex])
          {
               //Increase the Current menu Parameter by 1
              case BTN_DRIGHT :
               {
                    ButtonHolding[PlayerIndex] = true;
                    if (ParameterIndex > 0) //currentParameter
                    {
                         if (MenuIndex < GameOKMenu.PanelCount)
                         {
                              SwapParameter(GameOKMenu,1);
                         }
                         else
                         {
                              if (ParameterIndex == 1) //currentParameter
                              {
                                   if (MenuCup < 3)  //cupSelect
                                   {
                                        MenuCup++;
                                   }
                              }
                              else
                              {
                                   short *l_courseID;

                                   if (SYSTEM_Region == 0x00)
                                   {
                                        GlobalAddressA = (cup_PAL + (MenuCup * 8) + ((ParameterIndex - 2) * 2));
                                   }
                                   else
                                   {
                                        GlobalAddressA = (cup_NTSC + (MenuCup * 8) + ((ParameterIndex - 2) * 2));
                                   }

                                   l_courseID = (short *)GlobalAddressA;

                                   if (*l_courseID < 19)
                                   {
                                        *l_courseID = *l_courseID + 1;
                                        CopyCourseTable(1);
                                   }
                              }
                         }
                    }
                    else
                    {
                         if (MenuIndex < GameOKMenu.PanelCount)
                         {
                              MenuIndex++;
                              MenuOverflow = 0;
                              PlaySound(0x49008001);
                         }
                         else
                         {
                              PlaySound(0x49008002);
                         }
                    }
                    break;

               }
               //Decrease the Current menu Parameter by 1
               case BTN_DLEFT :
               {
                    ButtonHolding[PlayerIndex] = true;
                    if (ParameterIndex > 0) //currentParameter
                    {
                         if (MenuIndex < GameOKMenu.PanelCount)
                         {
                              SwapParameter(GameOKMenu,0);
                         }
                         else
                         {
                              if (ParameterIndex == 1) //currentParameter
                              {
                                   if (MenuCup > 0)  //cupSelect
                                   {
                                        MenuCup--;
                                   }
                              }
                              else
                              {
                                   short *l_courseID;

                                   if (SYSTEM_Region == 0x00)
                                   {
                                        GlobalAddressA = (cup_PAL + (MenuCup * 8) + ((ParameterIndex - 2) * 2));
                                   }
                                   else
                                   {
                                        GlobalAddressA = (cup_NTSC + (MenuCup * 8) + ((ParameterIndex - 2) * 2));
                                   }
                                   l_courseID = (short *)GlobalAddressA;
                                   if (*l_courseID > 0)
                                   {
                                        *l_courseID = *l_courseID - 1;
                                        CopyCourseTable(1);
                                   }
                              }
                         }
                    }
                    else
                    {
                         if (MenuIndex > 0)
                         {
                              MenuIndex--;
                              MenuOverflow = 0;
                              PlaySound(0x49008001);
                         }
                         else
                         {
                              PlaySound(0x49008002);
                         }
                    }
                    break;

               }
               //Move forward to Next option
               case BTN_DDOWN :
               {
                    ButtonHolding[PlayerIndex] = true;
                    if (MenuIndex < GameOKMenu.PanelCount)
                    {
                         GlobalIntA = GameOKMenu.PanelAddress[MenuIndex].OptionCount;
                    }
                    else
                    {
                         GlobalIntA = 5; //cup editor
                    }
                    if (ParameterIndex + MenuOverflow < GlobalIntA) //currentParameter
                    {
                         if ((ParameterIndex == 7) && (MenuIndex < GameOKMenu.PanelCount))
                         {
                              MenuOverflow++;
                         }
                         else
                         {
                              ParameterIndex++; //currentParameter
                         }
                         PlaySound(0x4900801C);
                    }
                    break;

                    //
               }
               //Move Backward to previous option
               case BTN_DUP :
               {
                    ButtonHolding[PlayerIndex] = true;
                    if (ParameterIndex + MenuOverflow > 0) //currentParameter
                    {
                         if ((ParameterIndex == 1) && (MenuIndex < GameOKMenu.PanelCount) && (MenuOverflow > 0))
                         {
                              MenuOverflow--;
                         }
                         else
                         {
                              ParameterIndex--; //currentParameter
                         }
                         PlaySound(0x4900801C);
                    }
                    break;
               }

          }
          // End of menu Dpad Code.


     }

}

void TitleMenuHandler(short PlayerIndex)
{
     CheckMenuButton(PlayerIndex);
     //MenuButtonHeld is set to 0x01 when a Button is held down.
     if(ButtonHolding[PlayerIndex] == false)
     {
          
          // Uses the Control Stick or Dpad to switch through the menu.
          switch(MenuButtonHeld[PlayerIndex])
          {
               //Increase the Current menu Parameter by 1
               case BTN_DRIGHT :
               {
                    ButtonHolding[PlayerIndex] = true;
                    if (ParameterIndex > 0) //currentParameter
                    {
                         if (MenuIndex < 2)
                         {
                              SwapParameter(GameOKMenu,1);
                         }
                         else
                         {
                              if (ParameterIndex == 1) //currentParameter
                              {
                                   if (MenuCup < 3)  //cupSelect
                                   {
                                        MenuCup++;
                                   }
                              }
                              else
                              {
                                   short *l_courseID;

                                   if (SYSTEM_Region == 0x00)
                                   {
                                        GlobalAddressA = (cup_PAL + (MenuCup * 8) + ((ParameterIndex - 2) * 2));
                                   }
                                   else
                                   {
                                        GlobalAddressA = (cup_NTSC + (MenuCup * 8) + ((ParameterIndex - 2) * 2));
                                   }

                                   l_courseID = (short *)GlobalAddressA;

                                   if (*l_courseID < 19)
                                   {
                                        *l_courseID = *l_courseID + 1;
                                        CopyCourseTable(1);
                                   }
                              }
                         }
                    }
                    else
                    {
                         if (MenuIndex < 4)
                         {
                              MenuIndex++;
                              MenuOverflow = 0;
                              PlaySound(0x49008001);
                         }
                         else
                         {
                              PlaySound(0x49008002);
                         }
                    }
                    break;

               }
               //Decrease the Current menu Parameter by 1
               case BTN_DLEFT :
               {
                    ButtonHolding[PlayerIndex] = true;
                    if (ParameterIndex > 0) //currentParameter
                    {
                         if (MenuIndex < 2)
                         {
                              SwapParameter(GameOKMenu,0);
                         }
                         else
                         {
                              if (ParameterIndex == 1) //currentParameter
                              {
                                   if (MenuCup > 0)  //cupSelect
                                   {
                                        MenuCup--;
                                   }
                              }
                              else
                              {
                                   short *l_courseID;

                                   if (SYSTEM_Region == 0x00)
                                   {
                                        GlobalAddressA = (cup_PAL + (MenuCup * 8) + ((ParameterIndex - 2) * 2));
                                   }
                                   else
                                   {
                                        GlobalAddressA = (cup_NTSC + (MenuCup * 8) + ((ParameterIndex - 2) * 2));
                                   }
                                   l_courseID = (short *)GlobalAddressA;
                                   if (*l_courseID > 0)
                                   {
                                        *l_courseID = *l_courseID - 1;
                                        CopyCourseTable(1);
                                   }
                              }
                         }


                    }
                    else
                    {
                         if (MenuIndex > 0)
                         {
                              MenuIndex--;
                              MenuOverflow = 0;
                              PlaySound(0x49008001);
                         }
                         else
                         {
                              PlaySound(0x49008002);
                         }
                    }
                    break;

               }
               //Move forward to Next option
               case BTN_DDOWN :
               {
                    ButtonHolding[PlayerIndex] = true;
                    if (ParameterIndex + MenuOverflow < GameOKMenu.PanelAddress[MenuIndex].OptionCount) //currentParameter
                    {
                         if ((ParameterIndex == 4) && (MenuIndex < 3))
                         {
                              MenuOverflow++;
                         }
                         else
                         {
                              ParameterIndex++; //currentParameter
                         }
                         PlaySound(0x4900801C);
                    }
                    break;

                    //
               }
               //Move Backward to previous option
               case BTN_DUP :
               {
                    ButtonHolding[PlayerIndex] = true;
                    if (ParameterIndex + MenuOverflow > 0) //currentParameter
                    {
                         if ((ParameterIndex == 1) && (MenuIndex < 3) && (MenuOverflow > 0))
                         {
                              MenuOverflow--;
                         }
                         else
                         {
                              ParameterIndex--; //currentParameter
                         }
                         PlaySound(0x4900801C);
                    }
                    break;
               }


          }
          // End of menu Dpad Code.


     }

     
     
     //OptionsMenu(175,2,1);
}

void Title2()
{
     return;
     gDPPipeSync(GraphPtrOffset++);
	gDPSetCycleType(GraphPtrOffset++, G_CYC_FILL);
     gDPSetColorImage(GraphPtrOffset++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320,
					 K0_TO_PHYS(CfbPtrs[DispFrame]));
     gDPSetFillColor(GraphPtrOffset++, GPACK_RGBA5551(255,0,0,1)<<16 | GPACK_RGBA5551(255,0,0,1));
     gDPFillRectangle(GraphPtrOffset++, 0, 0, 319, 239);
}

void TitleMenu()
{
     

    if(TitleDemo > 4)
    {
        TitleDemo = 4;   //This is a Timer that runs at the title screen. Locking at 4 Prevents the demo courses from being displayed.
    }

     
     MatrixCount = 0;
     

     
     #if OverKartBuild
      
     if (MenuChanged != 10)
     {
          return;
     }
     MatrixCount = 0;
     
     
     InitRDP();
     SetViewport((Screen*)&g_Screen1);                
     gSPClearGeometryMode(GraphPtrOffset++, 0xFFFFFFFF);
     gSPSetGeometryMode(GraphPtrOffset++, G_CLIPPING |G_SHADE |G_SHADING_SMOOTH);
     ClearZBuffer2((Screen*)&g_Screen1);

     
     
     guPerspective(&DynamicP->MTXPersp[0],&PerspectiveValue, 80.0f, 320.0f/240.0f, 1.0f, 15000.0f, 1.0f);
     gSPPerspNormalize(GraphPtrOffset++, PerspectiveValue);
     guLookAt(&DynamicP->MTXLookAt[0],
          0,0,0,
          0,0,-7500,
          0,1,0);



     gSPMatrix(GraphPtrOffset++, &DynamicP->MTXPersp[0], 
               G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
     gSPMatrix(GraphPtrOffset++, &DynamicP->MTXLookAt[0], 
               G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);

     

     //Segmented Address of title screen 3D logo is 0x0A000000
     gSPSetGeometryMode(GraphPtrOffset++,G_CULL_BACK | G_ZBUFFER | G_SHADING_SMOOTH | G_SHADE);
     
     gSPDisplayList(GraphPtrOffset++,0x08000000);		


          

     #endif
     
     LoadFont();
     PrintString(10,200,VersionString);




     
}




short CheckPlayerSelect(int TargetController, short Direction)
{
     short Target = PlayerCharacterSelect[TargetController];
     while (true)
     {
          GlobalBoolC = true;
          if (((Target + Direction) < 8) && ((Target + Direction) > -1))
          {
               Target += Direction;          
               { int ThisController; for (ThisController = 0; ThisController < PlayerCount; ThisController++)
               {
                    if (ThisController != TargetController)
                    {
                         if (PlayerCharacterSelect[ThisController] == Target)
                         {
                              GlobalBoolC = false;
                         }
                    }
               } }
               if (GlobalBoolC)
               {
                    return Target;
               }
          }
          else
          {
               return -1;
          }
     }
}



void PlayerSelectSetup()
{

     
	*TargetAddress = (long)&BackupPortraitTable;
	*SourceAddress = (long)&SelectPortraitTable;
	DataLength = 288;
	RunRAM();
	

	OKPSelSkipCursor[0] = 1;
	OKPSelSkipCursor[1] = 1;
	OKPSelSkipCursor[2] = 1;
	OKPSelSkipCursor[3] = 1;
	
	
	PlayerSelectPositions[0].y -= 10;
	PlayerSelectPositions[1].y -= 10;
	PlayerSelectPositions[2].y -= 10;
	PlayerSelectPositions[3].y -= 10;
	
	*TargetAddress = (long)&BackupNamePlateTable;
	*SourceAddress = (long)&SelectNamePlateTable;
	DataLength = 32;
	RunRAM();
}

void PlayerSelectMenu(short StatsMode, short PlayerIndex)
{
     
     CheckMenuButton(PlayerIndex);
     GlobalIntC = *(int*)(long)(&PlayerOK);
     
     if ((KBGNumberNext) && (MenuBackup == 0))
     {
          KBGNumberNext = 0;
          
     }
     { int ThisJump; for (ThisJump = 0; ThisJump < 8; ThisJump++)
     {
          OKPSelSkipPortrait[ThisJump] = 1;
     } }

     *(int*)(long)&PlayerShowStats = 0;
     
     
     HotSwapID = 0;
     CopyCourseTable(0);

	CourseValue = -1;
     //StockASM();
     HsLabel = -1;

     OKPSelSkipAnime = 0;

     
     //
     if (MenuBackup == 0)
     {    
          if (StatsMode == 2)
          {
               SetEngine(CharacterConvert[PlayerCharacterSelect[PlayerIndex] + 1],PlayerEngineSelect[PlayerIndex]);
               SetSteering(CharacterConvert[PlayerCharacterSelect[PlayerIndex] + 1], PlayerSteerSelect[PlayerIndex]);
               
               if (MenuProgress[PlayerIndex] == 0)
               {
                    PlayerOK[PlayerIndex] = 0;
                    PlayerShowStats[PlayerIndex] = 0;
                    OKPSelSkipCursor[PlayerIndex] = 1;
                    switch (MenuButtonHeld[PlayerIndex])
                    {
                         case BTN_DLEFT:
                         {    
                              if (CheckPlayerSelect(PlayerIndex,-1) != -1)
                              {
                                   PlaySound(0x49008001);
                                   PlayerCharacterSelect[PlayerIndex] = CheckPlayerSelect(PlayerIndex, -1);
                              }
                              OKPSelSkipAnime = 1;
                              break;
                         }
                         case BTN_DRIGHT:
                         {
                              if (CheckPlayerSelect(PlayerIndex,1) != -1)
                              {
                                   PlaySound(0x49008001);
                                   PlayerCharacterSelect[PlayerIndex] = CheckPlayerSelect(PlayerIndex, 1);
                              }   
                              OKPSelSkipAnime = 1;
                              break;
                         }
                         case BTN_A:
                         {
                              PlaySound(0x49008001);
                              MenuProgress[PlayerIndex]++;  
                              break;
                         }
                         case BTN_B:
                         {                              
                              SetFadeOutB();
                              *(int*)(long)&PlayerShowStats = 0;
                              MenuBackup = 1;
                              break;
                         }
                         default:
                         {
                            break;
                         }
                    }
               }
               else if (MenuProgress[PlayerIndex] == 1)
               {
                    PlayerOK[PlayerIndex] = 0;
                    PlayerShowStats[PlayerIndex] = 1;
                    OKPSelSkipCursor[PlayerIndex] = 0;


                    switch (MenuButtonHeld[PlayerIndex])
                    {
                         case BTN_DLEFT:
                         {    
                              if ((int)PlayerEngineSelect[PlayerIndex] > 0)
                         {
                              PlaySound(0x49008001);
                              PlayerEngineSelect[PlayerIndex]--;                                   
                         }   
                              break;
                         }
                         case BTN_DRIGHT:
                         {
                              if ((int)PlayerEngineSelect[PlayerIndex] < 2)
                              {
                                   PlaySound(0x49008001);
                                   PlayerEngineSelect[PlayerIndex]++;                                   
                              }  
                              break;
                         }
                         case BTN_A:
                         {
                              PlaySound(0x49008001);
                              MenuProgress[PlayerIndex]++;        
                              break;
                         }
                         case BTN_B:
                         {                              
                              PlaySound(0x49008002);
                              MenuProgress[PlayerIndex]--;
                              break;
                         }
                    }
               }
               else if (MenuProgress[PlayerIndex] == 2)
               {
                    PlayerOK[PlayerIndex] = 0;
                    PlayerShowStats[PlayerIndex] = 1;
                    OKPSelSkipCursor[PlayerIndex] = 0;



                    switch (MenuButtonHeld[PlayerIndex])
                    {
                         case BTN_DLEFT:
                         {    
                              if ((int)PlayerSteerSelect[PlayerIndex] > 0)
                              {
                                   PlaySound(0x49008001);
                                   PlayerSteerSelect[PlayerIndex]--;                                   
                              } 
                              break;
                         }
                         case BTN_DRIGHT:
                         {
                              if ((int)PlayerSteerSelect[PlayerIndex] < 2)
                              {
                                   PlaySound(0x49008001);
                                   PlayerSteerSelect[PlayerIndex]++;                                   
                              }   
                              break;
                         }
                         case BTN_A:
                         {
                              PlaySound(0x49008001);   
                              MenuProgress[PlayerIndex]++;
                              break;
                         }
                         case BTN_B:
                         {                              
                              PlaySound(0x49008002);
                              MenuProgress[PlayerIndex]--;
                              break;
                         }
                    }
               }
               else if (MenuProgress[PlayerIndex] == 3)
               {
                    PlayerShowStats[PlayerIndex] = 0;
                    if (PlayerOK[PlayerIndex] != 1)
                    {
                         int SoundOffset;
                         PlayerOK[PlayerIndex] = 1;
                         SoundOffset = (0x10 * CharacterConvert[(int)PlayerCharacterSelect[PlayerIndex] + 1]);
                         PlaySound(0x2900800e + SoundOffset);  
                    }
                    if ((MenuButtonHeld[PlayerIndex] & BTN_B) == BTN_B)
                    {    
                         PlaySound(0x49008002);
                         MenuProgress[PlayerIndex]--;
                    }
               }
               
          }
          else
          {
               
               switch (MenuProgress[PlayerIndex])
               {
                    case 0:
                    {
                         PlayerShowStats[PlayerIndex] = 1;
                         OKPSelSkipCursor[PlayerIndex] = 1;
                         PlayerOK[PlayerIndex] = 0;

                         switch (MenuButtonHeld[PlayerIndex])
                         {
                              case BTN_DLEFT:
                              {    
                                   if (CheckPlayerSelect(PlayerIndex,-1) != -1)
                                   {
                                        PlaySound(0x49008001);
                                        PlayerCharacterSelect[PlayerIndex] = CheckPlayerSelect(PlayerIndex, -1);
                                   }
                                   OKPSelSkipAnime = 1;
                                   break;
                              }
                              case BTN_DRIGHT:
                              {
                                   if (CheckPlayerSelect(PlayerIndex,1) != -1)
                                   {
                                        PlaySound(0x49008001);
                                        PlayerCharacterSelect[PlayerIndex] = CheckPlayerSelect(PlayerIndex, 1);
                                   }   
                                   OKPSelSkipAnime = 1;
                                   break;
                              }
                              case BTN_A:
                              {
                                   PlaySound(0x49008001);
                                   MenuProgress[PlayerIndex]++;  
                                   break;
                              }
                              case BTN_B:
                              {                              
                                   SetFadeOutB();
                                   *(int*)(long)&PlayerShowStats = 0;
                                   MenuBackup = 1;
                                   break;
                              }
                         }

                         break;  
                    }
                    case 1:
                    {
                         PlayerShowStats[PlayerIndex] = 0;
                         if (PlayerOK[PlayerIndex] != 1)
                         {
                              int SoundOffset;
                              PlayerOK[PlayerIndex] = 1;
                              SoundOffset = (0x10 * CharacterConvert[(int)PlayerCharacterSelect[PlayerIndex] + 1]);
                              PlaySound(0x2900800e + SoundOffset);  
                         }
                         
                         OKPSelSkipCursor[PlayerIndex] = 0;

                              
                         if ((MenuButtonHeld[PlayerIndex] & BTN_B) == BTN_B)
                         {    
                              PlaySound(0x49008002);
                              MenuProgress[PlayerIndex]--;
                         }
                         break;
                    }
               }
                          
          }          
     }
          
     GlobalBoolA = true;
     { int ThisController; for (ThisController = 0; ThisController < PlayerCount; ThisController++)
     {
          if (PlayerOK[ThisController] == 0)
          {
               GlobalBoolA = false;
          }
     } }
     if (GlobalBoolA)
     {
          if (MenuScreenC !=2)
          {
               MenuScreenC = 2;
          }
          else
          {
               if ((MenuButtonHeld[PlayerIndex] & BTN_A) == BTN_A)
               {
                    PlaySound(0x49008000);
                    SetFadeOut(30);
               }
          }
     }


     if ((MenuScreenC == 2) && ((MenuButtonHeld[PlayerIndex] & BTN_B) == BTN_B))
     {
          MenuScreenC = 0;
          { int ThisController; for (ThisController = 0; ThisController < PlayerCount; ThisController++)
          {
               PlayerOK[ThisController] = 0;
               if (StatsMode == 2)
               {
                    MenuProgress[ThisController] = 1;
               }
               else
               {
                    MenuProgress[ThisController] = 0;
               }
          } }
          OKPSelSkipCursor[0] = 1;
          OKPSelSkipCursor[1] = 1;
          OKPSelSkipCursor[2] = 1;
          OKPSelSkipCursor[3] = 1;
     }
     
     { int CurrentHUD; for (CurrentHUD = 0; CurrentHUD < 32; CurrentHUD++)
     {
          if ((GlobalMenuHUD[CurrentHUD].Kind >= KO_PSEL_MA) && (GlobalMenuHUD[CurrentHUD].Kind <= KO_PSEL_KU))
          {
               GlobalMenuHUD[CurrentHUD].Pattern = 4;               
          }
     } }
}

void PlayerSelectMenuAfter()
{
     { int ThisController; for (ThisController = 0; ThisController < PlayerCount; ThisController++)
     {
          OKPSelSkipCursor[ThisController] = 1;
          CharacterSelections[ThisController] = CharacterConvert[(int)PlayerCharacterSelect[ThisController] + 1];
          PlayerCharacterSelect[ThisController] = ThisController;
          MenuProgress[ThisController] = 0;
     } }
}


void PlayerSelectMenuBefore()
{

     GlobalIntA = 160 - ((PlayerCount * 64 + ((PlayerCount - 1) * 6)) / 2);
     { int ThisPlayer; for (ThisPlayer = 0; ThisPlayer < PlayerCount; ThisPlayer++)
     {
          PlayerSelectPositions[ThisPlayer].x = GlobalIntA;
          GlobalIntA += 70;
     } }
     MenuBackup = 0;
     { int ThisController; for (ThisController = 0; ThisController < PlayerCount; ThisController++)
     {						
          PlayerCharacterSelect[ThisController] = ThisController;
          OKPSelSkipCursor[ThisController] = 1;
     } }
}
void MapSelectMenu(short PlayerIndex)
{
     if (MapIsCurrentlyLoading)
     {
          return;
     }
    
     if ((MenuScreenA == 3) && (GlobalController[4]->ButtonPressed == BTN_A))
     {
          if (HotSwapID > 0)
          {
               HsTableSet();
          }
     }

     if (GameMode == GAMEMODE_BATTLE)
     {
          GlobalShortA = 4;
     }
     else
     {
          GlobalShortA = 1;
     }

     if (ROptionPressed != 0)
     {    
          if (ROptionPressed + 2 <= GlobalFrameCount)
          {
               ROptionPressed = 0;                 
               KBGNumber = 6;
               KBGNumberNext = 6;
               KBGChange = 1;
               SetFadeOutTaData();
          }
     }
     if ((GlobalController[PlayerIndex]->ButtonPressed & BTN_R) == BTN_R)
     {
          PlaySound(0x49009010);   
          ROptionPressed = GlobalFrameCount;
     }

     if (MenuScreenA == GlobalShortA)
     {
          if ((GlobalController[PlayerIndex]->ButtonPressed & BTN_CLEFT) == BTN_CLEFT)
          {
               SwapHS(0);
          }
          else if ((GlobalController[PlayerIndex]->ButtonPressed & BTN_CRIGHT) == BTN_CRIGHT)
          {
               if (HotSwapID < SetLimit)
               {
                    SwapHS(1);
               }
               
          }
          LoadCustomHeader(CourseValue + GPCourseIndex);
     }

     *(int*)(&PlayerOK) = 0;
     if (HotSwapID > 0)
     {
          CourseValue = -1;
          GPCourseIndex = 0;
          switch(GameMode)
          {
               //GRAND PRIX

               case 0:
               {
                    CourseValue = (CupSelect * 4);                    
                    break;
               }
               case 1:
               case 2:
               case 3:
               {
                    CourseValue = (CupSelect * 4) + CourseSelect;
                    break;
               }
          }
     }
     else
     {
          LoadCustomHeader(-1);
     }
     
}



char GameMenuIndex = 0;
#define PSMode 0
#define GSMode 1
#define CCMode 2
void DrawGameSelect()
{
    
    DrawBox(65,18,190,25,0,0,0,175);
    PrintBigText(90,16, 1.0f,"GAME SETUP");
    
    if (ROptionPressed > 0)
    {
        KWTexture2DRGBA(70, 200, 0, 1.0f, (Uchar*)MenuIconsRAM + 0x1000, (void*)&V64x32, 64, 32, 64, 32);
    }
    else
    {
        KWTexture2DRGBA(70, 200, 0, 1.0f, (Uchar*)MenuIconsRAM, (void*)&V64x32, 64, 32, 64, 32);
    }
     


    /*
    //Player Count
    //Game Mode
    //CC Mode


    DrawBox(25,70,130,28,0,0,0,175);
    DrawBox(25,105,130,28,0,0,0,175);
    DrawBox(25,140,130,28,0,0,0,175);

    PrintBigText(38, 75, 0.6f, "Player Count");
    PrintBigText(53, 110, 0.6f, "Game Mode");
    PrintBigText(38, 145, 0.6f, "Engine Speed");
    

    
    DrawBox(160,50,140,130,0,0,0,175);


    
    //Draw MenuIcon
    switch (GameMenuIndex)
    {
        case PSMode:
        {
            //Player Select Controller Icons
            GlobalAddressA  = (MenuIconsRAM + MenuGFX_Controller_Offset);         
            break;
        }
    }

    switch (PlayerCount)
    {
        case 1:
        {
            KWTexture2DRGBA32PT(230,110,0,1.0,(Uchar*)GlobalAddressA,(void*)(&V6432B),64,64,64,16);                         
            break;
        }
        case 2:
        {
            KWTexture2DRGBA32PT(215,110,0,1.0,(Uchar*)GlobalAddressA,(void*)(&V6432B),64,64,64,16);
            KWTexture2DRGBA32PT(245,110,0,1.0,(Uchar*)GlobalAddressA,(void*)(&V6432B),64,64,64,16);
            break;
        }
        case 3:
        {
            
            KWTexture2DRGBA32PT(215,90,0,1.0,(Uchar*)GlobalAddressA,(void*)(&V6432B),64,64,64,16);
            KWTexture2DRGBA32PT(245,90,0,1.0,(Uchar*)GlobalAddressA,(void*)(&V6432B),64,64,64,16);
            KWTexture2DRGBA32PT(230,130,0,1.0,(Uchar*)GlobalAddressA,(void*)(&V6432B),64,64,64,16);
            break;
        }
        case 4:
        {
            KWTexture2DRGBA32PT(215,90,0,1.0,(Uchar*)GlobalAddressA,(void*)(&V6432B),64,64,64,16);
            KWTexture2DRGBA32PT(245,90,0,1.0,(Uchar*)GlobalAddressA,(void*)(&V6432B),64,64,64,16);
            KWTexture2DRGBA32PT(215,130,0,1.0,(Uchar*)GlobalAddressA,(void*)(&V6432B),64,64,64,16);
            KWTexture2DRGBA32PT(245,130,0,1.0,(Uchar*)GlobalAddressA,(void*)(&V6432B),64,64,64,16);
            break;
        }
    }
    */
     
}
void DrawMapSelect()
{
     DrawBox(65,18,190,25,0,0,0,175);
     if (HotSwapID == 0)
     {
          PrintBigText(80,16, 0.9f,"Original Set");
     }
     else if (HotSwapID < 10)
     {
          PrintBigTextNumberNoGap(80,16, 0.9f,"Custom Set ",HotSwapID);
     }
     else
     {
          PrintBigTextNumberNoGap(80,16, 0.9f,"Custom Set",HotSwapID);
     }
     
     if (ROptionPressed > 0)
     {
          KWTexture2DRGBA(70, 200, 0, 1.0f, (Uchar*)MenuIconsRAM + 0x1000, (void*)&V64x32, 64, 32, 64, 32);
     }
     else
     {
          KWTexture2DRGBA(70, 200, 0, 1.0f, (Uchar*)MenuIconsRAM, (void*)&V64x32, 64, 32, 64, 32);
     }
     SpriteBtnCLeft(45,35,1.0,false);
     SpriteBtnCRight(279,35,1.0,false);
     
}
void DrawPlayerSelect(short StatsMode)
{
     
     DrawBox(65,18,190,25,0,0,0,175);
     PrintBigText(75,16, 0.9f,"Player Select");
     
     
     
     

     #ifndef CFLG_PlayerSelect


          { int CurrentPlayer; for (CurrentPlayer = 0; CurrentPlayer < PlayerCount; CurrentPlayer++)
          {	
               
                              
               //Set Portraits and Banner for Character Selected
               { int ThisPortrait; for (ThisPortrait = 0; ThisPortrait < 8; ThisPortrait++)
               {
                    SelectPortraitTable[ThisPortrait].TextureOffset[CurrentPlayer] = BackupPortraitTable[ThisPortrait].TextureOffset[(int)PlayerCharacterSelect[CurrentPlayer]];
               } }
               SelectNamePlateTable.TextureOffset[CurrentPlayer] = BackupNamePlateTable.TextureOffset[(int)PlayerCharacterSelect[CurrentPlayer]];	
               //

               
               OKPSelSkipPortrait[CurrentPlayer] = 0;
               
               //PrintStringNumber(5,5,"",(int)(GlobalStat.PowerDownRT[(int)CharacterConvert[(int)PlayerCharacterSelect[CurrentPlayer] +1]][7] * 1000));
               //PrintStringUnsignedHex(5,15,"",(int)(&GlobalStat.PowerDownRT[0][0]));
               if (PlayerShowStats[CurrentPlayer])
               {
                    //PrintStringNumber(5,5,"PlayerSelect",GlobalIntC);
                    GlobalIntA = PlayerSelectPositions[CurrentPlayer].x ;
                    GlobalIntB = PlayerSelectPositions[CurrentPlayer].y +76;
                    
                    if ((MenuProgress[CurrentPlayer] == 1) && (StatsMode == 2))
                    {
                         if (MenuFlash[CurrentPlayer] > 255)
                         {
                              MenuFlash[CurrentPlayer] = 0;
                         }
                         else
                         {
                              MenuFlash[CurrentPlayer]+=10;
                         }		
                         GraphPtr = FillRect1ColorF(GraphPtr,GlobalIntA - 1,GlobalIntB - 1,GlobalIntA + 66,GlobalIntB + 71,(int)MenuFlash[CurrentPlayer],(int)MenuFlash[CurrentPlayer],(int)MenuFlash[CurrentPlayer],255);	
                                   
                    }
                    GraphPtr = FillRect1ColorF(GraphPtr,GlobalIntA,GlobalIntB,GlobalIntA + 64,GlobalIntB + 69,0,0,0,255);	
                    
                    
                    
                    GlobalIntA = PlayerSelectPositions[CurrentPlayer].x + 3;
                    { int ThisBox; for (ThisBox = 0; ThisBox < 6; ThisBox++)
                    {
                         
                         GlobalIntB = PlayerSelectPositions[CurrentPlayer].y + 88;
                         GraphPtr = FillRect1ColorF(GraphPtr,GlobalIntA,GlobalIntB,GlobalIntA + 8,GlobalIntB + 4,255,255,255,255);
                         GlobalIntB +=17;
                         GraphPtr = FillRect1ColorF(GraphPtr,GlobalIntA,GlobalIntB,GlobalIntA + 8,GlobalIntB + 4,255,255,255,255);
                         GlobalIntB +=17;
                         GraphPtr = FillRect1ColorF(GraphPtr,GlobalIntA,GlobalIntB,GlobalIntA + 8,GlobalIntB + 4,255,255,255,255);
                         GlobalIntB +=17;
                         GraphPtr = FillRect1ColorF(GraphPtr,GlobalIntA,GlobalIntB,GlobalIntA + 8,GlobalIntB + 4,255,255,255,255);
                         GlobalIntA += 10;
                    } }
                    
                    

                    
                    
                    GlobalIntA = PlayerSelectPositions[CurrentPlayer].x + 3;
                    GlobalIntB = PlayerSelectPositions[CurrentPlayer].y +76;

                    GlobalIntB +=12;
                    if (StatsMode != 2)
                    {
                         GlobalIntC = 58 - (58 * 8 * (1 - (GlobalStat.AccelerationCount[RaceClass][(int)CharacterConvert[(int)PlayerCharacterSelect[CurrentPlayer] + 1]] / GlobalStat.AccelerationCount[RaceClass][6])));
                         
                         GraphPtr = FillRect1ColorF(GraphPtr,GlobalIntA,GlobalIntB + 1,GlobalIntA + GlobalIntC,GlobalIntB + 3,255,0,0,255);
                         GlobalIntB +=17;
                         GlobalIntC = 58 + (58 * (1 - (GlobalStat.PowerDownRT[(int)CharacterConvert[(int)PlayerCharacterSelect[CurrentPlayer] +1]][7] / GlobalStat.PowerDownRT[6][7])));
                         
                         GraphPtr = FillRect1ColorF(GraphPtr,GlobalIntA,GlobalIntB + 1,GlobalIntA + GlobalIntC,GlobalIntB + 3,0,255,0,255);
                         GlobalIntB +=17;
                         GlobalIntC = 58 * (GlobalStat.PowerBandAcceleration[(int)CharacterConvert[(int)PlayerCharacterSelect[CurrentPlayer] + 1]] / GlobalStat.PowerBandAcceleration[6]);
                         
                         GraphPtr = FillRect1ColorF(GraphPtr,GlobalIntA,GlobalIntB + 1,GlobalIntA + GlobalIntC,GlobalIntB + 3,0,0,255,255);
                         GlobalIntB +=17;
                         GlobalIntC = 58 + (58 * (1 - (GlobalStat.ProOffsetAngle[(int)CharacterConvert[(int)PlayerCharacterSelect[CurrentPlayer] + 1]] / GlobalStat.ProOffsetAngle[7])));
                         
                         GraphPtr = FillRect1ColorF(GraphPtr,GlobalIntA,GlobalIntB + 1,GlobalIntA + GlobalIntC,GlobalIntB + 3,255,0,0,255);
                    }
                    else
                    {
                         GlobalIntC = 58 - (58 * 8 * (1 - (GlobalStat.AccelerationCount[RaceClass][(int)CharacterConvert[(int)PlayerCharacterSelect[CurrentPlayer] + 1]] / (float)(EngineSpeed[RaceClass][2]))));
                         
                         GraphPtr = FillRect1ColorF(GraphPtr,GlobalIntA,GlobalIntB + 1,GlobalIntA + GlobalIntC,GlobalIntB + 3,255,0,0,255);
                         GlobalIntB +=17;
                         GlobalIntC = 58 + (58 * (1 - (GlobalStat.PowerDownRT[(int)CharacterConvert[(int)PlayerCharacterSelect[CurrentPlayer] +1]][7] /((float)EnginePowerDownRT[1][7] / 100))));
                         
                         GraphPtr = FillRect1ColorF(GraphPtr,GlobalIntA,GlobalIntB + 1,GlobalIntA + GlobalIntC,GlobalIntB + 3,0,255,0,255);
                         GlobalIntB +=17;
                         GlobalIntC = 29 + (29 * (GlobalStat.PowerBandAcceleration[(int)CharacterConvert[(int)PlayerCharacterSelect[CurrentPlayer] + 1]] /((float)PowerBand[1] / 10)));
                         
                         GraphPtr = FillRect1ColorF(GraphPtr,GlobalIntA,GlobalIntB + 1,GlobalIntA + GlobalIntC,GlobalIntB + 3,0,0,255,255);
                         GlobalIntB +=17;
                         GlobalIntC = 58 + (58 * (1 - (GlobalStat.ProOffsetAngle[(int)CharacterConvert[(int)PlayerCharacterSelect[CurrentPlayer] + 1]] /((float)SteerAngle[2] / 100))));
                         
                         GraphPtr = FillRect1ColorF(GraphPtr,GlobalIntA,GlobalIntB + 1,GlobalIntA + GlobalIntC,GlobalIntB + 3,255,0,0,255);					
                    }


                    
                    GlobalIntA = PlayerSelectPositions[CurrentPlayer].x;
                    GlobalIntB = PlayerSelectPositions[CurrentPlayer].y +76;

                    GlobalIntB -= 18;
                    GlobalIntA -= 16;
                    LoadFontF3D((Uint)(&RedPaletteF3D));
                    PrintString(GlobalIntA,GlobalIntB,"Speed");
                    
                    GlobalIntB +=17;
                    LoadFontF3D((Uint)(&GreenPaletteF3D));
                    PrintString(GlobalIntA,GlobalIntB,"Grip");
                    
                    GlobalIntB +=17;
                    LoadFontF3D((Uint)(&BluePaletteF3D));
                    PrintString(GlobalIntA,GlobalIntB,"Boost");
                    
                    GlobalIntB +=17;               
                    LoadFontF3D((Uint)(&RedPaletteF3D));
                    PrintString(GlobalIntA,GlobalIntB,"Steer");
                    
               }	
          } }
     #endif
     
}



void GameSelectMenu(short PlayerIndex)
{
     CheckMenuButton(PlayerIndex);

     switch (MenuButtonHeld[PlayerIndex])
     {
          case BTN_DUP:
          {
               if (GameMenuIndex > 0)
               {
                    GameMenuIndex++;
                    PlaySound(0x49008000);
               }
               else
               {
                    PlaySound(0x49008002);
               }
          }
          case BTN_DDOWN:
          {
               if (GameMenuIndex < 2)
               {
                    GameMenuIndex++;
                    PlaySound(0x49008000);
               }
               else
               {
                    PlaySound(0x49008002);
               }
          }
     }
     switch (GameMenuIndex)
     {
          case PSMode:
          {
               //PlayerSelect
               switch (MenuButtonHeld[PlayerIndex])
               {
                    case BTN_DLEFT:
                    {
                         if (PlayerCount > 1)
                         {
                              PlayerCount--;
                              PlaySound(0x49008000);
                         }
                         else
                         {
                              PlaySound(0x49008002);
                         }
                         
                         break;
                    }
                    case BTN_DRIGHT:
                    {
                         if (PlayerCount < 4)
                         {
                              PlayerCount++;
                         }
                         PlaySound(0x49008000);
                         break;
                    }
               }
          }
     }
     return;
}



void PlayerSelectSwitch(OSContPad *pad,u16 i, u16 newbutton)
{
     if ((GlobalController[i]->ButtonPressed & BTN_R) == BTN_R)
     {
          MenuToggle = !MenuToggle;
     }     
     if (MenuToggle)
     {
		GameOptionsHandler(i);
     }
     else
     {
          PlayerSelectMenu(SaveGame.GameSettings.StatsMode, i);
     }
}

void MapSelectSwitch(Controller *controller, u16 i)
{
	(void) controller; //for compiler warning
	MapSelectMenu(i);
	MSelController(GlobalController[i], i);
}

void GameSelectSwitch(Controller *controller, u16 i)
{
	(void) controller; //for compiler warning
     if (ROptionPressed != 0)
     {    
          if (ROptionPressed + 2 <= GlobalFrameCount)
          {
               ROptionPressed = 0;                 
               KBGNumber = 6;
               KBGNumberNext = 6;
               KBGChange = 1;
               SetFadeOutTaData();
          }
     }
     if ((GlobalController[i]->ButtonPressed & BTN_R) == BTN_R)
     {
          PlaySound(0x49009010);   
          ROptionPressed = GlobalFrameCount;
     }
     else
     {
          //GameSelectMenu(i);
          GSelController(GlobalController[i], i);

     }
     
}

void DataMenuController(Controller *controller, u16 i)
{
	(void) controller;
     ROptionPressed = 0;
     if ((GlobalController[i]->ButtonPressed & BTN_B) == BTN_B)
     {
          KBGNumber = LastMenuID;
          KBGNumberNext = LastMenuID;
          KBGChange = 1;
          SetFadeOutB();
     }
}

short     TitleSwitch, TitleCheck;
int       TitleFrame;

void TitleMenuSwitch(Controller *controller, u16 i)
{
	(void) controller;

     if (TitleCheck != 0)
     {
          if (TitleFrame + 2 <= GlobalFrameCount)
          {
               KBGNumber = TitleCheck;
               KBGNumberNext = TitleCheck;
               TitleCheck = 0;
               TitleFrame = 0;
               SetFadeOutB();
               KBGChange = 1;                         
               return;
          }
     }
     

     
     if 
     ( 
          ((BTN_DLEFT == (GlobalController[i]->ButtonPressed & BTN_DLEFT)) || (BTN_DRIGHT == (GlobalController[i]->ButtonPressed & BTN_DRIGHT))) ||
          ((BTN_DLEFT == (GlobalController[i]->AnalogPressed & BTN_DLEFT)) || (BTN_DRIGHT == (GlobalController[i]->AnalogPressed & BTN_DRIGHT)))
     )
     {
          if (TitleSwitch == 0)
          {
               if (SaveGame.RenderSettings.CullMode == 1)
               {
                    SaveGame.RenderSettings.CullMode = 0;
               }
               else
               {
                    SaveGame.RenderSettings.CullMode = 1;
               }
          }
          else
          {
               if (SaveGame.RenderSettings.Platform == 1)
               {
                    SaveGame.RenderSettings.Platform = 0;
               }
               else
               {
                    SaveGame.RenderSettings.Platform = 1;
               }
          }
     }
     if 
     ( 
          ((BTN_DDOWN == (GlobalController[i]->ButtonPressed & BTN_DDOWN)) || (BTN_DUP == (GlobalController[i]->ButtonPressed & BTN_DUP))) ||
          ((BTN_DDOWN == (GlobalController[i]->AnalogPressed & BTN_DDOWN)) || (BTN_DUP == (GlobalController[i]->AnalogPressed & BTN_DUP)))
     )
     {
          
          if (TitleSwitch == 0)
          {
               TitleSwitch = 1;
          }
          else
          {
               TitleSwitch = 0;
          }
     }
     
     #if OverKartBuild     
     if (CheckCheat())
     {
          PlaySound(0x4900801A);
          TitleCheck = 99;       
          TitleFrame = GlobalFrameCount;   
          
     }
     #endif
     
     if ( (GlobalController[4]->ButtonPressed & BTN_A) || (GlobalController[4]->ButtonPressed & BTN_START) )
     {

          NaSeqFadeout(0x19);
          PlaySound(0x4900801A);
          TitleCheck = 11;
          TitleFrame = GlobalFrameCount;
     }

     
}


void MiniMapDraw()
{    

     if (SaveGame.GameSettings.GameType == 1)
     {
          int CoinCount = 0;
          { int ThisObject; for (ThisObject = 0; ThisObject < 100; ThisObject++)
          {
               if (SimpleObjectArray[ThisObject].Category == 47)
               {
                    float fx;
                    float fz;
                    short DrawX;
                    short DrawY;

                    CoinCount++;
                    fx = (float)(SimpleObjectArray[ThisObject].Position[0]) * MapScale;
                    fz = (float)(SimpleObjectArray[ThisObject].Position[2]) * MapScale;
                    DrawX = MapX[0] + Rzerox - (MapWidth/2) +  StartX + (short)fx;
                    DrawY = MapY[0] + Rzeroy - (MapHeight/2) +  StartY + (short)fz;
                    KWSpriteScale(DrawX,DrawY,0.5,(Ushort*)&RCIconMap,8,8);

                    if (CoinCount == 8)
                    {
                         break;
                    }
               }
               
          } }
          
     }
     
     
     KWReturnViewport();
     KawanoDrawFinal();
}






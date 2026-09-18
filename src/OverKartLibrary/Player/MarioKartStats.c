#include "../MainInclude.h"

extern u8 _mainSegmentStart[];
extern u8 _mainSegmentRomStart[];

static long MainDataRom(void *ram)
{
     return (long) ((uintptr_t) _mainSegmentRomStart +
                    ((uintptr_t) ram - (uintptr_t) _mainSegmentStart));
}

static void RestoreStockSpeedCap(void)
{
     int i;

     for (i = 0; i < 8; i++)
     {
          gKartTopSpeedTable[i] = 9.0f;
     }
}

void SetEngine(int CharacterID, int ThisEngine)
{
     int ThisAccel;
     int ThisPower;

     gTopSpeed50cc[CharacterID] = (float) EngineSpeed[0][ThisEngine];
     gTopSpeed100cc[CharacterID] = (float) EngineSpeed[1][ThisEngine];
     gTopSpeed150cc[CharacterID] = (float) EngineSpeed[2][ThisEngine];
     gTopSpeedExtra[CharacterID] = (float) EngineSpeed[1][ThisEngine];
     gTopSpeedBattle[CharacterID] = (float) EngineSpeed[3][ThisEngine];
     for (ThisAccel = 0; ThisAccel < 10; ThisAccel++)
     {
          gKartAccelerationTables[CharacterID][ThisAccel] =
               ((float) (AccelerationCurve[ThisEngine][ThisAccel]) / 10);
     }
     for (ThisPower = 0; ThisPower < 15; ThisPower++)
     {
          D_800E2EB0[CharacterID][ThisPower] =
               ((float) (EnginePowerDownFT[ThisEngine][ThisPower]) / 100);
          D_800E2E90[CharacterID][ThisPower] =
               ((float) (EnginePowerDownRT[ThisEngine][ThisPower]) / 100);
     }
     gKartTripleABoost[CharacterID] = ((float) PowerBand[ThisEngine] / 10);
     RestoreStockSpeedCap();
}

void SetSteering(int CharacterID, int ThisSteering)
{
     gKartHandlingTable[CharacterID] = ((float) (SteerAngle[ThisSteering]) / 100);
     gKartTurnSpeedReductionTable0[CharacterID] = ((float) (SteerValue[ThisSteering]) / 1000);
     gKartTurnSpeedReductionTable1[CharacterID] = ((float) (SteerValue[ThisSteering]) / 1000);
}

void dmaLoop(int loopCount)
{
     {
         int currentPass;
     for (currentPass = 0; currentPass < loopCount; currentPass++)
     {
          runDMA();
          *targetAddress = *targetAddress + dataLength;
     }
     }
}


void RAMLoop(int loopCount)
{
     {
         int currentPass;
     for (currentPass = 0; currentPass < loopCount; currentPass++)
     {
          runRAM();
          *targetAddress = *targetAddress + dataLength;
     }
     }
}
void Unknown33Func(int mode)
{
     
     switch(mode)
     {
          case 0x00:
          {
               *sourceAddress = MainDataRom(&D_800E2AD0);
               *targetAddress = (int) &D_800E2AD0;
               dataLength = 0x1E0;
               runDMA();
               break;
          }
          case 0x01:
          {
               *sourceAddress = (long)(&Unknown33) + 0x78;               
               dataLength = 0x3C;
               *targetAddress = (int) &D_800E2AD0;
               RAMLoop(8);
               break;
          }
          
     }
}



void Unknown41Func(int mode)
{
     switch(mode)
     {
          case 0x00:
          {
               *sourceAddress = MainDataRom(&D_800E2CB0);
               *targetAddress = (int) &D_800E2CB0;
               dataLength = 0x1E0;
               runDMA();
               break;
          }
          case 0x01:
          {
               *sourceAddress = (long)(&Unknown41) + 0x78;    
               *targetAddress = (int) &D_800E2CB0;
               dataLength = 0x3C;
               RAMLoop(8);
               break;
          }
     }
}


void AccelerationFunc(int mode)
{    
     switch(mode)
     {
          case 0x00:
          {
               *sourceAddress = MainDataRom(&gKartAccelerationMario);
               *targetAddress = (int) &gKartAccelerationMario;
               dataLength = 0x140;
               runDMA();
               break;
          }
          case 0x01:
          {
               *sourceAddress = (int)(&AccelerationTable) + 0x50;
               dataLength = 0x28;
               *targetAddress = (int) &gKartAccelerationMario;
               RAMLoop(8);
               break;
          }
     }
}



void equalStats(int mode)
{
     int i;

     RestoreStockSpeedCap();
     switch (mode)
     {
          case 0x00:
          {
               static const float stockPower50[8] = {3364.0f, 3364.0f, 3457.0f, 3457.0f, 3364.0f, 3364.0f, 3457.0f, 3364.0f};
               static const float stockPower100[8] = {3844.0f, 3844.0f, 3943.0f, 3943.0f, 3844.0f, 3844.0f, 3943.0f, 3844.0f};
               static const float stockPower150[8] = {4096.0f, 4096.0f, 4199.0f, 4199.0f, 4096.0f, 4096.0f, 4199.0f, 4096.0f};
               static const float stockPowerEx[8] = {3844.0f, 3844.0f, 3943.0f, 3943.0f, 3844.0f, 3844.0f, 3943.0f, 3844.0f};
               static const float stockCc50[8] = {290.0f, 290.0f, 294.0f, 294.0f, 290.0f, 290.0f, 294.0f, 290.0f};
               static const float stockCc100[8] = {310.0f, 310.0f, 314.0f, 314.0f, 310.0f, 310.0f, 314.0f, 310.0f};
               static const float stockCc150[8] = {320.0f, 320.0f, 324.0f, 324.0f, 320.0f, 320.0f, 324.0f, 320.0f};
               static const float stockCcEx[8] = {310.0f, 310.0f, 314.0f, 314.0f, 310.0f, 310.0f, 314.0f, 310.0f};
               static const float stockTurnA[8] = {0.0f, 0.0f, 0.002f, 0.002f, -0.002f, -0.002f, 0.002f, -0.002f};
               static const float stockTap[8] = {2.0f, 2.0f, 3.0f, 3.0f, 1.5f, 1.5f, 3.0f, 3.0f};
               static const float stockHandle[8] = {1.25f, 1.25f, 1.28f, 1.28f, 1.15f, 1.15f, 1.28f, 1.15f};

               for (i = 0; i < 8; i++)
               {
                    D_800E24C8[i] = stockPower50[i];
                    D_800E24E8[i] = stockPower100[i];
                    D_800E2508[i] = stockPower150[i];
                    D_800E2528[i] = stockPowerEx[i];
                    gTopSpeed50cc[i] = stockCc50[i];
                    gTopSpeed100cc[i] = stockCc100[i];
                    gTopSpeed150cc[i] = stockCc150[i];
                    gTopSpeedExtra[i] = stockCcEx[i];
                    gKartTurnSpeedReductionTable0[i] = stockTurnA[i];
                    gKartTurnSpeedReductionTable1[i] = stockTurnA[i];
                    gKartTripleABoost[i] = stockTap[i];
                    gKartHandlingTable[i] = stockHandle[i];
               }
               break;
          }
          case 0x01:
          {
               for (i = 0; i < 8; i++)
               {
                    D_800E24C8[i] = 3457.0f;
                    D_800E24E8[i] = 3943.0f;
                    D_800E2508[i] = 4199.0f;
                    D_800E2528[i] = 3943.0f;
                    gTopSpeed50cc[i] = 294.0f;
                    gTopSpeed100cc[i] = 314.0f;
                    gTopSpeed150cc[i] = 324.0f;
                    gTopSpeedExtra[i] = 314.0f;
                    gKartTurnSpeedReductionTable0[i] = 0.002f;
                    gKartTurnSpeedReductionTable1[i] = 0.002f;
                    gKartTripleABoost[i] = 3.0f;
                    gKartHandlingTable[i] = 1.28f;
               }
               break;
          }
     }
     AccelerationFunc(mode);
     Unknown33Func(mode);
     Unknown41Func(mode);
}

void RandomStats(int mode)
{
     GlobalShortA = MakeRandomLimmit(100);
     u11_Mario = 3364 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u11_Luigi = 3364 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u11_Yoshi = 3364 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u11_Toad = 3364 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u11_DK = 3364 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u11_Wario = 3364 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u11_Peach = 3364 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u11_Bowser = 3364 + GlobalShortA;

     GlobalShortA = MakeRandomLimmit(100);
     u12_Mario = 3844 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u12_Luigi = 3844 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u12_Yoshi = 3844 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u12_Toad = 3844 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u12_DK = 3844 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u12_Wario = 3844 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u12_Peach = 3844 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u12_Bowser = 3844 + GlobalShortA;

     GlobalShortA = MakeRandomLimmit(100);
     u13_Mario = 4096 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u13_Luigi = 4096 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u13_Yoshi = 4096 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u13_Toad = 4096 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u13_DK = 4096 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u13_Wario = 4096 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u13_Peach = 4096 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u13_Bowser = 4096 + GlobalShortA;

     GlobalShortA = MakeRandomLimmit(100);
     u14_Mario = 3844 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u14_Luigi = 3844 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u14_Yoshi = 3844 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u14_Toad = 3844 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u14_DK = 3844 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u14_Wario = 3844 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u14_Peach = 3844 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(100);
     u14_Bowser = 3844 + GlobalShortA;


     GlobalShortA = MakeRandomLimmit(6);
     cc50_Mario = 290 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(6);
     cc50_Luigi = 290 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(6);
     cc50_Yoshi = 290 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(6);
     cc50_Toad = 290 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(6);
     cc50_DK = 290 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(6);
     cc50_Wario = 290 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(6);
     cc50_Peach = 290 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(6);
     cc50_Bowser = 290 + GlobalShortA;

     GlobalShortA = MakeRandomLimmit(8);
     cc100_Mario = 310 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(8);
     cc100_Luigi = 310 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(8);
     cc100_Yoshi = 310 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(8);
     cc100_Toad = 310 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(8);
     cc100_DK = 310 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(8);
     cc100_Wario = 310 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(8);
     cc100_Peach = 310 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(8);
     cc100_Bowser = 310 + GlobalShortA;

     GlobalShortA = MakeRandomLimmit(10);
     cc150_Mario = 320 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(10);
     cc150_Luigi = 320 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(10);
     cc150_Yoshi = 320 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(10);
     cc150_Toad = 320 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(10);
     cc150_DK = 320 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(10);
     cc150_Wario = 320 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(10);
     cc150_Peach = 320 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(10);
     cc150_Bowser = 320 + GlobalShortA;

     GlobalShortA = MakeRandomLimmit(8);
     ccextra_Mario = 310 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(8);
     ccextra_Luigi = 310 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(8);
     ccextra_Yoshi = 310 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(8);
     ccextra_Toad = 310 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(8);
     ccextra_DK = 310 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(8);
     ccextra_Wario = 310 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(8);
     ccextra_Peach = 310 + GlobalShortA;
     GlobalShortA = MakeRandomLimmit(8);
     ccextra_Bowser = 310 + GlobalShortA;



     GlobalShortA = MakeRandomLimmit(8);
     turncoA_Mario = -0.004f * ((float)GlobalShortA * 0.001f);
     GlobalShortA = MakeRandomLimmit(8);
     turncoA_Luigi = -0.004f * ((float)GlobalShortA * 0.001f);
     GlobalShortA = MakeRandomLimmit(8);
     turncoA_Yoshi = -0.004f * ((float)GlobalShortA * 0.001f);
     GlobalShortA = MakeRandomLimmit(8);
     turncoA_Toad = -0.004f * ((float)GlobalShortA * 0.001f);
     GlobalShortA = MakeRandomLimmit(8);
     turncoA_DK = -0.004f * ((float)GlobalShortA * 0.001f);
     GlobalShortA = MakeRandomLimmit(8);
     turncoA_Wario = -0.004f * ((float)GlobalShortA * 0.001f);
     GlobalShortA = MakeRandomLimmit(8);
     turncoA_Peach = -0.004f * ((float)GlobalShortA * 0.001f);
     GlobalShortA = MakeRandomLimmit(8);
     turncoA_Bowser = -0.004f * ((float)GlobalShortA * 0.001f);

     GlobalShortA = MakeRandomLimmit(8);
     turncoB_Mario = -0.004f * ((float)GlobalShortA * 0.001f);
     GlobalShortA = MakeRandomLimmit(8);
     turncoB_Luigi = -0.004f * ((float)GlobalShortA * 0.001f);
     GlobalShortA = MakeRandomLimmit(8);
     turncoB_Yoshi = -0.004f * ((float)GlobalShortA * 0.001f);
     GlobalShortA = MakeRandomLimmit(8);
     turncoB_Toad = -0.004f * ((float)GlobalShortA * 0.001f);
     GlobalShortA = MakeRandomLimmit(8);
     turncoB_DK = -0.004f * ((float)GlobalShortA * 0.001f);
     GlobalShortA = MakeRandomLimmit(8);
     turncoB_Wario = -0.004f * ((float)GlobalShortA * 0.001f);
     GlobalShortA = MakeRandomLimmit(8);
     turncoB_Peach = -0.004f * ((float)GlobalShortA * 0.001f);
     GlobalShortA = MakeRandomLimmit(8);
     turncoB_Bowser = -0.004f * ((float)GlobalShortA * 0.001f);

     GlobalShortA = MakeRandomLimmit(30);
     tripleTap_Mario = 1.0f + ((float)GlobalShortA * 0.10f);
     GlobalShortA = MakeRandomLimmit(30);
     tripleTap_Luigi = 1.0f + ((float)GlobalShortA * 0.10f);
     GlobalShortA = MakeRandomLimmit(30);
     tripleTap_Yoshi = 1.0f + ((float)GlobalShortA * 0.10f);
     GlobalShortA = MakeRandomLimmit(30);
     tripleTap_Toad = 1.0f + ((float)GlobalShortA * 0.10f);
     GlobalShortA = MakeRandomLimmit(30);
     tripleTap_DK = 1.0f + ((float)GlobalShortA * 0.10f);
     GlobalShortA = MakeRandomLimmit(30);
     tripleTap_Wario = 1.0f + ((float)GlobalShortA * 0.10f);
     GlobalShortA = MakeRandomLimmit(30);
     tripleTap_Peach = 1.0f + ((float)GlobalShortA * 0.10f);
     GlobalShortA = MakeRandomLimmit(30);
     tripleTap_Bowser = 1.0f + ((float)GlobalShortA * 0.10f);


     GlobalShortA = MakeRandomLimmit(3);
     tripleTap_Mario = 1.0f + ((float)GlobalShortA * 0.10f);
     GlobalShortA = MakeRandomLimmit(3);
     tripleTap_Luigi = 1.0f + ((float)GlobalShortA * 0.10f);
     GlobalShortA = MakeRandomLimmit(3);
     tripleTap_Yoshi = 1.0f + ((float)GlobalShortA * 0.10f);
     GlobalShortA = MakeRandomLimmit(3);
     tripleTap_Toad = 1.0f + ((float)GlobalShortA * 0.10f);
     GlobalShortA = MakeRandomLimmit(3);
     tripleTap_DK = 1.0f + ((float)GlobalShortA * 0.10f);
     GlobalShortA = MakeRandomLimmit(3);
     tripleTap_Wario = 1.0f + ((float)GlobalShortA * 0.10f);
     GlobalShortA = MakeRandomLimmit(3);
     tripleTap_Peach = 1.0f + ((float)GlobalShortA * 0.10f);
     GlobalShortA = MakeRandomLimmit(3);
     tripleTap_Bowser = 1.0f + ((float)GlobalShortA * 0.10f);

     AccelerationFunc(1);
     Unknown33Func(1);
     Unknown41Func(1);



}


void checkStats(int mode)
{
     if (mode != StatsID)
     {
          equalStats(mode);
          StatsID = mode;
     }
}

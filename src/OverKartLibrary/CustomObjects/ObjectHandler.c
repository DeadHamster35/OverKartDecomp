#include "../MainInclude.h"



void BombThrowRolloverWrap(Player* Kart, char Kno)
{
	int PlayerID = (*(long*)&Kart - (long)&g_PlayerStructTable) / 0xDD8;
		
	SetBombThrowRollover(Kart,PlayerID);
}


void RolloverWrap(Player* Kart, char Kno)
{
	int PlayerID = (*(long*)&Kart - (long)&g_PlayerStructTable) / 0xDD8;
		
	SetRollover(Kart,PlayerID);
}


void WheelSpinWrap(Player* Kart, char Kno)
{
	int PlayerID = (*(long*)&Kart - (long)&g_PlayerStructTable) / 0xDD8;
		
	SetWheelspin(Kart,PlayerID);
}

void BrokenWrap(Player* Kart, char Kno)
{
	int PlayerID = (*(long*)&Kart - (long)&g_PlayerStructTable) / 0xDD8;
		
	SetBroken(Kart,PlayerID);
	
}

void ThunderWrap(Player* Kart, char Kno)
{
	int PlayerID = (*(long*)&Kart - (long)&g_PlayerStructTable) / 0xDD8;
		
	SetThunder(Kart,PlayerID);
}

void SpinWrap(Player* Kart, char Kno)
{
	int PlayerID = (*(long*)&Kart - (long)&g_PlayerStructTable) / 0xDD8;
		
	SetSpin(Kart,PlayerID);
}
void BombRolloverWrap(Player* Kart, char Kno)
{
	int PlayerID = (*(long*)&Kart - (long)&g_PlayerStructTable) / 0xDD8;
		
	SetBombRollover(Kart,PlayerID);
}

void ProWheelSpinWrap(Player* Kart, char Kno)
{
	int PlayerID = (*(long*)&Kart - (long)&g_PlayerStructTable) / 0xDD8;
		
	SetProWheelSpin(Kart,PlayerID);
}

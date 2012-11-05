// TankAIBot.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CTankAIBotRandom.h"
#include "CTankAIBotRamming.h"



int _tmain(int argc, _TCHAR* argv[])
{
	// CTankAIBotRandom tank; 
	CTankAIBotRamming tank; 
	tank.Run(); 

	return 0;
}


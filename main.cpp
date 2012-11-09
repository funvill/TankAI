// TankAIBot.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CTankAIServer.h"
#include "CTankAIBotRandom.h"
#include "CTankAIBotRamming.h"
#include "CTankAIBotCow.h"




int _tmain(int argc, _TCHAR* argv[])
{
	CTankAIServer server ; 

	
	// server.AddPlayer( new CTankAIBotCow() ); 
	server.AddPlayer( new CTankAIBotCow() ); 
	server.AddPlayer( new CTankAIBotRandom()  ); 
	server.AddPlayer( new CTankAIBotRamming() ); 
	
	server.Go(); 

	return 0;
}


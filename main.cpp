/******************************************************************************
 *  Tank AI Programming challenge
 *
 *  Written by: Steven Smethurst 
 *  Created: Nov 04, 2012 
 * 
 ******************************************************************************/


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

	// 10 of each bot 
	for( int i = 0 ; i < 10 ; i++ ) {
		server.AddBot( new CTankAIBotCow() ); 
		server.AddBot( new CTankAIBotRandom() ); 
		server.AddBot( new CTankAIBotRamming() ); 
	}
	server.Go(); 

	return 0;
}


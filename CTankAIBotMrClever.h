/******************************************************************************
 *  Tank AI Programming challenge
 *
 *  Written by: Steven Smethurst 
 *  Created: Nov 04, 2012 
 * 
 ******************************************************************************/

#pragma once
#include "helpers.h"
#include "ctankaibase.h"
#include "CTankAction.h"

/**
 * Compair two pings to each other to test for movement of projecties. 
 * if projectiles are with in 10 tiles on X or Y, move out of the way. 
 * if tank is with in 10 spaces on x or y fire projectiles 
 * if no tanks are with in 10 spaces move towards the closest one. 
 */ 

class CTankAIBotMrClever : public CTankAIBase
{
	private:

		CTankAction		m_nextAction; 
		unsigned char	m_pingRate ; 
		unsigned int	m_turnCount;


		void UpdateTankAI( );
		unsigned short GetDistance( char x, char y) ; 

	public:
		CTankAIBotMrClever(void);
		std::string Go( std::string command ) ; 
		std::string const GetName() { return "BotMrClever"; } ;	
		void Reset(); 
};


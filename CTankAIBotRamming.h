/******************************************************************************
 *  Tank AI Programming challenge
 *
 *  Written by: Steven Smethurst 
 *  Created: Nov 04, 2012 
 * 
 ******************************************************************************/

#pragma once
#include "ctankaibase.h"
#include "CTankAction.h"

class CTankAIBotRamming : public CTankAIBase
{
	private:
		
		CTankAction::DirectionsEnum m_enemyDirection ; 
		unsigned char m_pingRate; 

		unsigned int CalculateDistance ( const char x, const char y); 	
		void UpdateEnemyDirection() ; 

	public:
		CTankAIBotRamming(); 
		std::string Go( std::string command ) ; 
		std::string GetName() { return "BotRamming"; } ;
	
};


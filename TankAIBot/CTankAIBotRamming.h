/******************************************************************************
 *  Tank AI Programming challenge
 *
 *  Written by: Steven Smethurst 
 *  Created: Nov 04, 2012 
 * 
 ******************************************************************************/

#pragma once
#include "ctankaibase.h"
class CTankAIBotRamming :
	public CTankAIBase
{

private:
	DirectionsEnum m_enemyDirection ; 
	unsigned int CalculateDistance ( char x, char y); 	

public:

	void Run(); 
};


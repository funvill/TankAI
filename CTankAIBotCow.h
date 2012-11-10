/******************************************************************************
 *  Tank AI Programming challenge
 *
 *  Written by: Steven Smethurst 
 *  Created: Nov 08, 2012 
 * 
 ******************************************************************************/

#pragma once
#include "ctankaibase.h"
#include "CTankAction.h"
#include "CTankAIBotCow.h"

class CTankAIBotCow : public CTankAIBase
{
	private:
		CTankAction::DirectionsEnum GetRandomDirection(); 

	public:
		std::string Go( std::string command ) ; 
		std::string GetName() { return "BotCow"; } ;
};


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

class CTankAIBotRandom : public CTankAIBase
{
private:
	CTankAction::DirectionsEnum GetRandomDirection(); 

public:
	std::string Go( std::string command ) ; 
};


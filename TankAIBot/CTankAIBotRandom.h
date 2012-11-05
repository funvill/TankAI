/******************************************************************************
 *  Tank AI Programming challenge
 *
 *  Written by: Steven Smethurst 
 *  Created: Nov 04, 2012 
 * 
 ******************************************************************************/
#pragma once
#include "ctankaibase.h"

class CTankAIBotRandom : public CTankAIBase
{
private:
	DirectionsEnum GetRandomDirection(); 

public:
	void Run(); 
};


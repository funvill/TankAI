#pragma once

#include "ctankaibase.h"
#include "CTankAction.h"
#include "CTankAIBotCow.h"

class CTankAIBotRock : public CTankAIBase
{
public:
	CTankAIBotRock(void) {
		this->m_actions.Set( CTankAction::PING ); 
	}

	std::string Go( std::string command ) {
		return this->m_actions.Encode(); 
	}; 
	std::string const GetName() { return "BotRock"; } ;
};


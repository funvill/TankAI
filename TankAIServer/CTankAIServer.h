#pragma once

#include "CTank.h"
#include "CTankAIBase.h"

#include <string>

class CTankAIServer
{
private:
	CTank m_playerA ; 
	CTank m_playerB ; 

	void DoCommand( std::string commandLine ); 
	void DoResponse( CTankAIBase::ResponseEnum response );

public:

	void Reset(); 
	void Run(); 

};


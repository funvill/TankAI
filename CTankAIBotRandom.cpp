/******************************************************************************
 *  Tank AI Programming challenge
 *
 *  Written by: Steven Smethurst 
 *  Created: Nov 04, 2012 
 * 
 ******************************************************************************/
#include "CTankAIBotRandom.h"

CTankAction::DirectionsEnum CTankAIBotRandom::GetRandomDirection() {
	// Get a random direction.
	switch( rand() % 4 ) {
		default: 
		case 0 :
			return CTankAction::NORTH;
			break; 
		case 1 :
			return CTankAction::EAST; 
			break; 
		case 2 :
			return CTankAction::SOUTH; 
			break; 
		case 3 :
			return CTankAction::WEST;
			break; 
	};
}

std::string CTankAIBotRandom::Go( std::string command ) {
	switch( rand() % 3 ) 
	{
		default: 
		case 0: {
			// Move
			this->m_actions.Set( CTankAction::MOVE, this->GetRandomDirection() ); 
			break;
		}
		case 1: {
			// shoot
			this->m_actions.Set( CTankAction::SHOOT, this->GetRandomDirection() ); 
			break;
		}
		case 2: {
			// Ping
			this->m_actions.Set( CTankAction::PING ); 
			break;
		}
	}

	return this->m_actions.Encode(); 
}
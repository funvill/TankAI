/******************************************************************************
 *  Tank AI Programming challenge
 *
 *  Written by: Steven Smethurst 
 *  Created: Nov 04, 2012 
 * 
 ******************************************************************************/

#include "CTankAIBotCow.h"

CTankAction::DirectionsEnum CTankAIBotCow::GetRandomDirection() {
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

std::string CTankAIBotCow::Go( std::string command ) {
	this->m_actions.Set( CTankAction::MOVE, this->GetRandomDirection() ); 
	// this->m_actions.Set( CTankAction::SHOOT, CTankAction::NORTH ); 
	// this->m_actions.Set( CTankAction::PING ); 
	return this->m_actions.Encode(); 
}
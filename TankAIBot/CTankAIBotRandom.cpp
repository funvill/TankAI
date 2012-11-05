/******************************************************************************
 *  Tank AI Programming challenge
 *
 *  Written by: Steven Smethurst 
 *  Created: Nov 04, 2012 
 * 
 ******************************************************************************/
#include "CTankAIBotRandom.h"

CTankAIBotRandom::DirectionsEnum CTankAIBotRandom::GetRandomDirection() {
	// Get a random direction.
	switch( rand() % 3 ) {
		case 0 :
			return CTankAIBase::NORTH;
			break; 
		case 1 :
			return CTankAIBase::EAST; 
			break; 
		case 2 :
			return CTankAIBase::SOUTH; 
			break; 
		case 3 :
			return CTankAIBase::WEST;
			break; 
	};
}

void CTankAIBotRandom::Run() {

	bool done = false; 
	while( ! done ) {

		switch( rand() % 3 ) {
			case 0: {
				// Move
				this->ActionMove( this->GetRandomDirection() ); 
				break;
			}
			case 1: {
				// shoot
				this->ActionShoot( this->GetRandomDirection() ); 
				break;
			}
			case 2: {
				// Ping
				this->ActionRadar( ); 
				break;
			}
		}

		
		CTankAIBase::ResponseEnum response = this->CheckResponse(); 
		if( response == CTankAIBase::ResponseEnum::PASS ) {
			// ToDo: something. 
		} else {
			// ToDo: Something has gone wrong. 
			done = true; 
		}
	}

}
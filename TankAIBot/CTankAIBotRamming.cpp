/******************************************************************************
 *  Tank AI Programming challenge
 *
 *  Written by: Steven Smethurst 
 *  Created: Nov 04, 2012 
 * 
 ******************************************************************************/

#include "CTankAIBotRamming.h"
#include <stdio.h>
#include <stdlib.h>

unsigned int CTankAIBotRamming::CalculateDistance ( char x, char y) {
	return abs( x ) + abs( y ) ; 

}

void CTankAIBotRamming::Run() {
	
	unsigned int turnCount = 0 ; 

	bool done = false; 
	while( ! done ) {

		// First turn: Ping to update the location of the closest enemies
		// Every 5 turns: Ping to update the location of the closest enemies
		// Every 6 turns: Fire towards the enemy 
		// Else move. 

		if( turnCount % 5 == 0 || turnCount== 0 ) {
			// Pint to update the map 
			// The first turn we should ping to find the location of all the enemies. 
			this->ActionRadar(); 
		} else if( turnCount % 6 == 0 ) {
			// Shoot 
			this->ActionShoot( this->m_enemyDirection ); 
		} else {
			// Move 
			this->ActionMove( this->m_enemyDirection ); 
		}

		// Handle the response. 
		CTankAIBase::ResponseEnum response = this->CheckResponse(); 
		if( response == CTankAIBase::ResponseEnum::PASS ) {
			// We got a good reponse. 

			// We just did a radar ping. Update the direction 
			if( turnCount % 5 == 0 || turnCount== 0 ) {

				char distance ; 
				char dx, dy ; 

				// Loop though all the objects on the map to find the closest enemy. 
				for( std::vector<CTankAIObject>::iterator it = this->m_objects.begin() ; it != this->m_objects.end() ; it++ ) {
					// Find the cloest enemy
					if( (*it).m_type == CTankAIObject::enemy ) {
						// If this is the first enemy or 
						// if this enemy is closer then the last closest one. 
						if( it == this->m_objects.begin() || 
							distance > this->CalculateDistance( (*it).m_dx, (*it).m_dy )) 
						{
							dx = (*it).m_dx ; 
							dy = (*it).m_dy ; 
							distance = this->CalculateDistance( (*it).m_dx, (*it).m_dy ) ; 
						}
					}					
				}

				// Update the direction. 
				if( abs( dx ) > abs( dy ) ) {
					// East or west 
					if( dx > 0 ) {
						this->m_enemyDirection = CTankAIBase::EAST ; 
					} else {
						this->m_enemyDirection = CTankAIBase::WEST ; 
					}
				} else {
					// North or south 
					if( dy > 0 ) {
						this->m_enemyDirection = CTankAIBase::NORTH ; 
					} else {
						this->m_enemyDirection = CTankAIBase::SOUTH ; 
					}
				}

			}

		} else {
			// Something has gone wrong. 
			printf( "This bot got a response that it does not understand. die" ); 
			done = true; 
		}

		// Next turn. 
		turnCount++; 
	}
}
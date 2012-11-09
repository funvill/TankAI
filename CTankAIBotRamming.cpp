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

CTankAIBotRamming::CTankAIBotRamming() {
	this->m_enemyDirection = CTankAction::NORTH ; 
}
unsigned int CTankAIBotRamming::CalculateDistance ( char x, char y) {
	return abs( x ) + abs( y ) ; 

}
std::string CTankAIBotRamming::Go( std::string serverResponse ) 
{
	// Check to see if we got a message and update the battle map if we did. 
	this->CheckResponse( serverResponse ) ; 

	// Check to see if we have updated the battle map and where the new enemy is. 
	this->UpdateEnemyDirection(); 

	// Actions 
	// First turn: Ping to update the location of the closest enemies
	// Every 5 turns: Ping to update the location of the closest enemies
	// Every 6 turns: Fire towards the enemy 
	// Else move. 

	static unsigned char turnCount = 0 ; 
	turnCount++; 

	if( turnCount % 5 == 0 || turnCount == 1 ) {
		// Ping to update the map 
		// The first turn we should ping to find the location of all the enemies. 
		this->m_actions.Set( CTankAction::PING ); 
	} else if( turnCount % 6 == 0 ) {
		// Shoot 
		this->m_actions.Set( CTankAction::SHOOT, this->m_enemyDirection ); 
	} else {
		// Move 
		this->m_actions.Set( CTankAction::MOVE, this->m_enemyDirection ); 
	}

	return this->m_actions.Encode(); 
	
}

void CTankAIBotRamming::UpdateEnemyDirection() {

	// Only update the direction if we have recently done a radar ping
	if( this->m_objects.size() <= 0 ) {
		return ; // Nothing to do. 
	}

	unsigned int distance = 0 ; 
	char dx, dy ; 

	// Loop though all the objects on the map to find the closest enemy. 
	for( std::vector<CTankAIObject>::iterator it = this->m_objects.begin() ; it != this->m_objects.end() ; it++ ) {
		// Find the cloest enemy
		if( (*it).m_type == CTankAIObject::tank ) {
			// If this is the first enemy or 
			// if this enemy is closer then the last closest one. 
			if( it == this->m_objects.begin() || 
				distance > this->CalculateDistance( (*it).m_x, (*it).m_y )) 
			{
				dx = (*it).m_x ; 
				dy = (*it).m_y ; 
				distance = this->CalculateDistance( (*it).m_x, (*it).m_y ) ; 
			}
		}					
	}

	// Update the direction. 
	if( abs( dx ) > abs( dy ) ) {
		// East or west 
		if( dx > 0 ) {
			this->m_enemyDirection = CTankAction::EAST ; 
		} else {
			this->m_enemyDirection = CTankAction::WEST ; 
		}
	} else {
		// North or south 
		if( dy > 0 ) {
			this->m_enemyDirection = CTankAction::NORTH ; 
		} else {
			this->m_enemyDirection = CTankAction::SOUTH ; 
		}
	}
}
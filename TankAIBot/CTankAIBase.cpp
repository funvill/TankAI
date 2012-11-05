/******************************************************************************
 *  Tank AI Programming challenge
 *
 *  Written by: Steven Smethurst 
 *  Created: Nov 04, 2012 
 * 
 ******************************************************************************/

#include "CTankAIBase.h"
#include <iostream>


void CTankAIBase::DoAction( std::string action, std::string commands ) {
	printf( "%s %s\n", action.c_str(), commands.c_str() ); 
}
std::string CTankAIBase::DirectionToString( DirectionsEnum dir ) {
	switch( dir ) {
		case CTankAIBase::NORTH: {
			return "north" ;
			break ;
		}
		case CTankAIBase::EAST: {
			return "east" ;
			break ;
		}
		case CTankAIBase::SOUTH: {
			return "south" ;
			break ;
		}
		case CTankAIBase::WEST: {
			return "west" ;
			break ;
		}
	}
}

void CTankAIBase::ActionMove( CTankAIBase::DirectionsEnum dir ) {
	this->DoAction( TANK_ACTION_MOVE, this->DirectionToString( dir ) ); 
}
void CTankAIBase::ActionShoot( CTankAIBase::DirectionsEnum dir ) {
	this->DoAction( TANK_ACTION_SHOOT, this->DirectionToString( dir ) ); 
}
void CTankAIBase::ActionRadar() {
	this->DoAction( TANK_ACTION_RADAR, "" ); 
}
CTankAIBase::ResponseEnum CTankAIBase::CheckResponse() {
	
	
	
	std::string response ; 
	// std::cin >> response ;

	// ToDo: if this is a radar ping then there will be information about all the other objects in the map. 
	// Fake data 
	m_objects.clear(); 
	m_objects.push_back( CTankAIObject( CTankAIObject::enemy, (char ) rand() % 256 , (char ) rand() % 256 ) ); 
	m_objects.push_back( CTankAIObject( CTankAIObject::projectile, (char ) rand() % 256 , (char ) rand() % 256 ) ); 

	return CTankAIBase::PASS ; // Debug 

	if( response.compare("pass") == 0 ) {
		return CTankAIBase::PASS ;
	} else if( response.compare("fail") == 0 ) {
		return CTankAIBase::FAIL ;
	} else {
		printf("Error: Could not understand response. response=[%s]\n", response.c_str() );
		return CTankAIBase::FAIL ;
	}
}

/******************************************************************************
 *  Tank AI Programming challenge
 *
 *  Written by: Steven Smethurst 
 *  Created: Nov 04, 2012 
 * 
 ******************************************************************************/

#include "CTankAIBase.h"
#include <iostream>
#include <regex>

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
		default:
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
CTankAIBase::ResponseEnum CTankAIBase::CheckResponse() 
{	
	std::string response ; 
	// std::cin >> response ;

	// Debug 
	// Example response 
	// response = TANK_RESPONSE_PASS ; 
	// response = TANK_RESPONSE_FAIL ; 
	response = std::string ( TANK_RESPONSE_PASS ) + std::string( " enemy(1233,-3,100), projectile(1234,-5,100), projectile(1235,-15,100), projectile(1236,-10,100) \n" ) ; 

	// Check the response. 
	if( response.find( TANK_RESPONSE_PASS ) == std::string::npos ) {
		printf("Error: Could not understand response. response=[%s]\n", response.c_str() );
		return CTankAIBase::FAIL ;
	}
	// This is a good response. 
	// Check for radar ping response and populate the m_objects with the information. 
	this->ProcessRadarResponse( response );	

	// Everything looks good. 
	return CTankAIBase::PASS ;
}

void CTankAIBase::ProcessRadarResponse( std::string response ) 
{
	// Clear the object list as it is no longer valid. 
	m_objects.clear(); 

	unsigned int UUID ;
	CTankAIObject::TypeEnum type;
	char dx; 
	char dy;

    // regular expression
	const std::regex pattern("(projectile|enemy)\\(([0-9]+),-?([0-9]+),-?([0-9]+)\\)");
	const std::regex patternNumber("-?([0-9]+)");

    const std::sregex_token_iterator end;
	for (std::sregex_token_iterator it(response.cbegin(), response.cend(), pattern);  it != end; ++it) {
		
		// Token to string. 
		std::string singleObject = (*it) ; 

		// Find the type 
		if( singleObject.find( TANK_OBJECT_TYPE_ENEMY ) != std::string::npos ) {
			type = CTankAIObject::enemy ; 
		} else if( singleObject.find( TANK_OBJECT_TYPE_PROJECTILE ) != std::string::npos ) {
			type = CTankAIObject::projectile ; 
		} else {
			// Unknown type 
			continue; 
		}

		// Loop thought the prameters extracting the UUID, dx, dy. 
		int offset = 0 ; 
		const std::sregex_token_iterator end2;
		for (std::sregex_token_iterator it2( singleObject.cbegin(), singleObject.cend(), patternNumber);  it2 != end2; ++it2) {
		
			switch( offset ) {
				case 0:
					UUID = atoi( (*it2).str().c_str() ); 
					break; 
				case 1:
					dx = (char ) atoi( (*it2).str().c_str() ); 
					break; 
				case 2:
					dy = (char ) atoi( (*it2).str().c_str() ); 
					break; 
			}
			offset++; 
		}
		this->m_objects.push_back( CTankAIObject( UUID, type,	dx , dy ) ); 
		// std::cout << singleObject << "\n" ; // Debug 
	}

}
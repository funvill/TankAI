/******************************************************************************
 *  Tank AI Programming challenge
 *
 *  Written by: Steven Smethurst 
 *  Created: Nov 04, 2012 
 * 
 ******************************************************************************/
#pragma once

#include <string>

#define	TANK_ACTION_MOVE	"move"
#define	TANK_ACTION_SHOOT	"shoot"
#define	TANK_ACTION_RADAR	"radar"

#define TANK_RESPONSE_PASS	"pass"
#define TANK_RESPONSE_FAIL	"fail"

class CTankAIBase
{
	public:

		enum DirectionsEnum {
			NORTH, EAST, SOUTH, WEST
		};

		enum ResponseEnum {
			PASS, FAIL
		};

		void ActionMove	( DirectionsEnum dir ) ; 
		void ActionShoot( DirectionsEnum dir ) ; 
		void ActionRadar();
		ResponseEnum CheckResponse(); 

	private:
		void DoAction( std::string action, std::string commands ); 
		std::string DirectionToString( DirectionsEnum dir ); 

};


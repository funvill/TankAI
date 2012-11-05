/******************************************************************************
 *  Tank AI Programming challenge
 *
 *  Written by: Steven Smethurst 
 *  Created: Nov 04, 2012 
 * 
 ******************************************************************************/
#pragma once

#include <string>
#include <vector>

#define	TANK_ACTION_MOVE	"move"
#define	TANK_ACTION_SHOOT	"shoot"
#define	TANK_ACTION_RADAR	"radar"

#define TANK_RESPONSE_PASS	"pass"
#define TANK_RESPONSE_FAIL	"fail"

class CTankAIObject 
{
public:
	enum TypeEnum {
		enemy, projectile
	};

	TypeEnum m_type ; 
	char m_dx ; 
	char m_dy ; 

	void Set( TypeEnum type, char dx, char dy ) {
		this->m_type = type ; 
		this->m_dx = dx ; 
		this->m_dy = dy ; 
	}

	CTankAIObject( TypeEnum type, char dx, char dy ) {
		this->Set( type, dx, dy ); 
	}

};

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

		std::vector<CTankAIObject> m_objects; 

	private:
		void DoAction( std::string action, std::string commands ); 
		std::string DirectionToString( DirectionsEnum dir ); 

};


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
#include "CTankAction.h"

#define TANK_OBJECT_TYPE_TANK		"tank"
#define TANK_OBJECT_TYPE_PROJECTILE	"projectile"

// This is a global var used to give evert object in the game a uniquee number. 
extern unsigned int g_UUID ; 

class CTankAIObject 
{
public:
	enum TypeEnum {
		tank, projectile
	};

	unsigned int m_UUID ; 
	TypeEnum m_type ; 
	char m_x ; 
	char m_y ; 

	void Set( unsigned int UUID, TypeEnum type, char x, char y ) {
		this->m_UUID = UUID ; 
		this->m_type = type ; 
		this->m_x = x ; 
		this->m_y = y ; 
	}

	CTankAIObject() {
		this->m_UUID = g_UUID++; 

	}
	CTankAIObject( unsigned int UUID, TypeEnum type, char x, char y ) {
		CTankAIObject() ; 
		this->Set( UUID, type, x, y ); 
	}

};



class CTankAIBase
{
	public:
		
		// These just make calling the commands easier. 
		CTankAction m_actions; 
		
		// List of all the objects that we can currently see on the map 
		std::vector<CTankAIObject> m_objects; 

		/**
		 * This is the main function that you are required to overload. 
		 * 
		 */ 
		virtual std::string Go( std::string serverResponse = "" ) = 0 ; 
		virtual const std::string GetName() = 0 ; 
		virtual void Reset() {} ; 

		// This will process the response and check for error messages. 
		void CheckResponse( std::string response ); 		
		
};


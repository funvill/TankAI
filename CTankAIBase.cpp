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



void CTankAIBase::CheckResponse( std::string response  ) 
{	
	// Debug 
	// Example response 
	// response = TANK_RESPONSE_PASS ; 
	// response = TANK_RESPONSE_FAIL ; 
	// response = std::string ( TANK_RESPONSE_PASS ) + std::string( " enemy(1233,-3,100), projectile(1234,-5,100), projectile(1235,-15,100), projectile(1236,-10,100) \n" ) ; 

	// Check the response
	if( response.size() <= 0 ) {
		// Nothing to do. 
		return ;
	}

	// This is a good response. 
	// Check for radar ping response and populate the m_objects with the information. 
	// Clear the object list as it is no longer valid. 
	m_objects.clear(); 

	unsigned int UUID ;
	CTankAIObject::TypeEnum type;
	char x; 
	char y;

    // regular expression
	// Example:  enemy(1233,-3,100), projectile(1234,-5,100), projectile(1235,-15,100), projectile(1236,-10,100) 
	const std::regex pattern("(projectile|enemy)\\(([0-9]+),-?([0-9]+),-?([0-9]+)\\)");
	const std::regex patternNumber("-?([0-9]+)");

    const std::sregex_token_iterator end;
	for (std::sregex_token_iterator it(response.cbegin(), response.cend(), pattern);  it != end; ++it) {
		
		// Token to string. 
		std::string singleObject = (*it) ; 

		// Find the type 
		if( singleObject.find( TANK_OBJECT_TYPE_ENEMY ) != std::string::npos ) {
			type = CTankAIObject::tank ; 
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
					x = (char ) atoi( (*it2).str().c_str() ); 
					break; 
				case 2:
					y = (char ) atoi( (*it2).str().c_str() ); 
					break; 
			}
			offset++; 
		}

		

		this->m_objects.push_back( CTankAIObject( UUID, type, x , y ) ); 
		// std::cout << singleObject << "\n" ; // Debug 
	}
}



/******************************************************************************
 *  Tank AI Programming challenge
 *
 *  Written by: Steven Smethurst 
 *  Created: Nov 04, 2012 
 * 
 ******************************************************************************/

#pragma once

#include <string> 
#include <iostream>
#include <regex>

class CTankAction
{
	public:
		enum ActionEnum {
			SHOOT=0, MOVE, PING
		};

		enum DirectionsEnum {
			NORTH=0, EAST, SOUTH, WEST, NONE
		};

		ActionEnum		m_action; 
		DirectionsEnum	m_direction; 

		CTankAction();
		void Set( const ActionEnum action, const DirectionsEnum direction = DirectionsEnum::NONE ) ; 

		bool Decode( const std::string command ); 
		std::string Encode(); 

		

	private:

		// Helpers 
		std::string DirectionToString( const DirectionsEnum direction ); 
		std::string ActionToString( const ActionEnum action ); 

		bool StringToAction( std::string value, ActionEnum &action ); 
		bool StringToDirection( std::string value, DirectionsEnum  &direction ); 



		
};


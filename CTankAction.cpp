#include "CTankAction.h"
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

// trim from start
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}



const char* kTankAIActionNames[3] = {
	"shoot", "move", "ping" 
};
const char* kTankAIDirectionsNames[5] = {
	"north", "east", "south", "west", "" 
};


CTankAction::CTankAction() {
	this->m_action = CTankAction::MOVE ; 
	this->m_direction = CTankAction::NONE ;
}


std::string CTankAction::ActionToString( const CTankAction::ActionEnum action ) {
	return kTankAIActionNames[ action ]  ; 
}
std::string CTankAction::DirectionToString( const CTankAction::DirectionsEnum direction ) {
	return kTankAIDirectionsNames[ direction ]  ; 
}

void CTankAction::Set( const ActionEnum action, const DirectionsEnum direction ) {
	this->m_action = action ; 
	this->m_direction = direction ; 
}
std::string CTankAction::Encode() {
	// Generate the string. 
	return ActionToString( this->m_action ) + std::string( " " ) + DirectionToString( this->m_direction ) + std::string( "\n" ) ; 
	
}
bool CTankAction::Decode( std::string command ) {

	size_t split = command.find( " " ) ; 
	if( split == std::string::npos ) {
		return false; 
	}

	// Get the action. 
	if( ! StringToAction( command.substr( 0, split ), this->m_action ) ) {
		return false; 
	}
	
	// Get the direction if it is there 
	if( ! StringToDirection( command.substr( split+1 ), this->m_direction ) ) {
		return false; 
	}
	return true; 
}

bool CTankAction::StringToAction( std::string value, CTankAction::ActionEnum &action ) {
	value = trim( value ) ; 
	if( value.size() <= 0 ) {
		return false; 
	}

	// K.I.S.S
	// There are more effectent methods but this works and its easy. 
	if( value.find( kTankAIActionNames[ ActionEnum::MOVE ] ) != std::string::npos ) {
		action = ActionEnum::MOVE ; 
		return true; 
	}
	if( value.find( kTankAIActionNames[ ActionEnum::SHOOT ] ) != std::string::npos ) {
		action = ActionEnum::SHOOT ; 
		return true; 
	}
	if( value.find( kTankAIActionNames[ ActionEnum::PING ] ) != std::string::npos ) {
		action = ActionEnum::PING ; 
		return true;  
	}
	return false; 
}

bool CTankAction::StringToDirection( std::string value, CTankAction::DirectionsEnum  &direction ) {
	value = trim( value ) ; 
	if( value.size() <= 0 ) {
		return false; 
	}

	// K.I.S.S
	// There are more effectent methods but this works and its easy. 
	if( value.find( kTankAIDirectionsNames[ DirectionsEnum::NORTH ] ) != std::string::npos ) {
		direction = DirectionsEnum::NORTH ; 
		return true; 
	}
	if( value.find( kTankAIDirectionsNames[ DirectionsEnum::EAST ] ) != std::string::npos ) {
		direction = DirectionsEnum::EAST ; 
		return true; 
	}
	if( value.find( kTankAIDirectionsNames[ DirectionsEnum::SOUTH ] ) != std::string::npos ) {
		direction = DirectionsEnum::SOUTH ; 
		return true; 
	}
	if( value.find( kTankAIDirectionsNames[ DirectionsEnum::WEST ] ) != std::string::npos ) {
		direction = DirectionsEnum::WEST ; 
		return true; 
	}
	
	return false; 
}
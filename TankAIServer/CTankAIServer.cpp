#include "CTankAIServer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>


void CTankAIServer::Reset() {
	// Move the two players to random locations on the map 
	this->m_playerA.MoveTo( rand() % 256 + 1, rand() % 256 + 1 ); 
	this->m_playerA.MoveTo( rand() % 256 + 1, rand() % 256 + 1 ); 

	// Ensure that the two players are not on the same tile to start. 
	if( this->m_playerA.m_x == this->m_playerB.m_x &&
		this->m_playerA.m_y == this->m_playerB.m_y ) {
		// The tanks are on the same tile. 
		this->Reset(); 
		return; 
	}
}

void CTankAIServer::DoResponse( CTankAIBase::ResponseEnum response ) {
	switch( response ) {
		case CTankAIBase::PASS: {
			printf( TANK_RESPONSE_PASS ); 
			break; 
		}
		default: 
		case CTankAIBase::FAIL: {
			printf( TANK_RESPONSE_FAIL ); 
			break; 
		}
	}
	printf( "\n" );
}

void CTankAIServer::DoCommand( std::string commandLine ) {
	if( commandLine.size() <= 0 ) {
		// No command this should not happen. 
		this->DoResponse( CTankAIBase::FAIL );
	}

	// ToDo: 

	// Radar 
	// ---------------------------------------------------------------------------
	// Print the players positions. 
	printf( "{x=%d, y=%d}", this->m_playerA.m_x, this->m_playerA.m_y ); 
	printf( "{x=%d, y=%d}", this->m_playerB.m_x, this->m_playerB.m_y ); 
}

void CTankAIServer::Run() {

	// Reset the system and set the defaults. 
	this->Reset(); 

	std::string command; 

	bool done = false; 
	while( ! done ) {
		std::cin >> command; 
		this->DoCommand( command ); 
	}
}
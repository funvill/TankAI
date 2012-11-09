#include "CTankAIServer.h"
#include "CTankAction.h"
#include <sstream>
#include <iostream>

CTankAIServer::CTankAIServer()
{
	this->Reset(); 
}

void CTankAIServer::Reset() {
	this->m_UUID = 0 ; 
	this->m_players.clear(); 
}
void CTankAIServer::AddPlayer( CTankAIBase * player )  {
	this->m_players.push_back( player ); 
}

void CTankAIServer::Go() {
	size_t playerCount = this->m_players.size() ; 

	// Loop though all the different players. 
	for( std::vector<CTankAIBase *>::iterator playerOneOffset = this->m_players.begin(); playerOneOffset != this->m_players.end(); playerOneOffset++ ) {
		// We don't need to run the same test twice. 
		for( std::vector<CTankAIBase *>::iterator playerTwoOffset = (playerOneOffset+1); playerTwoOffset != this->m_players.end(); playerTwoOffset++ ) {
			this->RunBattle( (*playerOneOffset), (*playerTwoOffset), MAX_TURN_COUNT );
		}
	}
}

void CTankAIServer::RunBattle( CTankAIBase * playerOne, CTankAIBase * playerTwo, unsigned int turnCount ) {
	if( playerOne == NULL || playerTwo == NULL ) {
		return ; // Error 
	}

	
	// Set up the two tanks. 
	this->m_playerOneInfo.Set( this->m_UUID++, CTankAIObject::tank, 10, 10 ); 
	this->m_playerTwoInfo.Set( this->m_UUID++, CTankAIObject::tank, 20, 20 ); 
	
	// Set up holding strings for response and actions 
	std::string playerOneResponse, playerTwoResponse ; 

	// Run though all the turns 
	for( unsigned int turn = 0 ; turn < turnCount ; turn++ ) {

		printf("\nTurn %d\n", turn ); 

		// Do actions 
		printf( "Player 1 << " ); 
		this->RunPlayer( playerOne, this->m_playerOneInfo, playerOneResponse, this->m_playerTwoInfo ) ;
		printf( "Player 2 << " ); 
		this->RunPlayer( playerTwo, this->m_playerTwoInfo, playerTwoResponse, this->m_playerOneInfo ) ;

		
		// Move projecties and check to see if it hits any of the tanks. 
		for( int moves = 0 ; moves < TANK_PROJECTILES_SPEED ; moves++ ) {
			MoveProjecties( m_playerOneInfo );
			MoveProjecties( m_playerTwoInfo );

			for( int offset = 0 ; offset < TANK_PROJECTILES; offset++ ) 
			{
				// Check to see if player one has been hit. 
				if( ( m_playerOneInfo.m_x == m_playerOneInfo.m_projectile[ offset ].m_x && m_playerOneInfo.m_y == m_playerOneInfo.m_projectile[ offset ].m_y) ) {
					printf( "\n\n Player one !!! Bullet(%d) From player one\n\n", m_playerOneInfo.m_projectile[ offset ].m_UUID ); // Player one got hit. 
					return ;
				}
				if( ( m_playerOneInfo.m_x == m_playerTwoInfo.m_projectile[ offset ].m_x && m_playerOneInfo.m_y == m_playerTwoInfo.m_projectile[ offset ].m_y) ) {
					printf( "\n\n Player one !!! Bullet(%d) From player two\n\n", m_playerTwoInfo.m_projectile[ offset ].m_UUID ); // Player one got hit. 
					return ;
				}
				if( ( m_playerTwoInfo.m_x == m_playerOneInfo.m_projectile[ offset ].m_x && m_playerTwoInfo.m_y == m_playerOneInfo.m_projectile[ offset ].m_y) ) {
					printf( "\n\n Player two !!! Bullet(%d) From player one\n\n", m_playerOneInfo.m_projectile[ offset ].m_UUID ); // Player one got hit. 
					return ;
				}
				if( ( m_playerTwoInfo.m_x == m_playerTwoInfo.m_projectile[ offset ].m_x && m_playerTwoInfo.m_y == m_playerTwoInfo.m_projectile[ offset ].m_y) ) {
					printf( "\n\n Player two !!! Bullet(%d) From player two\n\n", m_playerTwoInfo.m_projectile[ offset ].m_UUID ); // Player one got hit. 
					return ;
				}
			}
		}		

		// ToDo: 

		// Check for victory conditions. 
		// Collions 
		if( this->m_playerOneInfo.m_x == this->m_playerTwoInfo.m_x && 
			this->m_playerOneInfo.m_y == this->m_playerTwoInfo.m_y )
		{
			// Crash, No winner 
			printf( "Tie - Crash \n"); 
			printf( "   Player One x=%d y%d\n", (int) m_playerOneInfo.m_x, (int) m_playerOneInfo.m_y ); 
			printf( "   Player Two x=%d y%d\n", (int) m_playerTwoInfo.m_x, (int) m_playerTwoInfo.m_y ); 
			return ; 
		}
		 

		/*
		// If there is a response show it. 
		if( playerOneResponse.size() > 0 ) { 
			printf("Player 1 >> %s\n", playerOneResponse.c_str() ) ;
		}
		if( playerTwoResponse.size() > 0 ) { 
			printf("Player 2 >> %s\n", playerTwoResponse.c_str() ) ;
		}
		*/


		// Print game status for debug 
		printf( "*-------------------------------------------------*\n" ) ; 
		printf( "| Player 1 \n" ); 
		this->PrintPlayerInfo( m_playerOneInfo ); 		
		printf( "| Player 2 \n" ); 
		this->PrintPlayerInfo( m_playerTwoInfo ); 
	}

	printf( "Tie - Turns have run out\n" ); 
	return ;
}

void CTankAIServer::RunPlayer( CTankAIBase * playerBot, CTankAIServerTank  & playersInfo, std::string & playerServerResponse, CTankAIServerTank & enemyBot ) {
	if( playerBot == NULL ) {
		return ; 
	}

	playerServerResponse = ""; 

	std::string playerActionString = playerBot->Go( playerServerResponse ) ; 
	CTankAction playerAction;
	playerAction.Decode( playerActionString ) ; 

	printf( playerAction.Encode().c_str() ); 
	
	switch( playerAction.m_action ) {
		case CTankAction::MOVE:
		{
			MoveObject( playersInfo, playerAction.m_direction ); 
			break; 			
		}
		case CTankAction::SHOOT:
		{
			// Search for a free bullet 
			for( int offset = 0 ; offset < TANK_PROJECTILES; offset++ ) {
				if( playersInfo.m_projectile[ offset ].lifeSpan <= 0 ) {
					// We found an unactivated bullet. 
					// Activate it and set it on its way. 
					playersInfo.m_projectile[ offset ].lifeSpan = 0xff ; 
					playersInfo.m_projectile[ offset ].m_direction = playerAction.m_direction ; 
					playersInfo.m_projectile[ offset ].m_x = playersInfo.m_x ; 
					playersInfo.m_projectile[ offset ].m_y = playersInfo.m_y ; 
					playersInfo.m_type = CTankAIObject::projectile ; 
					
					break; 
				}
			}
			break; 			
		}
		case CTankAction::PING:
		{
			std::stringstream ss ;

			// Print your own bullets. 
			for( int offset = 0 ; offset < TANK_PROJECTILES; offset++ ) {
				if( playersInfo.m_projectile[ offset ].lifeSpan > 0 ) {
					ss << TANK_OBJECT_TYPE_PROJECTILE << "(" << playersInfo.m_projectile[ offset ].m_UUID << ", "  << playersInfo.m_x - playersInfo.m_projectile[ offset ].m_x << ", "  << playersInfo.m_y - playersInfo.m_projectile[ offset ].m_y << ") " ; 
				}
			}

			// Print the enemy
			ss << TANK_OBJECT_TYPE_ENEMY << "(" << enemyBot.m_UUID << ", " << playersInfo.m_x - enemyBot.m_x << ", " << playersInfo.m_y - enemyBot.m_y << ") " ; 
			
			// Print the enemy bullets 
			for( int offset = 0 ; offset < TANK_PROJECTILES; offset++ ) {
				if( enemyBot.m_projectile[ offset ].lifeSpan > 0 ) {
					ss << TANK_OBJECT_TYPE_PROJECTILE << "(" << enemyBot.m_projectile[ offset ].m_UUID << ", "  << playersInfo.m_x - enemyBot.m_projectile[ offset ].m_x << ", "  << playersInfo.m_y - enemyBot.m_projectile[ offset ].m_y << ") " ; 
				}
			}
			
			playerServerResponse = ss.str(); 
			break; 			
		}
	}
}

void CTankAIServer::MoveObject( CTankAIObject & object, CTankAction::DirectionsEnum direction ) {
	switch( direction ) {
		case CTankAction::NORTH: {
			object.m_y-- ; 
			break ; 
		}
		case CTankAction::EAST: {
			object.m_x++ ; 
			break ; 
		}
		case CTankAction::SOUTH: {
			object.m_y++ ; 
			break ; 
		}
		case CTankAction::WEST: {
			object.m_x-- ; 
			break ; 
		}
	}
}


void CTankAIServer::MoveProjecties( CTankAIServerTank & playerA ) {
	for( int bullets = 0 ; bullets < TANK_PROJECTILES; bullets++ ) {
		if( playerA.m_projectile[ bullets ].lifeSpan > 0 ) {
			playerA.m_projectile[ bullets ].lifeSpan-- ; 
			this->MoveObject( playerA.m_projectile[ bullets ], playerA.m_projectile[ bullets ].m_direction ) ; 
		}
	}
}

void CTankAIServer::PrintPlayerInfo( CTankAIServerTank & playersInfo ) {
	printf( "|   UUID=[%d], x=%d, y=%d\n", playersInfo.m_UUID, (int) playersInfo.m_x, (int) playersInfo.m_y ) ; 
	for( int offset = 0 ; offset < TANK_PROJECTILES ; offset++ ) {
		if( playersInfo.m_projectile[ offset ].lifeSpan > 0 ) {
			printf( "|   Projectiles life=[%d], UUID=[%d], x=[%d], y=[%d]\n", playersInfo.m_projectile[ offset ].lifeSpan, playersInfo.m_projectile[ offset ].m_UUID, (int) playersInfo.m_projectile[ offset ].m_x, (int) playersInfo.m_projectile[ offset ].m_y ); 
		}
	}
	printf( "*-------------------------------------------------*\n" ) ; 
}
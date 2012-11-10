#include "CTankAIServer.h"
#include "CTankAction.h"
#include <sstream>
#include <iostream>

CTankAIServer::CTankAIServer() {
	this->m_UUID = 0 ; 
}
void CTankAIServer::AddBot( CTankAIBase * bot )  {
	
	CTankAIServerTank * tankBot = new CTankAIServerTank(); 
	tankBot->m_bot = bot ; 
	this->m_players.push_back( tankBot ); 
}

void CTankAIServer::GameSetup() {
	// Loop though all the bots and reset the bots stats 
	for( std::vector<CTankAIServerTank *>::iterator playerOffset = this->m_players.begin(); playerOffset != this->m_players.end(); playerOffset++ ) {

		// Reset bot running vars 
		(*playerOffset)->m_alive	= true ; 
		(*playerOffset)->m_type		= CTankAIObject::tank ; 
		(*playerOffset)->m_response	= ""; 
		for( int i = 0 ; i < TANK_PROJECTILES ; i++ ) {
			(*playerOffset)->m_projectile[ i ].lifeSpan = 0 ; 
		}

		// Select a random location for this bot to start. 
		// Note: the random location may be on top of another bot... oops bad luck. 
		(*playerOffset)->m_x = char (rand() % 256 ) ; 
		(*playerOffset)->m_y = char (rand() % 256 ) ; 

		m_stats["Bot.Count"]++; 
	}
}

void CTankAIServer::Go() {
	// repeat the entire series over again 
	for( unsigned int session = 0 ; session < MAX_SESSION_COUNT ; session++ ) 
	{
		// Reset the game 
		this->GameSetup(); 

		// Loop though the turns 
		bool done = false; 
		for( size_t turnOffset = 0 ; turnOffset < MAX_TURN_COUNT && ! done ; turnOffset++ ) 
		{
			printf( "\nTurn %d\n", turnOffset ); 
			// Loop though all the different players. 
			for( std::vector<CTankAIServerTank *>::iterator playerOffset = this->m_players.begin(); playerOffset != this->m_players.end(); playerOffset++ ) 
			{
				if( (*playerOffset) == NULL ) {
					continue ; // This is a programing error 
				}
				if( ! (*playerOffset)->m_alive ) {
					continue ; // This player is dead continue; 
				}
			
				this->GoBot( (*playerOffset) ); 
			}

			// Move projectiles and check to see if they hit anyone. 
			this->GoProjectiles(); 

			// Everything has moved. 
			this->CheckForCrashes() ;

			// Respond to anyone that pinged the map. 
			// Check for victory conditions 
			done = this->GoEndOfTurn(); 

			// Debug 
			if( turnOffset % 10 == 0 ) {
				this->DebugInfo(); 
			}
		}

		// Print global stats
		for( std::map<std::string, unsigned int>::iterator it = this->m_stats.begin() ; it != this->m_stats.end() ; it++ ) {
			printf( "[%s]=%d\n", (*it).first.c_str(), (*it).second ); 
		}
	}

	// Print global stats
	for( std::map<std::string, unsigned int>::iterator it = this->m_stats.begin() ; it != this->m_stats.end() ; it++ ) {
		printf( "[%s]=%d\n", (*it).first.c_str(), (*it).second ); 
	}

}

void CTankAIServer::GoProjectiles() 
{
	// Move projecties and check to see if it hits any of the tanks. 
	for( int projectilesMoves = 0 ; projectilesMoves < TANK_PROJECTILES_SPEED ; projectilesMoves++ ) 
	{
		// Loop though all the player's projectiles
		for( std::vector<CTankAIServerTank *>::iterator projectilesPlayerOffset = this->m_players.begin(); projectilesPlayerOffset != this->m_players.end(); projectilesPlayerOffset++ ) 
		{
			// Loop though all the projectiles 
			for( int projectilesOffset = 0 ; projectilesOffset < TANK_PROJECTILES; projectilesOffset++ ) 
			{
				if( (*projectilesPlayerOffset)->m_projectile[projectilesOffset].lifeSpan <= 0 ) {
					continue;// This projectile is not alive and can't hurt anyone. 
				}

				// Move the projectile
				(*projectilesPlayerOffset)->m_projectile[projectilesOffset].lifeSpan--;
				MoveObject( (*projectilesPlayerOffset)->m_projectile[projectilesOffset], (*projectilesPlayerOffset)->m_projectile[projectilesOffset].m_direction ); 
				
				// We have an active projectile
				// loop though the players looking for people that got hit by this projectiles. 
				for( std::vector<CTankAIServerTank *>::iterator playerOffset = this->m_players.begin(); playerOffset != this->m_players.end(); playerOffset++ ) 
				{
					if( (*projectilesPlayerOffset)->m_projectile[projectilesOffset].m_x == (*playerOffset)->m_x && 
						(*projectilesPlayerOffset)->m_projectile[projectilesOffset].m_y == (*playerOffset)->m_y )
					{
						// We have a hit ! 
						(*projectilesPlayerOffset)->m_projectile[projectilesOffset].lifeSpan = 0 ; 
						(*playerOffset)->m_alive = false; 

						// Debug message. 
						printf( "bot (%s) was killed by projectile (%d) shot by (%s)\n", 
											(*playerOffset)->m_bot->GetName().c_str(),
											(*projectilesPlayerOffset)->m_projectile[projectilesOffset].m_UUID,
											(*projectilesPlayerOffset)->m_bot->GetName().c_str() ); 
						// ToDo: add stats to check for bot accuracy 
						this->m_stats[ (*playerOffset)->m_bot->GetName() + std::string( ".DeadByProjectile") ]++; 
						this->m_stats[ (*projectilesPlayerOffset)->m_bot->GetName() + std::string( ".KillByProjectile") ]++; 
					}
				}
			}
		}
	}
}

void CTankAIServer::CheckForCrashes() {
	for( std::vector<CTankAIServerTank *>::iterator playerOffset = this->m_players.begin(); playerOffset != this->m_players.end(); playerOffset++ ) 
	{
		if( ! (*playerOffset)->m_alive ) {
			continue ; // This player is dead continue; 
		}

		// Check to make sure that two tanks did not crash
		for( std::vector<CTankAIServerTank *>::iterator playerCrashOffset = playerOffset+1; playerCrashOffset != this->m_players.end(); playerCrashOffset++ ) 
		{
			if( ! (*playerCrashOffset)->m_alive ) {
				continue; // Dead tank 
			}

			if( (*playerCrashOffset)->m_x == (*playerOffset)->m_x && (*playerCrashOffset)->m_y == (*playerOffset)->m_y ){
				// These two thanks crashed in to each other. 
				printf( "Bot (%s) crashed in to bot (%s) killing both\n", (*playerCrashOffset)->m_bot->GetName().c_str(), (*playerOffset)->m_bot->GetName().c_str() ) ; 
				(*playerCrashOffset)->m_alive = false; 
				(*playerOffset)->m_alive = false; 

				this->m_stats[ (*playerCrashOffset)->m_bot->GetName() + std::string( ".DeadByCrash") ]++; 
				this->m_stats[ (*playerOffset)->m_bot->GetName() + std::string( ".DeadByCrash") ]++; 

				break; 
			}
		}
	}
}

bool CTankAIServer::GoEndOfTurn() {
	// Respond to anyone that pinged the map. 
	// Check for victory conditions 

	unsigned int aliveTanks = 0 ; 

	for( std::vector<CTankAIServerTank *>::iterator playerOffset = this->m_players.begin(); playerOffset != this->m_players.end(); playerOffset++ ) 
	{
		if( ! (*playerOffset)->m_alive ) {
			continue ; // This player is dead continue; 
		}
		
		// There is one more alive tank.
		aliveTanks++ ; 

		// Check for pings 
		if( (*playerOffset)->m_botAction.m_action == CTankAction::PING ) {
			// This player wants an update of the battle field. 
			this->UpdateBotPing( (*playerOffset) ); 
		}
	}

	m_stats["Bot.Alive"] = aliveTanks ; 
	printf( "Alive tanks=%d\n", aliveTanks ); 

	if( aliveTanks <= 0  ) {
		return true ; // One or less tanks are left.
	}

	// We are not done yet 
	return false; 
}

void CTankAIServer::UpdateBotPing( CTankAIServerTank * playerInfo ) {
	if( playerInfo == NULL ) {
		return ; 
	}

	std::stringstream ss ;

	// Loop though all the bots including yourself
	for( std::vector<CTankAIServerTank *>::iterator playerOffset = this->m_players.begin(); playerOffset != this->m_players.end(); playerOffset++ ) 
	{
		// Print tank location. 
		ss << TANK_OBJECT_TYPE_ENEMY << "(" << (*playerOffset)->m_UUID << ", " << playerInfo->m_x - (*playerOffset)->m_x << ", " << playerInfo->m_y - (*playerOffset)->m_y << "), " ; 

		// Print projectile
		for( int projectileOffset = 0 ; projectileOffset < TANK_PROJECTILES; projectileOffset++ ) {
			if( (*playerOffset)->m_projectile[ projectileOffset ].lifeSpan > 0 ) {
				ss << TANK_OBJECT_TYPE_PROJECTILE << "(" << (*playerOffset)->m_projectile[ projectileOffset ].m_UUID << ", "  << playerInfo->m_x - (*playerOffset)->m_projectile[ projectileOffset ].m_x << ", "  << playerInfo->m_y - (*playerOffset)->m_projectile[ projectileOffset ].m_y << "), " ; 
			}
		}
	}
			
	playerInfo->m_response = ss.str(); 

}

void CTankAIServer::GoBot( CTankAIServerTank * playerInfo ) {
	if( playerInfo == NULL ) {
		return ; 
	}

	// Print some info about this player. 
	printf( "Bot:%s(%d,%d) << ",  playerInfo->m_bot->GetName().c_str(), playerInfo->m_x, playerInfo->m_y ); 

	// Get the players actions 
	std::string playerActionString = playerInfo->m_bot->Go( playerInfo->m_response ) ; 
	printf( playerActionString.c_str() ); 
	
	playerInfo->m_botAction.Decode( playerActionString ) ; 

	// Do the action depending on the bots command. 		
	switch( playerInfo->m_botAction.m_action) {
		case CTankAction::MOVE:
		{
			MoveObject( *playerInfo, playerInfo->m_botAction.m_direction ); 
			this->m_stats[ playerInfo->m_bot->GetName() + std::string( ".action.move") ]++; 
			break; 			
		}
		case CTankAction::SHOOT:
		{
			this->m_stats[ playerInfo->m_bot->GetName() + std::string( ".action.shoot") ]++; 

			// Search for a free bullet 
			bool found = false; 
			for( int offset = 0 ; offset < TANK_PROJECTILES; offset++ ) {
				if( playerInfo->m_projectile[ offset ].lifeSpan <= 0 ) {
					// We found an unactivated bullet. 
					// Activate it and set it on its way. 
					playerInfo->m_projectile[ offset ].Set( this->m_UUID++, CTankAIObject::projectile, playerInfo->m_x, playerInfo->m_y); 
					playerInfo->m_projectile[ offset ].lifeSpan = 0xff ; 
					playerInfo->m_projectile[ offset ].m_direction = playerInfo->m_botAction.m_direction ; 
					found =true ; 

					printf( "FYI: Projectile (%d) created\n", playerInfo->m_projectile[ offset ].m_UUID );
					this->m_stats[ playerInfo->m_bot->GetName() + std::string( ".shots") ]++; 
					break; 
				}
			}

			if( ! found ) {
				printf( "FYI: All projectile are in use. \n" );
				this->m_stats[ playerInfo->m_bot->GetName() + std::string( ".missfire") ]++; 
			}

			break; 			
		}
		case CTankAction::PING:
		{
			this->m_stats[ playerInfo->m_bot->GetName() + std::string( ".action.ping") ]++; 
			// Delayed until after everyone has moved and shot. 
			// Do nothing 
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

void CTankAIServer::DebugInfo() {
	printf( "*-------------------------------------------------*\n" ) ; 

	for( std::vector<CTankAIServerTank *>::iterator playerOffset = this->m_players.begin(); playerOffset != this->m_players.end(); playerOffset++ ) 
	{
		this->PrintPlayerInfo( *(*playerOffset) ); 
	}

	// Print global stats
	for( std::map<std::string, unsigned int>::iterator it = this->m_stats.begin() ; it != this->m_stats.end() ; it++ ) {
		printf( "[%s]=%d\n", (*it).first.c_str(), (*it).second ); 
	}
}

void CTankAIServer::PrintPlayerInfo( CTankAIServerTank & playersInfo ) {
	printf( "| %s\n", playersInfo.m_bot->GetName().c_str() ); 
	printf( "|   UUID=[%d], x=%d, y=%d\n", playersInfo.m_UUID, (int) playersInfo.m_x, (int) playersInfo.m_y ) ; 
	for( int offset = 0 ; offset < TANK_PROJECTILES ; offset++ ) {
		if( playersInfo.m_projectile[ offset ].lifeSpan > 0 ) {
			printf( "|   Projectiles life=[%d], UUID=[%d], x=[%d], y=[%d]\n", playersInfo.m_projectile[ offset ].lifeSpan, playersInfo.m_projectile[ offset ].m_UUID, (int) playersInfo.m_projectile[ offset ].m_x, (int) playersInfo.m_projectile[ offset ].m_y ); 
		}
	}
	printf( "*-------------------------------------------------*\n" ) ; 
}
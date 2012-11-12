#include "CTankAIBotMrClever.h"


CTankAIBotMrClever::CTankAIBotMrClever(void)
{
	this->Reset(); 
}

void CTankAIBotMrClever::Reset() {
	this->m_turnCount	= 0; 
	this->m_pingRate	= 1; 
}

unsigned short CTankAIBotMrClever::GetDistance( char x, char y) {
	return abs( x ) + abs( y ) ; 
}

std::string CTankAIBotMrClever::Go( std::string command ) 
{
	// Update the turn counter. 
	this->m_turnCount++; 

	// Check to see if we got a message from the server, if we did update the battle map
	this->CheckResponse( command ) ;

	// Update the tank commands 
	UpdateTankAI( ); 
	
	return this->m_nextAction.Encode() ;
}

void CTankAIBotMrClever::UpdateTankAI( ) 
{
	if(this->m_objects.size() <= 0 ) {

		if( m_turnCount % m_pingRate == 0 ) {
			// ITs time to ping 
			this->m_nextAction.Set( CTankAction::PING ); 
			return; 
		}

		// Keep on doing what we are doing. 
		return ; // Nothing to process. 
	}

	// Find the closet tank - To Attack 
	char closestTankX, closestTankY;
	unsigned short closestTankDistance = 0xFFFF; 
	
	// Find the closest projectile - To Avoid 
	char closestProjectileX, closestProjectileY;
	unsigned short closestProjectileDistance = 0xFFFF; 	

	// Loop though all the objects 
	for( std::vector<CTankAIObject>::iterator it = this->m_objects.begin() ; it != this->m_objects.end() ; it++ ) {
		CTankAIObject & object = (*it) ; 
		
		switch( object.m_type ) 
		{
			case CTankAIObject::tank:
			{
				// Check to see if this is a closer tank 
				unsigned short distance = GetDistance( object.m_x, object.m_y ); 
				if( distance == 0 ) {
					continue; // This is us. 
				}

				if( distance < closestTankDistance ) {
					// Found a tank that is closer. 
					closestTankX = object.m_x ; 
					closestTankY = object.m_y ; 
					closestTankDistance = distance ; 
				}
				break; 
			}

			case CTankAIObject::projectile:
			{
				// Check to see if this is a closer tank 
				unsigned short distance = GetDistance( object.m_x, object.m_y ); 
				if( distance < closestProjectileDistance ) {
					// Found a tank that is closer. 
					closestProjectileX = object.m_x ; 
					closestProjectileY = object.m_y ; 
					closestProjectileDistance = distance ; 
				}
				break; 
			}
		}		
	}

	// We don't need the objects any more. 
	this->m_objects.clear(); 


	// If we are close to a projectile then we need to move out of the way. 
	if( closestProjectileDistance <= TANK_PROJECTILES_SPEED ) {
		if( closestProjectileX == 0 ) {
			// We need to move in the Y direction 
			this->m_nextAction.Set( CTankAction::MOVE, CTankAction::NORTH ); 
			return; 
		} else if( closestProjectileY == 0 ) {
			// We need to move in the X direction 
			this->m_nextAction.Set( CTankAction::MOVE, CTankAction::EAST ); 
			return; 
		}
	}

	// Update the ping rate. 
	this->m_pingRate = TANK_PROJECTILES_SPEED ; 

	// if we are farther then TANK_PROJECTILES_SPEED spaces away from another tank. We should move closer. 
	if( closestTankDistance > TANK_PROJECTILES_SPEED ) {
		// We are far away from any other tank. Move closer 
		if( abs( closestTankX ) >= abs( closestTankY ) ) {
			// East or west 
			if( closestTankX < 0 ) {
				this->m_nextAction.Set( CTankAction::MOVE, CTankAction::EAST ); 
			} else {
				this->m_nextAction.Set( CTankAction::MOVE, CTankAction::WEST ); 
			}
		} else {
			// North or south 
			if( closestTankY > 0 ) {
				this->m_nextAction.Set( CTankAction::MOVE, CTankAction::NORTH ); 
			} else {
				this->m_nextAction.Set( CTankAction::MOVE, CTankAction::SOUTH ); 
			}
		}
		return ; 
	}

	this->m_pingRate = 2; 

	// We are close then TANK_PROJECTILES_SPEED 
	// If we are on the same col/file, shoot at them. 
	if( closestTankX == 0 || closestTankY == 0 ) {
		// We are within firing range and on the same col or file as them. FIRE! 
		if( closestTankX == 0 ) {
			if( closestTankY < 0 ) {
				this->m_nextAction.Set( CTankAction::SHOOT, CTankAction::NORTH ); 
			} else {
				this->m_nextAction.Set( CTankAction::SHOOT, CTankAction::SOUTH ); 
			}
		} else {
			if( closestTankX < 0 ) {
				this->m_nextAction.Set( CTankAction::SHOOT, CTankAction::WEST ); 
			} else {
				this->m_nextAction.Set( CTankAction::SHOOT, CTankAction::EAST ); 
			}
		}
		return ; 
	}

	// We are not on the same col/file, we need to move, but becareful that we don't run in to them. 
	if( abs( closestTankX ) < TANK_PROJECTILES_SPEED && abs( closestTankY ) < TANK_PROJECTILES_SPEED ) {
		// Match up on the Y and shoot. 
		if( closestTankY > 0 ) {
			this->m_nextAction.Set( CTankAction::MOVE, CTankAction::NORTH ); 
		} else {
			this->m_nextAction.Set( CTankAction::MOVE, CTankAction::SOUTH ); 
		}
		return ; 
	}
	

	// We need to move closer.
	if( abs( closestTankX ) >= abs( closestTankY ) ) {
		// East or west 
		if( closestTankX < 0 ) {
			this->m_nextAction.Set( CTankAction::MOVE, CTankAction::EAST ); 
		} else {
			this->m_nextAction.Set( CTankAction::MOVE, CTankAction::WEST ); 
		}
	} else {
		// North or south 
		if( closestTankY > 0 ) {
			this->m_nextAction.Set( CTankAction::MOVE, CTankAction::NORTH ); 
		} else {
			this->m_nextAction.Set( CTankAction::MOVE, CTankAction::SOUTH ); 
		}
	}
	
	return; 

}

#pragma once

#include "CTankAIBase.h"
#include <vector>

#define MAX_TURN_COUNT			256*100
#define TANK_SPEED				1 
#define TANK_PROJECTILES		3 
#define TANK_PROJECTILES_SPEED	2 

class CTankAIServerProjectile : public CTankAIObject 
{
	public:
		CTankAction::DirectionsEnum m_direction; 
		unsigned char lifeSpan  ; 

};

class CTankAIServerTank : public CTankAIObject
{
	public:

		CTankAIServerTank() {
			this->m_type = CTankAIObject::tank ;
			for( int i = 0 ; i < TANK_PROJECTILES ; i++ ) {
				this->m_projectile[ i ].lifeSpan = 0 ; 
			}
		}

		unsigned char				m_bullets; 
		CTankAIServerProjectile		m_projectile[ TANK_PROJECTILES ] ; 
};



class CTankAIServer
{
private:
	unsigned int					m_UUID ; 
	std::vector<CTankAIBase *>		m_players ; 

	// Current players info 
	CTankAIServerTank	m_playerOneInfo ; 
	CTankAIServerTank	m_playerTwoInfo ; 


	void Reset(); 
	void RunBattle( CTankAIBase * playerOne, CTankAIBase * playerTwo, unsigned int turnCount ) ; 

	void RunPlayer( CTankAIBase * playerBot, CTankAIServerTank & playersInfo, std::string & playerServerResponse, CTankAIServerTank & enemyBot ); 
	void MoveObject( CTankAIObject & object, CTankAction::DirectionsEnum direction ); 


	void PrintPlayerInfo( CTankAIServerTank & playersInfo ); 
	void MoveProjecties( CTankAIServerTank & playerInfo );
	bool TestProjectiesHit( CTankAIServerTank & playerA, CTankAIServerTank & playerB );

public:
	CTankAIServer();

	void Go(); 
	void AddPlayer( CTankAIBase * player ) ; 
};


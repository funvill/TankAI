/******************************************************************************
 *  Tank AI Programming challenge
 *
 *  Written by: Steven Smethurst 
 *  Created: Nov 04, 2012 
 * 
 ******************************************************************************/

#pragma once

#include "CTankAIBase.h"
#include <vector>
#include <map>

#define MAX_SESSION_COUNT		100 
#define MAX_TURN_COUNT			256*100

#define TANK_SPEED				1 
#define TANK_PROJECTILES		3 
#define TANK_PROJECTILES_SPEED	10 


class CTankAIServerProjectile : public CTankAIObject 
{
	public:
		CTankAction::DirectionsEnum m_direction; 
		unsigned char lifeSpan  ; 

};

class CTankAIServerTank : public CTankAIObject
{
	public:
		bool						m_alive; 
		CTankAIServerProjectile		m_projectile[ TANK_PROJECTILES ] ; 
		CTankAIBase	*				m_bot;
		std::string					m_response; 
		CTankAction					m_botAction;

		CTankAIServerTank(); 
		void Reset(); 
		std::string GetName() ; 
};



class CTankAIServer
{
private:
	unsigned int						m_UUID ; 
	std::vector<CTankAIServerTank *>	m_players ; 
	std::map<std::string, unsigned int>	m_stats; 

	void GameSetup(); 
	void GoBot( CTankAIServerTank * playerInfo ) ; 
	void GoProjectiles(); 
	void CheckForCrashes(); 

	bool GoEndOfTurn( ); 
	void UpdateBotPing( CTankAIServerTank * playerInfo ); 
	void DebugInfo(); 

	void MoveObject( CTankAIObject & object, CTankAction::DirectionsEnum direction ); 


	void PrintPlayerInfo( CTankAIServerTank & playersInfo ); 
	void MoveProjecties( CTankAIServerTank & playerInfo );
	bool TestProjectiesHit( CTankAIServerTank & playerA, CTankAIServerTank & playerB );

public:
	CTankAIServer();

	void Go(); 
	void AddBot( CTankAIBase * bot ) ; 
};


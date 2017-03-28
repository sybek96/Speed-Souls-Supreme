#pragma once
#include "entityx\Entity.h"


/// <summary>
/// @author Bartosz Zych & Rafael Plugge
/// @version 1.0
/// @brief Component representing a that an entity is a player.
/// 
/// Contains player's stats
/// </summary>
struct Player : public entityx::Component<Player>
{
	// default constructor
	Player(
		const int& currency = 100
		, const int& engine = 1
		, const int& handling = 1
		, const int& wheel = 1);

	// determines amount that player can purchase
	int m_currency;
	// determines top speed (1-3)
	int m_engine;
	// determines turn-ability (1-3)
	int m_handling;
	// determines brake-ability (1-3)
	int m_wheel;
};


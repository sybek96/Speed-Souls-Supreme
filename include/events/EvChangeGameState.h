#pragma once

#include "util\GameStates.h"

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Changes Current Game State.
/// 
/// Event causes the current game state, to change to the
/// new game state that this event contains
/// </summary>
struct EvChangeGameState : public entityx::Event<EvChangeGameState>
{
	/// <summary>
	/// @brief Default event constructor.
	/// 
	/// Construct a event with defaulted parameters
	/// </summary>
	/// <param name="gameState">defines the new game state</param>
	EvChangeGameState(GameState gameState = GameState::None)
		: m_gameState(gameState)
	{}

	// new game state
	GameState m_gameState;
};
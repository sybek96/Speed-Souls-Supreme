#pragma once

#include "entityx\Event.h"

#include "util\ResourceTypes.h"
#include "util\GameStates.h"
#include "util\MenuLoader.h"

/// <summary>
/// @brief Assets loaded event.
/// 
/// Event is called from level system,
/// after every necessary asset is loaded
/// </summary>
struct EvAssetsLoaded : public entityx::Event<EvAssetsLoaded>
{
	EvAssetsLoaded(GameState gameState = GameState::None)
		: m_gameState(gameState)
	{}

	GameState m_gameState;
};
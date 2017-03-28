#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "entityx\entityx.h"

#include "events\EvChangeGameState.h"

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Top level game manager class.
/// 
/// Will manage all the different game states,
/// as well as their initialisation.
/// </summary>
class GameManager : public entityx::Receiver<GameManager>
{
public:
	GameManager(entityx::EntityManager& entityManager
		, entityx::EventManager& eventManager
		, entityx::SystemManager& systemManager);

	void init();

	std::shared_ptr<GameState> getGameState() const;

	// For each subscribe in GameManager::init(), there must be a corresponding receive
	void receive(const EvChangeGameState& changeGameState);

private:

	// reference to the entity manager
	entityx::EntityManager& m_entityManager;

	// reference to the event manager
	entityx::EventManager& m_eventManager;

	// reference to the system manager
	entityx::SystemManager& m_systemManager;

	// Current game state
	std::shared_ptr<GameState> m_gameState;

	// Current track level
	int m_currentTrack;
};

#endif // !GAME_MANAGER_H
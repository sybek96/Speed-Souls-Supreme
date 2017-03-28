#include "GameManager.h"

/// <summary>
/// @brief Game Manager constructor.
/// 
/// Initializes all member variables
/// </summary>
/// <param name="entityManager">defines the entity manager whoose reference is stored</param>
/// <param name="eventManager">defines the event manager whoose reference is stored</param>
/// <param name="systemManager">defines the system manager whoose reference is stored</param>
GameManager::GameManager(
	entityx::EntityManager& entityManager
	, entityx::EventManager& eventManager
	, entityx::SystemManager& systemManager)

	: m_entityManager(entityManager)
	, m_eventManager(eventManager)
	, m_systemManager(systemManager)
	, m_gameState(std::make_shared<GameState>(GameState::None))
{
}

/// <summary>
/// @brief Game manager's initialization.
/// 
/// Subscribes to the event manager
/// </summary>
void GameManager::init()
{
	m_eventManager.subscribe<EvChangeGameState>(*this);
}

/// <summary>
/// @brief Gets the current game states.
/// 
/// 
/// </summary>
/// <returns>constant reference of current GameState</returns>
std::shared_ptr<GameState> GameManager::getGameState() const
{
	return m_gameState;
}

/// <summary>
/// @brief Called when param type is emitted.
/// 
/// Event causes Game manager to set the game to the playing state
/// </summary>
/// <param name="e">constant reference to the event struct</param>
void GameManager::receive(const EvChangeGameState & e)
{
	// Used to do any deallocations and unloading of resources
	switch (*m_gameState)
	{
	case GameState::LicenseScreen:
	case GameState::SplashScreen:
	case GameState::MainMenu:
	case GameState::OptionsScreen:
	case GameState::CarSelection:
	case GameState::TrackSelection:
	case GameState::Paused:
	case GameState::Exit:
	case GameState::Playing:
	case GameState::None:
	default:
		m_entityManager.reset();
		break;
	}

	// assign new game state
	(*m_gameState) = e.m_gameState;

	// Used to do any allocations and loading of resources
	switch (*m_gameState)
	{
	case GameState::None:
		break;
	case GameState::LicenseScreen:
	case GameState::SplashScreen:
	case GameState::MainMenu:
	case GameState::OptionsScreen:
	case GameState::CarSelection:
	case GameState::TrackSelection:
	case GameState::Paused:
	case GameState::Exit:
	case GameState::Playing:

		break;
	default:
		break;
	}


}

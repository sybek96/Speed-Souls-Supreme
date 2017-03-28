#ifndef GAME_H
#define GAME_H

#include "SFML\Graphics.hpp"
#include "sfeMovie\Movie.hpp"
#include "entityx\entityx.h"
#include <time.h>

#include "entities\CarCreator.h"

#include "systems\LevelSystem.h"
#include "systems\RenderSystem.h"
#include "systems\InputSystem.h"
#include "systems\MenuSystem.h"
#include "systems\MenuRenderSystem.h"
#include "systems\MotionSystem.h"
#include "systems\ScrollSystem.h"
#include "systems\CollisionSystem.h"
#include "systems\MiniMapSystem.h"
#include "systems\MenuSystem.h"
#include "systems\CarTrackingSystem.h"
#include "systems\AiSystem.h"
#include "systems\MovieSystem.h"
#include "systems\HudSystem.h"
#include "systems\ParticleSystem.h"
#include "systems\SoundSystem.h"
#include "systems\HeadLightSystem.h"

#include "util\ResourcePath.h"
#include "util\ResourceTypes.h"
#include "util\ScreenSize.h"
#include "util\Xbox360Controller.h"
#include "util\StaticTime.h"

#include "GameManager.h"

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Top Level class which manages game initialisation, rendering and updates.
/// 
/// 
/// </summary>
class Game
{
public:
	Game();
	~Game();
	void init();
	void run();
private:
	void processEvents();
	void update();
	void render(double ms);

	void createSystems();

	// update per milliseconds
	static const sf::Int32 s_MS_PER_UPDATE;

	// SFML main game window
	sf::RenderWindow m_window;

	// ECS Events manager
	entityx::EventManager m_eventManager;

	// ECS Entity manager
	entityx::EntityManager m_entityManager;

	// ECS System manager
	entityx::SystemManager m_systemManager;

	// object to manage game states
	GameManager m_gameManager;

};

#endif // !GAME_H
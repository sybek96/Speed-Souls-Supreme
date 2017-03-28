#ifndef HUD_SYSTEM_H
#define HUD_SYSTEM_H

#include "SFML\Graphics.hpp"

#include "entityx\System.h"

#include "events\EvChangeGameState.h"
#include "events\EvAssetsLoaded.h"
#include "events\EvLapCompleted.h"
#include "events\EvLapUndone.h"

#include "util\ResourceTypes.h"
#include "util\ScreenSize.h"

#include "components\Player.h"
#include "components\Ai.h"
#include "components\Car.h"
#include "components\Road.h"

/// <summary>
/// @author Huong Thi Thanh Huynh
/// @version 1.0
/// @brief Will display the best lap, current lap, best time and current time
/// 
/// 
/// </summary>
class HudSystem
	: public entityx::System<HudSystem>
	, public entityx::Receiver<HudSystem>
{
public:
	HudSystem(sf::RenderWindow & window, entityx::EntityManager& entityManager, std::shared_ptr<FontBank> font, std::shared_ptr<sf::View> view);

	void update(entityx::EntityManager& entities
		, entityx::EventManager& events
		, double dt) override;

	void configure(entityx::EventManager & eventManager);

	void receive(const EvAssetsLoaded& e);
	void receive(const EvLapCompleted& e);
	void receive(const EvLapUndone& e);

	void receive(const entityx::ComponentAddedEvent<Player>& e);
	void receive(const entityx::ComponentRemovedEvent<Player>& e);

	void receive(const entityx::ComponentAddedEvent<Ai>& e);
	void receive(const entityx::ComponentRemovedEvent<Ai>& e);

	void receive(const entityx::ComponentAddedEvent<Car>& e);
	void receive(const entityx::ComponentRemovedEvent<Car>& e);

private:
	sf::RenderWindow & m_window; // to draw the text to screen

	entityx::EntityManager& m_entityManager;

	int m_bestLap; // to store best lap
	int m_currentPlayerLap; //players current lap
	int m_lapUndone; // when player undo a lap
	float m_bestTime; // to store best time
	float m_currentTime; // to store current time of player

	//the rectangle that holds the texture of minimap border
	sf::RectangleShape m_minimapBorder; 

	// the dial rectangle
	sf::RectangleShape m_dialRect;

	// to display the text to the screen
	sf::Text m_hud;

	// shared pointer to a font
	std::shared_ptr<FontBank> m_font;

	// shared pointer to a view
	std::shared_ptr<sf::View> m_view;

	// keeps an entity player id
	entityx::Entity::Id m_playerId;

	// current race position
	int m_playerRacePos;
	
	// keeps an entity ai id
	std::vector<entityx::Entity::Id> m_aiId;
	// keeps current lap of the ai
	std::vector<int> m_aiCurrentLap;

	// keeps entity of car Ids
	std::vector<entityx::Entity::Id> m_carIds;

	// keeps road number for player
	int m_playerRoadNum;

	// keeps road Id for ai
	std::vector<int> m_aiRoadNum;

	entityx::Entity carThatCompletedLap;
};

#endif // !HUD_SYSTEM_H
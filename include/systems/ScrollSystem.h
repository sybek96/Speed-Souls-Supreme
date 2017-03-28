#ifndef SCROLL_SYSTEM_H
#define SCROLL_SYSTEM_H

#include "SFML\Graphics.hpp"

#include "entityx\System.h"

#include "components\Position.h"
#include "components\Player.h"

#include "util\Debug.h"

#include "Thor\Vectors.hpp"

#include "events\EvAssetsLoaded.h"

/// <summary>
/// @author Huong Thi Thanh Huynh
/// @version 1.0
/// @brief Will handle the scrolling of the player's view
/// 
/// 
/// </summary>

class ScrollSystem
	: public entityx::System<ScrollSystem>
	, public entityx::Receiver<ScrollSystem>
{
public:
	ScrollSystem(
		sf::RenderWindow& window
		, std::shared_ptr<sf::RectangleShape> viewRect
		, std::shared_ptr<sf::View> view);

	void update(entityx::EntityManager& entities
		, entityx::EventManager& events
		, double dt) override;

	void configure(entityx::EventManager & eventManager);
	void receive(const entityx::ComponentAddedEvent<Player>& e);
	void receive(const EvAssetsLoaded& e);

	void setView(Position::Handle playerPos);
private:
	sf::RenderWindow & m_window; // to draw the new view
	
	std::shared_ptr<sf::View> m_view; // shared pointer of the view of the player
	std::shared_ptr<sf::RectangleShape> m_viewRect; // shared pointer rect of view

	sf::CircleShape m_circle; // circle of view
	entityx::Entity::Id m_playerID; // to store the player ID

};

#endif // !SCROLL_SYSTEM_H
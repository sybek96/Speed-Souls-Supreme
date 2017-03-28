#include "systems\ScrollSystem.h"

/// <summary>
/// @brief Scroll system constructor.
/// 
/// 
/// </summary>
ScrollSystem::ScrollSystem(
	sf::RenderWindow & window
	, std::shared_ptr<sf::RectangleShape> viewRect
	, std::shared_ptr<sf::View> view)

	: m_window(window)
	, m_circle(500.0f)
	, m_viewRect(viewRect)
	, m_view(view)
{
}

/// <summary>
/// @brief Configures the scroll system with any events that may happen
/// 
/// 
/// </summary>
/// <param name="eventManager">Event Manager </param>
void ScrollSystem::configure(entityx::EventManager & eventManager)
{
	eventManager.subscribe<entityx::ComponentAddedEvent<Player>>(*this);
	eventManager.subscribe<EvAssetsLoaded>(*this);
}

/// <summary>
/// @brief To receive an entity with a player component when a player is added
/// </summary>
/// <param name="e">A player adding event</param>
void ScrollSystem::receive(const entityx::ComponentAddedEvent<Player>& e)
{
	m_playerID = e.entity.id(); // get id of player
	entityx::Entity player = e.entity; // get the player entity
	Position::Handle pos = player.component<Position>(); // get pos of player

	//getting the size of the window
	const sf::Vector2f size = sf::Vector2f(
		static_cast<float>(m_window.getSize().x),
		static_cast<float>(m_window.getSize().y));

	m_view->setSize(size); // set the view size to the window size
	m_view->zoom(4.5f); // zooms the view out

	m_view->setCenter(pos->m_position); // set the view once to the player pos
	m_window.setView(*m_view); // set the new view to the window

	m_viewRect->setSize(m_view->getSize()); // set size of view
	m_viewRect->setOrigin((m_view->getSize() / 2.0f)); // set the origin
}

void ScrollSystem::receive(const EvAssetsLoaded& e)
{

}

/// <summary>
/// @brief Updates the scroll system
/// 
/// 
/// </summary>
/// <param name="entities">EntityX entity manager</param>
/// <param name="events">EntityX event manager</param>
/// <param name="dt">milliseconds between updates</param>
void ScrollSystem::update(entityx::EntityManager & entities
	, entityx::EventManager & events
	, double dt)
{
	entityx::Entity player = entities.get(m_playerID); // get the player entity
	Position::Handle playerPos = player.component<Position>(); // get pos of player
	// check if there is player
	if (m_playerID.id())
	{
		setView(playerPos); // set the view to the player position
		m_viewRect->setPosition(m_view->getCenter()); // set the view rectangle to the position of the players view
		//m_window.draw(*m_viewRect);
	}

}
/// <summary>
/// @brief Method to update the view of the window when player goes out of the circle of view.
/// 
/// 
/// </summary>
/// <param name="playerPos"></param>
void ScrollSystem::setView(Position::Handle playerPos)
{
	// check to see if the player is outside of circle before moving the view 
	if ((m_circle.getRadius() * m_circle.getRadius()) < thor::squaredLength(m_circle.getPosition() - playerPos->m_position))
	{

		// vector from circle to player
		sf::Vector2f vectorBetween(playerPos->m_position - m_circle.getPosition());

		// direction from circle to player
		sf::Vector2f vectorBetweenDir = thor::unitVector(vectorBetween);

		// length from centre of circle to player
		float lengthToPlayer = thor::length(vectorBetween);

		// difference between player to centre of circle and the radius of the circle
		float distanceToMove = lengthToPlayer - m_circle.getRadius();

		m_circle.move((vectorBetweenDir * distanceToMove)); // set new pos of circle
		m_view->setCenter(m_circle.getPosition()); // set new center view as the the circle pos
		m_window.setView(*m_view); // set the new view to the window
	}
}


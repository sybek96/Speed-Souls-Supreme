#pragma once
//ecs
#include "entityx\System.h"

//sfml
#include "SFML\Graphics\RectangleShape.hpp"

//components
#include "components\Car.h"
#include "components\Position.h"
#include "components\CollisionBox.h"

//comment
#include "util\ResourceTypes.h"

//events
#include "events\EvDayTime.h"
#include "events\EvNightTime.h"

/// <summary>
/// @author Bartosz Zych
/// @version 1.0
/// @brief this system will controll movement of the ai cars
/// 
/// 
/// @ai cars will follow a set of points on the track which will be 
/// controlled by this system. 
/// </summary>
class HeadLightSystem
	: public entityx::System<HeadLightSystem>
	, public entityx::Receiver<HeadLightSystem>
{
public:
	HeadLightSystem(sf::RenderWindow& window, std::shared_ptr<ResourceHolders> resources);

	void configure(entityx::EventManager& eventManager) override;

	void update(entityx::EntityManager& entities, entityx::EventManager& events, double dt) override;

	void receive(const entityx::ComponentAddedEvent<Car>& e);
	void receive(const entityx::ComponentRemovedEvent<Car>& e);
	void receive(const EvDayTime& e);
	void receive(const EvNightTime& e);

	//left headlights
	std::vector<sf::RectangleShape> m_leftHeadlight;
	//right headlights
	std::vector<sf::RectangleShape> m_rightHeadlight;
	//car ids
	std::vector<entityx::Entity::Id> m_carIDs;
	const float HALF_LIGHT_WIDTH;
	const float HALF_LIGHT_HEIGHT;

	bool m_on;

	//window used to draw the shader
	sf::RenderWindow& m_window;

	//resource manager
	std::shared_ptr<ResourceHolders> m_resources;
};


#ifndef CAR_TRACKING_SYSTEM_H
#define CAR_TRACKING_SYSTEM_H

#include "entityx\System.h"

#include "events\EvChangeGameState.h"
#include "events\EvAssetsLoaded.h"
#include "events\EvLapCompleted.h"
#include "events\EvLapUndone.h"

#include "Thor\Vectors\VectorAlgebra2D.hpp"

#include "components\Position.h"
#include "components\CollisionBox.h"
#include "components\Display.h"
#include "components\Car.h"
#include "components\Road.h"
#include "components\Acceleration.h"
#include "components\Player.h"

#include "util\CollisionDetector.h"
#include "util\RoadCount.h"

/// <summary>
/// @author Rafael Plugge & Sebastian Kruzel
/// @version 1.0
/// @brief tracks if car entity is outside of road.
/// 
/// will also keep track of the tile number that the
/// car is currently on,
/// will send out an event when lap is finished
/// </summary>
class CarTrackingSystem
	: public entityx::System<CarTrackingSystem>
	, public entityx::Receiver<CarTrackingSystem>
{
public:
	//constructor
	CarTrackingSystem(entityx::EntityManager& entityManager);

	//subscribe to events
	void configure(entityx::EventManager& eventManager) override;

	//update system logic
	void update(entityx::EntityManager& entities, entityx::EventManager& events, double dt) override;

	//receive car component added event
	void receive(const entityx::ComponentAddedEvent<Car>& e);
	
	//receive road component added event
	void receive(const entityx::ComponentAddedEvent<Road>& e);
	
	//receive car component removed event
	void receive(const entityx::ComponentRemovedEvent<Car>& e);

	//receive road component removed event
	void receive(const entityx::ComponentRemovedEvent<Road>& e);

	//receive assets loaded event
	void receive(const EvAssetsLoaded& e);

	//Detect if the car entity is off the track
	void detectOffTrack(entityx::Entity& car, entityx::Entity& road, sf::RectangleShape carRect, sf::RectangleShape roadRect);

private:
	
	// reference to entity manager
	entityx::EntityManager& m_entityManager;

	// vector of car ids
	std::vector<entityx::Entity::Id> m_carIds;

	// vector of road ids
	std::vector<entityx::Entity::Id> m_roadIds;

	//Previous road that car was on
	entityx::Entity::Id m_previousRoad;
};

#endif // !CAR_TRACKING_SYSTEM_H
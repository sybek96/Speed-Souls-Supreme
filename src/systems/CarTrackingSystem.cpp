#include "systems\CarTrackingSystem.h"

/// <summary>
/// @brief Default system constructor.
/// 
/// will initialize variables using
/// the initializer list
/// </summary>
CarTrackingSystem::CarTrackingSystem(entityx::EntityManager& entityManager)
	: m_entityManager(entityManager)
	, m_carIds()
	, m_roadIds()
	, m_previousRoad(0u)
	
{
}

/// <summary>
/// @brief configures event manager.
/// 
/// Subscribes system to all receiveable events
/// </summary>
/// <param name="eventManager">defines the event manager</param>
void CarTrackingSystem::configure(entityx::EventManager & eventManager)
{
	eventManager.subscribe<entityx::ComponentAddedEvent<Car>>(*this);
	eventManager.subscribe<entityx::ComponentAddedEvent<Road>>(*this);
	eventManager.subscribe<entityx::ComponentRemovedEvent<Car>>(*this);
	eventManager.subscribe<entityx::ComponentRemovedEvent<Road>>(*this);
	eventManager.subscribe<EvAssetsLoaded>(*this);
}

/// <summary>
/// @brief Checks if any car is off road.
/// 
/// goes through all cars and checks if they are on or off the road
/// when going onto a new road that roads id is assigned to the car
/// </summary>
/// <param name="entities">reference to entity manager</param>
/// <param name="events">reference to event manager</param>
/// <param name="dt">time between frames, in milliseconds</param>
void CarTrackingSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, double dt)
{
	//the car rectangle
	sf::RectangleShape carRect;

	//the road rectangle
	sf::RectangleShape roadRect;

	//needed to do collision checking
	CollisionMinMax collisionMinMax;

	//go through all cars that are currently to be processed
	for (entityx::Entity::Id & carId : m_carIds)
	{
		//get the car
		entityx::Entity& carEntity = entities.get(carId); 
		//get car components
		Position::Handle& carPos = carEntity.component<Position>();
		//get the collision box component
		const CollisionBox::Handle& carBox = carEntity.component<CollisionBox>();
		//set rectangle details
		carRect.setPosition(carPos->m_position.x , carPos->m_position.y);
		carRect.setOrigin((carBox->m_rectangle.width / 2.0f), (carBox->m_rectangle.height / 2.0f));
		carRect.setRotation(carPos->m_rotation);
		sf::Vector2f carRectSize = sf::Vector2f(carBox->m_rectangle.width, carBox->m_rectangle.height);
		carRect.setSize(carRectSize);
		//get the road entity
		entityx::Entity& carOnRoadEntity = entities.get(carEntity.component<Car>()->m_roadId); 
		//get road components
		const Position::Handle& carOnRoadPos = carOnRoadEntity.component<Position>();
		const Display::Handle& carOnRoadDisplay = carOnRoadEntity.component<Display>();
		//set road rectangle
		roadRect.setPosition(carOnRoadPos->m_position.x, carOnRoadPos->m_position.y );
		roadRect.setOrigin(carOnRoadDisplay->m_origin.x, carOnRoadDisplay->m_origin.y);
		roadRect.setRotation(carOnRoadPos->m_rotation);
		sf::Vector2f roadRectSize = sf::Vector2f(carOnRoadDisplay->m_coord.width, carOnRoadDisplay->m_coord.height);
		roadRect.setSize(roadRectSize);

		//check if there is no collision between the car and road rectangle
		if (!CollisionDetector::collision(carRect, roadRect, collisionMinMax))
		{
			//loop through all roads and check if colliding with another one
			for (entityx::Entity::Id & roadId : m_roadIds)
			{
				//get road components
				entityx::Entity& roadEntity = entities.get(roadId);
				const Position::Handle& roadPos = roadEntity.component<Position>();
				const Display::Handle& roadDisplay = roadEntity.component<Display>();

				//set road params
				roadRect.setPosition(roadPos->m_position.x, roadPos->m_position.y);
				roadRect.setOrigin(roadDisplay->m_origin.x, roadDisplay->m_origin.y);
				roadRect.setRotation(roadPos->m_rotation);
				sf::Vector2f roadRectSize = sf::Vector2f(roadDisplay->m_coord.width, roadDisplay->m_coord.height);
				roadRect.setSize(roadRectSize);

				//get the car struct
				Car::Handle& car = carEntity.component<Car>();
				//check collision between road and car
				if (CollisionDetector::collision(carRect, roadRect, collisionMinMax)) 
				{
					//previous road id set to same as current
					m_previousRoad = car->m_roadId; 
					//current id changes to the road id currently checked
					car->m_roadId = roadId;			
					//the previous road entity
					entityx::Entity& prevRoadEntity = entities.get(m_previousRoad); 
					//previous road number
					const auto& prevRoadNum = prevRoadEntity.component<Road>()->m_roadNum; 
					//current road number
					const auto& roadNum = roadEntity.component<Road>()->m_roadNum; 
					// if skipping a road tile set him back to the last road that
					// the car was on currently dont allow any skipping
					if (
						(roadNum - prevRoadNum > 1 || roadNum - prevRoadNum < -1)
						) 
					{
						//check if the lap was completed
						//this happens on transition between last tile and first tile
						if ((prevRoadNum == RoadCount::getLast()) && (roadNum == 0))
						{
							//emit the lap completed event
							events.emit<EvLapCompleted>(carId);
						}
						//check if lap was undone (reversed)
						//this happens on transition between the first tile and the last tile
						else if ((prevRoadNum == 0) && (roadNum == RoadCount::getLast()))
						{
							//emit the lap undone event
							events.emit<EvLapUndone>(carId);
						}
						//otherwise skip has happened so lets reset the car to the last position
						else if (!(
								((prevRoadNum == RoadCount::getLast()) && (roadNum == 0))
								 ||
								((prevRoadNum == 0) && (roadNum == RoadCount::getLast()))
							))
						{
							//get the previous road position
							const auto& prevRoadPos = prevRoadEntity.component<Position>()->m_position;
							//get the previous road rotation
							const auto& prevRoadRotation = prevRoadEntity.component<Position>()->m_rotation;
							//reset car on previous road
							carPos->m_position = prevRoadPos;
							//assign current road id to current
							car->m_roadId = m_previousRoad;
							//reset car's speed
							car->m_accelerateVal = 0;
							//make rotation of car same as road
							carPos->m_rotation = prevRoadRotation + 180;
							//if below zero set to 270 (some roads are rotated at -90)
							if (prevRoadRotation <= 0)
							{
								carPos->m_rotation = 270;
							}
						}
					}
					break;
				}
			}
		}
		//if has component player detect offtrack
		if (carEntity.has_component<Player>())
		{
			detectOffTrack(carEntity, carOnRoadEntity, carRect, roadRect);
		}
	}
}

/// <summary>
/// @brief When car component is added.
/// 
/// Start tracking car entity
/// </summary>
/// <param name="e">defines the event struct</param>
void CarTrackingSystem::receive(const entityx::ComponentAddedEvent<Car>& e)
{
	bool carFound = false;

	// check if a car has already been added
	const auto& begin = m_carIds.begin();
	const auto& end = m_carIds.end();
	for (auto itt = begin; itt != end; itt++)
	{
		if (*(itt) == e.entity.id())
		{
			carFound = true;
			break;
		}
	}

	if (!carFound)
	{
		//add to the car-ids vector
		m_carIds.push_back(e.entity.id());
	}
}

/// <summary>
/// @brief When road component is added.
/// 
/// Start tracking road entity
/// </summary>
/// <param name="e">defines the event struct</param>
void CarTrackingSystem::receive(const entityx::ComponentAddedEvent<Road>& e)
{
	//add to the road-ids vector
	m_roadIds.push_back(e.entity.id());
}

/// <summary>
/// @brief When car component is removed.
/// 
/// Stop tracking car entity
/// </summary>
/// <param name="e">defines the event struct</param>
void CarTrackingSystem::receive(const entityx::ComponentRemovedEvent<Car>& e)
{
	//erase the element that was removed
	const auto& begin = m_carIds.begin();
	const auto& end = m_carIds.end();
	for (auto itt = begin; itt != end; itt++)
	{
		if ((*itt) == e.entity.id())
		{
			m_carIds.erase(itt);
			break;
		}
	}
}

/// <summary>
/// @brief When road component is removed.
/// 
/// Stop tracking road entity
/// </summary>
/// <param name="e">defines the event struct</param>
void CarTrackingSystem::receive(const entityx::ComponentRemovedEvent<Road>& e)
{
	//erase the element that was removed
	const auto& begin = m_carIds.begin();
	const auto& end = m_carIds.end();
	for (auto itt = begin; itt != end; itt++)
	{
		if ((*itt) == e.entity.id())
		{
			m_roadIds.erase(itt);
			break;
		}
	}
}

/// <summary>
/// @brief All entities are created.
/// 
/// Assign all car's car component' road id to the starting road
/// </summary>
/// <param name="e">defines the event struct</param>
void CarTrackingSystem::receive(const EvAssetsLoaded & e)
{
	//if playing
	if (e.m_gameState == GameState::Playing)
	{
		//if there if there are roads and cars
		if ((m_carIds.size() > 0) && (m_roadIds.size() > 0))
		{
			//get id of road
			entityx::Entity::Id startRoadId;
			//loop through all roads
			for (auto & roadId : m_roadIds)
			{
				//get the road struct
				Road::Handle road = m_entityManager.get(roadId).component<Road>();
				//cars start on checkered road
				if (road->m_type == Assets::Textures::Track::CheckStart)
				{
					//assign start road to road id
					startRoadId = roadId;
				}
			}
			//loop through cars and assing start road as their current road
			for (auto & carId : m_carIds)
			{
				Car::Handle car = m_entityManager.get(carId).component<Car>();

				car->m_roadId = startRoadId;
			}
		}

	}
}


/// <summary>
/// @brief detect if a car is off the road
/// 
/// Sets up collision rectangles for straight and curved roads and calculates
/// if the car is off the road or on it
/// </summary>
/// <param name="car"></param>
/// <param name="road"></param>
void CarTrackingSystem::detectOffTrack(entityx::Entity& carEnt, entityx::Entity& roadEnt, sf::RectangleShape carRect, sf::RectangleShape roadRect)
{
	//needed for collision detection
	CollisionMinMax collisionMinMax;
	//origin of the car
	sf::Vector2f carOrigin; 
	//get the car component
	Car::Handle& car = carEnt.component<Car>();
	//get car position collision box and acceleration components
	const Position::Handle& carPos = carEnt.component<Position>(); //position of the car
	const CollisionBox::Handle& carBox = carEnt.component<CollisionBox>(); //collision box of the car
	Acceleration::Handle& carAccel = carEnt.component<Acceleration>(); //acceleration of the car

	const Position::Handle& roadPos = roadEnt.component<Position>(); //road position
	const Display::Handle& roadDisplay = roadEnt.component<Display>(); //road display
	const Road::Handle& road = roadEnt.component<Road>(); //the road component

	//get rotation of the road
	const float& rotation = roadPos->m_rotation;

	//bottom left corner of road (rotated)
	sf::Vector2f m_bottomLeft(roadPos->m_position + thor::rotatedVector(sf::Vector2f(-roadDisplay->m_origin.x, 0.0f), rotation));
	//bottom Right corner
	sf::Vector2f rightOffset(0.0f, 0.0f);
	rightOffset.x = roadDisplay->m_coord.width - roadDisplay->m_origin.x;
	sf::Vector2f m_bottomRight(roadPos->m_position + thor::rotatedVector(rightOffset, rotation));

	//check offroad for start/finish line
	//CHECKERED ROAD
	if (road->m_type == Assets::Textures::Track::CheckStart) 
	{
		//if no collision
		if(!(CollisionDetector::collision(carRect, roadRect, collisionMinMax))) 
		{
			car->m_onRoad = false;
		}
		//if collision
		else  
		{
			car->m_onRoad = true;
		}
	}
	//STRAIGHT ROAD
	else if (road->m_type == Assets::Textures::Track::Straight) 
	{
		if (!(CollisionDetector::collision(carRect, roadRect, collisionMinMax)))
		{
			car->m_onRoad = false;
		}
		else
		{
			car->m_onRoad = true;
		}
	}
	//CURVE LEFT
	else if (road->m_type == Assets::Textures::Track::CurveLeft) 
	{
			if (!(thor::length(carRect.getPosition() - m_bottomLeft) < 1126 && thor::length(carRect.getPosition() - m_bottomLeft) > 204))
			{
				car->m_onRoad = false;
			}
			else
			{
				car->m_onRoad = true;
			}
	}
	//CURVE RIGHT
	else if (road->m_type == Assets::Textures::Track::CurveRight) 
	{
			if (!(thor::length(carRect.getPosition() - m_bottomRight) < 1126 && thor::length(carRect.getPosition() - m_bottomRight) > 204))
			{
				car->m_onRoad = false;
			}
			else
			{
				car->m_onRoad = true;
			}
	}
}

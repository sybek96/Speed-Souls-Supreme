#include "systems\CollisionSystem.h"


// enter comment here bart 
CollisionSystem::CollisionSystem()
	: m_finalEntityRot(0)
	, m_finalPlayerRot(0)
{
	

}

/// <summary>
/// @brief Configures the collision system with any events that may happen
/// 
/// 
/// 
/// </summary>
/// <param name="eventManager">Event Manager </param>
void CollisionSystem::configure(entityx::EventManager & eventManager)
{
	eventManager.subscribe<entityx::ComponentAddedEvent<Player>>(*this);
	eventManager.subscribe<entityx::ComponentAddedEvent<Ai>>(*this);
	eventManager.subscribe<entityx::ComponentRemovedEvent<Ai>>(*this);
}


/// <summary>
/// @brief Main update function for collision system
/// 
/// 
/// 
/// Checks for any collision between entities with CollisionBox, Display and Position 
/// run the appropriete collision response
/// </summary>
/// <param name="entities"> entity manager which manages all entitiesin the game</param>
/// <param name="events"> event managaer which manages all events in the game</param>
/// <param name="dt">delta time</param>
void CollisionSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, double dt)
{
	Position::Handle position;
	Velocity::Handle velocity;
	CollisionBox::Handle collisionBox;

	CollisionMinMax collisionMinMax;

	const float& deltaTime = static_cast<float>(dt / 1000.0);

	//make player entity
	entityx::Entity& player = entities.get(m_playerID);

	//get position of player entity
	Position::Handle& playerPos = player.component<Position>();
	//get the velocity component of player entity
	Velocity::Handle& playerVel = player.component<Velocity>();
	//get collison box component of the player entity
	CollisionBox::Handle& playerBox = player.component<CollisionBox>();

	if (player.has_component<Player>())
	{
		//get the rotation angle of the player
		const float& l_playerRot = (playerPos->m_rotation);

		//set size of the player rectangle
		m_playerRect.setSize(sf::Vector2f(playerBox->m_rectangle.width, playerBox->m_rectangle.height));

		//set origin of the player rectangle to players origin
		m_playerRect.setOrigin(m_playerRect.getSize() * 0.5f);

		//set position of the player rectangle to players position
		m_playerRect.setPosition(playerPos->m_position);

		//set rotation of the player rectangle to players rotation
		m_playerRect.setRotation(l_playerRot);

		//construct a velocity vector for the player
		m_playerVelf = playerVel->m_velocity;

	}

	

	for (entityx::Entity::Id& aiId : m_aiIDs)
	{
		entityx::Entity& entity = entities.get(aiId);
		Position::Handle& aiPos = entity.component<Position>();
		Velocity::Handle& aiVel = entity.component<Velocity>();
		CollisionBox::Handle& aiBox = entity.component<CollisionBox>();

		
		if (entity.has_component<Ai>())
		{
			//get the rotation angle of the player
			const float& l_aiRot = (aiPos->m_rotation);

			//set size of the ai rectangle
			m_entityRect.setSize(sf::Vector2f(aiBox->m_rectangle.width, aiBox->m_rectangle.height));

			//set origin of the ai rectangle to ais origin
			m_entityRect.setOrigin(m_entityRect.getSize() * 0.5f);

			//set position of the ai rectangle to ais position
			m_entityRect.setPosition(aiPos->m_position);

			//set rotation of the ai rectangle to ais rotation
			m_entityRect.setRotation(l_aiRot);

			//construct a velocity vector for the ai
			m_entityVelf = aiVel->m_velocity;
		}
		else
		{

			const float& l_entityRot = (position->m_rotation);

			//make a rectangle shape for the current entity in ther loop according to its collision box
			m_entityRect.setSize(sf::Vector2f(collisionBox->m_rectangle.width, collisionBox->m_rectangle.height));

			//set values of the entity rectangle
			//set origin of entity's rect
			m_entityRect.setOrigin(m_entityRect.getSize() * 0.5f);

			//set position of entity's rect
			m_entityRect.setPosition(position->m_position);

			//set rotation of entity's rect
			m_entityRect.setRotation(l_entityRot);

			//construct a velocity vector for the entity
			m_entityVelf = velocity->m_velocity;

		}

		if (CollisionDetector::collision(m_playerRect, m_entityRect, collisionMinMax))
		{
			//check for collision between ai 
			collisionResponse(player, entity, collisionMinMax, deltaTime);
		}

	}

	//debugMsg("player rot", playerPos->m_rotation);
	//debugMsg("entity rot", position->m_rotation);
}

/// <summary>
/// @brief assign member variable when CollisionBox is added into an entity
/// 
/// 
/// 
/// </summary>
/// <param name="e">event of adding a CollisionBox component</param>
void CollisionSystem::receive(const entityx::ComponentAddedEvent<Player>& e)
{
	m_playerID = e.entity.id();
}

void CollisionSystem::receive(const entityx::ComponentAddedEvent<Ai>& e)
{
	m_aiIDs.push_back(e.entity.id());
}

void CollisionSystem::receive(const entityx::ComponentRemovedEvent<Ai>& e)
{
	const auto& begin = m_aiIDs.begin();
	const auto& end = m_aiIDs.end();
	
	for (auto itt = begin; itt != end; itt++)
	{
		if (*(itt) == e.entity.id())
		{
			m_aiIDs.erase(itt);
			break;
		}
	}
}

/// <summary>
/// @brief Run basic collision response
/// 
/// 
/// </summary>
/// <param name="playerPos"> will modify players position</param>
/// <param name="entityPos">will modify entities position</param>
void CollisionSystem::collisionResponse(entityx::Entity& player, entityx::Entity& entity, const CollisionMinMax& minMax, const float& dt)
{


	//get position of player
	sf::Vector2f& l_playerPos = player.component<Position>()->m_position;
	//get velocity of player
	sf::Vector2f& l_playerVel = player.component<Velocity>()->m_velocity;
	//get acceleration of player
	sf::Vector2f& l_playerAcc = player.component<Acceleration>()->m_acceleration;
	//get rotation of player
	float& l_playerRot = player.component<Position>()->m_rotation;

	//get position of entity
	sf::Vector2f& l_entityPos = entity.component<Position>()->m_position;
	//get velocity of entity
	sf::Vector2f& l_entityVel = entity.component<Velocity>()->m_velocity;
	//get rotation of entity
	float& l_entityRot = entity.component<Position>()->m_rotation;
	float entityLen = thor::length(l_entityVel);

	//create an axis between 2 origins
	sf::Vector2f force = l_playerPos - l_entityPos;
	if (!(force.x == 0.0f && force.y == 0.0f))
	{
		//make a unti vector of the axis
		sf::Vector2f forceDir = thor::unitVector(force);

		//cheking how much to movve the player back by getting the distance it went into the entity
		if (minMax.m_max1 > minMax.m_min2 && minMax.m_max2 > minMax.m_min1)
		{
			const float& dif1 = (minMax.m_max1 - minMax.m_min2);
			const float& dif2 = (minMax.m_max2 - minMax.m_min1);
			if (dif1 < dif2)
			{
				//make a force vector according to the push back value
				force = dif1 * forceDir;
			}
			else
			{
				//make a force vector according to the push back value
				force = dif2 * forceDir;
			}
		}
	}

	//apply move back vector to the player's position so it will not go into the entity
	l_playerPos += (force * dt) * 0.05f;

	//slow down the player when he runs into a ai car
	if (player.component<Car>()->m_accelerateVal > 400)
	{
		player.component<Car>()->m_accelerateVal -= 30.0f; // <-- magic
	}


	

	//get the difference between 2 angles
	float player_entity_DIF = l_playerRot - l_entityRot;


	//when player collides with the entity and the difference angle between the 2 is 
	// greater than 0 and less than 90 then 
	// rotate player anticlockwise
	// rotate entity clockwise
	if (player_entity_DIF > 0 && player_entity_DIF < 90)
	{
		l_playerRot--;
		l_entityRot++;
	}

	//when player collides with the entity and the difference angle between the 2 is 
	// greater than -90 and less than 0 then 
	// rotate player clockwise
	// rotate entity anticlockwise
	else if (player_entity_DIF > -90 && player_entity_DIF < 0)
	{
		l_playerRot++;
		l_entityRot--;
	}

	//when player collides with the entity and the difference angle between the 2 is 
	// greater than 0 and less than 180 then 
	// rotate player clockwise
	// rotate entity anticlockwise
	else if (player_entity_DIF > 0 && player_entity_DIF < 180)
	{
		l_playerRot++;
		l_entityRot--;
	}

	//when player collides with the entity and the difference angle between the 2 is 
	// less than -90 and greater than -360 then 
	// rotate player anticlockwise
	// rotate entity clockwise
	else if (player_entity_DIF < -90 && player_entity_DIF > -360)
	{
		l_playerRot--;
		l_entityRot++;
	}
	else
	{

#pragma region Elastic collision code
		//sf::Vector2f playerVelVector = l_playerVel;
		//sf::Vector2f entityVelVector = l_entityVel;

		////normalise the force vector
		//sf::Vector2f l_normalForce = l_playerVel - l_entityVel;
		////get the prependicula vector
		//sf::Vector2f l_perpendicular = thor::perpendicularVector(l_normalForce);

		////normalise force vector if not 0
		//if (!(l_normalForce.x == 0.0f && l_normalForce.y == 0.0f))
		//{
		//	l_normalForce = thor::unitVector(l_normalForce);
		//}

		////initial velocity of player along the normalised vector
		//float l_playerInitVelN = thor::dotProduct(playerVelVector, l_normalForce);
		////inital velocity of the entity along the normalised vector
		//float l_entityInitVelN = thor::dotProduct(entityVelVector, l_normalForce);

		////initial velocity of the player along the perpendicual vector to normalised vector
		//float l_playerInitVelP = thor::dotProduct(playerVelVector, l_perpendicular);
		////initial velocity of the entity along the perpendicula vector to the normalised vector
		//float l_entityInitVelP = thor::dotProduct(entityVelVector, l_perpendicular);

		////calculate the magnitude of final velocity of thge player 
		//float l_playerFinalVel = ((CAR_MASS * l_entityInitVelN * (E + 1) + l_playerInitVelN * (CAR_MASS - (1 * CAR_MASS))) / CAR_MASS + CAR_MASS);
		////calculate the magnitude of final velocity of the entity
		//float l_entityFinalVel = (((CAR_MASS * l_playerInitVelN * (E + 1)) + l_entityInitVelN * (CAR_MASS - (1 * CAR_MASS))) / CAR_MASS + CAR_MASS);

		//sf::Vector2f l_finalPlayerVector, l_finalEntityVector; //final vecotor of player and entity

		////calculate the final velocity vector for the player  by using the dot product
		//l_finalPlayerVector = ((l_playerFinalVel * l_normalForce) + (l_playerFinalVel * l_perpendicular));
		////calculate the final velocity vector of the entity 
		////by using the dot product
		//l_finalEntityVector = ((l_entityFinalVel * l_normalForce) + (l_entityFinalVel * l_perpendicular));

		//l_playerVel = l_finalPlayerVector;
		//l_entityVel = l_finalEntityVector;


		//if (!(l_finalPlayerVector.x == 0 && l_finalPlayerVector.y == 0))
		//{
		//l_playerRot = thor::polarAngle(l_finalPlayerVector);
		//}
		//if (!(l_finalEntityVector.x == 0 && l_finalEntityVector.y == 0))
		//{
		//l_entityRot = thor::polarAngle(l_finalEntityVector) ;
		//}
#pragma endregion
	}

	



	
}





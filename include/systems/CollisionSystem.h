#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "entityx\System.h"
#include <memory>
#include <Thor\Math\Trigonometry.hpp>
#include <Thor\Vectors\VectorAlgebra2D.hpp>

//Components
#include "components\CollisionBox.h"
#include "components\Display.h"
#include "components\Position.h"
#include "components\Player.h"
#include "components\Ai.h"


//Utils
#include "util\CollisionDetector.h"
#include "util\BTMath.h"
#include "util\Debug.h"

//Entities
#include "entities\CarCreator.h"


/// <summary>
/// @author Bartosz Zych
/// @version 1.0
/// @brief This System will handle collision response
/// 
/// this system will handle collision response between player and
/// entities with CollisionBox, Display, and Position Components
/// </summary>
class CollisionSystem 
	: public entityx::System<CollisionSystem>
	, public entityx::Receiver<CollisionSystem>
{
public:

	CollisionSystem();
	
	void configure(entityx::EventManager& eventManager) override;
	
	void update(entityx::EntityManager& entities, entityx::EventManager& events, double dt) override;

	void receive(const entityx::ComponentAddedEvent<Player>& e);
	void receive(const entityx::ComponentAddedEvent<Ai>& e);
	void receive(const entityx::ComponentRemovedEvent<Ai>& e);

private:

	

	//Player id 
	entityx::Entity::Id m_playerID;
	std::vector<entityx::Entity::Id> m_aiIDs;

	sf::RectangleShape m_playerRect;	//make a player rectangle
	sf::RectangleShape m_entityRect;	//make an entity rectangle

	float m_finalPlayerRot;
	float m_finalEntityRot;

	sf::Vector2f m_playerVelf;
	sf::Vector2f m_entityVelf;
	

	

	void collisionResponse(entityx::Entity& player, entityx::Entity& entity, const CollisionMinMax& minMax, const float& dt);
	const int E = 1;
	const int CAR_MASS = 1;
};

#endif // !COLLISION_SYSTEM_H

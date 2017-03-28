#pragma once

//ecs
#include "entityx\System.h"

//smfl
#include "SFML\System\Vector2.hpp"

//thor
#include <Thor\Math\Trigonometry.hpp>
#include <Thor\Vectors\VectorAlgebra2D.hpp>

//components
#include "components\Ai.h"
#include "components\Position.h"
#include "components\Velocity.h"
#include "components\Acceleration.h"
#include "components\Road.h"
#include "components\Display.h"
#include "components\CollisionBox.h"

//utils
#include "util\Debug.h"
#include "util\BTMath.h"


/// <summary>
/// @author Bartosz Zych
/// @version 1.0
/// @brief this system will controll movement of the ai cars
/// 
/// 
/// @ai cars will follow a set of points on the track which will be 
/// controlled by this system. 
/// </summary>
class AiSystem 
	: public entityx::System<AiSystem>
	, public entityx::Receiver<AiSystem>
{
public:

	AiSystem();
	
	void configure(entityx::EventManager& eventManager) override;

	void update(entityx::EntityManager& entities, entityx::EventManager& events, double dt) override;

	void receive(const entityx::ComponentAddedEvent<Ai>& e);
	void receive(const entityx::ComponentAddedEvent<Road>& e);
	void receive(const entityx::ComponentRemovedEvent<Ai>& e);
	void receive(const entityx::ComponentRemovedEvent<Road>& e);

private:
	//sf::Vector2f truncate(sf::Vector2f v, float const num);

	// will return a vector to the next point in the path
	//and manage the looping between the points
	sf::Vector2f followPath(const sf::Vector2f& aiPos, const int& pathNode, int& node);

	/// <summary>
	/// @brief find closest object to an ai car 
	/// 
	/// 
	/// </summary>
	/// <param name="id">id of which will represent and ai car</param>
	/// <param name="entities">enitity manager whoch holds all entities</param>
	/// <returns></returns>
	const sf::CircleShape mostThreatening(entityx::Entity::Id id, entityx::EntityManager& entities);

	/// <summary>
	/// @brief create a avoiding vector
	/// 
	/// 
	/// make a vector and add it onto a velocity vector of an ai car 
	/// when it gets in range of an obstacle
	/// </summary>
	/// <param name="id">id of ai car</param>
	/// <param name="entities">enitity manager whoch holds all entities</param>
	/// <returns></returns>
	sf::Vector2f collisionAvoidance(entityx::Entity::Id id, entityx::EntityManager & entities);

	//will contain all points of a path
	std::vector<std::vector<sf::Vector2f>> m_paths;

	std::vector<sf::CircleShape> m_obstacles;

	//id of the ai
	 std::vector<entityx::Entity::Id> m_aiIDs;
	
	 //vector representing point that the ai seeks
	 sf::Vector2f m_vectorToSeek;

	 // distance that the ai must be
	 //  to be considered "touching" the node
	 const float m_NODE_TRIGGER_DIST;



	//--------------------------------------
	//current points of the ai to follow
	

	sf::Vector2f m_ahead;

	sf::Vector2f m_halfAhead;

	const float MAX_SEE_AHEAD;

	const float MAX_AVOID_FORCE;

	const float MAX_FORCE;

	const float MAX_SPEED;

	
};



#include "systems\AiSystem.h"


AiSystem::AiSystem()
	: m_vectorToSeek(0.0f, 0.0f)
	, m_NODE_TRIGGER_DIST(450.0f)
	, MAX_SEE_AHEAD(100.0f)
	, MAX_AVOID_FORCE(50.0f)
	, MAX_FORCE(50.0f)
	, MAX_SPEED(1000.0f)
{
	
}

void AiSystem::configure(entityx::EventManager & eventManager)
{
	eventManager.subscribe<entityx::ComponentAddedEvent<Ai>>(*this);
	eventManager.subscribe<entityx::ComponentAddedEvent<Road>>(*this);
	eventManager.subscribe<entityx::ComponentRemovedEvent<Ai>>(*this);
	eventManager.subscribe<entityx::ComponentRemovedEvent<Road>>(*this);
}

void AiSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, double dt)
{

	const float& deltaTime = static_cast<float>(dt) / 1000;
	//create a handle to represent ai component
	Ai::Handle ai;

	
		for (int i = 0; i < m_aiIDs.size(); i++)
		{
			entityx::Entity& aiEntity = entities.get(m_aiIDs[i]);

			if (
				aiEntity.has_component<Position>()
				&& aiEntity.has_component<Velocity>()
				)
			{
				Ai::Handle ai = aiEntity.component<Ai>();
				Position::Handle position = aiEntity.component<Position>();
				Velocity::Handle velocity = aiEntity.component<Velocity>();
				
				auto& aiPos = position->m_position;
				auto& aiRot = position->m_rotation;
				auto& aiVel = velocity->m_velocity;
				auto& aiPath = ai->m_nodePath;
				auto& aiSteering = ai->m_steering;
				auto& aiNodeNum = ai->m_nodeNum;

				
				m_vectorToSeek = followPath(aiPos, aiPath, aiNodeNum);
				if (!(m_vectorToSeek.x == 0 && m_vectorToSeek.y == 0))
				{
					//m_vectorToSeek = thor::unitVector(m_vectorToSeek);
					// max turning force of the car
					
					aiSteering = Math::truncate(m_vectorToSeek, MAX_FORCE);
					aiSteering += collisionAvoidance(m_aiIDs[i], entities);
					//max speed of the car
					aiVel = Math::truncate(aiSteering + aiVel, MAX_SPEED);

					//update cars position
					aiPos += aiVel * deltaTime;
					m_obstacles[i].setPosition(aiPos);
					//set rotation of the car acording to its velocity vector
					aiRot = (thor::polarAngle(aiVel) - 90.0f);
					m_obstacles[i].setRotation(aiRot);
				}
			}
		
		
	}
}

/// <summary>
/// @brief assign id of the ai whenever an ai component is added to a car
/// 
/// 
/// </summary>
/// <param name="e">event of adding ai component</param>
void AiSystem::receive(const entityx::ComponentAddedEvent<Ai>& e)
{
	m_aiIDs.push_back(e.entity.id());
	//get ai's entity
	entityx::Entity aiEntity = e.entity;
	sf::CircleShape aiCirc;
	const CollisionBox::Handle aiBox = aiEntity.component<CollisionBox>();
	sf::Vector2f aiOrigin = aiEntity.component<Display>()->m_origin;
	aiCirc.setRadius((static_cast<float>(aiBox->m_rectangle.height)));
	aiCirc.setOrigin(aiOrigin);
	m_obstacles.push_back(std::move(aiCirc));

}

/// <summary>
/// @brief assign id of the ai whenever an ai component is added to a car
/// 
/// 
/// </summary>
/// <param name="e">event of adding ai component</param>
void AiSystem::receive(const entityx::ComponentAddedEvent<Road>& e)
{
	//represents the road width
	float const ROAD_WIDTH = 922;
	//represents the road height
	float const ROAD_HEIGHT = 886;
	//represents one part of the roads height
	float verticalPart;
	//represents one part of the roads width
	float horizontalPart = ROAD_WIDTH / 4;

	sf::Vector2f l_pointToAdd;
	

	//represents a road entity 
	entityx::Entity roadEntity = e.entity;
	if (m_paths.size() <= 0)
	{
		for (int i = 0; i < 3; i++)
		{
			m_paths.push_back(std::vector<sf::Vector2f>());
		}
	}

	//type of road checkstar/srtaight/leftTurn/rightTurn
	auto roadType = roadEntity.component<Road>()->m_type;
	//roads position
	const sf::Vector2f& roadPos = roadEntity.component<Position>()->m_position;
	//roads rotation
	float const roadRot = roadEntity.component<Position>()->m_rotation;
	//roads origin 
	sf::Vector2f roadOrigin = roadEntity.component<Display>()->m_origin;

	//bottom left conrener of the road 
	sf::Vector2f l_bottomLeft;

	//position of the final y value for each road
	float yPos;

	//check if the road is a straight road
	if (roadType == Assets::Textures::Track::Straight)
	{
		verticalPart = ROAD_HEIGHT / 4;
		//pick random 3 points on a straight road
		
			//vertical point of the road
			yPos = verticalPart *3 ;

			l_pointToAdd.y = -(yPos);
			//generate a point  for first path
			l_pointToAdd.x = -horizontalPart;
			//right hand side of the road and rotate about the roads rotation
			m_paths[0].push_back(std::move(roadPos + thor::rotatedVector(l_pointToAdd, roadRot)));

			//generate a point on second path
			l_pointToAdd.x = 0.0f;
			//centre of the road and rotate about the roads rotation
			m_paths[1].push_back(std::move(roadPos + thor::rotatedVector(l_pointToAdd, roadRot)));

			//generate a point for third path
			l_pointToAdd.x = horizontalPart;
			//left side of the road and rotate about the roads rotation
			m_paths[2].push_back(std::move(roadPos + thor::rotatedVector(l_pointToAdd, roadRot)));
		
	}
	else if (roadType == Assets::Textures::Track::CurveLeft)
	{
		//l_bottomLeft = sf::Vector2f(roadPos + sf::Vector2f(-roadOrigin.x, 0.0f));
		//divide verticle size of the road into 4
		verticalPart = ROAD_HEIGHT / 4;
		
		//declare points for 1st path
		for (int i = 0; i < 6; i++)
		{
			switch (i)
			{
			case 0:
				//1st point
				l_pointToAdd.x = -horizontalPart;
				l_pointToAdd.y = 0;
				break;
			case 1:
				//2st point
				l_pointToAdd.x = -horizontalPart - (horizontalPart / 4);
				l_pointToAdd.y = -verticalPart - (verticalPart / 2);
				break;
			case 2:
				//3st point
				l_pointToAdd.x = -horizontalPart - (horizontalPart / 2);
				l_pointToAdd.y = -(verticalPart * 2);
				break;
			case 3:
				//4st point
				l_pointToAdd.x = -(horizontalPart * 2) + (horizontalPart / 4);
				l_pointToAdd.y = -(verticalPart * 2) - (verticalPart / 2);
				break;
			case 4:
				//5st point
				l_pointToAdd.x = -(horizontalPart*2) - (horizontalPart/4);
				l_pointToAdd.y = -(verticalPart * 3) + (verticalPart / 4);
				break;
			case 5:
				//6st point
				l_pointToAdd.x = -(horizontalPart * 3);
				l_pointToAdd.y = -(verticalPart * 3);
				break;
			default:
				yPos = verticalPart + (verticalPart / 2);
				l_pointToAdd.x = (-horizontalPart) - ((horizontalPart / 4) * i);
				l_pointToAdd.y = verticalPart + (yPos * i);
				break;
			}
			
			m_paths[0].push_back(std::move(roadPos + thor::rotatedVector(l_pointToAdd , roadRot)));
			
		}

		//declare points for second path of the ai
		for (int i = 0; i < 5; i++)
		{
			//make i/8 of a path part
			yPos = verticalPart / 4;
			

			switch (i)
			{
			case 0:
				//1st point
				l_pointToAdd.x = 0;
				l_pointToAdd.y = 0;
				break;
			case 1:
				//2nd point
				l_pointToAdd.x = -(horizontalPart / 4);
				l_pointToAdd.y =  -verticalPart + yPos;
				break;
			case 2:
				//3rd point
				l_pointToAdd.x = -(horizontalPart)+ (horizontalPart / 4);
				l_pointToAdd.y = -verticalPart - yPos;
				break;
			case 3:
				//4th point
				l_pointToAdd.x = -(horizontalPart) - (horizontalPart/2);
				l_pointToAdd.y = -(verticalPart*2) + yPos;
				break;
			case 4:
				//5th point
				l_pointToAdd.x = -(horizontalPart * 3);
				l_pointToAdd.y = -verticalPart*2;
				break;
			default:
				break;
			}
			m_paths[1].push_back(std::move(roadPos + thor::rotatedVector(l_pointToAdd, roadRot)));

		}

		//declare points for third path
		for (int i = 0; i < 5; i++)
		{

			yPos = (verticalPart) / 4;
			switch (i)
			{
			case 0:
				//1st point
				l_pointToAdd.x = horizontalPart;
				l_pointToAdd.y = 0;
				break;
			case 1:
				//2nd point
				l_pointToAdd.x = horizontalPart - (horizontalPart/4);
				l_pointToAdd.y = -verticalPart + yPos;
				break;
			case 2:
				//3rd point
				l_pointToAdd.x = horizontalPart/4;
				l_pointToAdd.y = -verticalPart - yPos;
				break;
			case 3:
				//4th point
				l_pointToAdd.x = -horizontalPart - (horizontalPart / 4);
				l_pointToAdd.y = -(verticalPart*2) + yPos;
				break;
			case 4:
				//5th point
				l_pointToAdd.x = -(horizontalPart*3);
				l_pointToAdd.y = -(verticalPart * 2);
				break;
			default:
				break;
			}
			
			m_paths[2].push_back(std::move(roadPos + thor::rotatedVector(l_pointToAdd, roadRot)));
		}
	}
	else if (roadType == Assets::Textures::Track::CurveRight)
	{
		//l_bottomLeft = sf::Vector2f(roadPos + sf::Vector2f(-roadOrigin.x, 0.0f));
		//divide verticle size of the road into 4
		verticalPart = ROAD_HEIGHT / 4;

		//declare points for first path of the ai onna right curve
		for (int i = 0; i < 4; i++)
		{
			//make 1/8 of the vertical part
			yPos = verticalPart / 2;
			switch (i)
			{
			case 0:
				l_pointToAdd.x = -horizontalPart;
				l_pointToAdd.y = 0;
				break;
				//second point of the second path
			case 1:
				l_pointToAdd.x = -(horizontalPart / 2);
				l_pointToAdd.y = -verticalPart - yPos;
				break;
				//third point of the second path
			case 2:
				l_pointToAdd.x = (horizontalPart / 2);
				l_pointToAdd.y = (-verticalPart * 2) - yPos;
				break;
			case 3:
				l_pointToAdd.x = (horizontalPart * 3);
				l_pointToAdd.y = -verticalPart * 3;
				break;
			default:
				break;
			}
			
			sf::Vector2f rightOffset(0.0f, 0.0f);
			rightOffset.x = (ROAD_WIDTH/2) - roadOrigin.x;
			//sf::Vector2f m_bottomRight(roadPos + thor::rotatedVector(rightOffset + l_pointToAdd, roadRot));
			m_paths[0].push_back(std::move(roadPos + thor::rotatedVector(rightOffset + l_pointToAdd, roadRot)));
		}

		//declare points for 2nd path
		for (int i = 0; i < 6; i++)
		{
			switch (i)
			{
			case 0:
				l_pointToAdd.x = 0;
				l_pointToAdd.y = 0;
				break;
			case 1:
				yPos = verticalPart / 4;
				l_pointToAdd.x = (horizontalPart / 4);
				l_pointToAdd.y = -verticalPart + yPos;
				break;
			case 2:
				yPos = verticalPart / 4;
				l_pointToAdd.x = ((horizontalPart / 4) * 2);
				l_pointToAdd.y = -verticalPart - yPos;
				break;
			case 3:
				yPos = verticalPart / 4;
				l_pointToAdd.x = horizontalPart;
				l_pointToAdd.y = -verticalPart - (yPos * 2);
				break;
			case 4:
				//divide 1 part of the verticle part into 4 (make 1/8th part)
				yPos = verticalPart / 4;
				l_pointToAdd.x = horizontalPart + ((horizontalPart / 4)*2);
				l_pointToAdd.y = -(verticalPart * 2) + yPos;
				break;
			case 5:
				l_pointToAdd.x = horizontalPart*3;
				l_pointToAdd.y = -(verticalPart * 2);
				break;
			
			default:
				break;
			}

			m_paths[1].push_back(std::move(roadPos + thor::rotatedVector(l_pointToAdd, roadRot)));

		}

		

		//declare points for third path
		for (int i = 0; i < 5; i++)
		{

			yPos = (verticalPart) / 4;
			switch (i)
			{
			case 0:
				l_pointToAdd.x = horizontalPart;
				l_pointToAdd.y = 0;
				break;
			case 1:
				l_pointToAdd.x = horizontalPart + (horizontalPart/4);
				l_pointToAdd.y = -(verticalPart*2) + yPos;
				break;
			case 2:
				l_pointToAdd.x = horizontalPart + ((horizontalPart / 4) * 2);
				l_pointToAdd.y = -(verticalPart * 3) + yPos;
				break;
			case 3:
				l_pointToAdd.x = horizontalPart*2;
				l_pointToAdd.y = -ROAD_HEIGHT + (yPos*2);
				break;
			case 4:
				l_pointToAdd.x = horizontalPart*3;
				l_pointToAdd.y = -ROAD_HEIGHT;
				break;
			default:
				break;
			}

			m_paths[2].push_back(std::move(roadPos + thor::rotatedVector(l_pointToAdd, roadRot)));
		}
	}

	//m_paths[0].push_back(roadPos);
}

void AiSystem::receive(const entityx::ComponentRemovedEvent<Ai>& e)
{
	if (m_aiIDs.size() <= 0)
	{
		const auto& begin = m_aiIDs.begin();
		const auto& end = m_aiIDs.end();
		for (auto itt = begin; itt != end; itt++)
		{
			if ((*itt) == e.entity.id())
			{
				m_aiIDs.erase(itt);
				break;
			}
		}
	}
}
/// <summary>
/// @brief clear path points when road is removed
/// </summary>
/// <param name="e"></param>
void AiSystem::receive(const entityx::ComponentRemovedEvent<Road>& e)
{
	entityx::Entity roadEntity = e.entity;

	if (m_paths.size() <= 0)
	{
		for (auto& path : m_paths)
		{
			if (path.size() <= 0)
			{
				const auto& begin = path.begin();
				const auto& end = path.end();
				for (auto itt = begin; itt != end; itt++)
				{
					if ((*itt) == roadEntity.component<Position>()->m_position)
					{
						path.erase(itt);
						break;
					}
				}

			}
		}
	}
}


/// <summary>
/// @brief follow a set of points which is held in a vector
/// 
/// controll the ai car which of the point to follow next 
/// </summary>
/// <param name="aiPos">Ai 2D position</param>
/// <param name="pathNode">Ai's current set path</param>
/// <param name="node">THIS IS A POINT THAT THE AI GOES TO</param>
/// <returns></returns>
sf::Vector2f AiSystem::followPath(const sf::Vector2f & aiPos, const int& pathNode, int& node)
{
	const auto& path = m_paths.at(pathNode-1);
	const sf::Vector2f& target = path.at(node);
	
	if (Math::distanceSq(aiPos, target) <= (m_NODE_TRIGGER_DIST * m_NODE_TRIGGER_DIST))
	{
		//increment the point if the ai is in a range of the current point
		node += 1;

		//let the ai restart its path when it reaches the last point
		if (node >= path.size())
		{
			node = 0;
		}
	}
	return target - aiPos;
}

const sf::CircleShape AiSystem::mostThreatening(entityx::Entity::Id id, entityx::EntityManager & entities)
{
	entityx::Entity aiCar = entities.get(id);
	
	Position::Handle position = aiCar.component<Position>();
	sf::CircleShape mostThreatening(0);
	mostThreatening.setRadius(0);
	bool collision = false;
	
	for (int i = 0; i < m_obstacles.size(); i++)
	{
		if (id != m_aiIDs.at(i))
		{

			collision = Math::lineIntersectsCircle(m_ahead, m_halfAhead, m_obstacles[i]);

			if (collision && (mostThreatening.getRadius() == 0 || Math::distance(position->m_position, m_obstacles[i].getPosition()) < Math::distance(position->m_position, mostThreatening.getPosition())))
			{
				mostThreatening = m_obstacles[i];
			}
		}
	}

	return mostThreatening;
}

sf::Vector2f AiSystem::collisionAvoidance(entityx::Entity::Id id, entityx::EntityManager & entities)
{
	entityx::Entity aiCar = entities.get(id);
	Position::Handle aiPos = aiCar.component<Position>();


	auto headingRadians = thor::toRadian(aiPos->m_rotation);
	sf::Vector2f headingVector(std::cos(headingRadians) * MAX_SEE_AHEAD, std::sin(headingRadians) * MAX_SEE_AHEAD);
	m_ahead = aiPos->m_position + headingVector;
	m_halfAhead = aiPos->m_position + (headingVector * 0.5f);
	const sf::CircleShape object = mostThreatening(id, entities);
	sf::Vector2f avoidance(0, 0);
	if (object.getRadius() != 0.0)
	{
		auto threatPos = object.getPosition();
		auto mypos = aiPos->m_position;
		avoidance.x = m_ahead.x - object.getPosition().x;
		avoidance.y = m_ahead.y - object.getPosition().y;
		avoidance = thor::unitVector(avoidance);
		avoidance *= MAX_AVOID_FORCE;
	}
	else
	{
		avoidance *= 0.0f;
	}
	return avoidance;
}






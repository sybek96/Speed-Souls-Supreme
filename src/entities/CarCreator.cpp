
#include "entities\CarCreator.h"

/// <summary>
/// @brief Stores necessary data for Car entity.
/// 
/// 
/// </summary>
/// <param name="isPlayer">defines if car is the player</param>
/// <param name="position">defines car's 2D position</param>
/// <param name="rotation">defines car's heading, in degrees</param>
/// <param name="coord">defines car's texture coordinate</param>
/// <param name="origin">defines car's texture origin</param>
/// <param name="color">defines car's color filter</param>
CarCreator::CarCreator(
	const bool& isPlayer
	, const sf::Vector2f & position
	, const float & rotation
	, const sf::IntRect & texel
	, const sf::Vector2f origin
	, const sf::Color & color
	, const Assets::Textures::Car& type)

	: m_position(position)
	, m_rotation(rotation)
	, m_texel(texel)
	, m_origin(origin)
	, m_color(color)
	, m_type(type)
	, m_isPlayer(isPlayer)
{
}

/// <summary>
/// @brief Assign's components to entity.
/// 
/// Components are assigned to passed entity using the member variables
/// </summary>
/// <param name="entity">entity which components are assigned</param>
void CarCreator::create(entityx::Entity & entity)
{
	entity.assign<Position>(m_position, m_rotation + 180);
	entity.assign<Display>(m_texel, m_color, m_origin);
	entity.assign<Car>(m_type);
	entity.assign<CollisionBox>(sf::IntRect(0, 0, 135, 330));
	entity.assign<Acceleration>();
	entity.assign<Velocity>();
	if (m_isPlayer)
	{
		entity.assign<InputControl>();
		entity.assign<Player>();
	}
	else
	{
		entity.assign<Ai>();
	} 
		
}

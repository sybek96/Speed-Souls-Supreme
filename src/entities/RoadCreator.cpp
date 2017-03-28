#include "entities\RoadCreator.h"



RoadCreator::RoadCreator(
	const sf::Vector2f & position
	, const float & rotation
	, const sf::IntRect & texel
	, const sf::Color & color
	, const sf::Vector2f& origin
	, const Assets::Textures::Track& type
	, const int& roadNum)

	: m_position(position)
	, m_heading(rotation)
	, m_texel(texel)
	, m_color(color)
	, m_origin(origin)
	, m_type(type)
	, m_roadNum(roadNum)
{
}

void RoadCreator::create(entityx::Entity & entity)
{
	entity.assign<Position>(m_position, m_heading);
	entity.assign<Display>(m_texel, m_color, m_origin);
	entity.assign<Road>(m_type, m_roadNum);
}


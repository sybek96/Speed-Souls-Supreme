#ifndef TRACK_CREATOR_H
#define TRACK_CREATOR_H

#include "EntityCreator.h"

#include <map>

#include "SFML\System\Vector2.hpp"
#include "SFML\Graphics\Rect.hpp"

#include "components\Position.h"
#include "components\Display.h"
#include "components\Road.h"

#include "util\ResourceTypes.h"

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Creates a road entity and assigns it components.
/// 
/// 
/// </summary>
class RoadCreator : public EntityCreator
{
public:
	RoadCreator(
		const sf::Vector2f& position
		, const float& rotation
		, const sf::IntRect& texel
		, const sf::Color& color
		, const sf::Vector2f& origin
		, const Assets::Textures::Track& type
		, const int& roadNum);

	void create(entityx::Entity& entity) override;

private:
	sf::Vector2f m_position;
	float m_heading;
	sf::IntRect m_texel;
	sf::Color m_color;
	sf::Vector2f m_origin;
	Assets::Textures::Track m_type;
	int m_roadNum;
};

#endif // !TRACK_CREATOR_H
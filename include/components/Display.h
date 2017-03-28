#pragma once

#include "entityx\Entity.h"
#include "SFML\Graphics\Rect.hpp"
#include "SFML\Graphics\Color.hpp"
#include "util\ResourceTypes.h"

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Component representing a texture rectangle and a color filter.
/// 
/// 
/// </summary>
struct Display : public entityx::Component<Display>
{
	// default constructor,
	//  with defaulted parameters
	Display(
		const sf::IntRect& newCoord = sf::IntRect(0, 0, 0, 0)
		, const sf::Color& newColor = sf::Color(255u, 255u, 255u, 255u));

	Display(
		const sf::IntRect& newCoord
		, const sf::Color& newColor
		, const sf::Vector2f& newOrigin
	);

	// texture coordinate rectangle
	sf::IntRect m_coord;

	// texture origin
	sf::Vector2f m_origin;

	// color filter
	sf::Color m_color;

};


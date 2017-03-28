#pragma once

#include "entityx\Entity.h"
#include "SFML\System\Vector2.hpp"

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Component representing a 2D position and a orientation.
/// 
/// Creates 
/// </summary>
struct Position : public entityx::Component<Position>
{
	// default constructor,
	//  with defaulted parameters
	Position(
		const sf::Vector2f& newPosition = sf::Vector2f(0.0f, 0.0f)
		, const float& newRotation = 0.0f);

	// 2D position
	sf::Vector2f m_position;

	// rotation in degrees
	float m_rotation;
};
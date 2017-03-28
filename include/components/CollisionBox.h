#pragma once
#include "entityx\Entity.h"
#include "SFML\Graphics\Rect.hpp"


/// <summary>
/// @author Bartosz Zych
/// @version 1.0
/// @brief Component representing a rectangle collision box of an entity.
/// 
/// 
/// </summary>
struct CollisionBox : public entityx::Component<CollisionBox>
{
	/*default constructor*/
	CollisionBox(const sf::IntRect& newRect = sf::IntRect(0, 0, 0, 0));

	/*CollisonBox rectangle*/
	sf::IntRect m_rectangle;
};


#ifndef VELOCITY_H
#define VELOCITY_H

#include "entityx\Entity.h"
#include "SFML\Graphics.hpp"

/// <summary>
/// @author Sebastian Kruzel
/// @version 1.0
/// @brief Component representing the velocity of an entity
/// 
/// 
/// </summary>
struct Velocity : public entityx::Component<Velocity>
{
	//Default constructor with defaulted arguments
	Velocity(sf::Vector2f velocity = sf::Vector2f(0.0f,0.0f));

	//velocity of the entity
	sf::Vector2f m_velocity;
};

#endif // !VELOCITY_H

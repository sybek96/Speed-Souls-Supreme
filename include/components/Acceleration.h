#ifndef ACCELERATION_H
#define ACCELERATION_H

#include "entityx\Entity.h"
#include "SFML\Graphics.hpp"

/// <summary>
/// @author Sebastian Kruzel
/// @version 1.0
/// @brief Component representing the acceleration of an entity
/// 
/// 
/// </summary>
struct Acceleration : public entityx::Component<Acceleration>
{
public:
	//Default constructor with defaulted parameters

	Acceleration(sf::Vector2f acceleration = sf::Vector2f(0.0f,0.0f), float accelVal = 0.0f);


	//Acceleration vector
	sf::Vector2f m_acceleration;

	//Allows acceleration to be positive or negative
	float m_accelerateVal;
};

#endif // !ACCELERATION_H
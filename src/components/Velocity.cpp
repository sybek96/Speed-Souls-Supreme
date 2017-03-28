#include "components\Velocity.h"


/// <summary>
/// @brief Default constructor.
/// 
/// Creates a velocity component using parameters (can be defaulted)
/// </summary>
/// <param name="velocity">the velocity of the entity</param>
Velocity::Velocity(sf::Vector2f velocity)
	: m_velocity(velocity)
{
}



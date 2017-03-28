#include "util\Physics.h"

float Physics::m_gravity = 9.8f;
float Physics::m_coefficientOfFriction = 0.2f;
float Physics::m_frictionOffRoad = 0.5f;

/// <summary>
/// @brief set gravity
/// 
/// 
/// </summary>
/// <param name="newGrav">new gravity</param>
void Physics::setGravity(const float & newGrav)
{
	m_gravity = newGrav;
}

/// <summary>
/// @brief set coefficient of friction
/// 
/// 
/// </summary>
/// <param name="newFriction">new friction</param>
void Physics::setCoefficientOfFriction(const float & newFriction)
{
	m_coefficientOfFriction = newFriction;
}

/// <summary>
/// @brief gets gravity
/// 
/// 
/// </summary>
/// <returns>gravity</returns>
float Physics::getGravity()
{
	return m_gravity;
}

/// <summary>
/// @brief gets coefficient of friction
/// 
/// 
/// </summary>
/// <returns>coefficient of friction</returns>
float Physics::getCoefficientOfFriction()
{
	return m_coefficientOfFriction;
}

/// <summary>
/// @brief returns the friction offroad
/// 
/// 
/// </summary>
/// <returns>coefficient offroad</returns>
float Physics::getFrictionOffroad()
{
	return m_frictionOffRoad;
}




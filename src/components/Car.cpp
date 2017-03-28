#include "components\Car.h"

/// <summary>
/// @brief Default constructor.
/// 
/// Creates a Car component using parameters (can be defaulted)
/// </summary>
/// <param name="type">defines type of car</param>
Car::Car(Assets::Textures::Car type)
	: m_type(type)
	, m_roadId()
	, m_onRoad(true)
	, m_accelerateVal(0.0f)
	, m_maxAccel(1500.0f)
	, m_slowDownApplied(false)
	, m_lap(0)
{
}

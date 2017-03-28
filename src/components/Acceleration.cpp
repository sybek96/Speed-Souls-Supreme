#include "components\Acceleration.h"


/// <summary>
/// @brief Default constructor.
/// 
/// Creates an acceleration component using parameters (can be defaulted)
/// </summary>
/// <param name="magnitude">defines magnitude of acceleration</param>
Acceleration::Acceleration(sf::Vector2f acceleration, float accelVal)
	: m_acceleration(acceleration)
	, m_accelerateVal(accelVal)
{
}





#include "components\Position.h"


/// <summary>
/// @brief Default constructor.
/// 
/// Creates a Position component using parameters (can be defaulted)
/// </summary>
/// <param name="newPosition">defines a 2D position</param>
/// <param name="newRotation">defines a rotation, in degrees</param>
Position::Position(
	const sf::Vector2f & newPosition
	, const float & newRotation)

	: m_position(newPosition)
	, m_rotation(newRotation)
{
}

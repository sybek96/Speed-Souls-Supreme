#include "components\CollisionBox.h"


/// <summary>
/// @brief Default Constructor
/// 
/// 
/// Default Constructor which is creating a CollisionBox component
/// </summary>
/// <param name="newRect">defines a rectangle of a texture used in collisions</param>
CollisionBox::CollisionBox(const sf::IntRect & newRect)
	: m_rectangle(newRect)
{
}

#pragma once
#include "SFML\Graphics\RectangleShape.hpp"


#include "util\TranformedBoundingBox.h"

struct CollisionMinMax
{
	float m_min1, m_min2;
	float m_max1, m_max2;
};

/// <summary>
/// @Author Bartosz Zych
/// @Version 1.0
/// @breif check for collision between 2 objects
/// 
/// 
/// 
/// </summary>
class CollisionDetector
{
public:

	

	/// <summary>
	/// @brief return a true if collision is detected between 2 sprites
	/// 
	/// 
	/// 
	/// <param name="sprite1">first sprite to check collision for</param>
	/// <param name="sprite2">second sprite to check collisoin for</param>
	/// </summary>
	bool static collision(const sf::RectangleShape& rect1, const sf::RectangleShape& rect2, CollisionMinMax& minMax);

	/// <summary>
	/// @brief Normal rectangular collision detection.
	/// 
	/// 
	/// </summary>
	/// <param name="rect1"></param>
	/// <param name="rect2"></param>
	/// <returns>returns true if rects are intersecting, else false</returns>
	bool static collision(const sf::FloatRect& rect1, const sf::FloatRect& rect2);
};


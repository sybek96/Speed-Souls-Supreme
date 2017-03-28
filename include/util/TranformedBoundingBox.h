#pragma once
#include "SFML\System\Vector2.hpp"
#include "SFML\Graphics\RectangleShape.hpp"


/// <summary>
/// @author Bartosz Zych
/// @version 1.0
/// @brief Get Transform Matrix of an object
///
/// 
/// Make an object which will get transfrom matrix and project each point onto a custom axis
/// </summary>
class TranformedBoundingBox
{
public:


	/// <summary>
	/// @brief Get 4 points of an object after transformation
	///
	/// 
	/// It will calculate 4 points of an object after it has been transfromed 
	/// it will keep the object position, rotation, scale and origin
	/// /// <param name="sprite">the object that will be transformed</param>
	/// </summary>
	TranformedBoundingBox(const sf::RectangleShape& rectangle);
	

	sf::Vector2f m_points[4];

	/// <summary>
	/// @brief project each point of the object onto a custom axis
	/// 
	/// 
	/// 
	/// </summary>
	/// <param name="axis">custom made axis</param>
	/// <param name="minValue">min value on that custom axis</param>
	/// <param name="maxValue">max value on the custom axis</param>
	void projectOntoAxis(const sf::Vector2f& axis, float& minValue, float& maxValue);


};


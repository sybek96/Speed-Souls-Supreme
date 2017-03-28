

#include "util\TranformedBoundingBox.h"



TranformedBoundingBox::TranformedBoundingBox(const sf::RectangleShape& rect)
{
	// get a transfrom matrix of the ractangle
	sf::Transform transform = rect.getTransform(); 

	float width = rect.getLocalBounds().width;		//get the width of the rectangle
	float height = rect.getLocalBounds().height;	//get the height of the renctangle

	m_points[0] = transform.transformPoint(0.f, 0.f);		//transform top left to (0, 0)  of the window
	m_points[1] = transform.transformPoint(width, 0.f);		//transform top right to (width of the sprite, 0) of the window
	m_points[2] = transform.transformPoint(0.f, height);	//transform bottom left to (0, height of the sprite) of the window
	m_points[3] = transform.transformPoint(width, height);	//transform bottom right to (width of the sprite, height of the sprite) of the window
}

void TranformedBoundingBox::projectOntoAxis(const sf::Vector2f & axis, float& minValue, float& maxValue)
{
	minValue = ((m_points[0].x * axis.x) + (m_points[0].y * axis.y)); // set min and max values to projected first point of the sprite
	maxValue = minValue;
	for (int j = 1; j < 4; j++)
	{
		float projection = ((m_points[j].x*axis.x) + (m_points[j].y*axis.y)); //project each point to the custom made axis

		//if the projected point is less than min value then set min value to projected point
		if (projection < minValue)
		{
			minValue = projection;
		}
		//if the projected point onto the custom axis is greater than max value set max value to the projected point
		if (projection > maxValue)
		{
			maxValue = projection;
		}
	}
}




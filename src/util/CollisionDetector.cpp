#include "util\CollisionDetector.h"




bool CollisionDetector::collision(const sf::RectangleShape& rect1, const sf::RectangleShape& rect2, CollisionMinMax& minMax)
{
	TranformedBoundingBox tbb_sprite1(rect1);
	TranformedBoundingBox tbb_sprite2(rect2);

	sf::Vector2f Axes[4] =   //make 2 axes for the way that rectangle is faced. we make 4 because there is 2 objects
	{
		//first object's custom axis
		sf::Vector2f(tbb_sprite1.m_points[1].x - tbb_sprite1.m_points[0].x, tbb_sprite1.m_points[1].y - tbb_sprite1.m_points[0].y),   	    //first axis  [	     						]
		sf::Vector2f(tbb_sprite1.m_points[1].x - tbb_sprite1.m_points[3].x, tbb_sprite1.m_points[1].y - tbb_sprite1.m_points[3].y),		    //second axis [  2 axes for first rectangle ]

		//second object's custom axis
		sf::Vector2f(tbb_sprite2.m_points[0].x - tbb_sprite2.m_points[2].x, tbb_sprite2.m_points[0].y - tbb_sprite2.m_points[2].y),		    //third axis   [					          ]
		sf::Vector2f(tbb_sprite2.m_points[0].x - tbb_sprite2.m_points[1].x, tbb_sprite2.m_points[0].y - tbb_sprite2.m_points[1].y)		   //fourth axis   [  2 axes for second rectangle ]
	};

	float min1 = 0.0f;
	float max1 = 0.0f;
	float min2 = 0.0f;
	float max2 = 0.0f;

	

	for (int i = 0; i < 4; i++)
	{

		tbb_sprite1.projectOntoAxis(Axes[i], min1, max1); //project first sprites points onto the custom axis
		tbb_sprite2.projectOntoAxis(Axes[i], min2, max2); //project second sprite's points onto the custom axis made above

		
		if (!((min2 <= max1) && (max2 >= min1)))
		{
			return false;
		}
	}

	sf::Vector2f originAxis = rect1.getPosition() - rect2.getPosition();
	tbb_sprite1.projectOntoAxis(originAxis, minMax.m_min1, minMax.m_max1);
	tbb_sprite2.projectOntoAxis(originAxis, minMax.m_min2, minMax.m_max2);

	return true;

}


bool CollisionDetector::collision(const sf::FloatRect & rect1, const sf::FloatRect & rect2)
{
	return rect1.intersects(rect2);
}

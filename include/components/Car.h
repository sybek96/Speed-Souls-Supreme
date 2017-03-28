#ifndef CAR_H
#define CAR_H

#include "entityx\Entity.h"

#include "util\ResourceTypes.h"

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// </summary>
struct Car : public entityx::Component<Car>
{
	Car(Assets::Textures::Car type = Assets::Textures::Car::Silver);

	Assets::Textures::Car m_type;

	// id of the road the car is currently on
	entityx::Entity::Id m_roadId;

	// whether car is on road or off road tile
	bool m_onRoad;

	float m_accelerateVal;
	float m_maxAccel;
	bool m_slowDownApplied;
	int m_lap;
};

#endif // !CAR_H
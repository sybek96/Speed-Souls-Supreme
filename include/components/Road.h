#ifndef ROAD_H
#define ROAD_H

#include "entityx\Entity.h"

#include "util\ResourceTypes.h"

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Road components.
/// 
/// Contains the type of road
/// </summary>
struct Road : public entityx::Component<Road>
{
	Road(
		const Assets::Textures::Track& type = Assets::Textures::Track::Straight
		, const int& roadN = 0
	);

	Assets::Textures::Track m_type;

	int m_roadNum;
};


#endif // !ROAD_H
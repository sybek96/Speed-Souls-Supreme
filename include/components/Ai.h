#ifndef AI_H
#define AI_H

#include "entityx\Entity.h"
#include "SFML\System\Vector2.hpp"


/// <summary>
/// @author Bartosz Zych
/// @version 1.0
/// @brief Component representing an entity that is an Ai
/// 
/// 
/// </summary>
struct Ai : public entityx::Component<Ai>
{
	Ai();

	// The randomly choosen path of the AI
	int m_nodePath;

	// steering vector needed for AI logic
	sf::Vector2f m_steering;

	// current node that AI is steering towards
	int m_nodeNum;
};

#endif // !AI_H

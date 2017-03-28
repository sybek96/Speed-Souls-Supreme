#ifndef ENTITY_CREATOR_H
#define ENTITY_CREATOR_H

#include "entityx\Entity.h"

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Abstract entity creator class.
/// 
/// 
/// </summary>
class EntityCreator
{
public:

	virtual ~EntityCreator() {}

	virtual void create(entityx::Entity& entity) = 0;
};

#endif // !ENTITY_CREATOR_H
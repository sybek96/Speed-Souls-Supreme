#pragma once

#pragma once

#include "entityx\Event.h"
#include "entityx\Entity.h"

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Erases a particle.
/// 
/// Event causes Particle system to erase the particle whoose Id it matches.
/// </summary>
struct EvEraseParticle : public entityx::Event<EvEraseParticle>
{
	/// <summary>
	/// @brief Default event constructor.
	/// 
	/// Erased particle passed in
	/// </summary>
	/// <param name="id">defines the id of the particle to be erased</param>
	EvEraseParticle(const entityx::Entity::Id& id)
		: m_Id(id)
	{}

	entityx::Entity::Id m_Id;
};
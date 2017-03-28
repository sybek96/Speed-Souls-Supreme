#pragma once


#include "entityx\Event.h"

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Emitted when a lap is completed.
/// 
/// 
/// </summary>
struct EvLapCompleted : public entityx::Event<EvLapCompleted>
{
	EvLapCompleted(entityx::Entity::Id carId)
		: m_carId(carId)
	{
	}

	entityx::Entity::Id m_carId;

};
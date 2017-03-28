#pragma once

#include "entityx\Event.h"

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief This event is called when user backtracks a lap.
/// 
/// 
/// </summary>
struct EvLapUndone : public entityx::Event<EvLapUndone>
{
	EvLapUndone(entityx::Entity::Id carId)
		: m_carId(carId)
	{}

	entityx::Entity::Id m_carId;
};
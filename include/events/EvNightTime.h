#pragma once

#include "entityx\Event.h"


/// <summary>
/// @brief Day time event
/// 
/// Event is called from Render system,
/// emitted when it is changed to nighttime
/// </summary>
struct EvNightTime : public entityx::Event<EvNightTime>
{
	EvNightTime()
	{}
};
#pragma once

#include "entityx\Event.h"

/// <summary>
/// @author Bartosz Zych
/// @version 1.0
/// @brief This event is called when a race is started
/// 
/// 
/// </summary>
struct EvStartRace : public entityx::Event<EvStartRace>
{
	EvStartRace()
	{}
};
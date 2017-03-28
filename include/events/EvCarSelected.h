#pragma once

#include "entityx\Event.h"

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Event called when a car is selected.
/// 
/// 
/// </summary>
struct EvCarSelected : public entityx::Event<EvCarSelected>
{
	EvCarSelected(const int& index)
		: m_index(index)
	{}

	int m_index;
};
#pragma once

#include "entityx\Event.h"

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Event called when a track is selected.
/// 
/// 
/// </summary>
struct EvTrackSelected : public entityx::Event<EvTrackSelected>
{
	EvTrackSelected(const int& index)
		: m_index(index)
	{}

	int m_index;
};
#pragma once

#include "entityx\Event.h"

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Event called when a upgrade is purchased.
/// 
/// 
/// </summary>
struct EvUpgradeEngine : public entityx::Event<EvUpgradeEngine>
{
	EvUpgradeEngine(const int& tier, const int& cost)
		: m_tier(tier)
		, m_cost(cost)
	{}

	int m_tier;
	int m_cost;
};
#pragma once

#include "entityx\Entity.h"

/// <summary>
/// @author Bartosz Zych
/// @version 1.0
/// @brief I did this with no help.
/// 
/// 
/// </summary>
struct Light : public entityx::Component<Light>
{
	Light(const bool& on = false);

	bool m_on;
};


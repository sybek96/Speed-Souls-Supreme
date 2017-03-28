
#pragma once

#include "entityx\Entity.h"

#include "SFML\Graphics.hpp"

/// <summary>
/// @author Huong Thi Thanh Huynh
/// @version 1.0
/// @brief Component representing a FloatRect
/// 
/// </summary>
struct Video : public entityx::Component<Video>
{
	Video();

	sf::FloatRect m_frame;
};

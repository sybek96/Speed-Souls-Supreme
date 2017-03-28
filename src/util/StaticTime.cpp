#include "util\StaticTime.h"

sf::Time StaticTime::s_time = sf::Time::Zero;
sf::Time StaticTime::s_MSSinceLastUpdate = sf::Time::Zero;
sf::Clock StaticTime::s_clock;

/// <summary>
/// @brief
/// 
/// 
/// </summary>
void StaticTime::start()
{
	s_clock.restart();
}

void StaticTime::reset()
{
	s_time = sf::Time::Zero;
}

/// <summary>
/// @brief update the timers
/// 
/// Update both time and time since last update variables
/// </summary>
/// <param name="dt">time since last update, in milliseconds</param>
void StaticTime::update(sf::Int32 dt)
{
	s_time += sf::milliseconds(dt);
	if (s_time.asSeconds() < 0.0f)
	{
		s_time = sf::Time::Zero;
	}
}


#ifndef STATIC_TIME_H
#define STATIC_TIME_H
#include "SFML\Graphics.hpp"

/// <summary>
/// @author Sebastian Kruzel
/// @version 1.0
/// @brief a static class that updates time
/// 
/// Separate from the update of the game
/// this class is used to see how much time passed 
/// since last update
/// </summary>
class StaticTime
{
public:
	static void start();
	static void reset();
	static void update(sf::Int32);
	static sf::Clock s_clock;
	static sf::Time s_time;
	static sf::Time s_MSSinceLastUpdate;
};

#endif // !STATIC_TIME_H

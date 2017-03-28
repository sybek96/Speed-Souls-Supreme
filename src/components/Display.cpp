#include "components\Display.h"


/// <summary>
/// @brief Default constructor.
/// 
/// Creates a Display component using parameters (can be defaulted)
/// </summary>
/// <param name="newCoord">defines a coordinate texture rectangle</param>
/// <param name="newColor">defines a color filter</param>
Display::Display(
	const sf::IntRect & newCoord
	, const sf::Color & newColor)

	: m_coord(newCoord)
	, m_color(newColor)
	, m_origin(newCoord.width / 2.0f, newCoord.height / 2.0f)
{
}

Display::Display(
	const sf::IntRect & newCoord
	, const sf::Color & newColor
	, const sf::Vector2f & newOrigin)

	: m_coord(newCoord)
	, m_color(newColor)
	, m_origin(newOrigin)
{
}

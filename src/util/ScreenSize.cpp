#include "util\ScreenSize.h"

int ScreenSize::s_width;
int ScreenSize::s_height;

/// <summary>
/// @brief sets screen width.
/// 
/// 
/// </summary>
/// <param name="newWidth"></param>
void ScreenSize::setWidth(const int & newWidth)
{
	s_width = newWidth;
}

/// <summary>
/// @brief sets screen height.
/// 
/// 
/// </summary>
/// <param name="newHeight"></param>
void ScreenSize::setHeight(const int & newHeight)
{
	s_height = newHeight;
}

/// <summary>
/// @brief gets screen width.
/// 
/// 
/// </summary>
/// <returns></returns>
int ScreenSize::width()
{
	return s_width;
}

/// <summary>
/// @brief gets screen height
/// </summary>
/// <returns></returns>
int ScreenSize::height()
{
	return s_height;
}

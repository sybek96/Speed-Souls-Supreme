#ifndef SCREEN_SIZE_H
#define SCREEN_SIZE_H

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Purely static class that contains Screen size.
/// 
/// 
/// </summary>
class ScreenSize
{
public:

	static void setWidth(const int& newWidth);

	static void setHeight(const int& newHeight);

	static int width();

	static int height();

private:

	static int s_width;

	static int s_height;

};

#endif // !SCREEN_SIZE_H
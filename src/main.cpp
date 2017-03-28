#ifdef __DEBUG
#pragma comment(lib,"sfml-graphics-d.lib")
#pragma comment(lib,"sfml-audio-d.lib")
#pragma comment(lib,"sfml-system-d.lib")
#pragma comment(lib,"sfml-window-d.lib")
#pragma comment(lib,"sfml-network-d.lib")
#pragma comment(lib,"sfeMovie-d.lib")
#pragma comment(lib,"entityx-d.lib")
#pragma comment(lib,"libyaml-cppmdd")
#pragma comment(lib,"thor-d.lib")
#else
#pragma comment(lib,"sfml-graphics.lib")
#pragma comment(lib,"sfml-audio.lib")
#pragma comment(lib,"sfml-system.lib")
#pragma comment(lib,"sfml-window.lib")
#pragma comment(lib,"sfml-network.lib")
#pragma comment(lib,"sfeMovie.lib")
#pragma comment(lib,"libyaml-cppmd")
#pragma comment(lib,"entityx.lib")
#pragma comment(lib,"thor.lib")
#endif
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

#include "Game.h"

/// <summary>
/// @brief Main application entry point.
/// 
/// 
/// </summary>
/// <param name="argc">number of argument parameters</param>
/// <param name="argv">c-style string array of executable parameters</param>
/// <returns>exit code, 0 for success</returns>
int main(int argc, char* agrv[])
{
	Game& game = Game();
	game.init();
	game.run();
	return EXIT_SUCCESS;
}
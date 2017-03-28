#ifndef GAME_STATE_H
#define GAME_STATE_H

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Different Game modes.
/// 
/// 
/// </summary>
enum class GameState
{
	None,
	LicenseScreen,
	SplashScreen,
	MainMenu,
	OptionsScreen,
	CarSelection,
	TrackSelection,
	Upgrade,
	HelpScreen,
	Credits,
	Playing,
	Paused,
	Exit
};

#endif // !GAME_STATE_H
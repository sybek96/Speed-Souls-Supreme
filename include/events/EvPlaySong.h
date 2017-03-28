#pragma once

#include "entityx\Event.h"

#include "util\ResourceTypes.h"


/// <summary>
/// @author Bartosz Zych
/// @version 1.0
/// @brief event for start playing a soing.
/// 
/// this event will be emmited when a song need to be played
/// </summary>
struct EvPlaySong : public entityx::Event<EvPlaySong>
{
	/// <summary>
	/// @brief event for playing a song
	/// 
	/// 
	/// <param name="song">song that will be played</param>
	/// </summary>
	EvPlaySong(Assets::Sounds::Songs song = Assets::Sounds::Songs::None)
		: m_song(song)
	{}

	Assets::Sounds::Songs m_song;
};

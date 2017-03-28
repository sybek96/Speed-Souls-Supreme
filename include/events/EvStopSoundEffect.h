#pragma once

#include "entityx\Event.h"

#include "util\ResourceTypes.h"


/// <summary>
/// @author Sebastian Kruzel
/// @version 1.0
/// @brief event for stop playing sound effect.
/// 
/// this event will be emmited when a sound needs to be stopped
/// </summary>
struct EvStopSoundEffect : public entityx::Event<EvStopSoundEffect>
{
	/// <summary>
	/// @brief event for playing a song
	/// 
	/// 
	/// <param name="song">song that will be played</param>
	/// </summary>
	EvStopSoundEffect(Assets::Sounds::SoundEffects sound = Assets::Sounds::SoundEffects::None)
		: m_sound(sound)
	{}

	Assets::Sounds::SoundEffects m_sound;
};
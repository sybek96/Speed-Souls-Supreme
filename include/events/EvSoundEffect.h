#pragma once
#include "entityx\Event.h"

#include "util\ResourceTypes.h"

/// <summary>
/// @author Bartosz Zych
/// @version 1.0
/// @brief event for playing a sound.
/// 
/// this event is called when a sound effect needs to be played
/// </summary>
struct EvSoundEffect : public entityx::Event<EvSoundEffect>
{

	/// <summary>
	/// @brief event for playing a sound effect
	/// 
	/// 
	/// <param name="soundEffect">which sound to play</param>
	/// </summary>
	EvSoundEffect(Assets::Sounds::SoundEffects soundEffect = Assets::Sounds::SoundEffects::None)
		: m_soundEffect(soundEffect)
	{}

	Assets::Sounds::SoundEffects m_soundEffect;
};
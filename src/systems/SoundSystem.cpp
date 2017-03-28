#include "systems\SoundSystem.h"



SoundSystem::SoundSystem(std::shared_ptr<ResourceHolders> resources)
	: m_resources(resources)
{
	m_soundEffect.push_back(sf::Sound());
	m_soundEffect.push_back(sf::Sound());
	m_soundEffect.push_back(sf::Sound());
	m_soundEffect.push_back(sf::Sound());
}

void SoundSystem::configure(entityx::EventManager & eventManager)
{
	eventManager.subscribe<EvAssetsLoaded>(*this);
	eventManager.subscribe<EvSoundEffect>(*this);
	eventManager.subscribe<EvPlaySong>(*this);
	eventManager.subscribe<EvStopSoundEffect>(*this);
}

void SoundSystem::receive(const EvAssetsLoaded & e)
{
	typedef Assets::Sounds::SoundEffects SoundEffects;
	const auto& soundEffects = *(m_resources->m_sounds->m_soundEffects);

	auto soundType = SoundEffects::ButtonClick;
	auto soundEffectNum = static_cast<int>(soundType);
	m_soundEffect[soundEffectNum].setBuffer(soundEffects[soundType]);

	soundType = SoundEffects::Engine;
	soundEffectNum = static_cast<int>(soundType);
	m_soundEffect[soundEffectNum].setBuffer(soundEffects[soundType]);

	soundType = SoundEffects::Handbrake;
	soundEffectNum = static_cast<int>(soundType);
	m_soundEffect[soundEffectNum].setBuffer(soundEffects[soundType]);

	soundType = SoundEffects::SliderMove;
	soundEffectNum = static_cast<int>(soundType);
	m_soundEffect[soundEffectNum].setBuffer(soundEffects[soundType]);

	typedef Assets::Sounds::Songs Songs;
	const auto& songs = *(m_resources->m_sounds->m_songs);
}

void SoundSystem::receive(const EvSoundEffect & e)
{
	const auto& soundNum = static_cast<int>(e.m_soundEffect);
	auto& soundEffect = m_soundEffect[soundNum];

	if (soundEffect.getStatus() != sf::Sound::Status::Playing)
	{
		soundEffect.play();
	}

}

void SoundSystem::receive(const EvPlaySong & e)
{
	const auto& song = e.m_song;
	auto& songs = *(m_resources->m_sounds->m_songs);
	if (m_song && m_song->getStatus() == sf::Music::Status::Playing)
	{
		m_song->stop();
	}

	m_song = songs[song];
	m_song->play();
}

void SoundSystem::receive(const EvStopSong & e)
{
	const auto& song = e.m_song;
	auto& songs = *(m_resources->m_sounds->m_songs);
	if (m_song && m_song->getStatus() == sf::Music::Status::Playing)
	{
		m_song->stop();
	}
}

void SoundSystem::receive(const EvStopSoundEffect & e)
{
	const auto& soundNum = static_cast<int>(e.m_sound);
	auto& soundEffect = m_soundEffect[soundNum];

	if (soundEffect.getStatus() == sf::Sound::Status::Playing)
	{
		soundEffect.stop();
	}
}

void SoundSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, double dt)
{
	return;
}



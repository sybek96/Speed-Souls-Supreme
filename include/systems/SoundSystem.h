#pragma once
#include "entityx\System.h"

#include "events\EvAssetsLoaded.h"
#include "events\EvPlaySong.h"
#include "events\EvSoundEffect.h"
#include "events\EvStopSong.h"
#include "events\EvStopSoundEffect.h"

#include "util\ResourceTypes.h"


/// <summary>
/// @author Bartosz Zych
/// @version 1.0
/// @brief Systems is used to play sound effects and songs
/// 
/// 
/// 
/// </summary>
class SoundSystem
	: public entityx::System<SoundSystem>
	, public entityx::Receiver<SoundSystem>
{
public:
	SoundSystem(std::shared_ptr<ResourceHolders> resources);

	void configure(entityx::EventManager& eventManager) override;

	void receive(const EvAssetsLoaded& e);
	void receive(const EvSoundEffect& e);
	void receive(const EvPlaySong& e);
	void receive(const EvStopSong& e);
	void receive(const EvStopSoundEffect& e);

	void update(entityx::EntityManager& entities, entityx::EventManager& events, double dt) override;

	std::shared_ptr<ResourceHolders> m_resources;
	std::vector<sf::Sound> m_soundEffect;
	std::shared_ptr<sf::Music> m_song;
	
};


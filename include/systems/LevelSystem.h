#ifndef LEVEL_SYSTEM_H
#define LEVEL_SYSTEM_H

#include "entityx\System.h"

#include "yaml-cpp\yaml.h"

#include "SFML\Audio\SoundBuffer.hpp"
#include "SFML\Graphics\Texture.hpp"
#include "SFML\Graphics\Font.hpp"

#include "Thor\Resources.hpp"
#include "Thor\Math.hpp"
#include "Thor\Vectors\VectorAlgebra2D.hpp"

#include "gui\GUI.h"

#include "events\EvChangeGameState.h"
#include "events\EvAssetsLoaded.h"
#include "events\EvTrackSelected.h"
#include "events\EvCarSelected.h"
#include "events\EvUpgradeEngine.h"
#include "events\EvUpgradeHandling.h"
#include "events\EvUpgradeWheels.h"

#include "entities\CarCreator.h"
#include "entities\RoadCreator.h"

#include "util\Debug.h"
#include "util\ResourcePath.h"
#include "util\ResourceTypes.h"
#include "util\CarLoader.h"
#include "util\TrackLoader.h"
#include "util\MenuLoader.h"
#include "util\PlayerYaml.h"
#include "util\RoadCount.h"

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Level loading and saving system.
/// 
/// 
/// </summary>
class LevelSystem
	: public entityx::System<LevelSystem>
	, public entityx::Receiver<LevelSystem>
{
private:
	typedef sf::Texture Texture;
	typedef thor::ResourceLoader<Texture> LoaderTexture;

	typedef sf::Font Font;
	typedef thor::ResourceLoader<Font> LoaderFont;

	typedef sf::SoundBuffer Sound;
	typedef thor::ResourceLoader<Sound> LoaderSound;

public:
	// Default constructor
	LevelSystem(
		entityx::EntityManager& entityManager
		, entityx::EventManager& eventManager
		, const std::shared_ptr<ResourceHolders>& resourceHolders);

	// Event subscription function
	void configure(entityx::EventManager& eventManager) override;

	// Overriden update function
	// won't be ran
	void update(
		entityx::EntityManager& entities
		, entityx::EventManager& events
		, double dt) override;

	// will load assets based on new state
	void receive(const EvChangeGameState& e);

	// will save track selected
	void receive(const EvTrackSelected& e);

	// will save car selected
	void receive(const EvCarSelected& e);

	// will check if its possible to upgrade

	void receive(const EvUpgradeEngine& e);
	
	void receive(const EvUpgradeHandling& e);

	void receive(const EvUpgradeWheel& e);

private:
	void createCar(const Assets::Textures::Car&, const bool& noEntity = false);
	void createTrack(const int& trackNum = 1, const bool& noEntities = false);
	void createMenu(const GameState&);
	void loadPlayer(PlayerData& playerData);
	void createPlayer(const PlayerData& playerData);
	void createOpponents(const bool& noEntities = false);
	void loadParticles();
	void loadSounds();

	// reference to the entity manager
	entityx::EntityManager& m_entityManager;

	// reference to the event manager
	entityx::EventManager& m_eventManager;

	// holds all game assets
	std::shared_ptr<ResourceHolders> m_resources;

	// holds all car types
	std::vector<Assets::Textures::Car> m_carTypes;
};

#endif // !LEVEL_SYSTEM_H
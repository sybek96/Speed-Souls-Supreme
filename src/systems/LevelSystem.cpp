#include "systems\LevelSystem.h"

/// <summary>
/// @brief Default Level System constructor.
/// 
/// 
/// </summary>
/// <param name="entityManager">defines the entity manager that is referenced</param>
/// <param name="eventManager">defines the event manager that is referenced</param>
LevelSystem::LevelSystem(
	entityx::EntityManager & entityManager
	, entityx::EventManager & eventManager
	, const std::shared_ptr<ResourceHolders>& resourceHolders)

	: m_entityManager(entityManager)
	, m_eventManager(eventManager)
	, m_resources(resourceHolders)
{
}

/// <summary>
/// @brief Subscribes all events that this system receives.
/// 
/// 
/// </summary>
/// <param name="eventManager">reference to event manager that system subscribes to</param>
void LevelSystem::configure(entityx::EventManager & eventManager)
{
	eventManager.subscribe<EvChangeGameState>(*this);
	eventManager.subscribe<EvTrackSelected>(*this);
	eventManager.subscribe<EvCarSelected>(*this);
	eventManager.subscribe<EvUpgradeEngine>(*this);
	eventManager.subscribe<EvUpgradeHandling>(*this);
	eventManager.subscribe<EvUpgradeWheel>(*this);
}

/// <summary>
/// @brief Level system does not update every frame.
/// 
/// Can only be interacted with events
/// </summary>
/// <param name="entities">reference to entity manager</param>
/// <param name="events">reference to event manager</param>
/// <param name="dt">time per update</param>
void LevelSystem::update(
	entityx::EntityManager & entities
	, entityx::EventManager & events
	, double dt)
{
	return;
}

/// <summary>
/// @brief Event receiver function.
/// 
/// Load assets based on new game state
/// </summary>
/// <param name="e">constant reference to event</param>
void LevelSystem::receive(const EvChangeGameState & e)
{
	// Stores game state
	GameState state(e.m_gameState);
	try
	{
		loadSounds();

		// Reset static road counter
		RoadCount::reset();
		switch (state)
		{
		case GameState::LicenseScreen:
		case GameState::SplashScreen:
			// Assets handled by movie system
			//  not part of programming spec
			break;
		case GameState::Upgrade:
		{
			// Re-create vector of carTypes
			//  used to so that that every car EXCEPT the car
			//  the player chose is created as a AI car
			const auto& last = static_cast<unsigned>(Assets::Textures::Car::Last);
			for (unsigned i = 1u; i < last; i++)
			{
				const auto& type = static_cast<Assets::Textures::Car>(i);
				m_carTypes.push_back(std::move(type));
			}

			createMenu(state);
			PlayerData player;
			loadPlayer(player);
			createPlayer(player);
			break;
		}
		case GameState::CarSelection:
		{
			PlayerData player;
			loadPlayer(player);
			if (player.m_carSelected == Assets::Textures::Car::Last)
			{
				createMenu(state);
				createOpponents(true);
			}
			else
			{
				m_eventManager.emit<EvChangeGameState>(GameState::TrackSelection);
				return;
			}
			break;
		}
		case GameState::TrackSelection:
			createMenu(state);
			createTrack(1, true);
			createTrack(2, true);
			createTrack(3, true);
			break;
		case GameState::MainMenu:
		case GameState::OptionsScreen:
			createMenu(state);
			break;
		case GameState::Paused:
			break;
		case GameState::Playing:
		{
			// needed to load font for hud
			createMenu(state);

			m_carTypes.clear();
			const auto& last = static_cast<unsigned>(Assets::Textures::Car::Last);
			for (unsigned i = 1u; i < last; i++)
			{
				const auto& type = static_cast<Assets::Textures::Car>(i);
				m_carTypes.push_back(std::move(type));
			}

			PlayerData player;
			loadPlayer(player);
			createTrack(player.m_trackSelected);
			createPlayer(player);
			createOpponents();
			loadParticles();
		}
			break;
		case GameState::None:
			break;
		case GameState::Exit:
			break;
		default:
			break;
		}

	}
	catch (thor::ResourceLoadingException& e)
	{
		std::string s("Error in resource loading:\n ");
		s += " Occured in ";
		s += typeid(*this).name();
		debugMsg(s.c_str(), e.what());
	}
	catch (thor::ResourceAccessException& e)
	{
		std::string s("Error accessing:\n ");
		s += " Occured in ";
		s += typeid(*this).name();
		debugMsg(s.c_str(), e.what());
	}
	catch (const std::exception& e)
	{
		std::string s("Error:\n ");
		s += "Occured in ";
		s += typeid(*this).name();
		debugMsg(s.c_str(), e.what());
	}
	m_eventManager.emit<EvAssetsLoaded>(state);
}

/// <summary>
/// @brief Saves selected track.
/// 
/// Player yaml file is loaded up and assigned the selected track,
/// from the event struct and than it's saved back into Player yaml file
/// </summary>
/// <param name="e">defines event struct</param>
void LevelSystem::receive(const EvTrackSelected & e)
{
	PlayerData playerData;
	
	PlayerYaml::load(playerData);

	playerData.m_trackSelected = e.m_index;

	PlayerYaml::save(playerData);

}

/// <summary>
/// @brief Stores selected car.
/// 
/// Player yaml file is loaded up and assigned the selected car,
/// from the event struct and than it's saved back into the Player yaml file
/// </summary>
/// <param name="e">defines event struct</param>
void LevelSystem::receive(const EvCarSelected & e)
{
	PlayerData playerData;
	PlayerYaml::load(playerData);

	playerData.m_carSelected = static_cast<Assets::Textures::Car>(e.m_index);
	if (playerData.m_tracksCompleted < 0)
	{
		playerData.m_tracksCompleted = 0;
	}
	if (playerData.m_trackSelected < 0)
	{
		playerData.m_trackSelected = 0;
	}

	PlayerYaml::save(playerData);
}

void LevelSystem::receive(const EvUpgradeEngine & e)
{
	PlayerData playerData;

	PlayerYaml::load(playerData);

	playerData.m_stats.m_engine = e.m_tier;

	PlayerYaml::save(playerData);
}

void LevelSystem::receive(const EvUpgradeHandling & e)
{
	PlayerData playerData;

	PlayerYaml::load(playerData);

	playerData.m_stats.m_handling = e.m_tier;

	PlayerYaml::save(playerData);
}

void LevelSystem::receive(const EvUpgradeWheel & e)
{
	PlayerData playerData;

	PlayerYaml::load(playerData);

	playerData.m_stats.m_wheel = e.m_tier;

	PlayerYaml::save(playerData);
}

/// <summary>
/// @brief Helper function.
/// 
/// Creates a car entity based on type and loaded data, and loads car assets
/// </summary>
/// <param name="carType">defines the type of car created</param>
void LevelSystem::createCar(const Assets::Textures::Car& carType, const bool& noEntity)
{
	CarL::CarData carData;

	if (!CarLoader::load(static_cast<int>(carType), carData))
	{
		std::string msg("Error loading: ");
		msg += "car";
		msg += std::to_string(static_cast<int>(carType));
		msg += ".yaml file";
		throw std::exception(msg.c_str());
	}
	m_resources->m_textures->m_car->acquire(
		carType
		, thor::Resources::fromFile<Texture>(carData.m_texture.m_fileName)
		, thor::Resources::KnownIdStrategy::Reuse);
	
	auto& textureCar = (*(m_resources->m_textures->m_car));
	textureCar[carType].setSmooth(true);

	if (!noEntity)
	{
		CarCreator(
			false
			, carData.m_position
			, 0.0f
			, carData.m_texture.m_texels[0]
			, carData.m_texture.m_origin
			, sf::Color::White
			, carType
		).create(m_entityManager.create());
	}
}

/// <summary>
/// @brief Helper function.
/// 
/// Creates multiple road entities based on type and loaded data of a entire track
/// </summary>
void LevelSystem::createTrack(const int& trackNum, const bool& noEntities)
{
	TrackL::TrackData trackData;

	if (!TrackLoader::load(trackNum, trackData))
	{
		std::string msg("Error loading: ");
		msg += "track";
		msg += std::to_string(1);
		msg += ".yaml file";
		throw std::exception(msg.c_str());
	}

	auto& trackTexture = m_resources->m_textures->m_track;
	const auto& trackType = Assets::Textures::Tracks::Track;

	auto preview = static_cast<int>(Assets::Textures::Tracks::Preview);

	const auto& prevType = static_cast<Assets::Textures::Tracks>(preview + trackNum);

	trackTexture->acquire(
		prevType
		, thor::Resources::fromFile<Texture>(trackData.m_texture.m_prevFileName)
		, thor::Resources::KnownIdStrategy::Reuse);

	trackTexture->acquire(
		Assets::Textures::Tracks::Background
		, thor::Resources::fromFile<Texture>(trackData.m_texture.m_bgFileName)
		, thor::Resources::KnownIdStrategy::Reload);

	auto& trackBgTexture = (*trackTexture)[Assets::Textures::Tracks::Background];

	trackBgTexture.setRepeated(true);

	if (!noEntities)
	{
		trackTexture->acquire(
			trackType
			, thor::Resources::fromFile<Texture>(trackData.m_texture.m_fileName)
			, thor::Resources::KnownIdStrategy::Reuse);
		auto& textureRoad = *(trackTexture);
		textureRoad[trackType].setSmooth(true);

		const unsigned& NUM_ROAD_TILES = trackData.m_roadNodes.size();
		const sf::Color& trackFilter = sf::Color::White;

		sf::Vector2f position(0.0f, 0.0f);
		float heading = trackData.m_heading;
		float rotation = heading;

		for (unsigned i = 0u; i < NUM_ROAD_TILES; i++)
		{
			const Assets::Textures::Track& type = trackData.m_roadNodes[i].m_type;
			const sf::IntRect& rectStraight = trackData.m_texture.m_texels.at(Assets::Textures::Track::Straight);
			const sf::IntRect& rect = trackData.m_texture.m_texels.at(type);

			sf::Vector2f rectOffset(0.0f, 0.0f);
			sf::Vector2f origin((rectStraight.width / 2.0f), rect.height);
			heading = rotation;

			//position += origin;

			switch (type)
			{
			case Assets::Textures::Track::CurveLeft:
				rotation -= 90.0f;
				origin.x = rect.width - origin.x;
				rectOffset.x = -origin.x;
				rectOffset.y = -(rect.height);
				break;
			case Assets::Textures::Track::CurveRight:
				//rectOffset.x = (trackData.m_texture.m_texels.at(Assets::Track::Straight).width / 2.0f);
				rotation += 90.0f;
				rectOffset.x = ((rect.width) - origin.x) - 1.0f;
				rectOffset.y = -(rect.height);
				break;
			case Assets::Textures::Track::CheckStart:
			case Assets::Textures::Track::Straight:
				rectOffset.y = -(rectStraight.height);
				break;
			case Assets::Textures::Track::CurveLongLeft:
				break;
			case Assets::Textures::Track::CurveLongRight:
				break;
			default:
				break;
			}

			RoadCount::inc();
			RoadCreator(
				position,
				heading,
				rect,
				trackFilter,
				origin,
				type,
				static_cast<int>(i)
			).create(m_entityManager.create());


			switch (type)
			{
			case Assets::Textures::Track::CurveLeft:
				//rectOffset.x = -(rect.width / 2.0f);
				rectOffset.y += (rectStraight.width / 2.0f);
				break;
			case Assets::Textures::Track::CurveRight:
				//rectOffset.x += (rectStraight.width / 2.0f);
				rectOffset.y += ((rectStraight.width / 2.0f));
				break;
			case Assets::Textures::Track::CurveLongLeft:
				break;
			case Assets::Textures::Track::CurveLongRight:
				break;
			case Assets::Textures::Track::CheckStart:
			case Assets::Textures::Track::Straight:
				break;
			default:
				break;
			}
			position += thor::rotatedVector(rectOffset, heading);

		}

	}
}

/// <summary>
/// @brief Creates a menu.
/// 
/// Loads menu assets through the shaerd pointer
/// to our resource holder struct, which contains all our
/// thor resource holder that could be used in this game,
/// based on what game states is passed
/// </summary>
/// <param name="state">reference to menu</param>
void LevelSystem::createMenu(const GameState& state)
{
	MenuL::MenuData menuData;

	// load our menu data from our yaml file,
	//  which is determined by the game state enum
	if (!MenuLoader::load(state, menuData))
	{
		std::string msg("Error loading: ");
		msg += "menu";
		msg += " yaml file";
		msg += "\nOccured in ";
		msg += typeid(*this).name();
		throw std::exception(msg.c_str());
	}

	switch (state)
	{
	case GameState::LicenseScreen:
	case GameState::SplashScreen:
		// Plays a movie,
		//  not part of programming spec
		break;
	case GameState::MainMenu:
	case GameState::OptionsScreen:
	case GameState::CarSelection:
	case GameState::TrackSelection:
	case GameState::Upgrade:
	case GameState::HelpScreen:
	case GameState::Credits:
	{
		// store reference to our gui texture
		//  used so that no errors occur with operator precedence
		//  in pointer dereferencing of our resource holder

		auto& guiTexture = *(m_resources->m_textures->m_gui);
		auto& guiFont = *(m_resources->m_fonts->m_gui);

		// Store a special GUI enum Last
		//  as the last element of that enum
		const unsigned& WIDGET_TYPES = static_cast<unsigned>(Assets::Textures::GUI::Last);

		// define our thor Id Strategy
		//  Reuse defines that if ID already has a resource loaded for it
		//  skip it, else load up the resource
		const auto& idStrat = thor::Resources::KnownIdStrategy::Reuse;
		// iterate through our gui texture enum
		for (auto i = 0; i < WIDGET_TYPES; i++)
		{
			// reference our gui texture type from the int to gui conversion
			const auto& type = static_cast<Assets::Textures::GUI>(i);

			if (type == Assets::Textures::GUI::Background)
			{
				// load resource through thor resource manager
				//  with the reload id strategy
				guiTexture.acquire(
					type
					, thor::Resources::fromFile<Texture>(menuData.m_texture[type].m_fileName)
					, thor::Resources::KnownIdStrategy::Reload);
			}
			else
			{
				// load resource through thor resource manager
				//  with predefined id strategy
				guiTexture.acquire(
					type
					, thor::Resources::fromFile<Texture>(menuData.m_texture[type].m_fileName)
					, idStrat);
			}
		}

	}
	case GameState::Playing:
	{
		// reference our chosen font type
		const auto& fontType = Assets::Fonts::GUI::Content;

		// load resource through thor resource manager
		//  with defined id strategy
		m_resources->m_fonts->m_gui->acquire(
			fontType
			, thor::Resources::fromFile<Font>(menuData.m_font.m_fileName)
			, thor::Resources::KnownIdStrategy::Reuse);

		break;
	}
	case GameState::Paused:
	case GameState::Exit:
	case GameState::None:
	default:
		// No menu system is loaded
		break;
	}
	

}

/// <summary>
/// @brief Loads player's data without entity creation.
/// 
/// 
/// </summary>
/// <param name="playerData">reference to our player data</param>
void LevelSystem::loadPlayer(PlayerData & playerData)
{
	if (!PlayerYaml::load(playerData))
	{
		std::string msg("Error loading: ");
		msg += "player.yaml";
		throw std::exception(msg.c_str());
	}
}

/// <summary>
/// @brief Helper function.
/// 
/// Creates player entity using player data
/// </summary>
/// <param name="playerData">reference to our player data</param>
void LevelSystem::createPlayer(const PlayerData& playerData)
{
	CarL::CarData carData;
	// reference player's selected car
	const Assets::Textures::Car& carType = playerData.m_carSelected;

	// iterate through car types vector
	//  using vector iterators
	//  remove the car type that the player selected

	const auto& begin = m_carTypes.begin();
	const auto& end = m_carTypes.end();
	for (auto itt = begin; itt != end; itt++)
	{
		if ((*itt) == carType)
		{
			m_carTypes.erase(itt);
			break;
		}
	}

	// Load yaml file based on selected car type
	//  and extrapolate it into car data
	if (!CarLoader::load(static_cast<int>(carType), carData))
	{
		std::string msg("Error loading: ");
		msg += "car";
		msg += std::to_string(static_cast<int>(carType));
		msg += ".yaml file";
		throw std::exception(msg.c_str());
	}
	// load resource through thor resource manager
	//  with defined id strategy
	m_resources->m_textures->m_car->acquire(
		carType
		, thor::Resources::fromFile<Texture>(carData.m_texture.m_fileName)
		, thor::Resources::KnownIdStrategy::Reuse);

	// reference our car texture
	//  needed so that no mistakes are made with operator precedence
	auto& textureCar = (*(m_resources->m_textures->m_car));
	// set texture property smooth to true
	//  (helps with how the texture appears when zoomed out by the camera)
	textureCar[carType].setSmooth(true);

	// create our player entity through the entity manager
	//  and store a reference to it
	entityx::Entity& playerEnt = m_entityManager.create();

	// apply all our components
	//  to our player entity by assigning components to our entity
	CarCreator(
		true
		, carData.m_position
		, 0.0f
		, carData.m_texture.m_texels[0]
		, carData.m_texture.m_origin
		, sf::Color::White
		, carType
	).create(playerEnt);

	// store references to our players stats

	auto& engine = playerEnt.component<Player>()->m_engine;
	auto& handling = playerEnt.component<Player>()->m_handling;
	auto& wheel = playerEnt.component<Player>()->m_wheel;

	// set our player's stats to the stats parsed in the yaml file

	engine = playerData.m_stats.m_engine;
	handling = playerData.m_stats.m_handling;
	wheel = playerData.m_stats.m_wheel;
}

/// <summary>
/// @brief Creates ai cars.
/// 
/// Makes sure that ai car's are different from player's car
/// </summary>
/// <param name="noEntities">
/// defines whether entities should be created or not,
/// needed so that all car assets can be loaded without entities being created
/// </param>
void LevelSystem::createOpponents(const bool& noEntities)
{
	if (noEntities)
	{
		// store reference to our special last enum element
		const int& LAST = static_cast<int>(Assets::Textures::Car::Last);
		// load car assets for all cars
		for (int i = 1; i < LAST; i++)
		{
			const auto& type = static_cast<Assets::Textures::Car>(i);
			createCar(type, true);
		}
	}
	else
	{
		// create car entities and load appropriate assets
		//  for each element in our car types vector
		for (const auto& carType : m_carTypes)
		{
			createCar(carType);
		}
	}
}

/// <summary>
/// @brief Loads resources for our particles.
/// 
/// Since our particles have no associated yaml file,
/// our particles assets are loaded here
/// </summary>
void LevelSystem::loadParticles()
{
	// shortcut to thor resources namespace
	namespace res = thor::Resources;

	// defines file directory, file name and extension
	std::string filename(resourcePath());
	filename += "textures/radial_effect.png";

	// store reference to our resource holder
	//  needed to avoid any mistakes with operator precedence
	auto& particleTexture = *(m_resources->m_textures->m_particle);

	// load our texture through thor Resource holder
	//  with the defined Id strategy
	particleTexture.acquire(
		Assets::Textures::Particles::Radial
		, res::fromFile<Texture>(filename)
		, res::KnownIdStrategy::Reuse);

	filename = resourcePath() + "textures/cone_effect.png";


	particleTexture.acquire(
		Assets::Textures::Particles::Cone
		, res::fromFile<Texture>(filename)
		, res::KnownIdStrategy::Reuse);
}

/// <summary>
/// @brief Loads sounds assets.
/// 
/// 
/// </summary>
void LevelSystem::loadSounds()
{
	// -------- Loading Sound Effects -------- //

	typedef sf::SoundBuffer SoundBuffer;
	typedef Assets::Sounds::SoundEffects SoundEffects;
	namespace ref = thor::Resources;

	auto& soundEffects = *(m_resources->m_sounds->m_soundEffects);
	auto& songs = *(m_resources->m_sounds->m_songs);

	const auto& idStrat = thor::Resources::KnownIdStrategy::Reuse;

	std::string fileName = resourcePath() + "sounds/ButtonClick.ogg";
	soundEffects.acquire(
		SoundEffects::ButtonClick
		, ref::fromFile<SoundBuffer>(fileName)
		, idStrat);

	fileName = resourcePath() + "sounds/engine.ogg";
	soundEffects.acquire(
		SoundEffects::Engine
		, ref::fromFile<SoundBuffer>(fileName)
		, idStrat);

	fileName = resourcePath() + "sounds/Handbrake.ogg";
	soundEffects.acquire(
		SoundEffects::Handbrake
		, ref::fromFile<SoundBuffer>(fileName)
		, idStrat);

	fileName = resourcePath() + "sounds/slider-move.ogg";
	soundEffects.acquire(
		SoundEffects::SliderMove
		, ref::fromFile<SoundBuffer>(fileName)
		, idStrat);

	// ----------- Loading Songs ------------ //

	typedef sf::Music Song;
	typedef Assets::Sounds::Songs Songs;
	

	std::shared_ptr<Song> song;

	fileName = resourcePath() + "sounds/GreenHill.ogg";
	song = std::make_shared<Song>();
	song->openFromFile(fileName);
	songs[Songs::GreenHill] = std::move(song);

	fileName = resourcePath() + "sounds/RockyRoad.ogg";
	song = std::make_shared<Song>();
	song->openFromFile(fileName);
	songs[Songs::RockyRoad] = std::move(song);

	fileName = resourcePath() + "sounds/sandy-beach-song.ogg";
	song = std::make_shared<Song>();
	song->openFromFile(fileName);
	songs[Songs::SandyBeach] = std::move(song);

	fileName = resourcePath() + "sounds/Menu.ogg";
	song = std::make_shared<Song>();
	song->openFromFile(fileName);
	songs[Songs::MenuSong] = std::move(song);
}

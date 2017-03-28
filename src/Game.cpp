#include "Game.h"

// Updates per millisecond
const sf::Int32 Game::s_MS_PER_UPDATE = 10.0;

/// <summary>
/// @brief Game Constructor.
/// 
/// Initializes all member variables
/// </summary>
Game::Game()
	: m_window()
	, m_eventManager()
	, m_entityManager(m_eventManager)
	, m_systemManager(m_entityManager, m_eventManager)
	, m_gameManager(m_entityManager, m_eventManager, m_systemManager)
{
	
}

/// <summary>
/// @brief Game destructor.
/// 
/// Removes all member variables off the stack
/// </summary>
Game::~Game()
{
}

/// <summary>
/// @brief Game initializer.
/// 
/// 
/// </summary>
void Game::init()
{
	//create a seed
	srand(time(nullptr));

	// This all the available 
	std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();

	// Defining window dimensions
	ScreenSize::setWidth(800);
	ScreenSize::setHeight(600);

	sf::Uint32 screenWidth = static_cast<sf::Uint32>(ScreenSize::width());
	sf::Uint32 screenHeight = static_cast<sf::Uint32>(ScreenSize::height());

	// Define window settings
	sf::ContextSettings windowSettings = sf::ContextSettings();
	windowSettings.antialiasingLevel = 4u;

	m_window.create(sf::VideoMode(screenWidth, screenHeight), "Speed Souls Supreme", sf::Style::Default, windowSettings);

	m_window.setVerticalSyncEnabled(true);

	m_gameManager.init();

	// Sample code for setting own custom icon
	//sf::Image image;
	//image.loadFromFile("filename");
	//m_window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());

	createSystems();

	m_eventManager.emit<EvChangeGameState>(GameState::LicenseScreen);
}

/// <summary>
/// @brief Implements the main game loop: handle windows system events, then update and render everything.
/// 
/// The update loop runs at a different FPS than the draw loop,
/// with the priority for the update loop, thus if the PC is too slow, 
/// we keep the updates but draw less frames.
/// </summary>
void Game::run()
{
	sf::Clock clock;
	sf::Int32 lag = 0;
	sf::Time dt = sf::Time::Zero;
	while (m_window.isOpen())
	{
		dt = clock.restart();
		lag += dt.asMilliseconds();
		StaticTime::update(lag);
		processEvents();
		while (lag > s_MS_PER_UPDATE)
		{
			update();
			lag -= s_MS_PER_UPDATE;
		}
		update();
		render(static_cast<double>(dt.asMilliseconds()));
	}
}

/// <summary>
/// @brief Processes the SFML window events.
/// 
/// </summary>
void Game::processEvents()
{
	sf::Event event;

	while (m_window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			m_window.close();
			break;
		case sf::Event::KeyPressed:
			break;
		case sf::Event::KeyReleased:
			break;
		default:
			break;
		}
	}
}

/// <summary>
/// @brief top level game update logic.
/// 
/// Updates relevant systems based on current Game State
/// </summary>
void Game::update()
{

	GameState state(*m_gameManager.getGameState());

	m_systemManager.update<InputSystem>(s_MS_PER_UPDATE);

	switch (state)
	{
	case GameState::LicenseScreen:
		break;
	case GameState::SplashScreen:
		break;
	case GameState::Upgrade:
	case GameState::MainMenu:
	case GameState::OptionsScreen:
	case GameState::CarSelection:
	case GameState::TrackSelection:
		m_systemManager.update<MenuSystem>(s_MS_PER_UPDATE);
		break;
	case GameState::Playing:
		m_systemManager.update<MotionSystem>(s_MS_PER_UPDATE);
		m_systemManager.update<CollisionSystem>(s_MS_PER_UPDATE);
		m_systemManager.update<CarTrackingSystem>(s_MS_PER_UPDATE);
		m_systemManager.update<AiSystem>(s_MS_PER_UPDATE);
		m_systemManager.update<ParticleSystem>(s_MS_PER_UPDATE);
		
		break;
	case GameState::Paused:
		break;
	case GameState::Exit:
		m_window.close();
		break;
	case GameState::None:
	default:
		break;
	}
}

/// <summary>
/// @brief top level game rendering logic.
/// 
/// </summary>
/// <param name="ms">time since last render, in milliseconds</param>
void Game::render(double ms)
{
	m_window.clear(sf::Color::Black);

	GameState state(*m_gameManager.getGameState());
	
	switch (state)
	{
	case GameState::LicenseScreen:
	case GameState::SplashScreen:
	case GameState::HelpScreen:
	case GameState::Credits:
		m_systemManager.update<MovieSystem>(ms);
		break;
	case GameState::Upgrade:
	case GameState::MainMenu:
	case GameState::OptionsScreen:
	case GameState::CarSelection:
	case GameState::TrackSelection:
		m_systemManager.update<MenuRenderSystem>(ms);
		break;
	case GameState::Paused:
	case GameState::Playing:
		m_systemManager.update<RenderSystem>(ms);
		m_systemManager.update<ScrollSystem>(ms);
		m_systemManager.update<HudSystem>(ms);
		m_systemManager.update<HeadLightSystem>(ms);
		m_systemManager.update<MiniMapSystem>(ms);
		break;
	case GameState::Exit:
		break;
	case GameState::None:
	default:
		break;
	}

	m_window.display();
}

/// <summary>
/// @brief Helper function.
/// 
/// Adds all systems to the system manager,
/// and than runs the SystemManager::configure(),
/// which runs all of the added systems configure function
/// </summary>
void Game::createSystems()
{
	std::shared_ptr<sf::RectangleShape> viewRect = std::make_shared<sf::RectangleShape>();
	std::shared_ptr<sf::View> view = std::make_shared<sf::View>();

	// create a shared resource holder
	//  allocate it on the heap,
	//  share it between all systems that need resources
	std::shared_ptr<ResourceHolders> resourceHolder = std::make_shared<ResourceHolders>();
	
	// create a shared pointer to controller
	//  allocate it on the heap,
	//  share it between all systems that require the controller
	std::shared_ptr<Xbox360Controller> controller = std::make_shared<Xbox360Controller>();

	std::shared_ptr<GUI> gui = std::make_shared<GUI>(controller);

	std::shared_ptr<std::vector<sf::RectangleShape>> textRects = std::make_shared<std::vector<sf::RectangleShape>>();

	std::shared_ptr<sf::Sprite> bgSprite = std::make_shared<sf::Sprite>();

	std::shared_ptr<sf::RectangleShape> lbRect = std::make_shared<sf::RectangleShape>();
	std::shared_ptr<sf::RectangleShape> rbRect = std::make_shared<sf::RectangleShape>();

	m_systemManager.add<LevelSystem>(m_entityManager, m_eventManager, resourceHolder);
	m_systemManager.add<MenuSystem>(m_eventManager, m_entityManager, m_window, gui, controller, resourceHolder, textRects, bgSprite , lbRect , rbRect);
	m_systemManager.add<MenuRenderSystem>(gui, m_window, textRects, bgSprite, lbRect, rbRect);
	m_systemManager.add<RenderSystem>(m_window, viewRect, view, resourceHolder);
	m_systemManager.add<InputSystem>(controller);
	m_systemManager.add<MotionSystem>(m_eventManager);
	m_systemManager.add<CollisionSystem>();
	m_systemManager.add<CarTrackingSystem>(m_entityManager);
	m_systemManager.add<ScrollSystem>(m_window, viewRect, view);
	m_systemManager.add<MiniMapSystem>(m_window, resourceHolder);
	m_systemManager.add<AiSystem>();
	m_systemManager.add<MovieSystem>(m_window, m_eventManager);
	m_systemManager.add<HudSystem>(m_window, m_entityManager, resourceHolder->m_fonts, view);
	m_systemManager.add<ParticleSystem>(m_entityManager, resourceHolder);
	m_systemManager.add<SoundSystem>(resourceHolder);
	m_systemManager.add<HeadLightSystem>(m_window, resourceHolder);

	m_systemManager.configure();
}

#include "systems\MenuSystem.h"

/// <summary>
/// @brief menu system constructor.
/// 
/// 
/// </summary>
MenuSystem::MenuSystem(
	entityx::EventManager& eventManager
	, entityx::EntityManager& entityManager
	, sf::RenderWindow& window
	, std::shared_ptr<GUI> gui
	, std::shared_ptr<Xbox360Controller> controller
	, const std::shared_ptr<ResourceHolders>& resources
	, std::shared_ptr<std::vector<sf::RectangleShape>> textRects
	, std::shared_ptr<sf::Sprite> bgSprite
	, std::shared_ptr<sf::RectangleShape> lbRect
	, std::shared_ptr<sf::RectangleShape> rbRect)

	: m_eventManager(eventManager)
	, m_entityManager(entityManager)
	, m_window(window)
	, m_gameState(GameState::None)
	, m_controller(controller)
	, m_gui(gui)
	, m_resources(resources)
	, m_textureRects(textRects)
	, m_bgSprite(bgSprite)
	, m_LBRect(lbRect)
	, m_RBRect(rbRect)
{
	// make shared textures
	m_LBTexture = std::make_shared<sf::Texture>();
	m_RBTexture = std::make_shared<sf::Texture>();

	// loading the textures for lb and rb
	if (!m_LBTexture->loadFromFile("./resources/textures/lb-button.png"))
	{
		std::cout << "LB texture didn't load" << std::endl;
	}
	if (!m_RBTexture->loadFromFile("./resources/textures/rb-button.png"))
	{
		std::cout << "RB texture didn't load" << std::endl;
	}

	// set up the rectangles
	m_LBRect->setTexture(&(*m_LBTexture), true);
	m_LBRect->setSize(sf::Vector2f(m_LBTexture->getSize()));
	m_LBRect->setPosition(sf::Vector2f(0, 90));

	m_RBRect->setTexture(&(*m_RBTexture), true);
	m_RBRect->setSize(sf::Vector2f(m_RBTexture->getSize()));
	m_RBRect->setPosition(sf::Vector2f(800 - m_RBTexture->getSize().x, 90));
}

/// <summary>
/// @brief Configures system's events by subscribing to the event manager.
/// 
/// 
/// </summary>
/// <param name="eventManager">defines the event manager</param>
void MenuSystem::configure(entityx::EventManager &eventManager)
{
	eventManager.subscribe<EvChangeGameState>(*this);
	eventManager.subscribe<EvAssetsLoaded>(*this);
	eventManager.subscribe<entityx::ComponentAddedEvent<Player>>(*this);
}

/// <summary>
/// @brief receives event when game state is changed.
/// 
/// 
/// </summary>
/// <param name="e">event struct, contains new game state</param>
void MenuSystem::receive(const EvChangeGameState & e)
{
	m_window.setView(m_window.getDefaultView());
}

/// <summary>
/// @brief receives event when assets are finished loading is changed.
/// 
/// 
/// </summary>
/// <param name="e">event struct, is merely a flag</param>
void MenuSystem::receive(const EvAssetsLoaded& e)
{
	m_gameState = e.m_gameState;
	m_textureRects->clear();

	const sf::Vector2f& zero = sf::Vector2f(0.0f, 0.0f);
	
	auto& font = *(m_resources->m_fonts->m_gui);
	auto& guiTexture = *(m_resources->m_textures->m_gui);
	auto& carTexture = *(m_resources->m_textures->m_car);
	auto& trackTexture = *(m_resources->m_textures->m_track);

	auto& textureRect = (*m_textureRects);

	m_gui->clear();


	switch (e.m_gameState)
	{
	case GameState::MainMenu:
		m_eventManager.emit<EvPlaySong>(Assets::Sounds::Songs::MenuSong);

		m_bgSprite->setTexture(guiTexture[Assets::Textures::GUI::Background]);

		m_gui->addButton(
			std::bind(&MenuSystem::mainMenuBtnStartGame, this)
			, "Start game"
			, zero
			, font[Assets::Fonts::GUI::Content]
			, 25u
			, guiTexture[Assets::Textures::GUI::Button]
			, Button::s_TEXT_RECT_LEFT
			, Button::s_TEXT_RECT_MID
			, Button::s_TEXT_RECT_RIGHT);

		m_gui->addButton(
			std::bind(&MenuSystem::mainMenuOptions, this)
			, "Options"
			, zero
			, font[Assets::Fonts::GUI::Content]
			, 25u
			, guiTexture[Assets::Textures::GUI::Button]
			, Button::s_TEXT_RECT_LEFT
			, Button::s_TEXT_RECT_MID
			, Button::s_TEXT_RECT_RIGHT);

		m_gui->addButton(
			std::bind(&MenuSystem::mainMenuBtnHelp, this)
			, "Help"
			, zero
			, font[Assets::Fonts::GUI::Content]
			, 25u
			, guiTexture[Assets::Textures::GUI::Button]
			, Button::s_TEXT_RECT_LEFT
			, Button::s_TEXT_RECT_MID
			, Button::s_TEXT_RECT_RIGHT);

		m_gui->addButton(
			std::bind(&MenuSystem::mainMenuBtnCredits, this)
			, "Credits"
			, zero
			, font[Assets::Fonts::GUI::Content]
			, 25u
			, guiTexture[Assets::Textures::GUI::Button]
			, Button::s_TEXT_RECT_LEFT
			, Button::s_TEXT_RECT_MID
			, Button::s_TEXT_RECT_RIGHT);

		m_gui->addButton(
			std::bind(&MenuSystem::mainMenuBtnExitGame, this)
			, "Exit Game"
			, zero
			, font[Assets::Fonts::GUI::Content]
			, 25u
			, guiTexture[Assets::Textures::GUI::Button]
			, Button::s_TEXT_RECT_LEFT
			, Button::s_TEXT_RECT_MID
			, Button::s_TEXT_RECT_RIGHT);
		
		m_gui->configure(GUI::Layouts::StackVertically);
		m_prevGameState = GameState::MainMenu;
		break;
	case GameState::OptionsScreen:

		m_bgSprite->setTexture(guiTexture[Assets::Textures::GUI::Background]);

		m_gui->addSlider(
			font[Assets::Fonts::GUI::Content]
			, "Music Volume"
			, 25u
			, zero
			, 300
			, 50
			, 0
			, 100
			, valueMusicVolume
			, guiTexture[Assets::Textures::GUI::Slider]
			, Slider::s_TEXT_RECT_EMPTY
			, Slider::s_TEXT_RECT_FILL
			, Slider::s_TEXT_RECT_SQUARE);

		m_gui->addSlider(
			font[Assets::Fonts::GUI::Content]
			, "Sound Effects"
			, 25u
			, zero
			, 300
			, 50
			, 0
			, 100
			, valueSoundEffect
			, guiTexture[Assets::Textures::GUI::Slider]
			, Slider::s_TEXT_RECT_EMPTY
			, Slider::s_TEXT_RECT_FILL
			, Slider::s_TEXT_RECT_SQUARE);

		m_gui->addCheckbox(
			font[Assets::Fonts::GUI::Content]
			, "Music"
			, zero
			, 1
			, guiTexture[Assets::Textures::GUI::Checkbox]
			, CheckBox::s_TEXT_RECT_ON
			, CheckBox::s_TEXT_RECT_OFF
			, check
			, 25u);

		m_gui->configure(GUI::Layouts::StackVertically);

		m_prevGameState = GameState::MainMenu;

		break;
	case GameState::CarSelection:
	{
		maxTexture = 4;
		m_bgSprite->setTexture(guiTexture[Assets::Textures::GUI::Background]); // get background and set to the sprite

		m_index = 1; // reset index to 1

		m_gui->addButton(
			std::bind(&MenuSystem::carSelectionBtnSelectCar, this)
			, "Select Car"
			, sf::Vector2f(m_window.getSize().x * 0.5f, 350)
			, font[Assets::Fonts::GUI::Content]
			, 15u
			, guiTexture[Assets::Textures::GUI::Button]
			, Button::s_TEXT_RECT_LEFT
			, Button::s_TEXT_RECT_MID
			, Button::s_TEXT_RECT_RIGHT);


		const int& LAST = static_cast<int>(Assets::Textures::Car::Last);
		for (int i = 1; i < LAST; i++)
		{
			textureRect.push_back(std::move(sf::RectangleShape()));
			const auto& type = static_cast<Assets::Textures::Car>(i);
			textureRect[i - 1].setTexture(&(carTexture[type]), true);
		}

		// setting up the texture rectangles
		textureRect[0].setSize(sf::Vector2f(80, 180));
		textureRect[0].setOrigin(textureRect[0].getSize() / 2.f);
		textureRect[0].setPosition(sf::Vector2f(m_window.getSize().x * 0.25 - 50, 150));

		textureRect[1].setSize(sf::Vector2f(120, 300));
		textureRect[1].setOrigin(textureRect[1].getSize() / 2.f);
		textureRect[1].setPosition(sf::Vector2f(m_window.getSize().x * 0.5, 150));

		textureRect[2].setSize(sf::Vector2f(80, 160));
		textureRect[2].setOrigin(textureRect[2].getSize() / 2.f);
		textureRect[2].setPosition(sf::Vector2f(m_window.getSize().x * 0.75 + 50, 150));


		textureRect[3].setSize(sf::Vector2f(80, 160));
		textureRect[3].setOrigin(textureRect[3].getSize() / 2.f);
		textureRect[3].setPosition(sf::Vector2f(m_window.getSize().x + 50, 150));


		m_gui->configure(GUI::Layouts::Custom);
		m_prevGameState = GameState::MainMenu;
		break;
	}
	case GameState::TrackSelection:
	{
		maxTexture = 3;
		m_bgSprite->setTexture(guiTexture[Assets::Textures::GUI::Background]); // get background and set to the sprite

		m_index = 1; // reset index to 1

		m_gui->addButton(
			std::bind(&MenuSystem::trackSelectionStartRace, this)
			, "Start Race"
			, sf::Vector2f(m_window.getSize().x * 0.5f, 350)
			, font[Assets::Fonts::GUI::Content]
			, 25u
			, guiTexture[Assets::Textures::GUI::Button]
			, Button::s_TEXT_RECT_LEFT
			, Button::s_TEXT_RECT_MID
			, Button::s_TEXT_RECT_RIGHT);

		m_gui->addButton(
			std::bind(&MenuSystem::trackSelectionUpgradeCar, this)
			, "Upgrade Car"
			, sf::Vector2f(m_window.getSize().x * 0.5f, 450)
			, font[Assets::Fonts::GUI::Content]
			, 25u
			, guiTexture[Assets::Textures::GUI::Button]
			, Button::s_TEXT_RECT_LEFT
			, Button::s_TEXT_RECT_MID
			, Button::s_TEXT_RECT_RIGHT);

		//for (int i = 0; i < 3; i++)
		//{
		//	textureRect[i].setTexture(&m_texture[i], true);
		//}

		const auto& trackPrev = *(m_resources->m_textures->m_track);
		const auto& start = static_cast<int>(Assets::Textures::Tracks::Prev1);
		const auto& end = static_cast<int>(Assets::Textures::Tracks::Last);

		for (auto itt = start; itt < end; itt++)
		{
			textureRect.push_back(std::move(sf::RectangleShape()));
			const auto& type = static_cast<Assets::Textures::Tracks>(itt);
			textureRect[itt - start].setTexture(&(trackTexture[type]), true);
		}

		// setting up the texture rectangles
		textureRect[0].setSize(sf::Vector2f(100, 100));
		textureRect[0].setOrigin(sf::Vector2f(50, 50));
		textureRect[0].setPosition(sf::Vector2f(m_window.getSize().x * 0.25 - 50, 150));

		textureRect[1].setSize(sf::Vector2f(200, 200));
		textureRect[1].setOrigin(sf::Vector2f(100, 100));
		textureRect[1].setPosition(sf::Vector2f(m_window.getSize().x * 0.5, 150));

		textureRect[2].setSize(sf::Vector2f(100, 100));
		textureRect[2].setOrigin(sf::Vector2f(50, 50));
		textureRect[2].setPosition(sf::Vector2f(m_window.getSize().x * 0.75 + 50, 150));

		m_gui->configure(GUI::Layouts::Custom);
		m_prevGameState = GameState::MainMenu;
		break;
	}
	case GameState::Playing:
		// settings icon top right
		m_prevGameState = GameState::TrackSelection;
		break;
	case GameState::Paused:

		m_gui->addButton(
			std::bind(&MenuSystem::resumePlaying, this)
			, "Resume"
			, zero
			, font[Assets::Fonts::GUI::Content]
			, 15u
			, guiTexture[Assets::Textures::GUI::Button]
			, Button::s_TEXT_RECT_LEFT
			, Button::s_TEXT_RECT_MID
			, Button::s_TEXT_RECT_RIGHT);
		m_gui->addButton(
			std::bind(&MenuSystem::mainMenuOptions, this)
			, "Options"
			, zero
			, font[Assets::Fonts::GUI::Content]
			, 15u
			, guiTexture[Assets::Textures::GUI::Button]
			, Button::s_TEXT_RECT_LEFT
			, Button::s_TEXT_RECT_MID
			, Button::s_TEXT_RECT_RIGHT);
		m_gui->configure(GUI::Layouts::Custom);
		m_prevGameState = GameState::Playing;
	case GameState::Exit:
		m_window.close();
		m_prevGameState = GameState::Exit;
		break;
	case GameState::Upgrade:
		m_gui->clear();

		m_gui->addButton(
			std::bind(&MenuSystem::upgradeWheelsBtn, this)
			, "Wheels"
			, sf::Vector2f(200.0f, 100.0f)
			, font[Assets::Fonts::GUI::Content]
			, 25u
			, guiTexture[Assets::Textures::GUI::Button]
			, Button::s_TEXT_RECT_LEFT
			, Button::s_TEXT_RECT_MID
			, Button::s_TEXT_RECT_RIGHT);

		m_gui->addButton(
			std::bind(&MenuSystem::upgradeHandlingBtn, this)
			, "Handling"
			, sf::Vector2f(200.0f, 300.0f)
			, font[Assets::Fonts::GUI::Content]
			, 25u
			, guiTexture[Assets::Textures::GUI::Button]
			, Button::s_TEXT_RECT_LEFT
			, Button::s_TEXT_RECT_MID
			, Button::s_TEXT_RECT_RIGHT);

		m_gui->addButton(
			std::bind(&MenuSystem::upgradeEngineBtn, this)
			, "Engine"
			, sf::Vector2f(200.0f, 500.0f)
			, font[Assets::Fonts::GUI::Content]
			, 25u
			, guiTexture[Assets::Textures::GUI::Button]
			, Button::s_TEXT_RECT_LEFT
			, Button::s_TEXT_RECT_MID
			, Button::s_TEXT_RECT_RIGHT);
		m_prevGameState = GameState::TrackSelection;
		break;
	case GameState::None:
		break;
	default:
		break;
	}
}

void MenuSystem::receive(const entityx::ComponentAddedEvent<Player>& e)
{
	m_playerId = e.entity.id();
}

/// <summary>
/// @brief updates menu system.
/// 
/// 
/// </summary>
/// <param name="entities">reference to entity manager</param>
/// <param name="events">reference to events manager</param>
/// <param name="dt">time per update</param>
void MenuSystem::update(entityx::EntityManager & entities
	, entityx::EventManager & events
	, double dt)
{
	// Update the texture rectangles
	if (m_gameState == GameState::TrackSelection || m_gameState == GameState::CarSelection)
	{
		updateRect();
		m_LBRect->setScale(sf::Vector2f(1, 1));
		m_RBRect->setScale(sf::Vector2f(1, 1));
	}
	else
	{
		m_LBRect->setScale(sf::Vector2f(0, 0));
		m_RBRect->setScale(sf::Vector2f(0, 0));
	}

	// Back function
	if (m_controller->m_currentState.m_B && !press)
	{
		if (m_gameState == GameState::Upgrade)
		{
			// this explicit component removal is required
			//  as it fixes a bug caused by reset() not calling
			//  the componentRemovedEvent<Car>
			m_entityManager.remove<Car>(m_playerId);

			m_entityManager.reset();
		}
		m_eventManager.emit<EvChangeGameState>(m_prevGameState);
	}

	// updates the gui
	m_gui->update(static_cast<float>(dt));
}

void MenuSystem::mainMenuBtnStartGame()
{
	m_eventManager.emit<EvSoundEffect>(Assets::Sounds::SoundEffects::ButtonClick);
	m_eventManager.emit<EvChangeGameState>(GameState::CarSelection);
}

void MenuSystem::mainMenuBtnExitGame()
{
	m_eventManager.emit<EvSoundEffect>(Assets::Sounds::SoundEffects::ButtonClick);
	m_eventManager.emit<EvChangeGameState>(GameState::Exit);
}

void MenuSystem::mainMenuOptions()
{
	m_eventManager.emit<EvSoundEffect>(Assets::Sounds::SoundEffects::ButtonClick);
	m_eventManager.emit<EvChangeGameState>(GameState::OptionsScreen);
}

void MenuSystem::mainMenuBtnHelp()
{
	m_eventManager.emit<EvSoundEffect>(Assets::Sounds::SoundEffects::ButtonClick);
	m_eventManager.emit<EvChangeGameState>(GameState::HelpScreen);
}

void MenuSystem::mainMenuBtnCredits()
{
	m_eventManager.emit<EvSoundEffect>(Assets::Sounds::SoundEffects::ButtonClick);
	m_eventManager.emit<EvChangeGameState>(GameState::Credits);
}

void MenuSystem::gameOverPlayAgain()
{
	m_eventManager.emit<EvSoundEffect>(Assets::Sounds::SoundEffects::ButtonClick);
	m_eventManager.emit<EvChangeGameState>(GameState::MainMenu);
}

void MenuSystem::resumePlaying()
{
	m_eventManager.emit<EvSoundEffect>(Assets::Sounds::SoundEffects::ButtonClick);
	m_eventManager.emit<EvChangeGameState>(GameState::Playing);
}

void MenuSystem::carSelectionBtnSelectCar()
{
	m_eventManager.emit<EvSoundEffect>(Assets::Sounds::SoundEffects::ButtonClick);
	m_eventManager.emit<EvCarSelected>(m_index + 1);
	m_eventManager.emit<EvChangeGameState>(GameState::TrackSelection);
}

void MenuSystem::trackSelectionStartRace()
{
	m_eventManager.emit<EvStopSong>(Assets::Sounds::Songs::MenuSong);
	m_eventManager.emit<EvSoundEffect>(Assets::Sounds::SoundEffects::ButtonClick);
	m_eventManager.emit<EvTrackSelected>(m_index + 1);
	if (m_index == 1)
	{
		m_eventManager.emit<EvPlaySong>(Assets::Sounds::Songs::RockyRoad);
	}
	else if (m_index == 2)
	{
		m_eventManager.emit<EvPlaySong>(Assets::Sounds::Songs::GreenHill);
	}
	else
	{
		m_eventManager.emit<EvPlaySong>(Assets::Sounds::Songs::SandyBeach);
	}
	m_eventManager.emit<EvChangeGameState>(GameState::Playing);
}

void MenuSystem::trackSelectionUpgradeCar()
{
	m_eventManager.emit<EvSoundEffect>(Assets::Sounds::SoundEffects::ButtonClick);
	m_eventManager.emit<EvChangeGameState>(GameState::Upgrade);
}
void MenuSystem::updateRect()
{
	auto& textureRect = (*m_textureRects);
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A) || m_controller->m_currentState.m_LB) && !press  )
	{
		if (m_index < maxTexture - 1)
		{
			m_eventManager.emit<EvSoundEffect>(Assets::Sounds::SoundEffects::SliderMove);
			press = true;
			m_index++;
			for (int i = 0; i < maxTexture; i++)
			{
				textureRect[i].setPosition(textureRect[i].getPosition().x - m_window.getSize().x * 0.25 - 50, textureRect[i].getPosition().y);
				if (m_gameState == GameState::CarSelection)
				{
					textureRect[i].setSize(sf::Vector2f(80, 180));
					if (i == 2 || i == 3)
					{
						textureRect[i].setSize(sf::Vector2f(80, 160));
					}
				}
				else
				{
					textureRect[i].setSize(sf::Vector2f(100, 100));
				}
				textureRect[i].setOrigin(textureRect[i].getSize() / 2.f);
			}
			if (m_gameState == GameState::CarSelection)
			{
				textureRect[m_index].setSize(sf::Vector2f(120, 300));
				if (m_index == 2 || m_index == 3)
				{
					textureRect[m_index].setSize(sf::Vector2f(120, 260));
				}
			}
			else
			{
				textureRect[m_index].setSize(sf::Vector2f(200, 200));
			}
			textureRect[m_index].setOrigin(textureRect[m_index].getSize() / 2.f);
			textureRect[m_index].setPosition(sf::Vector2f(m_window.getSize().x * 0.5, 150));
		}

	}
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) || m_controller->m_currentState.m_RB) && !press)
	{
		if (m_index > 0)
		{
			m_eventManager.emit<EvSoundEffect>(Assets::Sounds::SoundEffects::SliderMove);
			press = true;
			m_index--;
			for (int i = 0; i < maxTexture; i++)
			{
				textureRect[i].setPosition(textureRect[i].getPosition().x + m_window.getSize().x * 0.25 + 50, textureRect[i].getPosition().y);
				if (m_gameState == GameState::CarSelection)
				{
					textureRect[i].setSize(sf::Vector2f(80, 180));
					if (i == 2 || i == 3)
					{
						textureRect[i].setSize(sf::Vector2f(80, 160));
					}
				}
				else
				{
					textureRect[i].setSize(sf::Vector2f(100, 100));
				}
				textureRect[i].setOrigin(textureRect[i].getSize() / 2.f);
			}
			if (m_gameState == GameState::CarSelection)
			{
				textureRect[m_index].setSize(sf::Vector2f(120, 300));
				if (m_index == 2 || m_index == 3)
				{
					textureRect[m_index].setSize(sf::Vector2f(120, 260));
				}
			}
			else
			{
				textureRect[m_index].setSize(sf::Vector2f(200, 200));
			}
			textureRect[m_index].setOrigin(textureRect[m_index].getSize() / 2.f);
			textureRect[m_index].setPosition(sf::Vector2f(m_window.getSize().x * 0.5, 150));
		}
	}
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !sf::Keyboard::isKeyPressed(sf::Keyboard::A)
		&& (!m_controller->m_currentState.m_RB && !m_controller->m_currentState.m_LB && !m_controller->m_currentState.m_B)
		&& press)
	{
		press = false;
	}
}

void MenuSystem::upgradeWheelsBtn()
{
	auto& player = m_entityManager.get(m_playerId);
	auto& currency = player.component<Player>()->m_currency;
	auto& wheels = player.component<Player>()->m_wheel;

	const auto& cost = m_cost * (wheels + 1);

	if (wheels < 3)
	{
		if (currency > cost)
		{
			m_eventManager.emit<EvSoundEffect>(Assets::Sounds::SoundEffects::ButtonClick);
			currency -= cost;
			wheels++;
		}
		else
		{
			// can't afford
		}
	}
	else
	{
		// can't purchase
	}
}
void MenuSystem::upgradeHandlingBtn()
{
	auto& player = m_entityManager.get(m_playerId);
	auto& currency = player.component<Player>()->m_currency;
	auto& handling = player.component<Player>()->m_handling;

	const auto& cost = m_cost * (handling + 1);

	if (handling < 3)
	{
		if (currency > cost)
		{
			m_eventManager.emit<EvSoundEffect>(Assets::Sounds::SoundEffects::ButtonClick);
			currency -= cost;
			handling++;
		}
		else
		{
			// can't afford
		}
	}
	else
	{
		// can't purchase
	}
}

void MenuSystem::upgradeEngineBtn()
{
	m_eventManager.emit<EvSoundEffect>(Assets::Sounds::SoundEffects::ButtonClick);
	auto& player = m_entityManager.get(m_playerId);
	auto& currency = player.component<Player>()->m_currency;
	auto& engine = player.component<Player>()->m_engine;

	const auto& cost = m_cost * (engine + 1);

	if (engine < 3)
	{
		if (currency > cost)
		{
			m_eventManager.emit<EvSoundEffect>(Assets::Sounds::SoundEffects::ButtonClick);
			currency -= cost;
			engine++;
		}
		else
		{
			// can't afford
		}
	}
	else
	{
		// can't purchase
	}
}

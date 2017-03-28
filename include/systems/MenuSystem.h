#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include "entityx\System.h"

#include "events\EvChangeGameState.h"
#include "events\EvAssetsLoaded.h"
#include "events\EvTrackSelected.h"
#include "events\EvCarSelected.h"
#include "events\EvUpgradeEngine.h"
#include "events\EvUpgradeHandling.h"
#include "events\EvUpgradeWheels.h"
#include "events\EvSoundEffect.h"
#include "events\EvPlaySong.h"
#include "events\EvStopSong.h"

#include "components\Player.h"
#include "components\Car.h"

#include "util\GameStates.h"
#include "util\Xbox360Controller.h"

#include "gui\GUI.h"

/// <summary>
/// @author Rafael Plugge & Huong Huynh
/// @version 1.0
/// @brief Will handle Menus and GUIs.
/// 
/// </summary>
class MenuSystem
	: public entityx::System<MenuSystem>
	, public entityx::Receiver<MenuSystem>
{
public:
	MenuSystem(
		entityx::EventManager& eventManager
		, entityx::EntityManager& entityManager
		, sf::RenderWindow& window
		, std::shared_ptr<GUI> gui
		, std::shared_ptr<Xbox360Controller> controller
		, const std::shared_ptr<ResourceHolders>& resources
		, std::shared_ptr<std::vector<sf::RectangleShape>> textRects
		, std::shared_ptr<sf::Sprite> bgSprite
		, std::shared_ptr<sf::RectangleShape> lbRect
		, std::shared_ptr<sf::RectangleShape> rbRect);

	void configure(entityx::EventManager &eventManager) override;

	void receive(const EvChangeGameState& e);
	void receive(const EvAssetsLoaded& e);
	void receive(const entityx::ComponentAddedEvent<Player> &e);

	void update(entityx::EntityManager& entities, entityx::EventManager& events, double dt) override;

private:
	// ----- BUTTON FUNCTIONS ----- //

	// start game
	void mainMenuBtnStartGame();

	// exit game
	void mainMenuBtnExitGame();

	// go to options
	void mainMenuOptions();

	// go to help
	void mainMenuBtnHelp();

	// go to credits
	void mainMenuBtnCredits();

	// go to main menu
	void gameOverPlayAgain();

	// resume game
	void resumePlaying();

	// go to track selection
	void carSelectionBtnSelectCar();

	// starts race
	void trackSelectionStartRace();

	// go to upgrade car (work shop)
	void trackSelectionUpgradeCar();

	// upgrade screen button functions

	void upgradeWheelsBtn();
	void upgradeHandlingBtn();
	void upgradeEngineBtn();

	// reference to our event manager (allows for emission of events)
	entityx::EventManager& m_eventManager;

	// reference to our entity manager (needed to retrieve player entity)
	entityx::EntityManager& m_entityManager;

	// reference to our render target
	sf::RenderWindow& m_window;

	// reference to our game state
	GameState m_gameState;

	// reference to a shared pointer of the controller
	std::shared_ptr<Xbox360Controller> m_controller;

	// unique pointer to our managed gui
	std::shared_ptr<GUI> m_gui;

	// shared pointer to our resources
	std::shared_ptr<ResourceHolders> m_resources;

	// To hold textures for car selection and track selection
	std::shared_ptr<std::vector<sf::RectangleShape>> m_textureRects;

	// Current index the cars/tracks texture rectangles is on
	int m_index;

	// checks for LB and RB clicks and updates the cars/tracks texture rectangles
	void updateRect();

	// to check if button is pressed or not
	bool press = false;

	// stores background texture
	sf::Texture bg_texture;
	// sprite to draw the background texture
	std::shared_ptr<sf::Sprite> m_bgSprite;

	// value for Music Volume slider
	float valueMusicVolume = 100.0f;

	// value for Sound Effect slider
	float valueSoundEffect = 100.0f;

	// previous game state
	GameState m_prevGameState;

	// checking on or off for Music check box
	bool check = false;

	entityx::Entity::Id m_playerId;

	// base upgrade cost
	const int m_cost = 30;

	// max textures count
	int maxTexture;

	// LB and RB textures
	std::shared_ptr<sf::Texture> m_LBTexture;
	std::shared_ptr<sf::Texture> m_RBTexture;
	// LB and RB rect
	std::shared_ptr<sf::RectangleShape> m_LBRect;
	std::shared_ptr<sf::RectangleShape> m_RBRect;
};

#endif // !MENU_SYSTEM_H
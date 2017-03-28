#pragma once

#include "SFML\Graphics.hpp"

#include "Thor\Resources.hpp"

#include "entityx\System.h"

#include "events\EvAssetsLoaded.h"

#include "components\Display.h"
#include "components\Position.h"
#include "components\Car.h"
#include "components\Road.h"
#include "components\Particle.h"
#include "components\Light.h"

#include "util\ScreenSize.h"
#include "util\ResourceTypes.h"
#include "util\Debug.h"

#include "events\EvDayTime.h"
#include "events\EvNightTime.h"

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief System to handle the rendering of all game images.
/// 
/// 
/// </summary>
class RenderSystem
	: public entityx::System<RenderSystem>
	, public entityx::Receiver<RenderSystem>
{
public:
	// Default constructor
	RenderSystem(
		sf::RenderWindow& window
		, const std::shared_ptr<sf::RectangleShape>& viewRect
		, const std::shared_ptr<sf::View>& view
		, const std::shared_ptr<ResourceHolders>& resourceHolder
	);

	void configure(entityx::EventManager& eventManager) override;

	void receive(const EvAssetsLoaded& e);

	// Runs system logic
	void update(
		entityx::EntityManager& entities
		, entityx::EventManager& events
		, double dt) override;
	// updates the alpha to give a daytime and nighttime effect
	void updateAlpha(entityx::EventManager & events);

private:

	// reference to the SFML render window
	sf::RenderWindow& m_window;

	// used to cull entities outside of rect
	std::shared_ptr<sf::RectangleShape> m_viewRect; 

	// shared pointer to texture resource holders
	std::shared_ptr<ResourceHolders> m_resources;

	// sprite used for 2D draws
	sf::RectangleShape m_sprite;

	// current view of player 
	std::shared_ptr<sf::View> m_view;

	// to draw the bg texture
	sf::Sprite m_bgSprite;
	// to load in a shader
	sf::Shader m_shader;
	// shader applied to this rectangle shader
	sf::RectangleShape m_rectShader;
	// control the alpha of the shader
	float m_alpha = 0;
	// bool to check for day or night
	bool m_daytime = true;
	// shader color blue
	sf::Color m_blue{ 0 , 0, 255, 255 };
};


#ifndef MINI_MAP_SYSTEM_H
#define MINI_MAP_SYSTEM_H

#include "SFML\Graphics.hpp"

#include "entityx\System.h"

#include "components\Display.h"
#include "components\Position.h"
#include "components\Car.h"
#include "components\Road.h"
#include "components\Player.h"
#include "components\Particle.h"

#include "Thor\Vectors.hpp"

#include "events\EvAssetsLoaded.h"

/// <summary>
/// @author Huong Thi Thanh Huynh
/// @version 1.0
/// @brief Will handle the mini map view
/// 
/// 
/// </summary>
class MiniMapSystem 
	: public entityx::System<MiniMapSystem>
	, public entityx::Receiver<MiniMapSystem>

{
public:
	MiniMapSystem(sf::RenderWindow& window
		, const std::shared_ptr<ResourceHolders>& resourceHolder);
	void update(
		entityx::EntityManager& entities
		, entityx::EventManager& events
		, double dt) override;
	void configure(entityx::EventManager & eventManager);
	void receive(const entityx::ComponentAddedEvent<Player>& e);
	void setView(Position::Handle playerPos);
	void receive(const EvAssetsLoaded& e);
	void MiniMapSystem::updateAlpha();

private:
	// reference to the SFML render window
	sf::RenderWindow& m_window;

	// shared pointer to texture resource holders
	std::shared_ptr<ResourceHolders> m_spResource;

	// sprite used for drawing car and roads
	sf::RectangleShape m_sprite;

	// mini map view on top right corner
	sf::View m_minimap;

	sf::CircleShape m_circle; // circle of view
	entityx::Entity::Id m_playerID; // to store the player ID

	// the background texture
	sf::Texture m_bg;
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

#endif //!MINI_MAP_SYSTEM_H


#include "systems\MiniMapSystem.h"

/// <summary>
/// @brief Mini Map system constructor.
/// 
/// 
/// </summary>
MiniMapSystem::MiniMapSystem(sf::RenderWindow& window
	, const std::shared_ptr<ResourceHolders>& resourceHolder)
	: m_window(window)
	, m_spResource(resourceHolder)
	, m_sprite()
{
	m_minimap.setViewport(sf::FloatRect(0.67f, 0.03f, 0.30f, 0.30f)); // put on top right corner
	m_minimap.zoom(7.5f); // make mini map smaller

	// loading in a shader 
	if (!m_shader.loadFromFile("./resources/shaders/shader.txt", sf::Shader::Fragment))
	{
		std::cout << "Shader didn't load" << std::endl;
	}

	m_alpha = 0; // no darkness
	m_blue.a = m_alpha; // apply to the color
	m_shader.setParameter("color", m_blue); // set the shader color

	m_rectShader.setSize(sf::Vector2f(m_window.getSize())* 7.f); // set the size
	m_rectShader.setOrigin(m_rectShader.getSize() / 2.f); // set the origin 
	m_rectShader.setScale(10, 10); // set the scale

}
/// <summary>
/// @brief Configures the scroll system with any events that may happen
/// 
/// 
/// </summary>
/// <param name="eventManager">Event Manager </param>
void MiniMapSystem::configure(entityx::EventManager & eventManager)
{
	eventManager.subscribe<entityx::ComponentAddedEvent<Player>>(*this);
	eventManager.subscribe<EvAssetsLoaded>(*this);
}

/// <summary>
/// @ brief To receive an entity with a player component when a player is added
/// </summary>
/// <param name="e">A player adding event</param>
void MiniMapSystem::receive(const entityx::ComponentAddedEvent<Player>& e)
{
	m_playerID = e.entity.id(); // get id of player
	entityx::Entity player = e.entity; // get the player entity
	Position::Handle pos = player.component<Position>(); // get pos of player

	m_minimap.setCenter(pos->m_position); // set the view once to the player pos
	m_window.setView(m_minimap); // set the new view to the window
}

/// <summary>
/// @brief Updates the Mini Map system and draws the mini map
/// 
/// 
/// </summary>
/// <param name="entities">EntityX entity manager</param>
/// <param name="events">EntityX event manager</param>
/// <param name="dt">milliseconds between updates</param>
void MiniMapSystem::update(
	entityx::EntityManager& entities
	, entityx::EventManager& events
	, double dt)
{
	const TextureCarHolder& carResourceHolder = *(m_spResource->m_textures->m_car);
	const TextureTrackHolder& trackResourceHolder = *(m_spResource->m_textures->m_track);
	const auto& shaderResourceHolder = *(m_spResource->m_shaders);

	Display::Handle display;
	Position::Handle position;

	m_window.setView(m_minimap); // set the mini map
	m_window.draw(m_bgSprite);//draw background texture
	// Draws the whole map but uses the view of the mini map
	for (entityx::Entity entity : entities.entities_with_components(display, position))
	{
		if (entity.has_component<Car>())
		{
			const Car::Handle& car = entity.component<Car>();
			m_sprite.setTexture(&(carResourceHolder[car->m_type]), true);
		}
		if (entity.has_component<Road>())
		{
			m_sprite.setTexture(&(trackResourceHolder[Assets::Textures::Tracks::Track]), true);
		}
		if (entity.has_component<Particle>())
		{
			m_sprite.setTexture(&(trackResourceHolder[Assets::Textures::Tracks::Background]), true);
		}
		m_sprite.setPosition(position->m_position);
		m_sprite.setRotation(position->m_rotation);
		m_sprite.setTextureRect(display->m_coord);
		m_sprite.setSize(sf::Vector2f(display->m_coord.width, display->m_coord.height));
		m_sprite.setFillColor(display->m_color);
		m_sprite.setOrigin(display->m_origin);

		if (entity.has_component<Particle>())
		{
			auto& timeDur = entity.component<Particle>()->m_liveDur;
			auto& timeLeft = entity.component<Particle>()->m_timeToLive;
			auto& size = entity.component<Particle>()->m_size;
			m_sprite.setSize(size);
			m_sprite.setOrigin(m_sprite.getSize() / 2.0f);

			auto& shader = *(shaderResourceHolder[static_cast<int>(Assets::Shaders::Simple)]);
			const auto& rect = m_sprite.getGlobalBounds();
			shader.setParameter("texture", shader.CurrentTexture);
			shader.setParameter("timeDur", timeDur);
			shader.setParameter("timeElapsed", timeLeft);

			m_window.draw(m_sprite, &(shader));
		}
		else
		{
			m_window.draw(m_sprite);
		}

		entityx::Entity player = entities.get(m_playerID); // get the player entity
		Position::Handle playerPos = player.component<Position>(); // get pos of player
																   // check if there is player
		if (m_playerID.id())
		{
			setView(playerPos); // set the view on the player
		}
	}

	updateAlpha();
	// draw the shader
	m_window.draw(m_rectShader, &m_shader);
}
void MiniMapSystem::setView(Position::Handle playerPos)
{
	// check to see if the player is outside of circle before moving the view 
	if ((m_circle.getRadius() * m_circle.getRadius()) < thor::squaredLength(m_circle.getPosition() - playerPos->m_position))
	{

		// vector from circle to player
		sf::Vector2f vectorBetween(playerPos->m_position - m_circle.getPosition());

		// direction from circle to player
		sf::Vector2f vectorBetweenDir = thor::unitVector(vectorBetween);

		// length from centre of circle to player
		float lengthToPlayer = thor::length(vectorBetween);

		// difference between player to centre of circle and the radius of the circle
		float distanceToMove = lengthToPlayer - m_circle.getRadius();

		m_circle.move((vectorBetweenDir * distanceToMove)); // set new pos of circle
		m_minimap.setCenter(m_circle.getPosition()); // set new center view as the the circle pos
		m_window.setView(m_minimap); // set the new view to the window
	}
}
void MiniMapSystem::receive(const EvAssetsLoaded& e)
{
	if (e.m_gameState == GameState::Playing)
	{
		const auto& bgTrack = *(m_spResource->m_textures->m_track);
		m_bgSprite.setTexture(bgTrack[Assets::Textures::Tracks::Background]); // set the texture of sprite
		m_bgSprite.setOrigin(m_bgSprite.getLocalBounds().width / 2.f, m_bgSprite.getLocalBounds().height / 2.f); // set the origin of the sprite
		m_bgSprite.setScale(10, 10); // make it bigger
	}
}
/// <summary>
/// @brief updates the alpha of the shader color to give an impression of day and night in the game
/// 
/// </summary>
void MiniMapSystem::updateAlpha()
{
	if (m_alpha > 0 && !m_daytime) // if its night time
	{
		m_alpha -= 0.05; // Make everything brighter
		if (m_alpha <= 0)
		{
			m_daytime = true;
		}
	}

	if (m_daytime) // if its day time
	{
		m_alpha += 0.1; // Make everything darker
		if (m_alpha >= 200)
		{
			m_daytime = false;
		}
	}
	m_blue.a = m_alpha; // set alpha to color
	m_shader.setParameter("color", m_blue); // set color to shader
}


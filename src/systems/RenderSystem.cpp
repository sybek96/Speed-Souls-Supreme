#include "systems\RenderSystem.h"


/// <summary>
/// @brief Constructs RenderSystem.
/// 
/// initializes member variables and stores parameters
/// </summary>
/// <param name="window"></param>
/// <param name="texture"></param>
RenderSystem::RenderSystem(
	sf::RenderWindow& window
	, const std::shared_ptr<sf::RectangleShape>& viewRect
	, const std::shared_ptr<sf::View>& view
	, const std::shared_ptr<ResourceHolders>& resourceHolder)

	: m_window(window)
	, m_resources(resourceHolder)
	, m_viewRect(viewRect)
	, m_sprite()
	, m_view(view)
{	
	m_alpha = 0; // no darkness
	m_blue.a = m_alpha;
	m_shader.setParameter("color", m_blue); // set the shader color
}

void RenderSystem::configure(entityx::EventManager & eventManager)
{
	eventManager.subscribe<EvAssetsLoaded>(*this);
}

void RenderSystem::receive(const EvAssetsLoaded & e)
{
	if (e.m_gameState == GameState::Playing)
	{
		const auto& background = *(m_resources->m_textures->m_track);
		m_bgSprite.setTexture(background[Assets::Textures::Tracks::Background]); // set the texture of sprite
		m_bgSprite.setOrigin(m_bgSprite.getLocalBounds().width / 2.f, m_bgSprite.getLocalBounds().height / 2.f); // set the origin of the sprite
		m_bgSprite.setScale(10, 10); // make it bigger

		// load the shader
		if (!m_shader.loadFromFile("./resources/shaders/shader.txt", sf::Shader::Fragment))
		{
			debugMsg("Shader didn't load");
		}

		m_rectShader.setSize(sf::Vector2f(m_bgSprite.getLocalBounds().width, m_bgSprite.getLocalBounds().height)); // set the size
		m_rectShader.setOrigin(m_rectShader.getSize() / 2.f); // set the origin 
		m_rectShader.setScale(10, 10); // set the scale
	}
}

/// <summary>
/// @brief Draws entities with Position and Display.
/// 
/// 
/// </summary>
/// <param name="entities">EntityX entity manager</param>
/// <param name="events">EntityX event manager</param>
/// <param name="dt">milliseconds between updates</param>
void RenderSystem::update(entityx::EntityManager & entities
	, entityx::EventManager & events
	, double dt)
{
	const auto& carResourceHolder = *(m_resources->m_textures->m_car);
	const auto& trackResourceHolder = *(m_resources->m_textures->m_track);
	auto& particleResourceHolder = *(m_resources->m_textures->m_particle);

	auto& shaderResourceHolder = *(m_resources->m_shaders);

	Display::Handle display;
	Position::Handle position;
	m_window.setView(*m_view);
	m_window.draw(m_bgSprite);//draw background texture

	for (entityx::Entity entity : entities.entities_with_components(display, position))
	{
		if (entity.has_component<Car>())
		{
			const Car::Handle& car = entity.component<Car>();
			m_sprite.setTexture(&(carResourceHolder[car->m_type]), true);
		}
		else if (entity.has_component<Road>())
		{
			m_sprite.setTexture(&(trackResourceHolder[Assets::Textures::Tracks::Track]), true);
		}
		else if (entity.has_component<Particle>())
		{
			m_sprite.setTexture(&(particleResourceHolder[Assets::Textures::Particles::Radial]), true);
		}
		else if (entity.has_component<Light>())
		{
			m_sprite.setTexture(&(particleResourceHolder[Assets::Textures::Particles::Cone]), true);
		}
		m_sprite.setPosition(position->m_position);
		m_sprite.setRotation(position->m_rotation);
		m_sprite.setTextureRect(display->m_coord);
		m_sprite.setSize(sf::Vector2f(display->m_coord.width, display->m_coord.height));
		m_sprite.setFillColor(display->m_color);
		m_sprite.setOrigin(display->m_origin);

		// dont draw if the sprite is not inside the view rect - Culling
		if (m_sprite.getGlobalBounds().intersects(m_viewRect->getGlobalBounds()))
		{
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
				shader.setParameter("color", display->m_color);

				m_window.draw(m_sprite, &(shader));
			}
			else if (entity.has_component<Light>())
			{
				auto& shader = *(shaderResourceHolder[static_cast<int>(Assets::Shaders::Light)]);
				shader.setParameter("texture", shader.CurrentTexture);
				shader.setParameter("color", display->m_color);
			}
			else
			{
				m_window.draw(m_sprite);
			}
		}
	}

	// update the alpha of shader
	updateAlpha(events);
	// draw the shader
	m_window.draw(m_rectShader, &m_shader);
}

/// <summary>
/// @brief updates the alpha of the shader color to give an impression of day and night in the game
/// 
/// </summary>
void RenderSystem::updateAlpha(entityx::EventManager & events)
{
	if (m_alpha > 0 && !m_daytime) // if its night time
	{
		m_alpha -= 0.05; // Make everything brighter
		if (m_alpha < 100)
		{
			events.emit<EvDayTime>(); // emit the day time event
		}
		if (m_alpha <= 0)
		{
			m_daytime = true;
		}
	}

	if (m_daytime) // if its day time
	{
		m_alpha += 0.1; // Make everything darker
		if (m_alpha >= 100)
		{
			events.emit<EvNightTime>(); // emit the day time event
		}
		if (m_alpha >= 200)
		{
			m_daytime = false;
		}
	}
	m_blue.a = m_alpha; // set alpha to color
	m_shader.setParameter("color", m_blue); // set color to shader
}

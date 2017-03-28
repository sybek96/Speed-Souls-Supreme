#include "systems\ParticleSystem.h"

/// <summary>
/// @brief Default system constructor
/// 
/// store any references and shared pointers
/// </summary>
ParticleSystem::ParticleSystem(
	entityx::EntityManager& entityManager
	, const std::shared_ptr<ResourceHolders>& resources)

	: m_entityManager(entityManager)
	, m_resources(resources)
{
}

/// <summary>
/// @brief Receiver configuration function.
/// 
/// Allows this to subscribe to "listen" for event emissions
/// </summary>
/// <param name="eventManager">reference to event manager</param>
void ParticleSystem::configure(entityx::EventManager & eventManager)
{
	eventManager.subscribe<EvChangeGameState>(*this);
	eventManager.subscribe<EvAssetsLoaded>(*this);
	eventManager.subscribe<EvEmitParticles>(*this);
	eventManager.subscribe<EvEraseParticle>(*this);
	eventManager.subscribe<entityx::ComponentRemovedEvent<Particle>>(*this);
}

/// <summary>
/// @brief Updates particles status.
/// 
/// Updates particles based on their position, velocity, type and time to live
/// </summary>
/// <param name="entities">defines entity manager</param>
/// <param name="events">defines the event manager</param>
/// <param name="dt">time between updates, in milliseconds</param>
void ParticleSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, double dt)
{
	// conversion of dt into delta time from
	//  dt = milliseconds(double) into deltaTime = seconds(float)
	const float& deltaTime = static_cast<float>(dt / 1000.0);

	// for every particle id in our vector particle ids
	for (auto& particleID : m_particleIDs)
	{
		// get our entity through it's id
		entityx::Entity particle = entities.get(particleID);
		
		// store references to our used component elements
		//  used so to make it much clearer what exactly this system
		//  uses and to make increase readability

		auto& partPos = particle.component<Position>()->m_position;
		auto& partVel = particle.component<Velocity>()->m_velocity;
		auto& partType = particle.component<Particle>()->m_type;
		auto& partTime = particle.component<Particle>()->m_timeToLive;
		auto& partDur = particle.component<Particle>()->m_liveDur;
		auto& partColour = particle.component<Display>()->m_color;
		auto& partAlpha = particle.component<Particle>()->m_alpha;

		switch (partType)
		{
		case ParticleType::Fade:
		{
			// update particle if particle hasn't ran out of time
			if (partTime > 0.0f)
			{
				partTime -= deltaTime;
				partPos += (partVel * deltaTime);
				partColour.a = static_cast<sf::Uint8>(partAlpha);
			}
			break;
		}
		default:
			break;
		}
		// if particle has ran out of remove particle
		if (partTime <= 0.0f)
		{
			events.emit<EvEraseParticle>(particleID);
			break;
		}
	}
}

/// <summary>
/// @brief Game State changed even.
/// 
/// 
/// </summary>
/// <param name="e">event struct</param>
void ParticleSystem::receive(const EvChangeGameState & e)
{
}

/// <summary>
/// @brief loads up particle shader
/// 
/// Needed because of thor's resource holder
/// not working with sf::Shader as it simply seemed to not work well with
/// sf::Shader::loadFromFile()
/// </summary>
/// <param name="e"></param>
void ParticleSystem::receive(const EvAssetsLoaded & e)
{
	std::string fileNameLightFrag(resourcePath()), fileNameFrag(resourcePath());
	fileNameLightFrag += "shaders/light.frag";
	fileNameFrag += "shaders/shader.frag";

	// store reference to our shader holder
	//  used to avoid any mistakes with operator precedence
	auto& shaders = *(m_resources->m_shaders);

	try
	{
		std::shared_ptr<sf::Shader> shader = std::make_shared<sf::Shader>();
		shader->loadFromFile(fileNameFrag, sf::Shader::Type::Fragment);

		shaders.push_back(std::move(shader));

		shader = std::make_shared<sf::Shader>();
		shader->loadFromFile(fileNameLightFrag, sf::Shader::Type::Fragment);

		shaders.push_back(std::move(shader));
	}
	catch (const std::exception& e)
	{
		std::string msg("Error: \n");
		msg += " occured in: ";
		msg += typeid(*this).name();
		debugMsg(msg.c_str(), e.what());
	}

}

/// <summary>
/// @brief Particle creation event.
/// 
/// This event creates a particle from the defined event sruct's parameters
/// </summary>
/// <param name="e">event struct</param>
void ParticleSystem::receive(const EvEmitParticles & e)
{
	const int& amountOfParticles = e.m_numParticles;
	sf::IntRect texel = sf::IntRect(0, 0, 500, 500);
	const sf::Color& colour = e.m_color;

	sf::Vector2f size;
	size.x = static_cast<float>(e.m_size.x);
	size.y = static_cast<float>(e.m_size.y);

	for (int i = 0; i < amountOfParticles; i++)
	{
		entityx::Entity particle = m_entityManager.create();
		particle.assign<Position>(e.m_position);
		particle.assign<Velocity>(e.m_velocity);
		particle.assign<Display>(texel, colour);
		particle.assign<Particle>(e.m_type, size, e.m_timeToLive);

		m_particleIDs.push_back(std::move(particle.id()));
	}
}

/// <summary>
/// @brief Particle deletion event.
/// 
/// This event deletes a particle whoose Id matches the on defined in the event
/// </summary>
/// <param name="e">event struct</param>
void ParticleSystem::receive(const EvEraseParticle & e)
{
	const auto& begin = m_particleIDs.begin();
	const auto& end = m_particleIDs.end();
	for (auto itt = begin; itt != end; itt++)
	{
		if ((*itt) == e.m_Id)
		{
			m_entityManager.destroy(*itt);
			m_particleIDs.erase(itt);
			break;
		}
	}
}

/// <summary>
/// @brief Particle deleted event.
/// 
/// When a particle component is removed,
/// remove it's ID from our vector of ids
/// </summary>
/// <param name="e">event struct</param>
void ParticleSystem::receive(const entityx::ComponentRemovedEvent<Particle>& e)
{
	const auto& begin = m_particleIDs.begin();
	const auto& end = m_particleIDs.end();
	for (auto itt = begin; itt != end; itt++)
	{
		if ((*itt) == e.entity.id())
		{
			m_particleIDs.erase(itt);
			break;
		}
	}
}

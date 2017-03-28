#pragma once

#include "entityx\Event.h"

#include "SFML\System\Vector2.hpp"

#include "util\ParticleType.h"

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Creates particles.
/// 
/// Event causes Particle system to create a number of particles.
/// </summary>
struct EvEmitParticles : public entityx::Event<EvEmitParticles>
{
	/// <summary>
	/// @brief Default event constructor.
	/// 
	/// Construct particles with defined parameters
	/// </summary>
	/// <param name="position">defines spawn position</param>
	/// <param name="velocity">defines velocity of particles</param>
	/// <param name="timeToLive">defines amount of time particles are alive</param>
	/// <param name="type">defines particle type</param>
	/// <param name="numParticles">defines number of particles to be spawned (defaulted to 1)</param>
	EvEmitParticles(
		const sf::Vector2f& position
		, const sf::Vector2f& velocity
		, const sf::Vector2i& size
		, const float& timeToLive
		, const ParticleType& type
		, const sf::Color& color
		, const int& numParticles = 1)

		: m_position(position)
		, m_velocity(velocity)
		, m_size(size)
		, m_timeToLive(timeToLive)
		, m_type(type)
		, m_numParticles(numParticles)
		, m_color(color)
	{}

	// spawn position
	sf::Vector2f m_position;

	// spawn velocity
	sf::Vector2f m_velocity;

	// width and height in pixels
	sf::Vector2i m_size;

	// amount of seconds to stay alive
	float m_timeToLive;

	// type of particle(s)
	ParticleType m_type;

	// number of particles to spawn
	int m_numParticles;

	//color of the particle
	sf::Color m_color;
};
#ifndef PARTICLE_H
#define PARTICLE_H

#include "entityx\Entity.h"

#include "SFML\System\Vector2.hpp"

#include "util\ParticleType.h"

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Defines a component of a particle entity.
/// 
/// 
/// </summary>
struct Particle : public entityx::Component<Particle>
{
	Particle(
		const ParticleType& type = ParticleType::Fade
		, const sf::Vector2f& size = sf::Vector2f(0.0f, 0.0f)
		, const float& timeToLive = 0.0f
		, const float& alpha = 255.0f);

	// defines type of particle and it's movement
	ParticleType m_type;

	// total time alive
	const float m_liveDur;

	// amount of time particle stays alive
	float m_timeToLive;

	// alpha value (used only for fade types)
	float m_alpha;

	sf::Vector2f m_size;
};

#endif // !PARTICLE_H
#include "components\Particle.h"

/// <summary>
/// @brief Default constructor.
/// 
/// 
/// </summary>
/// <param name="type"></param>
/// <param name="timeToLive"></param>
Particle::Particle(
	const ParticleType & type
	, const sf::Vector2f& size
	, const float & timeToLive
	, const float & alpha)

	: m_type(type)
	, m_timeToLive(timeToLive)
	, m_liveDur(timeToLive)
	, m_alpha(alpha)
	, m_size(size)
{
}

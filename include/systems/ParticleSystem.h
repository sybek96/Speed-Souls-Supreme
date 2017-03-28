#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "entityx\System.h"

#include "events\EvChangeGameState.h"
#include "events\EvAssetsLoaded.h"
#include "events\EvEmitParticles.h"
#include "events\EvEraseParticle.h"

#include "components\Position.h"
#include "components\Velocity.h"
#include "components\Display.h"
#include "components\Particle.h"

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Updates and controls all particle related effects.
/// 
/// 
/// </summary>
class ParticleSystem
	: public entityx::System<ParticleSystem>
	, public entityx::Receiver<ParticleSystem>
{
public:
	ParticleSystem(
		entityx::EntityManager& entityManager
		, const std::shared_ptr<ResourceHolders>& resources);

	void configure(entityx::EventManager& eventManager) override;
	void update(entityx::EntityManager& entities, entityx::EventManager& events, double dt) override;
	void receive(const EvChangeGameState& e);
	void receive(const EvAssetsLoaded& e);
	void receive(const EvEmitParticles& e);
	void receive(const EvEraseParticle& e);
	void receive(const entityx::ComponentRemovedEvent<Particle>& e);

private:
	// reference to entity manager
	entityx::EntityManager& m_entityManager;

	// shared pointer to resources
	std::shared_ptr<ResourceHolders> m_resources;

	// IDs of all particles
	std::vector<entityx::Entity::Id> m_particleIDs;
};

#endif // !PARTICLE_SYSTEM_H
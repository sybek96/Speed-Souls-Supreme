#ifndef MOTION_SYSTEM_H
#define MOTION_SYSTEM_H


#include <algorithm>

#include "Thor\Math.hpp"
#include "Thor\Vectors\VectorAlgebra2D.hpp"
#include "Thor\Vectors\VectorAlgebra3D.hpp"

#include "entityx\System.h"

#include "util\Physics.h"
#include "util\BTMath.h"
#include "util\Debug.h"
#include "util\StaticTime.h"

#include "components\InputControl.h"
#include "components\Player.h"
#include "components\Ai.h"
#include "components\Position.h"
#include "components\Velocity.h"
#include "components\Acceleration.h"
#include "components\Car.h"

#include "events\EvEmitParticles.h"
#include "events\EvSoundEffect.h"
#include "events\EvPlaySong.h"
#include "events\EvStopSong.h"
#include "events\EvStopSoundEffect.h"

/// <summary>
/// @author Sebastian Kruzel
/// @version 2.0
/// @brief System to handle the updating of player motion within the game.
/// 
/// The system will update the player movement and his imputs
/// </summary>
class MotionSystem 
	: public entityx::System<MotionSystem>
	, public entityx::Receiver<MotionSystem>
{

public:
	//default constructor
	MotionSystem(entityx::EventManager& eventManager);
	
	//subscribes to events
	void configure(entityx::EventManager& eventManager) override;

	//Update system logic
	void update(entityx::EntityManager& entities
		, entityx::EventManager& events
		, double dt) override;

	//receives component added event
	void receive(const entityx::ComponentAddedEvent<Car>& e);

	//receives component removed event
	void receive(const entityx::ComponentRemovedEvent<Car>& e);

private:
	//update the car movement
	void updateCarMovement(entityx::EntityManager & entities
		, entityx::EventManager & events
		, double dt);

	//linear motion logic
	void linearMotion(
		Acceleration::Handle& acceleration
		, Velocity::Handle& velocity
		, Position::Handle& position
		, const float& rotation
		, Car::Handle& car
		, const float & dt
		, bool onRoad);

	//ease in (quad)
	float easeIn(float t, float b, float c, float d);

	//ease out (quad)
	float easeOut(float t, float b, float c, float d);

	//check acceleration value is positive or negative
	void accelCheck(Acceleration::Handle& accel, float& accelerateVal);

	//ease inout (quad)
	float easeInOut(float t, float b, float c, float d);

	//check if steering above or below max/min
	void checkSteering(const float& minSteerForce, const float& maxSteerForce);

	//check if handbrake pressed
	bool m_handBrake;

	//the degree at which car may turn
	float m_steeringForce;

	//check if car was slowed down
	bool m_slowDownApplied;

	//a vector of car id's to process
	std::vector<entityx::Entity::Id> m_carIds;
	
	//used for keeping track of how often particles are spawned
	float m_particleSpawnRate;

	// reference to our event manager (allows for emission of events)
	entityx::EventManager& m_eventManager;

};

#endif // !MOTION_SYSTEM_H

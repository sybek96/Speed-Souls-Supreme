#include "systems\MotionSystem.h"

/// <summary>
/// @brief constructs motion system.
/// 
/// will use initialiser list to initialize any
/// variables used in system
/// </summary>
MotionSystem::MotionSystem(entityx::EventManager& eventManager)
	: m_handBrake(false)
	, m_steeringForce(1.0f)
	, m_particleSpawnRate(0.0f)
	, m_eventManager(eventManager)
{
}

/// <summary>
/// @bried subscribe to events
/// 
/// this method will subscribe to the component car
/// added and the component car removed events
/// </summary>
/// <param name="eventManager"></param>
void MotionSystem::configure(entityx::EventManager & eventManager)
{
	eventManager.subscribe<entityx::ComponentAddedEvent<Car>>(*this);
	eventManager.subscribe<entityx::ComponentRemovedEvent<Car>>(*this);
}

/// <summary>
/// @brief will run all calculation methods
/// 
/// This will call all the system logic
/// </summary>
/// <param name="entities">EntityX entity manager</param>
/// <param name="events">EntityX event manager</param>
/// <param name="dt">milliseconds between updates</param>
void MotionSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, double dt)
{
	//this method will run all car motion updates
	updateCarMovement(entities, events, dt);
}

void MotionSystem::updateCarMovement(entityx::EntityManager & entities, entityx::EventManager & events, double dt)
{
	//Input control component handle
	InputControl::Handle input;
	//acceleration component handle
	Acceleration::Handle acceleration;
	//velocity component handle
	Velocity::Handle velocity;
	//position component handle
	Position::Handle position;
	//car component handle
	Car::Handle car;

	//The variables that control steering
	//determines how loose the steering feels
	const float& loose = 2.0f;
	//determines how stiff the steering can get
	const float& stiff = 6.0f;
	//minimum set of the graph (bigger extends the graph making the action longer)
	const float& min = 2000.0f;
	//the minimum degree of steering
	const float& minSteerForce = 0.5f;
	//the maximum degree of rotation
	const float& maxSteerForce = 0.9f;
	//delta time
	const float& deltaTime = static_cast<float>(dt / 1000.0); //divide delta time by 1000 here since its in MS
	//duration of tween
	const float& easeDur = static_cast<float>(sf::seconds(4.0f).asSeconds());
	//update static time class used for easing
	StaticTime::update(dt);
	//easing of the tween
	const float& easeTween = static_cast<float>(StaticTime::s_time.asSeconds());
	//increase spawnrate of particles
	m_particleSpawnRate += dt;
	//For all cars that are currently in the game
	for (auto& carId : m_carIds)
	{
		//get the car entity
		entityx::Entity entity = entities.get(carId);
		//assign position handle
		position = entity.component<Position>();
		//assign velocity handle
		velocity = entity.component<Velocity>();
		//assign acceleration handle
		acceleration = entity.component<Acceleration>();
		//assign car handle
		car = entity.component<Car>();

		//reference acelerate value
		auto& accelerateVal = car->m_accelerateVal;
		//reference max acceleration
		auto& maxAccel = car->m_maxAccel;
		//reference if slowdown was applied
		auto& slowDownApplied = car->m_slowDownApplied;
		//rotation of cars is +90 because sprite is not at 0 rotation
		float rotation = position->m_rotation + 90;
		//if a car entity has a player and input control components process its input
		if (entity.has_component<InputControl>() && entity.has_component<Player>())
		{
			//assign input handle
			input = entity.component<InputControl>();

			//if accelerating add to accelerate
			if (input->m_accelerate)
			{
				m_eventManager.emit<EvSoundEffect>(Assets::Sounds::SoundEffects::Engine);
				//if not handbreaking play engine sound
				//if (!m_handBrake)
				//{
				//	
				//}
				//if accelerate value is less than max acceleration then we may accelerate
				if (accelerateVal < maxAccel)
				{
					//add to acceleration value
					accelerateVal += 5;
					//Calculates how much steering the player may do based on the velocity
					//a pseudo easing function (courtesy of Rafael Giaro)
					m_steeringForce = loose * (1 / (thor::length(velocity->m_velocity) * stiff)) * min;
				}
			}
			//else reset the static time
			else
			{
				StaticTime::reset();
			}
			//if decceleration decrease acceleration
			if (input->m_brake) 
			{
				//check if the person is breaking rather than reversing
				if (accelerateVal > -maxAccel && accelerateVal > 0)
				{
					//apply breaking force
					accelerateVal -= 10;
					//Calculates how much steering the player may do based on the velocity
					//a pseudo easing function (courtesy of Rafael Giaro)
					m_steeringForce = (loose * loose * loose) * (1 / (thor::length(velocity->m_velocity) * stiff)) * min;
					if (accelerateVal > 500 && m_particleSpawnRate > 20)
					{
						m_eventManager.emit<EvSoundEffect>(Assets::Sounds::SoundEffects::Handbrake);
						// To emit a particle,
						//  it needs a position, a velocity, its size (in pixels, width/height),
						//  time it stays alive (in seconds) and it's type (currently there's only one)
						//  and a color
						//  you may also spawn more than one particle although the value is defaulted to one if not specified
						sf::Vector2f pos1(position->m_position + thor::rotatedVector(sf::Vector2f(-120.0f, 50.0f), rotation)), vel1(0.0f, 0.0f);
						sf::Vector2f pos2(position->m_position + thor::rotatedVector(sf::Vector2f(-120.0f, -50.0f), rotation)), vel2(0.0f, 0.0f);
						sf::Vector2i size(70,70);
						float timeToLive(1.2f);
						ParticleType type(ParticleType::Fade);
						sf::Color col = sf::Color::Black;
						//int numParticles(1); // is defaulted to one
						events.emit<EvEmitParticles>(pos1, vel1, size, timeToLive, type, col);
						events.emit<EvEmitParticles>(pos2, vel2, size, timeToLive, type, col);
						m_particleSpawnRate = 0.0f;
					}
				}
				//else if reversing
				else if (accelerateVal > -maxAccel)
				{
					m_eventManager.emit<EvSoundEffect>(Assets::Sounds::SoundEffects::Engine);
					//reverse by reverse value (slow)
					accelerateVal -= 3;
					//Calculates how much steering the player may do based on the velocity
					//a pseudo easing function (courtesy of Rafael Giaro)
					m_steeringForce = loose * (1 / (thor::length(velocity->m_velocity) * stiff)) * min;
				}
			}
			//if the handbrake is pressed
			if (input->m_handbrake)
			{
				//set the friction to be 0.9
				//Physics::setCoefficientOfFriction(0.9f);
				//make steering force large
				m_steeringForce = 1.5f;
				//set handbrake to true (in order to do once)
				m_handBrake = true;
				if (accelerateVal > 200)
				{
					//deccelerate car significantly
					accelerateVal *= 0.97f;
				}

				if (accelerateVal > 10 && m_particleSpawnRate > 10)
				{
					// To emit a particle,
					//  it needs a position, a velocity, its size (in pixels, width/height),
					//  time it stays alive (in seconds) and it's type (currently there's only one)
					//  you may also spawn more than one particle although the value is defaulted to one if not specified
					sf::Vector2f pos(position->m_position + thor::rotatedVector(sf::Vector2f(-130.0f, 0.0f), rotation));
					sf::Vector2f vel = thor::rotatedVector(sf::Vector2f(300.0f, 0.0f), rotation - 180);
					sf::Vector2i size(150, 150);
					float timeToLive(1.0f);
					ParticleType type(ParticleType::Fade);
					sf::Color col = sf::Color::White;
					//int numParticles(1); // is defaulted to one
					events.emit<EvEmitParticles>(pos, vel, size, timeToLive, type, col);
					m_particleSpawnRate = 0.0f;
				}
			}
			//else not handbreaking
			else
			{
				//set friction to be 0.2 (normal)
				Physics::setCoefficientOfFriction(0.2f);
				//handbrake is false
				m_handBrake = false;
			}
			//if axis above 30 rotate car anticlockwise
			if (input->m_turn > 30.0f) 
			{
				//if the squared length of velocity is greater than 100 squared 
				// (player may not rotate when slower)
				if (thor::squaredLength(velocity->m_velocity) > (100.0f * 100.0f))
				{
					//check if steering within acceptable values
					checkSteering(minSteerForce, maxSteerForce);
					//add steering force to rotation of car
					position->m_rotation += m_steeringForce;
					//loop rotation if equal or above 360 to 0
					if (position->m_rotation >= 360.0)
					{
						position->m_rotation = 0;
					}
				}
			}
			//if axis below -30 rotate car clockwise
			if (input->m_turn < -30.0f) 
			{
				//if the squared length of velocity is greater than 100 squared 
				// (player may not rotate when slower)
				if (thor::squaredLength(velocity->m_velocity) > (100.0f * 100.0f))
				{
					//check if steering within acceptable values
					checkSteering(minSteerForce, maxSteerForce);
					//take away from car rotation
					position->m_rotation -= m_steeringForce;
					//loop rotation if equal or below 0 to 360
					if (position->m_rotation <= 0.0)
					{
						position->m_rotation = 359.0;
					}
				}
			}
			if (m_handBrake && input->m_accelerate)
			{
				m_eventManager.emit<EvSoundEffect>(Assets::Sounds::SoundEffects::Handbrake);
			}
			//get the cos of rotation (in radians)
			const float& cosSine = std::cos(thor::toRadian(rotation));
			//get the sin of rotation (in radians)
			const float& sine = std::sin(thor::toRadian(rotation));
			//set acceleration to accelerate value by sin and cos
			acceleration->m_acceleration = sf::Vector2f(accelerateVal * cosSine,
				accelerateVal * sine);
			//do linear motion calculations
			linearMotion(acceleration, velocity, position, rotation, car, deltaTime, car->m_onRoad);
		}
		//AI motion done in AISystem
	}
}

/// <summary>
/// @brief receive car component added event
/// 
/// When a car component is added, then
/// add the car id to the vector of car id's
/// </summary>
/// <param name="e"></param>
void MotionSystem::receive(const entityx::ComponentAddedEvent<Car>& e)
{
	m_carIds.push_back(std::move(e.entity.id()));
}

/// <summary>
/// @brief receive car component removed event
/// 
/// when car component is removed then 
/// remove it from the vector of car id's
/// </summary>
/// <param name="e"></param>
void MotionSystem::receive(const entityx::ComponentRemovedEvent<Car>& e)
{
	//the begin iterator of carId's
	const auto& begin = m_carIds.begin();
	//the end iterator of carId's
	const auto& end = m_carIds.end();
	//loop through all the cars untill reached last element and erase it
	for (auto itt = begin; itt != end; itt++)
	{
		if ((*itt) == e.entity.id())
		{
			m_carIds.erase(itt);
			break;
		}
	}
}

/// <summary>
/// @brief Update acceleration,velocity and position
/// 
/// This method updates the entities acceleration, velocity and position
/// using the maths formulas :
/// acceleration = -coeffFriction * g * velocity
/// velocity = velocity + acceleration * dt.
/// position = ut + 1/2at^2
/// </summary>
/// <param name="acceleration">acceleration handle</param>
/// <param name="velocity">velocity handle</param>
/// <param name="dt">delta time</param>
void MotionSystem::linearMotion(
	Acceleration::Handle & acceleration
	, Velocity::Handle & velocity
	, Position::Handle & position
	, const float& rotation
	, Car::Handle & car
	, const float & dt
	, bool onRoad)
{
	//cos of rotation
	const float& cosSine = std::cos(thor::toRadian(rotation));
	//sin of rotation
	const float& sine = std::sin(thor::toRadian(rotation));

	//get slowdown applied
	auto& slowDownApplied = car->m_slowDownApplied;
	//get max acceleration
	auto& maxAccel = car->m_maxAccel;
	//get accelerate value
	auto& accelerateVal = car->m_accelerateVal;
	//get position
	sf::Vector2f& pos = position->m_position;

	//If velocity is not 0 then calculate motion
	if (!((velocity->m_velocity.x == 0.0f && velocity->m_velocity.y == 0.0f)))
	{
		//acceleration = -coefficientOfFriction * gravity * unit(velocity).
		acceleration->m_acceleration += -(Physics::getCoefficientOfFriction() * (Physics::getGravity()) * thor::unitVector(velocity->m_velocity));

		//check if acceleration positive or negative
		accelCheck(acceleration, accelerateVal);
		//if car is on road then normal movement
		if (onRoad) 
		{
			//max acceleration is
			maxAccel = 1500.0f;
			slowDownApplied = false;
		}
		//if car is offroad then make the car slower
		else
		{
			//if reverse offroad and slowdown was not applied
			if (accelerateVal < 0 && slowDownApplied == false) 
			{
				//slow down to -100
				accelerateVal = -100.0f;
				//slowdown was now applied
				slowDownApplied = true;
			}
			//if head offroad and slowdown was not applied
			else if (slowDownApplied == false)
			{
				//slow down to 100
				accelerateVal = 100.0f;
				//slowdown was now applied
				slowDownApplied = true;
			}
			//set max accel to be 500
			maxAccel = 500.0f;
		}
		//set velocity to accelerate value
		velocity->m_velocity = sf::Vector2f(accelerateVal * cosSine, accelerateVal * sine);
	}


	//v = u + at
	velocity->m_velocity = velocity->m_velocity + (acceleration->m_acceleration * dt);

	//s = ut + 1/2 at^2
	pos.x += (velocity->m_velocity.x * dt) + ((0.5f) * (acceleration->m_acceleration.x * (dt * dt)));
	pos.y += (velocity->m_velocity.y * dt) + ((0.5f) * (acceleration->m_acceleration.y * (dt * dt)));

}

/// <summary>
/// @brief This is an easing cubic function that eases in a value
/// 
/// This function will ease in from one value to another
/// in a quadratic curvature.
/// </summary>
/// <param name="time">current time</param>
/// <param name="begin">beginning value</param>
/// <param name="change">change between first and final value</param>
/// <param name="duration">total time of ease</param>
/// <returns></returns>
float MotionSystem::easeIn(float time, float begin, float change, float duration)
{

	//ease calculation
	float motion = change * (time /= duration) * time * time + begin;

	//clamp calculation
	if (motion <= change)
	{
		return motion;
	}
	else
	{
		return change;
	}
}

/// <summary>
/// @brief This is an easing cubic function that eases out a value
/// 
/// This function will ease out a value from one value to another
/// in a quadratic curvature
/// </summary>
/// <param name="time">current time</param>
/// <param name="begin">beginning value</param>
/// <param name="change">change between first and final value</param>
/// <param name="duration">total time of ease</param>
/// <returns></returns>
float MotionSystem::easeOut(float time, float begin, float change, float duration)
{
	return change * ((time = time / time - 1) * time * duration + 1) + begin;
}

/// <summary>
/// @brief this is an ease inout cubic function
/// </summary>
/// <param name="t">time</param>
/// <param name="b">begin</param>
/// <param name="c">change</param>
/// <param name="d">duration</param>
/// <returns></returns>
float MotionSystem::easeInOut(float t, float b, float c, float d)
{
	if ((t /= d / 2) < 1) return c / 2 * t*t*t + b;
	const float& motion = c / 2 * ((t -= 2)*t*t + 2) + b;

	//clamp value
	if (1500.0f <= motion)
	{
		return 1500.0f;
	}
	if (motion <= c)
	{
		return motion;
	}
	else
	{
		return c;
	}
}

/// <summary>
/// @brief clamps the steering force
/// 
/// This function will clamp the steering force
/// between the passed in min and max values
/// </summary>
/// <param name="minSteerForce"></param>
/// <param name="maxSteerForce"></param>
void MotionSystem::checkSteering(const float & minSteerForce, const float & maxSteerForce)
{
	//if steer force less than minimum set to minimum
	if (m_steeringForce < minSteerForce)
	{
		m_steeringForce = minSteerForce;
	}
	//else if steering force greater than max and not handbreaking 
	// (in order to allow an increase in rotation when handbreaking)
	//set to max steering
	else if ((m_steeringForce > maxSteerForce) && !m_handBrake)
	{
		m_steeringForce = maxSteerForce;
	}
}

/// <summary>
/// @brief Checks which direction to apply the acceleration
/// 
/// If acceleration value is positive go forward else go back
/// </summary>
/// <param name="accel"></param>
void MotionSystem::accelCheck(Acceleration::Handle& accel, float& accelerateVal)
{
	//if value greater than 0 then go forward
	if (accelerateVal > 0)
	{
		accelerateVal = thor::length(accel->m_acceleration);
	}
	//else go backwards
	else
	{
		accelerateVal = (thor::length(accel->m_acceleration)) * -1;
	}
}

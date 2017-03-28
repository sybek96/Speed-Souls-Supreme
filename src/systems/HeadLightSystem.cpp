#include "systems\HeadLightSystem.h"



HeadLightSystem::HeadLightSystem(sf::RenderWindow& window, std::shared_ptr<ResourceHolders> resources)
	: HALF_LIGHT_WIDTH(300)
	, HALF_LIGHT_HEIGHT(350)
	, m_resources(resources)
	, m_window(window)
	, m_on(false)
{
}

/// <summary>
/// @brief subscribe to all events
/// </summary>
/// <param name="eventManager"></param>
void HeadLightSystem::configure(entityx::EventManager & eventManager)
{
	eventManager.subscribe<entityx::ComponentAddedEvent<Car>>(*this);
	eventManager.subscribe<entityx::ComponentRemovedEvent<Car>>(*this);
	eventManager.subscribe<EvNightTime>(*this);
	eventManager.subscribe<EvDayTime>(*this);
}

void HeadLightSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, double dt)
{
	auto& particleResourceHolder = *(m_resources->m_textures->m_particle);
	auto& shaderResourceHolder = *(m_resources->m_shaders);
	auto& shader = *(shaderResourceHolder[static_cast<int>(Assets::Textures::Particles::Cone)]);

	for (int i = 0; i < m_carIDs.size(); i++)
	{
		entityx::Entity& carEntity = entities.get(m_carIDs[i]);

		Position::Handle& entPosition = carEntity.component<Position>();
		CollisionBox::Handle& entBox = carEntity.component<CollisionBox>();
		sf::Vector2f& position = entPosition->m_position;
		float& rotation = entPosition->m_rotation;
		float positionOffsetX = entBox->m_rectangle.height / 2;
		float positionOffsetY = entBox->m_rectangle.width / 4;

		auto& leftHeadlight = m_leftHeadlight[i];
		auto& rightHeadlight = m_rightHeadlight[i];

		//set shader perameters using our variables
		shader.setParameter("texture", sf::Shader::CurrentTexture);
		shader.setParameter("on", m_on);

		leftHeadlight.setTexture(&(particleResourceHolder[Assets::Textures::Particles::Cone]), true);
		leftHeadlight.setPosition(position.x /*+ positionOffsetX*/, position.y /*- positionOffsetY*/);
		leftHeadlight.setRotation(rotation);

		//asign the colour in the shader of the headlight color 
		shader.setParameter("color", leftHeadlight.getFillColor());

		m_window.draw(leftHeadlight, &shader);

		rightHeadlight.setTexture(&(particleResourceHolder[Assets::Textures::Particles::Cone]), true);
		rightHeadlight.setPosition(position.x /*+ positionOffsetX*/, position.y/* + positionOffsetY*/);
		rightHeadlight.setRotation(rotation);

		shader.setParameter("color", rightHeadlight.getFillColor());

		m_window.draw(rightHeadlight, &shader);
	}

	/*for (int i = 0; i < m_rightHeadlight.size(); i++)
	{
		m_window.draw(m_leftHeadlight[i], &shader);
		m_window.draw(m_rightHeadlight[i], &shader);
	}*/
}

/// <summary>
/// @brief create 2 rectangles for each car
/// 
/// 
/// creates 2 rectangles for each car to represent it as headlight.
/// the origin for each rectangle is set to be the end of the car.
/// </summary>
/// <param name="e">event</param>
void HeadLightSystem::receive(const entityx::ComponentAddedEvent<Car>& e)
{
	const auto& color = sf::Color(255u, 255u, 255u, 25u);
	
	//push back id when a car component is added
	m_carIDs.push_back(std::move(e.entity.id()));
	

	//add a rectangle to leftheadlight vector for each car
	m_leftHeadlight.push_back(sf::RectangleShape(sf::Vector2f(HALF_LIGHT_WIDTH * 2.0f, HALF_LIGHT_HEIGHT * 2.0f)));
	//add a rectangle to rightheadlight vector for each car
	m_rightHeadlight.push_back(sf::RectangleShape(sf::Vector2f(HALF_LIGHT_WIDTH * 2.0f, HALF_LIGHT_HEIGHT * 2.0f)));

	//get the last element of the leftheadlight vector
	auto& lastElementLeft = *(--m_leftHeadlight.end());
	//get the last element of the rightheadlight vector
	auto& lastElementRight = *(--m_rightHeadlight.end());

	//set the lest rectanlge'origin of the leftheadlight vector
	lastElementLeft.setOrigin(HALF_LIGHT_WIDTH-50, -(HALF_LIGHT_HEIGHT - 200));
	lastElementLeft.setFillColor(color);
	//set the lest rectanlge'origin of the right headlight vector
	lastElementRight.setOrigin(HALF_LIGHT_WIDTH+50, -(HALF_LIGHT_HEIGHT - 200));
	lastElementRight.setFillColor(color);

}

void HeadLightSystem::receive(const EvDayTime& e)
{
	m_on = false;
}

void HeadLightSystem::receive(const EvNightTime& e)
{
	m_on = true;
}

/// <summary>
/// @brief destruct the ids of the cars and headlight rectangles
/// 
/// 
/// </summary>
/// <param name="e"></param>
void HeadLightSystem::receive(const entityx::ComponentRemovedEvent<Car>& e)
{
	const auto& beginCar = m_carIDs.begin();
	const auto& endCar = m_carIDs.end();
	
	const auto& beginLeftLight = m_leftHeadlight.begin();
	const auto& beginRightLight = m_rightHeadlight.begin();

	auto ittLeftLight = beginLeftLight;
	auto ittRightLight = beginRightLight;
	for (auto ittCar = beginCar; ittCar != endCar; ittCar++, ittLeftLight++, ittRightLight++)
	{
		if (*(ittCar) == e.entity.id())
		{
			m_carIDs.erase(ittCar);
			m_leftHeadlight.erase(ittLeftLight);
			m_rightHeadlight.erase(ittRightLight);
			break;
		}
	}
}




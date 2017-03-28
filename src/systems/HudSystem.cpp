#include "systems\HudSystem.h"

/// <summary>
/// @brief Hud system constructor.
/// 
/// 
/// </summary>
HudSystem::HudSystem(sf::RenderWindow & window, entityx::EntityManager& entityManager, std::shared_ptr<FontBank> font, std::shared_ptr<sf::View> view)
	: m_window(window)
	, m_entityManager(entityManager)
	, m_font(font)
	, m_view(view)
	, m_minimapBorder(sf::Vector2f(1125.0f, 850.0f))
{
	m_minimapBorder.setPosition(ScreenSize::width() - m_minimapBorder.getSize().x, 100.0f);
	m_minimapBorder.setFillColor(sf::Color::Black);
}

/// <summary>
/// @brief Updates the hud system
/// 
/// 
/// </summary>
/// <param name="entities">EntityX entity manager</param>
/// <param name="events">EntityX event manager</param>
/// <param name="dt">milliseconds between updates</param>
void HudSystem::update(entityx::EntityManager& entities
		  , entityx::EventManager& events
	      , double dt)
{

	for (const entityx::Entity::Id& carId : m_carIds)
	{
		entityx::Entity& carEntity = entities.get(carId);
		const Car::Handle& carComp = carEntity.component<Car>();
		const auto& carLap = carComp->m_lap;

		if (carEntity.has_component<Player>())
		{
			m_currentPlayerLap = carComp->m_lap;
		}

		entityx::Entity& roadEntity = entities.get(carComp->m_roadId);
		const Road::Handle& roadComp = roadEntity.component<Road>();
		const auto& roadNum = roadComp->m_roadNum;

		for (const entityx::Entity::Id& otherCarId : m_carIds)
		{
			if (otherCarId != carId)
			{
				entityx::Entity& otherCarEntity = entities.get(otherCarId);
				const Car::Handle& otherCarComp = otherCarEntity.component<Car>();
				const auto& otherCarLap = otherCarComp->m_lap;

				entityx::Entity& otherRoadEntity = entities.get(otherCarComp->m_roadId);
				const Road::Handle& otherRoadComp = otherRoadEntity.component<Road>();
				const auto& otherRoadNum = otherRoadComp->m_roadNum;

				if (carEntity.has_component<Player>())
				{
					if (carLap == otherCarLap)
					{
						if (roadNum > otherRoadNum) // ahead of ai
						{
							if (m_playerRacePos > 1)
							{
								m_playerRacePos--;
							}
						}
						else if (roadNum < otherRoadNum) // behind ai
						{
							if (m_playerRacePos < m_aiId.size())
							{
								m_playerRacePos++;
							}
						}
					}
				}
			}
		}
	}

	m_currentTime += dt / 1000.f; // convert milliseconds to seconds

								  // update the string of the hud
	m_hud.setString("Current Time : " + std::to_string(static_cast<int>(m_currentTime)) + "s"
		+ "\nCurrent Lap : " + std::to_string(m_currentPlayerLap + 1)
		+ "\nBest Time : " + std::to_string(static_cast<int>(m_bestTime)) + "s"
		+ "\nBest Lap : " + std::to_string(m_bestLap)
		+ "\nRace Position: " + std::to_string(m_playerRacePos));

	// Draw the hud to screen
	m_hud.setPosition(m_view->getCenter().x - 1790, m_view->getCenter().y - 1320);
	m_window.draw(m_hud);

	//Draw the minimap border
	m_minimapBorder.setPosition(m_view->getCenter().x + 590, m_view->getCenter().y - 1290.0f);
	m_window.draw(m_minimapBorder);
}

/// <summary>
/// @brief Configures the scroll system with any events that may happen
/// 
/// 
/// </summary>
/// <param name="eventManager">Event Manager </param>
void HudSystem::configure(entityx::EventManager & eventManager)
{
	eventManager.subscribe<EvAssetsLoaded>(*this);
	eventManager.subscribe<EvLapCompleted>(*this);
	eventManager.subscribe<EvLapUndone>(*this);
	eventManager.subscribe<entityx::ComponentAddedEvent<Player>>(*this);
	eventManager.subscribe<entityx::ComponentRemovedEvent<Player>>(*this);
	eventManager.subscribe<entityx::ComponentAddedEvent<Ai>>(*this);
	eventManager.subscribe<entityx::ComponentRemovedEvent<Ai>>(*this);
	eventManager.subscribe<entityx::ComponentAddedEvent<Car>>(*this);
	eventManager.subscribe<entityx::ComponentRemovedEvent<Car>>(*this);
}

/// <summary>
/// @brief Receives the EvAssetsLoaded event
/// 
/// </summary>
/// <param name="e"></param>
void HudSystem::receive(const EvAssetsLoaded& e) // when there is a load asset event , then you can do this
{ // get the current lap
	const Assets::Fonts::GUI& fontAsset = Assets::Fonts::GUI::Content;
	const auto& font = (*m_font->m_gui);

	switch (e.m_gameState)
	{
	case GameState::Paused:
		break;
	case GameState::Playing:
		m_hud.setFont(font[fontAsset]); // set the font
		m_hud.setCharacterSize(80);// set the font size
		m_hud.setColor(sf::Color::White); // make the color
		m_hud.setPosition(m_view->getCenter().x - 1790, m_view->getCenter().y - 1320); // set the pos of hud
		break;
	default:
		break;
	}
}

/// <summary>
/// @brief Receives the EvLapCompleted event
/// 
/// </summary>
/// <param name="e"></param>
void HudSystem::receive(const EvLapCompleted& e)
{
	entityx::Entity& carEntity = m_entityManager.get(e.m_carId);
	Car::Handle& carComp = carEntity.component<Car>();

	if (e.m_carId == m_playerId) // check if its the player car
	{
		if (m_lapUndone == 0) // if the player didnt go backwards
		{
			carComp->m_lap++; // increment current lap
			//debugMsg(carComp->m_lap);
			if (m_bestTime == 0) // if the player's best time was 0
			{ // assign it new best time and lap based on the current time and lap
				m_bestTime = m_currentTime;
				m_bestLap = carComp->m_lap;
			}
			else if (m_currentTime < m_bestTime) // if you completed a lap and your time is better than the best time
			{
				m_bestLap = carComp->m_lap; // make the current lap top best lap
				m_bestTime = m_currentTime; // make the current time to best time
			}
			m_currentTime = 0; // reset the time
		}
		else
		{
			m_lapUndone--; // decreased the lap undone
		}
		//debugMsg(carComp->m_lap);
	}
	for (int i = 0; i < m_aiId.size(); i++)
	{
		if (e.m_carId == m_aiId.at(i))
		{
			carComp->m_lap++;
		}
	}
	
}
/// <summary>
/// @brief Receives the EvLapUndone event
/// 
/// </summary>
/// <param name="e"></param>
void HudSystem::receive(const EvLapUndone & e)
{
	if (e.m_carId == m_playerId)// check if its the player car
	{
		m_lapUndone++; // increment lap undone
	}
}

/// <summary>
/// @brief Receives the playerId for when a player component is added
/// 
/// </summary>
/// <param name="e"></param>
void HudSystem::receive(const entityx::ComponentAddedEvent<Player>& e)
{
	m_playerId = e.entity.id(); // get the player id
}

/// <summary>
/// @brief Receives the playerId for when a player component is removed
/// 
/// </summary>
/// <param name="e"></param>
void HudSystem::receive(const entityx::ComponentRemovedEvent<Player>& e)
{
	m_playerId = entityx::Entity::Id(); // make it into a default entity id
}

/// <summary>
/// @brief Receives the aiId for when a Ai component is added
/// 
/// </summary>
/// <param name="e"></param>
void HudSystem::receive(const entityx::ComponentAddedEvent<Ai>& e)
{
	m_aiId.push_back(e.entity.id());
	m_aiCurrentLap.push_back(1);
	m_aiRoadNum.push_back(-1);
}

/// <summary>
/// @brief Receives the aiId for when a Ai component is removed
/// 
/// </summary>
/// <param name="e"></param>
void HudSystem::receive(const entityx::ComponentRemovedEvent<Ai>& e)
{
	// erase the ai id that was removed
	const auto& begin = m_aiId.begin();
	const auto& end = m_aiId.end();
	for (auto itt = begin; itt != end; itt++)
	{
		if ((*itt) == e.entity.id())
		{
			m_aiId.erase(itt);
			break;
		}
	}
}
/// <summary>
/// @brief Receives the carId for when a Car component is added
/// 
/// </summary>
/// <param name="e"></param>
void HudSystem::receive(const entityx::ComponentAddedEvent<Car>& e)
{
	m_carIds.push_back(e.entity.id());
}

/// <summary>
/// @brief Receives the carId for when a Car component is removed
/// 
/// </summary>
/// <param name="e"></param>
void HudSystem::receive(const entityx::ComponentRemovedEvent<Car>& e)
{
	// erase the car id that was removed
	const auto& begin = m_carIds.begin();
	const auto& end = m_carIds.end();
	for (auto itt = begin; itt != end; itt++)
	{
		if ((*itt) == e.entity.id())
		{
			m_carIds.erase(itt);
			break;
		}
	}
}
#include "systems\MovieSystem.h"

/// <summary>
/// @brief constructor for a movie system
/// 
/// </summary>
/// <param name="movie"></param>
MovieSystem::MovieSystem(sf::RenderWindow& window, entityx::EventManager& eventManager)
	: m_window(window),
	m_eventManager(eventManager)
{

}

/// <summary>
/// @brief Updates the movie system
/// 
/// 
/// </summary>
/// <param name="entities">EntityX entity manager</param>
/// <param name="events">EntityX event manager</param>
/// <param name="dt">milliseconds between updates</param>
void MovieSystem::update(entityx::EntityManager& entities
	, entityx::EventManager& events
	, double dt)
{
	if (m_gameState == GameState::LicenseScreen)
	{
		if (m_movie.getStatus() == 0) // if movie stopped playing
		{
			m_eventManager.emit<EvChangeGameState>(GameState::SplashScreen); // go to splash screen
		}
	}
	if (m_gameState == GameState::SplashScreen)
	{
		if (m_movie.getStatus() == 0) // if movie stopped playing
		{
			m_eventManager.emit<EvChangeGameState>(GameState::MainMenu); // go to main menu
		}
	}
	if (m_gameState == GameState::Credits)
	{
		if (m_movie.getStatus() == 0) // if movie stopped playing
		{
			m_eventManager.emit<EvChangeGameState>(GameState::MainMenu); // go to main menu
		}
	}
	if (m_gameState == GameState::HelpScreen)
	{
		if (m_movie.getStatus() == 0 && m_indexHelp <= 4) // if movie stopped playing and it didn't through all the instructions
		{
			m_indexHelp++; // play next instructions
			m_eventManager.emit<EvChangeGameState>(GameState::HelpScreen); // go to help
			if (m_indexHelp == 5)
			{
				m_indexHelp = 1;
				m_eventManager.emit<EvChangeGameState>(GameState::MainMenu); // go back to main menu 
			}
		}
	}
		m_movie.update(); // updates the movie
		m_window.draw(m_movie); // draws it to the screen
}

/// <summary>
/// @brief Configures the movie system with any events that may happen
/// 
/// 
/// </summary>
/// <param name="eventManager">Event Manager </param>
void MovieSystem::configure(entityx::EventManager & eventManager)
{
	eventManager.subscribe<EvChangeGameState>(*this);// <-- Change event to load assets
}
/// <summary>
/// @ brief To receive an EvChangeGameState when the gamestate changed
/// </summary>
/// <param name="e">A player adding event</param>
void MovieSystem::receive(const EvChangeGameState& e) // <-- Change event to load assets
{
	m_gameState = e.m_gameState; // get the ref to the gamestate

	m_movie.setPosition(-100, 0); // set the pos of the movie

	switch (e.m_gameState)
	{
	case GameState::LicenseScreen:
		// temporary loading the files
		if (!m_movie.openFromFile("./resources/movies/license.mp4"))
		{
			std::cout << "License Movie didnt Load" << std::endl;
		}
		m_movie.setScale(1, 1.12);
		m_movie.play(); // play the movie once
		break;
	case GameState::SplashScreen:
		if (!m_movie.openFromFile("./resources/movies/splashscreen.mp4"))
		{
			std::cout << "Splash Movie didnt Load" << std::endl;
		}
		m_movie.play(); // play the movie once
		break;
	case GameState::Credits:
		if (!m_movie.openFromFile("./resources/movies/credits.mp4"))
		{
			std::cout << "Credits Movie didnt Load" << std::endl;
		}
		m_movie.setPosition(-15, 0); // set the pos of the movie
		m_movie.setScale(0.85, 1.12);
		m_movie.play(); // play the movie once
		break;
	case GameState::HelpScreen:
	{
		if (m_indexHelp == 1) // Load Acceleration Movie
		{
			if (!m_movie.openFromFile("./resources/movies/Acceleration.mp4"))
			{
				std::cout << "Acceleration Movie didnt Load" << std::endl;
			}
		}
		else if (m_indexHelp == 2) // Load BrakeReverse Movie
		{
			if (!m_movie.openFromFile("./resources/movies/BrakeReverse.mp4"))
			{
				std::cout << "BrakeReverse Movie didnt Load" << std::endl;
			}
		}
		else if (m_indexHelp == 3) // Load HandBraking Movie
		{
			if (!m_movie.openFromFile("./resources/movies/Handbrake.mp4"))
			{
				std::cout << "HandBraking Movie didnt Load" << std::endl;
			}
		}
		else if (m_indexHelp == 4) // Load Turning Movie
		{
			if (!m_movie.openFromFile("./resources/movies/Turning.mp4"))
			{
				std::cout << "Turning Movie didnt Load" << std::endl;
			}
			m_movie.setPosition(-20, 0); // an offset
		}
			
		m_movie.play(); // play the movie once
		break;
	}
	default:
		break;
	}
}

#ifndef MOVIE_SYSTEM_H
#define MOVIE_SYSTEM_H

#include "sfeMovie\Movie.hpp"

#include "entityx\System.h"

#include "events\EvChangeGameState.h"

/// <summary>
/// @author Huong Thi Thanh Huynh
/// @version 1.0
/// @brief Will handle the playing of a video
/// 
/// Using Sfe::Movie
/// </summary>
class MovieSystem
	: public entityx::System<MovieSystem>
	, public entityx::Receiver<MovieSystem>
{
public:
	MovieSystem(sf::RenderWindow& window,
			entityx::EventManager& eventManager);

	void update(entityx::EntityManager& entities
		, entityx::EventManager& events
		, double dt) override;

	void configure(entityx::EventManager & eventManager);
	void receive(const EvChangeGameState& e);

private:

	// hold and play a movie
	sfe::Movie m_movie;
	sf::RenderWindow& m_window;

	// reference to our game state
	GameState m_gameState;

	// reference to our event manager (allows for emission of events)
	entityx::EventManager& m_eventManager;

	// current movie index on help screen
	int m_indexHelp = 1;
};

#endif //!MOVIE_SYSTEM_H

#ifndef CAR_CREATOR_H
#define CAR_CREATOR_H

#include "entities\EntityCreator.h"

#include "components\Display.h"
#include "components\Position.h"
#include "components\Car.h"
#include "components\CollisionBox.h"
#include "components\Acceleration.h"
#include "components\Velocity.h"
#include "components\InputControl.h"
#include "components\Player.h"
#include "components\Ai.h"

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Creates a car entity and assigns it components.
/// 
/// 
/// </summary>
class CarCreator : public EntityCreator
{
public:
	CarCreator(
		const bool& isPlayer
		, const sf::Vector2f& position
		, const float& rotation
		, const sf::IntRect& texel
		, const sf::Vector2f origin
		, const sf::Color& color
		, const Assets::Textures::Car& type);

	void create(entityx::Entity& entity) override;

private:
	sf::Vector2f m_position;
	float m_rotation;
	sf::IntRect m_texel;
	sf::Vector2f m_origin;
	sf::Color m_color;
	Assets::Textures::Car m_type;
	bool m_isPlayer;
};

#endif // !CAR_CREATOR_H
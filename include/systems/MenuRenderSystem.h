#ifndef MENU_RENDER_SYSTEM_H
#define MENU_RENDER_SYSTEM_H

#include "systems\MenuSystem.h"

/// <summary>
/// @author
/// @version
/// @brief
/// 
/// 
/// </summary>
class MenuRenderSystem : public entityx::System<MenuRenderSystem>
{
public:
	MenuRenderSystem(
		std::shared_ptr<GUI> gui
		, sf::RenderWindow& window
		, std::shared_ptr<std::vector<sf::RectangleShape>> textRects
		, std::shared_ptr<sf::Sprite> bgSprite
		,std::shared_ptr<sf::RectangleShape> lbRect
		,std::shared_ptr<sf::RectangleShape> rbRect);

	void update(entityx::EntityManager& entities, entityx::EventManager& events, double dt) override;

private:
	// reference to window
	sf::RenderWindow& m_window;

	// shared pointer to our gui
	std::shared_ptr<GUI> m_gui;

	std::shared_ptr<std::vector<sf::RectangleShape>> m_textureRects;

	std::shared_ptr<sf::Sprite> m_bgSprite;

	// LB and RB rect
	std::shared_ptr<sf::RectangleShape> m_LBRect;
	std::shared_ptr<sf::RectangleShape> m_RBRect;
};

#endif // !MENU_RENDER_SYSTEM_H
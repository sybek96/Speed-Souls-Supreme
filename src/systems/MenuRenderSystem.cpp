#include "systems\MenuRenderSystem.h"

/// <summary>
/// @brief
/// 
/// 
/// </summary>
/// <param name="gui"></param>
/// <param name="window"></param>
MenuRenderSystem::MenuRenderSystem(
	std::shared_ptr<GUI> gui
	, sf::RenderWindow & window
	, std::shared_ptr<std::vector<sf::RectangleShape>> textRects
	, std::shared_ptr<sf::Sprite> bgSprite
	, std::shared_ptr<sf::RectangleShape> lbRect
	, std::shared_ptr<sf::RectangleShape> rbRect)

	: m_window(window)
	, m_gui(gui)
	, m_textureRects(textRects)
	, m_bgSprite(bgSprite)
	, m_LBRect(lbRect)
	, m_RBRect(rbRect)
{

}

/// <summary>
/// @brief
/// 
/// 
/// </summary>
/// <param name="entities"></param>
/// <param name="events"></param>
/// <param name="dt"></param>
void MenuRenderSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, double dt)
{
	m_window.draw(*m_bgSprite);

	m_window.draw(*m_LBRect);
	m_window.draw(*m_RBRect);

	const auto& textureRect = (*m_textureRects);
	for (int i = 0; i < textureRect.size(); i++)
	{
		m_window.draw(textureRect[i]);
	}

	m_gui->draw(m_window);
}

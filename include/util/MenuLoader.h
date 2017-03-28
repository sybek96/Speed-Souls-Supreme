#ifndef MENU_LOADER_H
#define MENU_LOADER_H

#include "yaml-cpp\yaml.h"

#include "SFML\System\Vector2.hpp"
#include "SFML\Graphics\Rect.hpp"
#include "SFML\System\String.hpp"

#include "gui\GUI.h"

#include "util\Debug.h"
#include "util\ResourcePath.h"
#include "util\ResourceTypes.h"
#include "util\GameStates.h"

namespace MenuL
{
	struct TextureData
	{
		std::string m_fileName;
		sf::Vector2f m_origin;
		sf::IntRect m_texel;
	};

	struct FontData
	{
		std::string m_fileName;
		unsigned m_charSize;
	};

	struct WidgetData
	{
		GUI::Widgets m_type;
		sf::Vector2f m_position;
		std::string m_text;
	};

	struct GuiData
	{
		GUI::Layouts m_style;
		std::vector<WidgetData> m_widgets;
	};

	struct MenuData
	{
		GuiData m_gui;
		FontData m_font;
		std::map<Assets::Textures::GUI, TextureData> m_texture;
	};
}

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Static yaml parser.
/// 
/// 
/// </summary>
class MenuLoader
{
public:
	// yaml node extraction operators

	friend void operator >> (const YAML::Node& menuNode, MenuL::MenuData& menuData);
	friend void operator >> (const YAML::Node& guiNode, MenuL::GuiData& guiData);
	friend void operator >> (const YAML::Node& textureNode, MenuL::TextureData& textureData);
	friend void operator >> (const YAML::Node& fontNode, MenuL::FontData& fontData);
	friend void operator >> (const YAML::Node& widgetNode, MenuL::WidgetData& widgetData);

	static bool load(const GameState&, MenuL::MenuData&);

private:
	static std::map<std::string, GUI::Layouts> s_layoutMap;
	static std::map<std::string, GUI::Widgets> s_widgetMap;
};


#endif // !MENU_LOADER_H
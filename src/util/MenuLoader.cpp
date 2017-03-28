#include "util\MenuLoader.h"

std::map<std::string, GUI::Layouts> MenuLoader::s_layoutMap;
std::map<std::string, GUI::Widgets> MenuLoader::s_widgetMap;

/// <summary>
/// @brief YAML Parser operator.
/// 
/// Extracts menu data from the menu yaml node
/// and places it into the passed menu struct
/// </summary>
/// <param name="menuNode">defines the yaml node</param>
/// <param name="guiData">defines the struct</param>
void operator >> (const YAML::Node & menuNode, MenuL::MenuData & menuData)
{
	const YAML::Node& textureNode = menuNode["texture"];
	const int& button = static_cast<int>(Assets::Textures::GUI::Button);
	const int& checkbox = static_cast<int>(Assets::Textures::GUI::Checkbox);
	const int& slider = static_cast<int>(Assets::Textures::GUI::Slider);
	const int& background = 4;
	textureNode["button"] >> menuData.m_texture[Assets::Textures::GUI::Button];
	textureNode["checkbox"] >> menuData.m_texture[Assets::Textures::GUI::Checkbox];
	textureNode["slider"] >> menuData.m_texture[Assets::Textures::GUI::Slider];
	textureNode["background"] >> menuData.m_texture[Assets::Textures::GUI::Background];

	menuNode["font"] >> menuData.m_font;
	menuNode["gui"] >> menuData.m_gui;
}

/// <summary>
/// @brief YAML Parser operator.
/// 
/// Extracts gui data from the gui yaml node
/// and places it into the passed gui struct
/// </summary>
/// <param name="guiNode">defines the yaml node</param>
/// <param name="guiData">defines the struct</param>
void operator >> (const YAML::Node & guiNode, MenuL::GuiData & guiData)
{
	const std::string& style = guiNode["style"].as<std::string>();
	guiData.m_style = MenuLoader::s_layoutMap[style];
	
	const YAML::Node& elementsNode = guiNode["elements"];
	const unsigned& NODE_SIZE = elementsNode.size();
	for (unsigned i = 0u; i < NODE_SIZE; i++)
	{
		MenuL::WidgetData widgetData;
		elementsNode[i] >> widgetData;
		guiData.m_widgets.push_back(std::move(widgetData));
	}
}

/// <summary>
/// @brief YAML Parser operator.
/// 
/// Extracts texture data from the texture yaml node
/// and places it into the passed texture struct
/// </summary>
/// <param name="textureNode">defines the yaml node</param>
/// <param name="textureData">defines the struct</param>
void operator >> (const YAML::Node& textureNode, MenuL::TextureData& textureData)
{
	textureData.m_fileName = resourcePath() + textureNode["file"].as<std::string>();

	auto& rectData = textureData.m_texel;
	const YAML::Node& rectNode = textureNode["rectangles"][0]["coord"];

	rectData.left = rectNode["left"].as<int>();
	rectData.top = rectNode["top"].as<int>();
	rectData.width = rectNode["width"].as<int>();
	rectData.height = rectNode["height"].as<int>();

	textureData.m_origin.x = rectData.width / 2.0f;
	textureData.m_origin.y = rectData.height / 2.0f;
}

/// <summary>
/// @brief YAML Parser operator.
/// 
/// Extracts font data from the font yaml node
/// and places it into the passed font struct
/// </summary>
/// <param name="fontNode">defines the yaml node</param>
/// <param name="fontData">defines the struct</param>
void operator >> (const YAML::Node & fontNode, MenuL::FontData & fontData)
{
	const YAML::Node& contentNode = fontNode["content"];
	fontData.m_fileName = resourcePath() + contentNode["file"].as<std::string>();
	fontData.m_charSize = contentNode["char_size"].as<unsigned>();
}

/// <summary>
/// @brief YAML Parser operator.
/// 
/// Extracts widget data from the widget yaml node
/// and places it into the passed widget struct
/// </summary>
/// <param name="widgetNode">defines the yaml node</param>
/// <param name="widgetData">defines the struct</param>
void operator >> (const YAML::Node & widgetNode, MenuL::WidgetData & widgetData)
{
	const std::string& type = widgetNode["type"].as<std::string>();
	widgetData.m_type = MenuLoader::s_widgetMap[type];
	widgetData.m_position.x = widgetNode["position"]["x"].as<float>();
	widgetData.m_position.y = widgetNode["position"]["y"].as<float>();
	widgetData.m_text = widgetNode["text"].as<std::string>();
}

/// <summary>
/// @brief Loads all menu data.
/// 
/// 
/// </summary>
/// <param name="gui">defines a shared pointer to gui container</param>
/// <param name="guiData">reference to gui data struct that parsed data will be stored in</param>
/// <returns>true, if all data is loaded successfully, else false</returns>
bool MenuLoader::load(const GameState& state, MenuL::MenuData& menuData)
{
	std::string type;

	switch (state)
	{
	case GameState::LicenseScreen:
		type = "license_screen";
		break;
	case GameState::SplashScreen:
		type = "splash_screen";
		break;
	case GameState::Playing:
	case GameState::MainMenu:
		type = "main_menu";
		break;
	case GameState::OptionsScreen:
		type = "options_screen";
		break;
	case GameState::CarSelection:
		type = "car_selection";
		break;
	case GameState::TrackSelection:
		type = "track_selection";
		break;
	case GameState::Upgrade:
		type = "upgrade";
		break;
	default:
		break;
	}

	//type = "main_menu";

	std::stringstream guiSS;
	
	guiSS << resourcePath();
	guiSS << "yaml/gui/";
	guiSS << type;
	guiSS << ".yaml";

	try
	{
		YAML::Node menuNode = YAML::LoadFile(guiSS.str());
		if (menuNode.IsNull())
		{
			std::string msg("file: " + guiSS.str() + " not found");
			throw std::exception(msg.c_str());
		}

		menuNode[type.c_str()] >> menuData;

	}
	catch (YAML::ParserException& e)
	{
		std::string s("Error:\n ");
		s += "Occured in ";
		s += typeid(MenuLoader).name();
		debugMsg(s.c_str(), e.what());
		return false;
	}
	catch (const std::exception& e)
	{
		std::string s("Error:\n ");
		s += "Occured in ";
		s += typeid(MenuLoader).name();
		debugMsg(s.c_str(), e.what());
		return false;
	}

	return true;
}

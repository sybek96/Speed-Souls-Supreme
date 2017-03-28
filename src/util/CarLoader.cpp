#include "util\CarLoader.h"

/// <summary>
/// sound enum map between sound string type
/// and its corresponding enum
/// </summary>
std::map<std::string, Assets::Sounds::Engine> CarLoader::s_soundEnumMap;

/// <summary>
/// @brief YAML Parser operator.
/// 
/// Extracts car data from the car yaml node
/// and places it into the passed car struct
/// </summary>
/// <param name="carNode">defines the yaml node</param>
/// <param name="carData">defines the struct</param>
void operator >> (const YAML::Node& carNode, CarL::CarData& carData)
{
	carData.m_position.x = carNode["position"]["x"].as<float>();
	carData.m_position.y = carNode["position"]["y"].as<float>();

	carNode["texture"] >> carData.m_texture;

	const YAML::Node& soundNode = carNode["sound"];

	const auto& nodeSize = soundNode.size();

	for (unsigned i = 0u; i < nodeSize; i++)
	{
		CarL::SoundData soundData;
		soundNode[i] >> soundData;
		carData.m_sounds.push_back(std::move(soundData));
	}
}

/// <summary>
/// @brief YAML Parser operator.
/// 
/// Extracts texture data from the texture yaml node
/// and places it into the passed texture struct
/// </summary>
/// <param name="textureNode">defines the texture yaml node</param>
/// <param name="textureData">defines the struct</param>
void operator >> (const YAML::Node& textureNode, CarL::TextureData& textureData)
{
	textureData.m_fileName = (resourcePath() + textureNode["file"].as<std::string>());

	textureData.m_origin.x = textureNode["origin"]["x"].as<float>();
	textureData.m_origin.y = textureNode["origin"]["y"].as<float>();

	const YAML::Node& rectNode = textureNode["rectangles"];

	const auto& nodeSize = textureNode.size() + 1u;
	for (unsigned i = 0u; i < nodeSize; i++)
	{
		sf::IntRect coord;
		rectNode[i]["coord"] >> coord;
		textureData.m_texels.push_back(std::move(coord));
	}
}

/// <summary>
/// @brief YAML Parser operator.
/// 
/// Extracts rectangle data from the rectangle yaml node
/// and places it into the passed sfml rect
/// </summary>
/// <param name="rectNode">defines the rectangle yaml node</param>
/// <param name="rectData">defines the sfml rect</param>
void operator >> (const YAML::Node& rectNode, sf::IntRect& rectData)
{
	rectData.left = rectNode["left"].as<int>();
	rectData.top = rectNode["top"].as<int>();
	rectData.width = rectNode["width"].as<int>();
	rectData.height = rectNode["height"].as<int>();
}

/// <summary>
/// @brief YAML Parser operator.
/// 
/// Extracts sound data from sound yaml node
/// and places it into the passed sound struct
/// </summary>
/// <param name="soundNode">defines the sound yaml node</param>
/// <param name="soundData">defines the sound struct</param>
void operator >> (const YAML::Node & soundNode, CarL::SoundData & soundData)
{
	soundData.m_fileName = (resourcePath() + soundNode["file"].as<std::string>());
	const std::string& type = soundNode["type"].as<std::string>();
	soundData.m_type = CarLoader::s_soundEnumMap[type];
}

/// <summary>
/// @brief Loads all car data.
/// 
/// 
/// </summary>
/// <param name="carNum">defines which car's data to load (1 to 6)</param>
/// <param name="carData">reference to car data struct that parsed data will be stored in</param>
/// <returns>true, if all data is loaded successfully, else false</returns>
bool CarLoader::load(const int& carNum, CarL::CarData& carData)
{
	if (s_soundEnumMap.size() <= 0)
	{
		s_soundEnumMap["idle"] = Assets::Sounds::Engine::Idle;
		s_soundEnumMap["start"] = Assets::Sounds::Engine::Start;
	}

	std::string carN("car");
	carN += std::to_string(carNum);

	std::stringstream carNumSS;

	carNumSS << resourcePath();
	carNumSS << "yaml/cars/";
	carNumSS << carN;
	carNumSS << "/";
	carNumSS << carN;
	carNumSS << ".yaml";

	try
	{
		YAML::Node carNode = YAML::LoadFile(carNumSS.str());
		if (carNode.IsNull())
		{
			std::string msg("file: " + carNumSS.str() + " not found");
			throw std::exception(msg.c_str());
		}
		
		carNode[carN.c_str()] >> carData;
		
	}
	catch (YAML::ParserException& e)
	{
		std::string s("Error:\n ");
		s += "Occured in ";
		s += typeid(CarLoader).name();
		debugMsg(s.c_str(), e.what());
		return false;
	}
	catch (const std::exception& e)
	{
		std::string s("Error:\n ");
		s += "Occured in ";
		s += typeid(CarLoader).name();
		debugMsg(s.c_str(), e.what());
		return false;
	}

	return true;
}
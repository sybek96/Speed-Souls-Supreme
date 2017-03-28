#ifndef CAR_LOADER_H
#define CAR_LOADER_H

#include "yaml-cpp\yaml.h"

#include "SFML\System\Vector2.hpp"
#include "SFML\Graphics\Rect.hpp"
#include "SFML\System\String.hpp"

#include "util\Debug.h"
#include "util\ResourcePath.h"
#include "util\ResourceTypes.h"

namespace CarL
{
	// Data structs

	struct SoundData
	{
		std::string m_fileName;
		Assets::Sounds::Engine m_type;
	};

	struct TextureData
	{
		std::string m_fileName;
		sf::Vector2f m_origin;
		std::vector<sf::IntRect> m_texels;
	};

	struct CarData
	{
		sf::Vector2f m_position;
		TextureData m_texture;
		std::vector<SoundData> m_sounds;
	};
}

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Load car details from yaml file.
/// 
/// Purely static class that parses yaml node,
/// into Data structs
/// </summary>
class CarLoader
{
public:
	// yaml node extraction operators

	friend void operator >> (const YAML::Node& carNode, CarL::CarData& carData);
	friend void operator >> (const YAML::Node& textureNode, CarL::TextureData& textureData);
	friend void operator >> (const YAML::Node& rectNode, sf::IntRect& rectData);
	friend void operator >> (const YAML::Node& soundNode, CarL::SoundData& soundData);

	static bool load(const int& carNum, CarL::CarData& carData);

private:
	static std::map<std::string, Assets::Sounds::Engine> s_soundEnumMap;

	CarLoader() {}
	~CarLoader() {}
};

#endif // !CAR_LOADER_H
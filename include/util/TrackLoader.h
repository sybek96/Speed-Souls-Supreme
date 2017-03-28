#ifndef TRACK_LOADER_H
#define TRACK_LOADER_H

#include "yaml-cpp\yaml.h"

#include "util\Debug.h"
#include "util\ResourcePath.h"
#include "util\ResourceTypes.h"

/// <summary>
/// @brief Track Loader data struct namespace.
/// 
/// 
/// </summary>
namespace TrackL
{
	struct TextureData
	{
		std::string m_prevFileName;
		std::string m_bgFileName;
		std::string m_fileName;
		std::map<Assets::Textures::Track, sf::IntRect> m_texels;
	};

	struct RoadData
	{
		Assets::Textures::Track m_type;
	};

	struct TrackData
	{
		float m_heading;
		TextureData m_texture;
		std::vector<RoadData> m_roadNodes;
	};
}

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Load track details from yaml file.
/// 
/// Purely static class that parses yaml node,
/// into Data structs
/// </summary>
class TrackLoader
{
public:
	// yaml node extraction operators

	friend void operator >> (const YAML::Node& trackNode, TrackL::TrackData& trackData);
	friend void operator >> (const YAML::Node& roadNode, TrackL::RoadData& roadData);
	friend void operator >> (const YAML::Node& textureNode, TrackL::TextureData& textureData);

	static bool load(const int& trackNum, TrackL::TrackData& trackData);

private:
	static std::map<std::string, Assets::Textures::Track> s_trackEnumMap;
};
#endif // !TRACK_LOADER_H
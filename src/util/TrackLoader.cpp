#include "util\TrackLoader.h"

/// <summary>
/// track enum map between track string type
/// and its corresponding enum
/// </summary>
std::map<std::string, Assets::Textures::Track> TrackLoader::s_trackEnumMap;

/// <summary>
/// @brief YAML Parser operator.
/// 
/// Extracts track data from the track yaml node
/// and places it into the passed track struct
/// </summary>
/// <param name="trackNode">defines the yaml node</param>
/// <param name="trackData">defines the struct</param>
void operator >> (const YAML::Node & trackNode, TrackL::TrackData & trackData)
{
	trackData.m_heading = trackNode["rotation"].as<float>();

	const YAML::Node& roadNodes = trackNode["roads"];

	trackNode["texture"] >> trackData.m_texture;
	
	for (unsigned i = 0u; i < roadNodes.size(); i++)
	{
		TrackL::RoadData roadData;
		roadNodes[i] >> roadData;
		trackData.m_roadNodes.push_back(std::move(roadData));
	}
}

/// <summary>
/// @brief YAML Parser operator.
/// 
/// Extracts road data from the road yaml node
/// and places it into the passed road struct
/// </summary>
/// <param name="roadNode">defines the yaml node</param>
/// <param name="roadData">defines the struct</param>
void operator >> (const YAML::Node & roadNode, TrackL::RoadData & roadData)
{
	const std::string& type = roadNode["type"].as<std::string>();
	roadData.m_type = TrackLoader::s_trackEnumMap[type];
}

/// <summary>
/// @brief YAML Parser operator.
/// 
/// Extracts texture data from the texture yaml node
/// and places it into the passed texture struct
/// </summary>
/// <param name="textureNode">defines the yaml node</param>
/// <param name="textureData">defines the struct</param>
void operator >> (const YAML::Node& textureNode, TrackL::TextureData& textureData)
{
	textureData.m_prevFileName = (resourcePath() + textureNode["preview"].as<std::string>());
	textureData.m_bgFileName = (resourcePath() + textureNode["background"].as<std::string>());
	textureData.m_fileName = (resourcePath() + textureNode["file"].as<std::string>());

	const YAML::Node& textRectNode = textureNode["rectangles"];

	for (unsigned i = 0u; i < textRectNode.size(); i++)
	{
		const YAML::Node& rectNode = textRectNode[i]["coord"];
		const std::string& typeS = textRectNode[i]["type"].as<std::string>();
		const Assets::Textures::Track& type = TrackLoader::s_trackEnumMap[typeS];

		sf::IntRect rect;
		rect.left = rectNode["left"].as<int>();
		rect.top = rectNode["top"].as<int>();
		rect.width = rectNode["width"].as<int>();
		rect.height = rectNode["height"].as<int>();

		textureData.m_texels[type] = std::move(rect);
	}
}

/// <summary>
/// @brief Loads all track data.
/// 
/// 
/// </summary>
/// <param name="trackNum"></param>
/// <param name="trackData"></param>
/// <returns></returns>
bool TrackLoader::load(const int & trackNum, TrackL::TrackData & trackData)
{
	if (s_trackEnumMap.size() <= 0)
	{
		s_trackEnumMap["road_check"] = Assets::Textures::Track::CheckStart;
		s_trackEnumMap["road_straight"] = Assets::Textures::Track::Straight;
		s_trackEnumMap["road_curve_left"] = Assets::Textures::Track::CurveLeft;
		s_trackEnumMap["road_curve_right"] = Assets::Textures::Track::CurveRight;
		s_trackEnumMap["road_long_curve_left"] = Assets::Textures::Track::CurveLongLeft;
		s_trackEnumMap["road_long_curve_right"] = Assets::Textures::Track::CurveLongRight;
	}

	std::string trackN("track");
	trackN += std::to_string(trackNum);

	std::stringstream trackSS;
	trackSS << resourcePath();
	trackSS << "yaml/tracks/";
	trackSS << trackN;
	trackSS << "/track.yaml";

	try
	{
		YAML::Node trackNode = YAML::LoadFile(trackSS.str());

		if (trackNode.IsNull())
		{
			std::string msg("file: " + trackN + " not found");
			throw std::exception(msg.c_str());
		}

		trackNode[trackN.c_str()] >> trackData;

	}
	catch (YAML::ParserException& e)
	{
		std::string s("Error:\n ");
		s += "Occured in ";
		s += typeid(TrackLoader).name();
		debugMsg(s.c_str(), e.what());
		return false;
	}
	catch (const std::exception& e)
	{
		std::string s("Error:\n ");
		s += "Occured in ";
		s += typeid(TrackLoader).name();
		debugMsg(s.c_str(), e.what());
		return false;
	}


	return true;
}

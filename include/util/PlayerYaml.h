#ifndef PLAYER_YAML_H
#define PLAYER_YAML_H

#include <iostream>
#include <fstream>

#include "yaml-cpp\yaml.h"

#include "Debug.h"
#include "ResourcePath.h"
#include "ResourceTypes.h"

struct StatsData
{
	// top speed of the car
	int m_engine;
	// determines "brake-ability" of car
	int m_wheel;
	// determines how effective it can turn
	int m_handling;
};

/// <summary>
/// @brief Player data struct.
/// 
/// 
/// </summary>
struct PlayerData
{

	PlayerData()
		: m_carSelected()
		, m_tracksCompleted(0)
		, m_trackSelected(-1)
	{}

	Assets::Textures::Car m_carSelected;
	int m_trackSelected;
	int m_tracksCompleted;
	StatsData m_stats;
};

/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Handles yaml parsing the player's progress.
/// 
/// Purely static functions that handle saving and loading,
/// player's progress to and from a yaml file.
/// </summary>
class PlayerYaml
{
public:
	friend void operator >>(const YAML::Node&, PlayerData&);
	friend void operator <<(YAML::Node&, const PlayerData&);

	static bool load(PlayerData& playerData);
	static bool save(const PlayerData& playerData);

private:
	static void orderEmitterFromYaml(const YAML::Node& node, YAML::Emitter& emitter);
	static void writeNode(const YAML::Node& node, YAML::Emitter& emitter);

	static std::stringstream returnPlayerPath();

	static std::map<std::string, Assets::Textures::Car> s_carSelectionMap;
	static std::stringstream s_playerFile;
};

#endif // !PLAYER_YAML_H
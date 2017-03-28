#include "util\PlayerYaml.h"

// basic string to enum map
std::map<std::string, Assets::Textures::Car> PlayerYaml::s_carSelectionMap;

std::stringstream PlayerYaml::s_playerFile = std::move(PlayerYaml::returnPlayerPath());

/// <summary>
/// @brief Extracts from node into data.
/// 
/// Parses contents of player node into player data struct
/// </summary>
/// <param name="playerNode">defines player yaml node</param>
/// <param name="playerData">defines the player data struct</param>
void operator >> (const YAML::Node& playerNode, PlayerData& playerData)
{
	const std::string& carSelected = playerNode["car-selected"].as<std::string>();
	playerData.m_carSelected = PlayerYaml::s_carSelectionMap[carSelected];

	const YAML::Node& statsNode = playerNode["stats"];
	StatsData& statsData = playerData.m_stats;

	statsData.m_engine = statsNode["engine"].as<int>();
	statsData.m_handling = statsNode["handling"].as<int>();
	statsData.m_wheel = statsNode["wheel"].as<int>();

	playerData.m_tracksCompleted = playerNode["tracks-completed"].as<int>();
	playerData.m_trackSelected = playerNode["tracks-selected"].as<int>();
}

/// <summary>
/// @brief Inserts data into node.
/// 
/// Inserts player data into player node
/// </summary>
/// <param name="playerNode">defines player yaml node</param>
/// <param name="playerData">defines the player data struct</param>
void operator << (YAML::Node& playerNode, const PlayerData& playerData)
{
	const auto& carSelection = PlayerYaml::s_carSelectionMap;
	std::string carSelected;

	for (const auto& mapKey : carSelection)
	{
		if (mapKey.second == playerData.m_carSelected)
		{
			carSelected = mapKey.first;
			break;
		}
	}

	playerNode["car-selected"] = carSelected;

	YAML::Node& statsNode = playerNode["stats"];
	const StatsData& statsData = playerData.m_stats;

	statsNode["engine"] = statsData.m_engine;
	statsNode["handling"] = statsData.m_handling;
	statsNode["wheel"] = statsData.m_wheel;

	playerNode["tracks-completed"] = playerData.m_tracksCompleted;
	playerNode["tracks-selected"] = playerData.m_trackSelected;
}

/// <summary>
/// @brief Parses player yaml file.
/// 
/// Extract player data from player's yaml file
/// </summary>
/// <param name="playerData">defines the data extracted</param>
/// <returns>returns true if operation successful, else exception thrown</returns>
bool PlayerYaml::load(PlayerData & playerData)
{
	if (s_carSelectionMap.size() <= 0)
	{
		s_carSelectionMap["silver"] = Assets::Textures::Car::Silver;
		s_carSelectionMap["blue"] = Assets::Textures::Car::Blue;
		s_carSelectionMap["yellow"] = Assets::Textures::Car::Yellow;
		s_carSelectionMap["red"] = Assets::Textures::Car::Red;
		s_carSelectionMap["none"] = Assets::Textures::Car::Last;
	}

	std::string player("player");
	const std::stringstream& playerSS = s_playerFile;

	try
	{
		YAML::Node playerNode = YAML::LoadFile(playerSS.str());
		if (playerNode.IsNull())
		{
			std::string msg("file: " + playerSS.str() + " not found");
			throw std::exception(msg.c_str());
		}
		
		// load yaml node content into player data
		playerNode[player.c_str()] >> playerData;

	}
	catch (YAML::ParserException& e)
	{
		std::string s("Error:\n ");
		s += "Occured in ";
		s += typeid(PlayerYaml).name();
		debugMsg(s.c_str(), e.what());
		return false;
	}
	catch (const std::exception& e)
	{
		std::string s("Error:\n ");
		s += "Occured in ";
		s += typeid(PlayerYaml).name();
		debugMsg(s.c_str(), e.what());
		return false;
	}

	return true;
}

bool PlayerYaml::save(const PlayerData & playerData)
{
	const std::stringstream& playerSS = s_playerFile;

	try
	{
		YAML::Node playerNode = YAML::LoadFile(playerSS.str());
		if (playerNode.IsNull())
		{
			std::string msg("file: " + playerSS.str() + " not found");
			throw std::exception(msg.c_str());
		}

		// save player data into yaml node content
		playerNode["player"] << playerData;

		YAML::Emitter playerEmitter;

		orderEmitterFromYaml(playerNode, playerEmitter);

		std::ofstream saveFile;
		saveFile.open(playerSS.str().c_str());
		if (saveFile.is_open())
		{
			saveFile << playerEmitter.c_str();
			saveFile.close();
		}
		else
		{
			std::string msg("file: " + playerSS.str() + " failed to save");
			throw std::exception(msg.c_str());
		}
	}
	catch (YAML::ParserException& e)
	{
		std::string s("Error:\n ");
		s += "Occured in ";
		s += typeid(PlayerYaml).name();
		debugMsg(s.c_str(), e.what());
		return false;
	}
	catch (const std::exception& e)
	{
		std::string s("Error:\n ");
		s += "Occured in ";
		s += typeid(PlayerYaml).name();
		debugMsg(s.c_str(), e.what());
		return false;
	}

	return true;
}

/// <summary>
/// @brief Alters emitter to contain ordered yaml nodes.
/// 
/// 
/// </summary>
/// <param name="node">parsed yaml node</param>
/// <param name="emitter">defines emitter to be changed</param>
void PlayerYaml::orderEmitterFromYaml(const YAML::Node& node, YAML::Emitter& emitter)
{
	writeNode(node, emitter);
	debugMsg(emitter.c_str());
}

/// <summary>
/// @brief Yaml ordered saving workaround.
/// 
/// This is in fact a known issue of yaml-cpp library, however there's a simple yet effective workaround
/// https://github.com/jbeder/yaml-cpp/issues/169#issuecomment-219763811
/// This workaround has the YAML emitter contain all the yaml nodes in alphabetical order
/// </summary>
/// <param name="node">parsed yaml node</param>
/// <param name="emitter">yaml emitter</param>
void PlayerYaml::writeNode(const YAML::Node & node, YAML::Emitter & emitter)
{
	switch (node.Type())
	{
	case YAML::NodeType::Sequence:
	{
		emitter << YAML::BeginSeq;
		for (size_t i = 0; i < node.size(); i++)
		{
			writeNode(node[i], emitter);
		}
		emitter << YAML::EndSeq;
		break;
	}
	case YAML::NodeType::Map:
	{
		emitter << YAML::BeginMap;

		// First collect all the keys
		std::vector<std::string> keys(node.size());
		int key_it = 0;
		for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
		{
			keys[key_it++] = it->first.as<std::string>();
		}

		// Then sort them
		std::sort(keys.begin(), keys.end());

		// Then emit all the entries in sorted order.
		for (size_t i = 0; i < keys.size(); i++)
		{
			emitter << YAML::Key;
			emitter << keys[i];
			emitter << YAML::Value;
			writeNode(node[keys[i]], emitter);
		}
		emitter << YAML::EndMap;
		break;
	}
	default:
		emitter << node;
		break;
	}
}

/// <summary>
/// @brief Player path string stream.
/// 
/// 
/// </summary>
/// <returns>returns string stream to player's yaml file path</returns>
std::stringstream PlayerYaml::returnPlayerPath()
{
	std::stringstream playerSS;

	playerSS << resourcePath();
	playerSS << "yaml/player/player.yaml";
	return playerSS;
}

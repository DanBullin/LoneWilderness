/*! \file settings.h
*
* \brief This class handles the player's personal settings for aspects which are configurable
*
* \author Daniel Bullin
*
*/
#include "settings/settings.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/log.h"

namespace Engine
{
	std::map<PlayerConfig::ConfigData, std::pair<uint32_t, uint32_t>> Settings::s_keyBinds;

	//! Settings()
	Settings::Settings()
	{
	}

	//! ~Settings()
	Settings::~Settings()
	{
	}

	//! loadFromFile()
	void Settings::loadFromFile()
	{
		nlohmann::json jsonData = ResourceManager::getJSON("assets/keybinds.json");

		for (auto& keybind : jsonData["keybinds"])
		{
			bindKey(PlayerConfig::toType(keybind["function"].get<std::string>()), keybind["key1"].get<uint32_t>(), keybind["key2"].get<uint32_t>());
		}
	}

	//! bindKey()
	/*!
	\param function a const PlayerConfig::ConfigData - The function to bind to a key (enum)
	\param key1 a const uint32_t - The key to bind
	\param key2 a const uint32_t - The key to bind
	*/
	void Settings::bindKey(const PlayerConfig::ConfigData function, const uint32_t key1, const uint32_t key2)
	{
		s_keyBinds[function] = { key1, key2 };
	}

	//! getKeys()
	/*!
	\param function a const PlayerConfig::ConfigData - The function to check if the keys for it as being pressed
	\return a std::pair<uint32_t, uint32_t> - The pair of keys bound to this function
	*/
	std::pair<uint32_t, uint32_t> Settings::getKeys(const PlayerConfig::ConfigData function)
	{
		if (s_keyBinds.find(function) != s_keyBinds.end())
			return s_keyBinds[function];
		else
			return { 0, 0 };
	}
}
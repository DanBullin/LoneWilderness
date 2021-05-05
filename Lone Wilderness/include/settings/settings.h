/*! \file settings.h
*
* \brief This class handles the player's personal settings for aspects which are configurable
*
* \author Daniel Bullin
*
*/
#ifndef SETTINGS_H
#define SETTINGS_H

#include <inttypes.h>
#include <json.hpp>
#include <map>

namespace Engine
{
	namespace PlayerConfig
	{
		/*! \enum ConfigData
		* \brief The different types of configurable data for the player
		*/
		enum ConfigData
		{
			None, MoveForward, MoveLeft, MoveRight, MoveBack, Pause, FullscreenWindow, Hotbar1, Hotbar2, Hotbar3, Hotbar4, Hotbar5, Inventory
		};

		//! toType()
		/*!
		\param configType a const std::string& - The config type as a string
		\return a const ConfigData - The config type
		*/
		static const ConfigData toType(const std::string& configType)
		{
			if (configType == "MoveForward") return ConfigData::MoveForward;
			if (configType == "MoveLeft") return ConfigData::MoveLeft;
			if (configType == "MoveRight") return ConfigData::MoveRight;
			if (configType == "MoveBack") return ConfigData::MoveBack;
			if (configType == "Pause") return ConfigData::Pause;
			if (configType == "FullscreenWindow") return ConfigData::FullscreenWindow;
			if (configType == "Hotbar1") return ConfigData::Hotbar1;
			if (configType == "Hotbar2") return ConfigData::Hotbar2;
			if (configType == "Hotbar3") return ConfigData::Hotbar3;
			if (configType == "Hotbar4") return ConfigData::Hotbar4;
			if (configType == "Hotbar5") return ConfigData::Hotbar5;
			if (configType == "Inventory") return ConfigData::Inventory;
			else return ConfigData::None;
		}
	}

	/*! \class Settings
	* \brief A class which handles the player's personal settings for aspects which are configurable
	*/
	class Settings
	{
	private:
		static std::map<PlayerConfig::ConfigData, std::pair<uint32_t, uint32_t>> s_keyBinds; //!< The available keybinds
	public:
		Settings(); //!< Constructor
		~Settings(); //!< ~Destructor

		static void loadFromFile(); //!< Load the settings stored in file
		static void bindKey(const PlayerConfig::ConfigData function, const uint32_t key1, const uint32_t key2); //!< Bind a key to a function
		static std::pair<uint32_t, uint32_t> getKeys(const PlayerConfig::ConfigData function); //!< Get the keys bound to the function
	};
}
#endif
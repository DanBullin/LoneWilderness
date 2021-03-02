/*! \file fontManager.cpp
*
* \brief A system which loads and manages fonts
*
* \author Daniel Bullin
*
*/
#include "independent/systems/systems/fontManager.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/log.h"

namespace Engine
{
	bool FontManager::s_enabled = false; //!< Is this system enabled
	std::map<std::string, Font*> FontManager::s_fontsList = std::map<std::string, Font*>(); //!< Initialise with empty list
	FT_Library FontManager::s_freetype; //!< The ft library
	unsigned char FontManager::s_firstGlyph = 0; //!< The first char (ASCII)
	unsigned char FontManager::s_lastGlyph = 0; //!< The last char (ASCII)

	//! FontManager
	FontManager::FontManager() : System(SystemType::FontManager)
	{
	}

	//! ~FontManager
	FontManager::~FontManager()
	{
	}

	//! start()
	void FontManager::start()
	{
		// Start system if its disabled
		if (!s_enabled)
		{
			ENGINE_INFO("[FontManager::start] Starting the font manager.");

			//Init freetype
			if (FT_Init_FreeType(&s_freetype))
				ENGINE_ERROR("[FontManager::start] Cannot initialise the FreeType library.");

			// Set the first and last chars to load (ASCII values)
			s_firstGlyph = 32;
			s_lastGlyph = 126;
			
			s_enabled = true;

			// Load all fonts stored in config file
			nlohmann::json configData = ResourceManager::getJSON("assets/config.json")["fonts"];
			for (auto& font : configData)
				FontManager::loadFont(font["name"].get<std::string>(), font["filePath"].get<std::string>(), font["characterSize"]);
		}
	}

	//! stop()
	void FontManager::stop()
	{
		// Stop system if its enabled
		if (s_enabled)
		{
			ENGINE_INFO("[FontManager::stop] Stopping the font manager.");

			if (s_fontsList.size() > 0)
			{
				for (auto& fontElement : s_fontsList)
				{
					if(fontElement.second)
						delete fontElement.second;
				}

				s_fontsList.clear();
			}

			s_enabled = false;
		}
	}

	//! loadFont()
	/*!
	\param fontName a const std::string& - The name of the font
	\param fontFilePath a const std::string& - The file path of the font
	\param pixelSize a const uint32_t - The size of the font in pixel size
	*/
	void FontManager::loadFont(const std::string& fontName, const std::string& fontFilePath, const uint32_t pixelSize)
	{
		if (s_enabled)
		{
			// Check if the font name is already taken
			if (s_fontsList.find(fontName) == s_fontsList.end() && fontName != "" && fontFilePath != "")
			{
				// Font name is free, so load the font
				Font* newFont = new Font(s_freetype, fontName.c_str(), fontFilePath.c_str(), pixelSize, s_firstGlyph, s_lastGlyph);

				// If the font was not loaded correctly, it will be a valid pointer but no name
				if (newFont->getName() == "")
					delete newFont;
				else
					s_fontsList[fontName] = newFont;
			}
			else
				ENGINE_ERROR("[FontManager::loadFont] Font name already exists or an invalid name was given, cannot load the new font. Name: {0}, Path: {1}.", fontName, fontFilePath);
		}
		else
			ENGINE_ERROR("[FontManager::loadFont] This system has not been enabled.");
	}

	//! destroyFont()
	/*!
	\param fontName a const std::string& - The name of the font
	*/
	void FontManager::destroyFont(const std::string& fontName)
	{
		if (s_enabled)
		{
			// Check if the font name exists
			if (s_fontsList.find(fontName) != s_fontsList.end())
			{
				if(s_fontsList[fontName])
					delete s_fontsList[fontName];

				s_fontsList.erase(fontName);
			}
			else
				ENGINE_ERROR("[FontManager::destroyFont] Font name does not exist, cannot delete the font. Name: {0}", fontName);
		}
		else
			ENGINE_ERROR("[FontManager::destroyFont] This system has not been enabled.");
	}

	//! getFont()
	/*!
	\param fontName a const std::string& - The name of the font
	\return a Font* - A pointer to the font
	*/
	Font* FontManager::getFont(const std::string& fontName)
	{
		if (s_enabled)
		{
			if (s_fontsList.find(fontName) != s_fontsList.end())
				return s_fontsList[fontName];
			else
				ENGINE_ERROR("[FontManager::getFont] Cannot find the font. Name: {0}", fontName);
		}
		else
			ENGINE_ERROR("[FontManager::getFont] This system has not been enabled.");

		// Cannot find font so return null
		return nullptr;
	}

	//! getFontList()
	/*!
	\return a std::map<std::string, Font*>& - A reference to the font list
	*/
	std::map<std::string, Font*>& FontManager::getFontList()
	{
		return s_fontsList;
	}
}
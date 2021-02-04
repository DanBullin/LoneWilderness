/*! \file fontManager.cpp
*
* \brief A system which loads and manages fonts
*
* \author Daniel Bullin
*
*/

#include "independent/systems/systems/fontManager.h"
#include "independent/systems/systems/log.h"

namespace Engine
{
	bool FontManager::s_enabled = false; //!< Is this system enabled
	Map<std::string, Font> FontManager::s_fontsList = Map<std::string, Font>(); //!< Initialise with empty list
	FT_Library FontManager::s_freetype; //!< The ft library
	unsigned char FontManager::s_firstGlyph = 0; //!< The first char (ASCII)
	unsigned char FontManager::s_lastGlyph = 0; //!< The last char (ASCII)

	//! FontManager
	FontManager::FontManager() : System(Systems::Type::FontManager)
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
				s_fontsList.clear();

			s_enabled = false;
		}
	}

	//! loadFont()
	/*!
	\param fontName a const char* - The name of the font
	\param fontFilePath a const char* - The file path of the font
	\param pixelSize a const uint32_t - The size of the font in pixel size
	*/
	void FontManager::loadFont(const char* fontName, const char* fontFilePath, const uint32_t pixelSize)
	{
		if (s_enabled)
		{
			// Check if the font name is already taken
			if (s_fontsList.find(fontName) == s_fontsList.end())
			{
				// Font name is free, so load the font
				Shared<Font> newFont;
				newFont.reset(new Font(s_freetype, fontName, fontFilePath, pixelSize, s_firstGlyph, s_lastGlyph));

				s_fontsList[fontName] = newFont;
			}
			else
				ENGINE_ERROR("[FontManager::loadFont] Font name already exists, cannot load the new font. Name: {0}", fontName);
		}
		else
			ENGINE_ERROR("[FontManager::loadFont] This system has not been enabled.");
	}

	//! destroyFont()
	/*!
	\param fontName a const char* - The name of the font
	*/
	void FontManager::destroyFont(const char* fontName)
	{
	}

	//! getFont()
	/*!
	\param fontName a const char* - The name of the font
	\return a std::shared_ptr<Font> - A pointer to the font
	*/
	Shared<Font> FontManager::getFont(const char* fontName)
	{
		if (s_enabled)
		{
			if (s_fontsList.find(fontName) != s_fontsList.end())
			{
				return s_fontsList[fontName];
			}
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
	\return a std::map<std::string, std::shared_ptr<Font>>& - A reference to the font list
	*/
	Map<std::string, Font>& FontManager::getFontList()
	{
		if (!s_enabled)
			ENGINE_ERROR("[FontManager::getFontList] This system has not been enabled.");

		return s_fontsList;
	}
}
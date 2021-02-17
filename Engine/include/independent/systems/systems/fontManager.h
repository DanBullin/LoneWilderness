/*! \file fontManager.h
*
* \brief A system which loads and manages fonts
*
* \author Daniel Bullin
*
*/
#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include "independent/systems/system.h"
#include "independent/systems/components/font.h"

namespace Engine
{
	/*! \class FontManager
	* \brief A system which loads and manages fonts
	*/
	class FontManager : public System 
	{
	private:
		static bool s_enabled; //!< Is this system enabled
		static std::map<std::string, Font*> s_fontsList; //!< List of all the loaded fonts
		static FT_Library s_freetype; //!< The freetype library
		static unsigned char s_firstGlyph; //!< The first char (ASCII)
		static unsigned char s_lastGlyph; //!< The last char (ASCII)
	public:
		FontManager(); //!< Constructor
		~FontManager(); //!< Destructor
		void start() override; //!< Start the system
		void stop() override; //!< Stop the system

		static void loadFont(const std::string& fontName, const std::string& fontFilePath, const uint32_t pixelSize); //!< Load the font into the manager
		static void destroyFont(const std::string& fontName); //!< Destroy a font and all its contents
		static Font* getFont(const std::string& fontName); //!< Get a font
		static std::map<std::string, Font*>& getFontList(); //!< Get the list of loaded fonts
	};
}
#endif
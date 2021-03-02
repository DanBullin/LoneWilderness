/*! \file font.h
*
* \brief A class which stores all the data about a font
*
* \author Daniel Bullin and DMU Course material
*
*/
#ifndef FONT_H
#define FONT_H

#include "ft2build.h"
#include "freetype/freetype.h"
#include "independent/core/common.h"
#include "independent/rendering/textures/textureAtlas.h"

namespace Engine
{
	/*! \struct GlyphData
	* \brief A struct containing glyph data
	*/
	struct GlyphData
	{
		unsigned char ch; //!< The character
		glm::vec2 size; //!< The dimensions of the glyph (Pixels)
		glm::vec2 bearing; //!< The glyph bearing
		float advance; //!< The advance of the glyph
		SubTexture* subTexture; //!< The subtexture of the glyph
	};

	/*! \class Font
	* \brief A class which stores all the data about a font
	*/
	class Font
	{
	private:
		std::string m_fontName; //!< The name of the font
		std::string m_fontFilePath; //!< The filepath of the font
		FT_Face m_fontFace; //!< A font face
		std::vector<GlyphData> m_glyphData; //!< All the loaded glyph data
		std::shared_ptr<TextureAtlas> m_glyphAtlas; //!< Texture atlas for the glyphs
		unsigned char m_firstGlyph; //!< The first glyph
		unsigned char m_lastGlyph; //!< The last glyph
	public:
		Font(FT_Library& ft, const std::string& fontName, const std::string& fontFilePath, const uint32_t pixelSize, unsigned char firstGlyph, unsigned char lastGlyph); //!< Constructor
		~Font(); //!< Destructor

		const std::string& getName() const; //!< Get the name of the font
		const std::string& getFilePath() const; //!< Get the filepath of the font
		const unsigned char getFirstGlyph() const; //!< Get the first glyph
		const unsigned char getLastGlyph() const; //!< Get the last glyph
		std::vector<GlyphData>& getGlyphData(); //!< Get the glyph data

		void printFontDetails(); //!< Print font details
		void printCharacterDetails(unsigned char character); //!< Print a character's glyph details
		void printCharactersDetails(); //!< Print all character glyph details

		void RtoRGBA(unsigned char* DSTBuffer, unsigned char* SRCBuffer, uint32_t width, uint32_t height); //!< Convert single channel data to 4 channels
	};
}
#endif
/*! \file font.cpp
*
* \brief A class which stores all the data about a font
*
* \author Daniel Bullin and DMU Course material
*
*/
#include "independent/systems/components/font.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/resourceManager.h"

namespace Engine
{
	//!	Font()
	/*!
	\param ft a FT_Library& - A reference to the freetype library
	\param fontName a const char* - The name of the font
	\param fontFilePath a const char* - The filepath of the font
	\param pixelSize a const uint32_t - The size of the font
	\param firstGlyph an unsigned char - The ASCII value of the first glyph
	\param lastGlyph an unsigned char - The ASCII value of the last glyph
	*/
	Font::Font(FT_Library& ft, const char* fontName, const char* fontFilePath, const uint32_t pixelSize, unsigned char firstGlyph, unsigned char lastGlyph)
	{
		ENGINE_INFO("[Font::Font] Creating font named: {0} from file path: {1}.", fontName, fontFilePath);
		m_fontName = fontName;
		m_firstGlyph = firstGlyph;
		m_lastGlyph = lastGlyph;

		// Load font into font face
		if (FT_New_Face(ft, fontFilePath, 0, &m_fontFace))
		{
			ENGINE_ERROR("[Font::Font] Could not load font from filepath: {0}.", fontFilePath);
			return;
		}

		// Set size of font (Pixel size)
		if (FT_Set_Pixel_Sizes(m_fontFace, 0, pixelSize))
		{
			ENGINE_ERROR("[Font::Font] Error freetype can not set font size: {0}.", pixelSize);
			return;
		}

		// Create the texture atlas which will create all the glyph subtextures
		m_glyphAtlas.reset(new TextureAtlas);

		// Resize all the glyph data about the glyphs we will load
		m_glyphData.resize(lastGlyph - firstGlyph);

		// Create some tmp glyph data
		std::vector<std::pair<unsigned char, std::pair<GlyphData, unsigned char*>>> tmpGlyphdata;
		tmpGlyphdata.resize(lastGlyph - firstGlyph);

		// Fill the texture atlas
		for (unsigned char ch = firstGlyph; ch < lastGlyph; ch++)
		{
			if (FT_Load_Char(m_fontFace, ch, FT_LOAD_RENDER))
				ENGINE_ERROR("[Font::Font] Cannot load character into font face: {0}", ch);
			else
			{
				// Get the tmp glyph data of the current character
				auto& pair1 = tmpGlyphdata.at(ch - firstGlyph);

				// Store the loaded character into the glyph data held in the tmp list
				pair1.first = ch;
				GlyphData& gd = pair1.second.first;

				gd.ch = ch; // Store the characyer
				gd.size = glm::vec2(m_fontFace->glyph->bitmap.width, m_fontFace->glyph->bitmap.rows); // Pixel dimensions
				gd.bearing = glm::vec2(m_fontFace->glyph->bitmap_left, -m_fontFace->glyph->bitmap_top);
				gd.advance = static_cast<float>(m_fontFace->glyph->advance.x >> 6);

				// Create the buffer data for the glyph [Dimensions * 4 channels * size of an unsigned char]
				glm::vec2 glyphDims = gd.size;
				uint32_t glyphBufferSize = static_cast<uint32_t>(glyphDims.x * glyphDims.y * 4 * sizeof(unsigned char));
				// Malloc, must delete once done
				pair1.second.second = static_cast<unsigned char*>(malloc(glyphBufferSize));

				// Convert single channel glyph data in font bitmap buffer to our 4 channel buffer
				RtoRGBA(pair1.second.second, m_fontFace->glyph->bitmap.buffer, static_cast<uint32_t>(gd.size.x), static_cast<uint32_t>(gd.size.y));
			}
		}

		// Sort the tmp glyph list into descending order of size
		std::sort(tmpGlyphdata.begin(), tmpGlyphdata.end(), [](std::pair<unsigned char, std::pair<GlyphData, unsigned char*>>& a, std::pair<unsigned char, std::pair<GlyphData, unsigned char*>>& b)
		{
			return a.second.first.size.x > b.second.first.size.x;
		}
		);

		// Go through each entry of the tmp list
		for (unsigned char currentGlyph = 0; currentGlyph < (lastGlyph - firstGlyph); currentGlyph++)
		{
			// Get the glyph data of the current character
			auto& gd = tmpGlyphdata.at(currentGlyph).second.first;

			// Get the glyph buffer
			auto glyphBuffer = tmpGlyphdata.at(currentGlyph).second.second;

			// Send the glyph data, updated glyph buffer and subtexture reference to atlas to create subtexture
			if (!m_glyphAtlas->add(static_cast<uint32_t>(gd.size.x), static_cast<uint32_t>(gd.size.y), 4, glyphBuffer, gd.subTexture, std::string(1, char(gd.ch))))
				ENGINE_ERROR("Font::Font] Error adding to text buffer to the texture atlas.");
			else
			{
				// Now update the actual list of loaded glyphs with tmp glyph data
				m_glyphData.at(currentGlyph) = gd;
			}
			// No longer need buffer, free the memory
			free(glyphBuffer);
		}

		// Sort actual list of glyph data by character order (ASCII)
		std::sort(m_glyphData.begin(), m_glyphData.end(), [](GlyphData& a, GlyphData& b)
		{
			return a.ch < b.ch;
		}
		);

		// Registering the texture atlas for this font with the resource manager
		ResourceManager::registerResource(getName(), m_glyphAtlas->getBaseTexture());

	}

	//! ~Font()
	Font::~Font()
	{
		ENGINE_INFO("[Font::~Font] Deleting font: {0}.", getName());

		for (auto& sub : m_glyphData)
		{
			delete sub.subTexture;
		}
	}

	//!	getName()
	/*!
	\return a const std::string& - The name of the font
	*/
	const std::string& Font::getName() const
	{
		return m_fontName;
	}

	//!	getFirstGlyph()
	/*!
	\return a const unsigned char - The first glyph
	*/
	const unsigned char Font::getFirstGlyph() const
	{
		return m_firstGlyph;
	}

	//!	getLastGlyph()
	/*!
	\return a const unsigned char - The last glyph
	*/
	const unsigned char Font::getLastGlyph() const
	{
		return m_lastGlyph;
	}

	//!	getGlyphData()
	/*!
	\return a std::vector<GlyphData>& - A reference to the glyph data
	*/
	std::vector<GlyphData>& Font::getGlyphData()
	{
		return m_glyphData;
	}

	//! printFontDetails()
	void Font::printFontDetails()
	{
		ENGINE_TRACE("Font Details for: {0}", getName());
		ENGINE_TRACE("==========================");
		ENGINE_TRACE("Glyph Data List Size: {0}", m_glyphData.size());
		ENGINE_TRACE("First Glyph: {0}", m_firstGlyph);
		ENGINE_TRACE("Last Glyph: {0}", m_lastGlyph);
		ENGINE_TRACE("Glyph Atlas Base Texture ID: {0}", m_glyphAtlas->getBaseTexture()->getID());
		ENGINE_TRACE("==========================");
	}

	//!	printCharacterDetails()
	/*!
	\param character an unsigned char - The character to print
	*/
	void Font::printCharacterDetails(unsigned char character)
	{
		ENGINE_TRACE("Font Details for: {0}", getName());
		ENGINE_TRACE("==========================");
		ENGINE_TRACE("Glyph Data Character: {0}.", m_glyphData.at(character - m_firstGlyph).ch);
		ENGINE_TRACE("Glyph Data Bearing: {0}, {1}.", m_glyphData.at(character - m_firstGlyph).bearing.x, m_glyphData.at(character - m_firstGlyph).bearing.y);
		ENGINE_TRACE("Glyph Data Advance: {0}.", m_glyphData.at(character - m_firstGlyph).advance);
		ENGINE_TRACE("Glyph Data Size: {0}, {1}.", m_glyphData.at(character - m_firstGlyph).size.x, m_glyphData.at(character - m_firstGlyph).size.y);
		ENGINE_TRACE("Glyph Data UVStart: {0}, {1},", m_glyphData.at(character - m_firstGlyph).subTexture->getUVStart().x, m_glyphData.at(character - m_firstGlyph).subTexture->getUVStart().y);
		ENGINE_TRACE("Glyph Data UVEnd: {0}, {1},", m_glyphData.at(character - m_firstGlyph).subTexture->getUVEnd().x, m_glyphData.at(character - m_firstGlyph).subTexture->getUVEnd().y);
		ENGINE_TRACE("==========================");
	}

	//! printCharactersDetails()
	void Font::printCharactersDetails()
	{
		for (unsigned char c = m_firstGlyph; c < m_lastGlyph; c++)
			printCharacterDetails(c);
	}

	//!	RtoRGBA()
	/*!
	\param DSTBuffer an unsigned char* - The destination buffer
	\param SRCBuffer an unsigned char* - The source single channel buffer
	\param width an uint32_t - The width of the glyph
	\param height an uint32_t - The height of the glyph
	*/
	void Font::RtoRGBA(unsigned char* DSTBuffer, unsigned char* SRCBuffer, uint32_t width, uint32_t height)
	{
		unsigned char* pWalker = DSTBuffer;

		int iHeight = static_cast<int>(height);
		int iWidth = static_cast<int>(width);
		for (int i = 0; i < iHeight; i++)
		{
			for (int j = 0; j < iWidth; j++)
			{
				*pWalker = 255; pWalker++;
				*pWalker = 255; pWalker++;
				*pWalker = 255; pWalker++;
				*pWalker = *SRCBuffer;
				pWalker++;
				SRCBuffer++;
			}
		}
	}
}
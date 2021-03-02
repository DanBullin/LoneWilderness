/*! \file textureAtlas.h
*
* \brief A dynamic texture atlas which holds a bunch of textures in one
*
* \author DMU Course material
*
*/
#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H

#include "independent/rendering/textures/subTexture.h"

namespace Engine
{
	/*! \struct SimpleRect
	* \brief A rectangle with position and size
	*/
	struct SimpleRect
	{
		int32_t x; //!< x position
		int32_t y; //!< y position
		int32_t w; //!< Width
		int32_t h; //!< Height
	};

	/*! \class TextureAtlas
	* \brief A dynamic texture atlas
	*/
	class TextureAtlas
	{
	private:
		std::string m_name; //!< The name of the atlas
		std::vector<SimpleRect> m_spaces; //! Gaps left which can be filled
		Texture2D* m_baseTexture; //!< Texture which holds all subtexture pixel data
	public:
		TextureAtlas(const std::string& name, const glm::ivec2 size = { 4096, 4096 }, const uint32_t channels = 4, const uint32_t reservedSpaces = 32); //!<	Constructor
		~TextureAtlas(); //!< Destructor

		inline Texture2D* getBaseTexture() const { return m_baseTexture; } //!< Get the base texture
			/*!< \return a const Texture2D* - The base texture pointer */

		bool add(const std::string& filePath, SubTexture*& result, const std::string& subTextureName); //!< Attempt to add a subtexture with a file
		bool add(int32_t width, int32_t height, uint32_t channels, unsigned char* data, SubTexture*& result, const std::string& subTextureName); //!< Attempt to add a subtexture
	};
}

#endif
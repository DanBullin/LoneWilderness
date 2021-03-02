/*! \file textureAtlas.cpp
*
* \brief A dynamic texture atlas which holds a bunch of textures in one
*
* \author DMU Course material
*
*/
#include "independent/rendering/textures/textureAtlas.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/resourceManager.h"
#include <stb_image.h>

namespace Engine
{
	//!	TextureAtlas()
	/*!
	\param name a const std::string& - The name of the texture atlas
	\param size a const glm::ivec2 - The size of the texture atlas
	\param channels a const uint32_t - The number of channels for this atlas
	\param reservedSpaces a const uint32_t - The number of spaces to reserve
	*/
	TextureAtlas::TextureAtlas(const std::string& name, const glm::ivec2 size, const uint32_t channels, const uint32_t reservedSpaces)
	{
		m_name = name;
		TextureProperties properties(size.x, size.y, "Repeat", "Repeat", "Repeat", "Linear", "Linear", false, false);
		m_baseTexture = Texture2D::create(name, properties, channels, nullptr);

		m_spaces.reserve(reservedSpaces);
		m_spaces.push_back({ 0, 0, size.x, size.y });
	}

	//! ~TextureAtlas()
	TextureAtlas::~TextureAtlas()
	{
		// Resource Manager will delete this resource, so lets just set to null, MAKE SURE TO REGISTER THE TEXTURE THOUGH
		m_baseTexture = nullptr;
	}

	//!	add()
	/*!
	\param filePath a const std::string& - The filepath of the texture to add
	\param result a SubTexture*& - The subtexture created when adding the texture to the atlas
	\param subTextureName a const std::string& - The name of the subtexture
	\return a bool - Returns whether the addition was successful
	*/
	bool TextureAtlas::add(const std::string& filePath, SubTexture*& result, const std::string& subTextureName)
	{
		int32_t width, height, channels;
		unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, static_cast<int>(m_baseTexture->getChannels()));

		if (data)
		{
			return add(width, height, channels, data, result, subTextureName);
		}
		stbi_image_free(data);
		return false;
	}

	//!	add()
	/*!
	\param width a int32_t - The width of the texture to add
	\param height a int32_t - The height of the texture to add
	\param channels a uint32_t - The number of channels of the texture
	\param data an unsigned char* - The data of the texture
	\param result a SubTexture*& - The subtexture created when adding the texture to the atlas
	\param subTextureName a const std::string& - The name of the subtexture
	\return a bool - Returns whether the addition was successful
	*/
	bool TextureAtlas::add(int32_t width, int32_t height, uint32_t channels, unsigned char * data, SubTexture*& result, const std::string& subTextureName)
	{
		if (channels != m_baseTexture->getChannels()) return false;

		if (!m_baseTexture)
		{
			ENGINE_ERROR("[TextureAtlas::add] This texture atlas has an invalid base texture.");
			return false;
		}

		for (auto it = m_spaces.begin(); it != m_spaces.end(); ++it)
		{
			SimpleRect& space = *it;

			// Does any space fit the incoming subtexture
			if (width < space.w && height < space.h)
			{
				m_baseTexture->edit(space.x, space.y, width, height, data);
				glm::vec2 UVStart(static_cast<float>(space.x) / static_cast<float>(m_baseTexture->getProperties().Width), static_cast<float>(space.y) / static_cast<float>(m_baseTexture->getProperties().Height));
				glm::vec2 UVEnd(static_cast<float>(space.x + width) / static_cast<float>(m_baseTexture->getProperties().Width), static_cast<float>(space.y + height) / static_cast<float>(m_baseTexture->getProperties().Height));

				result = new SubTexture(subTextureName, m_baseTexture, UVStart, UVEnd, false);

				// Create new space while erasing the current one
				if (width == space.w && height == space.h)
				{
					m_spaces.erase(it);
					return true;
				}
				else if (width == space.w)
				{
					space.y += height;
					space.h -= height;
					return true;
				}
				else if (height == space.h)
				{
					space.x += width;
					space.w -= width;
					return true;
				}
				else
				{
					SimpleRect newRect = { space.x, space.y + height, width, space.h - height };
					space.x += width;
					space.w -= width;
					m_spaces.push_back(newRect);

					std::sort(m_spaces.begin(), m_spaces.end(), [](SimpleRect& a, SimpleRect& b)
					{ return a.w < b.w; }
					);
					return true;
				}
			}
		}
		return false;
	}
}
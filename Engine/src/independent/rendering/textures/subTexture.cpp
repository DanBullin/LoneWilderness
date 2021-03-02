/*! \file subTexture.cpp
*
* \brief A subtexture of a base texture
*
* \author DMU Course material
*
*/
#include "independent/rendering/textures/subTexture.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/resourceManager.h"

namespace Engine
{
	//! SubTexture()
	/*!
	\param subTextureName a const std::string& - The name of the subtexture
	\param baseTexture a Texture2D* - The base texutre
	\param UVStart a const glm::vec2& - The UV start coordinates
	\param UVEnd a const glm::vec2& - The UV end coordinates
	\param convertToBottomLeft a const bool - Convert UVs to bottom left start system
	*/
	SubTexture::SubTexture(const std::string& subTextureName, Texture2D* baseTexture, const glm::vec2& UVStart, const glm::vec2& UVEnd, const bool convertToBottomLeft)
		: Resource(subTextureName, ResourceType::SubTexture), m_baseTexture(nullptr), m_UVStart(UVStart), m_UVEnd(UVEnd)
	{
		setBaseTexture(baseTexture, m_UVStart, m_UVEnd, convertToBottomLeft);
	}

	//! ~SubTexture()
	SubTexture::~SubTexture()
	{
		if (ResourceManager::getConfigValue(Config::PrintResourcesInDestructor))
			ENGINE_INFO("[SubTexture::~SubTexture] Deleting SubTexture with name: {0}.", m_name);

		m_baseTexture = nullptr;
	}

	//! setBaseTexture()
	/*!
	\param texture a Texture2D* - A pointer to the texture
	\param UVStart a const glm::vec2& - The UV start coordinates
	\param UVEnd a const glm::vec2& - The UV end coordinates
	\param convertToBottomLeft a const bool - Convert UVs to bottom left start system
	*/
	void SubTexture::setBaseTexture(Texture2D* texture, const glm::vec2& UVStart, const glm::vec2& UVEnd, const bool convertToBottomLeft)
	{
		if (texture)
		{
			m_baseTexture = texture;
			m_UVStart = UVStart;
			m_UVEnd = UVEnd;
			m_size.x = static_cast<int>((m_UVEnd.x - m_UVStart.x) * static_cast<float>(m_baseTexture->getProperties().Width));
			m_size.y = static_cast<int>((m_UVEnd.y - m_UVStart.y) * static_cast<float>(m_baseTexture->getProperties().Height));

			if (convertToBottomLeft)
				convertCoordsToBottomLeftSystem(m_UVStart, m_UVEnd);
		}
		else
			ENGINE_ERROR("[SubTexture::setBaseTexture] Cannot set base texture as it isn't a valid texture. SubTexture: {0}.", m_name);
	}

	//! setSize()
	/*!
	\param size a const glm::ivec2& - Set the size of the subtexture
	*/
	void SubTexture::setSize(const glm::ivec2& size)
	{
		if(size.x >= 0 && size.y >= 0)
			m_size = size;
		else
			ENGINE_ERROR("[SubTexture::setSize] Invalid size. Size: {0}, {1}. Material: {2}.", size.x, size.y, m_name);
	}

	//! setUVs()
	/*!
	\param UVStart a const glm::vec2& - Start of the UV coords
	\param UVEnd a const glm::vec2& - End of the UV coords
	*/
	void SubTexture::setUVs(const glm::vec2& UVStart, const glm::vec2& UVEnd)
	{
		m_UVStart = UVStart;
		m_UVEnd = UVEnd;
	}

	//! transformUV()
	/*!
	\param uv a const glm::vec2& - The UV coordinates
	\return a const glm::vec2 - The transformed UV coordinates
	*/
	const glm::vec2 SubTexture::transformUV(const glm::vec2& uv)
	{
		glm::vec2 result = { 0, 0 };
		result = (m_UVStart + ((m_UVEnd - m_UVStart) * uv));
		return result;
	}

	//! transformUV()
	/*!
	\param uv a const std::array<int16_t, 2>& - The UV coordinates
	\return a const std::array<int16_t, 2> - The transformed UV coordinates
	*/
	const std::array<int16_t, 2> SubTexture::transformUV(const std::array<int16_t, 2>& uv)
	{
		std::array<int16_t, 2> result = { 0, 0 };
		result[0] = static_cast<int16_t>(m_UVStart.x + ((m_UVEnd.x - m_UVStart.x) * uv[0]));
		result[1] = static_cast<int16_t>(m_UVStart.y + ((m_UVEnd.y - m_UVStart.y) * uv[1]));
		return result;
	}

	//! transformU()
	/*!
	\param u a const int16_t - The U coordinate
	\return a const int16_t - The transformed U coordinate
	*/
	const int16_t SubTexture::transformU(const int16_t u)
	{
		return static_cast<int16_t>(m_UVStart.x + ((m_UVEnd.x - m_UVStart.x) * u));
	}

	//! transformV()
	/*!
	\param v a const int16_t - The V coordinate
	\return a const int16_t - The transformed V coordinate
	*/
	const int16_t SubTexture::transformV(const int16_t v)
	{
		return static_cast<int16_t>(m_UVStart.y + ((m_UVEnd.y - m_UVStart.y) * v));
	}

	//! printDetails()
	void SubTexture::printDetails()
	{
		if (m_baseTexture)
			ENGINE_TRACE("Base Texture: {0}", m_baseTexture->getName());
		else
			ENGINE_TRACE("Base Texture: NULL");

		ENGINE_TRACE("UV Start: {0}, {1}.", m_UVStart.x, m_UVStart.y);
		ENGINE_TRACE("UV End: {0}, {1}.", m_UVEnd.x, m_UVEnd.y);
		ENGINE_TRACE("Size: {0}, {1}.", m_size.x, m_size.y);
	}
}
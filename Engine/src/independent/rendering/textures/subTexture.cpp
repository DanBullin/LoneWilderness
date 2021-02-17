/*! \file subTexture.cpp
*
* \brief A subtexture of a base texture
*
* \author DMU Course material
*
*/
#include "independent/rendering/textures/subTexture.h"
#include "independent/systems/systems/log.h"

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
		: Resource(subTextureName, ResourceType::SubTexture), m_baseTexture(baseTexture), m_UVStart(UVStart), m_UVEnd(UVEnd)
	{
		if (convertToBottomLeft)
			convertCoordsToBottomLeftSystem(m_UVStart, m_UVEnd);

		m_size.x = static_cast<int>((m_UVEnd.x - m_UVStart.x) * static_cast<float>(m_baseTexture->getProperties().Width));
		m_size.y = static_cast<int>((m_UVEnd.y - m_UVStart.y) * static_cast<float>(m_baseTexture->getProperties().Height));

		// Base texture must be given its reference counter increase before being received here
	}

	//! ~SubTexture()
	SubTexture::~SubTexture()
	{
		ENGINE_INFO("[SubTexture::~SubTexture] Deleting SubTexture. Name: {0}.", m_name);

		// If the subtexture has a valid base texture, reduce base textures reference counter
		if (m_baseTexture)
			m_baseTexture->decreaseCount();
	}

	//! setBaseTexture()
	/*!
	\param texture a Texture2D* - A pointer to the texture
	*/
	void SubTexture::setBaseTexture(Texture2D* texture)
	{
		// If it is overriding a valid base texture, reduce old base textures reference counter
		if (m_baseTexture)
			m_baseTexture->decreaseCount();

		// New base texture already received its increase reference counter when given to this function
		m_baseTexture = texture;
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
}
/*! \file material.cpp
*
* \brief A material class which is applied to a model when rendering. It contains all the data
* a shader needs in regards to textures and which shader to use.
*
* \author Daniel Bullin
*
*/

#include "independent/rendering/materials/material.h"
#include "independent/systems/systems/log.h"

namespace Engine
{
	//!	Material()
	/*!
	\param subTextures a const std::vector<SubTexture2D*>& - A list of all subtextures
	\param shader a ShaderProgram* - A pointer to the shader program
	\param tint a const glm::vec4& - A reference to a colour
	*/
	Material::Material(const std::vector<SubTexture2D*>& subTextures, ShaderProgram* shader, const glm::vec4& tint)
		: m_subTextures(subTextures), m_shader(shader), m_tint(tint)
	{

	}

	//! ~Material()
	Material::~Material()
	{
		ENGINE_INFO("[Material::~Material] Deleting material.");
	}

	//!	addSubTexture()
	/*!
	\param texture a SubTexture2D* - A pointer to the subtexture
	*/
	void Material::addSubTexture(SubTexture2D* texture)
	{
		m_subTextures.push_back(texture);
	}

	//!	setSubTexture()
	/*!
	\param index a const uint32_t - The index
	\param texture a SubTexture2D* - A pointer to the subtexture
	*/
	void Material::setSubTexture(const uint32_t index, SubTexture2D* texture)
	{
		if (m_subTextures.size() > index)
			m_subTextures.at(index) = texture;
		else
			ENGINE_ERROR("[Material::getSubTexture] Cannot set sub texture as index is too large. Index: {0}.", index);
	}

	//!	getSubTexture()
	/*!
	\param index a const uint32_t - The index
	\return a SubTexture2D* - A pointer to the subtexture
	*/
	SubTexture2D* Material::getSubTexture(const uint32_t index)
	{
		if (m_subTextures.size() > index)
			return m_subTextures.at(index);
		else
			ENGINE_ERROR("[Material::getSubTexture] Cannot get sub texture as index is too large. Index: {0}.", index);

		return nullptr;
	}

	//!	getSubTextures()
	/*!
	\return a std::vector<SubTexture2D*>& - A list of all sub textures
	*/
	std::vector<SubTexture2D*>& Material::getSubTextures()
	{
		return m_subTextures;
	}

	//! setShader()
	/*!
	\param shaderProgram a ShaderProgram* - A pointer to the shader program
	*/
	void Material::setShader(ShaderProgram* shaderProgram)
	{
		m_shader = shaderProgram;
	}

	//!	getShader()
	/*!
	\return a ShaderProgram* - A pointer to the shader program
	*/
	ShaderProgram* Material::getShader()
	{
		return m_shader;
	}

	//! setTint()
	/*!
	\param tint a const glm::vec4& - A reference to a colour
	*/
	void Material::setTint(const glm::vec4& tint)
	{
		m_tint = tint;
	}

	//! getTint()
	/*!
	\return a glm::vec4& - A reference to a colour
	*/
	glm::vec4& Material::getTint()
	{
		return m_tint;
	}
}
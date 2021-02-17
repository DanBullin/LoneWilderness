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
	\param materialName a const std::string& - The name of the material
	\param subTextures a const std::vector<SubTexture*>& - A list of all subtextures
	\param cubemaps a const std::vector<CubeMapTexture*>& - A list of all cubemaps
	\param shader a ShaderProgram* - A pointer to the shader program
	\param tint a const glm::vec4& - A reference to a colour
	*/
	Material::Material(const std::string& materialName, const std::vector<SubTexture*>& subTextures, const std::vector<CubeMapTexture*>& cubemaps, ShaderProgram* shader, const glm::vec4& tint)
		: Resource(materialName, ResourceType::Material), m_subTextures(subTextures), m_cubeMapTextures(cubemaps), m_shader(shader), m_tint(tint)
	{

	}

	//! ~Material()
	Material::~Material()
	{
		ENGINE_INFO("[Material::~Material] Deleting material with name: {0}.", m_name);

		// Decrease all references
		for (auto& subTexture : m_subTextures)
		{
			if (subTexture)
				subTexture->decreaseCount();
		}

		if (m_shader)
			m_shader->decreaseCount();
	}

	//!	addSubTexture()
	/*!
	\param texture a SubTexture* - A pointer to the subtexture
	*/
	void Material::addSubTexture(SubTexture* texture)
	{
		m_subTextures.push_back(texture);
	}

	//!	setSubTexture()
	/*!
	\param index a const uint32_t - The index
	\param texture a SubTexture* - A pointer to the subtexture
	*/
	void Material::setSubTexture(const uint32_t index, SubTexture* texture)
	{
		if (m_subTextures.size() > index)
			m_subTextures.at(index) = texture;
		else
			ENGINE_ERROR("[Material::setSubTexture] Cannot set sub texture as index is too large. Index: {0}.", index);
	}

	//!	getSubTexture()
	/*!
	\param index a const uint32_t - The index
	\return a SubTexture* - A pointer to the subtexture
	*/
	SubTexture* Material::getSubTexture(const uint32_t index)
	{
		if (m_subTextures.size() > index)
			return m_subTextures.at(index);
		else
			ENGINE_ERROR("[Material::getSubTexture] Cannot get sub texture as index is too large. Index: {0}.", index);

		return nullptr;
	}

	//!	getSubTextures()
	/*!
	\return a std::vector<SubTexture*>& - A list of all sub textures
	*/
	std::vector<SubTexture*>& Material::getSubTextures()
	{
		return m_subTextures;
	}

	//!	addCubemapTexture()
	/*!
	\param texture a CubeMapTexture* - A pointer to the cubemap texture
	*/
	void Material::addCubemapTexture(CubeMapTexture* texture)
	{
		m_cubeMapTextures.push_back(texture);
	}

	//!	setCubemapTexture()
	/*!
	\param index a const uint32_t - The index
	\param texture a CubeMapTexture* - A pointer to the cubemap texture
	*/
	void Material::setCubemapTexture(const uint32_t index, CubeMapTexture* texture)
	{
		if (m_cubeMapTextures.size() > index)
			m_cubeMapTextures.at(index) = texture;
		else
			ENGINE_ERROR("[Material::setCubemapTexture] Cannot set cubemap texture as index is too large. Index: {0}.", index);
	}

	//!	getCubemapTexture()
	/*!
	\param index a const uint32_t - The index
	\return a CubeMapTexture* - A pointer to the cubemap texture
	*/
	CubeMapTexture* Material::getCubemapTexture(const uint32_t index)
	{
		if (m_cubeMapTextures.size() > index)
			return m_cubeMapTextures.at(index);
		else
			ENGINE_ERROR("[Material::getCubemapTexture] Cannot get cubemap texture as index is too large. Index: {0}.", index);

		return nullptr;
	}

	//!	getCubemapTextures()
	/*!
	\return a std::vector<CubeMapTexture*>& - A list of all cubemap textures
	*/
	std::vector<CubeMapTexture*>& Material::getCubemapTextures()
	{
		return m_cubeMapTextures;
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
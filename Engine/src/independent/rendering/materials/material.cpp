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
#include "independent/systems/systems/resourceManager.h"

namespace Engine
{
	//!	Material()
	/*!
	\param materialName a const std::string& - The name of the material
	\param subTextures a const std::vector<SubTexture*>& - A list of all subtextures
	\param cubemaps a const std::vector<CubeMapTexture*>& - A list of all cubemaps
	\param shader a ShaderProgram* - A pointer to the shader program
	\param tint a const glm::vec4& - A reference to a colour
	\param shininess a const float - The material's shininess
	*/
	Material::Material(const std::string& materialName, const std::vector<SubTexture*>& subTextures, const std::vector<CubeMapTexture*>& cubemaps, ShaderProgram* shader, const glm::vec4& tint, const float shininess)
		: Resource(materialName, ResourceType::Material), m_subTextures(subTextures), m_cubeMapTextures(cubemaps), m_shader(shader), m_tint(tint), m_shininess(shininess)
	{
	}

	//! ~Material()
	Material::~Material()
	{
		if(ResourceManager::getConfigValue(Config::PrintResourcesInDestructor))
			ENGINE_INFO("[Material::~Material] Deleting material with name: {0}.", m_name);
	}

	//!	addSubTexture()
	/*!
	\param texture a SubTexture* - A pointer to the subtexture
	*/
	void Material::addSubTexture(SubTexture* texture)
	{
		if (texture)
			m_subTextures.push_back(texture);
		else
			ENGINE_ERROR("[Material::addSubTexture] Cannot add this subtexture as it is not a valid subtexture. Material: {0}.", m_name);
	}

	//!	setSubTexture()
	/*!
	\param index a const uint32_t - The index
	\param texture a SubTexture* - A pointer to the subtexture
	*/
	void Material::setSubTexture(const uint32_t index, SubTexture* texture)
	{
		if (m_subTextures.size() > index)
		{
			if (texture)
				m_subTextures.at(index) = texture;
			else
				ENGINE_ERROR("[Material::setSubTexture] Cannot set this subtexture at index {0} as it is not a valid subtexture. Material: {1}.", index, m_name);
		}
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
		if (texture)
			m_cubeMapTextures.push_back(texture);
		else
			ENGINE_ERROR("[Material::addCubemapTexture] Cannot add this cubemap texture as it is not a valid cubemap. Material: {0}.", m_name);
	}

	//!	setCubemapTexture()
	/*!
	\param index a const uint32_t - The index
	\param texture a CubeMapTexture* - A pointer to the cubemap texture
	*/
	void Material::setCubemapTexture(const uint32_t index, CubeMapTexture* texture)
	{
		if (m_cubeMapTextures.size() > index)
		{
			if (texture)
				m_cubeMapTextures.at(index) = texture;
			else
				ENGINE_ERROR("[Material::setCubemapTexture] Cannot set this cubemap texture at index {0} as it is not a valid cubemap. Material: {1}.", index, m_name);
		}
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
		if (shaderProgram)
			m_shader = shaderProgram;
		else
			ENGINE_ERROR("[Material::setShader] Cannot set this shader program as it is not a valid program. Material: {0}.", m_name);
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

	//! setShininess()
	/*
	\param shininess a const float - The material's shininess
	*/
	void Material::setShininess(const float shininess)
	{
		m_shininess = shininess;
	}

	//! getShininess()
	/*
	\return a float - The material's shininess
	*/
	float Material::getShininess() const
	{
		return m_shininess;
	}
	
	//! printDetails()
	void Material::printDetails()
	{
		for (int i = 0; i < m_subTextures.size(); i++)
		{
			auto t = m_subTextures[i]->getName();

			if (m_subTextures[i])
				ENGINE_TRACE("SubTexture{0}: {1}", i, m_subTextures[i]->getName());
			else
				ENGINE_TRACE("SubTexture{0}: NULL", i);
		}

		for (int i = 0; i < m_cubeMapTextures.size(); i++)
		{
			if (m_cubeMapTextures[i])
				ENGINE_TRACE("CubeTexture{0}: {1}", i, m_cubeMapTextures[i]->getName());
			else
				ENGINE_TRACE("CubeTexture{0}: NULL", i);
		}

		if (m_shader)
			ENGINE_TRACE("Shader: {0}", m_shader->getName());
		else
			ENGINE_TRACE("Shader: NULL");

		ENGINE_TRACE("Tint: R:{0}, G:{1}, B:{2}, A:{3}.", m_tint.r, m_tint.g, m_tint.b, m_tint.a);
		ENGINE_TRACE("Shininess: {0}", m_shininess);
	}
}
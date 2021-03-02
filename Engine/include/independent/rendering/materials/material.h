/*! \file material.h
*
* \brief A material class which is applied to a model when rendering. It contains all the data
* a shader needs in regards to textures and which shader to use.
*
* \author Daniel Bullin
*
*/
#ifndef MATERIAL_H
#define MATERIAL_H

#include "independent/core/common.h"
#include "independent/systems/components/resource.h"
#include "independent/rendering/shaders/shaderProgram.h"
#include "independent/rendering/textures/subTexture.h"

namespace Engine
{
	/*! \class Material
	* \brief A material class containing shader data to use when rendering
	*/
	class Material : public Resource
	{
	private:
		std::vector<SubTexture*> m_subTextures; //!< A list of all subtextures attached to this material
		std::vector<CubeMapTexture*> m_cubeMapTextures; //!< A list of all cubmeap textures attached to this material
		ShaderProgram* m_shader; //!< The shader program to use when applying this material
		glm::vec4 m_tint; //!< The tint to apply to objects
		float m_shininess; //!< The shininess value used in specular lighting calculations
	public:
		Material(const std::string& materialName, const std::vector<SubTexture*>& subTextures, const std::vector<CubeMapTexture*>& cubemaps, ShaderProgram* shader, const glm::vec4& tint, const float shininess); //!< Constructor
		~Material(); //!< Destructor

		void addSubTexture(SubTexture* texture); //!< Add the subtexture
		void setSubTexture(const uint32_t index, SubTexture* texture); //!< Set the subtexture at index
		SubTexture* getSubTexture(const uint32_t index); //!< Get the subtexture at index
		std::vector<SubTexture*>& getSubTextures(); //!< Get a list of all subtextures

		void addCubemapTexture(CubeMapTexture* texture); //!< Add the cubemap texture
		void setCubemapTexture(const uint32_t index, CubeMapTexture* texture); //!< Set the cubmap texture at index
		CubeMapTexture* getCubemapTexture(const uint32_t index); //!< Get the cubemap at index
		std::vector<CubeMapTexture*>& getCubemapTextures(); //!< Get a list of all cubemaps

		void setShader(ShaderProgram* shaderProgram); //!< Set the shader to use
		ShaderProgram* getShader(); //!< Get the shader to use

		void setTint(const glm::vec4& tint); //!< Set the material tint
		glm::vec4& getTint(); //!< Get the material tint

		void setShininess(const float shininess); //!< Set the material's shininess
		float getShininess() const; //!< Get the material's shininess

		void printDetails() override; //!< Print the resource details
	};
}
#endif
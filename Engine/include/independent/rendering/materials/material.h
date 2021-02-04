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
#include "independent/rendering/shaders/shaderProgram.h"
#include "independent/rendering/textures/subTexture.h"

namespace Engine
{
	/*! \class Material
	* \brief A material class containing shader data to use when rendering
	*/
	class Material
	{
	private:
		std::vector<SubTexture2D*> m_subTextures; //!< A list of all subtextures attached to this material
		ShaderProgram* m_shader; //!< The shader program to use when applying this material
		glm::vec4 m_tint; //!< The tint to apply to objects
	public:
		Material(const std::vector<SubTexture2D*>& subTextures, ShaderProgram* shader, const glm::vec4& tint); //!< Constructor
		~Material(); //!< Destructor

		void addSubTexture(SubTexture2D* texture); //!< Add the subtexture
		void setSubTexture(const uint32_t index, SubTexture2D* texture); //!< Set the subtexture at index
		SubTexture2D* getSubTexture(const uint32_t index); //!< Get the subtexture at index
		std::vector<SubTexture2D*>& getSubTextures(); //!< Get a list of all subtextures

		void setShader(ShaderProgram* shaderProgram); //!< Set the shader to use
		ShaderProgram* getShader(); //!< Get the shader to use

		void setTint(const glm::vec4& tint); //!< Set the material tint
		glm::vec4& getTint(); //!< Get the material tint
	};
}
#endif
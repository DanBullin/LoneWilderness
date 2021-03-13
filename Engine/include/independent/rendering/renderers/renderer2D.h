/*! \file renderer2D.h
*
* \brief A 2D renderer which allows 2D models to be submitted and drawn (Batched)
*
* \author Daniel Bullin
*
*/
#ifndef RENDERER2D_H
#define RENDERER2D_H

#include "independent/core/common.h"
#include "independent/rendering/textures/textureUnitManager.h"
#include "independent/rendering/materials/material.h"
#include "independent/entities/components/text.h"
#include "independent/entities/components/meshRender2D.h"
#include "independent/rendering/geometry/quad.h"

namespace Engine
{
	/*! \struct BatchEntry2D
	* \brief A struct containing the 2D submission data
	*/
	struct BatchEntry2D
	{
		ShaderProgram* shader; //!< The shader program
		std::vector<SubTexture*> subTextures; //!< The list of subtextures in this entry
		std::vector<int32_t> textureUnits; //!< The list of texture units used
		glm::mat4 modelMatrix; //!< The model matrix
		glm::vec4 tint; //!< The tint
	};

	/*! \class Renderer2D
	* \brief A static class which renders 2D geometry (Batched)
	*/
	class Renderer2D
	{
	private:
		static uint32_t s_batchCapacity; //!< Batch capacity for 2D rendering (Number of Quads)
		static TextureUnitManager* s_unitManager; //!< The texture unit manager
		static std::array<int32_t, 16> s_unit; //!< The texture unit
		static std::vector<BatchEntry2D> s_batchQueue; //!< The queue of 2D submissions

		static bool submissionChecks(ShaderProgram* shaderProgram, const std::vector<SubTexture*>& subTextures); //!< Check the submission
		static void sortSubmissions(std::vector<BatchEntry2D>& submissions); //!< Sort the submissions

		static void flush(); //!< Flush the scene
		static void draw(std::vector<BatchEntry2D>& submissionList); //!< Draw the scene
	public:
		static void initialise(const uint32_t batchCapacity); //!< Initialise the renderer

		static void begin(); //!< Begin a new 2D scene
		static void submit(ShaderProgram* shaderProgram, const std::vector<SubTexture*>& subTextures, const glm::mat4& modelMatrix, const glm::vec4& tint); //!< Submit a 2D entity
		static void submitText(Text* text, const glm::mat4& modelMatrix); //!< Submit text to renderer
		static void submitText(const std::string& text, const std::string& fontName, const glm::vec4& tint, const glm::mat4& modelMatrix); //!< Submit text to renderer

		static void end(); //!< End the 2D scene

		static void destroy(); //!< Destroy all internal data
		static void setTextureUnitManager(TextureUnitManager*& unitManager, const std::array<int32_t, 16>& unit); //!< Set the texture unit manager and units to use
	};
}
#endif
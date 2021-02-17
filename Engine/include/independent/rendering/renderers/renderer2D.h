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
#include "independent/rendering/geometry/vertex.h"
#include "independent/rendering/geometry/vertexArray.h"
#include "independent/rendering/uniformBuffer.h"
#include "independent/rendering/textures/textureUnitManager.h"
#include "independent/rendering/renderUtils.h"
#include "independent/rendering/materials/material.h"
#include "independent/entities/components/text.h"
#include "independent/entities/components/meshRender2D.h"

namespace Engine
{
	/*! \struct BatchEntry2D
	* \brief A struct containing the 2D submission data
	*/
	struct BatchEntry2D
	{
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
		static ShaderProgram* s_overridingShader; //!< A pointer to the shader program to override material shader
		static TextureUnitManager* s_unitManager; //!< The texture unit manager
		static std::array<int32_t, 16> s_unit; //!< The texture unit
		static uint32_t s_drawCount; //!< The number of quads we're currently drawing

		static std::map<ShaderProgram*, std::vector<BatchEntry2D>> s_batchQueue; //!< The queue of 2D submissions by shader program

		static void generateVertexList(ShaderProgram* shader, std::vector<BatchEntry2D>& batchEntries); //!< Generate vertex list
		static void flush(); //!< Flush the scene
		static void draw(ShaderProgram* shader); //!< Draw the scene

		static void generateListOfVertex2D(ShaderProgram* shader, std::vector<BatchEntry2D>& batchEntries); //!< Generate a list of vertices and edit the VBO
	public:
		static void initialise(const uint32_t batchCapacity); //!< Initialise the renderer
		static void begin(ShaderProgram* shaderProgram); //!< Begin a new 2D scene
		static void submit(Material* material, const glm::mat4& modelMatrix, const glm::vec4& tint = glm::vec4(-1.f, -1.f, -1.f, -1.f)); //!< Submit a 2D entity
		static void submitText(Text* text, const glm::mat4& modelMatrix); //!< Submit text to renderer
		static void end(); //!< End the 2D scene
		static void destroy(); //!< Destroy all internal data

		static void setTextureUnitManager(TextureUnitManager*& unitManager, const std::array<int32_t, 16>& unit); //!< Set the texture unit manager and units to use
	};
}
#endif
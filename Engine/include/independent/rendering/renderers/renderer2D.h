/*! \file renderer2D.h
*
* \brief A 2D renderer which allows 2D models to be submitted and drawn (Batched)
*
* \author Daniel Bullin
*
*/
#ifndef RENDERER2D_H
#define RENDERER2D_H

#include "independent/rendering/renderers/rendererData.h"
#include "independent/entities/components/text.h"
#include "independent/entities/components/meshRender2D.h"

namespace Engine
{
	/*! \class Renderer2D
	* \brief A static class which renders 2D geometry (Batched)
	*/
	class Renderer2D
	{
	private:
		static Renderer2DData* s_rendererData; //!< Internal data for the 2D renderer
		static ShaderProgram* s_overridingShader; //!< A pointer to the shader program to override material shader
	public:
		static void initialise(const uint32_t batchCapacity); //!< Initialise the renderer
		static void begin(ShaderProgram* shaderProgram, const SceneWideUniforms& sceneWideUniforms); //!< Begin a new 2D scene
		static void submit(Material* material, const glm::mat4& modelMatrix, const glm::vec4& tint = glm::vec4(-1.f, -1.f, -1.f, -1.f)); //!< Submit a 2D entity
		static void submitText(Text* text, const glm::mat4& modelMatrix); //!< Submit text to renderer
		static void flush(); //!< Flush the scene
		static void end(); //!< End the 2D scene
		static void destroy(); //!< Destroy all internal data

		static void setTextureUnitManager(const std::shared_ptr<TextureUnitManager>& unitManager, const std::array<int32_t, 16>& unit); //!< Set the texture unit manager and units to use
	};
}
#endif
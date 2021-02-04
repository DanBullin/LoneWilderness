/*! \file renderer3D.h
*
* \brief A 3D renderer which allows 3D models to be submitted and drawn (Batched)
*
* \author Daniel Bullin
*
*/
#ifndef RENDERER3D_H
#define RENDERER3D_H

#include "independent/rendering/renderers/rendererData.h"
#include "independent/entities/components/meshRender3D.h"

namespace Engine
{
	/*! \class Renderer3D
	* \brief A static class which renders 3D geometry (Batched)
	*/
	class Renderer3D
	{
	private:
		static Renderer3DData* s_rendererData; //!< The internal rendering data
		static ShaderProgram* s_overridingShader; //!< A pointer to the shader program to override material shader

		static void clearBatch(); //!< Clear current batch
	public:
		static void initialise(const uint32_t batchCapacity, const uint32_t vertexCapacity, const uint32_t indexCapacity); //!< Initialise the renderer
		static void begin(ShaderProgram* shaderProgram, const SceneWideUniforms& sceneWideUniforms); //!< Begin a new 3D scene
		static void submit(Geometry3D geometry, Material* material, const glm::mat4& modelMatrix); //!< Submit a piece of geometry to render

		static void flushBatch(); //!< Flush the current batch queue
		static void flushBatchCommands(ShaderProgram* shader, uint32_t instanceCount); //!< Flush the current batch commands
		static void end(); //!< End the current 3D scene
		static void destroy(); //!< Destroy all internal data

		static void addGeometry(std::vector<Vertex3D>& vertices, std::vector<uint32_t> indices, Geometry3D& geometry); //!< Add a piece of 3D geometry to the renderer's vertex buffer
		static void setTextureUnitManager(const std::shared_ptr<TextureUnitManager>& unitManager, const std::array<int32_t, 16>& unit); //!< Set the texture unit manager and units to use
	};
}
#endif
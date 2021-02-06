/*! \file rendererData.h
*
* \brief The internal data for a renderer
*
* \author DMU Course material
*
*/
#ifndef RENDERERDATA_H
#define RENDERERDATA_H

#include "independent/core/common.h"
#include "independent/rendering/geometry/vertex.h"
#include "independent/rendering/geometry/vertexArray.h"
#include "independent/rendering/geometry/indirectBuffer.h"
#include "independent/rendering/uniformBuffer.h"
#include "independent/rendering/textures/textureUnitManager.h"
#include "independent/rendering/renderUtils.h"
#include "independent/rendering/geometry/mesh3D.h"
#include "independent/rendering/materials/material.h"

namespace Engine
{
#define TEXTURELIMIT 4

	using SceneWideUniforms = std::unordered_map<const char*, std::shared_ptr<UniformBuffer>>; //!< Name alias

	/*! \struct BatchQueueEntry
	* \brief A struct containing the 3D submission data
	*/
	struct BatchQueueEntry
	{
		Geometry3D geometry; //!< The geometry
		Material* material; //!< The material
		glm::mat4 transformationMatrix; //!< The transformation matrix
	};

	/*! \struct Renderer3DData
	* \brief A struct containing internal 3D renderer data.
	*/
	struct Renderer3DData
	{
		SceneWideUniforms sceneWideUniforms; //!< Scene wide uniforms, this will be the list of UBOs

		uint32_t batchCapacity; //!< The limit of submissions in a batch before we need to flush
		uint32_t vertexCapacity; //!< The limit of the number of vertices in the vertex buffer
		uint32_t indexCapacity; //!< The limit of the number indices in the index buffer
		uint32_t nextVertex; //!< The next vertex in the vertex buffer where we can add new vertices
		uint32_t nextIndex; //!< The next index in the index buffer where we can add new indices

		std::shared_ptr<VertexArray> VAO; //!< The VAO of the entire 3D geometry currently loaded
		std::shared_ptr<IndirectBuffer> commands; //!< The indirect buffer containing all batch commands
		std::map<ShaderProgram*, std::vector<BatchQueueEntry>> batchQueue; //!< The list of submitted geometry in a queue seperated in shader categories
		std::vector<DrawElementsIndirectCommand> batchCommandsQueue; //!< The list of batch commands

		std::vector<glm::mat4> modelInstanceData; //!< The model matrix instance data
		std::vector<uint32_t> texUnitInstanceData; //!< The texture unit instance data
		std::vector<uint32_t> tintInstanceData; //!< The tint instance data
		std::vector<glm::vec4> subTextureUVs; //!< The Start and End UV coordinates of the subtexture
		std::vector<uint32_t> cubeTexUnitInstanceData; //!< The cubemap unit instance data

		std::shared_ptr<TextureUnitManager> unitManager; //!< The texture unit manager
		std::array<int32_t, 16> unit; //!< The texture units
	};

	/*! \struct Renderer2DData
	* \brief A struct containing internal 2D renderer data.
	*/
	struct Renderer2DData
	{
		uint32_t batchCapacity; //!< Batch capacity for 2D rendering (Number of Quads)
		uint32_t drawCount; //!< The current draw count in queue (Number of vertices filled in the vertices vector)

		std::vector<Vertex2D> vertices; //!< The list of vertices

		std::shared_ptr<VertexArray> VAO; //!< The VAO of the 2D quad

		std::shared_ptr<TextureUnitManager> unitManager; //!< The texture unit manager
		std::array<int32_t, 16> unit; //!< The texture unit
	};
}
#endif
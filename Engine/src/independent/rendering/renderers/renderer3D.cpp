/*! \file renderer3D.cpp
*
* \brief A 3D renderer which allows 3D models to be submitted and drawn (Batched)
*
* \author Daniel Bullin
*
*/

#include "independent/rendering/renderers/renderer3D.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/resourceManager.h"

namespace Engine
{
	Renderer3DData* Renderer3D::s_rendererData = nullptr; //!< Initialise with null pointer
	ShaderProgram* Renderer3D::s_overridingShader = nullptr; //!< Initialise with null pointer

	//! clearBatch()
	void Renderer3D::clearBatch()
	{
		s_rendererData->modelInstanceData.clear();
		s_rendererData->texUnitInstanceData.clear();
		s_rendererData->tintInstanceData.clear();
		s_rendererData->subTextureUVs.clear();

		for (auto& command : s_rendererData->batchCommandsQueue)
			command.InstanceCount = 0;
	}

	//! initialise()
	/*!
	\param batchCapacity a const uint32_t - The total number of 3D geometry submissions before a flush occurs
	\param vertexCapacity a const uint32_t - The total number of vertices allowed in the vertex buffer
	\param indexCapacity a const uint32_t - The total number of indices allowed in the index buffer
	*/
	void Renderer3D::initialise(const uint32_t batchCapacity, const uint32_t vertexCapacity, const uint32_t indexCapacity)
	{
		ENGINE_INFO("[Renderer3D::initialise] Initialising the 3D renderer.");
		// Initialise new renderer data
		s_rendererData = new Renderer3DData;
		// Set capacity values
		s_rendererData->batchCapacity = batchCapacity;
		s_rendererData->vertexCapacity = vertexCapacity;
		s_rendererData->indexCapacity = indexCapacity;
		s_rendererData->nextVertex = 0;
		s_rendererData->nextIndex = 0;

		// Reserve sizes
		s_rendererData->modelInstanceData.reserve(batchCapacity);
		s_rendererData->texUnitInstanceData.reserve(batchCapacity);
		s_rendererData->tintInstanceData.reserve(batchCapacity);
		s_rendererData->subTextureUVs.reserve(batchCapacity);

		// Set up the vertex array
		s_rendererData->VAO.reset(VertexArray::create());

		// Create vertex and index buffer
		std::shared_ptr<VertexBuffer> verticesVBO;
		std::shared_ptr<IndexBuffer> indicesBO;

		// Layout of a Vertex3D
		VertexBufferLayout layout = { {ShaderDataType::Float3, ShaderDataType::Float2, { ShaderDataType::Short3, true }, { ShaderDataType::Short3, true }, { ShaderDataType::Short3, true } }, sizeof(Vertex3D) };

		verticesVBO.reset(VertexBuffer::create(nullptr, static_cast<const uint32_t>(s_rendererData->vertexCapacity * sizeof(Vertex3D)), layout, VertexBufferUsage::StaticDraw));
		indicesBO.reset(IndexBuffer::create(nullptr, static_cast<const uint32_t>(s_rendererData->indexCapacity)));

		s_rendererData->VAO->addVertexBuffer(verticesVBO);
		s_rendererData->VAO->setIndexBuffer(indicesBO);

		// Add Instance buffers to array

		// Model matrix which is a mat4
		std::shared_ptr<VertexBuffer> modelVBO;
		VertexBufferLayout modelLayout = { {ShaderDataType::Mat4, false, 1 } };
		modelVBO.reset(VertexBuffer::create(nullptr, batchCapacity * sizeof(glm::mat4), modelLayout, VertexBufferUsage::StaticDraw));
		s_rendererData->VAO->addVertexBuffer(modelVBO);

		// Texture unit which is an integer
		std::shared_ptr<VertexBuffer> textureUnit1VBO;
		VertexBufferLayout unitLayout = { {ShaderDataType::FlatInt, false, 1 }, {ShaderDataType::FlatInt, false, 1 }, {ShaderDataType::FlatInt, false, 1 }, {ShaderDataType::FlatInt, false, 1 } };
		textureUnit1VBO.reset(VertexBuffer::create(nullptr, batchCapacity * (sizeof(uint32_t) * 4), unitLayout, VertexBufferUsage::StaticDraw));
		s_rendererData->VAO->addVertexBuffer(textureUnit1VBO);

		// Tint which is a unsigned 32bit integer (4 bytes normalised)
		std::shared_ptr<VertexBuffer> tintVBO;
		VertexBufferLayout tintLayout = { {ShaderDataType::Byte4, true, 1 } };
		tintVBO.reset(VertexBuffer::create(nullptr, batchCapacity * sizeof(uint32_t), tintLayout, VertexBufferUsage::StaticDraw));
		s_rendererData->VAO->addVertexBuffer(tintVBO);

		// The SubTexture UVs to recalculate geomery UVs in the shader program
		std::shared_ptr<VertexBuffer> subTextureUVVBO;
		VertexBufferLayout subTextureLayout = { { ShaderDataType::Float4, true, 1 } };
		subTextureUVVBO.reset(VertexBuffer::create(nullptr, batchCapacity * sizeof(glm::vec4), subTextureLayout, VertexBufferUsage::StaticDraw));
		s_rendererData->VAO->addVertexBuffer(subTextureUVVBO);

		// Create indirect buffer
		s_rendererData->commands.reset(IndirectBuffer::create(nullptr, batchCapacity));

		// Unbind VAO for safety
		s_rendererData->VAO->unbind();

	}

	//! begin()
	/*!
	\param shaderProgram a ShaderProgram* - A pointer to the shader program
	\param sceneWideUniforms a const SceneWideUniforms& - A reference to the scene wide uniforms
	*/
	void Renderer3D::begin(ShaderProgram* shaderProgram, const SceneWideUniforms& sceneWideUniforms)
	{
		// FBO Already bound by this point

		s_overridingShader = shaderProgram;
		s_rendererData->sceneWideUniforms = sceneWideUniforms;
	}

	//! submit()
	/*!
	\param geometry a Geometry3D - A piece of 3D geometry
	\param material a Material* - A pointer to a material
	\param modelMatrix a const glm::mat4& - A model matrix
	*/
	void Renderer3D::submit(Geometry3D geometry, Material* material, const glm::mat4& modelMatrix)
	{
		// Check queue size and add submission

		// Get total submission count for all shaders, etc
		uint32_t submissionCount = 0;

		// Count all current submissions in the current queue
		for (auto& shader : s_rendererData->batchQueue)
			submissionCount += static_cast<uint32_t>(shader.second.size());

		// If the count has reached capacity, flush
		if (submissionCount >= s_rendererData->batchCapacity)
			flushBatch();
		else
		{
			ShaderProgram* shader;
			// Add geometry render under material shader or if an overriding shader has been set, under that shader
			if (s_overridingShader) shader = s_overridingShader;
			else shader = material->getShader();

			// If new shader entry, reserve batch queue size
			if (s_rendererData->batchQueue.find(shader) == s_rendererData->batchQueue.end())
				s_rendererData->batchQueue[shader].reserve(s_rendererData->batchCapacity);

			// Add to the back of the batch queue for this entry's material shader
			s_rendererData->batchQueue[shader].push_back({ geometry, material, modelMatrix });
		}
	}

	//! flushBatch()
	void Renderer3D::flushBatch()
	{
		// Sort the queue by geometry ID
		for (auto& shaderEntry : s_rendererData->batchQueue)
		{
			std::sort(shaderEntry.second.begin(), shaderEntry.second.end(),
				[](BatchQueueEntry& a, BatchQueueEntry& b)
			{
				return a.geometry.ID < b.geometry.ID;
			}
			);
		}

		uint32_t runningInstanceCount = 0;
		int32_t unit = 0;

		// Go through each shader entry
		for (auto& shaderEntry : s_rendererData->batchQueue)
		{
			ShaderProgram* shader = shaderEntry.first;
			// For each geometry entry for this shader
			for (auto& geomEntry : shaderEntry.second)
			{
				auto textures = geomEntry.material->getSubTextures();

				// If texture unit manager is full, flush current batch and start fresh
				if (s_rendererData->unitManager->getRemainingUnitCount() < TEXTURELIMIT)
				{
					flushBatchCommands(shader, runningInstanceCount);
					runningInstanceCount = 0;
					clearBatch();
				}

				// Bind textures
				for (int i = 0; i < TEXTURELIMIT; i++)
				{
					if (i < textures.size())
					{
						if (s_rendererData->unitManager->getUnit(textures[i]->getBaseTexture()->getID(), unit))
							s_rendererData->unitManager->bindToUnit(textures[i]->getBaseTexture());

						s_rendererData->texUnitInstanceData.push_back(unit);
					}
					else
						s_rendererData->texUnitInstanceData.push_back(0);
				}

				auto& index = geomEntry.geometry.ID;

				// If this geometry is being rendered for the first time, set the command queue values
				if (s_rendererData->batchCommandsQueue.at(index).DrawCount == 0)
				{
					s_rendererData->batchCommandsQueue.at(index).DrawCount = geomEntry.geometry.IndexCount;
					s_rendererData->batchCommandsQueue.at(index).FirstIndex = geomEntry.geometry.FirstIndex;
					s_rendererData->batchCommandsQueue.at(index).FirstVertex = geomEntry.geometry.FirstVertex;
					s_rendererData->batchCommandsQueue.at(index).FirstInstance = runningInstanceCount;
				}

				// Increase instance count
				s_rendererData->batchCommandsQueue.at(index).InstanceCount++;
				runningInstanceCount++;

				// Add instance data for this submission
				s_rendererData->modelInstanceData.push_back(geomEntry.transformationMatrix);
				s_rendererData->tintInstanceData.push_back(MemoryUtils::pack(geomEntry.material->getTint()));
				s_rendererData->subTextureUVs.push_back(glm::vec4(geomEntry.material->getSubTexture(0)->getUVStart().x, 
																  geomEntry.material->getSubTexture(0)->getUVStart().y,
																  geomEntry.material->getSubTexture(0)->getUVEnd().x,
																  geomEntry.material->getSubTexture(0)->getUVEnd().y));
			}

			// All submissions for one shader entry done, flush if we have something to flush
			if (runningInstanceCount > 0)
			{
				flushBatchCommands(shaderEntry.first, runningInstanceCount);
				runningInstanceCount = 0;
				clearBatch();
			}
		}
		
		// Clear all render data for this current run
		s_rendererData->batchQueue.clear();
		clearBatch();
	}

	//! flushBatchCommands()
	/*!
	\param shader a ShaderProgram* - A pointer to the shader to use
	\param instanceCount an uint32_t - The number of submissions we're drawing
	*/
	void Renderer3D::flushBatchCommands(ShaderProgram* shader, uint32_t instanceCount)
	{
		shader->start();

		// Bind the uniform buffer to the current shader
		for (auto& dataPair : s_rendererData->sceneWideUniforms)
		{
			const char* nameOfUniformBlock = dataPair.first;
			dataPair.second->attachShaderBlock(shader, nameOfUniformBlock);
		}

		// Upload texture units
		shader->sendIntArray("u_diffuseMap", s_rendererData->unit.data(), 16);

		// Edit all instance data in GPU memory
		s_rendererData->VAO->getVertexBuffers().at(1)->edit(s_rendererData->modelInstanceData.data(), sizeof(glm::mat4) * instanceCount, 0);
		s_rendererData->VAO->getVertexBuffers().at(2)->edit(s_rendererData->texUnitInstanceData.data(), (sizeof(uint32_t) * 4) * instanceCount, 0);
		s_rendererData->VAO->getVertexBuffers().at(3)->edit(s_rendererData->tintInstanceData.data(), sizeof(uint32_t) * instanceCount, 0);
		s_rendererData->VAO->getVertexBuffers().at(4)->edit(s_rendererData->subTextureUVs.data(), sizeof(glm::vec4) * instanceCount, 0);
		
		// Bind VAO
		s_rendererData->VAO->bind();
		// Edit Indirect buffer
		s_rendererData->commands->edit(s_rendererData->batchCommandsQueue.data(), static_cast<uint32_t>(s_rendererData->batchCommandsQueue.size()), 0);

		// Draw
		RenderUtils::drawMultiIndirect(static_cast<uint32_t>(s_rendererData->batchCommandsQueue.size()));
	}

	//! end()
	void Renderer3D::end()
	{
		// If we've accumulated any submissions at the end of the frame, let's flush
		if (s_rendererData->batchQueue.size() > 0)
			flushBatch();
	}

	//! destroy()
	void Renderer3D::destroy()
	{
		ENGINE_INFO("[Renderer3D::destroy] Destroying the 3D renderer.");
		// Clean up renderer data
		delete s_rendererData;
		s_rendererData = nullptr;
		s_overridingShader = nullptr;
	}

	//! addGeometry()
	/*!
	\param vertices a std::vector<Vertex3D>& - The list of vertices
	\param indices a std::vector<uint32_t>& - The list of indices
	\param geometry a Geometry3D& - A reference to the model's geometry
	*/
	void Renderer3D::addGeometry(std::vector<Vertex3D>& vertices, std::vector<uint32_t> indices, Geometry3D& geometry)
	{
		uint32_t vertexCount = static_cast<uint32_t>(vertices.size()); // The total number of vertices we're adding
		uint32_t indexCount = static_cast<uint32_t>(indices.size()); // The total number of indices we're adding

		// Check if adding this geometry goes over buffer capacity
		// Total number + new amount > buffer capacity
		if (s_rendererData->nextVertex + vertexCount > s_rendererData->vertexCapacity)
		{
			ENGINE_ERROR("[Renderer3D::addGeometry] Cannot add geometry as vertex buffer limit has been reached.");
			return;
		}
		if (s_rendererData->nextIndex + indexCount > s_rendererData->indexCapacity)
		{
			ENGINE_ERROR("[Renderer3D::addGeometry] Cannot add geometry as index buffer limit has been reached.");
			return;
		}

		// Capacity wont be reached, so add the data to the buffer
		auto VBO = s_rendererData->VAO->getVertexBuffers().at(0);
		auto IBO = s_rendererData->VAO->getIndexBuffer();

		VBO->edit(vertices.data(), vertexCount * sizeof(Vertex3D), s_rendererData->nextVertex * sizeof(Vertex3D));
		IBO->edit(indices.data(), indexCount, s_rendererData->nextIndex);

		// Create geometry data
		geometry.ID = static_cast<uint32_t>(s_rendererData->batchCommandsQueue.size());
		geometry.FirstVertex = s_rendererData->nextVertex;
		geometry.FirstIndex = s_rendererData->nextIndex;
		geometry.VertexCount = vertexCount;
		geometry.IndexCount = indexCount;

		// Update the next vertex and index position
		s_rendererData->nextVertex += vertexCount;
		s_rendererData->nextIndex += indexCount;
		s_rendererData->batchCommandsQueue.push_back({ 0, 0, 0, 0, 0 });
	}

	//! setTextureUnitManager()
	/*!
	\param unitManager a const std::shared_ptr<TextureUnitManager>& - A reference to the texture unit manager
	\param unit a const std::array<int32_t, 16>& - A reference to the array of units
	*/
	void Renderer3D::setTextureUnitManager(const std::shared_ptr<TextureUnitManager>& unitManager, const std::array<int32_t, 16>& unit)
	{
		s_rendererData->unitManager = unitManager;
		s_rendererData->unit = unit;
	}
}
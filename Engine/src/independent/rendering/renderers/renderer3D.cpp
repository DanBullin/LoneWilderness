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
	uint32_t Renderer3D::s_batchCapacity = 0; //!< Initialise to 0
	uint32_t Renderer3D::s_vertexCapacity = 0; //!< Initialise to 0
	uint32_t Renderer3D::s_indexCapacity = 0; //!< Initialise to 0
	std::map<VertexBuffer*, uint32_t> Renderer3D::s_nextVertex; //!< Initialise to 0
	uint32_t Renderer3D::s_nextIndex = 0; //!< Initialise to 0
	TextureUnitManager* Renderer3D::s_unitManager = nullptr; //!< Initialise to null pointer
	std::array<int32_t, 16> Renderer3D::s_unit; //!< Initialise to empty list
	IndirectBuffer* Renderer3D::s_indirectBuffer = nullptr; //!< Initialise to null pointer
	std::map<VertexBuffer*, std::vector<DrawElementsIndirectCommand>> Renderer3D::s_batchCommandsQueue; //!< Initialise to empty list
	std::map<ShaderProgram*, std::vector<BatchEntry3D>> Renderer3D::s_batchQueue; //!< Initialise to empty map

	//! clearBatch()
	void Renderer3D::clearBatch()
	{
		for (auto& buffer : s_batchCommandsQueue)
		{
			for (auto& command : buffer.second)
				command.InstanceCount = 0;
		}
	}

	//! generateInstanceData()
	/*
	\param shader a ShaderProgram* - A pointer to the shader program
	\param batchEntries a std::vector<BatchEntry3D>& - A list of batch entries
	*/
	void Renderer3D::generateInstanceData(ShaderProgram* shader, std::vector<BatchEntry3D>& batchEntries, const uint32_t instanceCount)
	{
		// Generate a list of the instance data and edit the appropriate vertex buffers in the vao

		if (shader->getVertexArray() == ResourceManager::getResource<VertexArray>("vertexArray1"))
			generateBasic3D(shader, batchEntries, instanceCount);
		else if (shader->getVertexArray() == ResourceManager::getResource<VertexArray>("vertexArray2"))
			generateSkybox(shader, batchEntries, instanceCount);
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

		// Set capacity values
		s_batchCapacity = batchCapacity;
		s_vertexCapacity = vertexCapacity;
		s_indexCapacity = indexCapacity;

		// Set the indirect buffer by retrieving it from the resource manager
		s_indirectBuffer = ResourceManager::getResourceAndRef<IndirectBuffer>("indirectBuffer");
	}

	//! begin()
	void Renderer3D::begin()
	{
		// FBO Already bound by this point
	}

	//! submit()
	/*!
	\param geometry a Geometry3D - A piece of 3D geometry
	\param material a Material* - A pointer to a material
	\param modelMatrix a const glm::mat4& - A model matrix
	*/
	void Renderer3D::submit(Geometry3D geometry, Material* material, const glm::mat4& modelMatrix)
	{
		// Submit the geometry

		// Check what shader we're submitting with
		ShaderProgram* shader;
		shader = material->getShader();

		// Now we have the shader to submit our 3D geometry under, check if that shader's list of submissions is full or would be full if we were to submit
		if (s_batchQueue[shader].size() >= s_batchCapacity)
		{
			// Our number of entries under this shader program matches or exceeds the number of 3D submissions we've set as our limit
			// So flush the current contents of the queue
			flushBatch();
		}
		else
		{
			// Get a vector of all the subtextures in the material
			std::vector<SubTexture*> tmpList;
			tmpList.reserve(material->getSubTextures().size());
			for (auto& subtexture : material->getSubTextures())
				tmpList.push_back(subtexture);

			// Get a vector of all the cubemap textures in the material
			std::vector<CubeMapTexture*> tmpCubeList;
			tmpCubeList.reserve(material->getCubemapTextures().size());
			for (auto& cubeTexture : material->getCubemapTextures())
				tmpCubeList.push_back(cubeTexture);

			std::vector<int32_t> units;
			units.resize(tmpList.size());

			std::vector<int32_t> cubeUnits;
			cubeUnits.resize(tmpCubeList.size());

			s_batchQueue[shader].push_back({ geometry, tmpList, tmpCubeList, units, cubeUnits, modelMatrix, material->getTint() });
		}
	}

	//! flushBatch()
	void Renderer3D::flushBatch()
	{
		// Sort the queue by geometry ID
		for (auto& batchEntry : s_batchQueue)
		{
			std::sort(batchEntry.second.begin(), batchEntry.second.end(),
				[](BatchEntry3D& a, BatchEntry3D& b)
			{
				return a.geometry.ID < b.geometry.ID;
			}
			);
		}

		uint32_t runningInstanceCount = 0;
		int32_t unit = 0;

		// Go through each shader entry
		for (auto& shaderEntry : s_batchQueue)
		{
			ShaderProgram* shader = shaderEntry.first;
			// For each geometry entry for this shader
			for (auto& geomEntry : shaderEntry.second)
			{
				// Bind Textures

				// Check if we can bind the textures first
				if (s_unitManager->getRemainingUnitCount() < geomEntry.subTextures.size() + geomEntry.cubeTextures.size())
				{
					// If we cannot bind all the textures, lets draw the current queue to free the unit manager up
					flushBatchCommands(shaderEntry.first, runningInstanceCount);
					runningInstanceCount = 0;
					clearBatch();
				}

				// Unit manager can bind textures now
				int32_t unit = 0;
				for (int i = 0; i < geomEntry.subTextures.size(); i++)
				{
					// For each subtexture, lets bind the texture to a unit
					if (s_unitManager->getUnit(geomEntry.subTextures[i]->getBaseTexture()->getID(), unit))
						s_unitManager->bindToUnit(geomEntry.subTextures[i]->getBaseTexture());

					geomEntry.textureUnits.at(i) = unit;
				}

				for (int i = 0; i < geomEntry.cubeTextures.size(); i++)
				{
					// For each cubemap texture, lets bind the texture to a unit
					if (s_unitManager->getUnit(geomEntry.cubeTextures[i]->getID(), unit))
						s_unitManager->bindToUnit(geomEntry.cubeTextures[i]);

					geomEntry.cubeTextureUnits.at(i) = unit;
				}

				auto& index = geomEntry.geometry.ID;
				auto& commands = s_batchCommandsQueue[shaderEntry.first->getVertexArray()->getVertexBuffers().at(0)][index];
				// If this geometry is being rendered for the first time, set the command queue values
				if (commands.DrawCount == 0)
				{
					commands.DrawCount = geomEntry.geometry.IndexCount;
					commands.FirstIndex = geomEntry.geometry.FirstIndex;
					commands.FirstVertex = geomEntry.geometry.FirstVertex;
					commands.FirstInstance = runningInstanceCount;
				}

				// Increase instance count
				commands.InstanceCount++;
				runningInstanceCount++;
			}

			// Set the instance VBOs
			generateInstanceData(shaderEntry.first, shaderEntry.second, runningInstanceCount);

			// All submissions for one shader entry done, flush if we have something to flush
			if (runningInstanceCount > 0)
			{
				flushBatchCommands(shader, runningInstanceCount);
				runningInstanceCount = 0;
				clearBatch();
			}
		}

		// Clear all render data for this current run
		s_batchQueue.clear();
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

		// Attach the UBO
		for (auto& dataPair : shader->getUniformBuffers())
		{
			const char* nameOfUniformBlock = dataPair.first.c_str();
			dataPair.second->attachShaderBlock(shader, nameOfUniformBlock);
		}

		// Upload texture units
		auto uniforms = shader->getUniforms();
		if (uniforms.find("u_diffuseMap") != uniforms.end()) shader->sendIntArray("u_diffuseMap", s_unit.data(), 16);
		if (uniforms.find("u_cubeMap") != uniforms.end()) shader->sendIntArray("u_cubeMap", s_unit.data(), 16);

		// Bind VAO
		shader->getVertexArray()->bind();
		// Edit Indirect buffer
		auto& commands = s_batchCommandsQueue[shader->getVertexArray()->getVertexBuffers().at(0)];
		s_indirectBuffer->edit(commands.data(), static_cast<uint32_t>(commands.size()), 0);

		// Draw
		RenderUtils::drawMultiIndirect(static_cast<uint32_t>(commands.size()));
	}

	//! end()
	void Renderer3D::end()
	{
		if (s_batchQueue.size() > 0)
		{
			flushBatch();
		}
	}

	//! destroy()
	void Renderer3D::destroy()
	{
		ENGINE_INFO("[Renderer3D::destroy] Destroying the 3D renderer.");
		// Clean up renderer data
		if (s_indirectBuffer) s_indirectBuffer->decreaseCount();
		s_indirectBuffer = nullptr;

		s_unitManager = nullptr;
		s_batchQueue.clear();
		s_batchCommandsQueue.clear();
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

		auto VBO = geometry.VertexBuffer;
		auto IBO = ResourceManager::getResource<IndexBuffer>("IndexBuffer3D");

		if (s_nextVertex.find(VBO) == s_nextVertex.end())
			s_nextVertex[VBO] = 0;

		// Check if adding this geometry goes over buffer capacity
		// Total number + new amount > buffer capacity
		if (s_nextVertex[VBO] + vertexCount > ResourceManager::getConfigValue(ConfigData::VertexCapacity3D))
		{
			ENGINE_ERROR("[Renderer3D::addGeometry] Cannot add geometry as vertex buffer limit has been reached. VBO Name: {0}.", VBO->getName());
			return;
		}
		if (s_nextIndex + indexCount > ResourceManager::getConfigValue(ConfigData::IndexCapacity3D))
		{
			ENGINE_ERROR("[Renderer3D::addGeometry] Cannot add geometry as index buffer limit has been reached. IBO Name: {0}.", IBO->getName());
			return;
		}

		// Capacity wont be reached, so add the data to the buffer
		VBO->edit(vertices.data(), static_cast<uint32_t>(vertices.size()) * sizeof(Vertex3D), s_nextVertex[VBO] * sizeof(Vertex3D));
		IBO->edit(indices.data(), indexCount, s_nextIndex);

		// Create geometry data
		geometry.ID = static_cast<uint32_t>(s_batchCommandsQueue[VBO].size());
		geometry.FirstVertex = s_nextVertex[VBO];
		geometry.FirstIndex = s_nextIndex;
		geometry.VertexCount = vertexCount;
		geometry.IndexCount = indexCount;

		// Update the next vertex and index position
		s_nextVertex[VBO] += vertexCount;
		s_nextIndex += indexCount;
		s_batchCommandsQueue[VBO].push_back({ 0, 0, 0, 0, 0 });
	}

	//! setTextureUnitManager()
	/*!
	\param unitManager a TextureUnitManager*& - A reference to the texture unit manager
	\param unit a const std::array<int32_t, 16>& - A reference to the array of units
	*/
	void Renderer3D::setTextureUnitManager(TextureUnitManager*& unitManager, const std::array<int32_t, 16>& unit)
	{
		s_unitManager = unitManager;
		s_unit = unit;
	}

	//! generateBasic3D()
	/*
	\param shader a ShaderProgram* - A pointer to the shader program
	\param batchEntries a std::vector<BatchEntry3D>& - A list of batch entries
	*/
	void Renderer3D::generateBasic3D(ShaderProgram* shader, std::vector<BatchEntry3D>& batchEntries, const uint32_t instanceCount)
	{
		std::vector<glm::mat4> modelInstanceData; //!< The model matrix instance data
		std::vector<uint32_t> tintInstanceData; //!< The tint instance data
		std::vector<int32_t> unitInstanceData; //!< The texture unit data
		std::vector<glm::vec4> subTextureUVs; //!< The Start and End UV coordinates of the subtexture

		for (auto& entry : batchEntries)
		{
			modelInstanceData.push_back(entry.modelMatrix);
			unitInstanceData.push_back(entry.textureUnits[0]);
			tintInstanceData.push_back(MemoryUtils::pack(entry.tint));
			subTextureUVs.push_back(glm::vec4(entry.subTextures.at(0)->getUVStart().x, entry.subTextures.at(0)->getUVStart().y,
				entry.subTextures.at(0)->getUVEnd().x, entry.subTextures.at(0)->getUVEnd().y));
		}

		shader->getVertexArray()->getVertexBuffers().at(1)->edit(modelInstanceData.data(), sizeof(glm::mat4) * instanceCount, 0);
		shader->getVertexArray()->getVertexBuffers().at(2)->edit(unitInstanceData.data(), sizeof(int32_t) * instanceCount, 0);
		shader->getVertexArray()->getVertexBuffers().at(3)->edit(tintInstanceData.data(), sizeof(uint32_t) * instanceCount, 0);
		shader->getVertexArray()->getVertexBuffers().at(4)->edit(subTextureUVs.data(), sizeof(glm::vec4) * instanceCount, 0);
	}

	//! generateSkybox()
	/*
	\param shader a ShaderProgram* - A pointer to the shader program
	\param batchEntries a std::vector<BatchEntry3D>& - A list of batch entries
	*/
	void Renderer3D::generateSkybox(ShaderProgram* shader, std::vector<BatchEntry3D>& batchEntries, const uint32_t instanceCount)
	{
		std::vector<uint32_t> tintInstanceData; //!< The tint instance data
		std::vector<int32_t> unitInstanceData; //!< The texture unit data

		for (auto& entry : batchEntries)
		{
			unitInstanceData.push_back(entry.cubeTextureUnits[0]);
			tintInstanceData.push_back(MemoryUtils::pack(entry.tint));
		}

		shader->getVertexArray()->getVertexBuffers().at(1)->edit(unitInstanceData.data(), sizeof(int32_t) * instanceCount, 0);
		shader->getVertexArray()->getVertexBuffers().at(2)->edit(tintInstanceData.data(), sizeof(uint32_t) * instanceCount, 0);
	}
}
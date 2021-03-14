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
#include "independent/rendering/renderers/utils/fillBuffers.h"

namespace Engine
{
	TextureUnitManager* Renderer3D::s_unitManager = nullptr; //!< Initialise to null pointer
	std::array<int32_t, 16> Renderer3D::s_unit; //!< Initialise to empty list
	uint32_t Renderer3D::s_batchCapacity = 0; //!< Initialise to 0
	uint32_t Renderer3D::s_vertexCapacity = 0; //!< Initialise to 0
	uint32_t Renderer3D::s_indexCapacity = 0; //!< Initialise to 0

	std::vector<BatchEntry3D> Renderer3D::s_batchQueue = std::vector<BatchEntry3D>(); //!< Initialise to empty list
	std::map<VertexBuffer*, std::vector<DrawElementsIndirectCommand>> Renderer3D::s_batchCommandsQueue; //!< Initialise to empty list
	std::map<VertexBuffer*, uint32_t> Renderer3D::s_nextVertex = std::map<VertexBuffer*, uint32_t>(); //!< Initialise to empty list
	uint32_t Renderer3D::s_nextIndex = 0; //!< Initialise to  0
	IndirectBuffer* Renderer3D::s_indirectBuffer = nullptr; //!< Initialise to null pointer

	//! clearBatch()
	void Renderer3D::clearBatch()
	{
		// Clear all render commands
		for (auto& buffer : s_batchCommandsQueue)
		{
			for (auto& command : buffer.second)
			{
				command.InstanceCount = 0;
				command.DrawCount = 0;
				command.FirstIndex = 0;
				command.FirstVertex = 0;
				command.FirstInstance = 0;
			}
		}
	}

	//! initialise()
	/*!
	\param batchCapacity a const uint32_t - The total number of 3D geometry submissions before a flush occurs
	\param vertexCapacity a const uint32_t - The total number of vertices allowed in the vertex buffer
	\param indexCapacity a const uint32_t - The total number of indices allowed in the index buffer
	*/
	void Renderer3D::initialise(const uint32_t batchCapacity, const uint32_t vertexCapacity, const uint32_t indexCapacity)
	{
		ENGINE_TRACE("[Renderer3D::initialise] Initialising the 3D renderer.");

		// Set capacity values
		s_batchCapacity = batchCapacity;
		s_vertexCapacity = vertexCapacity;
		s_indexCapacity = indexCapacity;

		// Set the indirect buffer by retrieving it from the resource manager
		IndirectBuffer* indirectBuffer = IndirectBuffer::create("indirectBuffer", nullptr, batchCapacity);
		s_indirectBuffer = indirectBuffer;

		IndexBuffer* indexBuffer = IndexBuffer::create("IndexBuffer3D", nullptr, indexCapacity);
		ResourceManager::registerResource("IndexBuffer3D", indexBuffer);
	}

	//! begin()
	void Renderer3D::begin()
	{
		s_batchQueue.clear();
	}

	//! submissionChecks()
	/*
	\param material a Material* - A pointer to the material
	\param geom a Geometry3D& - A reference to the geometry
	*/
	bool Renderer3D::submissionChecks(Material* material, Geometry3D& geom)
	{
		if (!material)
		{
			ENGINE_ERROR("[Renderer3D::submissionChecks] An invalid material was provided.");
			return false;
		}

		if (!material->getShader())
		{
			ENGINE_ERROR("[Renderer3D::submissionChecks] An invalid shader is attached to the material that was provided. Material Name: {0}.", material->getName());
			return false;
		}

		if (geom.VertexCount == 0 || geom.IndexCount == 0)
		{
			ENGINE_ERROR("[Renderer3D::submissionChecks] An invalid geometry was provided in the submission.");
			return false;
		}

		for (auto& subTexture : material->getSubTextures())
		{
			if (subTexture)
			{
				if (!subTexture->getBaseTexture())
				{
					ENGINE_ERROR("[Renderer3D::submissionChecks] An invalid base texture was provided for subtexture: {0}.", subTexture->getName());
					return false;
				}
			}
			else
			{
				ENGINE_ERROR("[Renderer3D::submissionChecks] An invalid subtexture was provided.");
				return false;
			}
		}
		return true;
	}

	//! submit()
	/*!
	\param submissionName a const std::string& - The name of the submission
	\param geometry a Geometry3D - A piece of 3D geometry
	\param material a Material* - A pointer to a material
	\param modelMatrix a const glm::mat4& - A model matrix
	*/
	void Renderer3D::submit(const std::string& submissionName, Geometry3D geometry, Material* material, const glm::mat4& modelMatrix)
	{
		// First lets do some error checking
		if (submissionChecks(material, geometry))
		{
			/////
			// Submitting
			/////

			ShaderProgram* shader = material->getShader();

			if (s_batchQueue.size() >= s_batchCapacity)
				flushBatch();
			else
			{
				std::vector<int32_t> units;
				units.resize(material->getSubTextures().size());

				std::vector<int32_t> cubeUnits;
				cubeUnits.resize(material->getCubemapTextures().size());

				s_batchQueue.push_back({ submissionName, geometry, shader, material->getSubTextures(), material->getCubemapTextures(), units, cubeUnits, material->getShininess(), modelMatrix, material->getTint() });
			}
		}
	}

	//! sortSubmissions()
	/*
	\param submissions a std::vector<BatchEntry3D>& - A list of submissions
	*/
	void Renderer3D::sortSubmissions(std::vector<BatchEntry3D>& submissions)
	{
		// Sort by shader importance and by geometry ID
		std::sort(submissions.begin(), submissions.end(),
			[](BatchEntry3D& a, BatchEntry3D& b)
		{
			if (a.shader->getOrderImportance() != b.shader->getOrderImportance()) return a.shader->getOrderImportance() < b.shader->getOrderImportance();
			return a.geometry.ID < b.geometry.ID;
		}
		);
	}

	//! flushBatch()
	void Renderer3D::flushBatch()
	{
		/////
		// SORTING SHADERS
		/////
		sortSubmissions(s_batchQueue);

		/////
		// Begin setting the necassary data
		/////
		std::vector<BatchEntry3D> tmpList;
		ShaderProgram* currentShader = s_batchQueue.at(0).shader;
		uint32_t runningInstanceCount = 0;
		int32_t unit = 0;

		for (auto& submission : s_batchQueue)
		{
			// If we've moved onto a new shader, draw the current list
			if (submission.shader != currentShader)
			{
				// Set the instance VBOs
				generateInstanceData(tmpList);
				flushBatchCommands(tmpList);
				runningInstanceCount = 0;
				clearBatch();
				tmpList.clear();
				currentShader = submission.shader;
			}

			// If we cannot bind the textures for the current submission, draw the current list
			if (s_unitManager->getRemainingUnitCount() < submission.subTextures.size() + submission.cubeTextures.size())
			{
				// Set the instance VBOs
				if (tmpList.size() != 0)
				{
					generateInstanceData(tmpList);
					flushBatchCommands(tmpList);
				}
				runningInstanceCount = 0;
				clearBatch();
				s_unitManager->clear(true);
				tmpList.clear();
			}

			/////
			// Bind all textures and cubetextures to their slot
			/////
			for (int i = 0; i < submission.subTextures.size(); i++)
			{
				// For each subtexture, lets bind the texture to a unit
				if (s_unitManager->getUnit(submission.subTextures[i]->getBaseTexture()->getID(), unit))
					s_unitManager->bindToUnit(submission.subTextures[i]->getBaseTexture());

				submission.textureUnits.at(i) = unit;
			}

			for (int i = 0; i < submission.cubeTextures.size(); i++)
			{
				// For each cubemap texture, lets bind the texture to a unit
				if (s_unitManager->getUnit(submission.cubeTextures[i]->getID(), unit))
					s_unitManager->bindToUnit(submission.cubeTextures[i]);

				submission.cubeTextureUnits.at(i) = unit;
			}

			auto& index = submission.geometry.ID;
			auto& commands = s_batchCommandsQueue[submission.shader->getVertexArray()->getVertexBuffers().at(0)][index];
			// If this geometry is being rendered for the first time, set the command queue values
			if (commands.DrawCount == 0)
			{
				commands.DrawCount = submission.geometry.IndexCount;
				commands.FirstIndex = submission.geometry.FirstIndex;
				commands.FirstVertex = submission.geometry.FirstVertex;
				commands.FirstInstance = runningInstanceCount;
			}

			// Increase instance count
			commands.InstanceCount++;
			runningInstanceCount++;

			tmpList.push_back(submission);
		}

		// Draw anything left in the list
		if (tmpList.size() != 0)
		{
			// Set the instance VBOs
			generateInstanceData(tmpList);
			flushBatchCommands(tmpList);
		}

		// Clear all render data for this current run
		s_batchQueue.clear();
		clearBatch();
		tmpList.clear();
	}

	//! drawCheck()
	/*
	\param program a ShaderProgram* - A pointer to the shader program
	\param buffers a std::unordered_map<std::string, UniformBuffer*>& - A list of uniform buffers
	\param vArray a VertexArray* - A pointer to the vertex array
	\param indirectBuffer an IndirectBuffer* - A pointer to the indirect buffer
	*/
	bool Renderer3D::drawCheck(ShaderProgram* program, std::unordered_map<std::string, UniformBuffer*>& buffers, VertexArray* vArray, IndirectBuffer* indirectBuffer)
	{
		if (!program)
		{
			ENGINE_ERROR("[Renderer3D::drawCheck] An invalid shader was provided.");
			return false;
		}

		for (auto& buffer : buffers)
		{
			if (!buffer.second)
			{
				ENGINE_ERROR("[Renderer3D::drawCheck] An invalid uniform buffer was provided.");
				return false;
			}
		}

		if (!vArray)
		{
			ENGINE_ERROR("[Renderer3D::drawCheck] An invalid vertex array was provided.");
			return false;
		}

		if (!indirectBuffer)
		{
			ENGINE_ERROR("[Renderer3D::drawCheck] An invalid indirect buffer was provided.");
			return false;
		}

		return true;
	}

	//! flushBatchCommands()
	/*!
	\param queue a std::vector<BatchEntry3D>& - The list of geometry submissions for this shader
	*/
	void Renderer3D::flushBatchCommands(std::vector<BatchEntry3D>& queue)
	{
		// Check the variables to make sure they are valid
		if (drawCheck(queue.at(0).shader, queue.at(0).shader->getUniformBuffers(), queue.at(0).shader->getVertexArray(), s_indirectBuffer))
		{
			// Start the shader
			queue.at(0).shader->start();

			// Attach all the UBOs to the shader blocks
			for (auto& dataPair : queue.at(0).shader->getUniformBuffers())
			{
				const char* nameOfUniformBlock = dataPair.first.c_str();
				dataPair.second->attachShaderBlock(queue.at(0).shader, nameOfUniformBlock);
			}

			// Upload texture units
			auto uniforms = queue.at(0).shader->getUniforms();
			if (uniforms.find("u_diffuseMap") != uniforms.end()) queue.at(0).shader->sendIntArray("u_diffuseMap", s_unit.data(), 16);
			if (uniforms.find("u_cubeMap") != uniforms.end()) queue.at(0).shader->sendIntArray("u_cubeMap", s_unit.data(), 16);

			// Bind VAO which provides all of the attributes and the VBOs which provide the data
			VertexArray* vArray = queue.at(0).shader->getVertexArray();
			vArray->bind();

			// Edit Indirect buffer
			auto& commands = s_batchCommandsQueue[vArray->getVertexBuffers().at(0)];
			s_indirectBuffer->edit(commands.data(), static_cast<uint32_t>(commands.size()), 0);

			// Test variables
			auto queueList = queue;
			auto commandQueue = s_batchCommandsQueue[vArray->getVertexBuffers().at(0)];

			// Draw
			RenderUtils::drawMultiIndirect(static_cast<uint32_t>(commands.size()));
		}
	}

	//! end()
	void Renderer3D::end()
	{
		if (s_batchQueue.size() > 0)
			flushBatch();
	}

	//! destroy()
	void Renderer3D::destroy()
	{
		ENGINE_TRACE("[Renderer3D::destroy] Destroying the 3D renderer.");
		// Clean up renderer data
		s_unitManager = nullptr;
		s_batchQueue.clear();
		s_batchCommandsQueue.clear();

		if (s_indirectBuffer) delete s_indirectBuffer;
		s_indirectBuffer = nullptr;

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

		// Check a valid vertex count was provided
		if (vertexCount == 0 || indexCount == 0)
		{
			ENGINE_ERROR("[Renderer3D::addGeometry] An invalid vertex count or index count was provided. Vertex Count: {0}, Index Count: {1}.", vertexCount, indexCount);
			return;
		}

		auto VBO = geometry.VertexBuffer;
		auto IBO = ResourceManager::getResource<IndexBuffer>("IndexBuffer3D");

		// Check if we've seen this VBO before, if not set the next vertex to position 0
		if (s_nextVertex.find(VBO) == s_nextVertex.end())
			s_nextVertex[VBO] = 0;

		// Check if adding this geometry goes over buffer capacity
		// Total number + new amount > buffer capacity
		if (s_nextVertex[VBO] + vertexCount > ResourceManager::getConfigValue(Config::VertexCapacity3D))
		{
			ENGINE_ERROR("[Renderer3D::addGeometry] Cannot add geometry as vertex buffer limit has been reached. VBO Name: {0}.", VBO->getName());
			return;
		}
		if (s_nextIndex + indexCount > ResourceManager::getConfigValue(Config::IndexCapacity3D))
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

	//! addGeometry()
	/*!
	\param vertices a std::vector<TerrainVertex>& - The list of vertices
	\param indices a std::vector<uint32_t>& - The list of indices
	\param geometry a Geometry3D& - A reference to the model's geometry
	*/
	void Renderer3D::addGeometry(std::vector<TerrainVertex>& vertices, std::vector<uint32_t> indices, Geometry3D& geometry)
	{
		uint32_t vertexCount = static_cast<uint32_t>(vertices.size()); // The total number of vertices we're adding
		uint32_t indexCount = static_cast<uint32_t>(indices.size()); // The total number of indices we're adding

		// Check a valid vertex count was provided
		if (vertexCount == 0 || indexCount == 0)
		{
			ENGINE_ERROR("[Renderer3D::addGeometry] An invalid vertex count or index count was provided. Vertex Count: {0}, Index Count: {1}.", vertexCount, indexCount);
			return;
		}

		auto VBO = geometry.VertexBuffer;
		auto IBO = ResourceManager::getResource<IndexBuffer>("IndexBuffer3D");

		// Check if we've seen this VBO before, if not set the next vertex to position 0
		if (s_nextVertex.find(VBO) == s_nextVertex.end())
			s_nextVertex[VBO] = 0;

		// Check if adding this geometry goes over buffer capacity
		// Total number + new amount > buffer capacity
		if (s_nextVertex[VBO] + vertexCount > ResourceManager::getConfigValue(Config::VertexCapacity3D))
		{
			ENGINE_ERROR("[Renderer3D::addGeometry] Cannot add geometry as vertex buffer limit has been reached. VBO Name: {0}.", VBO->getName());
			return;
		}
		if (s_nextIndex + indexCount > ResourceManager::getConfigValue(Config::IndexCapacity3D))
		{
			ENGINE_ERROR("[Renderer3D::addGeometry] Cannot add geometry as index buffer limit has been reached. IBO Name: {0}.", IBO->getName());
			return;
		}

		// Capacity wont be reached, so add the data to the buffer
		VBO->edit(vertices.data(), static_cast<uint32_t>(vertices.size()) * sizeof(TerrainVertex), s_nextVertex[VBO] * sizeof(TerrainVertex));
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
}
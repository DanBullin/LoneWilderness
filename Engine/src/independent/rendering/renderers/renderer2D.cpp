/*! \file renderer2D.cpp
*
* \brief A 2D renderer which allows 2D models to be submitted and drawn (Batched)
*
* \author Daniel Bullin
*
*/
#include "independent/rendering/renderers/renderer2D.h"
#include "independent/rendering/geometry/quad.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/fontManager.h"
#include "independent/systems/systems/sceneManager.h"

namespace Engine
{
	ShaderProgram* Renderer2D::s_overridingShader = nullptr; //!< Initialise with null pointer
	uint32_t Renderer2D::s_batchCapacity = 0; //!< Set to 0
	TextureUnitManager* Renderer2D::s_unitManager = nullptr; //!< Set to null pointer
	std::array<int32_t, 16> Renderer2D::s_unit; //!< Initialise to empty array
	std::map<ShaderProgram*, std::vector<BatchEntry2D>> Renderer2D::s_batchQueue = std::map<ShaderProgram*, std::vector<BatchEntry2D>>(); //!< Initialise to empty list
	uint32_t Renderer2D::s_drawCount = 0; //!< The number of quads we're drawing

	//! initialise()
	/*!
	\param batchCapacity a const uint32_t - The total number of 2D geometry submissions before a flush occurs
	*/
	void Renderer2D::initialise(const uint32_t batchCapacity)
	{
		ENGINE_INFO("[Renderer2D::initialise] Initialising the 2D renderer.");

		// Set the batch capacity
		s_batchCapacity = batchCapacity;

		// Generate all the quads indices and store the resutls
		std::vector<uint32_t> indicesData = getQuadIndices(batchCapacity);
		ResourceManager::getResource<IndexBuffer>("QuadIBuffer")->edit(indicesData.data(), static_cast<uint32_t>(indicesData.size()), 0);
	}

	//! begin()
	/*!
	\param shaderProgram a ShaderProgram* - A pointer to the shader program
	\param sceneWideUniforms a const SceneWideUniforms& - A reference to the scene wide uniforms
	*/
	void Renderer2D::begin(ShaderProgram* shaderProgram)
	{
		// FBO already bound by this point

		// Set the overriding shader program, we can check later if its null to decide whether to use material shader or the overriding one
		s_overridingShader = shaderProgram;
		// Set the draw count to 0
		s_drawCount = 0;
	}

	//! submit()
	/*!
	\param material a Material* - A pointer to a material
	\param modelMatrix a const glm::mat4& - A model matrix
	\param tint a const glm::vec4& - The tint to override the material (Optional)
	*/
	void Renderer2D::submit(Material* material, const glm::mat4& modelMatrix, const glm::vec4& tint)
	{
		// Submit the geometry

		// Check what shader we're submitting with
		ShaderProgram* shader;
		if (s_overridingShader) shader = s_overridingShader;
		else shader = material->getShader();

		// Now we have the shader to submit our 2D geometry under, check if that shader's list of submissions is full or would be full if we were to submit
		if (s_batchQueue[shader].size() >= s_batchCapacity)
		{
			// Our number of entries under this shader program matches or exceeds the number of quads we've set as our limit
			// So flush the current contents of the queue
			flush();
		}
		else
		{
			// Let's do the tint check now, we need to check whether to use the material tint or the tint provided to us
			// This provided tint exists because it allows us to define a tint per Text object rather than material
			// Therefore it is only used in text submissions, ignore for MeshRender2D submissions

			// If the tint values are all -1, then use material tint
			glm::vec4 tmpTint;
			if (tint == glm::vec4(-1.f, -1.f, -1.f, -1.f))
				tmpTint = material->getTint();
			else
				tmpTint = tint;

			// Get a vector of all the subtextures in the material
			std::vector<SubTexture*> tmpList;
			tmpList.reserve(material->getSubTextures().size());
			for (auto& subtexture : material->getSubTextures())
				tmpList.push_back(subtexture);

			std::vector<int32_t> units;
			units.resize(tmpList.size());

			s_batchQueue[shader].push_back( { tmpList, units, modelMatrix, tmpTint } );
			s_drawCount++;
		}
	}

	//! submitText()
	/*!
	\param text a Text* - A pointer to some text
	\param modelMatrix a const glm::mat4& - A model matrix
	*/
	void Renderer2D::submitText(Text* text, const glm::mat4& modelMatrix)
	{
		// Get the length of the text we're rendering
		uint32_t len = static_cast<uint32_t>(strlen(text->getText().c_str()));

		// Text will only ever use one specific material, so lets check that from the resource manager
		Material* material = ResourceManager::getResource<Material>("textMaterial");

		// Get x position of the entity
		float advance = 0.f, x = modelMatrix[3][0];

		// Get the font for this text render
		auto font = FontManager::getFont(text->getFont().c_str());

		// For each character we want to render
		for (int i = 0; i < static_cast<int>(len); i++)
		{
			// Get the character by indexing
			char ch = text->getText().at(i);

			// Check whether this character has a glyph data associated with it, we check this by making sure the character is in between the range of the
			// first and last glyph
			if (ch > font->getFirstGlyph() && ch <= font->getLastGlyph())
			{
				// Get the character's glyph data from the font
				GlyphData& gd = font->getGlyphData().at(ch - font->getFirstGlyph());

				// Create the position and size of the text quad
				glm::vec2 pos = { x, modelMatrix[3][1] };
				glm::vec2 glyphHalfExtents(gd.size * 0.5f);
				glm::vec2 glyphCentre = (pos + gd.bearing) + glyphHalfExtents;

				advance = gd.advance;

				// Create a new model matrix
				glm::mat4 model = glm::mat4(1.f);
				model = glm::translate(model, glm::vec3(glyphCentre, modelMatrix[3][2]));
				model = glm::scale(model, glm::vec3(glyphHalfExtents * 2.f, 0.f));

				// Update the material's subtexture to the subtexture of the glyph we want to render
				material->setSubTexture(0, gd.subTexture);
				// Now we can submit this new quad
				submit(material, model, text->getColour());
			}

			x += advance;
		}
	}

	//! generateVertexList()
	/*
	\param shader a ShaderProgram* - A pointer to the shader program
	\param batchEntries a std::vector<BatchEntry2D>& - A list of batch entries
	*/
	void Renderer2D::generateVertexList(ShaderProgram* shader, std::vector<BatchEntry2D>& batchEntries)
	{
		// Generate a list of vertices and edit the VBO depending on the vertex array

		if(shader->getVertexArray() == ResourceManager::getResource<VertexArray>("QuadArray"))
			generateListOfVertex2D(shader, batchEntries);
	}

	//! flush()
	void Renderer2D::flush()
	{
		// A flush has been called, now let's go through each shader entry in the batch queue
		if (s_drawCount > 0)
		{
			// Sort shader entries by some property
			std::vector<std::pair<ShaderProgram*, std::vector<BatchEntry2D>>> mapConvertedVector(s_batchQueue.begin(), s_batchQueue.end());

			std::sort(mapConvertedVector.begin(), mapConvertedVector.end(),
				[](std::pair<ShaderProgram*, std::vector<BatchEntry2D>> a, std::pair<ShaderProgram*, std::vector<BatchEntry2D>> b)
			{
				return a.first->getOrderImportance() < b.first->getOrderImportance();
			}
			);

			for (auto& shaderEntry : mapConvertedVector)
			{
				// Now we can go through each batch entry and create the relevant vertices and add them to the appropriate vertex list
				for (auto& submissionEntry : shaderEntry.second)
				{
					// WE HAVE: List of SubTextures, Model Matrix, Tint

					// Bind Textures

					// Check if we can bind the textures first
					if (s_unitManager->getRemainingUnitCount() < submissionEntry.subTextures.size())
					{
						// If we cannot bind all the textures, lets draw the current queue to free the unit manager up
						draw(shaderEntry.first);
					}

					// Unit manager can bind textures now
					int32_t unit = 0;
					for (int i = 0; i < submissionEntry.subTextures.size(); i++)
					{
						// For each subtexture, lets bind the texture to a unit
						if (s_unitManager->getUnit(submissionEntry.subTextures[i]->getBaseTexture()->getID(), unit))
							s_unitManager->bindToUnit(submissionEntry.subTextures[i]->getBaseTexture());

						submissionEntry.textureUnits.at(i) = unit;
					}
				}

				generateVertexList(shaderEntry.first, shaderEntry.second);
				draw(shaderEntry.first);
			}

			s_batchQueue.clear();
			s_drawCount = 0;
		}
	}

	//! draw()
	/*
	\param shader a ShaderProgram* - The shader program to send our geometry to
	*/
	void Renderer2D::draw(ShaderProgram* shader)
	{
		// Use the shader program
		shader->start();

		// Attach the UBO
		for (auto& dataPair : shader->getUniformBuffers())
		{
			const char* nameOfUniformBlock = dataPair.first.c_str();
			dataPair.second->attachShaderBlock(shader, nameOfUniformBlock);
		}

		// Normally we need to check what kind of uniforms we have to send
		// I don't think we will ever use anything other than 2D samplers for 2D rendering
		// This will be different for 3D rendering
		shader->sendIntArray("u_textures", s_unit.data(), 16);

		// Bind VAO
		shader->getVertexArray()->bind();

		// Issue the draw call
		// The number of submissions in the shader entry in the queue * 6 indices
		RenderUtils::draw(static_cast<uint32_t>(s_batchQueue[shader].size()) * 6);

		// Clean up
		s_drawCount -= static_cast<uint32_t>(s_batchQueue[shader].size());
		s_batchQueue[shader].clear();
	}

	//! end()
	void Renderer2D::end()
	{
		// If we have something to draw, draw it
		if (s_drawCount > 0)
			flush();
	}

	//! destroy()
	void Renderer2D::destroy()
	{
		ENGINE_INFO("[Renderer2D::destroy] Destroying the 2D renderer.");

		// Clean up renderer data
		if (s_overridingShader) s_overridingShader->decreaseCount();
		s_overridingShader = nullptr;

		s_unitManager = nullptr;
		s_batchQueue.clear();
	}

	//! setTextureUnitManager()
	/*!
	\param unitManager a TextureUnitManager*& - A reference to the texture unit manager
	\param unit a const std::array<int32_t, 16>& - A reference to the array of units
	*/
	void Renderer2D::setTextureUnitManager(TextureUnitManager*& unitManager, const std::array<int32_t, 16>& unit)
	{
		s_unitManager = unitManager;
		s_unit = unit;
	}

	//! generateListOfVertex2D()
	/*
	\param shader a ShaderProgram* - A pointer to the shader program
	\param batchEntries a std::vector<BatchEntry2D>& - A list of batch entries
	*/
	void Renderer2D::generateListOfVertex2D(ShaderProgram* shader, std::vector<BatchEntry2D>& batchEntries)
	{
		// Create a fresh new list of vertices and then edit the VBO
		std::vector<Vertex2D> vertexList;
		vertexList.resize(ResourceManager::getConfigValue(ConfigData::BatchCapacity2D) * 4);

		// Starting from the beginning of the list
		uint32_t startIndex = 0;
		for (auto& entry : batchEntries)
		{
			// Edit the next 4 vertices in the list
			for (int i = 0; i < 4; i++)
			{
				vertexList[i + startIndex].Position = entry.modelMatrix * getQuadVertices().at(i).Position;
				vertexList[i + startIndex].TexUnit = entry.textureUnits[0];
				vertexList[i + startIndex].Tint = MemoryUtils::pack(entry.tint);
			}

			vertexList[startIndex].TexCoords = entry.subTextures.at(0)->getUVEnd();
			vertexList[startIndex + 1].TexCoords = { entry.subTextures.at(0)->getUVEnd().x, entry.subTextures.at(0)->getUVStart().y };
			vertexList[startIndex + 2].TexCoords = entry.subTextures.at(0)->getUVStart();
			vertexList[startIndex + 3].TexCoords = { entry.subTextures.at(0)->getUVStart().x, entry.subTextures.at(0)->getUVEnd().y };

			startIndex += 4;
		}

		shader->getVertexArray()->getVertexBuffers().at(0)->edit(vertexList.data(), static_cast<uint32_t>(sizeof(Vertex2D) * vertexList.size()), 0);

	}
}
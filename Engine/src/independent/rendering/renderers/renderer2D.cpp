/*! \file renderer2D.cpp
*
* \brief A 2D renderer which allows 2D models to be submitted and drawn (Batched)
*
* \author Daniel Bullin
*
*/
#include "independent/rendering/renderers/renderer2D.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/fontManager.h"
#include "independent/rendering/renderers/utils/fillBuffers.h"
#include "independent/rendering/renderUtils.h"

namespace Engine
{
	uint32_t Renderer2D::s_batchCapacity = 0; //!< Set to 0
	TextureUnitManager* Renderer2D::s_unitManager = nullptr; //!< Set to null pointer
	std::array<int32_t, 16> Renderer2D::s_unit; //!< Initialise to empty array
	std::vector<BatchEntry2D> Renderer2D::s_batchQueue = std::vector<BatchEntry2D>(); //!< Initialise to empty list

	//! initialise()
	/*!
	\param batchCapacity a const uint32_t - The total number of 2D geometry submissions before a flush occurs
	*/
	void Renderer2D::initialise(const uint32_t batchCapacity)
	{
		ENGINE_TRACE("[Renderer2D::initialise] Initialising the 2D renderer.");

		if (batchCapacity <= 0)
			ENGINE_ERROR("[Renderer2D::initialise] An unusual batch capacity was provided. Value: {0}.", batchCapacity);

		// Set the batch capacity
		s_batchCapacity = batchCapacity;

		// Create the index buffer used for 2D rendering and register it with the resource manager to handle
		std::vector<uint32_t> indicesData = Quad::getIndices(batchCapacity);
		IndexBuffer* indexBuffer = IndexBuffer::create("QuadIBuffer", indicesData.data(), static_cast<uint32_t>(indicesData.size()));
		ResourceManager::registerResource("QuadIBuffer", indexBuffer);
	}

	//! begin()
	void Renderer2D::begin()
	{
		s_batchQueue.clear();
	}

	//! submissionChecks()
	/*
	\param shaderProgram a ShaderProgram* - A pointer to the shader program
	\param subTextures a const std::vector<SubTexture*>& - A list of subtextures
	*/
	bool Renderer2D::submissionChecks(ShaderProgram* shaderProgram, const std::vector<SubTexture*>& subTextures)
	{
		// Check shader program, base textures and subtextures for validity
		if (!shaderProgram)
		{
			ENGINE_ERROR("[Renderer2D::submissionChecks] An invalid shader program was provided");
			return false;
		}

		for (auto& subTexture : subTextures)
		{
			if (subTexture)
			{
				if (!subTexture->getBaseTexture())
				{
					ENGINE_ERROR("[Renderer2D::submissionChecks] An invalid base texture was provided for subtexture: {0}.", subTexture->getName());
					return false;
				}
			}
			else
			{
				ENGINE_ERROR("[Renderer2D::submissionChecks] An invalid subtexture was provided.");
				return false;
			}
		}
		return true;
	}

	//! submit()
	/*!
	\param shaderProgram a ShaderProgram* - A pointer to the shader program
	\param subTextures a const std::vector<SubTexture*>& - A list of subtextures
	\param modelMatrix a const glm::mat4& - A model matrix
	\param tint a const glm::vec4& - The tint to apply
	*/
	void Renderer2D::submit(ShaderProgram* shaderProgram, const std::vector<SubTexture*>& subTextures, const glm::mat4& modelMatrix, const glm::vec4& tint)
	{
		// First lets do some error checking
		if (submissionChecks(shaderProgram, subTextures))
		{
			/////
			// Submitting
			/////

			// We need to check if this new submission would take us over capacity, if it does, flush the current contents and try submitting again
			if (s_batchQueue.size() >= s_batchCapacity)
				flush();

			// Each subtexture has a base texture which needs a texture unit to be bound to, so lets create a units list equal to the subtexture size
			std::vector<int32_t> units;
			units.resize(subTextures.size());

			// Add to the back of the queue [SHADER, SUBTEXTURES, UNITS, MODELMATRIX, TINT]
			s_batchQueue.push_back({ shaderProgram, subTextures, units, modelMatrix, tint });
		}
	}

	//! submitText()
	/*!
	\param text a Text* - A pointer to some text
	\param modelMatrix a const glm::mat4& - A model matrix
	*/
	void Renderer2D::submitText(Text* text, const glm::mat4& modelMatrix)
	{
		// First, let's check the text component is valid
		if (!text)
		{
			ENGINE_ERROR("[Renderer2D::submitText] An invalid text component was provided.");
			return;
		}

		submitText(text->getText(), text->getFont(), text->getColour(), modelMatrix);
	}

	//! submitText()
	/*!
	\param text a const std::string& - The text to render
	\param fontName a const std::string& - The font to use
	\param tint a const glm::vec4& - The colour of the text
	\param modelMatrix a const glm::mat4& - A model matrix
	*/
	void Renderer2D::submitText(const std::string& text, const std::string& fontName, const glm::vec4& tint, const glm::mat4& modelMatrix)
	{
		// Get the length of the text we're rendering
		uint32_t len = static_cast<uint32_t>(strlen(text.c_str()));

		// If the legnth of the text is 0, lets not bother doing any more processing and return
		if (len == 0) return;

		// Text will only ever use one specific material, so lets check that from the resource manager
		Material* material = ResourceManager::getResource<Material>("textMaterial");
		if (!material) return;

		// Get x position of the entity from the model matrix, and keep the current advance
		float advance = 0.f, x = modelMatrix[3][0];

		// Get the font for this text render
		auto font = FontManager::getFont(fontName);

		if (!font)
		{
			ENGINE_ERROR("[Renderer2D::submitText] An invalid font name was provided. Name: {0}.", fontName);
			return;
		}

		// For each character we want to render
		for (int i = 0; i < static_cast<int>(len); i++)
		{
			// Get the character by indexing
			char ch = text.at(i);

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
				submit(material->getShader(), material->getSubTextures(), model, tint);
			}

			// Move the x position along by the advance
			x += advance;
		}
	}

	//! sortSubmissions()
	/*
	\param submissions a std::vector<BatchEntry2D>& - A list of submissions
	*/
	void Renderer2D::sortSubmissions(std::vector<BatchEntry2D>& submissions)
	{
		// Sort by shader importance and by shader address
		std::sort(submissions.begin(), submissions.end(),
			[](BatchEntry2D& a, BatchEntry2D& b)
		{
			if (a.shader->getOrderImportance() != b.shader->getOrderImportance()) return a.shader->getOrderImportance() < b.shader->getOrderImportance();
			return a.modelMatrix[3][2] < b.modelMatrix[3][2];
		}
		);
	}

	//! flush()
	void Renderer2D::flush()
	{
		/////
		// SORTING SHADERS
		/////
		sortSubmissions(s_batchQueue);

		/////
		// Begin setting the necassary data
		/////
		std::vector<BatchEntry2D> tmpList;
		ShaderProgram* currentShader = s_batchQueue.at(0).shader;
		for (auto& submission : s_batchQueue)
		{
			// If we've moved onto a new shader, draw the current list
			if (submission.shader != currentShader)
			{
				generateVertexList(tmpList);
				draw(tmpList);
				tmpList.clear();
				currentShader = submission.shader;
			}

			// If we cannot bind the textures for the current submission, draw the current list
			if (s_unitManager->getRemainingUnitCount() < submission.subTextures.size())
			{
				generateVertexList(tmpList);
				draw(tmpList);
				s_unitManager->clear(true);
				tmpList.clear();
			}

			// Bind the textures
			int32_t unit = 0;
			for (int i = 0; i < submission.subTextures.size(); i++)
			{
				// For each subtexture, lets bind the texture to a unit
				if (s_unitManager->getUnit(submission.subTextures[i]->getBaseTexture()->getID(), unit))
					s_unitManager->bindToUnit(submission.subTextures[i]->getBaseTexture());

				// Store the unit in the texture units list of the submission entry
				submission.textureUnits.at(i) = unit;
			}

			tmpList.push_back(submission);
		}

		// Draw anything left in the list
		if (tmpList.size() != 0)
		{
			generateVertexList(tmpList);
			draw(tmpList);
		}

		// All shader entries have been drawn, lets clear all data
		s_batchQueue.clear();
		tmpList.clear();
	}

	//! draw()
	/*
	\param submissionList a std::vector<BatchEntry2D>& - A list of submissions
	*/
	void Renderer2D::draw(std::vector<BatchEntry2D>& submissionList)
	{
		if (submissionList.size() != 0)
		{
			// Use the shader program
			submissionList.at(0).shader->start();

			// Attach the UBO
			for (auto& dataPair : submissionList.at(0).shader->getUniformBuffers())
			{
				const char* nameOfUniformBlock = dataPair.first.c_str();
				dataPair.second->attachShaderBlock(submissionList.at(0).shader, nameOfUniformBlock);
			}

			// Normally we need to check what kind of uniforms we have to send
			// I don't think we will ever use anything other than 2D samplers for 2D rendering
			// This will be different for 3D rendering
			submissionList.at(0).shader->sendIntArray("u_textures", s_unit.data(), 16);

			// Bind VAO
			submissionList.at(0).shader->getVertexArray()->bind();
			ResourceManager::getResource<IndexBuffer>("QuadIBuffer")->bind();

			// Issue the draw call
			// The number of submissions for the shader that we want to render * 6 indices
			RenderUtils::draw(static_cast<uint32_t>(submissionList.size()) * 6);
		}
	}

	//! end()
	void Renderer2D::end()
	{
		// If we have something to draw, draw it
		if (s_batchQueue.size() > 0)
			flush();
	}

	//! destroy()
	void Renderer2D::destroy()
	{
		ENGINE_TRACE("[Renderer2D::destroy] Destroying the 2D renderer.");

		// Clean up renderer data
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
}
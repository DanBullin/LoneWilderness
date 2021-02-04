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

namespace Engine
{
	Renderer2DData* Renderer2D::s_rendererData = nullptr; //!< Initialise with null pointer
	ShaderProgram* Renderer2D::s_overridingShader = nullptr; //!< Initialise with null pointer

	//! initialise()
	/*!
	\param batchCapacity a const uint32_t - The total number of 2D geometry submissions before a flush occurs
	*/
	void Renderer2D::initialise(const uint32_t batchCapacity)
	{
		ENGINE_INFO("[Renderer2D::initialise] Initialising the 2D renderer.");

		// Initialise new renderer data
		s_rendererData = new Renderer2DData;

		// Set the batch capacity
		s_rendererData->batchCapacity = batchCapacity;
		s_rendererData->drawCount = 0;

		s_rendererData->vertices.resize(s_rendererData->batchCapacity * 4); // Number of Quads multiplied by 4 vertices
		// Initialise with empty 2D vertices
		for (int i = 0; i < s_rendererData->vertices.size(); i++)
			s_rendererData->vertices[i] = Vertex2D();

		// Create VAO
		s_rendererData->VAO.reset(VertexArray::create());

		// Create vertex and index buffer
		std::shared_ptr<VertexBuffer> verticesVBO;
		std::shared_ptr<IndexBuffer> indicesBO;

		// Layout of a Vertex2D
		VertexBufferLayout layout = { ShaderDataType::Float4, ShaderDataType::Float2, ShaderDataType::FlatInt, { ShaderDataType::Byte4, true } };

		verticesVBO.reset(VertexBuffer::create(s_rendererData->vertices.data(), static_cast<uint32_t>(s_rendererData->vertices.size() * sizeof(Vertex2D)), layout, VertexBufferUsage::StaticDraw));

		// Generate all the quads indices
		std::vector<uint32_t> indicesData = getQuadIndices(batchCapacity);
		indicesBO.reset(IndexBuffer::create(indicesData.data(), static_cast<uint32_t>(indicesData.size())));

		s_rendererData->VAO->addVertexBuffer(verticesVBO);
		s_rendererData->VAO->setIndexBuffer(indicesBO);

		// Unbind VAO for safety
		s_rendererData->VAO->unbind();

	}

	//! begin()
	/*!
	\param shaderProgram a ShaderProgram* - A pointer to the shader program
	\param sceneWideUniforms a const SceneWideUniforms& - A reference to the scene wide uniforms
	*/
	void Renderer2D::begin(ShaderProgram* shaderProgram, const SceneWideUniforms& sceneWideUniforms)
	{
		// FBO Already bound by this point

		// Start the shader program
		s_overridingShader = shaderProgram;
		s_rendererData->drawCount = 0;

		s_overridingShader->start();

		// Bind the uniform buffer to the current shader
		for (auto& dataPair : sceneWideUniforms)
		{
			const char* nameOfUniformBlock = dataPair.first;
			dataPair.second->attachShaderBlock(s_overridingShader, nameOfUniformBlock);
		}

		// Upload uniforms
		s_overridingShader->sendIntArray("u_diffuseMap", s_rendererData->unit.data(), 16);

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

		// If the current draw count + new Quad vertices would take us over the vertex capacity, then flush
		if (s_rendererData->drawCount + 4 > (s_rendererData->batchCapacity * 4))
			flush();

		int32_t unit;

		if (s_rendererData->unitManager->full())
			flush();

		// Bind the texture
		if (s_rendererData->unitManager->getUnit(material->getSubTexture(0)->getBaseTexture()->getID(), unit))
			s_rendererData->unitManager->bindToUnit(material->getSubTexture(0)->getBaseTexture());

		// Pack the tint from the material
		glm::vec4 tmpTint;
		if (tint == glm::vec4(-1.f, -1.f, -1.f, -1.f))
			tmpTint = material->getTint();
		else
			tmpTint = tint;

		uint32_t packedTint = MemoryUtils::pack(tmpTint);

		// Add new vertices to list of vertices
		uint32_t startIndex = s_rendererData->drawCount;
		for (int i = 0; i < 4; i++)
		{
			s_rendererData->vertices[i + startIndex].Position = modelMatrix * getQuadVertices().at(i).Position;
			s_rendererData->vertices[i + startIndex].TexUnit = unit;
			s_rendererData->vertices[i + startIndex].Tint = packedTint;
		}

		s_rendererData->vertices[startIndex].TexCoords = material->getSubTexture(0)->getUVEnd();
		s_rendererData->vertices[startIndex + 1].TexCoords = { material->getSubTexture(0)->getUVEnd().x, material->getSubTexture(0)->getUVStart().y };
		s_rendererData->vertices[startIndex + 2].TexCoords = material->getSubTexture(0)->getUVStart();
		s_rendererData->vertices[startIndex + 3].TexCoords = { material->getSubTexture(0)->getUVStart().x, material->getSubTexture(0)->getUVEnd().y };

		// Add 4 to the draw count
		s_rendererData->drawCount += 4;
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

		Material* material = ResourceManager::getMaterial("textMaterial").get();

		// Get x position of the entity
		float advance = 0.f, x = modelMatrix[3][0];

		// Get the font for this text render
		auto font = FontManager::getFont(text->getFont().c_str());

		// For each character we're rendering
		for (int i = 0; i < static_cast<int>(len); i++)
		{
			// Get the character
			char ch = text->getText().at(i);

			// If character one of the glyph's we've loaded
			if (ch > font->getFirstGlyph() && ch <= font->getLastGlyph())
			{
				GlyphData& gd = font->getGlyphData().at(ch - font->getFirstGlyph());

				glm::vec2 pos = { x, modelMatrix[3][1] };
				glm::vec2 glyphHalfExtents(gd.size * 0.5f);
				glm::vec2 glyphCentre = (pos + gd.bearing) + glyphHalfExtents;

				advance = gd.advance;

				glm::mat4 model = glm::mat4(1.f);
				model = glm::translate(model, glm::vec3(glyphCentre, 0.f));
				model = glm::scale(model, glm::vec3(glyphHalfExtents * 2.f, 1.f));

				material->setSubTexture(0, gd.subTexture.get());
				submit(material, model, text->getColour());
			}

			x += advance;
		}
	}

	//! flush()
	void Renderer2D::flush()
	{
		if (s_rendererData->drawCount > 0)
		{
			// Populate the VBO with vertex list
			s_rendererData->VAO->getVertexBuffers().at(0)->edit(s_rendererData->vertices.data(), static_cast<uint32_t>(sizeof(Vertex2D) * s_rendererData->vertices.size()), 0);

			// Draw
			s_rendererData->VAO->bind();

			RenderUtils::draw((s_rendererData->drawCount / 4) * 6);
			s_rendererData->drawCount = 0;
		}
	}

	//! end()
	void Renderer2D::end()
	{
		// If we have something to draw, draw it
		if (s_rendererData->drawCount > 0)
			flush();
	}

	//! destroy()
	void Renderer2D::destroy()
	{
		ENGINE_INFO("[Renderer2D::destroy] Destroying the 2D renderer.");
		// Clean up renderer data
		delete s_rendererData;
		s_rendererData = nullptr;
		s_overridingShader = nullptr;
	}

	//! setTextureUnitManager()
	/*!
	\param unitManager a const std::shared_ptr<TextureUnitManager>& - A reference to the texture unit manager
	\param unit a const std::array<int32_t, 16>& - A reference to the array of units
	*/
	void Renderer2D::setTextureUnitManager(const std::shared_ptr<TextureUnitManager>& unitManager, const std::array<int32_t, 16>& unit)
	{
		s_rendererData->unitManager = unitManager;
		s_rendererData->unit = unit;
	}
}
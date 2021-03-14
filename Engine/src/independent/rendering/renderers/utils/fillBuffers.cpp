/*! \file fillBuffers.cpp
*
* \brief A utility class to help fill the buffers with instance data needed in rendering
*
* \author Daniel Bullin
*
*/
#include "independent/rendering/renderers/utils/fillBuffers.h"

namespace Engine
{
	//! generateVertexList()
	/*
	\param batchEntries a std::vector<BatchEntry2D>& - A list of batch entries
	*/
	void generateVertexList(std::vector<BatchEntry2D>& batchEntries)
	{
		// Generate a list of vertices and edit the VBO depending on the vertex array
		if (batchEntries.at(0).shader->getVertexArray()->getName() == "QuadArray")
			generateListOfVertex2D(batchEntries);
		else if (batchEntries.at(0).shader->getVertexArray()->getName() == "QuadMultiTexturedArray")
			generateListOfVertex2DMutlitextured(batchEntries);
	}

	//! generateListOfVertex2D()
	/*
	\param batchEntries a std::vector<BatchEntry2D>& - A list of batch entries
	*/
	void generateListOfVertex2D(std::vector<BatchEntry2D>& batchEntries)
	{
		// Create a fresh new list of vertices and then edit the VBO
		std::vector<Vertex2D> vertexList;
		uint32_t count = static_cast<uint32_t>(batchEntries.size());
		vertexList.resize(count * 4);

		// Starting from the beginning of the list
		uint32_t startIndex = 0;
		for (auto& entry : batchEntries)
		{
			// Edit the next 4 vertices in the list
			for (int i = 0; i < 4; i++)
			{
				vertexList[i + startIndex].Position = entry.modelMatrix * Quad::getLocalVertices().at(i).Position;
				vertexList[i + startIndex].TexUnit = entry.textureUnits[0];
				vertexList[i + startIndex].Tint = MemoryUtils::pack(entry.tint);
			}

			vertexList[startIndex].TexCoords = entry.subTextures.at(0)->getUVEnd();
			vertexList[startIndex + 1].TexCoords = { entry.subTextures.at(0)->getUVEnd().x, entry.subTextures.at(0)->getUVStart().y };
			vertexList[startIndex + 2].TexCoords = entry.subTextures.at(0)->getUVStart();
			vertexList[startIndex + 3].TexCoords = { entry.subTextures.at(0)->getUVStart().x, entry.subTextures.at(0)->getUVEnd().y };

			startIndex += 4;
		}

		batchEntries.at(0).shader->getVertexArray()->getVertexBuffers().at(0)->edit(vertexList.data(), static_cast<uint32_t>(sizeof(Vertex2D) * vertexList.size()), 0);
	}

	//! generateListOfVertex2DMutlitextured()
	/*
	\param batchEntries a std::vector<BatchEntry2D>& - A list of batch entries
	*/
	void generateListOfVertex2DMutlitextured(std::vector<BatchEntry2D>& batchEntries)
	{
		// Create a fresh new list of vertices and then edit the VBO
		std::vector<Vertex2DMultiTextured> vertexList;
		uint32_t count = static_cast<uint32_t>(batchEntries.size());
		vertexList.resize(count * 4);

		// Starting from the beginning of the list
		uint32_t startIndex = 0;
		for (auto& entry : batchEntries)
		{
			// Edit the next 4 vertices in the list
			for (int i = 0; i < 4; i++)
			{
				vertexList[i + startIndex].Position = entry.modelMatrix * Quad::getLocalVertices().at(i).Position;
				vertexList[i + startIndex].TexUnit1 = entry.textureUnits[0];
				vertexList[i + startIndex].TexUnit2 = entry.textureUnits[1];
				vertexList[i + startIndex].Tint = MemoryUtils::pack(entry.tint);
			}

			vertexList[startIndex].TexCoords = entry.subTextures.at(0)->getUVEnd();
			vertexList[startIndex + 1].TexCoords = { entry.subTextures.at(0)->getUVEnd().x, entry.subTextures.at(0)->getUVStart().y };
			vertexList[startIndex + 2].TexCoords = entry.subTextures.at(0)->getUVStart();
			vertexList[startIndex + 3].TexCoords = { entry.subTextures.at(0)->getUVStart().x, entry.subTextures.at(0)->getUVEnd().y };

			startIndex += 4;
		}

		batchEntries.at(0).shader->getVertexArray()->getVertexBuffers().at(0)->edit(vertexList.data(), static_cast<uint32_t>(sizeof(Vertex2DMultiTextured) * vertexList.size()), 0);
	}

	//! generateInstanceData()
	/*
	\param batchEntries a std::vector<BatchEntry3D>& - A list of batch entries
	*/
	void generateInstanceData(std::vector<BatchEntry3D>& batchEntries)
	{
		// Generate a list of the instance data and edit the appropriate vertex buffers in the vao
		if (batchEntries.at(0).shader->getVertexArray()->getName() == "vertexArray1")
			generateBasic3D(batchEntries);
		else if (batchEntries.at(0).shader->getVertexArray()->getName() == "vertexArray2")
			generateSkybox(batchEntries);
		else if (batchEntries.at(0).shader->getVertexArray()->getName() == "vertexArray3")
			generateTerrain(batchEntries);
		else if (batchEntries.at(0).shader->getVertexArray()->getName() == "lightSourceArray")
			generateLightSource(batchEntries);
	}

	//! generateBasic3D()
	/*
	\param batchEntries a std::vector<BatchEntry3D>& - A list of batch entries
	*/
	void generateBasic3D(std::vector<BatchEntry3D>& batchEntries)
	{
		std::vector<glm::mat4> modelInstanceData; //!< The model matrix instance data
		std::vector<glm::vec4> tintInstanceData; //!< The tint instance data
		std::vector<int32_t> unit1InstanceData; //!< The texture unit data
		std::vector<int32_t> unit2InstanceData; //!< The texture unit data
		std::vector<float> shininessInstanceData; //!< The shininess data
		std::vector<glm::vec4> subTextureUVs; //!< The Start and End UV coordinates of the subtexture

		for (auto& entry : batchEntries)
		{
			modelInstanceData.push_back(entry.modelMatrix);
			unit1InstanceData.push_back(entry.textureUnits[0]);
			unit2InstanceData.push_back(entry.textureUnits[1]);
			tintInstanceData.push_back(entry.tint);
			shininessInstanceData.push_back(entry.shininess);
			subTextureUVs.push_back(glm::vec4(entry.subTextures.at(0)->getUVStart().x, entry.subTextures.at(0)->getUVStart().y,
				entry.subTextures.at(0)->getUVEnd().x, entry.subTextures.at(0)->getUVEnd().y));
		}

		batchEntries.at(0).shader->getVertexArray()->getVertexBuffers().at(1)->edit(modelInstanceData.data(), sizeof(glm::mat4) * static_cast<uint32_t>(batchEntries.size()), 0);
		batchEntries.at(0).shader->getVertexArray()->getVertexBuffers().at(2)->edit(unit1InstanceData.data(), sizeof(int32_t) * static_cast<uint32_t>(batchEntries.size()), 0);
		batchEntries.at(0).shader->getVertexArray()->getVertexBuffers().at(3)->edit(unit2InstanceData.data(), sizeof(int32_t) * static_cast<uint32_t>(batchEntries.size()), 0);
		batchEntries.at(0).shader->getVertexArray()->getVertexBuffers().at(4)->edit(tintInstanceData.data(), sizeof(glm::vec4) * static_cast<uint32_t>(batchEntries.size()), 0);
		batchEntries.at(0).shader->getVertexArray()->getVertexBuffers().at(5)->edit(shininessInstanceData.data(), sizeof(float) * static_cast<uint32_t>(batchEntries.size()), 0);
		batchEntries.at(0).shader->getVertexArray()->getVertexBuffers().at(6)->edit(subTextureUVs.data(), sizeof(glm::vec4) * static_cast<uint32_t>(batchEntries.size()), 0);
	}

	//! generateSkybox()
	/*
	\param batchEntries a std::vector<BatchEntry3D>& - A list of batch entries
	*/
	void generateSkybox(std::vector<BatchEntry3D>& batchEntries)
	{
		std::vector<glm::vec4> tintInstanceData; //!< The tint instance data
		std::vector<int32_t> unitInstanceData; //!< The texture unit data

		for (auto& entry : batchEntries)
		{
			unitInstanceData.push_back(entry.cubeTextureUnits[0]);
			tintInstanceData.push_back(entry.tint);
		}

		batchEntries.at(0).shader->getVertexArray()->getVertexBuffers().at(1)->edit(unitInstanceData.data(), sizeof(int32_t) * static_cast<uint32_t>(batchEntries.size()), 0);
		batchEntries.at(0).shader->getVertexArray()->getVertexBuffers().at(2)->edit(tintInstanceData.data(), sizeof(glm::vec4) * static_cast<uint32_t>(batchEntries.size()), 0);
	}

	//! generateLightSource()
	/*
	\param batchEntries a std::vector<BatchEntry3D>& - A list of batch entries
	*/
	void generateLightSource(std::vector<BatchEntry3D>& batchEntries)
	{
		std::vector<glm::vec4> tintInstanceData; //!< The tint instance data
		std::vector<glm::mat4> modelInstanceData; //!< The model matrix instance data

		for (auto& entry : batchEntries)
		{
			modelInstanceData.push_back(entry.modelMatrix);
			tintInstanceData.push_back(entry.tint);
		}

		batchEntries.at(0).shader->getVertexArray()->getVertexBuffers().at(1)->edit(modelInstanceData.data(), sizeof(glm::mat4) * static_cast<uint32_t>(batchEntries.size()), 0);
		batchEntries.at(0).shader->getVertexArray()->getVertexBuffers().at(2)->edit(tintInstanceData.data(), sizeof(glm::vec4) * static_cast<uint32_t>(batchEntries.size()), 0);
	}

	//! generateLightSource()
	/*
	\param batchEntries a std::vector<BatchEntry3D>& - A list of batch entries
	*/
	void generateTerrain(std::vector<BatchEntry3D>& batchEntries)
	{
		std::vector<glm::mat4> modelInstanceData; //!< The model matrix instance data
		std::vector<glm::vec4> tintInstanceData; //!< The tint instance data
		std::vector<int32_t> unit1InstanceData; //!< The texture unit data
		std::vector<glm::vec4> subTextureUVs; //!< The Start and End UV coordinates of the subtexture

		for (auto& entry : batchEntries)
		{
			modelInstanceData.push_back(entry.modelMatrix);
			unit1InstanceData.push_back(entry.textureUnits[0]);
			tintInstanceData.push_back(entry.tint);
			subTextureUVs.push_back(glm::vec4(entry.subTextures.at(0)->getUVStart().x, entry.subTextures.at(0)->getUVStart().y,
				entry.subTextures.at(0)->getUVEnd().x, entry.subTextures.at(0)->getUVEnd().y));
		}

		batchEntries.at(0).shader->getVertexArray()->getVertexBuffers().at(1)->edit(modelInstanceData.data(), sizeof(glm::mat4) * static_cast<uint32_t>(batchEntries.size()), 0);
		batchEntries.at(0).shader->getVertexArray()->getVertexBuffers().at(2)->edit(unit1InstanceData.data(), sizeof(int32_t) * static_cast<uint32_t>(batchEntries.size()), 0);
		batchEntries.at(0).shader->getVertexArray()->getVertexBuffers().at(3)->edit(tintInstanceData.data(), sizeof(glm::vec4) * static_cast<uint32_t>(batchEntries.size()), 0);
		batchEntries.at(0).shader->getVertexArray()->getVertexBuffers().at(4)->edit(subTextureUVs.data(), sizeof(glm::vec4) * static_cast<uint32_t>(batchEntries.size()), 0);
	}
}
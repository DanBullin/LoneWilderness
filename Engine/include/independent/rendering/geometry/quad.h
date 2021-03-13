/*! \file quad.h
*
* \brief The geometry associated with a Quad
*
* \author Daniel Bullin
*
*/
#ifndef QUAD_H
#define QUAD_H

#include "independent/core/common.h"
#include "independent/rendering/geometry/vertex.h"
#include "independent/systems/systems/windowManager.h"
#include "independent/systems/systems/log.h"

namespace Engine
{
	namespace Quad
	{
		//! getLocalVertices()
		/*!
		\return a std::vector<Vertex2D> - The list of local vertices for a quad
		*/
		static std::vector<Vertex2D> getLocalVertices()
		{
			std::vector<Vertex2D> localVertices;
			localVertices.resize(4);
			localVertices[0] = Vertex2D({  0.5f,  0.5f, 0.f, 1.f }, { 1.f, 1.f }, 0, { 1.f, 1.f, 1.f, 1.f });
			localVertices[1] = Vertex2D({  0.5f, -0.5f, 0.f, 1.f }, { 1.f, 0.f }, 0, { 1.f, 1.f, 1.f, 1.f });
			localVertices[2] = Vertex2D({ -0.5f, -0.5f, 0.f, 1.f }, { 0.f, 0.f }, 0, { 1.f, 1.f, 1.f, 1.f });
			localVertices[3] = Vertex2D({ -0.5f,  0.5f, 0.f, 1.f }, { 0.f, 1.f }, 0, { 1.f, 1.f, 1.f, 1.f });
			return localVertices;
		}

		//! getIndices()
		/*!
		\param capacity a const uint32_t - The batch capacity
		\return a std::vector<uint32_t> - The list of indices given a batch capacity
		*/
		static std::vector<uint32_t> getIndices(const uint32_t capacity)
		{
			// Works out all the indicies for the "capacity" number of quads

			std::vector<uint32_t> indices(capacity * 6);

			int j = 0;
			for (int i = 0; i < indices.size();)
			{
				indices[i] = j;
				indices[i + 1] = j + 1;
				indices[i + 2] = j + 3;
				indices[i + 3] = j + 1;
				indices[i + 4] = j + 2;
				indices[i + 5] = j + 3;
				j += 4;
				i += 6;
			}

			return indices;
		}

		//! getScreenQuadMatrix()
		/*!
		\return a glm::mat4 - A model matrix for a screen quad
		*/
		static glm::mat4 getScreenQuadMatrix()
		{
			// Returns the model matrix for a screen quad
			glm::mat4 modelMatrix = glm::mat4(1.f);

			Window* window = WindowManager::getFocusedWindow();
			if (!window)
			{
				ENGINE_ERROR("[getScreenQuad] Cannot get the focused window.");
				return modelMatrix;
			}

			glm::vec2 screenSize = { window->getProperties().getSizef().x, window->getProperties().getSizef().y };
			float xPos = screenSize.x / 2.f;
			float yPos = screenSize.y / 2.f;
			float zPos = -1.f;

			modelMatrix = glm::translate(modelMatrix, glm::vec3(xPos, yPos, zPos));
			modelMatrix = glm::scale(modelMatrix, { screenSize.x, screenSize.y, 1.f });
			return modelMatrix;
		}
	}
}
#endif
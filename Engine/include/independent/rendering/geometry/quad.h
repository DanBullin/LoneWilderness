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

namespace Engine
{
	//! getQuadVertices()
	/*!
	\return a std::vector<Vertex2D> - The list of local vertices for a quad
	*/
	static std::vector<Vertex2D> getQuadVertices()
	{
		std::vector<Vertex2D> localVertices;
		localVertices.resize(4);
		localVertices[0] = Vertex2D({ 0.5f,  0.5f, 1.f, 1.f }, { 1.f, 1.f }, 0, { 1.f, 1.f, 1.f, 1.f });
		localVertices[1] = Vertex2D({ 0.5f, -0.5f, 1.f, 1.f }, { 1.f, 0.f }, 0, { 1.f, 1.f, 1.f, 1.f });
		localVertices[2] = Vertex2D({ -0.5f, -0.5f, 1.f, 1.f }, { 0.f, 0.f }, 0, { 1.f, 1.f, 1.f, 1.f });
		localVertices[3] = Vertex2D({ -0.5f,  0.5f, 1.f, 1.f }, { 0.f, 1.f }, 0, { 1.f, 1.f, 1.f, 1.f });

		return localVertices;
	}

	//! getQuadIndices()
	/*!
	\param capacity a const uint32_t - The batch capacity
	\return a std::vector<uint32_t> - The list of indices given a batch capacity
	*/
	static std::vector<uint32_t> getQuadIndices(const uint32_t capacity)
	{
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
}
#endif
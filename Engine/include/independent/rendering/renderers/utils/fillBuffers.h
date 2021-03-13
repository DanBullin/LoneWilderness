/*! \file fillBuffers.h
*
* \brief A utility class to help fill the buffers with instance data needed in rendering
*
* \author Daniel Bullin
*
*/
#ifndef FILLBUFFERS_H
#define FILLBUFFERS_H

#include "independent/rendering/renderers/renderer2D.h"
#include "independent/rendering/renderers/renderer3D.h"

namespace Engine
{
	void generateVertexList(std::vector<BatchEntry2D>& batchEntries); //!< Generate vertex list
	void generateListOfVertex2D(std::vector<BatchEntry2D>& batchEntries); //!< Generate a list of vertices and edit the VBO
	void generateListOfVertex2DMutlitextured(std::vector<BatchEntry2D>& batchEntries); //!< Generate a list of vertices and edit the VBO

	void generateInstanceData(std::vector<BatchEntry3D>& batchEntries); //!< Generate the instance data
	void generateBasic3D(std::vector<BatchEntry3D>& batchEntries); //!< Generate the instance data
	void generateSkybox(std::vector<BatchEntry3D>& batchEntries); //!< Generate the instance data
	void generateLightSource(std::vector<BatchEntry3D>& batchEntries); //!< Generate the instance data
}
#endif
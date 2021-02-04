/*! \file openGLIndexBuffer.cpp
*
* \brief An OpenGL Index buffer
*
* \author DMU Course material
*
*/

#include <glad/glad.h>
#include "platform/OpenGL/geometry/openGLIndexBuffer.h"
#include "independent/systems/systems/log.h"

namespace Engine
{
	//! OpenGLIndexBuffer()
	/*!
	\param indices a const uint32_t* - The indices info
	\param count a const uint32_t - The number of indices
	*/
	OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* indices, const uint32_t count)
	{
		m_count = count;

		// Create the buffer and submit the indices data
		glCreateBuffers(1, &m_indexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, indices, GL_STATIC_DRAW);
	}

	//! ~OpenGLIndexBuffer()
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		// Delete the buffer
		ENGINE_INFO("[OpenGLIndexBuffer::~OpenGLIndexBuffer] Deleting Index buffer with ID: {0}", m_indexBufferID);
		glDeleteBuffers(1, &m_indexBufferID);
	}

	//! edit()
	/*!
	\param indices a const void* - The indices info
	\param size a const uint32_t - The number of indices in the indices data
	\param offset a const int32_t - The offset of where to place the new data in terms of index count
	*/
	void OpenGLIndexBuffer::edit(const void* indices, const uint32_t size, const uint32_t offset)
	{
		// Edit the buffer contents
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(uint32_t), size * sizeof(uint32_t), indices);
	}
}
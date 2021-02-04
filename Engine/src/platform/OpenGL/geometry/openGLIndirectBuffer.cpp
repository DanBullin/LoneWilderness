/*! \file openGLIndirectBuffer.cpp
*
* \brief An OpenGL Indirect buffer
*
* \author DMU Course material
*
*/

#include <glad/glad.h>
#include "platform/OpenGL/geometry/OpenGLIndirectBuffer.h"
#include "independent/systems/systems/log.h"

namespace Engine
{
	//! OpenGLIndirectBuffer()
	/*!
	\param commands a DrawElementsIndirectCommand* - The commands info
	\param count a const uint32_t - The number of commands
	*/
	OpenGLIndirectBuffer::OpenGLIndirectBuffer(DrawElementsIndirectCommand* commands, const uint32_t count)
	{
		// Create the buffer and set its original data
		glCreateBuffers(1, &m_bufferID);
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_bufferID);
		glBufferData(GL_DRAW_INDIRECT_BUFFER, count * sizeof(DrawElementsIndirectCommand), commands, GL_DYNAMIC_DRAW);
	}

	//! ~OpenGLIndirectBuffer()
	OpenGLIndirectBuffer::~OpenGLIndirectBuffer()
	{
		// Delete the buffer
		ENGINE_INFO("[OpenGLIndirectBuffer::~OpenGLIndirectBuffer] Deleting Indirect buffer with ID: {0}", m_bufferID);
		glDeleteBuffers(1, &m_bufferID);
	}

	//! edit()
	/*!
	\param commands a DrawElementsIndirectCommand* - The commands info
	\param count a const uint32_t - The number of commands
	\param offset a const uint32_t - The offset in the buffer
	*/
	void OpenGLIndirectBuffer::edit(DrawElementsIndirectCommand* commands, const uint32_t count, const uint32_t offset)
	{
		// Edit the buffer contents
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_bufferID);
		glBufferSubData(GL_DRAW_INDIRECT_BUFFER, offset, count * sizeof(DrawElementsIndirectCommand), commands);
	}
}
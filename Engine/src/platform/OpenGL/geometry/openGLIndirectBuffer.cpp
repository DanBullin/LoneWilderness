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
#include "independent/systems/systems/resourceManager.h"

namespace Engine
{
	//! OpenGLIndirectBuffer()
	/*!
	\param indirectBufferName a const std::string& - The name of the indirect buffer
	\param commands a DrawElementsIndirectCommand* - The commands info
	\param count a const uint32_t - The number of commands
	*/
	OpenGLIndirectBuffer::OpenGLIndirectBuffer(const std::string& indirectBufferName, DrawElementsIndirectCommand* commands, const uint32_t count) : IndirectBuffer(indirectBufferName)
	{
		// Create the buffer and set its original data
		m_byteSize = sizeof(DrawElementsIndirectCommand) * count;
		m_commandCount = count;

		glCreateBuffers(1, &m_bufferID);
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_bufferID);
		glBufferData(GL_DRAW_INDIRECT_BUFFER, m_byteSize, commands, GL_DYNAMIC_DRAW);
	}

	//! ~OpenGLIndirectBuffer()
	OpenGLIndirectBuffer::~OpenGLIndirectBuffer()
	{
		// Delete the buffer
		if (ResourceManager::getConfigValue(Config::PrintResourcesInDestructor))
			ENGINE_INFO("[OpenGLIndirectBuffer::~OpenGLIndirectBuffer] Deleting Indirect buffer with ID: {0}, Name: {1}.", m_bufferID, m_name);
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

	//! printDetails()
	void OpenGLIndirectBuffer::printDetails()
	{
		ENGINE_TRACE("Buffer ID: {0}.", m_bufferID);
		ENGINE_TRACE("Command Count: {0}.", m_commandCount);
		ENGINE_TRACE("Byte Size: {0}.", m_byteSize);
	}
}
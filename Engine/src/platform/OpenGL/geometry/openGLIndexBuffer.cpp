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
#include "independent/systems/systems/resourceManager.h"

namespace Engine
{
	//! OpenGLIndexBuffer()
	/*!
	\param indexBufferName a const std::string& - The name of the index buffer
	\param indices a const uint32_t* - The indices info
	\param count a const uint32_t - The number of indices
	*/
	OpenGLIndexBuffer::OpenGLIndexBuffer(const std::string& indexBufferName, const uint32_t* indices, const uint32_t count) : IndexBuffer(indexBufferName)
	{
		if (count <= 0)
			ENGINE_ERROR("[OpenGLIndexBuffer::OpenGLIndexBuffer] A count value of 0 or less was provided. Is this an error? Count: {0}, Name: {1}.", count, m_name);

		m_count = count;
		m_byteSize = count * static_cast<uint32_t>(sizeof(uint32_t));

		// Create the buffer and submit the indices data
		glCreateBuffers(1, &m_indexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_byteSize, indices, GL_STATIC_DRAW);
	}

	//! ~OpenGLIndexBuffer()
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		// Delete the buffer
		if (ResourceManager::getConfigValue(Config::PrintResourcesInDestructor))
			ENGINE_INFO("[OpenGLIndexBuffer::~OpenGLIndexBuffer] Deleting Index buffer with ID: {0}, Name: {1}.", m_indexBufferID, m_name);

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

		if (size <= 0)
			ENGINE_ERROR("[OpenGLIndexBuffer::edit] An invalid size was provided. Size: {0}: Offset: {1}. Name: {2}.", size, offset, m_name);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(uint32_t), size * sizeof(uint32_t), indices);
	}

	//! bind()
	void OpenGLIndexBuffer::bind()
	{
		if (m_indexBufferID == 0)
			ENGINE_ERROR("[OpenGLIndexBuffer::bind] Attempting to bind when ID is 0. Name: {0}.", m_name);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
	}

	//! unbind()
	void OpenGLIndexBuffer::unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	//! printDetails()
	void OpenGLIndexBuffer::printDetails()
	{
		ENGINE_TRACE("Buffer ID: {0}.", m_indexBufferID);
		ENGINE_TRACE("Index Count: {0}.", m_count);
		ENGINE_TRACE("Byte Size: {0}.", m_byteSize);
	}
}
/*! \file OpenGLVertexBuffer.cpp
*
* \brief An OpenGL Vertex buffer
*
* \author DMU Course material
*
*/
#include <glad/glad.h>
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/resourceManager.h"
#include "platform/OpenGL/geometry/OpenGLVertexBuffer.h"

namespace Engine
{
	//! OpenGLUsage()
	/*!
	\param usage a const VertexBufferUsage - The usage of the data desired
	\return an uint32_t - The usage of the data
	*/
	static uint32_t OpenGLUsage(const VertexBufferUsage usage)
	{
		// Return the GL enum for the usage desired
		switch (usage)
		{
		case VertexBufferUsage::StaticDraw:    return GL_STATIC_DRAW;
		case VertexBufferUsage::DynamicDraw:   return GL_DYNAMIC_DRAW;
		}
		return 0;
	}

	//! OpenGLVertexBuffer()
	/*!
	\param vertexBufferName a const std::string& - The name of the vertex buffer
	\param vertices a const void* - The vertices info
	\param size a const uint32_t - The size in bytes of the vertices data
	\param layout a const VertexBufferLayout - The layout of the vertex buffer
	\param usage a const VertexBufferUsage - The usage of the vertices data
	*/
	OpenGLVertexBuffer::OpenGLVertexBuffer(const std::string& vertexBufferName, const void* vertices, const uint32_t size, const VertexBufferLayout layout, const VertexBufferUsage usage) : VertexBuffer(vertexBufferName)
	{
		if (size <= 0)
			ENGINE_ERROR("[OpenGLVertexBuffer::OpenGLVertexBuffer] An invalid size was provided. Is this an error? Size: {0}, Name: {1}.", size, m_name);

		m_layout = layout;
		m_usage = usage;
		m_byteSize = size;

		// Create the buffer and set its original data
		glCreateBuffers(1, &m_bufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, OpenGLUsage(usage));
	}

	//! OpenGLVertexBuffer()
	/*!
	\param vertexBufferName a const std::string& - The name of the vertex buffer
	\param size a const uint32_t - The size in bytes of the buffer data
	\param usage a const VertexBufferUsage - The usage of the vertices data
	*/
	OpenGLVertexBuffer::OpenGLVertexBuffer(const std::string& vertexBufferName, const uint32_t size, const VertexBufferUsage usage) : VertexBuffer(vertexBufferName)
	{
		if (size <= 0)
			ENGINE_ERROR("[OpenGLVertexBuffer::OpenGLVertexBuffer] An invalid size was provided. Is this an error? Size: {0}, Name: {1}.", size, m_name);

		m_usage = usage;
		m_byteSize = size;

		// Create the buffer with set memory size but no data
		glCreateBuffers(1, &m_bufferID);
		bind();
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, OpenGLUsage(usage));
	}

	//! ~OpenGLVertexBuffer()
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		// Delete the buffer
		if (ResourceManager::getConfigValue(Config::PrintResourcesInDestructor))
			ENGINE_INFO("[OpenGLVertexBuffer::~OpenGLVertexBuffer] Deleting Vertex buffer with ID: {0}, Name: {1}.", m_bufferID, m_name);
		glDeleteBuffers(1, &m_bufferID);
	}

	//! edit()
	/*!
	\param vertices a const void* - The vertices info
	\param size a const uint32_t - The size in bytes of the vertices data
	\param offset a const int32_t - The offset of where to place the new data
	*/
	void OpenGLVertexBuffer::edit(const void* vertices, const uint32_t size, const uint32_t offset)
	{
		if (size <= 0)
			ENGINE_ERROR("[OpenGLVertexBuffer::edit] An invalid size was provided. Size: {0}: Offset: {1}. Name: {2}.", size, offset, m_name);

		// Edit the buffer contents
		bind();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices);
	}

	//! bind()
	void OpenGLVertexBuffer::bind()
	{
		if (m_bufferID == 0)
			ENGINE_ERROR("[OpenGLVertexBuffer::bind] Attempting to bind when ID is 0. Name: {0}.", m_name);

		glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
	}

	//! unbind()
	void OpenGLVertexBuffer::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//! printDetails()
	void OpenGLVertexBuffer::printDetails()
	{
		ENGINE_TRACE("Buffer ID: {0}.", m_bufferID);
		auto layout = m_layout.getElements();
		for (int i = 0; i < layout.size(); i++)
			ENGINE_TRACE("Element{0}: Type: {1}, Size: {2}, Offset: {3}, Normalized: {4}, Instance Divisor: {5}", i, SDT::convertSDTToString(layout[i].m_dataType), layout[i].m_size, layout[i].m_offset, layout[i].m_normalized, layout[i].m_instanceDivisor);
		ENGINE_TRACE("Usage: {0}.", static_cast<uint32_t>(m_usage));
		ENGINE_TRACE("Byte Size: {0}.", m_byteSize);
	}
}
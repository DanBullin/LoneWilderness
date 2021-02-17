/*! \file OpenGLVertexBuffer.cpp
*
* \brief An OpenGL Vertex buffer
*
* \author DMU Course material
*
*/
#include <glad/glad.h>
#include "independent/systems/systems/log.h"
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
		// Edit the buffer contents
		bind();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices);
	}

	//! bind()
	void OpenGLVertexBuffer::bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
	}

	//! unbind()
	void OpenGLVertexBuffer::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
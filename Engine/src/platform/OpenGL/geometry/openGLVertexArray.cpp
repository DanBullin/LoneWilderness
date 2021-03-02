/*! \file OpenGLVertexArray.cpp
*
* \brief An OpenGL Vertex Array
*
* \author DMU Course material
*
*/
#include <glad/glad.h>
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/resourceManager.h"
#include "platform/OpenGL/geometry/openGLVertexArray.h"

namespace Engine
{
	namespace SDT
	{
		//! toGLType()
		/*!
		\param type a ShaderDataType - The data type
		\return an uint32_t - The size in bytes of the data type
		*/
		static uint32_t toGLType(const ShaderDataType type)
		{
			switch (type)
			{
				case ShaderDataType::Short: return GL_SHORT;
				case ShaderDataType::Short2: return GL_SHORT;
				case ShaderDataType::Short3: return GL_SHORT;
				case ShaderDataType::Short4: return GL_SHORT;
				case ShaderDataType::Float: return GL_FLOAT;
				case ShaderDataType::Float2: return GL_FLOAT;
				case ShaderDataType::Float3: return GL_FLOAT;
				case ShaderDataType::Float4: return GL_FLOAT;
				case ShaderDataType::Mat3: return GL_FLOAT;
				case ShaderDataType::Mat4: return GL_FLOAT;
				case ShaderDataType::Int: return GL_INT;
				case ShaderDataType::Int2: return GL_INT;
				case ShaderDataType::Int3: return GL_INT;
				case ShaderDataType::Int4: return GL_INT;
				case ShaderDataType::Bool: return GL_BOOL;
				case ShaderDataType::Byte4: return GL_UNSIGNED_BYTE;
				case ShaderDataType::FlatInt: return GL_INT;
				case ShaderDataType::FlatByte: return GL_BYTE;
				default: return GL_INVALID_ENUM;
			}
		}
	}

	//! OpenGLVertexArray()
	/*
	\param vertexArrayName a const std::string& - The name of the vertex array
	*/
	OpenGLVertexArray::OpenGLVertexArray(const std::string& vertexArrayName) : VertexArray(vertexArrayName)
	{
		// Create array
		glCreateVertexArrays(1, &m_arrayID);
		bind();
		m_totalByteSize = 0;
	}

	//! ~OpenGLVertexArray()
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		// Delete the array
		if (ResourceManager::getConfigValue(Config::PrintResourcesInDestructor))
			ENGINE_INFO("[OpenGLVertexArray::~OpenGLVertexArray] Deleting Vertex array with ID: {0}, Name: {1}.", m_arrayID, m_name);
		glDeleteVertexArrays(1, &m_arrayID);
	}

	//! addVertexBuffer()
	/*!
	\param vertexBuffer a VertexBuffer* - The vertex buffer to add to the array
	*/
	void OpenGLVertexArray::addVertexBuffer(VertexBuffer* vertexBuffer)
	{
		m_vertexBuffers.push_back(vertexBuffer);

		// Bind array
		bind();
		vertexBuffer->bind();

		// Configure the vertex attributes based on the buffer layout
		const auto& layout = vertexBuffer->getLayout();

		for (const auto& element : layout)
		{
			if (element.m_dataType == ShaderDataType::FlatInt || element.m_dataType == ShaderDataType::FlatByte)
			{
				std::size_t convertedOffset = static_cast<std::size_t>(element.m_offset);
				glEnableVertexAttribArray(m_attribIndex);
				glVertexAttribIPointer(m_attribIndex, SDT::getComponentCount(element.m_dataType),
					SDT::toGLType(element.m_dataType), layout.getStride(), (void*)(convertedOffset));
				glVertexAttribDivisor(m_attribIndex, element.m_instanceDivisor);
				m_attribIndex++;
			}
			else if (element.m_dataType == ShaderDataType::Mat4)
			{
				uint8_t count = SDT::getComponentCount(element.m_dataType);
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(m_attribIndex);
					glVertexAttribPointer(m_attribIndex, count,
						SDT::toGLType(element.m_dataType), element.m_normalized ? GL_TRUE : GL_FALSE, layout.getStride(),
						(const void*)(sizeof(float) * count * i));
					glVertexAttribDivisor(m_attribIndex, element.m_instanceDivisor);
					m_attribIndex++;
				}
			}
			else
			{
				std::size_t convertedOffset = static_cast<std::size_t>(element.m_offset);
				glVertexAttribPointer(m_attribIndex, SDT::getComponentCount(element.m_dataType),
					SDT::toGLType(element.m_dataType), element.m_normalized ? GL_TRUE : GL_FALSE, layout.getStride(), (void*)(convertedOffset));
				glEnableVertexAttribArray(m_attribIndex);
				glVertexAttribDivisor(m_attribIndex, element.m_instanceDivisor);
				m_attribIndex++;
			}
		}

		m_totalByteSize += vertexBuffer->getByteSize();

	}

	//! setIndexBuffer()
	/*!
	\param indexBuffer an IndexBuffer* - The index buffer to set
	*/
	void OpenGLVertexArray::setIndexBuffer(IndexBuffer* indexBuffer)
	{
		bind();
		m_indexBuffer = indexBuffer;
		m_totalByteSize += indexBuffer->getByteSize();
		indexBuffer->bind();
	}

	//! bind
	void OpenGLVertexArray::bind()
	{
		glBindVertexArray(m_arrayID);
	}

	//! unbind
	void OpenGLVertexArray::unbind()
	{
		glBindVertexArray(0);
	}

	//! indexBufferBoundToArray()
	/*!
	\return a const bool - Is the index buffer correctly bound to this VAO
	*/
	const bool OpenGLVertexArray::indexBufferBoundToArray()
	{
		int boundIndexBuffer = 0;
		glGetVertexArrayiv(m_arrayID, GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundIndexBuffer);

		// Check the result ID is not 0 and is equal to the index buffer ID
		if (boundIndexBuffer != 0 && boundIndexBuffer == getIndexBuffer()->getBufferID())
			return true;
		else
			return false;
	}

	//! printDetails()
	void OpenGLVertexArray::printDetails()
	{
		ENGINE_TRACE("Array ID: {0}.", m_arrayID);
		ENGINE_TRACE("Number of Attributes: {0}.", m_attribIndex);

		for (int i = 0; i < m_vertexBuffers.size(); i++)
		{
			if (m_vertexBuffers[i])
				ENGINE_TRACE("VertexBuffer{0}: {1}", i, m_vertexBuffers[i]->getName());
			else
				ENGINE_TRACE("VertexBuffer{0}: NULL", i);
		}

		if (m_indexBuffer)
			ENGINE_TRACE("IndexBuffer: {0}", m_indexBuffer->getName());
		else
			ENGINE_TRACE("IndexBuffer: NULL");

		ENGINE_TRACE("Total Byte Size: {0}.", m_totalByteSize);
	}

}
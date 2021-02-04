/*! \file OpenGLVertexArray.cpp
*
* \brief An OpenGL Vertex Array
*
* \author DMU Course material
*
*/

#include <glad/glad.h>
#include "independent/systems/systems/log.h"
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
	OpenGLVertexArray::OpenGLVertexArray()
	{
		// Create array
		glCreateVertexArrays(1, &m_arrayID);
		glBindVertexArray(m_arrayID);
	}

	//! ~OpenGLVertexArray()
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		// Delete the array
		ENGINE_INFO("[OpenGLVertexArray::~OpenGLVertexArray] Deleting Vertex array with ID: {0}", m_arrayID);
		glDeleteVertexArrays(1, &m_arrayID);
	}

	//! addVertexBuffer()
	/*!
	\param vertexBuffer a const std::shared_ptr<VertexBuffer> - The vertex buffer to add to the array
	*/
	void OpenGLVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer)
	{
		m_vertexBuffers.push_back(vertexBuffer);

		// Bind array
		bind();
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->getBufferID());

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

	}

	//! setIndexBuffer()
	/*!
	\param indexBuffer a const std::shared_ptr<IndexBuffer> - The index buffer to set
	*/
	void OpenGLVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer> indexBuffer)
	{
		bind();
		m_indexBuffer = indexBuffer;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer->getBufferID());
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

}
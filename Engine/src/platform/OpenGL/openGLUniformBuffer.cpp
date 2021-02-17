/*! \file OpenGLUniformBuffer.cpp
*
* \brief An OpenGL Uniform buffer object
*
* \author DMU Course material
*
*/
#include "platform/OpenGL/openGLUniformBuffer.h"
#include "independent/systems/systems/log.h"
#include "independent/rendering/shaders/shaderProgram.h"
#include <glad/glad.h>

namespace Engine
{
	uint32_t OpenGLUniformBuffer::s_blockNumber = 0; //!< Initialise with value

	//! OpenGLUniformBuffer()
	/*!
	\param uniformBufferName a const std::string& - The name of the uniform buffer
	\param layout a const UniformBufferLayout& - The uniform buffer layout
	*/
	OpenGLUniformBuffer::OpenGLUniformBuffer(const std::string& uniformBufferName, const UniformBufferLayout& layout) : UniformBuffer(uniformBufferName)
	{
		// Assign this UBO a block number
		m_blockNumber = s_blockNumber;
		s_blockNumber++;

		// Set layout
		m_layout = layout;

		// Generate the UBO
		glGenBuffers(1, &m_bufferID);
		glBindBuffer(GL_UNIFORM_BUFFER, m_bufferID);
		glBufferData(GL_UNIFORM_BUFFER, m_layout.getStride(), nullptr, GL_DYNAMIC_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, m_blockNumber, m_bufferID, 0, m_layout.getStride());

		// Store all uniforms in the cache
		for (auto& element : m_layout)
			m_uniformCache[element.m_name] = std::pair<uint32_t, uint32_t>(element.m_offset, element.m_size);
	}

	//! ~OpenGLUniformBuffer
	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		ENGINE_INFO("[OpenGLUniformBuffer::~OpenGLUniformBuffer] Deleting Uniform buffer with ID: {0}, Name: {1}.", m_bufferID, m_name);
		glDeleteBuffers(1, &m_bufferID);
	}

	//! attachShaderBlock()
	/*!
	\param shader a ShaderProgram* - A pointer to the shader program
	\param blockName a const char* - The name of the uniform block in the shader program
	*/
	void OpenGLUniformBuffer::attachShaderBlock(ShaderProgram* shader, const char* blockName)
	{
		uint32_t blockIndex = glGetUniformBlockIndex(shader->getID(), blockName);
		if (blockIndex != GL_INVALID_INDEX)
			glUniformBlockBinding(shader->getID(), blockIndex, m_blockNumber);
		else
			ENGINE_ERROR("[OpenGLUniformBuffer::attachShaderBlock] Cannot find the uniform block in the shader program. Shader Name: {0}, Block name: {1}", shader->getName(), blockName);
	}

	//! uploadData()
	/*!
	\param uniformName a const char* - The name of the uniform in the uniform cache
	\param data a void* - The data as a void pointer
	*/
	void OpenGLUniformBuffer::uploadData(const char* uniformName, void* data)
	{
		auto& pair = m_uniformCache[uniformName];
		glBindBuffer(GL_UNIFORM_BUFFER, m_bufferID);
		glBufferSubData(GL_UNIFORM_BUFFER, pair.first, pair.second, data);
	}
}
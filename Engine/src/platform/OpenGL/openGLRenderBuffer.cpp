/*! \file openGLRenderBuffer.cpp
*
* \brief An OpenGL render buffer object
*
* \author Daniel Bullin
*
*/
#include "platform/OpenGL/openGLRenderBuffer.h"
#include <glad/glad.h>
#include "independent/systems/systems/log.h"

namespace Engine
{
	//! OpenGLRenderBuffer()
	/*!
	\param type a const AttachmentType - The type of attachment
	\param size a const glm::ivec2& - The dimensions of the renderbuffer targets
	*/
	OpenGLRenderBuffer::OpenGLRenderBuffer(const AttachmentType type, const glm::ivec2& size)
	{
		// Gen buffers
		glGenRenderbuffers(1, &m_bufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_bufferID);

		m_type = type;
		m_size = size;

		// Store differently based on attachment type
		switch (type)
		{
			case AttachmentType::Depth:
			{
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x, size.y);
				break;
			}
			case AttachmentType::DepthAndStencil:
			{
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
				break;
			}
		}
	}

	//! ~OpenGLRenderBuffer()
	OpenGLRenderBuffer::~OpenGLRenderBuffer()
	{
		// Delete the buffer
		ENGINE_INFO("[OpenGLRenderBuffer::~OpenGLRenderBuffer] Deleting Renderbuffer with ID: {0}", m_bufferID);
		glDeleteRenderbuffers(1, &m_bufferID);
	}
}
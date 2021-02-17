/*! \file openGLFrameBuffer.cpp
*
* \brief An OpenGL Frame buffer object
*
* \author Daniel Bullin
*
*/
#include "platform/OpenGL/openGLFrameBuffer.h"
#include "independent/systems/systems/log.h"
#include <glad/glad.h>

namespace Engine
{
	//! OpenGLFrameBuffer()
	/*!
	\param frameBufferName a const std::string& - The name of the framebuffer
	*/
	OpenGLFrameBuffer::OpenGLFrameBuffer(const std::string& frameBufferName) : FrameBuffer(frameBufferName)
	{
		m_bufferID = 0;
		m_size = { 0.f, 0.f };
	}

	//! OpenGLFrameBuffer()
	/*!
	\param frameBufferName a const std::string& - The name of the framebuffer
	\param size a const glm::ivec2& - The dimensions of the framebuffer targets
	\param layout a FrameBufferLayoutt& - A reference to the layout of the framebuffer
	*/
	OpenGLFrameBuffer::OpenGLFrameBuffer(const std::string& frameBufferName, const glm::ivec2& size, FrameBufferLayout& layout) : FrameBuffer(frameBufferName)
	{
		// Set the size and layout
		m_size = size;
		m_layout = layout;

		// Generate a new framebuffer
		glGenFramebuffers(1, &m_bufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_bufferID);

		uint32_t colourAttachmentCount = 0;

		for (auto&[type, isSampled] : m_layout)
		{
			if (isSampled)
			{
				switch (type)
				{
					case AttachmentType::Colour:
					{
						TextureProperties properties(size.x, size.y, "Repeat", "Repeat", "Repeat", "Linear", "Linear", false, false);
						std::string textureName = "Colour" + std::to_string(colourAttachmentCount);

						Texture2D* colourTexture = Texture2D::create(textureName, properties, 3, nullptr);
						m_sampledTargets[textureName] = colourTexture;
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colourAttachmentCount, GL_TEXTURE_2D, colourTexture->getID(), 0);
						colourAttachmentCount++;
						break;
					}
					case AttachmentType::Depth:
					{
						TextureProperties properties(size.x, size.y, "Repeat", "Repeat", "Repeat", "Linear", "Linear", false, false);
						std::string textureName = "Depth";

						Texture2D* depthTexture = Texture2D::create(textureName, properties, 2, nullptr);
						m_sampledTargets[textureName] = depthTexture;
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->getID(), 0);
						break;
					}
				}
			}
			else
			{
				switch (type)
				{
					case AttachmentType::Depth:
					{
						m_nonSampledTargets["Depth"] = RenderBuffer::create(type, size);
						glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_nonSampledTargets["Depth"]->getID());
						break;
					}
					case AttachmentType::DepthAndStencil:
					{
						m_nonSampledTargets["Depth"] = RenderBuffer::create(type, size);
						glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_nonSampledTargets["Depth"]->getID());
						break;
					}
				}
			}
		}

		// Check if the framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			ENGINE_ERROR("[OpenGLFrameBuffer::OpenGLFrameBuffer] The frame buffer is not complete. ID: {0}.", m_bufferID);

		// Unbind for safety
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	//! ~OpenGLFrameBuffer()
	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		// Delete the buffer
		ENGINE_INFO("[OpenGLFrameBuffer::~OpenGLFrameBuffer] Deleting Framebuffer with ID: {0}, Name: {1}.", m_bufferID, m_name);

		for (auto& target : m_sampledTargets)
			delete target.second;

		m_sampledTargets.clear();

		for (auto& target : m_nonSampledTargets)
			delete target.second;

		m_nonSampledTargets.clear();

		glDeleteFramebuffers(1, &m_bufferID);
	}

	//! bind()
	void OpenGLFrameBuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_bufferID);
	}
}
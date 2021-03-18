/*! \file openGLFrameBuffer.cpp
*
* \brief An OpenGL Frame buffer object
*
* \author Daniel Bullin
*
*/
#include "platform/OpenGL/openGLFrameBuffer.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/windowManager.h"
#include "independent/systems/systems/renderSystem.h"
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
		m_default = true;
		m_useSceneSize = false;
	}

	//! OpenGLFrameBuffer()
	/*!
	\param frameBufferName a const std::string& - The name of the framebuffer
	\param useSceneSize a const bool - Is this framebuffer to be the size of the scene view
	\param size a const glm::ivec2& - The dimensions of the framebuffer targets
	\param layout a FrameBufferLayoutt& - A reference to the layout of the framebuffer
	*/
	OpenGLFrameBuffer::OpenGLFrameBuffer(const std::string& frameBufferName, const bool useSceneSize, const glm::ivec2& size, FrameBufferLayout& layout) : FrameBuffer(frameBufferName)
	{
		// Set the size and layout
		m_bufferID = 0;
		m_size = size;
		m_layout = layout;
		m_useSceneSize = useSceneSize;
		m_default = false;

		resize(size);
	}

	//! ~OpenGLFrameBuffer()
	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		// Delete the buffer
		if (ResourceManager::getConfigValue(Config::PrintResourcesInDestructor))
			ENGINE_INFO("[OpenGLFrameBuffer::~OpenGLFrameBuffer] Deleting Framebuffer with ID: {0}, Name: {1}.", m_bufferID, m_name);

		// Delete all attachments
		for (auto& target : m_sampledTargets)
		{
			if (target.second)
				delete target.second;
		}

		m_sampledTargets.clear();

		for (auto& target : m_nonSampledTargets)
		{
			if (target.second)
				delete target.second;
		}

		m_nonSampledTargets.clear();

		glDeleteFramebuffers(1, &m_bufferID);
	}

	//! bind()
	void OpenGLFrameBuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_bufferID);
		if (m_default)
		{
			auto window = WindowManager::getFocusedWindow();
			glViewport(0, 0, window->getProperties().getSize().x, window->getProperties().getSize().y);
		}
		else
			glViewport(0, 0, m_size.x, m_size.y);
	}

	//! resize()
	/*
	\param size a const glm::ivec2& - The size (dimensions) of the framebuffer
	*/
	void OpenGLFrameBuffer::resize(const glm::ivec2& size)
	{
		// Check to see if we need to delete the current framebuffer
		if (ResourceManager::getConfigValue(Config::PrintResourcesInDestructor))
			ENGINE_INFO("[OpenGLFrameBuffer::resize] Resizing framebuffer: {0}.", m_name);

		if (m_default)
			return;

		if (m_bufferID)
		{
			// Delete all attachments
			for (auto& target : m_sampledTargets)
			{
				if (target.second)
					delete target.second;
			}

			m_sampledTargets.clear();

			for (auto& target : m_nonSampledTargets)
			{
				if (target.second)
					delete target.second;
			}

			m_nonSampledTargets.clear();

			glDeleteFramebuffers(1, &m_bufferID);
			m_bufferID = 0;
		}

		// Generate a new framebuffer
		glGenFramebuffers(1, &m_bufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_bufferID);

		m_size = size;

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
				case AttachmentType::Colour16:
				{
					TextureProperties properties(size.x, size.y, "ClampToEdge", "ClampToEdge", "ClampToEdge", "Linear", "Linear", false, false);
					std::string textureName = "Colour" + std::to_string(colourAttachmentCount);

					Texture2D* colourTexture = Texture2D::create(textureName, properties, 5, nullptr);
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

		unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(colourAttachmentCount, attachments);

		// Check if the framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			ENGINE_ERROR("[OpenGLFrameBuffer::OpenGLFrameBuffer] The frame buffer is not complete. ID: {0}.", m_bufferID);

		// Unbind for safety
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		RenderSystem::getTextureUnitManager()->clear(true);
	}

	//! printDetails()
	void OpenGLFrameBuffer::printDetails()
	{
		ENGINE_TRACE("Buffer ID: {0}.", m_bufferID);
		ENGINE_TRACE("Size: {0}, {1}.", m_size.x, m_size.y);

		auto attachments = m_layout.getAttachments();
		for (int i = 0; i < attachments.size(); i++)
			ENGINE_TRACE("Attachment{0}: Type: {1}, Sampled: {2}.", i, BufferAttachments::toString(attachments[i].first), attachments[i].second);

		ENGINE_TRACE("Is Default FrameBuffer: {0}.", m_default);

		for (auto& target : m_sampledTargets)
		{
			if (target.second)
			{
				ENGINE_TRACE("SampledAttachment: Name: {0}.", target.first);
				ENGINE_TRACE("=========================");
				target.second->printDetails();
				ENGINE_TRACE("=========================");
			}
			else
				ENGINE_TRACE("SampledAttachment: Name: {0}, Texture: NULL.", target.first);
		}

		for (auto& target : m_nonSampledTargets)
		{
			if (target.second)
				ENGINE_TRACE("NonSampledAttachment: Buffer ID: {0}, Type: {1}, Size: {2}, {3}.", target.second->getID(), BufferAttachments::toString(target.second->getAttachmentType()), target.second->getSize().x, target.second->getSize().y);
		}
	}
}
/*! \file frameBuffer.cpp
*
* \brief A frame buffer object
*
* \author Daniel Bullin
*
*/

#include "independent/systems/systems/log.h"
#include "independent/rendering/renderAPI.h"
#include "independent/rendering/frameBuffer.h"
#include "platform/OpenGL/openGLFrameBuffer.h"

namespace Engine
{
	//! create()
	/*!
	\param size a const glm::ivec2& - The dimensions of the framebuffer targets
	\param layout a FrameBufferLayoutt& - A reference to the layout of the framebuffer
	*/
	FrameBuffer* FrameBuffer::create(const glm::ivec2& size, FrameBufferLayout& layout)
	{
		switch (RenderAPI::getAPI())
		{
			case GraphicsAPI::None:
			{
				ENGINE_ERROR("[FrameBuffer::create] No rendering API selected.");
				break;
			}
			case GraphicsAPI::OpenGL:
			{
				return new OpenGLFrameBuffer(size, layout);
			}
			case GraphicsAPI::Direct3D:
			{
				ENGINE_ERROR("[FrameBuffer::create] Direct3D not supported.");
				break;
			}
			case GraphicsAPI::Vulkan:
			{
				ENGINE_ERROR("[FrameBuffer::create] Vulkan not supported.");
				break;
			}
		}
		return nullptr;
	}

	//! createDefault()
	FrameBuffer* FrameBuffer::createDefault()
	{
		switch (RenderAPI::getAPI())
		{
			case GraphicsAPI::None:
			{
				ENGINE_ERROR("[FrameBuffer::createDefault] No rendering API selected.");
				break;
			}
			case GraphicsAPI::OpenGL:
			{
				return new OpenGLFrameBuffer;
			}
			case GraphicsAPI::Direct3D:
			{
				ENGINE_ERROR("[FrameBuffer::createDefault] Direct3D not supported.");
				break;
			}
			case GraphicsAPI::Vulkan:
			{
				ENGINE_ERROR("[FrameBuffer::createDefault] Vulkan not supported.");
				break;
			}
		}
		return nullptr;
	}

	//! addAttachment()
	/*!
	\param type a const AttachmentType - The type of attachment
	\param sampled a const bool - Is the attachment to be sampled
	*/
	void FrameBufferLayout::addAttachment(const AttachmentType type, const bool sampled)
	{
		m_attachments.push_back(Attachment(type, sampled));
	}

	//! getSampledTarget()
	/*!
	\param targetName a const char* - The name of the target
	\return a Texture2D* - A pointer to the texture target
	*/
	Texture2D* FrameBuffer::getSampledTarget(const char* targetName)
	{
		if (m_sampledTargets.find(targetName) != m_sampledTargets.end())
			return m_sampledTargets[targetName];
		else
			return nullptr;
	}

	//! getNonSampledTarget()
	/*!
	\param targetName a const char* - The name of the target
	\return a RenderBuffer* - A pointer to the renderbuffer target
	*/
	RenderBuffer* FrameBuffer::getNonSampledTarget(const char* targetName)
	{
		if (m_nonSampledTargets.find(targetName) != m_nonSampledTargets.end())
			return m_nonSampledTargets[targetName];
		else
			return nullptr;
	}
}
/*! \file renderBuffer.cpp
*
* \brief A render buffer object
*
* \author Daniel Bullin
*
*/
#include "independent/rendering/renderBuffer.h"
#include "independent/systems/systems/log.h"
#include "independent/rendering/renderAPI.h"
#include "platform/OpenGL/openGLRenderBuffer.h"

namespace Engine
{
	//! RenderBuffer()
	RenderBuffer::RenderBuffer()
	{
	}

	//! ~RenderBuffer()
	RenderBuffer::~RenderBuffer()
	{
	}

	//! create()
	/*!
	\param type an AttachmentType - The type of attachment
	\param size a const glm::ivec2& - The dimensions of the render buffer targets
	\return a RenderBuffer* - The render buffer based on the current graphics API
	*/
	RenderBuffer* RenderBuffer::create(AttachmentType type, const glm::ivec2& size)
	{
		switch (RenderAPI::getAPI())
		{
			case GraphicsAPI::None:
			{
				ENGINE_ERROR("[RenderBuffer::create] No rendering API selected.");
				break;
			}
			case GraphicsAPI::OpenGL:
			{
				return new OpenGLRenderBuffer(type, size);
			}
			case GraphicsAPI::Direct3D:
			{
				ENGINE_ERROR("[RenderBuffer::create] Direct3D not supported.");
				break;
			}
			case GraphicsAPI::Vulkan:
			{
				ENGINE_ERROR("[RenderBuffer::create] Vulkan not supported.");
				break;
			}
		}
		return nullptr;
	}
}
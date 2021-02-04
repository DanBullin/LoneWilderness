/*! \file uniformBuffer.cpp
*
* \brief A uniform buffer object
*
* \author DMU Course material
*
*/

#include "independent/systems/systems/log.h"
#include "independent/rendering/renderAPI.h"
#include "independent/rendering/uniformBuffer.h"
#include "platform/OpenGL/openGLUniformBuffer.h"

namespace Engine
{
	//! create()
	/*!
	\param layout a const UniformBufferLayout& - The uniform buffer layout
	\return a UniformBuffer* - The uniform buffer based on the current graphics API
	*/
	UniformBuffer* UniformBuffer::create(const UniformBufferLayout& layout)
	{
		switch (RenderAPI::getAPI())
		{
			case GraphicsAPI::None:
			{
				ENGINE_ERROR("[UniformBuffer::create] No rendering API selected.");
				break;
			}
			case GraphicsAPI::OpenGL:
			{
				return new OpenGLUniformBuffer(layout);
			}
			case GraphicsAPI::Direct3D:
			{
				ENGINE_ERROR("[UniformBuffer::create] Direct3D not supported.");
				break;
			}
			case GraphicsAPI::Vulkan:
			{
				ENGINE_ERROR("[UniformBuffer::create] Vulkan not supported.");
				break;
			}
		}
		return nullptr;
	}
}
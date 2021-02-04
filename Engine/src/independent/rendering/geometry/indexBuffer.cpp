/*! \file indexBuffer.cpp
*
* \brief An API agnostic Index buffer
*
* \author DMU Course material
*
*/

#include "independent/rendering/renderAPI.h"
#include "independent/systems/systems/log.h"
#include "independent/rendering/geometry/indexBuffer.h"
#include "platform/OpenGL/geometry/openGLIndexBuffer.h"

namespace Engine
{
	//! create()
	/*!
	\param indices a const uint32_t* - The indices info
	\param count a const uint32_t - The number of indices
	\return an IndexBuffer* - The index buffer of type defined by the graphics API chosen
	*/
	IndexBuffer* IndexBuffer::create(const uint32_t* indices, const uint32_t count)
	{
		switch (RenderAPI::getAPI())
		{
			case GraphicsAPI::None:
			{
				ENGINE_ERROR("[IndexBuffer::create] No rendering API selected.");
				break;
			}
			case GraphicsAPI::OpenGL:
			{
				return new OpenGLIndexBuffer(indices, count);
			}
			case GraphicsAPI::Direct3D:
			{
				ENGINE_ERROR("[IndexBuffer::create] Direct3D not supported.");
				break;
			}
			case GraphicsAPI::Vulkan:
			{
				ENGINE_ERROR("[IndexBuffer::create] Vulkan not supported.");
				break;
			}
		}
		return nullptr;
	}
}
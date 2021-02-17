/*! \file uniformBuffer.cpp
*
* \brief A uniform buffer object
*
* \author DMU Course material
*
*/
#include "independent/rendering/uniformBuffer.h"
#include "independent/systems/systems/log.h"
#include "independent/rendering/renderAPI.h"
#include "platform/OpenGL/openGLUniformBuffer.h"

namespace Engine
{
	//! UniformBuffer()
	/*
	\param uniformBufferName a const std::string& - The name of the uniform buffer
	*/
	UniformBuffer::UniformBuffer(const std::string& uniformBufferName) : Resource(uniformBufferName, ResourceType::UniformBuffer)
	{
	}

	//! ~UniformBuffer()
	UniformBuffer::~UniformBuffer()
	{
	}

	//! create()
	/*!
	\param uniformBufferName a const std::string& - The name of the uniform buffer
	\param layout a const UniformBufferLayout& - The uniform buffer layout
	\return a UniformBuffer* - The uniform buffer based on the current graphics API
	*/
	UniformBuffer* UniformBuffer::create(const std::string& uniformBufferName, const UniformBufferLayout& layout)
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
				return new OpenGLUniformBuffer(uniformBufferName, layout);
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
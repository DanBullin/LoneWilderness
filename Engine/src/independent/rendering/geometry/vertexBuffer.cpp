/*! \file vertexBuffer.cpp
*
* \brief An API agnostic Vertex buffer
*
* \author DMU Course material
*
*/
#include "independent/rendering/renderAPI.h"
#include "independent/systems/systems/log.h"
#include "independent/rendering/geometry/vertexBuffer.h"
#include "platform/OpenGL/geometry/openGLVertexBuffer.h"

namespace Engine
{
	//! VertexBuffer()
	/*
	\param vertexBufferName a const std::string& - The name of the vertex buffer
	*/
	VertexBuffer::VertexBuffer(const std::string& vertexBufferName) : Resource(vertexBufferName, ResourceType::VertexBuffer)
	{
	}

	//! ~VertexBuffer()
	VertexBuffer::~VertexBuffer()
	{
	}

	//! create()
	/*!
	\param vertexBufferName a const std::string& - The name of the vertex buffer
	\param vertices a const void* - The vertices info
	\param size a const uint32_t - The size in bytes of the vertices data
	\param layout a const VertexBufferLayout - The layout of the vertex buffer
	\param usage a const VertexBufferUsage - The usage of the vertices data
	\return a VertexBuffer* - The vertex buffer based on the current graphics API
	*/
	VertexBuffer* VertexBuffer::create(const std::string& vertexBufferName, const void* vertices, const uint32_t size, const VertexBufferLayout& layout, const VertexBufferUsage usage)
	{
		switch (RenderAPI::getAPI())
		{
			case GraphicsAPI::None:
			{
				ENGINE_ERROR("[VertexBuffer::create] No rendering API selected.");
				break;
			}
			case GraphicsAPI::OpenGL:
			{
				return new OpenGLVertexBuffer(vertexBufferName, vertices, size, layout, usage);
			}
			case GraphicsAPI::Direct3D:
			{
				ENGINE_ERROR("[VertexBuffer::create] Direct3D not supported.");
				break;
			}
			case GraphicsAPI::Vulkan:
			{
				ENGINE_ERROR("[VertexBuffer::create] Vulkan not supported.");
				break;
			}
		}
		return nullptr;
	}

	//! create()
	/*!
	\param vertexBufferName a const std::string& - The name of the vertex buffer
	\param size a const uint32_t - The size in bytes of the buffer data
	\param usage a const VertexBufferUsage - The usage of the vertices data
	\return VertexBuffer* - The vertex buffer based on the current graphics API
	*/
	VertexBuffer* VertexBuffer::create(const std::string& vertexBufferName, const uint32_t size, const VertexBufferUsage usage)
	{
		switch (RenderAPI::getAPI())
		{
			case GraphicsAPI::None:
			{
				ENGINE_ERROR("[VertexBuffer::create] No rendering API selected.");
				break;
			}
			case GraphicsAPI::OpenGL:
			{
				return new OpenGLVertexBuffer(vertexBufferName, size, usage);
			}
			case GraphicsAPI::Direct3D:
			{
				ENGINE_ERROR("[VertexBuffer::create] Direct3D not supported.");
				break;
			}
			case GraphicsAPI::Vulkan:
			{
				ENGINE_ERROR("[VertexBuffer::create] Vulkan not supported.");
				break;
			}
		}
		return nullptr;
	}
}
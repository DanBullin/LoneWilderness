/*! \file OpenGLVertexBuffer.h
*
* \brief An OpenGL Vertex buffer
*
* \author DMU Course material
*
*/
#ifndef OPENGLVERTEXBUFFER_H
#define OPENGLVERTEXBUFFER_H

#include "independent/rendering/geometry/bufferLayout.h"
#include "independent/rendering/geometry/vertexBuffer.h"

namespace Engine
{
	/*! \class OpenGLVertexBuffer
	* \brief An OpenGL Vertex buffer object
	*/
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const void* vertices, const uint32_t size, const VertexBufferLayout layout, const VertexBufferUsage usage = VertexBufferUsage::StaticDraw); //!< Constructor
		OpenGLVertexBuffer(const uint32_t size, const VertexBufferUsage usage = VertexBufferUsage::DynamicDraw); //!< Constructor
		virtual ~OpenGLVertexBuffer(); //!< Destructor

		virtual void edit(const void* vertices, const uint32_t size, const uint32_t offset) override; //!< Edit the buffer contents
	};
}
#endif
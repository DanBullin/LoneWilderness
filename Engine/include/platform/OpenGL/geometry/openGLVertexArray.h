/*! \file OpenGLVertexArray.h
*
* \brief An OpenGL Vertex Array
*
* \author DMU Course material
*
*/
#ifndef OPENGLVERTEXARRAY_H
#define OPENGLVERTEXARRAY_H

#include "independent/rendering/geometry/vertexArray.h"
#include "platform/OpenGL/geometry/openGLVertexBuffer.h"
#include "platform/OpenGL/geometry/openGLIndexBuffer.h"

namespace Engine
{
	/*! \class OpenGLVertexArray
	* \brief An OpenGL vertex array object
	*/
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray(); //!< Constructor
		~OpenGLVertexArray(); //!< Destructor
		virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer) override; //!< Add vertex buffer to array
		virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer> indexBuffer) override; //!< Set the index buffer
		virtual void bind() override; //!< Bind the VAO
		virtual void unbind() override; //!< Unbind the VAO
	};
}
#endif
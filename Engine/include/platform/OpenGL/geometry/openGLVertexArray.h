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
		OpenGLVertexArray(const std::string& vertexArrayName); //!< Constructor
		~OpenGLVertexArray(); //!< Destructor
		void addVertexBuffer(VertexBuffer* vertexBuffer) override; //!< Add vertex buffer to array
		void setIndexBuffer(IndexBuffer* indexBuffer) override; //!< Set the index buffer
		void bind() override; //!< Bind the VAO
		void unbind() override; //!< Unbind the VAO
		const bool indexBufferBoundToArray() override; //!< Check if the index buffer set is correctly bound to the array
		void printDetails() override; //!< Print the resource details
	};
}
#endif
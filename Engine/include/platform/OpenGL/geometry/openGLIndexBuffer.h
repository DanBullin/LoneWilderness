/*! \file openGLIndexBuffer.h
*
* \brief An OpenGL Index buffer
*
* \author DMU Course material
*
*/
#ifndef OPENGLINDEXBUFFER_H
#define OPENGLINDEXBUFFER_H

#include "independent/rendering/geometry/indexBuffer.h"

namespace Engine
{
	/*! \class OpenGLIndexBuffer
	* \brief An OpenGL index buffer object
	*/
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const std::string& indexBufferName, const uint32_t* indices, const uint32_t count); //!< Constructor
		~OpenGLIndexBuffer(); //!< Destructor

		void edit(const void* indices, const uint32_t size, const uint32_t offset) override; //!< Edit the index buffer contents
		void bind() override; //!< Bind the buffer
		void unbind() override; //!< Unbind the buffer
		void printDetails() override; //!< Print the resource details
	};
}
#endif
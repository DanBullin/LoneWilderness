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
		OpenGLIndexBuffer(const uint32_t* indices, const uint32_t count); //!< Constructor
		virtual ~OpenGLIndexBuffer(); //!< Destructor

		virtual void edit(const void* indices, const uint32_t size, const uint32_t offset) override; //!< Edit the index buffer contents
	};
}
#endif
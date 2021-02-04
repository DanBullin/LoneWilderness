/*! \file openGLFrameBuffer.h
*
* \brief An OpenGL Frame buffer object
*
* \author Daniel Bullin
*
*/
#ifndef OPENGLFRAMEBUFFER_H
#define OPENGLFRAMEBUFFER_H

#include "independent/rendering/frameBuffer.h"

namespace Engine
{
	/*! \class OpenGLFrameBuffer
	* \brief An OpenGL Framebuffer
	*/
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(); //!< Default constructor
		OpenGLFrameBuffer(const glm::ivec2& size, FrameBufferLayout& layout); //!< Constructor
		~OpenGLFrameBuffer(); //!< Destructor

		void bind() override; //!< Bind the frame buffer
	};
}
#endif
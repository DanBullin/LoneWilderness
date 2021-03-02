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
		OpenGLFrameBuffer(const std::string& frameBufferName); //!< Default constructor
		OpenGLFrameBuffer(const std::string& frameBufferName, const bool useSceneSize, const glm::ivec2& size, FrameBufferLayout& layout); //!< Constructor
		~OpenGLFrameBuffer(); //!< Destructor

		void bind() override; //!< Bind the frame buffer
		void resize(const glm::ivec2& size) override; //!< Resize the frame buffer
		void printDetails() override; //!< Print the resource details
	};
}
#endif
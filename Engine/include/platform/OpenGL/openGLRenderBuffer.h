/*! \file openGLRenderBuffer.h
*
* \brief An OpenGL render buffer object
*
* \author Daniel Bullin
*
*/
#ifndef OPENGLRENDERBUFFER_H
#define OPENGLRENDERBUFFER_H

#include "independent/rendering/renderBuffer.h"

namespace Engine
{
	/*! \class OpenGLRenderBuffer
	* \brief An OpenGL render buffer object
	*/
	class OpenGLRenderBuffer : public RenderBuffer
	{
	public:
		OpenGLRenderBuffer(const AttachmentType type, const glm::ivec2& size); //!< Constructor
		virtual ~OpenGLRenderBuffer(); //!< Destructor
	};
}
#endif
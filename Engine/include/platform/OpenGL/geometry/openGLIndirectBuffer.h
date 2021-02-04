/*! \file openGLIndirectBuffer.h
*
* \brief An OpenGL Indirect buffer
*
* \author DMU Course material
*
*/
#ifndef OPENGLINDRIECTBUFFER_H
#define OPENGLINDRIECTBUFFER_H

#include "independent/rendering/geometry/indirectBuffer.h"

namespace Engine
{
	/*! \class OpenGLIndirectBuffer
	* \brief An OpenGL Indirect buffer
	*/
	class OpenGLIndirectBuffer : public IndirectBuffer
	{
	public:
		OpenGLIndirectBuffer(DrawElementsIndirectCommand* commands, const uint32_t count); //!< Constructor
		virtual ~OpenGLIndirectBuffer(); //!< Destructor
		virtual void edit(DrawElementsIndirectCommand* commands, const uint32_t count, const uint32_t offset); //!< Edit the buffer contents
	};
}
#endif
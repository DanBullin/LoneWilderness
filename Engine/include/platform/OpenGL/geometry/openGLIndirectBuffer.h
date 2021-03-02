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
		OpenGLIndirectBuffer(const std::string& indirectBufferName, DrawElementsIndirectCommand* commands, const uint32_t count); //!< Constructor
		~OpenGLIndirectBuffer(); //!< Destructor
		void edit(DrawElementsIndirectCommand* commands, const uint32_t count, const uint32_t offset) override; //!< Edit the buffer contents
		void printDetails() override; //!< Print the resource details
	};
}
#endif
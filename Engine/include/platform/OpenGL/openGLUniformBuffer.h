/*! \file OpenGLUniformBuffer.h
*
* \brief An OpenGL Uniform buffer object
*
* \author DMU Course material
*
*/
#ifndef OPENGLUNIFORMBUFFER_H
#define OPENGLUNIFORMBUFFER_H

#include "independent/rendering/uniformBuffer.h"

namespace Engine
{
	/*! \class OpenGLUniformBuffer
	* \brief An OpenGL implementation of the UniformBuffer
	*/
	class OpenGLUniformBuffer : public UniformBuffer
	{
	private:
		static uint32_t s_blockNumber; //!< Global block number
	public:
		OpenGLUniformBuffer(const std::string& uniformBufferName, const UniformBufferLayout& layout); //!< Constructor
		~OpenGLUniformBuffer(); //!< Destructor

		void attachShaderBlock(ShaderProgram* shader, const char* blockName) override; //!< Attach shader block
		void uploadData(const char* uniformName, void* data) override; //!< Upload data to buffer
		void printDetails() override; //!< Print the resource details
	};
}
#endif
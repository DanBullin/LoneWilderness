/*! \file uniformBuffer.h 
*
* \brief A uniform buffer object
*
* \author DMU Course material
*
*/
#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#include "independent/core/common.h"
#include "independent/rendering/geometry/bufferLayout.h"
#include "independent/rendering/shaders/shaderProgram.h"

namespace Engine
{
	/*! \class UniformBuffer
	* \brief An API agnostic uniform buffer object holding data to be sent to shader programs
	*/
	class UniformBuffer
	{
	protected:
		uint32_t m_bufferID; //!< The buffer ID
		UniformBufferLayout m_layout; //!< Uniform buffer layout
		std::unordered_map<const char*, std::pair<uint32_t, uint32_t>> m_uniformCache; //!< Stores uniform names with offsets and sizes
		uint32_t m_blockNumber; //!< Block number for this UBO
	public:
		virtual ~UniformBuffer() = default; //!< Destructor

		virtual inline const uint32_t getID() const { return m_bufferID; } //!< Get the uniform buffer id
			/*!< \return a const uint32_t - The uniform buffer ID */
		virtual inline UniformBufferLayout& getUniformLayout() { return m_layout; } //!< Get the uniform buffer layout
			/*!< \return a UniformBufferLayout& - The uniform buffer layout */

		virtual void attachShaderBlock(ShaderProgram* shader, const char* blockName) = 0; //!< Attach the shader block
			/*!< \param shader a ShaderProgram* - A pointer to the shader program
				 \param blockName a const char* - The name of the uniform block */
		virtual void uploadData(const char* uniformName, void* data) = 0; //!< Upload the data to the GPU
			/*!< \param uniformName a const char* - The uniform name
				 \param data a void* - A pointer to the data */

		static UniformBuffer* create(const UniformBufferLayout& layout); //!< Create a uniform buffer
	};
}
#endif
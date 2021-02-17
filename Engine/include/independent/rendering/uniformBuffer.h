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
#include "independent/systems/components/resource.h"
#include "independent/rendering/geometry/bufferLayout.h"

namespace Engine
{
	class ShaderProgram; //!< Forward declare shader program

	/*! \class UniformBuffer
	* \brief An API agnostic uniform buffer object holding data to be sent to shader programs
	*/
	class UniformBuffer : public Resource
	{
	protected:
		uint32_t m_bufferID; //!< The buffer ID
		UniformBufferLayout m_layout; //!< Uniform buffer layout
		std::unordered_map<std::string, std::pair<uint32_t, uint32_t>> m_uniformCache; //!< Stores uniform names with offsets and sizes
		uint32_t m_blockNumber; //!< Block number for this UBO
	public:
		static UniformBuffer* create(const std::string& uniformBufferName, const UniformBufferLayout& layout); //!< Create a uniform buffer

		UniformBuffer(const std::string& uniformBufferName); //!< Constructor
		virtual ~UniformBuffer(); //!< Destructor

		inline const uint32_t getID() const { return m_bufferID; } //!< Get the uniform buffer id
			/*!< \return a const uint32_t - The uniform buffer ID */
		inline UniformBufferLayout& getUniformLayout() { return m_layout; } //!< Get the uniform buffer layout
			/*!< \return a UniformBufferLayout& - The uniform buffer layout */

		virtual void attachShaderBlock(ShaderProgram* shader, const char* blockName) = 0; //!< Attach the shader block
			/*!< \param shader a ShaderProgram* - A pointer to the shader program
				 \param blockName a const char* - The name of the uniform block */
		virtual void uploadData(const char* uniformName, void* data) = 0; //!< Upload the data to the GPU
			/*!< \param uniformName a const char* - The uniform name
				 \param data a void* - A pointer to the data */
	};
}
#endif
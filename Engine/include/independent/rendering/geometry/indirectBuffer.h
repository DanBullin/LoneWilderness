/*! \file indirectBuffer.h
*
* \brief An API agnostic Indirect buffer
*
* \author DMU Course material
*
*/
#ifndef INDIRECTBUFFER_H
#define INDIRECTBUFFER_H

#include "independent/systems/components/resource.h"
#include <inttypes.h>

namespace Engine
{
	/*! \struct DrawElementsIndirectCommand
	* \brief The data needed to pass to draw command
	*/
	struct DrawElementsIndirectCommand
	{
		uint32_t DrawCount; //!< The number of indices to draw
		uint32_t InstanceCount; //!< The number of instances to draw
		uint32_t FirstIndex; //!< The first index to draw
		uint32_t FirstVertex; //!< The first vertex to draw
		uint32_t FirstInstance; //!< The first instance in this batch
	};

	/*! \class IndirectBuffer
	* \brief An API agnostic Indirect buffer
	*/
	class IndirectBuffer : public Resource
	{
	protected:
		uint32_t m_bufferID; //!< The indirect buffer ID
		uint32_t m_commandCount; //!< The number of commands
		uint32_t m_byteSize; //!< The size of the indirect buffer in bytes
	public:
		static IndirectBuffer* create(const std::string& indirectBufferName, DrawElementsIndirectCommand* commands, const uint32_t count); //!< Create an indirect buffer

		IndirectBuffer(const std::string& indirectBufferName); //!< Constructor
		virtual ~IndirectBuffer(); //!< Destructor

		virtual void edit(DrawElementsIndirectCommand* commands, const uint32_t count, const uint32_t offset) = 0; //!< Edit the buffer contents
			/*!< \param commands a DrawElementsIndirectCommand* - The commands info
				 \param count a const uint32_t - The number of commands
				 \param offset a const uint32_t - The offset in memory */

		inline const uint32_t getBufferID() const { return m_bufferID; }  //!< Get the buffer ID
			/*!< \return a const uint32_t - The ID of the buffer */
		inline const uint32_t getCount() const { return m_commandCount; }  //!< Get the command count
			/*!< \return a const uint32_t - The command count */

		virtual void printDetails() override = 0; //!< Print the resource details
	};
}
#endif
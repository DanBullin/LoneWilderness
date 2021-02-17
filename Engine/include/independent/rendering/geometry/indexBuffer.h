/*! \file indexBuffer.h
*
* \brief An API agnostic Index buffer
*
* \author DMU Course material
*
*/
#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include "independent/core/common.h"
#include "independent/systems/components/resource.h"

namespace Engine
{
	/*! \class IndexBuffer
	* \brief An API agnostic Index buffer
	*/
	class IndexBuffer : public Resource
	{
	protected:
		uint32_t m_indexBufferID; //!< The index Buffer ID
		uint32_t m_count; //!< The number of indices
		uint32_t m_byteSize; //!< The size of this buffer in bytes
	public:
		static IndexBuffer* create(const std::string& indexBufferName, const uint32_t* indices, const uint32_t count); //!< Create an index buffer

		IndexBuffer(const std::string& indexBufferName); //!< Constructor
		virtual ~IndexBuffer(); //!< Destructor

		inline const uint32_t getBufferID() const { return m_indexBufferID; } //!< Get the buffer ID
			/*!< \return a const uint32_t - The ID of the buffer */
		inline const uint32_t getByteSize() const { return m_byteSize; }  //!< Get the size of this buffer in bytes
			/*!< \return a const uint32_t - The size of the index buffer in bytes */
		inline const uint32_t getIndicesCount() const { return m_count; } //!< Get the number of indices
			/*!< \return a const uint32_t - The number of indices */

		virtual void edit(const void* indices, const uint32_t size, const uint32_t offset) = 0; //!< Edit the index buffer contents
			/*!< \param indices a const void* -The indices info
				 \param size a const uint32_t - The number of indices in the indices data
				 \param offset a const int32_t - The offset of where to place the new data in terms of index count */

		virtual void bind() = 0; //!< Bind the buffer
		virtual void unbind() = 0; //!< Unbind the buffer
	};
}
#endif
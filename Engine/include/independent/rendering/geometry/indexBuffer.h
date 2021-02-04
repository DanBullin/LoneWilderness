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

namespace Engine
{
	/*! \class IndexBuffer
	* \brief An API agnostic Index buffer
	*/
	class IndexBuffer
	{
	protected:
		uint32_t m_indexBufferID; //!< The index Buffer ID
		uint32_t m_count; //!< The number of indices
	public:
		virtual ~IndexBuffer() = default; //!< Destructor
		virtual inline const uint32_t getBufferID() const { return m_indexBufferID; } //!< Get the buffer ID
			/*!< \return a const uint32_t - The ID of the buffer */
		virtual inline const uint32_t getIndicesCount() const { return m_count; } //!< Get the number of indices
			/*!< \return a const uint32_t - The number of indices */

		virtual void edit(const void* indices, const uint32_t size, const uint32_t offset) = 0; //!< Edit the index buffer contents
			/*!< \param indices a const void* -The indices info
				 \param size a const uint32_t - The number of indices in the indices data
				 \param offset a const int32_t - The offset of where to place the new data in terms of index count */

		static IndexBuffer* create(const uint32_t* indices, const uint32_t count); //!< Create an index buffer
	};
}
#endif
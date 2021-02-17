/*! \file vertexArray.h
*
* \brief An API agnostic vertex array
*
* \author DMU Course material
*
*/
#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "independent/core/common.h"
#include "independent/rendering/geometry/vertexBuffer.h"
#include "independent/rendering/geometry/indexBuffer.h"

namespace Engine
{
	/*! \class VertexArray
	* \brief An API agnostic vertex array
	*/
	class VertexArray : public Resource
	{
	protected:
		uint32_t m_arrayID; //!< The array ID
		uint32_t m_attribIndex = 0; //!< The number of vertex attributes
		std::vector<VertexBuffer*> m_vertexBuffers; //!< All the vertex buffers in the array
		IndexBuffer* m_indexBuffer; //!< The index buffer for the array
		uint32_t m_totalByteSize; //!< The total size of all the buffers attached to the array
	public:
		static VertexArray* create(const std::string& vertexArrayName); //!< Create a vertex array

		VertexArray(const std::string& vertexArrayName); //!< Constructor
		virtual ~VertexArray(); //!< Destructor

		virtual void addVertexBuffer(VertexBuffer* vertexBuffer) = 0; //!< Add vertex buffer to array
			/*!< \param vertexBuffer a VertexBuffer* - A pointer to a vertex buffer */
		virtual void setIndexBuffer(IndexBuffer* indexBuffer) = 0; //!< Set the index buffer
			/*!< \param vertexBuffer an IndexBuffer* - A pointer to a index buffer */
		inline const uint32_t getDrawCount() const { if (m_indexBuffer) { return m_indexBuffer->getIndicesCount(); } else { return 0; } } //!< Get the indices count
			/*!< \return a const uint32_t - The number of indices */
		inline const uint32_t getID() const { return m_arrayID; } //!< Get the array ID
			/*!< \return a const uint32_t - The ID of the buffer */
		inline std::vector<VertexBuffer*>& getVertexBuffers() { return m_vertexBuffers; } //!< Get all vertex buffers
			/*!< \return a std::vector<VertexBuffer*>& - A reference to all vertex buffers */
		inline IndexBuffer* getIndexBuffer() { return m_indexBuffer; } //!< Get the index buffer
			/*!< \return a IndexBuffer* - A pointer to the index buffer */
		virtual void bind() = 0; //!< Bind the VAO
		virtual void unbind() = 0; //!< Unbind the VAO
		virtual const bool indexBufferBoundToArray() = 0; //!< Check if the index buffer set is correctly bound to the array
	};
}
#endif
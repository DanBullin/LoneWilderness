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
	class VertexArray
	{
	protected:
		uint32_t m_arrayID; //!< The array ID
		uint32_t m_attribIndex = 0; //!< The number of vertex attributes
		std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers; //!< All the vertex buffers in the array
		std::shared_ptr<IndexBuffer> m_indexBuffer; //!< The index buffer for the array
	public:
		virtual ~VertexArray() = default; //!< Destructor
		virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer) = 0; //!< Add vertex buffer to array
			/*!< \param vertexBuffer a const std::shared_ptr<VertexBuffer> - A pointer to a vertex buffer */
		virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer> indexBuffer) = 0; //!< Set the index buffer
			/*!< \param vertexBuffer a const std::shared_ptr<IndexBuffer> - A pointer to a index buffer */
		virtual inline const uint32_t getDrawCount() const { if (m_indexBuffer) { return m_indexBuffer->getIndicesCount(); } else { return 0; } } //!< Get the indices count
			/*!< \return a const uint32_t - The number of indices */
		virtual inline const uint32_t getID() const { return m_arrayID; } //!< Get the array ID
			/*!< \return a const uint32_t - The ID of the buffer */
		virtual inline std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() { return m_vertexBuffers; } //!< Get all vertex buffers
			/*!< \return a std::vector<std::shared_ptr<VertexBuffer>>& - A reference to all vertex buffers */
		virtual inline std::shared_ptr<IndexBuffer>& getIndexBuffer() { return m_indexBuffer; } //!< Get the index buffer
			/*!< \return a std::shared_ptr<IndexBuffer>& - A reference to the index buffer */
		virtual void bind() = 0; //!< Bind the VAO
		virtual void unbind() = 0; //!< Unbind the VAO

		static VertexArray* create(); //!< Create a vertex array
	};
}
#endif
/*! \file vertexBuffer.h
*
* \brief An API agnostic Vertex buffer
*
* \author DMU Course material
*
*/
#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "independent/core/common.h"
#include "independent/systems/components/resource.h"
#include "independent/rendering/geometry/bufferLayout.h"

namespace Engine
{
	/*! \enum VertexBufferUsage
	* \brief An enum class for the expected usage of the data
	*/
	enum class VertexBufferUsage
	{
		None = 0, StaticDraw = 1, DynamicDraw = 2
	};

	/*! \class VertexBuffer
	* \brief An API agnostic Vertex buffer
	*/
	class VertexBuffer : public Resource
	{
	protected:
		uint32_t m_bufferID; //!< The vertex buffer ID
		VertexBufferLayout m_layout; //!< The vertex buffer's layout
		VertexBufferUsage m_usage; //!< The usage of the vertex buffer data
		uint32_t m_byteSize; //!< The size of the vertex buffer in bytes
	public:
		static VertexBuffer* create(const std::string& vertexBufferName, const void* vertices, const uint32_t size, const VertexBufferLayout& layout, const VertexBufferUsage usage = VertexBufferUsage::StaticDraw); //!< Create a vertex buffer
		static VertexBuffer* create(const std::string& vertexBufferName, const uint32_t size, const VertexBufferUsage usage = VertexBufferUsage::DynamicDraw); //!< Create a vertex buffer
		
		VertexBuffer(const std::string& vertexBufferName); //!< Constructor
		virtual ~VertexBuffer(); //!< Destructor

		virtual void edit(const void* vertices, const uint32_t size, const uint32_t offset) = 0; //!< Edit the buffer contents
			/*!< \param vertices a const void* -The vertices info
				 \param size a const uint32_t - The size in bytes of the vertices data
				 \param offset a const int32_t - The offset of where to place the new data */

		virtual void bind() = 0; //!< Bind the buffer
		virtual void unbind() = 0; //!< Unbind the buffer
		virtual void printDetails() override = 0; //!< Print the resource details

		inline const uint32_t getBufferID() const { return m_bufferID; }  //!< Get the buffer ID
			/*!< \return a const uint32_t - The ID of the buffer */
		inline const uint32_t getByteSize() const { return m_byteSize; }  //!< Get the size of this buffer in bytes
			/*!< \return a const uint32_t - The size of the vertex buffer in bytes */
		inline const VertexBufferLayout& getLayout() const { return m_layout; } //!< Get the buffer layout
			/*!< \return a const VertexBufferLayout& - The buffer layout */
		inline const VertexBufferUsage getUsage() const { return m_usage; } //!< Get the usage of the data in the buffer
			/*!< \return a const VertexBufferUsage - The usage of the data in the buffer */
		inline void setLayout(const VertexBufferLayout& layout) { m_layout = layout; } //!< Set the buffer layout
			/*!< \param layout a const VertexBufferLayout& - The vertex buffer layout to set */
	};
}
#endif
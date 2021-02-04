/*! \file renderBuffer.h
*
* \brief A render buffer object
*
* \author Daniel Bullin
*
*/
#ifndef RENDERBUFFER_H
#define RENDERBUFFER_H

#include "independent/core/common.h"

namespace Engine
{
	/*! \enum AttachmentType
	* \brief The attachment types of a framebuffer
	*/
	enum class AttachmentType
	{
		None,
		Colour,
		Depth,
		Stencil,
		DepthAndStencil
	};

	using Attachment = std::pair<AttachmentType, bool>; //!< The attachment type and whether its a sampled attachment

	/*! \class RenderBuffer
	* \brief An API agnostic render buffer object
	*/
	class RenderBuffer
	{
	protected:
		uint32_t m_bufferID; //!< The buffer ID
		AttachmentType m_type; //!< The render buffer attachment type
		glm::ivec2 m_size; //!< The dimensions of the render buffer
	public:
		virtual ~RenderBuffer() = default; //!< Destructor

		virtual inline const uint32_t getID() const { return m_bufferID; } //!< Get the render buffer ID
			/*!< \return a const uint32_t - The render buffer ID */

		virtual inline const AttachmentType getAttachmentType() const { return m_type; } //!< Get the render buffer attachment type
			/*!< \return a const AttachmentType - The attachment type */

		inline glm::ivec2 getSize() const { return m_size; } //!< Get the dimensions of the renderbuffer
			/*!< \return a glm::ivec2 - The dimensions of the renderbuffer */
		
		static RenderBuffer* create(AttachmentType type, const glm::ivec2& size); //!< Create a render buffer
	};
}
#endif
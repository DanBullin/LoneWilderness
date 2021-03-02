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
		Colour16,
		Depth,
		Stencil,
		DepthAndStencil
	};

	namespace BufferAttachments
	{
		//! convertStringToAttachment()
		/*!
		\param attachmentLiteral a const std::string& - The attachment name as a string literal
		\return an AttachmentType - The attachment type
		*/
		static AttachmentType convertStringToAttachment(const std::string& attachmentLiteral)
		{
			if (attachmentLiteral == "Colour") return AttachmentType::Colour;
			else if (attachmentLiteral == "Colour16") return AttachmentType::Colour16;
			else if (attachmentLiteral == "Depth") return AttachmentType::Depth;
			else if (attachmentLiteral == "Stencil") return AttachmentType::Stencil;
			else if (attachmentLiteral == "DepthAndStencil") return AttachmentType::DepthAndStencil;
			else return AttachmentType::None;
		}

		//! toString()
		/*!
		\param attachment a const AttachmentType - The attachment type
		\return a std::string - The attachment type as a string
		*/
		static std::string toString(const AttachmentType attachment)
		{
			switch (attachment)
			{
				case AttachmentType::Colour:
					return "Colour";
				case AttachmentType::Colour16:
					return "Colour16";
				case AttachmentType::Depth:
					return "Depth";
				case AttachmentType::Stencil:
					return "Stencil";
				case AttachmentType::DepthAndStencil:
					return "DepthAndStencil";
				default:
					return "None";
			}
		}
	}

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
		static RenderBuffer* create(AttachmentType type, const glm::ivec2& size); //!< Create a render buffer

		RenderBuffer(); //!< Constructor
		virtual ~RenderBuffer(); //!< Destructor

		inline const uint32_t getID() const { return m_bufferID; } //!< Get the render buffer ID
			/*!< \return a const uint32_t - The render buffer ID */
		inline const AttachmentType getAttachmentType() const { return m_type; } //!< Get the render buffer attachment type
			/*!< \return a const AttachmentType - The attachment type */
		inline glm::ivec2 getSize() const { return m_size; } //!< Get the dimensions of the renderbuffer
			/*!< \return a glm::ivec2 - The dimensions of the renderbuffer */
	};
}
#endif
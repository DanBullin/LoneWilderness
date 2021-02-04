/*! \file frameBuffer.h
*
* \brief A frame buffer object
*
* \author Daniel Bullin
*
*/
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "independent/core/common.h"
#include "independent/rendering/renderBuffer.h"
#include "independent/rendering/textures/texture.h"

namespace Engine
{
	/*! \class FrameBufferLayout
	* \brief An abstraction of a frame buffer layout
	*/
	class FrameBufferLayout
	{
	private:
		std::vector<Attachment> m_attachments; //!< The attachments to the framebuffer
	public:
		FrameBufferLayout() {} //!< Default constructor
		FrameBufferLayout(const std::initializer_list<Attachment>& attachments) : m_attachments(attachments) {} //!< Constructor
			/*!< \param attachments a const std::initializer_list<Attachment>& - The attachments to add to the list of attachments */
		
		inline const std::vector<Attachment>& getAttachments() const { return m_attachments; } //!< Get the attachments in the frame buffer layout
			/*!< \return a const std::vector<Attachment>& - The attachments in the frame buffer layout */

		void addAttachment(const AttachmentType type, const bool sampled = false); //!< Add an attachment to the layout

		inline typename std::vector<Attachment>::iterator begin() { return m_attachments.begin(); } //!< Get the begin iterator for the attachments
			/*!< \return a std::vector<Attachment>::iterator - The begin iterator of the attachments list */
		inline typename std::vector<Attachment>::iterator end() { return m_attachments.end(); } //!< Get the end iterator for the attachments
			/*!< \return a std::vector<Attachment>::iterator - The end iterator of the attachments list */
		inline typename std::vector<Attachment>::const_iterator begin() const { return m_attachments.begin(); } //!< Get the const begin iterator for the attachments
			/*!< \return a std::vector<Attachment>::const_iterator - The begin const iterator of the attachments list */
		inline typename std::vector<Attachment>::const_iterator end() const { return m_attachments.end(); } //!< Get the const end iterator for the attachments
			/*!< \return a std::vector<Attachment>::const_iterator - The end const iterator of the attachments list */
	};

	/*! \class FrameBuffer
	* \brief An API agnostic frame buffer object
	*/
	class FrameBuffer
	{
	protected:
		uint32_t m_bufferID; //!< The frame buffer ID
		glm::ivec2 m_size; //!< The pixel size of the framebuffer
		FrameBufferLayout m_layout; //!< The layout of the framebuffer

		std::map<std::string, Texture2D*> m_sampledTargets; //!< A list of sampled targets
		std::map<std::string, RenderBuffer*> m_nonSampledTargets; //!< A list of non-sampled targets
	public:
		virtual ~FrameBuffer() = default; //!< Destructor

		inline const uint32_t getID() const { return m_bufferID; } //!< Get the frame buffer id
			/*!< \return a const uint32_t - The frame buffer ID */

		inline glm::ivec2 getSize() const { return m_size; } //!< Get the size of the framebuffer
			/*!< \return a glm::ivec2 - The size of the framebuffer */

		inline FrameBufferLayout& getLayout() { return m_layout; } //!< Get the framebuffer layout
			/*!< \return a FrameBufferLayout& - The layout of the framebuffer */

		Texture2D* getSampledTarget(const char* targetName); //!< Get a sampled target from the framebuffer
		RenderBuffer* getNonSampledTarget(const char* targetName); //!< Get a non-sampled target from the framebuffer

		virtual void bind() = 0; //!< Bind the frame buffer
		
		static FrameBuffer* create(const glm::ivec2& size, FrameBufferLayout& layout); //!< Create a frame buffer
		static FrameBuffer* createDefault(); //!< Create a default frame buffer
	};
}
#endif
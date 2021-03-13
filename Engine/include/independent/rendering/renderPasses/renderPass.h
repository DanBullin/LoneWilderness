/*! \file renderPass.h
*
* \brief A render pass constituting the rendering of the scene to a particular target
*
* \author Daniel Bullin
*
*/
#ifndef RENDERPASS_H
#define RENDERPASS_H

#include <vector>
#include "independent/rendering/frameBuffer.h"
#include "independent/entities/entity.h"

namespace Engine
{
	class Scene; //!< Forward declare scene

	/*! \class RenderPass
	* \brief An interface class for a render pass
	*/
	class RenderPass
	{
	protected:
		static bool s_initialised; //!< Has the pass been initialised
		bool m_enabled; //!< Is the render pass enabled
		uint32_t m_index; //!< The index of this pass in the list of passes this pass is connected with
		Scene* m_attachedScene; //!< The scene this pass is attached to
	public:
		RenderPass(); //!< Constructor
		virtual ~RenderPass(); //!< Destructor

		virtual void onAttach() {} //!< Called when the pass is attached to a scene
		virtual void onDetach() {} //!< Called when the pass is detached from a scene

		virtual void onRender(std::vector<Entity*>& entities) = 0; //!< The rendering to perform for this pass
		virtual FrameBuffer* getFrameBuffer() = 0; //!< Get a framebuffer from the render pass

		void attachScene(Scene* scene); //!< Attach the scene to this pass
		void setEnabled(const bool value); //!< Set whether to enable this render pass
		bool getEnabled() const; //!< Get whether to process this render pass

		void setIndex(const uint32_t index); //!< Set the index of this pass
		uint32_t getIndex() const; //!< Get index of this pass
	};
}
#endif
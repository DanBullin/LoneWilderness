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
		Scene* m_attachedScene; //!< The scene this pass is attached to
		FrameBuffer* m_frameBuffer; //!< The framebuffer to render to for this pass
	public:
		RenderPass(); //!< Constructor
		virtual ~RenderPass(); //!< Destructor

		virtual void onRender(std::vector<Entity*>& entities) = 0; //!< The rendering to perform for this pass

		void attachScene(Scene* scene); //!< Attach the scene to this pass
	};
}
#endif
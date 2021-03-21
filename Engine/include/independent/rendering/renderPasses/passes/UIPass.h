/*! \file UIPass.h
*
* \brief An UI render pass
*
* \author Daniel Bullin
*
*/
#ifndef UIPASS_H
#define UIPASS_H

#include "independent/rendering/renderPasses/renderPass.h"

namespace Engine
{
	/*! \class UIPass
	* \brief An UI render pass
	*/
	class UIPass : public RenderPass
	{
	private:
		static bool s_initialised; //!< Has the pass been initialised
		FrameBuffer* m_previousFBO; //!< A framebuffer

		void setupPass(); //!< Set up the pass by setting the settings
		void endPass(); //!< Set the settings to end the pass
	public:
		UIPass(); //!< Constructor
		~UIPass(); //!< Destructor

		void onAttach() override; //!< Called when the pass is attached to a scene
		void onRender(std::vector<Entity*>& entities) override; //!< The rendering to perform for this pass
		FrameBuffer* getFrameBuffer() override; //!< Get the framebuffer of this render pass
	};
}
#endif
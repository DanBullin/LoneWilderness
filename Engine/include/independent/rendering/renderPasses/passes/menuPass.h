/*! \file menuPass.h
*
* \brief The main menu render pass
*
* \author Daniel Bullin
*
*/
#ifndef MENUPASS_H
#define MENUPASS_H

#include "independent/rendering/renderPasses/renderPass.h"

namespace Engine
{
	/*! \class MenuPass
	* \brief A menu render pass
	*/
	class MenuPass : public RenderPass
	{
	private:
		static bool s_initialised; //!< Has the pass been initialised
		FrameBuffer* m_frameBuffer; //!< A framebuffer
		UniformBuffer* m_cameraUBO; //!< The camera UBO

		void setupPass(); //!< Set up the pass by setting the settings
		void endPass(); //!< Set the settings to end the pass
	public:
		MenuPass(); //!< Constructor
		~MenuPass(); //!< Destructor

		void onRender(std::vector<Entity*>& entities) override; //!< The rendering to perform for this pass
		FrameBuffer* getFrameBuffer() override; //!< Get the framebuffer of this render pass
	};
}
#endif
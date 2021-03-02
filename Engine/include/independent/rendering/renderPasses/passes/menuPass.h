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
		FrameBuffer* m_frameBuffer; //!< A framebuffer
	public:
		MenuPass(); //!< Constructor
		~MenuPass(); //!< Destructor

		void prepare(const uint32_t stage) override; //!< A prepare function for a particular stage of the pass
		void onRender(std::vector<Entity*>& entities) override; //!< The rendering to perform for this pass
		FrameBuffer* getFrameBuffer() override; //!< Get the framebuffer of this render pass
	};
}
#endif
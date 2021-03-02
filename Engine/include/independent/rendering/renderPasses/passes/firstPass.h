/*! \file firstPass.h
*
* \brief A first render pass
*
* \author Daniel Bullin
*
*/
#ifndef FIRSTPASS_H
#define FIRSTPASS_H

#include "independent/rendering/renderPasses/renderPass.h"

namespace Engine
{
	/*! \class FirstPass
	* \brief A first render pass
	*/
	class FirstPass : public RenderPass
	{
	private:
		FrameBuffer* m_frameBuffer; //!< A framebuffer
		void uploadLightData(); //!< Upload light data
	public:
		FirstPass(); //!< Constructor
		~FirstPass(); //!< Destructor

		void prepare(const uint32_t stage) override; //!< A prepare function for a particular stage of the pass
		void onRender(std::vector<Entity*>& entities) override; //!< The rendering to perform for this pass
		FrameBuffer* getFrameBuffer() override; //!< Get the framebuffer of this render pass
	};
}
#endif
/*! \file thirdPass.h
*
* \brief A third render pass
*
* \author Daniel Bullin
*
*/
#ifndef THIRDPASS_H
#define THIRDPASS_H

#include "independent/rendering/renderPasses/renderPass.h"

namespace Engine
{
	/*! \class ThirdPass
	* \brief A third render pass
	*/
	class ThirdPass : public RenderPass
	{
	private:
		FrameBuffer* m_frameBuffer; //!< A framebuffer
	public:
		ThirdPass(); //!< Constructor
		~ThirdPass(); //!< Destructor

		void prepare(const uint32_t stage) override; //!< A prepare function for a particular stage of the pass
		void onRender(std::vector<Entity*>& entities) override; //!< The rendering to perform for this pass
		FrameBuffer* getFrameBuffer() override; //!< Get the framebuffer of this render pass
	};
}
#endif
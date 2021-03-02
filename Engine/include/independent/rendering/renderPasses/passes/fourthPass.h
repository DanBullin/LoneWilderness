/*! \file fourthPass.h
*
* \brief A fourth render pass
*
* \author Daniel Bullin
*
*/
#ifndef FOURTHPASS_H
#define FOURTHPASS_H

#include "independent/rendering/renderPasses/renderPass.h"

namespace Engine
{
	/*! \class FourthPass
	* \brief A fourth render pass
	*/
	class FourthPass : public RenderPass
	{
	private:
		FrameBuffer* m_frameBuffer; //!< A framebuffer
	public:
		FourthPass(); //!< Constructor
		~FourthPass(); //!< Destructor

		void prepare(const uint32_t stage) override; //!< A prepare function for a particular stage of the pass
		void onRender(std::vector<Entity*>& entities) override; //!< The rendering to perform for this pass
		FrameBuffer* getFrameBuffer() override; //!< Get the framebuffer of this render pass
	};
}
#endif
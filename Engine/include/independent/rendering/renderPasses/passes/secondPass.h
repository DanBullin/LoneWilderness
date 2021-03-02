/*! \file secondPass.h
*
* \brief A Second render pass
*
* \author Daniel Bullin
*
*/
#ifndef SECONDPASS_H
#define SECONDPASS_H

#include "independent/rendering/renderPasses/renderPass.h"

namespace Engine
{
	/*! \class SecondPass
	* \brief A second render pass
	*/
	class SecondPass : public RenderPass
	{
	private:
		FrameBuffer* m_frameBuffers[2]; //!< A framebuffer
		uint32_t m_horizontal; //!< A variable to determine which axis to blur
	public:
		SecondPass(); //!< Constructor
		~SecondPass(); //!< Destructor

		void prepare(const uint32_t stage) override; //!< A prepare function for a particular stage of the pass
		void onRender(std::vector<Entity*>& entities) override; //!< The rendering to perform for this pass
		FrameBuffer* getFrameBuffer() override; //!< Get the framebuffer of this render pass
	};
}
#endif
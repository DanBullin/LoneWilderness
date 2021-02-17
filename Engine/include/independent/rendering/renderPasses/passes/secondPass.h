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
		FrameBuffer* m_frameBuffer; //!< A framebuffer
	public:
		SecondPass(); //!< Constructor
		~SecondPass(); //!< Destructor

		void onRender(std::vector<Entity*>& entities) override; //!< The rendering to perform for this pass
		FrameBuffer* getFrameBuffer() override; //!< Get the framebuffer of this render pass
	};
}
#endif
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
		static bool s_initialised; //!< Has the pass been initialised
		FrameBuffer* m_frameBuffer; //!< A framebuffer
		UniformBuffer* m_cameraUBO; //!< The camera UBO
		UniformBuffer* m_dirLightUBO; //!< The directional light UBO
		UniformBuffer* m_pointLightUBO; //!< The point lights UBO
		UniformBuffer* m_spotLightUBO; //!< The spot lights UBO UBO

		void uploadLightData(); //!< Upload light data
		void setupPass(); //!< Set up the pass by setting the settings
	public:
		FirstPass(); //!< Constructor
		~FirstPass(); //!< Destructor

		void onRender(std::vector<Entity*>& entities) override; //!< The rendering to perform for this pass
		FrameBuffer* getFrameBuffer() override; //!< Get the framebuffer of this render pass
	};
}
#endif
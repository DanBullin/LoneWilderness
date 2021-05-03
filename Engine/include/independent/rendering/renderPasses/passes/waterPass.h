/*! \file waterPass.h
*
* \brief A water render pass
*
* \author Daniel Bullin
*
*/
#ifndef WATERPASS_H
#define WATERPASS_H

#include "independent/rendering/renderPasses/renderPass.h"

namespace Engine
{
	/*! \class WaterPass
	* \brief A water render pass
	*/
	class WaterPass : public RenderPass
	{
	private:
		static bool s_initialised; //!< Has the pass been initialised
		FrameBuffer* m_reflectionFrameBuffer; //!< A framebuffer
		FrameBuffer* m_refractionFrameBuffer; //!< A framebuffer
		UniformBuffer* m_cameraUBO; //!< The camera UBO
		UniformBuffer* m_clipUBO; //!< The clip UBO
		void setupPass(); //!< Set up the pass by setting the settings
		void setupPass1(); //!< Set up the pass by setting the settings
	public:
		WaterPass(); //!< Constructor
		~WaterPass(); //!< Destructor

		void onRender(std::vector<Entity*>& entities) override; //!< The rendering to perform for this pass
		FrameBuffer* getFrameBuffer() override; //!< Get the framebuffer of this render pass
	};
}
#endif
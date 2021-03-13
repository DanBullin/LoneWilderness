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
		static bool s_initialised; //!< Has the pass been initialised
		FrameBuffer* m_frameBuffer; //!< A framebuffer
		UniformBuffer* m_cameraUBO; //!< The camera UBO
		UniformBuffer* m_bloomUBO; //!< The bloom UBO
		SubTexture* m_subTexture1; //!< The subtexture
		SubTexture* m_subTexture2; //!< The subtexture
		Material* m_bloomMaterial; //!< The bloom material
		void setupPass(); //!< Set up the pass by setting the settings
	public:
		ThirdPass(); //!< Constructor
		~ThirdPass(); //!< Destructor

		void onRender(std::vector<Entity*>& entities) override; //!< The rendering to perform for this pass
		FrameBuffer* getFrameBuffer() override; //!< Get the framebuffer of this render pass
	};
}
#endif
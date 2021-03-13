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
		static bool s_initialised; //!< Has the pass been initialised
		FrameBuffer* m_frameBuffers[2]; //!< A framebuffer
		FrameBuffer* m_previousFBO; //!< The previous framebuffer in the scene list
		UniformBuffer* m_cameraUBO; //!< The camera UBO
		UniformBuffer* m_bloomUBO; //!< The bloom UBO

		Material* m_blurMaterial; //!< The material to use to blur
		SubTexture* m_subTexture; //!< The subtexture to edit

		uint32_t m_horizontal; //!< A variable to determine which axis to blur
		void setupPass(); //!< Set up the pass by setting the settings
	public:
		SecondPass(); //!< Constructor
		~SecondPass(); //!< Destructor

		void onAttach(); //!< Called when the pass is attached to the scene
		void onRender(std::vector<Entity*>& entities) override; //!< The rendering to perform for this pass
		FrameBuffer* getFrameBuffer() override; //!< Get the framebuffer of this render pass
	};
}
#endif
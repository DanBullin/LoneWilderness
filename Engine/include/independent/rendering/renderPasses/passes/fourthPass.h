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
		static bool s_initialised; //!< Has the pass been initialised
		FrameBuffer* m_frameBuffer; //!< A framebuffer
		UniformBuffer* m_cameraUBO; //!< The camera UBO

		Material* m_screenQuadMaterial; //!< The screen quad material
		SubTexture* m_subTexture; //!< The subtexture
		FrameBuffer* m_previousFBO; //!< The previous framebuffer in the scene list
		ShaderProgram* m_screenQuadShader; //!< The shader to render the screen quad

		void setupPass(); //!< Set up the pass by setting the settings
		void endPass(); //!< Set the settings to end the pass
	public:
		FourthPass(); //!< Constructor
		~FourthPass(); //!< Destructor

		void onAttach() override; //!< Called when the pass is attached to a scene
		void onRender(std::vector<Entity*>& entities) override; //!< The rendering to perform for this pass
		FrameBuffer* getFrameBuffer() override; //!< Get the framebuffer of this render pass
	};
}
#endif
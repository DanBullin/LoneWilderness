/*! \file blurPass.h
*
* \brief A blur render pass
*
* \author Daniel Bullin
*
*/
#ifndef BLURPASS_H
#define BLURPASS_H

#include "independent/rendering/renderPasses/renderPass.h"

namespace Engine
{
	/*! \class BlurPass
	* \brief A blur render pass
	*/
	class BlurPass : public RenderPass
	{
	private:
		static bool s_initialised; //!< Has the pass been initialised
		FrameBuffer* m_framebuffer; //!< A framebuffer
		FrameBuffer* m_previousFBO; //!< The previous framebuffer
		SubTexture* m_subTexture; //!< The subtexture
		Material* m_screenQuadMaterial; //!< The screen quad material
		ShaderProgram* m_screenQuadShader; //!< The shader to render the screen quad

		void setupPass(); //!< Set up the pass by setting the settings
		void endPass(); //!< Set the settings to end the pass
	public:
		BlurPass(); //!< Constructor
		~BlurPass(); //!< Destructor

		void onAttach() override; //!< Called when the pass is attached to a scene
		void onRender(std::vector<Entity*>& entities) override; //!< The rendering to perform for this pass
		FrameBuffer* getFrameBuffer() override; //!< Get the framebuffer of this render pass
	};
}
#endif
/*! \file blurPass.cpp
*
* \brief A blur render pass
*
* \author Daniel Bullin
*
*/
#include "independent/rendering/renderPasses/passes/blurPass.h"
#include "independent/rendering/renderUtils.h"
#include "independent/systems/components/scene.h"
#include "independent/rendering/renderers/renderer2D.h"

namespace Engine
{
	bool BlurPass::s_initialised = false; //!< Initialise to false

	//! BlurPass()
	BlurPass::BlurPass()
	{
		m_framebuffer = ResourceManager::getResource<FrameBuffer>("blur3DFBO");
		m_previousFBO = nullptr;
		m_subTexture = ResourceManager::getResource<SubTexture>("screenQuadSubTexture1");
		m_screenQuadMaterial = ResourceManager::getResource<Material>("blur3DMaterial");
		m_screenQuadShader = ResourceManager::getResource<ShaderProgram>("blur3DShader");
		s_initialised = true;
	}

	//! ~BlurPass()
	BlurPass::~BlurPass()
	{
		m_framebuffer = nullptr;
		m_previousFBO = nullptr;
		m_subTexture = nullptr;
		m_screenQuadMaterial = nullptr;
		m_screenQuadShader = nullptr;
		s_initialised = false;
	}

	//! setupPass()
	void BlurPass::setupPass()
	{
		RenderUtils::clearBuffers(RenderParameter::COLOR_AND_DEPTH_BUFFER_BIT, m_attachedScene->getMainCamera()->getClearColour());
		RenderUtils::enableBlending(true);
		RenderUtils::setDepthComparison(RenderParameter::LESS_THAN_OR_EQUAL);
	}

	//! endPass()
	void BlurPass::endPass()
	{
		RenderUtils::enableBlending(false);
	}

	//! onAttach()
	void BlurPass::onAttach()
	{
		if (!m_previousFBO) m_previousFBO = m_attachedScene->getRenderPass(m_index - 1)->getFrameBuffer();
	}

	//! onRender()
	/*!
	\param entities a std::vector<Entity*>& - The list of entities to render
	*/
	void BlurPass::onRender(std::vector<Entity*>& entities)
	{
		// Bind the framebuffer chosen
		m_framebuffer->bind();

		// Set settings
		setupPass();

		Renderer2D::begin();

		m_subTexture->setBaseTexture(m_previousFBO->getSampledTarget("Colour0"), { 0.f, 0.f }, { 1.f, 1.f }, true);
		Renderer2D::submit(m_screenQuadMaterial->getShader(), m_screenQuadMaterial->getSubTextures(), Quad::getScreenQuadMatrix(), m_screenQuadMaterial->getTint());

		Renderer2D::end();

		// Set settings
		endPass();
	}

	//! getFrameBuffer()
	/*!
	\return a FrameBuffer* - The framebuffer
	*/
	FrameBuffer* BlurPass::getFrameBuffer()
	{
		return m_framebuffer;
	}
}
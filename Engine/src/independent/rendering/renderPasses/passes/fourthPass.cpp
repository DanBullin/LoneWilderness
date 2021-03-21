/*! \file fourthPass.cpp
*
* \brief A fourth render pass
*
* \author Daniel Bullin
*
*/
#include "independent/rendering/renderPasses/passes/fourthPass.h"
#include "independent/systems/components/scene.h"
#include "independent/rendering/renderUtils.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/rendering/renderers/renderer2D.h"
#include "independent/rendering/geometry/quad.h"

namespace Engine
{
	bool FourthPass::s_initialised = false; //!< Initialise to false

	//! FourthPass()
	FourthPass::FourthPass()
	{
		m_frameBuffer = ResourceManager::getResource<FrameBuffer>("defaultFBO");
		m_cameraUBO = ResourceManager::getResource<UniformBuffer>("CameraUBO");
		m_subTexture = ResourceManager::getResource<SubTexture>("screenQuadSubTexture1");
		m_screenQuadMaterial = ResourceManager::getResource<Material>("screenMaterial");
		m_previousFBO = nullptr;
		s_initialised = true;
	}

	//! ~FourthPass()
	FourthPass::~FourthPass()
	{
		m_frameBuffer = nullptr;
		m_cameraUBO = nullptr;
		m_subTexture = nullptr;
		m_screenQuadMaterial = nullptr;
		s_initialised = false;
	}

	//! setupPass()
	void FourthPass::setupPass()
	{
		RenderUtils::clearBuffers(RenderParameter::COLOR_AND_DEPTH_BUFFER_BIT, m_attachedScene->getMainCamera()->getClearColour());
		RenderUtils::enableBlending(true);
		RenderUtils::setDepthComparison(RenderParameter::LESS_THAN_OR_EQUAL);

		Camera* cam = m_attachedScene->getMainCamera();
		m_cameraUBO->uploadData("u_view", static_cast<void*>(&cam->getViewMatrix(false)));
		m_cameraUBO->uploadData("u_projection", static_cast<void*>(&cam->getProjectionMatrix(false)));
	}

	//! endPass()
	void FourthPass::endPass()
	{
		RenderUtils::enableBlending(false);
	}

	//! onAttach()
	void FourthPass::onAttach()
	{
		if (!m_previousFBO) m_previousFBO = m_attachedScene->getRenderPass(m_index - 1)->getFrameBuffer();
	}

	//! onRender()
	/*!
	\param entities a std::vector<Entity*>& - The list of entities to render
	*/
	void FourthPass::onRender(std::vector<Entity*>& entities)
	{
		// Bind the framebuffer chosen
		m_frameBuffer->bind();

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
	FrameBuffer* FourthPass::getFrameBuffer()
	{
		return m_frameBuffer;
	}
}
/*! \file secondPass.cpp
*
* \brief A Second render pass. This pass takes the brightness texture from the previous pass and blurs it
*
* \author Daniel Bullin
*
*/
#include "independent/rendering/renderPasses/passes/secondPass.h"
#include "independent/systems/components/scene.h"
#include "independent/rendering/renderUtils.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/rendering/renderers/renderer2D.h"

namespace Engine
{
	bool SecondPass::s_initialised = false; //!< Initialise to false

	//! SecondPass()
	SecondPass::SecondPass()
	{
		m_frameBuffers[0] = ResourceManager::getResource<FrameBuffer>("pingPongFBO1");
		m_frameBuffers[1] = ResourceManager::getResource<FrameBuffer>("pingPongFBO2");
		m_cameraUBO = ResourceManager::getResource<UniformBuffer>("CameraUBO");
		m_bloomUBO = ResourceManager::getResource<UniformBuffer>("BloomUBO");
		m_subTexture = ResourceManager::getResource<SubTexture>("screenQuadSubTexture1");
		m_blurMaterial = ResourceManager::getResource<Material>("BlurMaterial");
		m_previousFBO = nullptr;
		m_horizontal = 1;
		s_initialised = true;
	}

	//! ~SecondPass()
	SecondPass::~SecondPass()
	{
		m_frameBuffers[0] = nullptr;
		m_frameBuffers[1] = nullptr;
		m_cameraUBO = nullptr;
		m_bloomUBO = nullptr;
		m_blurMaterial = nullptr;
		m_previousFBO = nullptr;
		m_subTexture = nullptr;
		s_initialised = false;
	}

	//! setupPass()
	void SecondPass::setupPass()
	{
		// Clear the bloom FBO buffers
		Camera* cam = m_attachedScene->getMainCamera();
		m_cameraUBO->uploadData("u_view", static_cast<void*>(&cam->getViewMatrix(false)));
		m_cameraUBO->uploadData("u_projection", static_cast<void*>(&cam->getProjectionMatrix(false)));
	}

	//! onAttach()
	void SecondPass::onAttach()
	{
		if (!m_previousFBO) m_previousFBO = m_attachedScene->getRenderPass(m_index - 1)->getFrameBuffer();
	}

	//! onRender()
	/*!
	\param entities a std::vector<Entity*>& - The list of entities to render
	*/
	void SecondPass::onRender(std::vector<Entity*>& entities)
	{
		m_horizontal = 1;
		unsigned int amount = ResourceManager::getConfigValue(Config::BloomBlurFactor);

		// Set settings
		setupPass();

		for (unsigned int i = 0; i < amount; i++)
		{
			// Bind the PingPong buffer starting with PingPong2
			m_frameBuffers[m_horizontal]->bind();
			m_bloomUBO->uploadData("u_horizontal", static_cast<void*>(&m_horizontal));

			Renderer2D::begin();

			// If this is the first time, 
			if (i == 0)
				m_subTexture->setBaseTexture(m_previousFBO->getSampledTarget("Colour1"), { 0.f, 0.f }, { 1.f, 1.f }, true);
			else
				m_subTexture->setBaseTexture(m_frameBuffers[!m_horizontal]->getSampledTarget("Colour0"), { 0.f, 0.f }, { 1.f, 1.f }, true);

			Renderer2D::submit(m_blurMaterial->getShader(), m_blurMaterial->getSubTextures(), Quad::getScreenQuadMatrix(), m_blurMaterial->getTint());

			Renderer2D::end();

			m_horizontal = !m_horizontal;
		}

		m_horizontal = 0;
	}

	//! getFrameBuffer()
	/*!
	\return a FrameBuffer* - The framebuffer
	*/
	FrameBuffer* SecondPass::getFrameBuffer()
	{
		return m_frameBuffers[m_horizontal];
	}
}
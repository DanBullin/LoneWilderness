/*! \file secondPass.cpp
*
* \brief A Second render pass
*
* \author Daniel Bullin
*
*/
#include "independent/rendering/renderPasses/passes/secondPass.h"
#include "independent/systems/components/scene.h"
#include "independent/rendering/renderUtils.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/windowManager.h"
#include "independent/rendering/renderers/renderer2D.h"

namespace Engine
{
	//! SecondPass()
	SecondPass::SecondPass()
	{
		m_frameBuffers[0] = ResourceManager::getResource<FrameBuffer>("pingPongFBO1");
		m_frameBuffers[1] = ResourceManager::getResource<FrameBuffer>("pingPongFBO2");
		m_horizontal = 1;
	}

	//! ~SecondPass()
	SecondPass::~SecondPass()
	{
		m_frameBuffers[0] = nullptr;
		m_frameBuffers[1] = nullptr;
	}

	//! prepare()
	/*
	\param stage a const uint32_t - The current stage of the renderer
	*/
	void SecondPass::prepare(const uint32_t stage)
	{
		// Functions to call to prepare before or after rendering calls
		switch (stage)
		{
			case 0:
			{
				RenderUtils::enableDepthTesting(false);

				UniformBuffer* cameraUBO = ResourceManager::getResource<UniformBuffer>("CameraUBO");
				cameraUBO->uploadData("u_view", static_cast<void*>(&m_attachedScene->getMainCamera()->getViewMatrix(false)));
				cameraUBO->uploadData("u_projection", static_cast<void*>(&m_attachedScene->getMainCamera()->getProjectionMatrix(false)));
			}
			case 1:
			{
			}
		}
	}

	//! onRender()
	/*!
	\param entities a std::vector<Entity*>& - The list of entities to render
	*/
	void SecondPass::onRender(std::vector<Entity*>& entities)
	{
		Material* material = ResourceManager::getResource<Material>("bloomBlurMaterial");

		m_frameBuffers[0]->bind();
		RenderUtils::clearBuffers(RenderParameter::COLOR_BUFFER_BIT, m_attachedScene->getMainCamera()->getClearColour());
		m_frameBuffers[1]->bind();
		RenderUtils::clearBuffers(RenderParameter::COLOR_BUFFER_BIT, m_attachedScene->getMainCamera()->getClearColour());

		// Get the Bloom UBO
		UniformBuffer* bloomUBO = ResourceManager::getResource<UniformBuffer>("BloomUBO");

		// Set settings (Camera + Blending)
		prepare(0);

		m_horizontal = 1;
		unsigned int amount = ResourceManager::getConfigValue(Config::BloomBlurFactor);

		for (unsigned int i = 0; i < amount; i++)
		{
			// Bind the PingPong buffer starting with PingPong2
			m_frameBuffers[m_horizontal]->bind();
			bloomUBO->uploadData("u_horizontal", static_cast<void*>(&m_horizontal));

			Renderer2D::begin(nullptr);

			// If this is the first time, 
			if (i == 0)
				material->getSubTexture(0)->setBaseTexture(m_attachedScene->getRenderPass(m_index - 1)->getFrameBuffer()->getSampledTarget("Colour1"),
					{ 0.f, 0.f }, { 1.f, 1.f }, true);
			else
				material->getSubTexture(0)->setBaseTexture(m_frameBuffers[!m_horizontal]->getSampledTarget("Colour0"),
					{ 0.f, 0.f }, { 1.f, 1.f }, true);

			Renderer2D::submit(ResourceManager::getResource<ShaderProgram>("blurBright"),
				material->getSubTextures(),
				getScreenQuad(),
				material->getTint());

			Renderer2D::end();

			m_horizontal = !m_horizontal;
		}

		m_horizontal = 0;
		prepare(1);
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
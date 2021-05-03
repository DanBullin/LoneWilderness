/*! \file UIPass.cpp
*
* \brief An UI render pass
*
* \author Daniel Bullin
*
*/
#include "independent/rendering/renderPasses/passes/UIPass.h"
#include "independent/rendering/renderUtils.h"
#include "independent/systems/components/scene.h"
#include "independent/rendering/renderers/renderer2D.h"

namespace Engine
{
	bool UIPass::s_initialised = false; //!< Initialise to false

	//! UIPass()
	UIPass::UIPass()
	{
		m_previousFBO = nullptr;
		s_initialised = true;
	}

	//! ~UIPass()
	UIPass::~UIPass()
	{
		m_previousFBO = nullptr;
		s_initialised = false;
	}

	//! setupPass()
	void UIPass::setupPass()
	{
		if (m_previousFBO->getName() == "defaultFBO")
		{
			RenderUtils::clearBuffers(RenderParameter::COLOR_AND_DEPTH_BUFFER_BIT, m_attachedScene->getMainCamera()->getClearColour());
			RenderUtils::setDepthComparison(RenderParameter::LESS_THAN_OR_EQUAL);
			RenderUtils::enableDepthTesting(true);

			Camera* cam = m_attachedScene->getMainCamera();
			ResourceManager::getResource<UniformBuffer>("CameraUBO")->uploadData("u_view", static_cast<void*>(&cam->getViewMatrix(false)));
			ResourceManager::getResource<UniformBuffer>("CameraUBO")->uploadData("u_projection", static_cast<void*>(&cam->getProjectionMatrix(false)));
		}

		RenderUtils::enableBlending(true);

		Camera* cam = m_attachedScene->getMainCamera();
		ResourceManager::getResource<UniformBuffer>("CameraUBO")->uploadData("u_view", static_cast<void*>(&cam->getViewMatrix(false)));
		ResourceManager::getResource<UniformBuffer>("CameraUBO")->uploadData("u_projection", static_cast<void*>(&cam->getProjectionMatrix(false)));
	}

	//! endPass()
	void UIPass::endPass()
	{
		RenderUtils::enableBlending(false);
	}

	//! onAttach()
	void UIPass::onAttach()
	{
		if (!m_previousFBO)
		{
			if (m_index != 0)
				m_previousFBO = m_attachedScene->getRenderPass(m_index - 2)->getFrameBuffer();
			else
				m_previousFBO = ResourceManager::getResource<FrameBuffer>("defaultFBO");
		}
	}

	//! onRender()
	/*!
	\param entities a std::vector<Entity*>& - The list of entities to render
	*/
	void UIPass::onRender(std::vector<Entity*>& entities)
	{
		// Bind the framebuffer chosen
		m_previousFBO->bind();

		// Set settings
		setupPass();

		Renderer2D::begin();

		for (auto& entity : entities)
		{
			if (entity->getLayer()->getDisplayed() && entity->getDisplay())
			{
				if (entity->containsComponent<MeshRender2D>())
				{
					entity->getComponent<MeshRender2D>()->onRender();
				}
				if (entity->containsComponent<Text>())
				{
					entity->getComponent<Text>()->onRender();
				}
				if (entity->containsComponent<NativeScript>())
					entity->getComponent<NativeScript>()->onRender(Renderers::Renderer2D, "Default");
			}
		}

		Renderer2D::end();

		// Set settings
		endPass();
	}

	//! getFrameBuffer()
	/*!
	\return a FrameBuffer* - The framebuffer
	*/
	FrameBuffer* UIPass::getFrameBuffer()
	{
		return m_previousFBO;
	}
}
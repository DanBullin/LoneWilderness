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
		RenderUtils::enableBlending(true);
	}

	//! endPass()
	void UIPass::endPass()
	{
		RenderUtils::enableBlending(false);
	}

	//! onAttach()
	void UIPass::onAttach()
	{
		if (!m_previousFBO) m_previousFBO = m_attachedScene->getRenderPass(m_index - 1)->getFrameBuffer();
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
					entity->getComponent<NativeScript>()->onRender(Renderers::Renderer2D);
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
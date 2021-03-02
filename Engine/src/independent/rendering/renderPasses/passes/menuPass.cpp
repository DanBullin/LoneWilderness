/*! \file menuPass.cpp
*
* \brief The main menu render pass
*
* \author Daniel Bullin
*
*/
#include "independent/rendering/renderPasses/passes/menuPass.h"
#include "independent/systems/components/scene.h"
#include "independent/rendering/renderUtils.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/windowManager.h"
#include "independent/rendering/renderers/renderer3D.h"
#include "independent/rendering/renderers/renderer2D.h"

namespace Engine
{
	//! MenuPass()
	MenuPass::MenuPass()
	{
		m_frameBuffer = ResourceManager::getResource<FrameBuffer>("defaultFBO");
	}

	//! ~MenuPass()
	MenuPass::~MenuPass()
	{
		m_frameBuffer = nullptr;
	}

	//! prepare()
	/*
	\param stage a const uint32_t - The current stage of the renderer
	*/
	void MenuPass::prepare(const uint32_t stage)
	{
		// Functions to call to prepare before or after rendering calls
		switch (stage)
		{
			case 0:
			{
				RenderUtils::clearBuffers(RenderParameter::COLOR_AND_DEPTH_BUFFER_BIT, m_attachedScene->getMainCamera()->getClearColour());

				RenderUtils::enableBlending(true);

				UniformBuffer* cameraUBO = ResourceManager::getResource<UniformBuffer>("CameraUBO");
				cameraUBO->uploadData("u_view", static_cast<void*>(&m_attachedScene->getMainCamera()->getViewMatrix(false)));
				cameraUBO->uploadData("u_projection", static_cast<void*>(&m_attachedScene->getMainCamera()->getProjectionMatrix(false)));
				break;
			}
			case 1:
			{
				RenderUtils::enableBlending(false);
				break;
			}
		}
	}

	//! onRender()
	/*!
	\param entities a std::vector<Entity*>& - The list of entities to render
	*/
	void MenuPass::onRender(std::vector<Entity*>& entities)
	{
		m_frameBuffer->bind();

		prepare(0);

		Renderer2D::begin(nullptr);

		for (auto& entity : entities)
			if (entity->getLayer()->getDisplayed() && entity->getDisplay()) entity->onRender(Renderers::Renderer2D);

		Renderer2D::end();

		prepare(1);
	}

	//! getFrameBuffer()
	/*!
	\return a FrameBuffer* - The framebuffer
	*/
	FrameBuffer* MenuPass::getFrameBuffer()
	{
		return m_frameBuffer;
	}
}
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
#include "independent/rendering/renderers/renderer2D.h"

namespace Engine
{
	bool MenuPass::s_initialised = false; //!< Initialise to false

	//! MenuPass()
	MenuPass::MenuPass()
	{
		m_frameBuffer = ResourceManager::getResource<FrameBuffer>("defaultFBO");
		m_cameraUBO = ResourceManager::getResource<UniformBuffer>("CameraUBO");
		s_initialised = true;
	}

	//! ~MenuPass()
	MenuPass::~MenuPass()
	{
		m_frameBuffer = nullptr;
		m_cameraUBO = nullptr;
		s_initialised = false;
	}

	//! setupPass()
	void MenuPass::setupPass()
	{
		RenderUtils::clearBuffers(RenderParameter::COLOR_AND_DEPTH_BUFFER_BIT, m_attachedScene->getMainCamera()->getClearColour());
		RenderUtils::enableBlending(true);
		RenderUtils::enableDepthTesting(true);
		RenderUtils::setDepthComparison(RenderParameter::LESS_THAN_OR_EQUAL);

		Camera* cam = m_attachedScene->getMainCamera();
		m_cameraUBO->uploadData("u_view", static_cast<void*>(&cam->getViewMatrix(false)));
		m_cameraUBO->uploadData("u_projection", static_cast<void*>(&cam->getProjectionMatrix(false)));
	}

	//! endPass()
	void MenuPass::endPass()
	{
		RenderUtils::enableBlending(false);
		RenderUtils::setDepthComparison(RenderParameter::LESS);
	}

	//! onRender()
	/*!
	\param entities a std::vector<Entity*>& - The list of entities to render
	*/
	void MenuPass::onRender(std::vector<Entity*>& entities)
	{
		m_frameBuffer->bind();

		setupPass();

		Renderer2D::begin();

		for (auto& entity : entities)
		{
			if (entity->getLayer()->getDisplayed() && entity->getDisplay())
			{
				if (entity->containsComponent<MeshRender2D>())
					entity->getComponent<MeshRender2D>()->onRender();

				if (entity->containsComponent<Text>())
					entity->getComponent<Text>()->onRender();

				if (entity->containsComponent<NativeScript>())
					entity->getComponent<NativeScript>()->onRender(Renderers::Renderer2D);
			}
		}

		Renderer2D::end();

		endPass();
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
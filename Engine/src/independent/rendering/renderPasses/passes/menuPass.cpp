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
		m_frameBuffer = ResourceManager::getResourceAndRef<FrameBuffer>("frameBuffer1");
	}

	//! ~MenuPass()
	MenuPass::~MenuPass()
	{
		m_frameBuffer->decreaseCount();
		m_frameBuffer = nullptr;
	}

	//! onRender()
	/*!
	\param entities a std::vector<Entity*>& - The list of entities to render
	*/
	void MenuPass::onRender(std::vector<Entity*>& entities)
	{
		/////
		// 2D Rendering
		//

		m_frameBuffer->bind();

		RenderUtils::clearBuffers(RenderParameter::COLOR_AND_DEPTH_BUFFER_BIT, m_attachedScene->getMainCamera()->getClearColour());

		RenderUtils::enableBlending(true);

		UniformBuffer* cameraUBO = ResourceManager::getResource<UniformBuffer>("CameraUBO");
		cameraUBO->uploadData("u_view", static_cast<void*>(&m_attachedScene->getMainCamera()->getViewMatrix(false)));
		cameraUBO->uploadData("u_projection", static_cast<void*>(&m_attachedScene->getMainCamera()->getProjectionMatrix(false)));

		Renderer2D::begin(nullptr);

		for (auto& entity : entities)
		{
			if (entity->getDisplay())
				entity->onRender(Renderers::Renderer2D);
		}

		Renderer2D::end();

		RenderUtils::enableBlending(false);
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
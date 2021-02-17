/*! \file firstPass.cpp
*
* \brief A first render pass
*
* \author Daniel Bullin
*
*/
#include "independent/rendering/renderPasses/passes/firstPass.h"
#include "independent/systems/components/scene.h"
#include "independent/rendering/renderUtils.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/windowManager.h"
#include "independent/rendering/renderers/renderer3D.h"
#include "independent/rendering/renderers/renderer2D.h"

namespace Engine
{
	//! FirstPass()
	FirstPass::FirstPass()
	{
		m_frameBuffer = ResourceManager::getResourceAndRef<FrameBuffer>("frameBuffer2");
	}

	//! ~FirstPass()
	FirstPass::~FirstPass()
	{
		m_frameBuffer->decreaseCount();
		m_frameBuffer = nullptr;
	}

	//! onRender()
	/*!
	\param entities a std::vector<Entity*>& - The list of entities to render
	*/
	void FirstPass::onRender(std::vector<Entity*>& entities)
	{
		/////
		// 3D Rendering
		//

		m_frameBuffer->bind();

		RenderUtils::clearBuffers(RenderParameter::COLOR_AND_DEPTH_BUFFER_BIT, m_attachedScene->getMainCamera()->getClearColour());

		RenderUtils::enableDepthTesting(true);
		RenderUtils::setDepthComparison(RenderParameter::LESS_THAN_OR_EQUAL);

		UniformBuffer* cameraUBO = ResourceManager::getResource<UniformBuffer>("CameraUBO");
		cameraUBO->uploadData("u_view", static_cast<void*>(&m_attachedScene->getMainCamera()->getViewMatrix(true)));
		cameraUBO->uploadData("u_projection", static_cast<void*>(&m_attachedScene->getMainCamera()->getProjectionMatrix(true)));

		Renderer3D::begin();

		for (auto& entity : entities)
		{
			if (entity->getDisplay())
				entity->onRender(Renderers::Renderer3D);
		}

		if (m_attachedScene->getMainCamera()->getSkybox()) m_attachedScene->getMainCamera()->getSkybox()->onRender();

		Renderer3D::end();

		RenderUtils::setDepthComparison(RenderParameter::LESS);

		RenderUtils::enableBlending(true);

		cameraUBO = ResourceManager::getResource<UniformBuffer>("CameraUBO");
		cameraUBO->uploadData("u_view", static_cast<void*>(&m_attachedScene->getMainCamera()->getViewMatrix(false)));
		cameraUBO->uploadData("u_projection", static_cast<void*>(&m_attachedScene->getMainCamera()->getProjectionMatrix(false)));

		Renderer2D::begin(nullptr);

		for (auto& entity : entities)
		{
			if (entity->getDisplay() && entity->getName() != "ScreenQuad")
				entity->onRender(Renderers::Renderer2D);
		}

		Renderer2D::end();

		RenderUtils::enableBlending(false);
	}

	//! getFrameBuffer()
	/*!
	\return a FrameBuffer* - The framebuffer
	*/
	FrameBuffer* FirstPass::getFrameBuffer()
	{
		return m_frameBuffer;
	}
}
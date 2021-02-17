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
#include "independent/systems/systems/windowManager.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/rendering/renderers/renderer2D.h"

namespace Engine
{
	//! SecondPass()
	SecondPass::SecondPass()
	{
		m_frameBuffer = ResourceManager::getResourceAndRef<FrameBuffer>("frameBuffer1");
	}

	//! ~SecondPass()
	SecondPass::~SecondPass()
	{
		m_frameBuffer->decreaseCount();
		m_frameBuffer = nullptr;
	}

	//! onRender()
	/*!
	\param entities a std::vector<Entity*>& - The list of entities to render
	*/
	void SecondPass::onRender(std::vector<Entity*>& entities)
	{
		/////
		// 2D Rendering
		//
		SceneManager::getScene("scene1")->getEntity("ScreenQuad")->getComponent<MeshRender2D>()->getMaterial()->getSubTexture(0)->setBaseTexture(m_attachedScene->getRenderPasses().front()->getFrameBuffer()->getSampledTarget("Colour0"));

		m_frameBuffer->bind();

		RenderUtils::clearBuffers(RenderParameter::COLOR_AND_DEPTH_BUFFER_BIT, m_attachedScene->getMainCamera()->getClearColour());

		RenderUtils::enableBlending(true);

		UniformBuffer* cameraUBO = ResourceManager::getResource<UniformBuffer>("CameraUBO");
		cameraUBO->uploadData("u_view", static_cast<void*>(&m_attachedScene->getMainCamera()->getViewMatrix(false)));
		cameraUBO->uploadData("u_projection", static_cast<void*>(&m_attachedScene->getMainCamera()->getProjectionMatrix(false)));

		Renderer2D::begin(nullptr);
		SceneManager::getActiveScene()->getEntity("ScreenQuad")->onRender(Renderers::Renderer2D);
		Renderer2D::end();

		RenderUtils::enableBlending(false);
	}

	//! getFrameBuffer()
	/*!
	\return a FrameBuffer* - The framebuffer
	*/
	FrameBuffer* SecondPass::getFrameBuffer()
	{
		return m_frameBuffer;
	}
}
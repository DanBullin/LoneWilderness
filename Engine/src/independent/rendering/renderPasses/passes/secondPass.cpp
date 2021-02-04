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
#include "independent/rendering/renderers/renderer2D.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/sceneManager.h"

namespace Engine
{
	//! SecondPass()
	SecondPass::SecondPass()
	{
		m_frameBuffer = ResourceManager::getFrameBuffer("Default").get();

		if(SceneManager::getActiveScene()->getEntity("ScreenQuadd"))
			SceneManager::getActiveScene()->getEntity("ScreenQuadd")->getComponent<MeshRender2D>()->getMaterial()->getSubTexture(0)->setBaseTexture(ResourceManager::getFrameBuffer("ScreenQuad")->getSampledTarget("Colour0"));
	}

	//! ~SecondPass()
	SecondPass::~SecondPass()
	{
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

		m_frameBuffer->bind();

		RenderUtils::clearBuffers(RenderParameters::COLOR_BUFFER_BIT, m_attachedScene->getClearColour());

		RenderUtils::enableBlending(true);
		RenderUtils::enableDepthTesting(false);
		RenderUtils::enableWireframe(false);

		Shared<UniformBuffer> cameraUBO = ResourceManager::getUniformBuffer("Camera");
		cameraUBO->uploadData("u_view", static_cast<void*>(&m_attachedScene->getMainCamera()->getViewMatrix(false)));
		cameraUBO->uploadData("u_projection", static_cast<void*>(&m_attachedScene->getMainCamera()->getProjectionMatrix(false)));

		if (SceneManager::getActiveScene()->getEntity("ScreenQuadd"))
		{
			Renderer2D::begin(ResourceManager::getShader("quad").get(), { {"Camera", ResourceManager::getUniformBuffer("Camera")} });

			for (auto& entity : entities)
			{
				if (entity->getName() == "ScreenQuadd")
					entity->onRender(Renderers::Renderer2D);
			}

			Renderer2D::end();
		}

		Renderer2D::begin(ResourceManager::getShader("quad").get(), { {"Camera", ResourceManager::getUniformBuffer("Camera")} });

		for (auto& entity : entities)
		{
			if (entity->getName() != "ScreenQuadd")
				entity->onRender(Renderers::Renderer2D);
		}

		Renderer2D::end();

		RenderUtils::enableBlending(false);
	}
}
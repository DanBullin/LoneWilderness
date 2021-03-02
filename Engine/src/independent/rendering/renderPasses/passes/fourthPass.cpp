/*! \file fourthPass.cpp
*
* \brief A fourth render pass
*
* \author Daniel Bullin
*
*/
#include "independent/rendering/renderPasses/passes/fourthPass.h"
#include "independent/systems/components/scene.h"
#include "independent/rendering/renderUtils.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/windowManager.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/rendering/renderers/renderer2D.h"
#include "independent/rendering/geometry/quad.h"

namespace Engine
{
	//! FourthPass()
	FourthPass::FourthPass()
	{
		m_frameBuffer = nullptr;
	}

	//! ~FourthPass()
	FourthPass::~FourthPass()
	{
		m_frameBuffer = nullptr;
	}

	//! prepare()
	/*
	\param stage a const uint32_t - The current stage of the renderer
	*/
	void FourthPass::prepare(const uint32_t stage)
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
	void FourthPass::onRender(std::vector<Entity*>& entities)
	{
		// Get the screen quad's mesh render
		Material* material = ResourceManager::getResource<Material>("screenMaterial");

		m_frameBuffer = ResourceManager::getResource<FrameBuffer>("defaultFBO");

		// Bind the framebuffer chosen
		m_frameBuffer->bind();

		// Set settings
		prepare(0);

		Renderer2D::begin(nullptr);

		material->getSubTexture(0)->setBaseTexture(m_attachedScene->getRenderPass(m_index - 1)->getFrameBuffer()->getSampledTarget("Colour0"),
			{ 0.f, 0.f }, { 1.f, 1.f }, true);

		// The entire scene has been rendered to a texture, so we simply render that quad to the framebuffer bound
		// screenMaterial is just a simple quad material with a single texture
		// Use Colour0 of the previous pass
		Renderer2D::submit(ResourceManager::getResource<ShaderProgram>("sceneQuadPlain"),
			ResourceManager::getResource<Material>("screenMaterial")->getSubTextures(),
			getScreenQuad(),
			ResourceManager::getResource<Material>("screenMaterial")->getTint());

		Renderer2D::end();

		// Set settings
		prepare(1);
	}

	//! getFrameBuffer()
	/*!
	\return a FrameBuffer* - The framebuffer
	*/
	FrameBuffer* FourthPass::getFrameBuffer()
	{
		return m_frameBuffer;
	}
}
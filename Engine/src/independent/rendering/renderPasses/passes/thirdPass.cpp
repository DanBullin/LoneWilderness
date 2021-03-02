/*! \file thirdPass.cpp
*
* \brief A third render pass
*
* \author Daniel Bullin
*
*/
#include "independent/rendering/renderPasses/passes/thirdPass.h"
#include "independent/systems/components/scene.h"
#include "independent/rendering/renderUtils.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/windowManager.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/rendering/renderers/renderer3D.h"
#include "independent/rendering/renderers/renderer2D.h"

namespace Engine
{
	//! ThirdPass()
	ThirdPass::ThirdPass()
	{
		// Bloom FBO is the FBO we use to take the contents of the HDR buffer and the blurred brightness buffer
		m_frameBuffer = ResourceManager::getResource<FrameBuffer>("bloomFBO");
	}

	//! ~ThirdPass()
	ThirdPass::~ThirdPass()
	{
		m_frameBuffer = nullptr;
	}

	//! prepare()
	/*
	\param stage a const uint32_t - The current stage of the renderer
	*/
	void ThirdPass::prepare(const uint32_t stage)
	{
		// Functions to call to prepare before or after rendering calls
		switch (stage)
		{
			case 0:
			{
				// Clear the bloom FBO buffers
				RenderUtils::clearBuffers(RenderParameter::COLOR_AND_DEPTH_BUFFER_BIT, m_attachedScene->getMainCamera()->getClearColour());
				RenderUtils::enableBlending(true);

				// Upload 2D camera values
				UniformBuffer* cameraUBO = ResourceManager::getResource<UniformBuffer>("CameraUBO");
				cameraUBO->uploadData("u_view", static_cast<void*>(&m_attachedScene->getMainCamera()->getViewMatrix(false)));
				cameraUBO->uploadData("u_projection", static_cast<void*>(&m_attachedScene->getMainCamera()->getProjectionMatrix(false)));

				// Upload bloom bool variable
				uint32_t useBloom = ResourceManager::getConfigValue(Config::UseBloom);
				UniformBuffer* bloomUBO = ResourceManager::getResource<UniformBuffer>("BloomUBO");
				bloomUBO->uploadData("u_enableBloom", static_cast<void*>(&useBloom));

				break;
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
	void ThirdPass::onRender(std::vector<Entity*>& entities)
	{
		// Get the screen quad's mesh render
		Material* material = ResourceManager::getResource<Material>("bloomMaterial");

		// Bind the bloom fbo (Blur)
		m_frameBuffer->bind();
		
		// Set settings
		prepare(0);

		Renderer2D::begin(nullptr);

		// Now we render a quad with the bloom material
		// We need to set the first subtexture to the HDR colour 0 buffer
		// We need to set the second subtexture to the blurred bright texture from the previous pass's buffer
		material->getSubTexture(0)->setBaseTexture(m_attachedScene->getRenderPass(0)->getFrameBuffer()->getSampledTarget("Colour0"),
			{ 0.f, 0.f }, { 1.f, 1.f }, true);
		material->getSubTexture(1)->setBaseTexture(m_attachedScene->getRenderPass(m_index-1)->getFrameBuffer()->getSampledTarget("Colour0"),
			{ 0.f, 0.f }, { 1.f, 1.f }, true);
		
		Renderer2D::submit(ResourceManager::getResource<ShaderProgram>("sceneQuad"),
			material->getSubTextures(),
			getScreenQuad(),
			material->getTint());

		for (auto& entity : entities)
		{
			if (entity->getLayer()->getDisplayed() && entity->getDisplay())
				entity->onRender(Renderers::Renderer2D);
		}

		Renderer2D::end();

		prepare(1);
	}

	//! getFrameBuffer()
	/*!
	\return a FrameBuffer* - The framebuffer
	*/
	FrameBuffer* ThirdPass::getFrameBuffer()
	{
		return m_frameBuffer;
	}
}
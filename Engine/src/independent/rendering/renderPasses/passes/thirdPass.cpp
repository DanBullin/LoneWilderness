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
	bool ThirdPass::s_initialised = false; //!< Initialise to false

	//! ThirdPass()
	ThirdPass::ThirdPass()
	{
		m_frameBuffer = ResourceManager::getResource<FrameBuffer>("applyBloomFBO");
		m_cameraUBO = ResourceManager::getResource<UniformBuffer>("CameraUBO");
		m_bloomUBO = ResourceManager::getResource<UniformBuffer>("BloomUBO");

		m_subTexture1 = ResourceManager::getResource<SubTexture>("screenQuadSubTexture1");
		m_subTexture2 = ResourceManager::getResource<SubTexture>("screenQuadSubTexture2");

		if (!s_initialised)
		{
			ShaderProgram* newShader = ShaderProgram::create("bloomShader");
			newShader->build(ResourceManager::getResource<VertexArray>("QuadMultiTexturedArray"), "assets/shaders/bloomShader/vertex.vs", "assets/shaders/bloomShader/fragment.fs", "", "", "");
			newShader->setUniforms({ "u_textures" });
			newShader->setUniformBuffers({ { "Camera", m_cameraUBO }, { "Bloom", m_bloomUBO } });
			newShader->setOrderImportance(0);
			ResourceManager::registerResource("bloomShader", newShader);

			Material* bloomMaterial = new Material("bloomMaterial", { m_subTexture1, m_subTexture2 }, {}, newShader, { 1.f, 1.f, 1.f, 1.f }, 32.f);
			ResourceManager::registerResource("bloomMaterial", bloomMaterial);
			m_bloomMaterial = bloomMaterial;
			s_initialised = true;
		}

	}

	//! ~ThirdPass()
	ThirdPass::~ThirdPass()
	{
		m_frameBuffer = nullptr;
		m_cameraUBO = nullptr;
		m_bloomUBO = nullptr;
		m_subTexture1 = nullptr;
		m_subTexture2 = nullptr;
		m_bloomMaterial = nullptr;
		s_initialised = false;
	}

	//! setupPass()
	void ThirdPass::setupPass()
	{
		// Clear the bloom FBO buffers
		RenderUtils::clearBuffers(RenderParameter::COLOR_AND_DEPTH_BUFFER_BIT, m_attachedScene->getMainCamera()->getClearColour());
		RenderUtils::enableBlending(true);
		RenderUtils::setDepthComparison(RenderParameter::LESS_THAN_OR_EQUAL);

		// Upload 2D camera values
		Camera* cam = m_attachedScene->getMainCamera();
		m_cameraUBO->uploadData("u_view", static_cast<void*>(&cam->getViewMatrix(false)));
		m_cameraUBO->uploadData("u_projection", static_cast<void*>(&cam->getProjectionMatrix(false)));

		// Upload bloom bool variable
		uint32_t useBloom = ResourceManager::getConfigValue(Config::UseBloom);
		m_bloomUBO->uploadData("u_enableBloom", static_cast<void*>(&useBloom));
	}

	//! onRender()
	/*!
	\param entities a std::vector<Entity*>& - The list of entities to render
	*/
	void ThirdPass::onRender(std::vector<Entity*>& entities)
	{
		// Bind the bloom fbo (Blur)
		m_frameBuffer->bind();

		// Set settings
		setupPass();

		Renderer2D::begin();

		m_subTexture1->setBaseTexture(m_attachedScene->getRenderPass(0)->getFrameBuffer()->getSampledTarget("Colour0"), { 0.f, 0.f }, { 1.f, 1.f }, true);
		m_subTexture2->setBaseTexture(m_attachedScene->getRenderPass(m_index - 1)->getFrameBuffer()->getSampledTarget("Colour0"), { 0.f, 0.f }, { 1.f, 1.f }, true);
		Renderer2D::submit(m_bloomMaterial->getShader(), m_bloomMaterial->getSubTextures(), Quad::getScreenQuadMatrix(), m_bloomMaterial->getTint());

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
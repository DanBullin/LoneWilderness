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
		m_bloomMaterial = ResourceManager::getResource<Material>("bloomMaterial");;
		s_initialised = true;
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
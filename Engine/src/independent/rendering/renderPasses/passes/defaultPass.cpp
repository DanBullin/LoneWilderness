/*! \file defaultPass.cpp
*
* \brief A default render pass
*
* \author Daniel Bullin
*
*/

#include "independent/rendering/renderPasses/passes/defaultPass.h"
#include "independent/systems/components/scene.h"
#include "independent/rendering/renderUtils.h"
#include "independent/rendering/renderers/renderer3D.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/windowManager.h"

namespace Engine
{
	//! DefaultPass()
	DefaultPass::DefaultPass()
	{
		Shared<FrameBuffer> fbo;
		FrameBufferLayout layout = { { AttachmentType::Colour, true }, { AttachmentType::DepthAndStencil, false } };
		fbo.reset(FrameBuffer::create(WindowManager::getFocusedWindow()->getProperties().getSize(), layout));
		ResourceManager::registerFrameBuffer("ScreenQuad", fbo);

		m_frameBuffer = ResourceManager::getFrameBuffer("ScreenQuad").get();
	}

	//! ~DefaultPass()
	DefaultPass::~DefaultPass()
	{
	}

	//! onRender()
	/*!
	\param entities a std::vector<Entity*>& - The list of entities to render
	*/
	void DefaultPass::onRender(std::vector<Entity*>& entities)
	{
		m_frameBuffer->bind();

		RenderUtils::clearBuffers(RenderParameters::COLOR_AND_DEPTH_BUFFER_BIT, m_attachedScene->getClearColour());
		RenderUtils::enableDepthTesting(true);
		RenderUtils::enableFaceCulling(true);
		//RenderUtils::enableWireframe(true);

		/////
		// 3D Rendering
		//
		Shared<UniformBuffer> cameraUBO = ResourceManager::getUniformBuffer("Camera");

		cameraUBO->uploadData("u_view", static_cast<void*>(&m_attachedScene->getMainCamera()->getViewMatrix(true)));
		cameraUBO->uploadData("u_projection", static_cast<void*>(&m_attachedScene->getMainCamera()->getProjectionMatrix(true)));

		Renderer3D::begin(nullptr, { {"Camera", ResourceManager::getUniformBuffer("Camera")} });

		for (auto& entity : entities)
			entity->onRender(Renderers::Renderer3D);

		Renderer3D::end();
	}
}
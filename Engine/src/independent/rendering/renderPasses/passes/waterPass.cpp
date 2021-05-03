/*! \file waterPass.cpp
*
* \brief A water render pass
*
* \author Daniel Bullin
*
*/
#include "independent/rendering/renderPasses/passes/waterPass.h"
#include "independent/rendering/renderUtils.h"
#include "independent/systems/components/scene.h"
#include "independent/rendering/renderers/renderer3D.h"

namespace Engine
{
	bool WaterPass::s_initialised = false; //!< Initialise to false

	void WaterPass::setupPass()
	{
		glm::vec4 reflecPlane = { 0.f, 1.f, 0.f, -20.f };
		float reflectMode = 2.f;

		RenderUtils::clearBuffers(RenderParameter::COLOR_AND_DEPTH_BUFFER_BIT, m_attachedScene->getMainCamera()->getClearColour());
		RenderUtils::setDepthComparison(RenderParameter::LESS_THAN_OR_EQUAL);

		m_clipUBO->uploadData("u_plane", &reflecPlane);
		m_clipUBO->uploadData("u_mode", &reflectMode);
		RenderUtils::enableClipDistance(true);

		Camera* cam = m_attachedScene->getMainCamera();
		m_cameraUBO->uploadData("u_view", static_cast<void*>(&cam->getViewMatrix(true)));
		m_cameraUBO->uploadData("u_projection", static_cast<void*>(&cam->getProjectionMatrix(true)));
	}

	void WaterPass::setupPass1()
	{
		glm::vec4 refracPlane = { 0.f, -1.f, 0.f, 20.f };
		float refractMode = 1.f;

		RenderUtils::clearBuffers(RenderParameter::COLOR_AND_DEPTH_BUFFER_BIT, m_attachedScene->getMainCamera()->getClearColour());
		RenderUtils::setDepthComparison(RenderParameter::LESS_THAN_OR_EQUAL);

		m_clipUBO->uploadData("u_plane", &refracPlane);
		m_clipUBO->uploadData("u_mode", &refractMode);
		RenderUtils::enableClipDistance(true);

		Camera* cam = m_attachedScene->getMainCamera();
		m_cameraUBO->uploadData("u_view", static_cast<void*>(&cam->getViewMatrix(true)));
		m_cameraUBO->uploadData("u_projection", static_cast<void*>(&cam->getProjectionMatrix(true)));
		m_cameraUBO->uploadData("u_viewPos", static_cast<void*>(&cam->getWorldPosition()));
	}

	WaterPass::WaterPass()
	{
		m_reflectionFrameBuffer = ResourceManager::getResource<FrameBuffer>("reflectionFBO");
		m_refractionFrameBuffer = ResourceManager::getResource<FrameBuffer>("refractionFBO");
		m_clipUBO = ResourceManager::getResource<UniformBuffer>("ClipUBO");
		m_cameraUBO = ResourceManager::getResource<UniformBuffer>("CameraUBO");
		s_initialised = true;
	}

	WaterPass::~WaterPass()
	{
		m_reflectionFrameBuffer = nullptr;
		m_refractionFrameBuffer = nullptr;
		m_cameraUBO = nullptr;
		m_clipUBO = nullptr;
		s_initialised = false;
	}

	void WaterPass::onRender(std::vector<Entity*>& entities)
	{
		float normalMode = 0.f;

		// Bind FBO
		m_reflectionFrameBuffer->bind();
		
		glm::vec3 pos = m_attachedScene->getMainCamera()->getParent()->getParentEntity()->getComponent<Transform>()->getWorldPosition();
		float distance = 2 * (pos.y - 20.f);
		pos.y -= distance;
		m_attachedScene->getMainCamera()->getParent()->getParentEntity()->getComponent<Transform>()->setLocalPosition(pos);
		m_attachedScene->getMainCamera()->inversePitch();
		setupPass();
		
		RenderUtils::enablePatchDrawing(true);

		Renderer3D::begin();
		for (auto& entity : entities)
		{
			if (entity->getLayer()->getDisplayed() && entity->getDisplay())
			{
				if (entity->containsComponent<NativeScript>())
					entity->getComponent<NativeScript>()->onRender(Renderers::Renderer3D, "Terrain");
			}
		}
		Renderer3D::end();

		RenderUtils::enablePatchDrawing(false);
		RenderUtils::enableWireframe(false);
		RenderUtils::enableFaceCulling(false);
		Renderer3D::begin();
		// Go through each 3D object (including light source objects) + skybox and render them to HDR buffer + brightness texture
		for (auto& entity : entities)
		{
			if (entity->getLayer()->getDisplayed() && entity->getDisplay())
			{
				if (entity->getName() == "Water1") continue;

				if (entity->containsComponent<MeshRender3D>())
					entity->getComponent<MeshRender3D>()->onRender();

				if (entity->containsComponent<NativeScript>())
					entity->getComponent<NativeScript>()->onRender(Renderers::Renderer3D, "Default");
			}
		}

		Skybox* skybox = m_attachedScene->getMainCamera()->getSkybox();
		if (skybox)
			skybox->onRender();

		Renderer3D::end();

		pos.y += distance;
		m_attachedScene->getMainCamera()->getParent()->getParentEntity()->getComponent<Transform>()->setLocalPosition(pos);
		m_attachedScene->getMainCamera()->inversePitch();

		// Bind FBO
		m_refractionFrameBuffer->bind();
		setupPass1();

		RenderUtils::enablePatchDrawing(true);

		Renderer3D::begin();
		for (auto& entity : entities)
		{
			if (entity->getLayer()->getDisplayed() && entity->getDisplay())
			{
				if (entity->containsComponent<NativeScript>())
					entity->getComponent<NativeScript>()->onRender(Renderers::Renderer3D, "Terrain");
			}
		}
		Renderer3D::end();

		RenderUtils::enablePatchDrawing(false);
		RenderUtils::enableWireframe(false);
		RenderUtils::enableFaceCulling(false);
		Renderer3D::begin();
		// Go through each 3D object (including light source objects) + skybox and render them to HDR buffer + brightness texture
		for (auto& entity : entities)
		{
			if (entity->getLayer()->getDisplayed() && entity->getDisplay())
			{
				if (entity->getName() == "Water1") continue;

				if (entity->containsComponent<MeshRender3D>())
					entity->getComponent<MeshRender3D>()->onRender();

				if (entity->containsComponent<NativeScript>())
					entity->getComponent<NativeScript>()->onRender(Renderers::Renderer3D, "Default");
			}
		}

		skybox = m_attachedScene->getMainCamera()->getSkybox();
		if (skybox)
			skybox->onRender();

		Renderer3D::end();

		RenderUtils::enableClipDistance(false);
		m_clipUBO->uploadData("u_mode", &normalMode);
	}

	FrameBuffer * WaterPass::getFrameBuffer()
	{
		return m_reflectionFrameBuffer;
	}
}
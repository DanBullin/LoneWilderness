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
#include "independent/systems/systemManager.h"
#include "independent/systems/systems/windowManager.h"
#include "independent/rendering/renderers/renderer3D.h"
#include "independent/rendering/renderers/renderer2D.h"

namespace Engine
{
	//! uploadLightData()
	void FirstPass::uploadLightData()
	{
		std::vector<DirectionalLight*> dirLights = m_attachedScene->getClosestDirectionalLights();
		std::vector<PointLight*> pointLights = m_attachedScene->getClosestPointLights();
		std::vector<SpotLight*> spotLights = m_attachedScene->getClosestSpotLights();

		/////////
		// DIRECTIONAL LIGHTING
		/////////

		UniformBuffer* lightUBO = ResourceManager::getResource<UniformBuffer>("DirLightUBO");
		std::string name = "DirLight";
		DirectionalLightSDT dirLightSDT;
		if (dirLights.size() != 0)
		{
			dirLightSDT.direction = glm::vec4(dirLights[0]->getDirection(), 0.f);
			dirLightSDT.ambient = glm::vec4(dirLights[0]->getAmbientFactor(), 0.f);
			dirLightSDT.diffuse = glm::vec4(dirLights[0]->getDiffuseFactor(), 0.f);
			dirLightSDT.specular = glm::vec4(dirLights[0]->getSpecularFactor(), 0.f);
		}
		else
		{
			dirLightSDT.direction = glm::vec4(0.f, 0.f, 0.f, 0.f);
			dirLightSDT.ambient = glm::vec4(0.f, 0.f, 0.f, 0.f);
			dirLightSDT.diffuse = glm::vec4(0.f, 0.f, 0.f, 0.f);
			dirLightSDT.specular = glm::vec4(0.f, 0.f, 0.f, 0.f);
		}
		lightUBO->uploadData(name.c_str(), static_cast<void*>(&dirLightSDT));

		/////////
		// POINT LIGHTING
		/////////

		lightUBO = ResourceManager::getResource<UniformBuffer>("PointLightUBO");
		for (uint32_t i = 0; i < ResourceManager::getConfigValue(Config::MaxLightsPerDraw); i++)
		{
			PointLightSDT lightSDT;
			std::string name = "PointLight" + std::to_string(i);

			if (pointLights.size() > i)
			{
				lightSDT.position = glm::vec4(pointLights[i]->getWorldPosition(), 0.f);
				lightSDT.ambient = glm::vec4(pointLights[i]->getAmbientFactor(), 0.f);
				lightSDT.diffuse = glm::vec4(pointLights[i]->getDiffuseFactor(), 0.f);
				lightSDT.specular = glm::vec4(pointLights[i]->getSpecularFactor(), 0.f);
				lightSDT.constant = pointLights[i]->getConstant();
				lightSDT.linear = pointLights[i]->getLinear();
				lightSDT.quadratic = pointLights[i]->getQuadratic();
			}
			else
			{
				lightSDT.position = glm::vec4(0.f, 0.f, 0.f, 0.f);
				lightSDT.ambient = glm::vec4(0.f, 0.f, 0.f, 0.f);
				lightSDT.diffuse = glm::vec4(0.f, 0.f, 0.f, 0.f);
				lightSDT.specular = glm::vec4(0.f, 0.f, 0.f, 0.f);
				lightSDT.constant = 0.f;
				lightSDT.linear = 0.f;
				lightSDT.quadratic = 0.f;
			}
			lightUBO->uploadData(name.c_str(), static_cast<void*>(&lightSDT));
		}

		/////////
		// SPOT LIGHTING
		/////////

		lightUBO = ResourceManager::getResource<UniformBuffer>("SpotLightUBO");
		for (uint32_t i = 0; i < ResourceManager::getConfigValue(Config::MaxLightsPerDraw); i++)
		{
			SpotLightSDT lightSDT;
			std::string name = "SpotLight" + std::to_string(i);

			if (spotLights.size() > i)
			{
				lightSDT.position = glm::vec4(spotLights[i]->getWorldPosition(), 0.f);
				lightSDT.direction = glm::vec4(spotLights[i]->getDirection(), 0.f);
				lightSDT.ambient = glm::vec4(spotLights[i]->getAmbientFactor(), 0.f);
				lightSDT.diffuse = glm::vec4(spotLights[i]->getDiffuseFactor(), 0.f);
				lightSDT.specular = glm::vec4(spotLights[i]->getSpecularFactor(), 0.f);
				lightSDT.constant = spotLights[i]->getConstant();
				lightSDT.linear = spotLights[i]->getLinear();
				lightSDT.quadratic = spotLights[i]->getQuadratic();
				lightSDT.cutOff = spotLights[i]->getCutOff();
				lightSDT.outerCutOff = spotLights[i]->getOuterCutOff();
			}
			else
			{
				lightSDT.position = glm::vec4(0.f, 0.f, 0.f, 0.f);
				lightSDT.direction = glm::vec4(0.f, 0.f, 0.f, 0.f);
				lightSDT.ambient = glm::vec4(0.f, 0.f, 0.f, 0.f);
				lightSDT.diffuse = glm::vec4(0.f, 0.f, 0.f, 0.f);
				lightSDT.specular = glm::vec4(0.f, 0.f, 0.f, 0.f);
				lightSDT.constant = 0.f;
				lightSDT.linear = 0.f;
				lightSDT.quadratic = 0.f;
				lightSDT.cutOff = 0.f;
				lightSDT.outerCutOff = 0.f;
			}
			lightUBO->uploadData(name.c_str(), static_cast<void*>(&lightSDT));
		}

	}

	//! FirstPass()
	FirstPass::FirstPass()
	{
		m_frameBuffer = ResourceManager::getResource<FrameBuffer>("hdrFBO");
	}

	//! ~FirstPass()
	FirstPass::~FirstPass()
	{
		m_frameBuffer = nullptr;
	}

	//! prepare()
	/*
	\param stage a const uint32_t - The current stage of the renderer
	*/
	void FirstPass::prepare(const uint32_t stage)
	{
		// Functions to call to prepare before or after rendering calls
		switch (stage)
		{
			case 0:
			{
				// Clear both colour buffers in the HDR FBO
				RenderUtils::clearBuffers(RenderParameter::COLOR_AND_DEPTH_BUFFER_BIT, m_attachedScene->getMainCamera()->getClearColour());

				// Set some rendering settings
				RenderUtils::setDepthComparison(RenderParameter::LESS_THAN_OR_EQUAL);
				RenderUtils::enableDepthTesting(true);

				// Upload light data to the light UBO
				uploadLightData();

				// Upload camera perspective data to UBO
				UniformBuffer* cameraUBO = ResourceManager::getResource<UniformBuffer>("CameraUBO");
				cameraUBO->uploadData("u_view", static_cast<void*>(&m_attachedScene->getMainCamera()->getViewMatrix(true)));
				cameraUBO->uploadData("u_projection", static_cast<void*>(&m_attachedScene->getMainCamera()->getProjectionMatrix(true)));
				cameraUBO->uploadData("u_viewPos", static_cast<void*>(&m_attachedScene->getMainCamera()->getWorldPosition()));
				break;
			}
			case 1:
			{
				// Set depth comparison back to default
				RenderUtils::setDepthComparison(RenderParameter::LESS);
			}
		}
	}

	//! onRender()
	/*!
	\param entities a std::vector<Entity*>& - The list of entities to render
	*/
	void FirstPass::onRender(std::vector<Entity*>& entities)
	{
		// Bind the HDR FBO
		m_frameBuffer->bind();

		// Set settings
		prepare(0);

		Renderer3D::begin();

		// Go through each 3D object (including light source objects) + skybox and render them
		for (auto& entity : entities)
			if(entity->getLayer()->getDisplayed() && entity->getDisplay()) entity->onRender(Renderers::Renderer3D);

		if (m_attachedScene->getMainCamera()->getSkybox())
			m_attachedScene->getMainCamera()->getSkybox()->onRender();

		Renderer3D::end();

		// Set settings
		prepare(1);

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
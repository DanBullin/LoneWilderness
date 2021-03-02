/*! \file renderSystem.cpp
*
* \brief A render system which manages rendering (both 2D & 3D)
*
* \author Daniel Bullin
*
*/
#include "independent/systems/systems/renderSystem.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/rendering/renderUtils.h"

namespace Engine
{
	bool RenderSystem::s_enabled = false; //!< Is this system enabled
	bool RenderSystem::s_renderersInitialised = false; //!< Set to false
	TextureUnitManager* RenderSystem::s_unitManager = nullptr; //!< The texture unit manager
	std::array<int32_t, 16> RenderSystem::s_unit = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }; //!< The texture units

	//! RenderSystem
	RenderSystem::RenderSystem() : System(SystemType::RenderSystem)
	{
	}

	//! ~RenderSystem
	RenderSystem::~RenderSystem()
	{
	}

	//! start()
	void RenderSystem::start()
	{
		// Start system if its disabled
		if (!s_enabled)
		{
			ENGINE_INFO("[RenderSystem::start] Starting the render system.");
			s_unitManager = new TextureUnitManager(16, 0);
			s_renderersInitialised = false;
			s_enabled = true;

			RenderSystem::initialise();
		}
	}

	//! stop()
	void RenderSystem::stop()
	{
		// Stop system if its enabled
		if (s_enabled)
		{
			ENGINE_INFO("[RenderSystem::stop] Stopping the render system.");
			Renderer3D::destroy();
			Renderer2D::destroy();

			if(s_unitManager) delete s_unitManager;
			s_unitManager = nullptr;
			s_renderersInitialised = false;
			s_enabled = false;
		}
	}

	//! initialise()
	void RenderSystem::initialise()
	{
		if (!s_renderersInitialised && s_enabled)
		{
			// Initialise the 2D and 3D renderers with their capacity values
			Renderer2D::initialise(ResourceManager::getConfigValue(Config::BatchCapacity2D));
			Renderer3D::initialise(ResourceManager::getConfigValue(Config::BatchCapacity3D), ResourceManager::getConfigValue(Config::VertexCapacity3D), ResourceManager::getConfigValue(Config::IndexCapacity3D));

			// Send both renderers the central texture unit manager and units
			Renderer2D::setTextureUnitManager(s_unitManager, s_unit);
			Renderer3D::setTextureUnitManager(s_unitManager, s_unit);
			s_renderersInitialised = true;
		}
	}

	//! onRender()
	/*!
	\param scene a Scene* - A pointer to the scene to render
	*/
	void RenderSystem::onRender(Scene* scene)
	{
		// Get a list of all entities in the scene
		std::vector<Entity*> entityList = scene->getEntities();

		// Get a list of all the render passes for this scene
		std::vector<RenderPass*> renderPassList = scene->getRenderPasses();

		// Now we have all the render passes for this scene and a list of all entities
		// Go through each render pass in the order in which they were added and render
		for (auto& renderPass : renderPassList)
		{
			if(renderPass->getEnabled())
				renderPass->onRender(entityList);
		}
	}
}
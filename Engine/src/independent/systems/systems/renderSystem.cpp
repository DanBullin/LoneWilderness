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

#define BATCH3DCAPACITY 50000
#define VERTEX3DCAPACITY 1000000
#define INDICIES3DCAPACITY 1000000
#define BATCH2DCAPACITY 100

namespace Engine
{
	bool RenderSystem::s_enabled = false; //!< Is this system enabled
	std::shared_ptr<TextureUnitManager> RenderSystem::s_unitManager = nullptr; //!< The texture unit manager
	std::array<int32_t, 16> RenderSystem::s_unit = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }; //!< The texture units

	//! RenderSystem
	RenderSystem::RenderSystem() : System(Systems::Type::RenderSystem)
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
			s_unitManager.reset(new TextureUnitManager(16, 0));
			s_enabled = true;
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
			s_enabled = false;
		}
	}

	//! initialise()
	void RenderSystem::initialise()
	{
		// Initialise the 2D and 3D renderers with their capacity values
		Renderer3D::initialise(BATCH3DCAPACITY, VERTEX3DCAPACITY, INDICIES3DCAPACITY);
		Renderer2D::initialise(BATCH2DCAPACITY);

		// Send both renderers the central texture unit manager and units
		Renderer3D::setTextureUnitManager(s_unitManager, s_unit);
		Renderer2D::setTextureUnitManager(s_unitManager, s_unit);
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The timestep
	\param totalTime a const float - The total time of the application
	*/
	void RenderSystem::onUpdate(const float timestep, const float totalTime)
	{
	}

	//! onRender()
	/*!
	\param scene a Scene* - A pointer to the scene to render
	*/
	void RenderSystem::onRender(Scene* scene)
	{
		// A static list of all renderable entities
		static std::vector<Entity*> entityList; // List to be populated

		// Check if entity list has changed since last frame
		// This will therefore only update the entity list if it has changed
		if (scene->getNewEntitiesFlag())
		{
			// Scene's entities has changed, go full nuclear and clear list and reassemble
			entityList.clear();

			auto layers = scene->getLayerManager()->getLayers();

			for (auto& layer : layers)
			{
				// Go through each layer, if its displayed, insert the layer's entities into our temp list
				if (layer->getDisplayed())
				{
					// Only get the entities that have a renderable component
					auto layerList = layer->getRenderableEntities();
					entityList.insert(entityList.end(), layerList.begin(), layerList.end());
				}
			}
		}

		// We now have a list of all entities to be rendered in this frame (2D and 3D) in entityList

		auto passList = scene->getRenderPasses();

		// Now we have all the render passes for this scene and a list of all entities

		// Go through each render pass in the order in which they were added and render
		for (auto& pass : passList)
			pass->onRender(entityList);
	}
}
/*! \file scene.h
*
* \brief A scene contains all the data associated in a particular environment. This data will be loaded by the SceneLoader.
*
* \author Daniel Bullin
*
*/
#ifndef SCENE_H
#define SCENE_H

#include "independent/core/common.h"
#include "independent/core/destroyable.h"
#include "independent/entities/entity.h"
#include "independent/layers/layerManager.h"
#include "independent/rendering/renderPasses/renderPass.h"

namespace Engine
{
	/*! \class Scene
	* \brief A scene class which contains all entities and world data
	*/
	class Scene : public Destroyable
	{
	private:
		std::string m_sceneName; //!< The name of the scene
		std::string m_sceneFolderPath; //!< The filepath to the scene folder
		LayerManager* m_layerManager; //!< A layer manager for the scene
		std::vector<RenderPass*> m_renderPasses; //!< A list of all render passes for the scene
		std::map<std::string, Entity*> m_rootEntities; //!< List of all root entities in the scene
		Camera* m_mainCamera; //!< The current main camera

		bool m_entityListUpdated; //!< Has the entity list been updated
		std::vector<Entity*> m_entitiesList; //!< The list of entities in vector format
	public:
		Scene(const std::string& name, const std::string& sceneFolderPath); //!< Constructor
		~Scene(); //!< Destructor

		const std::string& getName(); //!< Get the name of the scene
		const std::string& getFolderPath(); //!< Get the file path to the scene folder

		void onUpdate(const float timestep, const float totalTime); //!< Update the scene

		void addEntity(const std::string& name, Entity* entity); //!< Add an entity to the scene
		Entity* getEntity(const std::string& name); //!< Get an entity in the scene
		std::vector<Entity*> getEntities(); //!< Get a list of all entities in the scene

		std::vector<PointLight*> getClosestPointLights(); //!< Get a list of the closest point lights
		std::vector<SpotLight*> getClosestSpotLights(); //!< Get a list of the closest spot lights
		std::vector<DirectionalLight*> getClosestDirectionalLights(); //!< Get a list of the closest directional lights

		bool checkRootEntityNameTaken(const std::string& name); //!< Returns whether the entity name has been taken

		void setMainCamera(Camera* camera); //!< Set the main camera
		Camera* getMainCamera() const; //!< Get the main camera

		LayerManager* getLayerManager(); //!< Get the layer manager

		void addRenderPass(RenderPass* pass); //!< Add a render pass to the list of passes
		std::vector<RenderPass*>& getRenderPasses(); //!< Get the list of render passes
		RenderPass* getRenderPass(const uint32_t index); //!< Get the render pass at index
		FrameBuffer* getFinalFrameBuffer(); //!< Get the final framebuffer in the list of passes for this scene

		void setEntityListUpdated(const bool value); //!< Confirm whether the entities in the scene have been updated
		const bool getEntityListUpdated(); //!< Get whether the entities have been updated

		void printSceneDetails(); //!< Print scene details
	};
}
#endif
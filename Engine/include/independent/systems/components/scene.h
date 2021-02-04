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
#include "independent/entities/entity.h"
#include "independent/layers/layerManager.h"
#include "independent/rendering/renderPasses/renderPass.h"

namespace Engine
{
	/*! \class Scene
	* \brief A scene class which contains all entities and world data
	*/
	class Scene
	{
	private:
		std::string m_sceneName; //!< The name of the scene
		LayerManager* m_layerManager; //!< A layer manager for the scene
		std::vector<RenderPass*> m_renderPasses; //!< A list of all render passes for the scene
		std::map<std::string, Entity*> m_entities; //!< List of all entities in the scene

		Camera* m_mainCamera; //!< The current main camera
		glm::vec4 m_clearColour; //!< The clear colour when rendering

		bool m_newEntities; //!< Have we added new entities since last frame (Used for rendering)
		bool m_delete; //!< Delete this scene at the end of the frame
	public:
		Scene(const char* name); //!< Constructor
		~Scene(); //!< Destructor

		void destroy(); //!< Destroy the scene

		const std::string& getName(); //!< Get the name of the scene

		const glm::vec4& getClearColour(); //!< Get the clear colour
		void setClearColour(const glm::vec4& colour); //!< Set the clear colour

		void onUpdate(const float timestep, const float totalTime); //!< Update the scene

		void addEntity(const char* name, Entity* entity); //!< Add an entity to the scene
		void deleteEntity(const char* name); //!< Delete an entity from the scene
		Entity* getEntity(const char* name); //!< Get an entity in the scene
		std::map<std::string, Entity*>& getRootEntities(); //!< Get a list of all root entities in the scene
		bool checkEntityNameTaken(const char* name); //!< Returns whether the entity name has been taken

		void setMainCamera(Camera* camera); //!< Set the main camera
		Camera* getMainCamera() const; //!< Get the main camera

		LayerManager* getLayerManager(); //!< Get the layer manager

		void addRenderPass(RenderPass* pass); //!< Add a render pass to the list of passes
		std::vector<RenderPass*>& getRenderPasses(); //!< Get the list of render passes

		void setDelete(const bool deletion); //!< Set the schedule scene deletion
		const bool getDelete() const; //!< Get the schedule scene deletion

		void setNewEntitiesFlag(const bool value); //!< Set the new entities bool flag
		const bool getNewEntitiesFlag() const; //!< Get the new entities bool flag

		void printEntityDetails(); //!< Print all entity details
		void printSceneDetails(); //!< Print scene details
	};
}
#endif
/*! \file sceneLoader.h
*
* \brief A scene loader class which loads the scene from a JSON file.
*
* \author Daniel Bullin
*
*/
#ifndef SCENELOADER_H
#define SCENELOADER_H

#include <string>
#include <json.hpp>

namespace Engine
{
	using json = nlohmann::json; //!< Type alias

	class Scene; //!< Forward declare scene
	class NativeScript; //!< Forward declare nativescript
	class Entity; //!< Forward declare entity
	class Layer; //!< Forward declare layer
	class RenderPass; //!< Forward declare render pass

	/*! \class SceneLoader
	* \brief A loader class which loads the scene object by creating all the objects from files
	*/
	class SceneLoader
	{
	private:
		static Layer* createLayer(const std::string& layerName); //!< Create a new layer by converting string layer name literal
		static RenderPass* createRenderPass(const std::string& passName); //!< Create a new render pass by converting string pass name literal
		static bool loadSceneProperties(Scene* scene, json sceneData); //!< Load the scenes properties/core data
		static bool loadEntityProperties(Entity* entity, json entityData); //!< Load the entity's properties/core data
		static void loadEntity(Scene* scene, Entity* parent, json entityData); //!< Load an entity
		static void loadComponents(Scene* scene, Entity* entity, json componentData); //!< Load components
	public:
		static NativeScript* createNewScript(const std::string& scriptName, nlohmann::json scriptData = nlohmann::json()); //!< Create a new script
		static void load(const std::string& sceneName, const std::string& sceneFolderPath); //!< Load the scene
		static void loadEntities(Scene* scene, const std::string& entityFilePath); //!< Load entities into the scene
	};
}

#endif
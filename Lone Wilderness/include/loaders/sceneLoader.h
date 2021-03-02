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
	class Entity; //!< Forward declare entity
	class Layer; //!< Forward declare layer
	class RenderPass; //!< Forward declare render pass

	/*! \class SceneLoader
	* \brief A loader class which loads the scene object by creating all the objects from files
	*/
	class SceneLoader
	{
	private:
		static Entity* createNewEntity(const std::string& entitySubType); //!< Create a new entity subtype
		static Layer* createLayer(const std::string& layerName); //!< Create a new layer by converting string layer name literal
		static RenderPass* createRenderPass(const std::string& passName); //!< Create a new render pass by converting string pass name literal
		static bool loadSceneProperties(Scene* scene, json sceneData); //!< Load the scenes properties/core data
		static bool loadEntityProperties(Entity* entity, json entityData); //!< Load the entity's properties/core data
	public:
		static void load(const std::string& sceneName, const std::string& sceneFolderPath); //!< Load the scene
		static void save(const std::string& sceneName, const std::string& sceneFolderPath); //!< Save the scene
	};
}

#endif
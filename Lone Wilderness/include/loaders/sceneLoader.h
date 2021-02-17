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

namespace Engine
{
	class Entity; //!< Forward declare entity
	class Layer; //!< Forward declare layer
	class RenderPass; //!< Forward declare render pass

	/*! \class SceneLoader
	* \brief A loader class which loads the scene object by creating all the objects from files
	*/
	class SceneLoader
	{
	private:
		static void loadResources(const std::string& resourceFolderPath); //!< Load the scene resources
		static Entity* createNewEntity(const std::string& entitySubType); //!< Create a new entity subtype
		static Layer* createLayer(const std::string& layerName); //!< Create a new layer by converting string layer name literal
		static RenderPass* createRenderPass(const std::string& passName); //!< Create a new render pass by converting string pass name literal
	public:
		static void load(const std::string& sceneName, const std::string& sceneFolderPath); //!< Load the scene
	};
}

#endif
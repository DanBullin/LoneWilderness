/*! \file resourceManager.h
*
* \brief A resource manager containing all the resources that are currently loaded. All resources are stored in a single map containing a key which is a unique string.
*
* \author Daniel Bullin
*
*/
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <json.hpp>
#include "independent/systems/system.h"
#include "independent/core/common.h"
#include "independent/systems/components/resource.h"

#include "independent/rendering/geometry/vertexBuffer.h"
#include "independent/rendering/geometry/indexBuffer.h"
#include "independent/rendering/geometry/vertexArray.h"
#include "independent/rendering/geometry/indirectBuffer.h"
#include "independent/rendering/uniformBuffer.h"
#include "independent/rendering/frameBuffer.h"
#include "independent/rendering/shaders/shaderProgram.h"
#include "independent/rendering/textures/texture.h"
#include "independent/rendering/textures/subTexture.h"
#include "independent/rendering/geometry/model3D.h"
#include "independent/rendering/materials/material.h"

namespace Engine
{
	/*! \enum ConfigData
	* \brief The different types of configurable data
	*/
	enum class ConfigData
	{
		MaxSubTexturesPerMaterial, VertexCapacity3D, IndexCapacity3D, BatchCapacity3D, BatchCapacity2D, MaxLayersPerScene, MaxRenderPassesPerScene, MaxComponentsInstancePerEntity
	};

	/*! \class ResourceManager
	* \brief A resource manager managing resources
	*/
	class ResourceManager : public System
	{
	private:
		static bool s_enabled; //!< Is this system enabled
		static std::map<std::string, Resource*> s_loadedResources; //!< A list of loaded resources

		static uint32_t s_maxSubTexturesPerMaterial; //!< Maximum subtextures allowed per material
		static uint32_t s_vertexCapacity3D; //!< Maximum number of vertices in a 3D vertex buffer
		static uint32_t s_indexCapacity3D; //!< Maximum number of indices in an 3D index buffer
		static uint32_t s_batchCapacity3D; //!< Maximum number of submissions in a 3D Batch
		static uint32_t s_batchCapacity2D; //!< Maximum number of quads in a 2D Batch
		static uint32_t s_maxRenderPassesPerScene; //!< Maximum number of render passes per scene
		static uint32_t s_maxLayersPerScene; //!< Maximum number of layers per scene
		static uint32_t s_maxComponentsInstancePerEntity; //!< Maximum number of instances of a type of component per entity
	public:
		ResourceManager(); //!< Constructor
		~ResourceManager(); //!< Destructor
		void start() override; //!< Start the system
		void stop() override; //!< Stop the system

		static const bool resourceExists(const std::string& resourceName); //!< Check if the resource name exists

		static void registerResource(const std::string& resourceName, Resource* resource); //!< Register a resource
		static void destroyResource(const std::string& resourceName = ""); //!< Destroy a resource by name or all of them

		template<typename T> static T* getResource(const std::string& resourceName); //!< Get a resource by name
		template<typename T> static T* getResourceAndRef(const std::string& resourceName); //!< Get a resource by name and increase reference counter
		static std::map<std::string, Resource*>& getResources(); //!< Get a list of all resources

		static std::string getCurrentDirectory(); //!< Returns the current directory
		static std::string getContents(const std::string& filePath); //!< Return the file contents
		static nlohmann::json getJSON(const std::string& filePath); //!< Load the contents of a file into a JSON object and return the object

		static const uint32_t getConfigValue(const ConfigData data); //!< Get a config value

		static void printResourceManagerDetails(); //!< Print resource manager details
		static void printResourceDetails(); //!< Print resource details
	};

	template<typename T>
	//! getResource()
	/*!
	\param resourceName a const std::string& - The name of the resource
	*/
	static T* ResourceManager::getResource(const std::string& resourceName)
	{
		// Check if resource exists by name
		// Cast and return it if it does, otherwise return nullptr
		// No type checking, quite frankly its your fault if you get it wrong
		if (resourceExists(resourceName))
			return static_cast<T*>(s_loadedResources[resourceName]);
		else
			return nullptr;
	}

	template<typename T>
	//! getResourceAndRef()
	/*!
	\param resourceName a const std::string& - The name of the resource
	*/
	static T* ResourceManager::getResourceAndRef(const std::string& resourceName)
	{
		// Get resource and increase reference counter
		T* res = getResource<T>(resourceName);

		// If the resource is valid (was found), now increase the reference counter and return it
		// It is up to the developer to ensure this returned resource is handled properly in order for the decrease counter to function properly
		if (res)
			static_cast<Resource*>(res)->increaseCount();

		return res;
	}
}

#endif
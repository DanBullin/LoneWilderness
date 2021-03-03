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
	namespace Config
	{
		/*! \enum ConfigData
		* \brief The different types of configurable data
		*/
		enum ConfigData
		{
			MaxSubTexturesPerMaterial = 0, VertexCapacity3D = 1, IndexCapacity3D = 2, BatchCapacity3D = 3, BatchCapacity2D = 4,
			MaxLayersPerScene = 5, MaxRenderPassesPerScene = 6, MaxLightsPerDraw = 7, UseBloom = 8, BloomBlurFactor = 9, PrintResourcesInDestructor = 10,
			PrintOpenGLDebugMessages = 11
		};
	}

	/*! \class ResourceManager
	* \brief A resource manager managing resources
	*/
	class ResourceManager : public System
	{
	private:
		static bool s_enabled; //!< Is this system enabled
		static std::map<std::string, Resource*> s_loadedResources; //!< A list of loaded resources
		static std::vector<uint32_t> s_configValues; //!< The config values
	public:
		ResourceManager(); //!< Constructor
		~ResourceManager(); //!< Destructor
		void start() override; //!< Start the system
		void stop() override; //!< Stop the system

		static const bool resourceExists(const std::string& resourceName); //!< Check if the resource name exists

		static void registerResource(const std::string& resourceName, Resource* resource); //!< Register a resource
		static void destroyResource(const std::string& resourceName = ""); //!< Destroy a resource by name or all of them

		template<typename T> static T* getResource(const std::string& resourceName); //!< Get a resource by name
		template<typename T> static std::vector<T*> getResourcesOfType(const ResourceType type); //!< Get all resources of a certain type
		static std::map<std::string, Resource*>& getResources(); //!< Get a list of all resources

		static std::string getCurrentDirectory(); //!< Returns the current directory
		static std::string getContents(const std::string& filePath); //!< Return the file contents
		static std::string getLineFromString(const std::string& stringContents, const uint32_t lineIndex); //!< Return the contents on line
		static nlohmann::json getJSON(const std::string& filePath); //!< Load the contents of a file into a JSON object and return the object

		static const uint32_t getConfigValue(const Config::ConfigData data); //!< Get a config value
		static void setConfigValue(const Config::ConfigData data, const uint32_t value); //!< Set a config value

		static FrameBuffer* getDefaultFrameBuffer(); //!< Get the default framebuffer

		static void printResourceManagerDetails(); //!< Print resource manager details
		static void printResourceDetails(const std::string& resourceName = ""); //!< Print resource details
	};

	template<typename T>
	//! getResource()
	/*!
	\param resourceName a const std::string& - The name of the resource
	\return a T* - The resource in type T
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
	//! getResourcesOfType()
	/*!
	\param type a const ResourceType - The resource type
	\return a std::vector<T*> - The list of resources of type T
	*/
	static std::vector<T*> ResourceManager::getResourcesOfType(const ResourceType type)
	{
		std::vector<T*> resources;

		for (auto& res : s_loadedResources)
		{
			if (res.second)
			{
				if (res.second->getType() == type)
					resources.emplace_back(static_cast<T*>(res.second));
			}
		}

		return resources;
	}
}

#endif
/*! \file resourceManager.cpp
*
* \brief A resource manager containing all the resources that are currently loaded. All resources are stored in a single map containing a key which is a unique string.
*
* \author Daniel Bullin
*
*/
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/log.h"

namespace Engine
{
	bool ResourceManager::s_enabled = false; //!< Set to false
	uint32_t ResourceManager::s_maxSubTexturesPerMaterial = 0; //!< Set to 0
	uint32_t ResourceManager::s_batchCapacity3D = 0; //!< Set to 0
	uint32_t ResourceManager::s_batchCapacity2D = 0; //!< Set to 0
	uint32_t ResourceManager::s_vertexCapacity3D = 0; //!< Set to 0
	uint32_t ResourceManager::s_indexCapacity3D = 0; //!< Set to 0
	uint32_t ResourceManager::s_maxLayersPerScene = 0; //!< Set to 0
	uint32_t ResourceManager::s_maxRenderPassesPerScene = 0; //!< Set to 0
	uint32_t ResourceManager::s_maxComponentsInstancePerEntity = 0; //!< Set to 0
	std::map<std::string, Resource*> ResourceManager::s_loadedResources; //!< A list of loaded resources

	//! ResourceManager()
	ResourceManager::ResourceManager() : System(SystemType::ResourceManager)
	{
	}

	//! ~ResourceManager()
	ResourceManager::~ResourceManager()
	{
	}

	//! start()
	void ResourceManager::start()
	{
		if (!s_enabled)
		{
			ENGINE_INFO("[ResourceManager::start] Starting the resource manager.");
			s_enabled = true;

			// Set all configuration values associated with resources
			nlohmann::json configData = getJSON("assets/config.json");
			s_maxSubTexturesPerMaterial = configData["maximumSubTexturesPerMaterial"];
			s_batchCapacity3D = configData["batch3DCapacity"];
			s_batchCapacity2D = configData["batch2DCapacity"];
			s_vertexCapacity3D = configData["maxCount3DVertices"];
			s_indexCapacity3D = configData["maxCount3DIndices"];
			s_maxLayersPerScene = configData["maxLayersPerScene"];
			s_maxRenderPassesPerScene = configData["maxRenderPassesPerScene"];
			s_maxComponentsInstancePerEntity = configData["maxNumberOfComponentsInstancePerEntity"];
		}
	}

	//! stop()
	void ResourceManager::stop()
	{
		if (s_enabled)
		{
			ENGINE_INFO("[ResourceManager::stop] Stopping the resource manager.");
			destroyResource(); //!< By providing no resource name, all resources will be deleted
			s_enabled = false;
		}
	}

	//! resourceExists()
	/*!
	\param resourceName a const std::string& - The name of the resource
	\return a const bool - Was the resource name found in the resource list
	*/
	const bool ResourceManager::resourceExists(const std::string& resourceName)
	{
		return s_loadedResources.find(resourceName) != s_loadedResources.end();
	}

	//! registerResource()
	/*!
	\param resourceName a const std::string& - The name of the resource
	\param resource a Resource* - A pointer to the resource
	*/
	void ResourceManager::registerResource(const std::string& resourceName, Resource* resource)
	{
		if (s_enabled)
		{
			// Check if the resource name is available and isn't blank
			if (!resourceExists(resourceName) && resourceName != "")
				s_loadedResources[resourceName] = resource;
			else
				ENGINE_ERROR("[ResourceManager::registerResource] This resource name has already been taken by another resource. Name: {0}.", resourceName);
		}
	}

	//! destroyResource()
	/*!
	\param resourceName a const std::string& - The name of the resource
	*/
	void ResourceManager::destroyResource(const std::string& resourceName)
	{
		// If the resource name is empty, delete all resources
		if (resourceName == "")
		{
			// Sort shader entries by some property
			std::vector<std::pair<std::string, Resource*>> mapConvertedVector(s_loadedResources.begin(), s_loadedResources.end());

			std::sort(mapConvertedVector.begin(), mapConvertedVector.end(),
				[](std::pair<std::string, Resource*> a, std::pair<std::string, Resource*> b)
			{
				return static_cast<int>(a.second->getType()) < static_cast<int>(b.second->getType());
			}
			);

			// Clean up all pointers
			for (auto& res : mapConvertedVector)
				delete res.second;

			// Clear the list
			s_loadedResources.clear();
		}
		else
		{
			// Check if resource name exists
			if (resourceExists(resourceName))
			{
				delete s_loadedResources[resourceName];
				s_loadedResources.erase(resourceName);
			}
		}
	}

	//! getResources()
	/*!
	\return a std::map<std::string, Resource*>& - A list of all resources that are currently loaded
	*/
	std::map<std::string, Resource*>& ResourceManager::getResources()
	{
		return s_loadedResources;
	}

	//! getConfigValue()
	/*!
	\param data const ConfigData - The configurable data to retrieve
	\return a const uint32_t - The value of the configured data
	*/
	const uint32_t ResourceManager::getConfigValue(const ConfigData data)
	{
		switch (data)
		{
			case ConfigData::MaxSubTexturesPerMaterial:
			{
				return s_maxSubTexturesPerMaterial;
			}
			case ConfigData::BatchCapacity3D:
			{
				return s_batchCapacity3D;
			}
			case ConfigData::BatchCapacity2D:
			{
				return s_batchCapacity2D;
			}
			case ConfigData::VertexCapacity3D:
			{
				return s_vertexCapacity3D;
			}
			case ConfigData::IndexCapacity3D:
			{
				return s_indexCapacity3D;
			}
			case ConfigData::MaxLayersPerScene:
			{
				return s_maxLayersPerScene;
			}
			case ConfigData::MaxRenderPassesPerScene:
			{
				return s_maxRenderPassesPerScene;
			}
			case ConfigData::MaxComponentsInstancePerEntity:
			{
				return s_maxComponentsInstancePerEntity;
			}
		}
		return 0;
	}

#pragma region "File System stuff"

	//! getCurrentDirectory()
	/*!
	\return a std::string - The current directory as a string
	*/
	std::string ResourceManager::getCurrentDirectory()
	{
		if (s_enabled)
		{
			char buff[FILENAME_MAX]; //create string buffer to hold path
			_getcwd(buff, FILENAME_MAX);
			std::string filePath = buff;
			// Replace all \ with /
			// Also add a / at the end
			std::replace(filePath.begin(), filePath.end(), '\\', '/');
			filePath.append("/");
			return filePath;
		}
		else
			ENGINE_ERROR("[ResourceManager::getCurrentDirectory] This system has not been enabled.");
		return std::string();
	}

	//! getContents()
	/*!
	\param filePath a const std::string& - A reference to the file path
	\return a std::string - The contents of the file
	*/
	std::string ResourceManager::getContents(const std::string& filePath)
	{
		if (s_enabled)
		{
			// Create file object and set exceptions
			std::ifstream file;
			file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			try
			{
				// open files
				file.open(filePath);
				std::stringstream fileStream;
				// read file's buffer contents into streams
				fileStream << file.rdbuf();
				// close file handlers
				file.close();
				// convert stream into string
				return fileStream.str();
			}
			catch (std::ifstream::failure& e)
			{
				// An error occured, log to the console the file path
				ENGINE_ERROR("[ResourceManager::getContents] Cannot read from file: {0}, error: {1}", filePath, e.what());
			}
		}
		else
			ENGINE_ERROR("[ResourceManager::getContents] This system has not been enabled.");
		
		return std::string();
	}

	//! getJSON()
	/*!
	\param filePath a const std::string& - A reference to the file path
	\return a nlohmann::json - The JSON object filled with data from the file
	*/
	nlohmann::json ResourceManager::getJSON(const std::string& filePath)
	{
		if (s_enabled)
		{
			// Create file object and set exceptions
			std::ifstream file;
			file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			try
			{
				// open files
				file.open(filePath);
				nlohmann::json j;
				// read stream into json object
				file >> j;
				file.close();
				return j;
			}
			catch (std::ifstream::failure& e)
			{
				// An error occured, log to the console the file path
				ENGINE_ERROR("[ResourceManager::getJSON] Cannot read from file: {0}, error: {1}", filePath, e.what());
			}
		}
		else
			ENGINE_ERROR("[ResourceManager::getJSON] This system has not been enabled.");
		return nlohmann::json();
	}

#pragma endregion

	//! printResourceManagerDetails()
	void ResourceManager::printResourceManagerDetails()
	{
		if (s_enabled)
		{
			ENGINE_TRACE("Resource Manager Details");
			ENGINE_TRACE("==========================");
			ENGINE_TRACE("Resource List Size: {0}", s_loadedResources.size());
			ENGINE_TRACE("==========================");
		}
		else
			ENGINE_ERROR("[ResourceManager::printResourceManagerDetails] This system has not been enabled.");
	}

	//! printResourceDetails()
	void ResourceManager::printResourceDetails()
	{
		if (s_enabled)
		{
			ENGINE_TRACE("Resource Details");
			ENGINE_TRACE("==========================");
			for (auto& res : s_loadedResources)
				ENGINE_TRACE("Resource Name: {0}, Address: {1}.", res.first, (void*)res.second);
			ENGINE_TRACE("==========================");
		}
		else
			ENGINE_ERROR("[ResourceManager::printResourceDetails] This system has not been enabled.");
	}
}
/*! \file resourceManager.cpp
*
* \brief A resource manager containing all the resources that are currently loaded. All resources are stored in a single map containing a key which is a unique string.
*
* \author Daniel Bullin
*
*/
#include "independent/systems/systems/resourceManager.h"
#include "independent/utils/resourceLoader.h"
#include "independent/systems/systems/log.h"

namespace Engine
{
	bool ResourceManager::s_enabled = false; //!< Set to false
	std::vector<uint32_t> ResourceManager::s_configValues;
	std::map<std::string, Resource*> ResourceManager::s_loadedResources; //!< A list of loaded resources

	//! getConfigAsString()
	/*
	\param data a const uint32_t - The config data to convert
	\return a std::string - The config data name as a string literal
	*/
	static std::string getConfigAsString(const uint32_t data)
	{
		switch (data)
		{
			case Config::ConfigData::MaxSubTexturesPerMaterial:
				return "[MaxSubtexturesPerMaterial]";
			case Config::ConfigData::VertexCapacity3D:
				return "[VertexCapacity3D]";
			case Config::ConfigData::IndexCapacity3D:
				return "[IndexCapacity3D]";
			case Config::ConfigData::BatchCapacity3D:
				return "[BatchCapacity3D]";
			case Config::ConfigData::BatchCapacity2D:
				return "[BatchCapacity2D]";
			case Config::ConfigData::MaxLayersPerScene:
				return "[MaxLayersPerScene]";
			case Config::ConfigData::MaxRenderPassesPerScene:
				return "[MaxRenderPassesPerScene]";
			case Config::ConfigData::MaxLightsPerDraw:
				return "[MaxLightsPerRenderCall]";
			case Config::ConfigData::UseBloom:
				return "[UseBloom]";
			case Config::ConfigData::BloomBlurFactor:
				return "[BloomBlurFactor]";
			case Config::ConfigData::PrintResourcesInDestructor:
				return "[PrintResourcesInDestructor]";
			case Config::ConfigData::PrintOpenGLDebugMessages:
				return "[PrintOpenGLDebugMessages]";
			default: return 0;
		}
	}

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

			ENGINE_INFO("[ResourceManager::start] Loading common configuration values.");
			// Set all configuration values associated with resources
			nlohmann::json configData = getJSON("assets/config.json");
			s_configValues.push_back(configData["maximumSubTexturesPerMaterial"]);
			s_configValues.push_back(configData["vertex3DCapacity"]);
			s_configValues.push_back(configData["index3DCapacity"]);
			s_configValues.push_back(configData["batchCapacity3D"]);
			s_configValues.push_back(configData["batchCapacity2D"]);
			s_configValues.push_back(configData["maxLayersPerScene"]);
			s_configValues.push_back(configData["maxRenderPassesPerScene"]);
			s_configValues.push_back(configData["maxLightsPerRenderCall"]);
			s_configValues.push_back(configData["useBloom"]);
			s_configValues.push_back(configData["bloomBlurFactor"]);
			s_configValues.push_back(configData["printResourcesInDestructor"]);
			s_configValues.push_back(configData["printOpenGLDebugMessages"]);

			ENGINE_INFO("[ResourceManager::start] Loading common resources.");
			ResourceLoader::loadVertexBuffers("assets/vertexBuffers.json");
			ResourceLoader::loadIndexBuffers("assets/indexBuffers.json");
			ResourceLoader::loadVertexArrays("assets/vertexArrays.json");
			ResourceLoader::loadIndirectBuffers("assets/indirectBuffers.json");
			ResourceLoader::loadUniformBuffers("assets/uniformBuffers.json");
			ResourceLoader::loadFrameBuffers("assets/frameBuffers.json");
			ResourceLoader::loadShaderPrograms("assets/shaders.json");
			ResourceLoader::loadTextures("assets/textures.json");
			ResourceLoader::loadSubTextures("assets/subTextures.json");
			ResourceLoader::loadMaterials("assets/materials.json");
			ResourceLoader::load3DModels("assets/models.json");

			printResourceManagerDetails();
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
			{
				if (!resource)
				{
					ENGINE_ERROR("[ResourceManager::registerResource] This resource is not a valid resource. Name: {0}.", resourceName);
					return;
				}

				s_loadedResources[resourceName] = resource;
			}
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
			// Sort resources entries by type, this order can be seen in resource.h
			std::vector<std::pair<std::string, Resource*>> mapConvertedVector(s_loadedResources.begin(), s_loadedResources.end());

			std::sort(mapConvertedVector.begin(), mapConvertedVector.end(),
				[](std::pair<std::string, Resource*> a, std::pair<std::string, Resource*> b)
			{
				return static_cast<int>(a.second->getType()) > static_cast<int>(b.second->getType());
			}
			);

			ResourceType type = mapConvertedVector.front().second->getType();
			ENGINE_INFO("[ResourceManager::destroyResource] Deleting all resources of type: {0}.", toString(type));
			// Clean up all pointers
			for (auto& res : mapConvertedVector)
			{
				if (res.second)
				{
					ResourceType currentType = res.second->getType();
					if(type != currentType)
						ENGINE_INFO("[ResourceManager::destroyResource] Deleting all resources of type: {0}.", toString(currentType));

					type = currentType;
					ENGINE_TRACE("Deleting resource: {0}.", res.second->getName());
					delete res.second;

				}
			}

			// Clear the list
			s_loadedResources.clear();
		}
		else
		{
			// Check if resource name exists
			if (resourceExists(resourceName))
			{
				if (s_loadedResources[resourceName])
				{
					ENGINE_TRACE("Deleting resource: {0}.", resourceName);
					delete s_loadedResources[resourceName];
				}
				else
					ENGINE_ERROR("[ResourceManager::destroyResource] The resource cannot be deleted as it is not a valid resource. Name: {0}", resourceName);

				s_loadedResources.erase(resourceName);
			}
			else
				ENGINE_ERROR("[ResourceManager::destroyResource] The resource by name was not found. Name: {0}", resourceName);
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
	\param data const Config::ConfigData - The configurable data to retrieve
	\return a const uint32_t - The value of the configured data
	*/
	const uint32_t ResourceManager::getConfigValue(const Config::ConfigData data)
	{
		if (s_configValues.size() > static_cast<uint32_t>(data))
			return s_configValues[static_cast<uint32_t>(data)];
		else
			return 0;
	}

	//! setConfigValue
	/*
	\param data a const Config::ConfigData - The config value to edit
	\param value a const uint32_t - The new config value
	*/
	void ResourceManager::setConfigValue(const Config::ConfigData data, const uint32_t value)
	{
		if (s_configValues.size() > static_cast<uint32_t>(data))
			s_configValues[static_cast<uint32_t>(data)] = value;
	}

	//! getDefaultFrameBuffer()
	/*
	\return a FrameBuffer* - The default framebuffer
	*/
	FrameBuffer* ResourceManager::getDefaultFrameBuffer()
	{
		for (auto& FBO : getResourcesOfType<FrameBuffer>(ResourceType::FrameBuffer))
		{
			if (FBO)
			{
				if (FBO->isDefault())
					return FBO;
			}
		}
		return nullptr;
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
		return nlohmann::json();
	}

#pragma endregion

	//! printResourceManagerDetails()
	void ResourceManager::printResourceManagerDetails()
	{
		if (s_enabled)
		{
			ENGINE_TRACE("==========================");
			ENGINE_TRACE("Resource Manager Details");
			ENGINE_TRACE("==========================");
			ENGINE_TRACE("Resource List Size: {0}", s_loadedResources.size());
			for (int i = 0; i < s_configValues.size(); i++)
				ENGINE_TRACE("Config: {0} has a value of {1}.", getConfigAsString(i), s_configValues[i]);
			ENGINE_TRACE("==========================");
		}
		else
			ENGINE_ERROR("[ResourceManager::printResourceManagerDetails] This system has not been enabled.");
	}

	//! printResourceDetails()
	/*
	\param resourceName a const std::string& - The name of the resource whose details to print
	*/
	void ResourceManager::printResourceDetails(const std::string& resourceName)
	{
		if (s_enabled)
		{
			// If name is blank, print all resource details
			if (resourceName != "")
			{
				if (resourceExists(resourceName))
				{
					auto resource = getResource<Resource>(resourceName);

					if (resource)
					{
						ENGINE_TRACE("Resource Details");
						ENGINE_TRACE("==========================");
						ENGINE_TRACE("Name: {0}.", resource->getName());
						ENGINE_TRACE("Type: {0}.", toString(resource->getType()));
						resource->printDetails();
						ENGINE_TRACE("==========================");
					}
					else
						ENGINE_ERROR("[ResourceManager::printResourceDetails] The resource is not a valid resource. Name: {0}.", resourceName);
				}
				else
					ENGINE_ERROR("[ResourceManager::printResourceDetails] The resource name was not found in the resource manager. Name: {0}.", resourceName);
			}
			else
			{
				ENGINE_TRACE("Resource Details");
				ENGINE_TRACE("==========================");
				for (auto& resource : s_loadedResources)
				{
					if (resource.second)
						resource.second->printDetails();
					ENGINE_TRACE("==========================");
				}
			}
		}
		else
			ENGINE_ERROR("[ResourceManager::printResourceDetails] This system has not been enabled.");
	}
}
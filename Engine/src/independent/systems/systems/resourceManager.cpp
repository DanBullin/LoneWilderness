/*! \file resourceManager.cpp
*
* \brief A resource manager containing all the resources that are currently loaded.
* All resources are stored in a map of strings and shared pointers with ownership to the data.
*
* \author Daniel Bullin
*
*/

#include "independent/systems/systems/log.h"
#include "independent/systems/systems/resourceManager.h"

namespace Engine
{
	bool ResourceManager::s_enabled = false; //!< Set to false
	Map<std::string, Model3D> ResourceManager::s_loaded3DModels = Map<std::string, Model3D>(); //!< Initialise empty list
	Map<std::string, ShaderProgram> ResourceManager::s_loadedShaders = Map<std::string, ShaderProgram>(); //!< Initialise empty list
	Map<std::string, Texture2D> ResourceManager::s_loadedTextures2D = Map<std::string, Texture2D>(); //!< Initialise empty list
	Map<std::string, CubeMapTexture> ResourceManager::s_loadedCubemaps = Map<std::string, CubeMapTexture>(); //!< Initialise empty list
	Map<std::string, SubTexture2D> ResourceManager::s_loadedSubTextures2D = Map<std::string, SubTexture2D>(); //!< Initialise empty list
	Map<std::string, Material> ResourceManager::s_loadedMaterials = Map<std::string, Material>(); //!< Initialise empty list
	Map<std::string, UniformBuffer> ResourceManager::s_loadedUniformBuffers = Map<std::string, UniformBuffer>(); //!< Initialise empty list
	Map<std::string, FrameBuffer> ResourceManager::s_loadedFrameBuffers = Map<std::string, FrameBuffer>(); //!< Initialise empty list

	//! ResourceManager()
	ResourceManager::ResourceManager() : System(Systems::Type::ResourceManager)
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
		}
	}

	//! stop()
	void ResourceManager::stop()
	{
		if (s_enabled)
		{
			ENGINE_INFO("[ResourceManager::stop] Stopping the resource manager.");
			destroyResource<FrameBuffer>();
			destroyResource<UniformBuffer>();
			destroyResource<ShaderProgram>();
			destroyResource<Material>();
			destroyResource<SubTexture2D>();
			destroyResource<CubeMapTexture>();
			destroyResource<Texture2D>();
			destroyResource<Model3D>();
			s_enabled = false;
		}
	}

	//! loadInternalResources()
	void ResourceManager::loadInternalResources()
	{
		if (s_enabled)
		{
			// Load UBOs
			Shared<UniformBuffer> ubo;

			// Camera
			UniformBufferLayout uboLayout = { {"u_view", ShaderDataType::Mat4}, {"u_projection", ShaderDataType::Mat4} };
			ubo.reset(UniformBuffer::create(uboLayout));
			registerUniformBuffer("Camera", ubo);

			uboLayout = { {"u_lightPos", ShaderDataType::Float3}, {"u_viewPos", ShaderDataType::Float3}, {"u_lightColour", ShaderDataType::Float3} };
			ubo.reset(UniformBuffer::create(uboLayout));
			registerUniformBuffer("Light", ubo);

			// FrameBuffers
			Shared<FrameBuffer> fbo;
			fbo.reset(FrameBuffer::createDefault());
			registerFrameBuffer("Default", fbo);
		}
		else
			ENGINE_ERROR("[ResourceManager::loadInternalResources] This system has not been enabled.");
	}

	//! loadResourcesByScene()
	/*!
	\param sceneName a const std::string& - The name of the scene
	*/
	void ResourceManager::loadResourcesByScene(const std::string& sceneName)
	{
		if (s_enabled)
		{
			loadResourcesByFilePath<Model3D>("assets/scenes/" + sceneName + "/models.json");
			loadResourcesByFilePath<ShaderProgram>("assets/scenes/" + sceneName + "/shaders.json");
			loadResourcesByFilePath<Texture2D>("assets/scenes/" + sceneName + "/textures.json");
			loadResourcesByFilePath<SubTexture2D>("assets/scenes/" + sceneName + "/subTextures.json");
			loadResourcesByFilePath<Material>("assets/scenes/" + sceneName + "/materials.json");
		}
		else
			ENGINE_ERROR("[ResourceManager::loadResourcesByScene] This system has not been enabled.");
	}

	//! register3DModel()
	/*!
	\param resourceName a const char* - The name of the resource
	\param res a const std::shared_ptr<Model3D>& - A reference to the resource
	*/
	void ResourceManager::register3DModel(const char* resourceName, const Shared<Model3D>& res)
	{
		if (s_enabled)
		{
			// 3D Model is being added
			if (s_loaded3DModels.find(resourceName) == s_loaded3DModels.end())
			{
				s_loaded3DModels[resourceName] = res;
			}
			else
				ENGINE_ERROR("[ResourceManager::register3DModel] This resource name has already been taken by another resource. Name: {0}.", resourceName);
		}
	}

	//! registerTexture2D()
	/*!
	\param resourceName a const char* - The name of the resource
	\param res a const std::shared_ptr<Texture2D>& - A reference to the resource
	*/
	void ResourceManager::registerTexture2D(const char* resourceName, const Shared<Texture2D>& res)
	{
		if (s_enabled)
		{
			// 2D Texture is being added
			if (s_loadedTextures2D.find(resourceName) == s_loadedTextures2D.end())
			{
				s_loadedTextures2D[resourceName] = res;
			}
			else
				ENGINE_ERROR("[ResourceManager::registerTexture2D] This resource name has already been taken by another resource. Name: {0}.", resourceName);
		}
	}

	//! registerCubemap()
	/*!
	\param resourceName a const char* - The name of the resource
	\param res a const std::shared_ptr<CubeMapTexture>& - A reference to the resource
	*/
	void ResourceManager::registerCubemap(const char* resourceName, const Shared<CubeMapTexture>& res)
	{
		if (s_enabled)
		{
			// Cubemap texture is being added
			if (s_loadedCubemaps.find(resourceName) == s_loadedCubemaps.end())
			{
				s_loadedCubemaps[resourceName] = res;
			}
			else
				ENGINE_ERROR("[ResourceManager::registerCubemap] This resource name has already been taken by another resource. Name: {0}.", resourceName);
		}
	}

	//! registerSubTexture2D()
	/*!
	\param resourceName a const char* - The name of the resource
	\param res a const std::shared_ptr<SubTexture2D>& - A reference to the resource
	*/
	void ResourceManager::registerSubTexture2D(const char* resourceName, const Shared<SubTexture2D>& res)
	{
		if (s_enabled)
		{
			// 2D Subtexture is being added
			if (s_loadedSubTextures2D.find(resourceName) == s_loadedSubTextures2D.end())
			{
				s_loadedSubTextures2D[resourceName] = res;
			}
			else
				ENGINE_ERROR("[ResourceManager::registerSubTexture2D] This resource name has already been taken by another resource. Name: {0}.", resourceName);
		}
	}

	//! registerMaterial()
	/*!
	\param resourceName a const char* - The name of the resource
	\param res a const std::shared_ptr<Material>& - A reference to the resource
	*/
	void ResourceManager::registerMaterial(const char* resourceName, const Shared<Material>& res)
	{
		if (s_enabled)
		{
			// Material is being added
			if (s_loadedMaterials.find(resourceName) == s_loadedMaterials.end())
			{
				s_loadedMaterials[resourceName] = res;
			}
			else
				ENGINE_ERROR("[ResourceManager::registerMaterial] This resource name has already been taken by another resource. Name: {0}.", resourceName);
		}
	}

	//! registerShader()
	/*!
	\param resourceName a const char* - The name of the resource
	\param res a const std::shared_ptr<ShaderProgram>& - A reference to the resource
	*/
	void ResourceManager::registerShader(const char* resourceName, const Shared<ShaderProgram>& res)
	{
		if (s_enabled)
		{
			// Shader is being added
			if (s_loadedShaders.find(resourceName) == s_loadedShaders.end())
			{
				s_loadedShaders[resourceName] = res;
			}
			else
				ENGINE_ERROR("[ResourceManager::registerShader] This resource name has already been taken by another resource. Name: {0}.", resourceName);
		}
	}

	//! registerUniformBuffer()
	/*!
	\param resourceName a const char* - The name of the resource
	\param res a const std::shared_ptr<UniformBuffer>& - A reference to the resource
	*/
	void ResourceManager::registerUniformBuffer(const char* resourceName, const Shared<UniformBuffer>& res)
	{
		if (s_enabled)
		{
			// UBO is being added
			if (s_loadedUniformBuffers.find(resourceName) == s_loadedUniformBuffers.end())
			{
				s_loadedUniformBuffers[resourceName] = res;
			}
			else
				ENGINE_ERROR("[ResourceManager::registerUniformBuffer] This resource name has already been taken by another resource. Name: {0}.", resourceName);
		}
	}

	//! registerFrameBuffer()
	/*!
	\param resourceName a const char* - The name of the resource
	\param res a const std::shared_ptr<FrameBuffer>& - A reference to the resource
	*/
	void ResourceManager::registerFrameBuffer(const char* resourceName, const Shared<FrameBuffer>& res)
	{
		if (s_enabled)
		{
			// FBO is being added
			if (s_loadedFrameBuffers.find(resourceName) == s_loadedFrameBuffers.end())
			{
				s_loadedFrameBuffers[resourceName] = res;
			}
			else
				ENGINE_ERROR("[ResourceManager::registerFrameBuffer] This resource name has already been taken by another resource. Name: {0}.", resourceName);
		}
	}

	//! getModel()
	/*!
	\param modelName a const char* - The 3D model name desired
	\return a const std::shared_ptr<Model3D> - The 3D model
	*/
	const Shared<Model3D> ResourceManager::get3DModel(const char* modelName)
	{
		if (s_enabled)
		{
			// Check if model name exists
			if (s_loaded3DModels.find(modelName) != s_loaded3DModels.end())
				return s_loaded3DModels[modelName];

			// Could not find model with name
			ENGINE_ERROR("[ResourceManager::get3DModel] 3D Model name: {0} could not be found.", modelName);
		}
		else
			ENGINE_ERROR("[ResourceManager::get3DModel] This system has not been enabled.");

		return nullptr;
	}

	//! getShader()
	/*!
	\param shaderName a const char* - The name of the shader desired
	\return a const std::shared_ptr<ShaderProgram> - A pointer to the shader
	*/
	const Shared<ShaderProgram> ResourceManager::getShader(const char* shaderName)
	{
		if (s_enabled)
		{
			// Check if shader name exists
			if (s_loadedShaders.find(shaderName) != s_loadedShaders.end())
				return s_loadedShaders[shaderName];

			// Could not find shader with name
			ENGINE_ERROR("[ResourceManager::getShader] Shader name: {0} could not be found.", shaderName);
		}
		else
			ENGINE_ERROR("[ResourceManager::getShader] This system has not been enabled.");
		
		return nullptr;
	}

	//! getTexture2D()
	/*!
	\param textureName a const char* - The name of the 2D texture desired
	\return a const std::shared_ptr<Texture2D> - A pointer to the texture
	*/
	const Shared<Texture2D> ResourceManager::getTexture2D(const char* textureName)
	{
		if (s_enabled)
		{
			// Check if texture name exists
			if (s_loadedTextures2D.find(textureName) != s_loadedTextures2D.end())
				return s_loadedTextures2D[textureName];

			// Could not find texture with name
			ENGINE_ERROR("[ResourceManager::getTexture2D] Texture name: {0} could not be found.", textureName);
		}
		else
			ENGINE_ERROR("[ResourceManager::getTexture2D] This system has not been enabled.");

		return nullptr;
	}

	//! getCubemap()
	/*!
	\param cubeMapTextureName a const char* - The name of the cubemap texture desired
	\return a const std::shared_ptr<CubeMapTexture> - A pointer to the cubemap texture
	*/
	const Shared<CubeMapTexture> ResourceManager::getCubemap(const char* cubeMapTextureName)
	{
		if (s_enabled)
		{
			// Check if cubemap texture name exists
			if (s_loadedCubemaps.find(cubeMapTextureName) != s_loadedCubemaps.end())
				return s_loadedCubemaps[cubeMapTextureName];

			// Could not find cubemap texture with name
			ENGINE_ERROR("[ResourceManager::getCubemap] Cubemap Texture name: {0} could not be found.", cubeMapTextureName);
		}
		else
			ENGINE_ERROR("[ResourceManager::getCubemap] This system has not been enabled.");

		return nullptr;
	}

	//! getSubTexture2D()
	/*!
	\param subTextureName a const char* - The name of the 2D subtexture desired
	\return a const std::shared_ptr<SubTexture2D> - A pointer to the 2D subtexture
	*/
	const Shared<SubTexture2D> ResourceManager::getSubTexture2D(const char* subTextureName)
	{
		if (s_enabled)
		{
			// Check if subtexture name exists
			if (s_loadedSubTextures2D.find(subTextureName) != s_loadedSubTextures2D.end())
				return s_loadedSubTextures2D[subTextureName];

			// Could not find subtexture with name
			ENGINE_ERROR("[ResourceManager::getSubTexture2D] Subtexture name: {0} could not be found.", subTextureName);
		}
		else
			ENGINE_ERROR("[ResourceManager::getSubTexture2D] This system has not been enabled.");

		return nullptr;
	}

	//! getMaterial()
	/*!
	\param materialName a const char* - The name of the material desired
	\return a const std::shared_ptr<Material> - A pointer to the material
	*/
	const Shared<Material> ResourceManager::getMaterial(const char* materialName)
	{
		if (s_enabled)
		{
			// Check if material name exists
			if (s_loadedMaterials.find(materialName) != s_loadedMaterials.end())
				return s_loadedMaterials[materialName];

			// Could not find material with name
			ENGINE_ERROR("[ResourceManager::getMaterial] Material name: {0} could not be found.", materialName);
		}
		else
			ENGINE_ERROR("[ResourceManager::getMaterial] This system has not been enabled.");

		return nullptr;
	}

	//! getUniformBuffer()
	/*!
	\param uniformBufferName a const char* - The name of the uniform buffer desired
	\return a const std::shared_ptr<UniformBuffer> - A pointer to the uniform buffer
	*/
	const Shared<UniformBuffer> ResourceManager::getUniformBuffer(const char* uniformBufferName)
	{
		if (s_enabled)
		{
			// Check if uniform buffer name exists
			if (s_loadedUniformBuffers.find(uniformBufferName) != s_loadedUniformBuffers.end())
				return s_loadedUniformBuffers[uniformBufferName];

			// Could not find uniform buffer with name
			ENGINE_ERROR("[ResourceManager::getUniformBuffer] Uniform buffer name: {0} could not be found.", uniformBufferName);
		}
		else
			ENGINE_ERROR("[ResourceManager::getUniformBuffer] This system has not been enabled.");

		return nullptr;
	}

	//! getFrameBuffer()
	/*!
	\param frameBufferName a const char* - The name of the frame buffer desired
	\return a const std::shared_ptr<FrameBuffer> - A pointer to the frame buffer
	*/
	const Shared<FrameBuffer> ResourceManager::getFrameBuffer(const char* frameBufferName)
	{
		if (s_enabled)
		{
			// Check if frame buffer name exists
			if (s_loadedFrameBuffers.find(frameBufferName) != s_loadedFrameBuffers.end())
				return s_loadedFrameBuffers[frameBufferName];

			// Could not find frame buffer with name
			ENGINE_ERROR("[ResourceManager::getFrameBuffer] Frame buffer name: {0} could not be found.", frameBufferName);
		}
		else
			ENGINE_ERROR("[ResourceManager::getFrameBuffer] This system has not been enabled.");

		return nullptr;
	}

	//! get3DModelList()
	/*!
	\return a const std::map<std::string, std::shared_ptr<Model3D>>& - A reference to the list
	*/
	const Map<std::string, Model3D>& ResourceManager::get3DModelList()
	{
		if (!s_enabled)
			ENGINE_ERROR("[ResourceManager::get3DModelList] This system has not been enabled.");
		return s_loaded3DModels;
	}

	//! getShadersList()
	/*!
	\return a const std::map<std::string, std::shared_ptr<ShaderProgram>>& - A reference to the list
	*/
	const Map<std::string, ShaderProgram>& ResourceManager::getShadersList()
	{
		if (!s_enabled)
			ENGINE_ERROR("[ResourceManager::getShadersList] This system has not been enabled.");
		return s_loadedShaders;
	}

	//! getTextures2DList()
	/*!
	\return a const std::map<std::string, std::shared_ptr<Texture2D>>& - A reference to the list
	*/
	const Map<std::string, Texture2D>& ResourceManager::getTextures2DList()
	{
		if (!s_enabled)
			ENGINE_ERROR("[ResourceManager::getTextures2DList] This system has not been enabled.");
		return s_loadedTextures2D;
	}

	//! getCubemapTextureList()
	/*!
	\return a const std::map<std::string, std::shared_ptr<CubeMapTexture>>& - A reference to the list
	*/
	const Map<std::string, CubeMapTexture>& ResourceManager::getCubemapTextureList()
	{
		if (!s_enabled)
			ENGINE_ERROR("[ResourceManager::getCubemapTextureList] This system has not been enabled.");
		return s_loadedCubemaps;
	}

	//! getSubTextures2DList()
	/*!
	\return a const std::map<std::string, std::shared_ptr<SubTexture2D>>& - A reference to the list
	*/
	const Map<std::string, SubTexture2D>& ResourceManager::getSubTextures2DList()
	{
		if (!s_enabled)
			ENGINE_ERROR("[ResourceManager::getSubTextures2DList] This system has not been enabled.");
		return s_loadedSubTextures2D;
	}

	//! getMaterialList()
	/*!
	\return a const std::map<std::string, std::shared_ptr<Material>>& - A reference to the list
	*/
	const Map<std::string, Material>& ResourceManager::getMaterialList()
	{
		if (!s_enabled)
			ENGINE_ERROR("[ResourceManager::getMaterialList] This system has not been enabled.");
		return s_loadedMaterials;
	}

	//! getUniformBufferList()
	/*!
	\return a const std::map<std::string, std::shared_ptr<UniformBuffer>>& - A reference to the list
	*/
	const Map<std::string, UniformBuffer>& ResourceManager::getUniformBufferList()
	{
		if(!s_enabled)
			ENGINE_ERROR("[ResourceManager::getUniformBufferList] This system has not been enabled.");
		return s_loadedUniformBuffers;
	}

	//! getFrameBufferList()
	/*!
	\return a const std::map<std::string, std::shared_ptr<FrameBuffer>>& - A reference to the list
	*/
	const Map<std::string, FrameBuffer>& ResourceManager::getFrameBufferList()
	{
		if (!s_enabled)
			ENGINE_ERROR("[ResourceManager::getFrameBufferList] This system has not been enabled.");
		return s_loadedFrameBuffers;
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
			GetCurrentDir(buff, FILENAME_MAX);
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

	//! printResourceManagerDetails()
	void ResourceManager::printResourceManagerDetails()
	{
		if (s_enabled)
		{
			ENGINE_TRACE("Resource Manager Details");
			ENGINE_TRACE("==========================");
			ENGINE_TRACE("3D Model List Size: {0}", s_loaded3DModels.size());
			ENGINE_TRACE("Shader Program List Size: {0}", s_loadedShaders.size());
			ENGINE_TRACE("Texture2D List Size: {0}", s_loadedTextures2D.size());
			ENGINE_TRACE("SubTexture2D List Size: {0}", s_loadedSubTextures2D.size());
			ENGINE_TRACE("Material List Size: {0}", s_loadedMaterials.size());
			ENGINE_TRACE("Uniform Buffer List Size: {0}", s_loadedUniformBuffers.size());
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
			for (auto& res : s_loaded3DModels)
				ENGINE_TRACE("3D Model Name: {0}, Address: {1}.", res.first, (void*)res.second.get());
			ENGINE_TRACE("==========================");
			for (auto& res : s_loadedShaders)
				ENGINE_TRACE("Shader Program Name: {0}, Address: {1}.", res.first, (void*)res.second.get());
			ENGINE_TRACE("==========================");
			for (auto& res : s_loadedTextures2D)
				ENGINE_TRACE("Texture2D Name: {0}, Address: {1}.", res.first, (void*)res.second.get());
			ENGINE_TRACE("==========================");
			for (auto& res : s_loadedSubTextures2D)
				ENGINE_TRACE("SubTexture2D Name: {0}, Address: {1}.", res.first, (void*)res.second.get());
			ENGINE_TRACE("==========================");
			for (auto& res : s_loadedMaterials)
				ENGINE_TRACE("Material Name: {0}, Address: {1}.", res.first, (void*)res.second.get());
			ENGINE_TRACE("==========================");
			for (auto& res : s_loadedUniformBuffers)
				ENGINE_TRACE("Uniform Buffer Name: {0}, Address: {1}.", res.first, (void*)res.second.get());
			ENGINE_TRACE("==========================");
		}
		else
			ENGINE_ERROR("[ResourceManager::printResourceDetails] This system has not been enabled.");
	}

#pragma endregion
}
/*! \file resourceManager.h
*
* \brief A resource manager containing all the resources that are currently loaded.
* All resources are stored in a map of strings and shared pointers with ownership to the data.
*
* \author Daniel Bullin
*
*/
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#define GetCurrentDir _getcwd //!< Macro for getting current directory

#include <json.hpp>
#include "independent/core/common.h"
#include "independent/systems/system.h"
#include "independent/utils/assimpLoader.h"

#include "independent/rendering/geometry/model3D.h"
#include "independent/rendering/textures/subTexture.h"
#include "independent/rendering/shaders/shaderProgram.h"
#include "independent/rendering/materials/material.h"
#include "independent/rendering/uniformBuffer.h"
#include "independent/rendering/frameBuffer.h"

namespace Engine
{
	/*! \class ResourceManager
	* \brief A resource manager managing resources
	*/
	class ResourceManager : public System
	{
	private:
		static bool s_enabled; //!< Is this system enabled

		static Map<std::string, Model3D> s_loaded3DModels; //!< The 3D models which have been loaded
		static Map<std::string, ShaderProgram> s_loadedShaders; //!< The Shader programs which have been loaded
		static Map<std::string, Texture2D> s_loadedTextures2D; //!< The 2D textures which have been loaded
		static Map<std::string, SubTexture2D> s_loadedSubTextures2D; //!< The 2D subtextures which have been loaded
		static Map<std::string, Material> s_loadedMaterials; //!< The Materials which have been loaded
		static Map<std::string, UniformBuffer> s_loadedUniformBuffers; //!< The Uniform buffers which have been loaded
		static Map<std::string, FrameBuffer> s_loadedFrameBuffers; //!< The Frame buffers which have been loaded
	public:
		ResourceManager(); //!< Constructor
		~ResourceManager(); //!< Destructor
		virtual void start() override; //!< Start the system
		virtual void stop() override; //!< Stop the system

		static void loadInternalResources(); //!< Load any internal resources
		static void loadResourcesByScene(const std::string& sceneName); //!< Load resources by scene
		template<typename T> static void loadResourcesByFilePath(const std::string& filePath); //!< Load a particular resource type from a JSON formatted file

		static void register3DModel(const char* resourceName, const Shared<Model3D>& res); //!< Register a 3D model into the resource manager
		static void registerTexture2D(const char* resourceName, const Shared<Texture2D>& res); //!< Register a 2D texture into the resource manager
		static void registerSubTexture2D(const char* resourceName, const Shared<SubTexture2D>& res); //!< Register a 2D subtexture into the resource manager
		static void registerMaterial(const char* resourceName, const Shared<Material>& res); //!< Register a Material into the resource manager
		static void registerShader(const char* resourceName, const Shared<ShaderProgram>& res); //!< Register a shader program into the resource manager
		static void registerUniformBuffer(const char* resourceName, const Shared<UniformBuffer>& res); //!< Register a uniform buffer into the resource manager
		static void registerFrameBuffer(const char* resourceName, const Shared<FrameBuffer>& res); //!< Register a frame buffer into the resource manager

		static const Shared<Model3D> get3DModel(const char* modelName); //!< Get a model by name
		static const Shared<ShaderProgram> getShader(const char* shaderName); //!< Get a shader by name
		static const Shared<Texture2D> getTexture2D(const char* textureName); //!< Get a 2D texture by name
		static const Shared<SubTexture2D> getSubTexture2D(const char* subTextureName); //!< Get a 2D sub texture by name
		static const Shared<Material> getMaterial(const char* materialName); //!< Get a material by name
		static const Shared<UniformBuffer> getUniformBuffer(const char* uniformBufferName); //!< Get a UBO by name
		static const Shared<FrameBuffer> getFrameBuffer(const char* frameBufferName); //!< Get a FBO by name

		static const Map<std::string, Model3D>& get3DModelList(); //!< Get the list of 3D models
		static const Map<std::string, ShaderProgram>& getShadersList(); //!< Get the list of shader programs
		static const Map<std::string, Texture2D>& getTextures2DList(); //!< Get the list of 2D textures
		static const Map<std::string, SubTexture2D>& getSubTextures2DList(); //!< Get the list of 2D subtextures
		static const Map<std::string, Material>& getMaterialList(); //!< Get the list of materials
		static const Map<std::string, UniformBuffer>& getUniformBufferList(); //!< Get the list of uniform buffer objects
		static const Map<std::string, FrameBuffer>& getFrameBufferList(); //!< Get the list of frame buffer objects

		template<typename T> static void destroyResource(); //!< Destroy all resources of type
		template<typename T> static void destroyResourceByName(const char* resourceName); //!< Destroy a resource by name

		static std::string getCurrentDirectory(); //!< Returns the current directory
		static std::string getContents(const std::string& filePath); //!< Return the file contents
		static nlohmann::json getJSON(const std::string& filePath); //!< Load the contents of a file into a JSON object and return the object

		static void printResourceManagerDetails(); //!< Print resource manager details
		static void printResourceDetails(); //!< Print resource details
	};

	template<typename T>
	//! loadResourcesByFilePath()
	/*!
	\param filePath a const std::string& - The filepath of the JSON formatted file
	*/
	static void ResourceManager::loadResourcesByFilePath(const std::string& filePath)
	{
		if (s_enabled)
		{
			if (typeid(T) == typeid(Model3D))
			{
				// User is requesting to load 3D Models
				nlohmann::json model3DData = getJSON(filePath);

				// Go through each model and load it
				for (auto& model : model3DData["models"])
				{
					// Get the model name
					std::string name = model["name"].get<std::string>();

					// Check if it exists
					if (s_loaded3DModels.find(name.c_str()) == s_loaded3DModels.end())
					{
						// Model name doesn't exist, we can use it. Now let's load the model
						Shared<Model3D> newModel;
						newModel.reset(new Model3D);

						// We will use ASSIMP to read the model file
						if (model["filePath"].get<std::string>() != "")
							AssimpLoader::loadModel(model["filePath"].get<std::string>(), newModel->getMeshes());

						s_loaded3DModels[name] = newModel;
					}
					else
						ENGINE_ERROR("[ResourceManager::loadResourcesByFilePath] 3D Model name already taken. Name: {0}", name);
				}
			}
			else if (typeid(T) == typeid(ShaderProgram))
			{
				// User is requesting to load shader programs
				nlohmann::json shaderProgramData = getJSON(filePath);

				// Go through each shader program and load it
				for (auto& shader : shaderProgramData["programs"])
				{
					// Get the shader name
					std::string name = shader["name"].get<std::string>();

					// Check if it exists
					if (s_loadedShaders.find(name.c_str()) == s_loadedShaders.end())
					{
						// Shader name doesn't exist, we can use it. Now let's load the shader
						Shared<ShaderProgram> newShader;
						newShader.reset(ShaderProgram::create());

						// Now build the shader program
						const std::string vertexShader = shader["vertexShader"].get<std::string>();
						const std::string fragmentShader = shader["fragmentShader"].get<std::string>();
						const std::string geometryShader = shader["geometryShader"].get<std::string>();
						const std::string tessControlShader = shader["tessControlShader"].get<std::string>();
						const std::string tessEvaluationShader = shader["tessEvaluationShader"].get<std::string>();

						// Build
						newShader->build(vertexShader.c_str(), fragmentShader.c_str(), geometryShader.c_str(), tessControlShader.c_str(), tessEvaluationShader.c_str());

						// Get and set all uniforms
						std::vector<std::string> uniforms;
						for (auto& uniform : shader["uniforms"])
							uniforms.push_back(uniform.get<std::string>());

						newShader->setUniforms(uniforms);

						s_loadedShaders[name] = newShader;
					}
					else
						ENGINE_ERROR("[ResourceManager::loadResourcesByFilePath] Shader program name already taken. Name: {0}", name);
				}
			}
			else if (typeid(T) == typeid(Texture2D))
			{
				// User is requesting to load 2D textures
				nlohmann::json textureData = getJSON(filePath);

				// Go through each 2D texture and load it
				for (auto& texture : textureData["textures2D"])
				{
					// Get the texture name
					std::string name = texture["name"].get<std::string>();
					// Check if it exists
					if (s_loadedTextures2D.find(name.c_str()) == s_loadedTextures2D.end())
					{
						// Fill texture properties
						// [Width], [Height], [WrapS], [WrapT], [WrapR], [MinFilter], [MaxFilter], [gammaCorrect], [FlipUVs]
						Texture2DProperties properties(0, 0,
							texture["wrapS"].get<std::string>(), texture["wrapT"].get<std::string>(), texture["wrapR"].get<std::string>(), texture["minFilter"].get<std::string>(), texture["maxFilter"].get<std::string>(),
							texture["gammaCorrect"].get<bool>(), texture["flipUV"].get<bool>());

						// Create new texture
						Shared<Texture2D> newTexture;
						newTexture.reset(Texture2D::create(texture["filePath"].get<std::string>().c_str(), properties));
						s_loadedTextures2D[name] = newTexture;
					}
					else
						ENGINE_ERROR("[ResourceManager::loadResourcesByFilePath] 2D Texture name already taken. Name: {0}", name);
				}
			}
			else if (typeid(T) == typeid(SubTexture2D))
			{
				// User is requesting to load 2D sub textures
				nlohmann::json textureData = getJSON(filePath);

				// Go through each subtexture and load it
				for (auto& texture : textureData["subTextures"])
				{
					// Get the texture name
					std::string name = texture["name"].get<std::string>();
					// Check if it exists
					if (s_loadedSubTextures2D.find(name.c_str()) == s_loadedSubTextures2D.end())
					{
						// Get the base texture name
						Texture2D* baseTexture = getTexture2D(texture["baseTextureName"].get<std::string>().c_str()).get();
						if (baseTexture)
						{
							// Create new subtexture
							Shared<SubTexture2D> newTexture;
							// [Texture], [UVStart], [UVEnd]
							newTexture.reset(new SubTexture2D(baseTexture, { texture["UVStart"][0], texture["UVStart"][1] }, { texture["UVEnd"][0], texture["UVEnd"][1] }, texture["ConvertBottomLeft"].get<bool>()));
							s_loadedSubTextures2D[name] = newTexture;
						}
						else
							ENGINE_ERROR("[ResourceManager::loadResourcesByFilePath] Invalid base texture provided. Name: {0}", name);
					}
					else
						ENGINE_ERROR("[ResourceManager::loadResourcesByFilePath] SubTexture name already taken. Name: {0}", name);
				}
			}
			else if (typeid(T) == typeid(Material))
			{
				// Load all materials
				nlohmann::json materialData = getJSON(filePath);

				// Go through each material and load it
				for (auto& material : materialData["materials"])
				{
					// Get the material name
					std::string name = material["name"].get<std::string>();
					// Check if it exists
					if (s_loadedMaterials.find(name.c_str()) == s_loadedMaterials.end())
					{
						// Create material
						Shared<Material> newMaterial;
						std::vector<SubTexture2D*> textures;
						textures.reserve(10);

						// Loop through all subtextures and add to the list
						for (auto& texture : material["subTextures"])
							textures.push_back(getSubTexture2D(texture.get<std::string>().c_str()).get());
						
						newMaterial.reset(new Material(textures, getShader(material["shader"].get<std::string>().c_str()).get(),
							{ material["tint"][0], material["tint"][1], material["tint"][2], material["tint"][3] }));

						s_loadedMaterials[name] = newMaterial;
					}
					else
						ENGINE_ERROR("[ResourceManager::loadResourcesByFilePath] Material name already taken. Name: {0}", name);
				}
			}
		}
		else
			ENGINE_ERROR("[ResourceManager::loadResourcesByFilePath] This system has not been enabled.");
	}

	template<typename T>
	//! destroyResource()
	static void ResourceManager::destroyResource()
	{
		if (s_enabled)
		{
			if (typeid(T) == typeid(Model3D))
				s_loaded3DModels.clear();
			else if (typeid(T) == typeid(ShaderProgram))
				s_loadedShaders.clear();
			else if (typeid(T) == typeid(UniformBuffer))
				s_loadedUniformBuffers.clear();
			else if (typeid(T) == typeid(Texture2D))
				s_loadedTextures2D.clear();
			else if (typeid(T) == typeid(SubTexture2D))
				s_loadedSubTextures2D.clear();
			else if (typeid(T) == typeid(Material))
				s_loadedMaterials.clear();
			else if (typeid(T) == typeid(FrameBuffer))
				s_loadedFrameBuffers.clear();
			else
				return;

			ENGINE_INFO("[ResourceManager::destroyResource] Deleted all {0} from the resource manager.", typeid(T).name());
		}
		else
			ENGINE_ERROR("[ResourceManager::destroyResource] This system has not been enabled");
	}

	template<typename T>
	//! destroyResourceByName()
	/*!
	\param resourceName a const char* - The name of the resource to delete
	*/
	static void ResourceManager::destroyResourceByName(const char* resourceName)
	{
		if (s_enabled)
		{
			if (typeid(T) == typeid(Model3D))
			{
				if (s_loaded3DModels.find(resourceName) != s_loaded3DModels.end())
					s_loaded3DModels.erase(resourceName);
			}
			else if (typeid(T) == typeid(ShaderProgram))
			{
				if (s_loadedShaders.find(resourceName) != s_loadedShaders.end())
					s_loadedShaders.erase(resourceName);
			}
			else if (typeid(T) == typeid(UniformBuffer))
			{
				if (s_loadedUniformBuffers.find(resourceName) != s_loadedUniformBuffers.end())
					s_loadedUniformBuffers.erase(resourceName);
			}
			else if (typeid(T) == typeid(Texture2D))
			{
				if (s_loadedTextures2D.find(resourceName) != s_loadedTextures2D.end())
					s_loadedTextures2D.erase(resourceName);
			}
			else if (typeid(T) == typeid(SubTexture2D))
			{
				if (s_loadedSubTextures2D.find(resourceName) != s_loadedSubTextures2D.end())
					s_loadedSubTextures2D.erase(resourceName);
			}
			else if (typeid(T) == typeid(Material))
			{
				if (s_loadedMaterials.find(resourceName) != s_loadedMaterials.end())
					s_loadedMaterials.erase(resourceName);
			}
			else if (typeid(T) == typeid(FrameBuffer))
			{
				if (s_loadedFrameBuffers.find(resourceName) != s_loadedFrameBuffers.end())
					s_loadedFrameBuffers.erase(resourceName);
			}
		}
		else
			ENGINE_ERROR("[ResourceManager::destroyResource] This system has not been enabled");
	}

}
#endif
/*! \file resourceLoader.cpp
*
* \brief A resource loader which loads resources from JSON config files
*
* \author Daniel Bullin
*
*/
#include "independent/utils/resourceLoader.h"
#include "independent/systems/systems/log.h"
#include "independent/rendering/geometry/vertex.h"
#include "independent/utils/assimpLoader.h"
#include "independent/systems/systems/windowManager.h"

namespace Engine
{
	//! getSize()
	/*!
	\param className a const std::string& - The name of the class
	\return a uint32_t - The size of the class in bytes
	*/
	uint32_t ResourceLoader::getSize(const std::string& className)
	{
		// Only support the needed classes
		if (className == "Vertex3D") return static_cast<uint32_t>(sizeof(Vertex3D));
		else if (className == "Vertex2D") return static_cast<uint32_t>(sizeof(Vertex2D));
		else if (className == "Mat4") return static_cast<uint32_t>(sizeof(glm::mat4));
		else if (className == "Vec4") return static_cast<uint32_t>(sizeof(glm::vec4));
		else if (className == "uint32_t") return static_cast<uint32_t>(sizeof(uint32_t));
		else if (className == "int32_t") return static_cast<uint32_t>(sizeof(int32_t));
		return 0;
	}

	//! getSize()
	/*!
	\param capacityLocation a const std::string& - The name of the type of capacity to apply
	\return a uint32_t - The size of the class in bytes
	*/
	uint32_t ResourceLoader::getCapacity(const std::string& capacityLocation)
	{
		// Only support the certain capacitys
		if (capacityLocation == "VertexCapacity3D") return ResourceManager::getConfigValue(ConfigData::VertexCapacity3D);
		else if (capacityLocation == "VertexCapacity2D") return ResourceManager::getConfigValue(ConfigData::BatchCapacity2D) * 4;
		else if (capacityLocation == "IndexCapacity3D") return ResourceManager::getConfigValue(ConfigData::IndexCapacity3D);
		else if (capacityLocation == "IndexCapacity2D") return ResourceManager::getConfigValue(ConfigData::BatchCapacity2D) * 6;
		else if (capacityLocation == "Batch3DCapacity") return ResourceManager::getConfigValue(ConfigData::BatchCapacity3D);
		else if (capacityLocation == "Batch2DCapacity") return ResourceManager::getConfigValue(ConfigData::BatchCapacity2D);
		return 0;
	}

	//! loadVertexBuffer()
	/*!
	\param filePath a const std::string& - The path to the vertex buffer data
	*/
	void ResourceLoader::loadVertexBuffers(const std::string& filePath)
	{
		nlohmann::json jsonData = ResourceManager::getJSON(filePath);

		// Go through each buffer and load it
		for (auto& buffer : jsonData["vertexBuffers"])
		{
			// Get the vertex buffer name
			std::string name = buffer["name"].get<std::string>();

			// Check if it exists
			if (!ResourceManager::resourceExists(name))
			{
				// Resource name is free, so we can now create a vertex buffer and assign that resource name to that resource
				VertexBuffer* newBuffer;

				VertexBufferUsage usage;
				if (buffer["usage"] == 1) usage = VertexBufferUsage::StaticDraw;
				if (buffer["usage"] == 2) usage = VertexBufferUsage::DynamicDraw;

				// Get the layout
				VertexBufferLayout layout = { {}, getSize(buffer["dataType"].get<std::string>()) };
				for (int i = 0; i < buffer["layout"].size();)
				{
					// Format is: [TYPE], [NORMALISED], [TYPE], [NORMALISED], etc
					layout.addElement({ SDT::convertStringToSDT(buffer["layout"][i].get<std::string>()) , buffer["layout"][i + 1].get<bool>(), buffer["layout"][i + 2].get<uint32_t>() });
					i += 3;
				}

				// Need to work out the size, we'll have to compare the string literals provided
				// First we need to get the datatype we'll be storing in the buffer to work out its size
				newBuffer = VertexBuffer::create(name, nullptr, getSize(buffer["dataType"].get<std::string>()) * getCapacity(buffer["size"].get<std::string>()), layout, usage);

				// Register buffer with resource manager
				ResourceManager::registerResource(name, newBuffer);
				ENGINE_INFO("[ResourceLoader::loadVertexBuffers] Loaded {0} from {1}.", name, filePath);
			}
			else
				ENGINE_ERROR("[ResourceLoader::loadVertexBuffers] Resource name already taken. Name: {0}", name);
		}
	}

	//! loadIndexBuffer()
	/*!
	\param filePath a const std::string& - The path to the index buffer data
	*/
	void ResourceLoader::loadIndexBuffers(const std::string& filePath)
	{
		nlohmann::json jsonData = ResourceManager::getJSON(filePath);

		// Go through each buffer and load it
		for (auto& buffer : jsonData["indexBuffers"])
		{
			// Get the index buffer name
			std::string name = buffer["name"].get<std::string>();

			// Check if it exists
			if (!ResourceManager::resourceExists(name))
			{
				// Resource name is free, so we can now create a index buffer and assign that resource name to that resource
				IndexBuffer* newBuffer;

				newBuffer = IndexBuffer::create(name, nullptr, getCapacity(buffer["size"].get<std::string>()));

				// Register buffer with resource manager
				ResourceManager::registerResource(name, newBuffer);
				ENGINE_INFO("[ResourceLoader::loadIndexBuffers] Loaded {0} from {1}.", name, filePath);
			}
			else
				ENGINE_ERROR("[ResourceLoader::loadIndexBuffers] Resource name already taken. Name: {0}", name);
		}
	}

	//! loadVertexArray()
	/*!
	\param filePath a const std::string& - The path to the current file
	*/
	void ResourceLoader::loadVertexArrays(const std::string& filePath)
	{
		nlohmann::json jsonData = ResourceManager::getJSON(filePath);

		// Go through each array and load it
		for (auto& array : jsonData["vertexArrays"])
		{
			// Get the vertex array name
			std::string name = array["name"].get<std::string>();

			// Check if it exists
			if (!ResourceManager::resourceExists(name))
			{
				// Resource name is free, so we can now create a vertex array and assign that resource name to that resource
				VertexArray* newArray;
				newArray = VertexArray::create(name);

				// Now attempt to assign the vertex buffers and set the index buffer
				// These buffers must be loaded before the array that uses them is created

				for (auto& buffer : array["vertexBuffers"])
				{
					// Go through each buffer and retrieve the buffer from the Resource Manager, increasing the reference counter
					if (ResourceManager::resourceExists(buffer.get<std::string>()))
						newArray->addVertexBuffer(ResourceManager::getResourceAndRef<VertexBuffer>(buffer.get<std::string>()));
				}

				// Now all vertex buffers have been added, lets set the index buffer
				if (ResourceManager::resourceExists(array["indexBuffer"].get<std::string>()))
					newArray->setIndexBuffer(ResourceManager::getResourceAndRef<IndexBuffer>(array["indexBuffer"].get<std::string>()));

				newArray->unbind();
				// Register array with resource manager
				ResourceManager::registerResource(name, newArray);
				ENGINE_INFO("[ResourceLoader::loadVertexArrays] Loaded {0} from {1}.", name, filePath);
			}
			else
				ENGINE_ERROR("[ResourceLoader::loadVertexArrays] Resource name already taken. Name: {0}", name);
		}
	}

	//! loadIndirectBuffer()
	/*!
	\param filePath a const std::string& - The path to the current file
	*/
	void ResourceLoader::loadIndirectBuffers(const std::string& filePath)
	{
		nlohmann::json jsonData = ResourceManager::getJSON(filePath);

		// Go through each buffer and load it
		for (auto& buffer : jsonData["indirectBuffers"])
		{
			// Get the indirect buffer name
			std::string name = buffer["name"].get<std::string>();

			// Check if it exists
			if (!ResourceManager::resourceExists(name))
			{
				// Resource name is free, so we can now create a indirect buffer and assign that resource name to that resource
				IndirectBuffer* newBuffer;

				// Need to work out the size, we'll have to compare the string literals provided
				// Only need a count
				newBuffer = IndirectBuffer::create(name, nullptr, getCapacity(buffer["size"].get<std::string>()));

				// Register buffer with resource manager
				ResourceManager::registerResource(name, newBuffer);
				ENGINE_INFO("[ResourceLoader::loadIndirectBuffers] Loaded {0} from {1}.", name, filePath);
			}
			else
				ENGINE_ERROR("[ResourceLoader::loadIndirectBuffers] Resource name already taken. Name: {0}", name);
		}
	}

	//! loadUniformBuffers()
	/*!
	\param filePath a const std::string& - The path to the current file
	*/
	void ResourceLoader::loadUniformBuffers(const std::string& filePath)
	{
		nlohmann::json jsonData = ResourceManager::getJSON(filePath);

		// Go through each buffer and load it
		for (auto& buffer : jsonData["uniformBuffers"])
		{
			// Get the uniform buffer name
			std::string name = buffer["name"].get<std::string>();

			// Check if it exists
			if (!ResourceManager::resourceExists(name))
			{
				// Resource name is free, so we can now create a uniform buffer and assign that resource name to that resource
				UniformBuffer* newBuffer;

				// Get the layout
				UniformBufferLayout layout;
				for (int i = 0; i < buffer["layout"].size();)
				{
					// Format is: [UNIFORM NAME], [TYPE], [UNIFORM NAME], [TYPE], etc
					layout.addElement({ buffer["layout"][i].get<std::string>().c_str(), SDT::convertStringToSDT(buffer["layout"][i+1].get<std::string>()) });
					i += 2;
				}

				layout.recalculateStride();

				// Only need to provide the layout
				newBuffer = UniformBuffer::create(name, layout);

				// Register buffer with resource manager
				ResourceManager::registerResource(name, newBuffer);
				ENGINE_INFO("[ResourceLoader::loadUniformBuffers] Loaded {0} from {1}.", name, filePath);
			}
			else
				ENGINE_ERROR("[ResourceLoader::loadUniformBuffers] Resource name already taken. Name: {0}", name);
		}
	}

	//! loadFrameBuffers()
	/*!
	\param filePath a const std::string& - The path to the current file
	*/
	void ResourceLoader::loadFrameBuffers(const std::string& filePath)
	{
		nlohmann::json jsonData = ResourceManager::getJSON(filePath);

		// Go through each buffer and load it
		for (auto& buffer : jsonData["frameBuffers"])
		{
			// Get the frame buffer name
			std::string name = buffer["name"].get<std::string>();

			// Check if it exists
			if (!ResourceManager::resourceExists(name))
			{
				// Resource name is free, so we can now create a frame buffer and assign that resource name to that resource
				FrameBuffer* newBuffer;

				// Check if the framebuffer is to be a default framebuffer
				if (!buffer["useDefault"].get<bool>())
				{
					// Get the layout of the framebuffer
					FrameBufferLayout layout;
					for (int i = 0; i < buffer["layout"].size();)
					{
						// Format is: [UNIFORM NAME], [TYPE], [UNIFORM NAME], [TYPE], etc
						layout.addAttachment(BufferAttachments::convertStringToAttachment(buffer["layout"][i].get<std::string>()), buffer["layout"][i + 1].get<bool>());
						i += 2;
					}

					// We now need to calculate the dimensions, first check if we should use the window dimensions
					glm::ivec2 dimensions;

					// We can either use the dimensions of the window or manually provide the dimensions
					if (buffer["useWindowDimensions"].get<bool>())
						dimensions = WindowManager::getFocusedWindow()->getProperties().getSize();
					else
						dimensions = glm::ivec2(buffer["size"][0], buffer["size"][1]);

					// Only need to provide the layout
					newBuffer = FrameBuffer::create(name, dimensions, layout);
				}
				else
					newBuffer = FrameBuffer::createDefault(name);

				// Register buffer with resource manager
				ResourceManager::registerResource(name, newBuffer);
				ENGINE_INFO("[ResourceLoader::loadFrameBuffers] Loaded {0} from {1}.", name, filePath);
			}
			else
				ENGINE_ERROR("[ResourceLoader::loadFrameBuffers] Resource name already taken. Name: {0}", name);
		}
	}

	//! loadShaderPrograms()
	/*!
	\param filePath a const std::string& - The path to the current file
	*/
	void ResourceLoader::loadShaderPrograms(const std::string& filePath)
	{
		nlohmann::json jsonData = ResourceManager::getJSON(filePath);

		// Go through each shader program and load it
		for (auto& shader : jsonData["programs"])
		{
			// Get the shader name
			std::string name = shader["name"].get<std::string>();

			// Check if it exists
			if (!ResourceManager::resourceExists(name))
			{
				// Shader name doesn't exist, we can use it. Now let's load the shader
				ShaderProgram* newShader = ShaderProgram::create(name);

				// Now build the shader program
				const std::string vertexShader = shader["vertexShader"].get<std::string>();
				const std::string fragmentShader = shader["fragmentShader"].get<std::string>();
				const std::string geometryShader = shader["geometryShader"].get<std::string>();
				const std::string tessControlShader = shader["tessControlShader"].get<std::string>();
				const std::string tessEvaluationShader = shader["tessEvaluationShader"].get<std::string>();

				VertexArray* vArray = ResourceManager::getResourceAndRef<VertexArray>(shader["vertexArray"].get<std::string>());

				// Build
				newShader->build(vArray, vertexShader.c_str(), fragmentShader.c_str(), geometryShader.c_str(), tessControlShader.c_str(), tessEvaluationShader.c_str());

				// Get and set all uniforms
				std::vector<std::string> uniforms;
				for (auto& uniform : shader["uniforms"])
					uniforms.push_back(uniform.get<std::string>());

				std::unordered_map<std::string, UniformBuffer*> ubos;
				for (int i = 0; i < shader["uniformBuffers"].size();)
				{
					// Format is: [UNIFORM BLOCK NAME], [UNIFORM BUFFER NAME], [UNIFORM BLOCK NAME], [UNIFORM BUFFER NAME], etc
					ubos[shader["uniformBuffers"][i].get<std::string>()] = (ResourceManager::getResourceAndRef<UniformBuffer>(shader["uniformBuffers"][i+1].get<std::string>()));
					i += 2;
				}

				newShader->setUniforms(uniforms);
				newShader->setUniformBuffers(ubos);
				newShader->setOrderImportance(shader["order"].get<uint32_t>());

				// Register shader with resource manager
				ResourceManager::registerResource(name, newShader);
				ENGINE_INFO("[ResourceLoader::loadShaderPrograms] Loaded {0} from {1}.", name, filePath);
			}
			else
				ENGINE_ERROR("[ResourceLoader::loadShaderPrograms] Resource name already taken. Name: {0}", name);
		}
	}

	//! loadTextures()
	/*!
	\param filePath a const std::string& - The path to the current file
	*/
	void ResourceLoader::loadTextures(const std::string& filePath)
	{
		nlohmann::json jsonData = ResourceManager::getJSON(filePath);

		// Go through each 2D texture and load it
		for (auto& texture : jsonData["textures2D"])
		{
			// Get the texture name
			std::string name = texture["name"].get<std::string>();

			// Check if it exists
			if (!ResourceManager::resourceExists(name))
			{
				// Fill texture properties
				// [Width], [Height], [WrapS], [WrapT], [WrapR], [MinFilter], [MaxFilter], [gammaCorrect], [FlipUVs]
				TextureProperties properties(0, 0,
					texture["wrapS"].get<std::string>(), texture["wrapT"].get<std::string>(), texture["wrapR"].get<std::string>(), 
					texture["minFilter"].get<std::string>(), texture["maxFilter"].get<std::string>(),
					texture["gammaCorrect"].get<bool>(), texture["flipUV"].get<bool>());

				// Create new texture
				Texture2D* newTexture = Texture2D::create(name, texture["filePath"].get<std::string>().c_str(), properties);

				// Register texture with resource manager
				ResourceManager::registerResource(name, newTexture);
				ENGINE_INFO("[ResourceLoader::loadTextures] Loaded {0} from {1}.", name, filePath);
			}
			else
				ENGINE_ERROR("[ResourceLoader::loadTextures] Resource name already taken. Name: {0}", name);
		}

		// Go through each texture for cubemap and load it
		for (auto& texture : jsonData["cubeMaps"])
		{
			// Get the texture name
			std::string name = texture["name"].get<std::string>();
			// Check if it exists
			if (!ResourceManager::resourceExists(name))
			{
				// Create new cubemap texture
				CubeMapTexture* newTexture = CubeMapTexture::create(name, texture["folderPath"].get<std::string>().c_str(), texture["fileType"].get<std::string>().c_str());

				// Register texture with resource manager
				ResourceManager::registerResource(name, newTexture);
				ENGINE_INFO("[ResourceLoader::loadTextures] Loaded {0} from {1}.", name, filePath);
			}
			else
				ENGINE_ERROR("[ResourceLoader::loadTextures] Resource name already taken. Name: {0}", name);
		}
	}

	//! loadSubTextures()
	/*!
	\param filePath a const std::string& - The path to the current file
	*/
	void ResourceLoader::loadSubTextures(const std::string& filePath)
	{
		nlohmann::json jsonData = ResourceManager::getJSON(filePath);

		// Go through each subtexture and load it
		for (auto& subTexture : jsonData["subTextures"])
		{
			// Get the subTexture name
			std::string name = subTexture["name"].get<std::string>();

			// Check if it exists
			if (!ResourceManager::resourceExists(name))
			{
				// Get the base texture name
				Texture2D* baseTexture = ResourceManager::getResourceAndRef<Texture2D>(subTexture["baseTextureName"].get<std::string>());

				if (baseTexture)
				{
					// Create new subtexture
					// [Texture], [UVStart], [UVEnd]

					SubTexture* newSubTexture = new SubTexture(name, baseTexture, { subTexture["UVStart"][0], subTexture["UVStart"][1] }, { subTexture["UVEnd"][0], subTexture["UVEnd"][1] }, subTexture["ConvertBottomLeft"].get<bool>());
					
					// Register subTexture with resource manager
					ResourceManager::registerResource(name, newSubTexture);
					ENGINE_INFO("[ResourceLoader::loadSubTextures] Loaded {0} from {1}.", name, filePath);
				}
				else
					ENGINE_ERROR("[ResourceLoader::loadSubTextures] Invalid base texture provided. Name: {0}", name);
			}
			else
				ENGINE_ERROR("[ResourceLoader::loadSubTextures] Resource name already taken. Name: {0}", name);
		}
	}

	//! load3DModels()
	/*!
	\param filePath a const std::string& - The path to the current file
	*/
	void ResourceLoader::load3DModels(const std::string& filePath)
	{
		nlohmann::json jsonData = ResourceManager::getJSON(filePath);

		// Go through each model and load it
		for (auto& model : jsonData["models"])
		{
			// Get the model name
			std::string name = model["name"].get<std::string>();

			// Check if it exists
			if (!ResourceManager::resourceExists(name))
			{
				// Model name doesn't exist, we can use it. Now let's load the model
				Model3D* newModel = new Model3D(name);

				// We will use ASSIMP to read the model file
				if (model["filePath"].get<std::string>() != "")
					AssimpLoader::loadModel(model["filePath"].get<std::string>(), newModel->getMeshes());

				// Register model with resource manager
				ResourceManager::registerResource(name, newModel);
				ENGINE_INFO("[ResourceLoader::load3DModels] Loaded {0} from {1}.", name, filePath);
			}
			else
				ENGINE_ERROR("[ResourceLoader::load3DModels] Resource name already taken. Name: {0}", name);
		}
	}

	//! loadMaterials()
	/*!
	\param filePath a const std::string& - The path to the current file
	*/
	void ResourceLoader::loadMaterials(const std::string& filePath)
	{
		nlohmann::json jsonData = ResourceManager::getJSON(filePath);

		// Go through each material and load it
		for (auto& material : jsonData["materials"])
		{
			// Get the material name
			std::string name = material["name"].get<std::string>();

			// Check if it exists
			if (!ResourceManager::resourceExists(name))
			{
				// Create material
				Material* newMaterial;
				std::vector<SubTexture*> textures;
				std::vector<CubeMapTexture*> cubeTextures;
				textures.reserve(ResourceManager::getConfigValue(ConfigData::MaxSubTexturesPerMaterial));
				cubeTextures.reserve(ResourceManager::getConfigValue(ConfigData::MaxSubTexturesPerMaterial));

				// Loop through all subtextures and add to the list
				for (auto& subTexture : material["subTextures"])
					textures.push_back(ResourceManager::getResourceAndRef<SubTexture>(subTexture.get<std::string>()));

				// Loop through all cubemap textures and add to the list
				for (auto& cubeTexture : material["cubeMapTextures"])
					cubeTextures.push_back(static_cast<CubeMapTexture*>(ResourceManager::getResourceAndRef<Texture>(cubeTexture.get<std::string>())));

				newMaterial = new Material(name, textures, cubeTextures, ResourceManager::getResourceAndRef<ShaderProgram>(material["shader"].get<std::string>()),
					{ material["tint"][0], material["tint"][1], material["tint"][2], material["tint"][3] });

				// Register model with resource manager
				ResourceManager::registerResource(name, newMaterial);
				ENGINE_INFO("[ResourceLoader::loadMaterials] Loaded {0} from {1}.", name, filePath);
			}
			else
				ENGINE_ERROR("[ResourceLoader::loadMaterials] Resource name already taken. Name: {0}", name);
		}
	}
}
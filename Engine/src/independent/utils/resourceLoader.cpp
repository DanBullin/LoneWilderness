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
	\param dataTypeName a const std::string& - The name of the data type
	\return a uint32_t - The size of the class in bytes
	*/
	uint32_t ResourceLoader::getSize(const std::string& dataTypeName)
	{
		// This function returns the size of the data type which matches the string literal passed (Used because files need a way to reference a data type)
		// This is required for objects which require the size of a data type
		// Update this function when necassary

		if (dataTypeName == "Vertex3D")
			return static_cast<uint32_t>(sizeof(Vertex3D));
		else if (dataTypeName == "Vertex2D")
			return static_cast<uint32_t>(sizeof(Vertex2D));
		else if (dataTypeName == "Vertex2DMultiTextured")
			return static_cast<uint32_t>(sizeof(Vertex2DMultiTextured));
		else if (dataTypeName == "Mat4")
			return static_cast<uint32_t>(sizeof(glm::mat4));
		else if (dataTypeName == "Vec4")
			return static_cast<uint32_t>(sizeof(glm::vec4));
		else if (dataTypeName == "uint32_t")
			return static_cast<uint32_t>(sizeof(uint32_t));
		else if (dataTypeName == "int32_t")
			return static_cast<uint32_t>(sizeof(int32_t));
		else if (dataTypeName == "float")
			return static_cast<uint32_t>(sizeof(float));
		return 0;
	}

	//! getCapacity()
	/*!
	\param capacityLocation a const std::string& - The name of the type of capacity to apply
	\return a uint32_t - The size of the class in bytes
	*/
	uint32_t ResourceLoader::getCapacity(const std::string& capacityLocation)
	{
		// This function returns the capacity values which are stored as config values in the resource manager
		// This is required for objects which require some capacity size
		// Update this function when necassary

		if (capacityLocation == "VertexCapacity3D")
			return ResourceManager::getConfigValue(Config::VertexCapacity3D);
		else if (capacityLocation == "VertexCapacity2D")
			return ResourceManager::getConfigValue(Config::BatchCapacity2D) * 4; // 4 vertices per quad
		else if (capacityLocation == "IndexCapacity3D")
			return ResourceManager::getConfigValue(Config::IndexCapacity3D);
		else if (capacityLocation == "IndexCapacity2D")
			return ResourceManager::getConfigValue(Config::BatchCapacity2D) * 6; // 6 indices per quad
		else if (capacityLocation == "Batch3DCapacity")
			return ResourceManager::getConfigValue(Config::BatchCapacity3D); // Number of 3D Submissions
		else if (capacityLocation == "Batch2DCapacity")
			return ResourceManager::getConfigValue(Config::BatchCapacity2D); // Number of quads
		return 0;
	}

	//! loadVertexBuffer()
	/*!
	\param filePath a const std::string& - The path to the vertex buffer data
	*/
	void ResourceLoader::loadVertexBuffers(const std::string& filePath)
	{
		nlohmann::json jsonData = ResourceManager::getJSON(filePath);

		ENGINE_INFO("[ResourceLoader::loadVertexBuffers] Loading Vertex Buffers");

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
					ShaderDataType sdt = SDT::convertStringToSDT(buffer["layout"][i].get<std::string>());
					if (sdt == ShaderDataType::None)
						ENGINE_ERROR("[ResourceLoader::loadVertexBuffers] Invalid SDT provided. Name: {0}", name);

					// Format is: [TYPE], [NORMALISED], [TYPE], [NORMALISED], etc
					layout.addElement({ sdt , buffer["layout"][i + 1].get<bool>(), buffer["layout"][i + 2].get<uint32_t>() });
					i += 3;
				}

				uint32_t size = getSize(buffer["dataType"].get<std::string>());
				if (size == 0)
				{
					ENGINE_ERROR("[ResourceLoader::loadVertexBuffers] Invalid size provided. Name: {0}", name);
				}

				uint32_t capacity = getCapacity(buffer["size"].get<std::string>());
				if (capacity == 0)
				{
					ENGINE_ERROR("[ResourceLoader::loadVertexBuffers] Invalid capacity provided. Name: {0}", name);
				}

				// Need to work out the size, we'll have to compare the string literals provided
				// First we need to get the datatype we'll be storing in the buffer to work out its size
				newBuffer = VertexBuffer::create(name, nullptr, size * capacity, layout, usage);

				// Register buffer with resource manager
				ResourceManager::registerResource(name, newBuffer);
				ENGINE_TRACE("Loaded {0} from {1}.", name, filePath);
			}
			else
				ENGINE_ERROR("[ResourceLoader::loadVertexBuffers] Resource name already taken. Name: {0}", name);
		}
	}

	//! loadVertexArray()
	/*!
	\param filePath a const std::string& - The path to the current file
	*/
	void ResourceLoader::loadVertexArrays(const std::string& filePath)
	{
		nlohmann::json jsonData = ResourceManager::getJSON(filePath);

		ENGINE_INFO("[ResourceLoader::loadVertexArrays] Loading Vertex Arrays");

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
						newArray->addVertexBuffer(ResourceManager::getResource<VertexBuffer>(buffer.get<std::string>()));
					else
						ENGINE_ERROR("[ResourceLoader::loadVertexArrays] The vertex buffer does not exist in the ResourceManager. Name: {0}", buffer.get<std::string>());
				}

				// Now all vertex buffers have been added, lets set the index buffer
				if (ResourceManager::resourceExists(array["indexBuffer"].get<std::string>()))
					newArray->setIndexBuffer(ResourceManager::getResource<IndexBuffer>(array["indexBuffer"].get<std::string>()));
				else
					ENGINE_ERROR("[ResourceLoader::loadVertexArrays] The index buffer does not exist in the ResourceManager. Name: {0}", array["indexBuffer"].get<std::string>());

				// Unbind for safety
				newArray->unbind();

				// Register array with resource manager
				ResourceManager::registerResource(name, newArray);
				ENGINE_TRACE("Loaded {0} from {1}.", name, filePath);
			}
			else
				ENGINE_ERROR("[ResourceLoader::loadVertexArrays] Resource name already taken. Name: {0}", name);
		}
	}

	//! loadUniformBuffers()
	/*!
	\param filePath a const std::string& - The path to the current file
	*/
	void ResourceLoader::loadUniformBuffers(const std::string& filePath)
	{
		nlohmann::json jsonData = ResourceManager::getJSON(filePath);

		ENGINE_INFO("[ResourceLoader::loadUniformBuffers] Loading Uniform Buffers");

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
					ShaderDataType sdt = SDT::convertStringToSDT(buffer["layout"][i + 1].get<std::string>());
					if (sdt == ShaderDataType::None)
						ENGINE_ERROR("[ResourceLoader::loadUniformBuffers] Invalid SDT provided. Name: {0}", name);

					// Format is: [UNIFORM NAME], [TYPE], [UNIFORM NAME], [TYPE], etc
					layout.addElement({ buffer["layout"][i].get<std::string>().c_str(), sdt });
					i += 2;
				}

				layout.calculateUniformBufferLayout();
				layout.recalculateStride();

				// Only need to provide the layout
				newBuffer = UniformBuffer::create(name, layout);

				// Register buffer with resource manager
				ResourceManager::registerResource(name, newBuffer);
				ENGINE_TRACE("Loaded {0} from {1}.", name, filePath);
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

		ENGINE_INFO("[ResourceLoader::loadFrameBuffers] Loading Framebuffers");

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
						AttachmentType attachment = BufferAttachments::convertStringToAttachment(buffer["layout"][i].get<std::string>());
						if (attachment == AttachmentType::None)
							ENGINE_ERROR("[ResourceLoader::loadFrameBuffers] Invalid attachment type provided. Name: {0}", name);

						// Format is: [ATTACHMENT TYPE], [SAMPLED], [ATTACHMENT TYPE], [SAMPLED], etc
						layout.addAttachment(attachment, buffer["layout"][i + 1].get<bool>());
						// Move i forward 2
						i += 2;
					}

					// We now need to calculate the dimensions, first check if we should use the window dimensions
					glm::ivec2 dimensions;

					// We can either use the dimensions of the scene view (starts as size of window) or enter them manually
					if (buffer["useSceneSize"].get<bool>())
					{
						auto window = WindowManager::getFocusedWindow();

						if (window)
							dimensions = WindowManager::getFocusedWindow()->getProperties().getSize();
						else
						{
							dimensions = glm::ivec2(buffer["size"][0], buffer["size"][1]);
							ENGINE_ERROR("[ResourceLoader::loadFrameBuffers] There isn't a valid focused window, cannot retrieve window dimensions.");
						}
					}
					else
						dimensions = glm::ivec2(buffer["size"][0], buffer["size"][1]);

					// Only need to provide the layout
					newBuffer = FrameBuffer::create(name, buffer["useSceneSize"].get<bool>(), dimensions, layout);
				}
				else
					newBuffer = FrameBuffer::createDefault(name);

				// Register buffer with resource manager
				ResourceManager::registerResource(name, newBuffer);
				ENGINE_TRACE("Loaded {0} from {1}.", name, filePath);
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

		ENGINE_INFO("[ResourceLoader::loadShaderPrograms] Loading Shader programs");

		// Go through each shader program and load it
		for (auto& shader : jsonData["programs"])
		{
			// Get the shader name
			std::string name = shader["name"].get<std::string>();

			// Check if it exists
			if (!ResourceManager::resourceExists(name))
			{
				// Now build the shader program
				const std::string vertexShader = shader["vertexShader"].get<std::string>();
				const std::string fragmentShader = shader["fragmentShader"].get<std::string>();
				const std::string geometryShader = shader["geometryShader"].get<std::string>();
				const std::string tessControlShader = shader["tessControlShader"].get<std::string>();
				const std::string tessEvaluationShader = shader["tessEvaluationShader"].get<std::string>();

				VertexArray* vArray = ResourceManager::getResource<VertexArray>(shader["vertexArray"].get<std::string>());
				if (!vArray)
				{
					ENGINE_ERROR("[ResourceLoader::loadShaderPrograms] The vertex array wasn't a valid array. Name: {0}", shader["vertexArray"].get<std::string>());
					return;
				}

				// Shader name doesn't exist, we can use it. Now let's load the shader
				ShaderProgram* newShader = ShaderProgram::create(name);

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

					// Uniform Block name must match the name in the shader program
					// Uniform buffer name is the buffer to provide that data
					ubos[shader["uniformBuffers"][i].get<std::string>()] = (ResourceManager::getResource<UniformBuffer>(shader["uniformBuffers"][i + 1].get<std::string>()));
					i += 2;
				}

				newShader->setUniforms(uniforms);
				newShader->setUniformBuffers(ubos);
				newShader->setOrderImportance(shader["orderImportance"].get<uint32_t>());

				// Register shader with resource manager
				ResourceManager::registerResource(name, newShader);
				ENGINE_TRACE("Loaded {0} from {1}.", name, filePath);
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

		ENGINE_INFO("[ResourceLoader::loadTextures] Loading Textures");

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
				TextureProperties properties(texture["width"], texture["height"],
					texture["wrapS"].get<std::string>(), texture["wrapT"].get<std::string>(), texture["wrapR"].get<std::string>(),
					texture["minFilter"].get<std::string>(), texture["maxFilter"].get<std::string>(),
					texture["gammaCorrect"].get<bool>(), texture["flipUV"].get<bool>());

				// Check if we're loading an image from file
				std::string filePath = texture["filePath"].get<std::string>().c_str();
				Texture2D* newTexture;

				// Either load an image into the texture or just leave texture blank
				if (filePath != "")
					newTexture = Texture2D::create(name, texture["filePath"].get<std::string>().c_str(), properties);
				else
					newTexture = Texture2D::create(name, properties, texture["channels"], nullptr);

				// Register texture with resource manager
				ResourceManager::registerResource(name, newTexture);
				ENGINE_TRACE("Loaded {0} from {1}.", name, filePath);
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
				ENGINE_TRACE("Loaded {0} from {1}.", name, filePath);
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

		ENGINE_INFO("[ResourceLoader::loadSubTextures] Loading Subtextures");

		// Go through each subtexture and load it
		for (auto& subTexture : jsonData["subTextures"])
		{
			// Get the subTexture name
			std::string name = subTexture["name"].get<std::string>();

			// Check if it exists
			if (!ResourceManager::resourceExists(name))
			{
				// Get the base texture name
				Texture2D* baseTexture = ResourceManager::getResource<Texture2D>(subTexture["baseTextureName"].get<std::string>());

				if (baseTexture)
				{
					// Create new subtexture
					// [Texture], [UVStart], [UVEnd]
					SubTexture* newSubTexture = new SubTexture(name, baseTexture, { subTexture["UVStart"][0], subTexture["UVStart"][1] }, { subTexture["UVEnd"][0], subTexture["UVEnd"][1] }, subTexture["ConvertBottomLeft"].get<bool>());

					// Register subTexture with resource manager
					ResourceManager::registerResource(name, newSubTexture);
					ENGINE_TRACE("Loaded {0} from {1}.", name, filePath);
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

		ENGINE_INFO("[ResourceLoader::load3DModels] Loading 3D Models");

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
				if (model["modelFilePath"].get<std::string>() != "")
				{
					AssimpLoader::loadModel(model["modelFilePath"].get<std::string>(), newModel->getMeshes());
				}

				if (newModel->getMeshes().size() == 0)
					ENGINE_ERROR("[ResourceLoader::load3DModels] The geometry was not successfully loaded. Name: {0}.", name);
				else
				{
					// Load the material file contents into a string
					std::string materialFileContents = "";
					std::string materialPath = model["materialFilePath"].get<std::string>();

					// If a valid path was given, load the file and read into it
					if (materialPath != "")
						materialFileContents = ResourceManager::getContents(materialPath);

					for (int i = 0; i < newModel->getMeshes().size(); i++)
					{
						if (materialFileContents != "")
						{
							std::string materialName = ResourceManager::getLineFromString(materialFileContents, i);
							if (materialName != "") newModel->getMeshes().at(i).setMaterial(ResourceManager::getResource<Material>(materialName));
							else newModel->getMeshes().at(i).setMaterial(ResourceManager::getResource<Material>("defaultMaterial3D"));
						}
						else
						{
							newModel->getMeshes().at(i).setMaterial(ResourceManager::getResource<Material>("defaultMaterial3D"));
						}
					}
				}

				// Register model with resource manager
				ResourceManager::registerResource(name, newModel);
				ENGINE_TRACE("Loaded {0} from {1}.", name, filePath);
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

		ENGINE_INFO("[ResourceLoader::loadMaterials] Loading Materials");

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
				std::vector<SubTexture*> subTextures;
				std::vector<CubeMapTexture*> cubeTextures;
				subTextures.reserve(ResourceManager::getConfigValue(Config::MaxSubTexturesPerMaterial));
				cubeTextures.reserve(ResourceManager::getConfigValue(Config::MaxSubTexturesPerMaterial));

				// Loop through all subtextures and add to the list
				for (auto& subTexture : material["subTextures"])
				{
					auto sub = ResourceManager::getResource<SubTexture>(subTexture.get<std::string>());

					if (sub)
						subTextures.push_back(sub);
					else
						ENGINE_ERROR("[ResourceLoader::loadMaterials] The resource manager does not contain a subtexture called {0}.", subTexture.get<std::string>());
				}

				// Loop through all cubemap textures and add to the list
				for (auto& cubeTexture : material["cubeMapTextures"])
				{
					auto cube = ResourceManager::getResource<CubeMapTexture>(cubeTexture.get<std::string>());

					if (cube)
						cubeTextures.push_back(cube);
					else
						ENGINE_ERROR("[ResourceLoader::loadMaterials] The resource manager does not contain a cubemap texture called {0}.", cubeTexture.get<std::string>());

					cubeTextures.push_back(cube);
				}

				ShaderProgram* shader = ResourceManager::getResource<ShaderProgram>(material["shader"].get<std::string>());

				if (!shader)
				{
					ENGINE_ERROR("[ResourceLoader::loadMaterials] The resource manager does not contain a shader program called {0} for material {1}.", material["shader"].get<std::string>(), name);
					continue;
				}

				newMaterial = new Material(name, subTextures, cubeTextures, shader,
					{ material["tint"][0], material["tint"][1], material["tint"][2], material["tint"][3] }, material["shininess"]);

				// Register model with resource manager
				ResourceManager::registerResource(name, newMaterial);
				ENGINE_TRACE("Loaded {0} from {1}.", name, filePath);
			}
			else
				ENGINE_ERROR("[ResourceLoader::loadMaterials] Resource name already taken. Name: {0}", name);
		}
	}
}
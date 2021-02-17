/*! \file sceneLoader.cpp
*
* \brief A scene loader class which loads the scene from a JSON file.
*
* \author Daniel Bullin
*
*/
#include "loaders/sceneLoader.h"
#include "independent/utils/resourceLoader.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/log.h"

#include "entities/player.h"
#include "entities/cyborg.h"
#include "entities/rectangleShape.h"
#include "entities/FPSCounter.h"
#include "entities/mainMenu/menuText.h"

#include "layers/defaultLayer.h"
#include "layers/UILayer.h"

#include "independent/rendering/renderPasses/passes/firstPass.h"
#include "independent/rendering/renderPasses/passes/secondPass.h"
#include "independent/rendering/renderPasses/passes/menuPass.h"

namespace Engine
{
	using json = nlohmann::json; //!< Type alias

	//! loadResources()
	/*!
	\param sceneFolderPath a const std::string& - The scene data folder path
	*/
	void SceneLoader::loadResources(const std::string& resourceFolderPath)
	{
		// Load all the various resources with the Engine's resource loader
		// resourceFolderPath takes us to the scene's root folder
		ResourceLoader::loadVertexBuffers(resourceFolderPath + "GraphicalObjects/vertexBuffers.json");
		ResourceLoader::loadIndexBuffers(resourceFolderPath + "GraphicalObjects/indexBuffers.json");
		ResourceLoader::loadVertexArrays(resourceFolderPath + "GraphicalObjects/vertexArrays.json");
		ResourceLoader::loadIndirectBuffers(resourceFolderPath + "GraphicalObjects/indirectBuffers.json");
		ResourceLoader::loadUniformBuffers(resourceFolderPath + "GraphicalObjects/uniformBuffers.json");
		ResourceLoader::loadFrameBuffers(resourceFolderPath + "GraphicalObjects/frameBuffers.json");
		ResourceLoader::loadShaderPrograms(resourceFolderPath + "shaders.json");
		ResourceLoader::loadTextures(resourceFolderPath + "textures.json");
		ResourceLoader::loadSubTextures(resourceFolderPath + "subTextures.json");
		ResourceLoader::loadMaterials(resourceFolderPath + "materials.json");
		ResourceLoader::load3DModels(resourceFolderPath + "models.json");
	}

	//! createNewEntity()
	/*!
	\param entitySubType a const std::string& - The name of the entity subclass
	\return an Entity* - A pointer to the entity subclass
	*/
	Entity* SceneLoader::createNewEntity(const std::string& entitySubType)
	{
		// All subclasses of Entity must be added and returned here
		if (entitySubType == "Player")
			return new Player;
		else if (entitySubType == "Cyborg")
			return new Cyborg;
		else if (entitySubType == "Rectangle")
			return new RectangleShape;
		else if (entitySubType == "FPSCounter")
			return new FPSCounter;
		else if (entitySubType == "MenuText")
			return new MenuText;
		else
			return nullptr;
	}

	//! createLayer()
	/*!
	\param layerName a const std::string& - The name of the layer
	\return a Layer* - A pointer to the layer subclass
	*/
	Layer* SceneLoader::createLayer(const std::string& layerName)
	{
		// All subclasses of layer must be added and returned here
		if (layerName == "Default")
			return new DefaultLayer;
		else if (layerName == "UI")
			return new UILayer;
		else
			return nullptr;
	}

	//! createRenderPass()
	/*!
	\param passName a const std::string& - The name of the render pass
	\return a RenderPass* - A pointer to the render pass subclass
	*/
	RenderPass* SceneLoader::createRenderPass(const std::string& passName)
	{
		// All subclasses of layer must be added and returned here
		if (passName == "FirstPass")
			return new FirstPass;
		else if (passName == "SecondPass")
			return new SecondPass;
		else if (passName == "MenuPass")
			return new MenuPass;
		else
			return nullptr;
	}

	//! load()
	/*!
	\param sceneName a const std::string& - The name of the scene
	\param sceneFolderPath a const std::string& - The scene data folder path
	*/
	void SceneLoader::load(const std::string& sceneName, const std::string& sceneFolderPath)
	{
		// Check if scene already exists, if it does just exit out of this function
		if (SceneManager::sceneExists(sceneName))
		{
			ENGINE_ERROR("[SceneLoader::load] Scene already exists. Name: {0}.", sceneName);
			return;
		}

		// Create scene
		Scene* scene = SceneManager::createScene(sceneName);
		// Load all resources associated with this scene
		loadResources(sceneFolderPath);
		// Set scene clear colour
		json sceneData = ResourceManager::getJSON(sceneFolderPath + "config.json");
		
		// Load layers
		for (auto& layer : sceneData["layers"])
			scene->getLayerManager()->attachLayer(createLayer(layer["name"].get<std::string>()));
		// Load Render passes
		for (auto& pass : sceneData["renderPasses"])
		{
			RenderPass* passInstance = createRenderPass(pass["name"].get<std::string>());
			passInstance->setEnabled(pass["enabled"].get<bool>());
			scene->addRenderPass(passInstance);
		}

		// Load entities
		sceneData = ResourceManager::getJSON(sceneFolderPath + "entities.json");

		// Go through each entity and add its components
		for (auto& entity : sceneData["entities"])
		{
			// First check if entity name already exists
			if (!scene->checkEntityNameTaken(entity["name"].get<std::string>().c_str()))
			{
				// If not, create new entity
				Entity* newEntity;

				// Create new entity based on subclass name
				newEntity = createNewEntity(entity["type"].get<std::string>());

				// An invalid entity sub class was provided, exit out
				if (!newEntity)
				{
					ENGINE_ERROR("[SceneLoader::load] Entity subclass provided wasn't a valid entity type. Type: {0}.", entity["type"].get<std::string>());
					continue;
				}

				// Add entity to the scene
				scene->addEntity(entity["name"].get<std::string>().c_str(), newEntity);

				// Set the entity's layer
				newEntity->setLayer(scene->getLayerManager()->getLayer(entity["layer"].get<std::string>()));

				newEntity->setDisplay(entity["display"].get<bool>());

				// Go through each component found in the file and add it to the entity
				for (auto& component : entity["components"])
				{
					std::string compName = component["name"].get<std::string>();

					// Check component name is free
					if (newEntity->getAllComponents().find(compName) == newEntity->getAllComponents().end())
					{
						ComponentType componentType = Entity::convertComponentClassType(component["type"].get<std::string>());

						switch (componentType)
						{
							case ComponentType::Camera:
							{
								// Camera Data:
								// [PosX, PosY, PosZ], [FrontX, FrontY, FrontZ], [UpX, UpY, UpZ], [WorldUpX, WorldupY, WorldUpZ],
								// yaw, pitch, speed, sensitivity, zoom
								CameraData camData(
									{ component["localPosition"][0], component["localPosition"][1], component["localPosition"][2] }, // Pos
									{ component["front"][0],		 component["front"][1],			component["front"][2] }, // Pos
									{ component["up"][0],			 component["up"][1],			component["up"][2] }, // Pos
									{ component["worldUp"][0],		 component["worldUp"][1],		component["worldUp"][2] }, // Pos
									component["Yaw"],
									component["Pitch"],
									component["Speed"],
									component["Sensitivity"],
									component["Zoom"]
								);

								newEntity->attach<Camera>(compName.c_str(), camData);

								newEntity->getComponent<Camera>(compName.c_str())->setClearColour({ component["clearColour"][0], component["clearColour"][1] , component["clearColour"][2] , component["clearColour"][3] });

								if (component["skybox"].size() != 0)
								{
									newEntity->getComponent<Camera>(compName.c_str())->setSkybox(new Skybox(ResourceManager::getResourceAndRef<Model3D>(component["skybox"][0]["model"].get<std::string>()), ResourceManager::getResourceAndRef<Material>(component["skybox"][0]["material"].get<std::string>())));
								}

								// Camera is main camera, set the scene's main camera to this
								if (component["setMainCamera"].get<bool>())
									scene->setMainCamera(static_cast<Camera*>(newEntity->getComponent<Camera>(compName.c_str())));

								break;
							}
							case ComponentType::Transform3D:
							{
								// Add transform component
								newEntity->attach<Transform3D>(compName.c_str(),
									component["position"][0], component["position"][1], component["position"][2],
									component["rotation"][0], component["rotation"][1], component["rotation"][2],
									component["scale"][0], component["scale"][1], component["scale"][2]
									);

								break;
							}
							case ComponentType::Transform2D:
							{
								// Add transform component
								newEntity->attach<Transform2D>(compName.c_str(),
									component["position"][0], component["position"][1],
									component["rotation"],
									component["scale"][0], component["scale"][1]
									);

								break;
							}
							case ComponentType::Text:
							{
								glm::vec4 tint = { component["colour"][0], component["colour"][1], component["colour"][2], component["colour"][3] };
								// Add text component
								newEntity->attach<Text>(compName.c_str(),
									component["text"].get<std::string>().c_str(),
									tint,
									component["fontName"].get<std::string>().c_str(),
									glm::vec3(component["localPosition"][0], component["localPosition"][1], component["localPosition"][2]),
									component["localRotation"],
									glm::vec2(component["localScale"][0], component["localScale"][1])
									);

								break;
							}
							case ComponentType::EventListener:
							{
								newEntity->attach<EventListener>(compName.c_str());
								break;
							}
							case ComponentType::MeshRender3D:
							{
								// Add mesh render component
								// [LocalPosition, LocalRotation, LocalScale, Model3D, Material]
								newEntity->attach<MeshRender3D>(compName.c_str(),
									glm::vec3(component["localPosition"][0], component["localPosition"][1], component["localPosition"][2]),
									glm::vec3(component["localRotation"][0], component["localRotation"][1], component["localRotation"][2]),
									glm::vec3(component["localScale"][0], component["localScale"][1], component["localScale"][2]),
									ResourceManager::getResourceAndRef<Model3D>(component["modelName"].get<std::string>()),
									ResourceManager::getResourceAndRef<Material>(component["materialName"].get<std::string>())
									);

								break;
							}
							case ComponentType::MeshRender2D:
							{
								// Add mesh render component
								// [LocalPosition, LocalRotation, LocalScale, Material]
								newEntity->attach<MeshRender2D>(compName.c_str(),
									glm::vec3(component["localPosition"][0], component["localPosition"][1], component["localPosition"][2]),
									component["localRotation"],
									glm::vec2(component["localScale"][0], component["localScale"][1]),
									ResourceManager::getResourceAndRef<Material>(component["materialName"].get<std::string>())
									);

								break;
							}
						}
					}
					else
						ENGINE_ERROR("[SceneLoader::load] Component name is already taken. Name: {0}.", compName);
				}
			}
		}
	}
}
/*! \file sceneLoader.cpp
*
* \brief A scene loader class which loads the scene from a JSON file.
*
* \author Daniel Bullin
*
*/
#include "loaders/sceneLoader.h"
#include "independent/systems/systems/sceneManager.h"

#include "scripts/gameObjects/player.h"
#include "scripts/gameObjects/cyborg.h"
#include "scripts/menus/mainMenuText.h"
#include "scripts/menus/pauseMenuText.h"
#include "scripts/menus/settingsMenuText.h"
#include "scripts/menus/settings/keybindText.h"
#include "scripts/gameObjects/staticLight.h"
#include "scripts/loading/resourcesScript.h"
#include "scripts/mainMenu/cameraRotationScript.h"
#include "scripts/menus/pauseScript.h"
#include "scripts/menus/inventoryElement.h"
#include "scripts/menus/settingsScript.h"
#include "scripts/menus/inventoryMenu.h"
#include "scripts/engineScript.h"
#include "scripts/testing/testScript.h"
#include "scripts/FPSCounter.h"
#include "scripts/gameTimer.h"
#include "scripts/gameObjects/terrain.h"
#include "scripts/gameObjects/placeObject.h"
#include "scripts/gameObjects/environment.h"
#include "scripts/gameObjects/water.h"
#include "scripts/gameObjects/sun.h"

#include "layers/defaultLayer.h"
#include "layers/UILayer.h"
#include "layers/pauseLayer.h"
#include "layers/settingsLayer.h"
#include "layers/inventoryLayer.h"

#include "independent/rendering/renderPasses/passes/firstPass.h"
#include "independent/rendering/renderPasses/passes/secondPass.h"
#include "independent/rendering/renderPasses/passes/thirdPass.h"
#include "independent/rendering/renderPasses/passes/fourthPass.h"
#include "independent/rendering/renderPasses/passes/UIPass.h"
#include "independent/rendering/renderPasses/passes/blurPass.h"
#include "independent/rendering/renderPasses/passes/waterPass.h"

namespace Engine
{
	//! createNewScript()
	/*!
	\param scriptName a const std::string& - The name of the script
	\return a NativeScript* - A pointer to the script
	*/
	NativeScript* SceneLoader::createNewScript(const std::string& scriptName, nlohmann::json scriptData)
	{
		// All subclasses of Entity must be added and returned here
		// All subclasses of Entity must be added and returned here
		if (scriptName == "Player") return new Player;
		else if (scriptName == "Cyborg") return new Cyborg;
		else if (scriptName == "FPSCounter") return new FPSCounter;
		else if (scriptName == "MainMenuText") return new MainMenuText;
		else if (scriptName == "PauseMenuText") return new PauseMenuText;
		else if (scriptName == "SettingsMenuText") return new SettingsMenuText;
		else if (scriptName == "StaticLight") return new StaticLight;
		else if (scriptName == "CameraRotationScript") return new CameraRotationScript;
		else if (scriptName == "ResourcesScript") return new ResourcesScript;
		else if (scriptName == "PauseScript") return new PauseScript;
		else if (scriptName == "SettingsScript") return new SettingsScript;
		else if (scriptName == "EngineScript") return new EngineScript;
		else if (scriptName == "KeybindText")
		{
			if (scriptData.empty())
				return new KeybindText;
			else
				return new KeybindText(scriptData);
		}
		else if (scriptName == "TestScript") return new TestScript;
		else if (scriptName == "Terrain") return new Terrain;
		else if (scriptName == "Water") return new Water;
		else if (scriptName == "PlaceObject") return new PlaceObject;
		else if (scriptName == "Environment") return new Environment;
		else if (scriptName == "GameTimer") return new GameTimer;
		else if (scriptName == "Sun") return new Sun;
		else if (scriptName == "InventoryMenu") return new InventoryMenu;
		else if (scriptName == "InventoryElement")
		{
			if (scriptData.empty())
				return new InventoryElement;
			else
				return new InventoryElement(scriptData);
		}
		return nullptr;
	}

	//! createLayer()
	/*!
	\param layerName a const std::string& - The name of the layer
	\return a Layer* - A pointer to the layer subclass
	*/
	Layer* SceneLoader::createLayer(const std::string& layerName)
	{
		// All subclasses of layers should be return here from a string
		if (layerName == "Default") return new DefaultLayer;
		else if (layerName == "UI") return new UILayer;
		else if (layerName == "Settings") return new SettingsLayer;
		else if (layerName == "Pause") return new PauseLayer;
		else if (layerName == "Inven") return new InventoryLayer;
		else return nullptr;
	}

	//! createRenderPass()
	/*!
	\param passName a const std::string& - The name of the render pass
	\return a RenderPass* - A pointer to the render pass subclass
	*/
	RenderPass* SceneLoader::createRenderPass(const std::string& passName)
	{
		// All subclasses of RenderPass should be return here from a string
		if (passName == "FirstPass") return new FirstPass;
		else if (passName == "SecondPass") return new SecondPass;
		else if (passName == "ThirdPass") return new ThirdPass;
		else if (passName == "FourthPass") return new FourthPass;
		else if (passName == "UIPass") return new UIPass;
		else if (passName == "BlurPass") return new BlurPass;
		else if (passName == "WaterPass") return new WaterPass;
		else return nullptr;
	}

	//! loadSceneProperties()
	/*
	\param scene a Scene* - A pointer to the scene we're loading
	\param sceneData a json - The scene's JSON data
	\return a bool - The success value
	*/
	bool SceneLoader::loadSceneProperties(Scene* scene, json sceneData)
	{
		if (scene)
		{
			// Load layers
			for (auto& layer : sceneData["layers"])
			{
				Layer* newLayer = createLayer(layer["name"].get<std::string>());

				if (newLayer)
				{
					newLayer->setActive(layer["active"].get<bool>());
					newLayer->setDisplayed(layer["displayed"].get<bool>());
					scene->getLayerManager()->attachLayer(newLayer);
				}
				else
				{
					ENGINE_ERROR("[SceneLoader::loadSceneProperties] An invalid layer was provided for this scene. Cannot add. Scene Name: {0}, Layer Name: {1}.", scene->getName(), layer["name"].get<std::string>());
					return false;
				}
			}

			// Load Render passes
			for (auto& pass : sceneData["renderPasses"])
			{
				RenderPass* newPass = createRenderPass(pass["name"].get<std::string>());

				if (newPass)
				{
					newPass->setEnabled(pass["enabled"].get<bool>());
					scene->addRenderPass(newPass);
				}
				else
				{
					ENGINE_ERROR("[SceneLoader::loadSceneProperties] An invalid render pass was provided for this scene. Cannot add. Scene Name: {0}, Pass Name: {1}.", scene->getName(), pass["name"].get<std::string>());
					return false;
				}
			}
			return true;
		}
		else
			ENGINE_ERROR("[SceneLoader::loadSceneProperties] Cannot construct because the scene is invalid.");
		return false;
	}

	//! loadSceneProperties()
	/*
	\param entity a Entity* - A pointer to the entity we're loading
	\param entityData a json - The entity's JSON data
	\return a bool - The success value
	*/
	bool SceneLoader::loadEntityProperties(Entity* entity, json entityData)
	{
		if (entity)
		{
			LayerManager* layerManager = entity->getParentScene()->getLayerManager();

			if (layerManager)
			{
				entity->setLayer(layerManager->getLayer(entityData["layer"].get<std::string>()));
				entity->setDisplay(entityData["display"].get<bool>());
				return true;
			}
			else
				ENGINE_ERROR("[SceneLoader::loadEntityProperties] Cannot construct because the layer manager is invalid. Entity Name: {0}.", entity->getName());
		}
		else
			ENGINE_ERROR("[SceneLoader::loadEntityProperties] Cannot construct because the entity is invalid.");

		return false;
	}

	//! loadEntity()
	/*
	\param scene a Scene* - A pointer to the scene
	\param parent an Entity* - A pointer to the parent entity
	\param entityData a json - The entity's JSON data
	*/
	void SceneLoader::loadEntity(Scene* scene, Entity* parent, json entityData)
	{
		std::string entityName = entityData["name"].get<std::string>();

		Entity* newEntity = new Entity;

		if (parent)
			parent->addChildEntity(entityName, newEntity);
		else
			scene->addEntity(entityName, newEntity);

		bool entityCoreSuccess = loadEntityProperties(newEntity, entityData);
		loadComponents(scene, newEntity, entityData["components"]);

		for (auto& child : entityData["childEntities"])
		{
			if (!newEntity->checkChildEntityNameTaken(child["name"].get<std::string>()))
			{
				loadEntity(scene, newEntity, child);
			}
			else
				ENGINE_ERROR("[SceneLoader::loadEntity] Child name already taken. Parent: {0}, Child: {1}.", entityName, child["name"].get<std::string>());
		}
	}

	//! loadComponents
	/*
	\param scene a Scene* - A pointer to the scene
	\param entity an Entity* - A pointer to an entity
	\param componentData a json - The json data for the components
	*/
	void SceneLoader::loadComponents(Scene* scene, Entity* entity, json componentData)
	{
		/////
		// Loading all components
		/////

		for (auto& component : componentData)
		{
			std::string compName = component["name"].get<std::string>();
			ComponentType componentType = Components::toType(component["type"].get<std::string>());

			switch (componentType)
			{
			case ComponentType::Camera:
			{
				CameraData camData({ component["front"][0], component["front"][1], component["front"][2] }, { component["up"][0], component["up"][1], component["up"][2] }, { component["worldUp"][0], component["worldUp"][1], component["worldUp"][2] }, component["Yaw"], component["Pitch"], component["Zoom"]);
				entity->attach<Camera>(compName, camData);
				entity->getComponent<Camera>()->setClearColour({ component["clearColour"][0], component["clearColour"][1] , component["clearColour"][2] , component["clearColour"][3] });

				// Load the cameras skybox if it has one
				if (component["skybox"].size() != 0)
					entity->getComponent<Camera>()->setSkybox(new Skybox(ResourceManager::getResource<Model3D>(component["skybox"][0]["model"].get<std::string>()), ResourceManager::getResource<Material>(component["skybox"][0]["material"].get<std::string>())));

				// Camera is main camera, set the scene's main camera to this
				if (component["setMainCamera"].get<bool>())
					scene->setMainCamera(entity->getComponent<Camera>());

				break;
			}
			case ComponentType::Transform:
			{
				entity->attach<Transform>(compName, component["position"][0], component["position"][1], component["position"][2], component["rotation"][0], component["rotation"][1], component["rotation"][2], component["scale"][0], component["scale"][1], component["scale"][2]);
				break;
			}
			case ComponentType::Text:
			{
				entity->attach<Text>(compName, component["text"].get<std::string>(), glm::vec4(component["colour"][0], component["colour"][1], component["colour"][2], component["colour"][3]), component["fontName"].get<std::string>());
				break;
			}
			case ComponentType::CharacterController:
			{
				entity->attach<CharacterController>(compName, component["speed"], component["sensitivity"], component["frozen"].get<bool>());
				break;
			}
			case ComponentType::MeshRender3D:
			{
				entity->attach<MeshRender3D>(compName, ResourceManager::getResource<Model3D>(component["modelName"].get<std::string>()), ResourceManager::getResource<Material>(component["materialName"].get<std::string>()));
				break;
			}
			case ComponentType::MeshRender2D:
			{
				entity->attach<MeshRender2D>(compName, ResourceManager::getResource<Material>(component["materialName"].get<std::string>()));
				break;
			}
			case ComponentType::PointLight:
			{
				entity->attach<PointLight>(compName, glm::vec3(component["ambient"][0], component["ambient"][1], component["ambient"][2]), glm::vec3(component["diffuse"][0], component["diffuse"][1], component["diffuse"][2]), glm::vec3(component["specular"][0], component["specular"][1], component["specular"][2]), component["constant"], component["linear"], component["quadratic"]);
				break;
			}
			case ComponentType::SpotLight:
			{
				entity->attach<SpotLight>(compName, glm::vec3(component["direction"][0], component["direction"][1], component["direction"][2]), glm::vec3(component["ambient"][0], component["ambient"][1], component["ambient"][2]), glm::vec3(component["diffuse"][0], component["diffuse"][1], component["diffuse"][2]), glm::vec3(component["specular"][0], component["specular"][1], component["specular"][2]), component["cutOff"], component["outerCutOff"], component["constant"], component["linear"], component["quadratic"]);
				break;
			}
			case ComponentType::DirectionalLight:
			{
				entity->attach<DirectionalLight>(compName, glm::vec3(component["direction"][0], component["direction"][1], component["direction"][2]), glm::vec3(component["ambient"][0], component["ambient"][1], component["ambient"][2]), glm::vec3(component["diffuse"][0], component["diffuse"][1], component["diffuse"][2]), glm::vec3(component["specular"][0], component["specular"][1], component["specular"][2]));
				break;
			}
			case ComponentType::UIElement:
			{
				entity->attach<UIElement>(compName, glm::vec2(component["anchor"][0], component["anchor"][1]), glm::vec2(component["offset"][0], component["offset"][1]), glm::vec2(component["scale"][0], component["scale"][1]), component["useAbsoluteSize"].get<bool>());
				break;
			}
			case ComponentType::NativeScript:
			{
				NativeScript* script = createNewScript(component["scriptName"].get<std::string>(), component);
				script->setName(compName);

				if (script)
					entity->attach<NativeScript>(script);
				else
					ENGINE_ERROR("[SceneLoader::loadComponents] The script name provided isn't a valid script name. Script: {0}.", component["scriptName"].get<std::string>());

				break;
			}
			default:
			{
				ENGINE_ERROR("[SceneLoader::loadComponents] An invalid component type was provided. Entity Name: {0}. Type: {1}.", entity->getName(), component["type"].get<std::string>());
				break;
			}
			}
		}
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

		/////
		// Create a new scene
		/////
		Scene* scene = SceneManager::createScene(sceneName, sceneFolderPath);

		if (scene)
		{
			/////
			// Loading Core Scene
			/////
			json sceneData = ResourceManager::getJSON(sceneFolderPath + "config.json");
			bool coreLoadSuccess = loadSceneProperties(scene, sceneData);

			loadEntities(scene, sceneFolderPath + "entities.json");
		}
		else
			ENGINE_ERROR("[SceneLoader::load] An invalid scene was created. Scene Name: {0}.", sceneName);
	}

	//! loadEntities()
	/*!
	\param scene a Scene* - A pointer to the scene
	\param entityFilePath a const std::string& - The filepath to the entity json file to load
	*/
	void SceneLoader::loadEntities(Scene* scene, const std::string& entityFilePath)
	{
		/////
		// Loading Entities
		/////
		json sceneData = ResourceManager::getJSON(entityFilePath);

		// Go through each entity and add its components
		for (auto& rootEntity : sceneData["entities"])
		{
			std::string rootEntityName = rootEntity["name"].get<std::string>();
			if (!scene->checkRootEntityNameTaken(rootEntityName))
			{
				loadEntity(scene, nullptr, rootEntity);
			}
			else
				ENGINE_ERROR("[SceneLoader::loadEntities] This root entity name is already taken. Name: {0}.", rootEntityName);
		}
	}
}
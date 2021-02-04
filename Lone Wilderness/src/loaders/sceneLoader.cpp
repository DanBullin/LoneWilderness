/*! \file sceneLoader.cpp
*
* \brief A scene loader class which loads the scene from a JSON file.
*
* \author Daniel Bullin
*
*/

#include "loaders/sceneLoader.h"
#include "independent/systems/systemManager.h"

#include "entities/player.h"
#include "entities/cyborg.h"
#include "entities/rectangleShape.h"
#include "entities/FPSCounter.h"

using json = nlohmann::json; //!< Type alias

//! createNewEntity()
/*!
\param entitySubType a const std::string& - The name of the entity subclass
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
	else
		return nullptr;
}

//! load()
/*!
\param scene a Scene* - A scene pointer 
\param sceneFolderPath a const std::string& - The scene data folder path
*/
void SceneLoader::load(Scene* scene, const std::string& sceneFolderPath)
{
	// Load entity data into JSON file
	json sceneData = ResourceManager::getJSON(sceneFolderPath + "entities.json");

	// Go through each entity and add its components
	for (auto& entity : sceneData["entities"])
	{
		// First check if entity name already exists
		if (!scene->checkEntityNameTaken(entity["name"].get<std::string>().c_str()))
		{
			// If not, create new entity and add to the scene
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

			newEntity->setLayer(scene->getLayerManager()->getLayer(entity["layer"].get<std::string>()));

			// Go through each component found in the file and add it to the entity
			for (auto& component : entity["components"])
			{
				std::string compName = component["name"].get<std::string>();

				// Check component name is free
				if (newEntity->getAllComponents().find(compName) == newEntity->getAllComponents().end())
				{
					std::string componentType = component["type"].get<std::string>();

					// If camera comp, create the camera data and check if its the main camera
					if (componentType == "Camera")
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

						// Camera is main camera, set the scene's main camera to this
						if (component["setMainCamera"].get<bool>())
							scene->setMainCamera(static_cast<Camera*>(newEntity->getComponent<Camera>(compName.c_str())));
					}
					else if (componentType == "Transform3D")
					{
						// Add transform component
						newEntity->attach<Transform3D>(compName.c_str(), 
							component["position"][0], component["position"][1], component["position"][2],
							component["rotation"][0], component["rotation"][1], component["rotation"][2],
							component["scale"][0], component["scale"][1], component["scale"][2]
							);
					}
					else if (componentType == "Transform2D")
					{
						// Add transform component
						newEntity->attach<Transform2D>(compName.c_str(),
							component["position"][0], component["position"][1],
							component["rotation"],
							component["scale"][0], component["scale"][1]
							);
					}
					else if (componentType == "Text")
					{
						glm::vec4 tint = { component["colour"][0], component["colour"][1], component["colour"][2], component["colour"][3] };
						// Add text component
						newEntity->attach<Text>(compName.c_str(),
							component["text"].get<std::string>().c_str(),
							tint,
							component["fontName"].get<std::string>().c_str(),
							glm::vec2(component["localPosition"][0], component["localPosition"][1]),
							component["localRotation"],
							glm::vec2(component["localScale"][0], component["localScale"][1])
							);
					}
					else if (componentType == "EventListener")
					{
						// Add mesh render component
						// [Model, Material]
						newEntity->attach<EventListener>(compName.c_str());
					}
					else if (componentType == "MeshRender3D")
					{
						// Add mesh render component
						// [Model, Material]
						newEntity->attach<MeshRender3D>(compName.c_str(),
							glm::vec3(component["localPosition"][0], component["localPosition"][1], component["localPosition"][2]),
							glm::vec3(component["localRotation"][0], component["localRotation"][1], component["localRotation"][2]),
							glm::vec3(component["localScale"][0], component["localScale"][1], component["localScale"][2]),
							ResourceManager::get3DModel(component["modelName"].get<std::string>().c_str()),
							ResourceManager::getMaterial(component["materialName"].get<std::string>().c_str())
							);
					}
					else if (componentType == "MeshRender2D")
					{
						// Add mesh render component
						// [Model, Material]
						newEntity->attach<MeshRender2D>(compName.c_str(),
							glm::vec2(component["localPosition"][0], component["localPosition"][1]),
							component["localRotation"],
							glm::vec2(component["localScale"][0], component["localScale"][1]),
							ResourceManager::getMaterial(component["materialName"].get<std::string>().c_str())
							);
					}
				}
				else
					ENGINE_ERROR("[SceneLoader::load] Component name is already taken. Name: {0}.", compName);
			}
		}
	}
}
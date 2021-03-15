/*! \file scene.cpp
*
* \brief A scene contains all the data associated in a particular environment. This data will be loaded by the SceneLoader.
*
* \author Daniel Bullin
*
*/
#include "independent/systems/components/scene.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/windowManager.h"

namespace Engine
{
	//! Scene()
	/*!
	\param name a const std::string& - The name of the scene
	\param sceneFolderPath a const std::string& - The file path to the folder of the scene
	*/
	Scene::Scene(const std::string& name, const std::string& sceneFolderPath)
	{
		m_sceneName = name;
		m_sceneFolderPath = sceneFolderPath;
		m_layerManager = new LayerManager(this);
		m_renderPasses.reserve(ResourceManager::getConfigValue(Config::MaxRenderPassesPerScene));
		m_mainCamera = nullptr;
		m_entityListUpdated = true;

		// Print the scene's details upon creation
		ENGINE_TRACE("[Scene::Scene] Scene details upon creation for: {0}.", m_sceneName);
		printSceneDetails();
	}

	//! ~Scene()
	Scene::~Scene()
	{
		ENGINE_INFO("[Scene::~Scene] Destroying scene: {0}.", m_sceneName);

		// If there is any entities in the scene, delete them
		if (m_rootEntities.size() != 0)
		{
			// Delete all root entities which will delete all children too
			for (auto& rootEntity : m_rootEntities)
				delete rootEntity.second;

			m_rootEntities.clear();
		}

		// If there is a valid layer manager, delete it
		if (m_layerManager)
		{
			delete m_layerManager;
			m_layerManager = nullptr;
		}

		// If there are valid render passes, delete them
		if (m_renderPasses.size() != 0)
		{
			for (auto& renderPass : m_renderPasses)
				if (renderPass)
				{
					renderPass->onDetach();
					delete renderPass;
				}

			m_renderPasses.clear();
		}

		// Camera pointer is deleted by the entity it is attached to, so we only need to set the pointer
		if (m_mainCamera) m_mainCamera = nullptr;

		m_entityListUpdated = true;
		m_entitiesList.clear();
	}

	//! getName()
	/*!
	\return a const std::string& - The name of the scene
	*/
	const std::string& Scene::getName()
	{
		return m_sceneName;
	}

	//! getFolderPath()
	/*!
	\return a const std::string& - The file path to the folder of the scene
	*/
	const std::string& Scene::getFolderPath()
	{
		return m_sceneFolderPath;
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The update time step
	\param totalTime a const float - The total application time
	*/
	void Scene::onUpdate(const float timestep, const float totalTime)
	{
		// Delete any entities that are scheuled to be deleted, can also update components if entity is not to be deleted
		for (auto it = m_rootEntities.cbegin(); it != m_rootEntities.cend(); )
		{
			// Check if entity is a valid entity
			if (it->second)
			{
				// Check if the entity is to be destroyed
				if (it->second->getDestroyed())
				{
					// Do NOT delete the entity which contains the main camera
					if (it->second == m_mainCamera->getParent())
					{
						ENGINE_ERROR("[Scene::onUpdate] Cannot delete the entity when it contains the scene's main camera. Scene: {0}, Entity: {1}.", m_sceneName, it->second->getName());
						it->second->undestroy();
						return;
					}

					// Check for validness again and then delete
					if (it->second) delete it->second;
					m_rootEntities.erase(it++);
					setEntityListUpdated(true);
				}
				else
				{
					// Entity is not to be deleted, update components
					if (it->second)
					{
						it->second->onUpdate(timestep, totalTime);
						for (auto& component : it->second->getAllComponents())
						{
							// Check if component is valid
							if (component)
								component->onUpdate(timestep, totalTime);
						}
						++it;
					}
				}
			}
		}

		// Update the layer manager if it is valid
		if (m_layerManager)
			m_layerManager->onUpdate(timestep, totalTime);
	}

	//! addEntity()
	/*!
	\param name a const std::string& - The name of the entity
	\param entity an Entity* - A pointer to the entity
	*/
	void Scene::addEntity(const std::string& name, Entity* entity)
	{
		if (!checkRootEntityNameTaken(name) && name != "")
		{
			// If an invalid entity was provided, just exit out the function
			if (!entity)
			{
				ENGINE_ERROR("[Scene:addEntity] An invalid entity pointer was provided for entity name: {0} for scene: {1}.", name, m_sceneName);
				return;
			}

			// Set the parent scene of the entity and its name
			m_rootEntities[name] = entity;
			entity->setParentScene(this);
			entity->setParentEntity(nullptr);
			entity->setName(name);
			setEntityListUpdated(true);
		}
		else
			ENGINE_ERROR("[Scene::addEntity] Name for entity already taken. Cannot add. Entity Name: {0} for scene: {1}.", name, m_sceneName);
	}

	//! getEntity()
	/*!
	\param name a const std::string& - The name of the entity
	\return an Entity* - A pointer to the entity
	*/
	Entity* Scene::getEntity(const std::string& name)
	{
		// Check if entity name exists
		if (checkRootEntityNameTaken(name) && name != "")
		{
			if (!m_rootEntities[name])
				ENGINE_ERROR("[Scene::getEntity] The entity we're retrieving is an invalid entity. Scene Name: {0}", m_sceneName);

			return m_rootEntities[name];
		}
		else
			ENGINE_ERROR("[Scene::getEntity] Cannot get entity named: {0}.", name);

		// Can't find entity
		return nullptr;
	}

	//! getEntities()
	/*!
	\return a std::vector<Entity*> - A list of all entities in the scene
	*/
	std::vector<Entity*> Scene::getEntities()
	{
		// A list of all the entities in this scene
		if (m_entityListUpdated)
		{
			// The entities in this scene has been updated
			// Clear the rendering entity list and refill it
			m_entitiesList.clear();

			for (auto& entity : m_rootEntities)
			{
				// Only add entity if its valid
				if (entity.second)
				{
					std::vector<Entity*> entList;
					entity.second->getAllEntities(entList);
					m_entitiesList.push_back(entity.second);
					m_entitiesList.insert(m_entitiesList.end(), entList.begin(), entList.end());
				}
			}

			m_entityListUpdated = false;
		}
		return m_entitiesList;
	}

	//! getClosestPointLights()
	/*
	\return a std::vector<PointLight*> - A list of point lights sorted by distance from main camera
	*/
	std::vector<PointLight*> Scene::getClosestPointLights()
	{
		std::vector<PointLight*> pointLights;

		for (auto& entity : getEntities())
		{
			auto pointLight = entity->getComponent<PointLight>();
			if (pointLight) pointLights.emplace_back(pointLight);
		}

		std::sort(pointLights.begin(), pointLights.end(),
			[=](PointLight*& a, PointLight*& b)
		{
			return glm::distance(a->getWorldPosition(), getMainCamera()->getWorldPosition()) < glm::distance(b->getWorldPosition(), getMainCamera()->getWorldPosition());
		}
		);

		return pointLights;
	}

	//! getClosestSpotLights()
	/*
	\return a std::vector<PointLight*> - A list of spot lights sorted by distance from main camera
	*/
	std::vector<SpotLight*> Scene::getClosestSpotLights()
	{
		std::vector<SpotLight*> spotLights;

		for (auto& entity : getEntities())
		{
			auto spotLight = entity->getComponent<SpotLight>();
			if (spotLight) spotLights.emplace_back(spotLight);
		}

		std::sort(spotLights.begin(), spotLights.end(),
			[=](SpotLight*& a, SpotLight*& b)
		{
			return glm::distance(a->getWorldPosition(), getMainCamera()->getWorldPosition()) < glm::distance(b->getWorldPosition(), getMainCamera()->getWorldPosition());
		}
		);

		return spotLights;
	}

	//! getClosestDirectionalLights()
	/*
	\return a std::vector<PointLight*> - A list of directional lights sorted by distance from main camera
	*/
	std::vector<DirectionalLight*> Scene::getClosestDirectionalLights()
	{
		std::vector<DirectionalLight*> dirLights;

		for (auto& entity : getEntities())
		{
			auto dirLight = entity->getComponent<DirectionalLight>();
			if (dirLight) dirLights.emplace_back(dirLight);
		}

		std::sort(dirLights.begin(), dirLights.end(),
			[=](DirectionalLight*& a, DirectionalLight*& b)
		{
			return glm::distance(a->getWorldPosition(), getMainCamera()->getWorldPosition()) < glm::distance(b->getWorldPosition(), getMainCamera()->getWorldPosition());
		}
		);

		return dirLights;
	}

	//! checkRootEntityNameTaken()
	/*!
	\param name a const std::string& - The name of the entity
	\return a bool - Does the entity name exist in our entity list
	*/
	bool Scene::checkRootEntityNameTaken(const std::string& name)
	{
		// Check if the name given exists in the entity list
		if (m_rootEntities.find(name) != m_rootEntities.end() && name != "")
			return true;
		return false;
	}

	//! setMainCamera()
	/*!
	\param camera a Camera* - A pointer to the camera
	*/
	void Scene::setMainCamera(Camera* camera)
	{
		if (!camera)
			ENGINE_ERROR("[Scene::setMainCamera] The camera provided is not a valid camera.", (void*)camera);
		else
		{
			// Set the current main camera's value to false
			if (m_mainCamera)
				m_mainCamera->setMainCamera(false);

			// Update main camera with camera passed
			m_mainCamera = camera;
			camera->setMainCamera(true);
			camera->updateProjection(WindowManager::getFocusedWindow()->getProperties().getSizef());
		}
	}

	//! getMainCamera()
	/*!
	\return a Camera* - A pointer to the main camera
	*/
	Camera* Scene::getMainCamera() const
	{
		return m_mainCamera;
	}

	//! getLayerManager()
	/*!
	\return a LayerManager* - A pointer to the layer manager
	*/
	LayerManager* Scene::getLayerManager()
	{
		return m_layerManager;
	}

	//! addRenderPass()
	/*!
	\param pass a RenderPass* - The render pass to add
	*/
	void Scene::addRenderPass(RenderPass* pass)
	{
		// If the maximum number of passes has not been reached
		if (m_renderPasses.size() < m_renderPasses.capacity())
		{
			// Check to make sure the pass we're adding is valid
			if (pass)
			{
				m_renderPasses.emplace_back(pass);
				pass->attachScene(this);
				pass->setIndex(static_cast<uint32_t>(m_renderPasses.size()) - 1);
				pass->onAttach();
			}
			else
				ENGINE_ERROR("[Scene::addRenderPass] The pass we're attempting to add is invalid. Scene Name: {0}", m_sceneName);
		}
		else
			ENGINE_ERROR("[Scene:addRenderPass] The render pass limit per scene has been reached. Scene Name: {0}.", m_sceneName);
	}

	//! getRenderPasses()
	/*!
	\return a std::vector<RenderPass*>& - A list of all renderpasses for this scene
	*/
	std::vector<RenderPass*>& Scene::getRenderPasses()
	{
		return m_renderPasses;
	}

	//! getRenderPass()
	/*!
	\param index a const uint32_t - The render pass at index
	\return a RenderPass* - The render pass at index
	*/
	RenderPass* Scene::getRenderPass(const uint32_t index)
	{
		if (m_renderPasses.size() > index)
			return m_renderPasses.at(index);
		else
			ENGINE_ERROR("[Scene:getRenderPass] The index provided doesn't match with an existing render pass. Scene Name: {0}, Index: {1}.", m_sceneName, index);

		return nullptr;
	}

	//! getFinalFrameBuffer()
	/*!
	\return a FrameBuffer* - The framebuffer
	*/
	FrameBuffer* Scene::getFinalFrameBuffer()
	{
		if (m_renderPasses.back())
			return m_renderPasses.back()->getFrameBuffer();
		else
		{
			ENGINE_ERROR("[Scene::getFinalFrameBuffer] This is an invalid framebuffer. Scene Name: {0}.", m_sceneName);
			return nullptr;
		}
	}

	//! setEntityListUpdated()
	/*
	\param value a const bool
	*/
	void Scene::setEntityListUpdated(const bool value)
	{
		m_entityListUpdated = value;
	}

	//! getEntityListUpdated()
	/*
	\return a const bool - Has the entity list been updated
	*/
	const bool Scene::getEntityListUpdated()
	{
		return m_entityListUpdated;
	}

	//! printSceneDetails()
	void Scene::printSceneDetails()
	{
		ENGINE_TRACE("===========================================");
		ENGINE_TRACE("Scene Address: {0}", (void*)this);
		ENGINE_TRACE("Scene Name: {0}", getName());
		ENGINE_TRACE("Layer Manager Address: {0}", (void*)getLayerManager());
		ENGINE_TRACE("Number of Render Passes: {0}", getRenderPasses().size());
		ENGINE_TRACE("Number of Root Entities: {0}", m_rootEntities.size());
		ENGINE_TRACE("Main Camera Address: {0}", (void*)getMainCamera());
		ENGINE_TRACE("Entity List Updated: {0}", m_entityListUpdated);
		ENGINE_TRACE("Scheduled for Deletion: {0}", getDestroyed());
		ENGINE_TRACE("===========================================");
	}
}
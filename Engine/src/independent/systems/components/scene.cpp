/*! \file scene.cpp
*
* \brief A scene contains all the data associated in a particular environment. This data will be loaded by the SceneLoader.
*
* \author Daniel Bullin
*
*/

#include "independent/systems/components/scene.h"
#include "independent/systems/systems/log.h"

#define RENDERPASSLIMIT 15

namespace Engine
{
	//! Scene()
	/*!
	\param name a const char* - The name of the scene
	*/
	Scene::Scene(const char* name) : m_sceneName(name)
	{
		ENGINE_INFO("[Scene::Scene] Creating new scene named: {0}.", name);
		m_mainCamera = nullptr;
		m_layerManager = new LayerManager(this);
		m_renderPasses.reserve(RENDERPASSLIMIT);
		m_delete = false;
		m_newEntities = true;
	}

	//! ~Scene()
	Scene::~Scene()
	{
		
	}

	//! destroy()
	void Scene::destroy()
	{
		ENGINE_INFO("[Scene::destroy] Destroying scene: {0}.", m_sceneName);

		// If there is any entities in the scene, delete them
		if (m_entities.size() != 0)
		{
			// Delete all entities
			for (auto& entity : m_entities)
				delete entity.second;

			m_entities.clear();
		}

		m_mainCamera = nullptr;

		if (m_layerManager)
			delete m_layerManager;

		for (auto& pass : m_renderPasses)
			delete pass;

		m_renderPasses.clear();
	}

	//! getName()
	/*!
	\return a const std::string& - The name of the scene
	*/
	const std::string& Scene::getName()
	{
		return m_sceneName;
	}

	//! getClearColour()
	/*!
	\return a const glm::vec4& - The clear colour
	*/
	const glm::vec4& Scene::getClearColour()
	{
		return m_clearColour;
	}

	//! setClearColour()
	/*!
	\param colour a const glm::vec4& - The clear colour
	*/
	void Scene::setClearColour(const glm::vec4& colour)
	{
		m_clearColour = colour;
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The update time step
	\param totalTime a const float - The total application time
	*/
	void Scene::onUpdate(const float timestep, const float totalTime)
	{
		// Delete any entities that are scheuled to be deleted, can also update components if entity is not to be deleted
		for (auto it = m_entities.cbegin(); it != m_entities.cend(); )
		{
			if (it->second->getDelete())
			{
				delete it->second;
				m_entities.erase(it++);
			}
			else
			{
				// Entity is not to be deleted, update components
				for (auto& component : it->second->getAllComponents())
					component.second->onUpdate(timestep, totalTime);
				++it;
			}
		}

		m_layerManager->onUpdate(timestep, totalTime);
	}

	//! addEntity()
	/*!
	\param name a const char* - The name of the entity
	\param entity an Entity* - A pointer to the entity
	*/
	void Scene::addEntity(const char* name, Entity* entity)
	{
		if (!checkEntityNameTaken(name))
		{
			m_entities[name] = entity;
			entity->setParentScene(this);
			entity->setName(name);
			setNewEntitiesFlag(true);
		}
		else
		{
			ENGINE_ERROR("[Scene::addEntity] Name for entity already taken. Cannot add. Entity Name: {0}", name);
			delete entity;
		}
	}

	//! deleteEntity()
	/*!
	\param name a const char* - The name of the entity
	*/
	void Scene::deleteEntity(const char * name)
	{
		// Check if entity name is in the list of entities
		if (checkEntityNameTaken(name))
		{
			// Found entity, schedule for deletion
			m_entities[name]->setDelete(true);
			setNewEntitiesFlag(true);
		}
		else
			ENGINE_ERROR("[Scene::deleteEntity] Name for entity wasn't found in the list. Entity Name: {0}", name);
	}

	//! getEntity()
	/*!
	\param name a const char* - The name of the entity
	\return an Entity* - A pointer to the entity
	*/
	Entity* Scene::getEntity(const char* name)
	{
		// Check if entity name exists
		if (checkEntityNameTaken(name))
			return m_entities[name];
		else
			ENGINE_ERROR("[Scene::getEntity] Cannot get entity named: {0}.", name);

		// Can't find entity
		return nullptr;
	}

	//! getRootEntities()
	/*!
	\return a std::map<std::string, Entity*>& - A reference to the root entity list
	*/
	std::map<std::string, Entity*>& Scene::getRootEntities()
	{
		return m_entities;
	}

	//! checkEntityNameTaken()
	/*!
	\param name a const char* - The name of the entity
	\return a bool - Does the entity name exist in our entity list
	*/
	bool Scene::checkEntityNameTaken(const char* name)
	{
		// Check if the name given exists in the entity list
		if (m_entities.find(name) != m_entities.end())
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
			if(m_mainCamera)
				m_mainCamera->setMainCamera(false);

			// Update main camera with camera passed
			m_mainCamera = camera;
			camera->setMainCamera(true);
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
		m_renderPasses.emplace_back(pass);
		pass->attachScene(this);
	}

	//! getRenderPasses()
	/*!
	\return a std::vector<RenderPass*>& - A list of all renderpasses for this scene
	*/
	std::vector<RenderPass*>& Scene::getRenderPasses()
	{
		return m_renderPasses;
	}

	//! setDelete()
	/*!
	\param deletion a const bool - Should this scene be deleted
	*/
	void Scene::setDelete(const bool deletion)
	{
		m_delete = deletion;
	}

	//! getDelete()
	/*!
	\return a const bool - Should this scene be deleted
	*/
	const bool Scene::getDelete() const
	{
		return m_delete;
	}

	//! setNewEntitiesFlag()
	/*!
	\param value a const bool - The new entities flag
	*/
	void Scene::setNewEntitiesFlag(const bool value)
	{
		m_newEntities = value;
	}

	//! getNewEntitiesFlag()
	/*!
	\return a const bool - The new entities flag
	*/
	const bool Scene::getNewEntitiesFlag() const
	{
		return m_newEntities;
	}

	//! printEntityDetails()
	void Scene::printEntityDetails()
	{
		for (auto& entity : m_entities)
		{
			entity.second->printEntityDetails();
		}
	}

	//! printSceneDetails()
	void Scene::printSceneDetails()
	{
		ENGINE_TRACE("Scene Details");
		ENGINE_TRACE("==========================");
		ENGINE_TRACE("Scene Address: {0}", (void*)this);
		ENGINE_TRACE("Scene Name: {0}", getName());
		ENGINE_TRACE("Scene Entity List Size: {0}", getRootEntities().size());
		ENGINE_TRACE("Scene Clear Colour: R:{0} G:{1} B:{2} A:{3}", getClearColour().r, getClearColour().g, getClearColour().b, getClearColour().a);
		ENGINE_TRACE("Scene Layer Manager Address: {0}", (void*)getLayerManager());
		ENGINE_TRACE("Scene Main Camera Address: {0}", (void*)getMainCamera());
		ENGINE_TRACE("Number of Render Passes: {0}", getRenderPasses().size());
		ENGINE_TRACE("Scheduled for Deletion: {0}", getDelete());
		ENGINE_TRACE("New Entities Flag: {0}", getNewEntitiesFlag());
		ENGINE_TRACE("==========================");
	}
}
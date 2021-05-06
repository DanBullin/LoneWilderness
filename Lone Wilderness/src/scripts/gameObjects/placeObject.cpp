/*! \file placeObject.cpp
*
* \brief A script which places objects in the world
*
* \author: Daniel Bullin
*
*/
#include "scripts/gameObjects/placeObject.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/entities/entity.h"
#include "scripts/gameObjects/terrain.h"
#include "scripts/gameObjects/player.h"

PlaceObject::PlaceObject()
{
	m_player = nullptr;
	m_camera = nullptr;
	m_currentEntity = nullptr;
	m_final = true;
}

PlaceObject::~PlaceObject()
{
}

void PlaceObject::onPreUpdate(const float timestep, const float totalTime)
{
	if (!m_camera)
		m_camera = getParent()->getParentScene()->getEntity("Player1")->getChildEntity("Camera1");

	if (!m_player)
		m_player = static_cast<Player*>(getParent()->getParentScene()->getEntity("Player1")->getComponent<NativeScript>());

	if (m_currentEntity)
	{
		glm::vec3 playerPos = m_camera->getComponent<Transform>()->getWorldPosition();
		glm::vec3 viewDir = m_camera->getComponent<Camera>()->getCameraData().Front * 4.f;
		auto trans = m_currentEntity->getComponent<Transform>();

		trans->setLocalPosition(playerPos.x + viewDir.x, playerPos.y + viewDir.y, playerPos.z + viewDir.z);
	}

	if (m_player->getInventory()->getItem(m_player->getHotbar()->getSelectedItem()))
	{
		if (Items::isPlaceable(m_player->getInventory()->getItem(m_player->getHotbar()->getSelectedItem())->getType()))
		{
			if (m_camera && !m_currentEntity)
			{
				glm::vec3 playerPos = m_camera->getComponent<Transform>()->getWorldPosition();
				glm::vec3 viewDir = m_camera->getComponent<Camera>()->getCameraData().Front * 4.f;

				Entity* ent = new Entity;
				SceneManager::getActiveScene()->addEntity("NewObj" + std::to_string(number), ent);
				ent->setDisplay(true);
				ent->setLayer(SceneManager::getActiveScene()->getLayerManager()->getLayer("Default"));
				ent->attach<Transform>("Transform1", playerPos.x + viewDir.x, playerPos.y + viewDir.y, playerPos.z + viewDir.z, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f);
				ent->attach<MeshRender3D>("MeshR", Items::getModel(m_player->getInventory()->getItem(m_player->getHotbar()->getSelectedItem())->getType()), ResourceManager::getResource<Material>("placeObjectMaterial"));
				number++;
				m_final = false;
				m_currentEntity = ent;
			}
		}
		else
		{
			if (m_currentEntity)
			{
				m_currentEntity->destroy();
				m_currentEntity = nullptr;
			}
		}
	}
}

void PlaceObject::onMouseRelease(MouseReleasedEvent & e, const float timestep, const float totalTime)
{
	if (e.getButton() == Mouse::RIGHTBUTTON)
	{
		if (m_currentEntity)
		{
			m_currentEntity->getComponent<MeshRender3D>()->setMaterial(Items::getWorldMaterial(m_player->getInventory()->getItem(m_player->getHotbar()->getSelectedItem())->getType()));
			m_currentEntity = nullptr;
			m_player->getInventory()->takeItem(m_player->getInventory()->getItem(m_player->getHotbar()->getSelectedItem())->getType(), 0, 1);
		}
	}
}
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

PlaceObject::PlaceObject()
{
	m_player = nullptr;
	m_final = true;
}

PlaceObject::~PlaceObject()
{
}

void PlaceObject::onPreUpdate(const float timestep, const float totalTime)
{
	if (!m_player)
		m_player = getParent()->getParentScene()->getEntity("Player1")->getChildEntity("Camera1");

	if (!m_final)
	{
		Entity* ent = getParent()->getParentScene()->getEntity("NewObj" + std::to_string((number - 1)));
		if (!ent) return;
		glm::vec3 playerPos = m_player->getComponent<Transform>()->getWorldPosition();
		glm::vec3 viewDir = m_player->getComponent<Camera>()->getCameraData().Front * 4.f;
		auto trans = ent->getComponent<Transform>();

		trans->setLocalPosition(playerPos.x + viewDir.x, playerPos.y + viewDir.y, playerPos.z + viewDir.z);
	}
}

void PlaceObject::onKeyPress(KeyPressedEvent & e, const float timestep, const float totalTime)
{
}

void PlaceObject::onKeyRelease(KeyReleasedEvent & e, const float timestep, const float totalTime)
{
	if (e.getKeyCode() == Keys::H)
	{
		if (m_player && m_final)
		{
			glm::vec3 playerPos = m_player->getComponent<Transform>()->getWorldPosition();
			glm::vec3 viewDir = m_player->getComponent<Camera>()->getCameraData().Front * 4.f;

			Entity* ent = new Entity;
			SceneManager::getActiveScene()->addEntity("NewObj" + std::to_string(number), ent);
			ent->setDisplay(true);
			ent->setLayer(SceneManager::getActiveScene()->getLayerManager()->getLayer("Default"));
			ent->attach<Transform>("Transform1", playerPos.x + viewDir.x, playerPos.y + viewDir.y, playerPos.z + viewDir.z, 0.f, 0.f, 0.f, 0.5f, 0.5f, 0.5f);
			ent->attach<MeshRender3D>("MeshR", ResourceManager::getResource<Model3D>("cyborg"), ResourceManager::getResource<Material>("placeObjectMaterial"));
			number++;
			m_final = false;
		}
		else
		{
			m_final = true;

			Entity* ent = getParent()->getParentScene()->getEntity("NewObj" + std::to_string((number - 1)));
			if (!ent) return;
			ent->getComponent<MeshRender3D>()->setMaterial(ResourceManager::getResource<Material>("cyborgMaterial"));
		}
	}
}
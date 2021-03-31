/*! \file placeObject.cpp
*
* \brief A script which places objects in the world
*
* \author: Daniel Bullin
*
*/
#include "scripts/placeObject.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/entities/entity.h"

PlaceObject::PlaceObject()
{
	m_player = nullptr;
}

PlaceObject::~PlaceObject()
{
}

void PlaceObject::onPreUpdate(const float timestep, const float totalTime)
{
	if (!m_player)
		m_player = getParent()->getParentScene()->getEntity("Player1");
}

void PlaceObject::onKeyPress(KeyPressedEvent & e, const float timestep, const float totalTime)
{
}

void PlaceObject::onKeyRelease(KeyReleasedEvent & e, const float timestep, const float totalTime)
{
	if (e.getKeyCode() == Keys::H)
	{
		if (m_player)
		{
			glm::vec3 playerPos = m_player->getComponent<Transform>()->getPosition();
			glm::vec3 viewDir = m_player->getComponent<Camera>()->getCameraData().Front;

			Entity* ent = new Entity;
			SceneManager::getActiveScene()->addEntity("NewObj", ent);
			ent->setDisplay(true);
			ent->setLayer(SceneManager::getActiveScene()->getLayerManager()->getLayer("Default"));
			ent->attach<Transform>("Transform1", playerPos.x + viewDir.x, playerPos.y + viewDir.y, playerPos.z + viewDir.z, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f);
			ent->attach<MeshRender3D>("MeshR", ResourceManager::getResource<Model3D>("cyborg"), nullptr);
		}
	}
}
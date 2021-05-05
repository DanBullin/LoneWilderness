/*! \file environment.cpp
*
* \brief A script for all the objects in the environment
*
* \author: Daniel Bullin
*
*/
#include "scripts/gameObjects/environment.h"
#include "scripts/gameObjects/player.h"
#include "independent/rendering/renderers/renderer3D.h"
#include "independent/utils/mathUtils.h"
#include "scripts/gameObjects/terrain.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/randomiser.h"

bool Environment::existsInsideBB(BoundingBox bb, Transform* otherTransform)
{
	auto camDir = SceneManager::getActiveScene()->getMainCamera()->getCameraData().Front * 1.f;
	glm::vec3 playerCamVector = otherTransform->getWorldPosition() + camDir;

	if (playerCamVector.x > bb.TopLeft && playerCamVector.x < bb.TopRight && playerCamVector.z > bb.BottomLeft && playerCamVector.z < bb.BottomRight && playerCamVector.y < bb.Height)
	{
		return true;
	}
	return false;
}

Environment::Environment()
{
	m_treeHighlightedEntity = nullptr;
	m_rockHighlightedEntity = nullptr;
}

Environment::~Environment()
{
}

void Environment::onAttach()
{
	m_treeHighlightedEntity = getParent()->getParentScene()->getEntity("Tree1");
	m_rockHighlightedEntity = getParent()->getParentScene()->getEntity("Rock1");
	m_treeModel = ResourceManager::getResource<Model3D>("tree");
	m_rockModel = ResourceManager::getResource<Model3D>("rock");

	for (int i = 0; i < 50; i++)
	{
		glm::vec3 generatedPos = { 0.f, -10.f, 0.f};
		while (generatedPos.y < 50.f)
		{
			generatedPos = generatePoint();
		}
		m_treePositions.push_back({ generatedPos, false });
		m_treeBB.push_back(generateBB(m_treePositions.back().first));
	}

	for (int i = 0; i < 50; i++)
	{
		glm::vec3 generatedPos = { 0.f, -10.f, 0.f };
		while (generatedPos.y < 50.f)
		{
			generatedPos = generatePoint();
		}
		m_rockPositions.push_back({generatedPos, false});
		m_rockBB.push_back(generateBB(m_rockPositions.back().first));
	}
}

void Environment::onPostUpdate(const float timestep, const float totalTime)
{
	auto trans = getParent()->getParentScene()->getEntity("Player1")->getChildEntity("Camera1")->getComponent<Transform>();
	auto camDir = SceneManager::getActiveScene()->getMainCamera()->getCameraData().Front;
	camDir *= 4.f;
	glm::vec3 playerCamVector = trans->getWorldPosition() + camDir;

	for (int i = 0; i < m_treePositions.size(); i++)
	{
		bool previous = m_treePositions[i].second;
		float angle = trans->angle(m_treePositions[i].first);
		float distance = trans->distance(m_treePositions[i].first);

		if (angle < 10.f && distance < 16.f)
			m_treePositions[i].second = true;
		else
			m_treePositions[i].second = false;

		if (m_treePositions[i].second)
		{
			m_treeHighlightedEntity->getComponent<Transform>()->setLocalPosition(m_treePositions[i].first);
			m_treeHighlightedEntity->setDisplay(true);
			m_treeHighlightedEntity->setSelected(true);
		}

		if (!m_treePositions[i].second && previous)
		{
			m_treeHighlightedEntity->setSelected(false);
		}
	}

	for (int i = 0; i < m_rockPositions.size(); i++)
	{
		bool previous = m_rockPositions[i].second;
		float angle = trans->angle(m_rockPositions[i].first);
		float distance = trans->distance(m_rockPositions[i].first);

		if (angle < 10.f && distance < 16.f)
			m_rockPositions[i].second = true;
		else
			m_rockPositions[i].second = false;

		if (m_rockPositions[i].second)
		{
			m_rockHighlightedEntity->getComponent<Transform>()->setLocalPosition(m_rockPositions[i].first);
			m_rockHighlightedEntity->setDisplay(true);
			m_rockHighlightedEntity->setSelected(true);
		}

		if (!m_rockPositions[i].second && previous)
		{
			m_rockHighlightedEntity->setSelected(false);
		}
	}
}

void Environment::onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime)
{
}

void Environment::onRender(const Renderers renderer, const std::string& renderState)
{
	if (renderer == Renderers::Renderer3D && renderState != "Terrain")
	{
		for (auto& pos : m_treePositions)
		{
			for (auto& mesh : m_treeModel->getMeshes())
			{
				Renderer3D::submit("Tree", mesh.getGeometry(), mesh.getMaterial(), MathUtils::getModelMatrix(pos.first, { 6.f, 6.f, 6.f }));
			}
		}

		for (auto& pos : m_rockPositions)
		{
			for (auto& mesh : m_rockModel->getMeshes())
			{
				Renderer3D::submit("Rock", mesh.getGeometry(), mesh.getMaterial(), MathUtils::getModelMatrix(pos.first, { 0.25f, 0.25f, 0.25f }));
			}
		}
	}
}

void Environment::onMousePress(MousePressedEvent & e, const float timestep, const float totalTime)
{
	if (e.getButton() == Mouse::LEFTBUTTON)
	{
		Player* player = static_cast<Player*>(getParent()->getParentScene()->getEntity("Player1")->getComponent<NativeScript>());

		for (int i = 0; i < m_treePositions.size(); i++)
		{
			if (m_treePositions[i].second)
			{
				m_treeHighlightedEntity->setDisplay(false);
				m_treeHighlightedEntity->setSelected(false);
				m_treePositions.erase(m_treePositions.begin()+i);
				player->getInventory()->giveItem(Items::Log, 0, 1);
				return;
			}
		}

		for (int i = 0; i < m_rockPositions.size(); i++)
		{
			if (m_rockPositions[i].second)
			{
				m_rockHighlightedEntity->setDisplay(false);
				m_rockHighlightedEntity->setSelected(false);
				m_rockPositions.erase(m_rockPositions.begin() + i);
				player->getInventory()->giveItem(Items::Stone, 0, 1);
				return;
			}
		}
	}
}

glm::vec3 Environment::generatePoint()
{
	Terrain* terrain = static_cast<Terrain*>(getParent()->getParentScene()->getEntity("Terrain1")->getComponent<NativeScript>());
	float x = Randomiser::uniformFloatBetween(-500.f, 500.f);
	float z = Randomiser::uniformFloatBetween(-500.f, 500.f);
	float posX = Randomiser::uniformIntBetween(0, 1);
	float posZ = Randomiser::uniformIntBetween(0, 1);
	if (posX == 0) x = -x;
	if (posZ == 0) z = -z;
	return { x, terrain->getYCoord(x, z), z };
}

BoundingBox Environment::generateBB(glm::vec3 pos)
{
	return {pos.x - 3, pos.x + 3, pos.z - 3, pos.z + 3, pos.y + 15};
}
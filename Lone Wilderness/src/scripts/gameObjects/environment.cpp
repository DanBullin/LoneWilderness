/*! \file environment.cpp
*
* \brief A script for all the objects in the environment
*
* \author: Daniel Bullin
*
*/
#include "scripts/gameObjects/environment.h"
#include "independent/rendering/renderers/renderer3D.h"
#include "independent/utils/mathUtils.h"
#include "scripts/gameObjects/terrain.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/randomiser.h"

bool Environment::existsInsideBB(BoundingBox bb, Transform* otherTransform)
{
	auto camDir = SceneManager::getActiveScene()->getMainCamera()->getCameraData().Front * 4.f;
	glm::vec3 playerCamVector = otherTransform->getWorldPosition() + camDir;

	if (playerCamVector.x > bb.TopLeft && playerCamVector.x < bb.TopRight && playerCamVector.z > bb.BottomLeft && playerCamVector.z < bb.BottomRight && playerCamVector.y < bb.Height)
	{
		ENGINE_INFO("YES");
		return true;
	}
	return false;
}

Environment::Environment()
{
}

Environment::~Environment()
{
}

void Environment::onAttach()
{
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
		m_rockPositions.push_back(generatedPos);
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
		m_treePositions[i].second = existsInsideBB(m_treeBB[i], trans);
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
				Renderer3D::submit("Rock", mesh.getGeometry(), mesh.getMaterial(), MathUtils::getModelMatrix(pos, { 0.25f, 0.25f, 0.25f }));
			}
		}
	}
}

void Environment::onMousePress(MousePressedEvent & e, const float timestep, const float totalTime)
{
	if (e.getButton() == Mouse::LEFTBUTTON)
	{
		for (int i = 0; i < m_treePositions.size(); i++)
		{
			if (m_treePositions[i].second)
			{
				m_treePositions.erase(m_treePositions.begin()+i);
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
	return {pos.x - 2, pos.x + 2, pos.z - 2, pos.z + 2, pos.y + 10};
}

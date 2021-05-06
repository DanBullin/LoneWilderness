/*! \file sun.h
*
* \brief A script for the rising and setting of the sun
*
* \author: Daniel Bullin
*
*/
#include "scripts/gameObjects/sun.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/rendering/renderers/renderer3D.h"

Sun::Sun()
{
	m_timer = nullptr;
	m_skyboxMaterial = nullptr;
	m_cycleTime = 600.f;
	m_approachingNight = true;
}

Sun::~Sun()
{
}

void Sun::onPreUpdate(const float timestep, const float totalTime)
{
	if (!m_timer)
	{
		m_timer = static_cast<GameTimer*>(getParent()->getParentScene()->getEntity("TimerEntity")->getComponent<NativeScript>());
	}

	if (!m_skyboxMaterial)
		m_skyboxMaterial = ResourceManager::getResource<Material>("skyboxMaterial");

	DirectionalLight* light = getParent()->getComponent<DirectionalLight>();

	if (m_timer->getTime() > m_cycleTime)
	{
		if (m_approachingNight)
		{
			light->setDiffuseFactor(light->getDiffuseFactor() - glm::vec3(0.001f, 0.001f, 0.001f));
			m_skyboxMaterial->setTint(m_skyboxMaterial->getTint() - glm::vec4(0.002f, 0.002f, 0.002f, 0.f));
		}
		else
		{
			light->setDiffuseFactor(light->getDiffuseFactor() + glm::vec3(0.001f, 0.001f, 0.001f));
			m_skyboxMaterial->setTint(m_skyboxMaterial->getTint() + glm::vec4(0.002f, 0.002f, 0.002f, 0.f));
		}

		if (m_skyboxMaterial->getTint().r <= 0.f && m_approachingNight)
		{
			m_timer->reset();
			m_approachingNight = false;
		}

		if (m_skyboxMaterial->getTint().r >= 1.f && !m_approachingNight)
		{
			m_timer->reset();
			m_approachingNight = true;
		}

	}
}

void Sun::onSubmit(const Renderers renderer, const std::string & renderState)
{
	if (getParent()->containsComponent<MeshRender3D>())
	{
		getParent()->getComponent<MeshRender3D>()->getMaterial()->setTint({ getParent()->getComponent<PointLight>()->getDiffuseFactor(), 1.f });
	}
}
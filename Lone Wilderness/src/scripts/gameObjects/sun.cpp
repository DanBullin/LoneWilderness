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

	if (m_timer->getTime() < (m_cycleTime / 2.f))
	{
		light->setDiffuseFactor(light->getDiffuseFactor() - glm::vec3(0.00002f, 0.00002f, 0.00002f));
		m_skyboxMaterial->setTint(m_skyboxMaterial->getTint() - glm::vec4(0.0002f, 0.0002f, 0.0002f, 0.f));
	}
	else
	{
		light->setDiffuseFactor(light->getDiffuseFactor() + glm::vec3(0.00002f, 0.00002f, 0.00002f));
		m_skyboxMaterial->setTint(m_skyboxMaterial->getTint() + glm::vec4(0.0002f, 0.0002f, 0.0002f, 0.f));

		if (m_timer->getTime() > m_cycleTime)
			m_timer->reset();
	}

}

void Sun::onSubmit(const Renderers renderer, const std::string & renderState)
{
	if (getParent()->containsComponent<MeshRender3D>())
	{
		getParent()->getComponent<MeshRender3D>()->getMaterial()->setTint({ getParent()->getComponent<PointLight>()->getDiffuseFactor(), 1.f });
	}
}
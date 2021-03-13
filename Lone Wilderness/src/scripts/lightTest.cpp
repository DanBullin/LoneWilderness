/*! \file lightTest.cpp
*
* \brief A light example dummy class
*
* \author: Daniel Bullin
*
*/
#include "scripts/lightTest.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/randomiser.h"
#include "independent/systems/systems/timerSystem.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/rendering/renderers/renderer3D.h"

LightTest::LightTest()
{
}

LightTest::~LightTest()
{
}

void LightTest::onPreUpdate(const float timestep, const float totalTime)
{
	Transform* trans = getParent()->getComponent<Transform>();
	if (!trans)
	{
		ENGINE_ERROR("[LightTest::onPreUpdate] A LightTest entity does not have a valid 3D transform.");
		return;
	}

	if (m_first)
	{
		m_originalPos = trans->getPosition();
		m_first = false;
	}

	if (getParent()->getName() == "LightSource6") trans->setPosition(m_originalPos + glm::vec3(0.f, 0.f, m_distance));

	if (m_distance > 20.f)
		m_increment = -0.03f;
	else if (m_distance < -20.f)
		m_increment = 0.03f;

	m_distance += m_increment;
}

void LightTest::onSubmit(const Renderers renderer)
{
	if (getParent()->containsComponent<MeshRender3D>())
	{
		getParent()->getComponent<MeshRender3D>()->getMaterial()->setTint({ getParent()->getComponent<PointLight>()->getDiffuseFactor(), 1.f });
	}
}
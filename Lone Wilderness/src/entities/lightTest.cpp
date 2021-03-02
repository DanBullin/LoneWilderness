/*! \file lightTest.cpp
*
* \brief A light example dummy class
*
* \author: Daniel Bullin
*
*/
#include "entities/lightTest.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/randomiser.h"
#include "independent/systems/systems/timerSystem.h"
#include "independent/rendering/renderers/renderer3D.h"

LightTest::LightTest()
{
}

LightTest::~LightTest()
{
}

void LightTest::onPreUpdate(const float timestep, const float totalTime)
{
	Transform* trans = getComponent<Transform>();
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

	if (getName() == "LightSource6") trans->setPosition(m_originalPos + glm::vec3(0.f, 0.f, m_distance));

	if (m_distance > 20.f)
		m_increment = -0.03f;
	else if (m_distance < -20.f)
		m_increment = 0.03f;

	m_distance += m_increment;
}

void LightTest::onKeyRelease(KeyReleasedEvent & e, const float timestep, const float totalTime)
{
}

void LightTest::onRender(const Renderers renderer)
{
	if (renderer == Renderers::Renderer3D)
	{
		if (containsComponent<MeshRender3D>())
		{
			MeshRender3D* meshRender = getComponent<MeshRender3D>();
			// Submit all 3D mesh renders
			meshRender->getMaterial()->setTint({ getComponent<PointLight>()->getDiffuseFactor(), 1.f });

			for (auto& mesh : meshRender->getModel()->getMeshes())
				Renderer3D::submit(mesh.getGeometry(), meshRender->getMaterial(), getComponent<Transform>()->getModelMatrix());
		}
	}
}
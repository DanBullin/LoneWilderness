/*! \file craftButton.cpp
*
* \brief A script to handle the craft button
*
* \author: Daniel Bullin
*
*/
#include "scripts/menus/craftButton.h"
#include "independent/entities/entity.h"
#include "independent/systems/components/scene.h"
#include "independent/rendering/renderers/renderer2D.h"
#include "scripts/gameObjects/player.h"

CraftButton::CraftButton()
{
	m_render = nullptr;
	m_hovered = false;
}

CraftButton::~CraftButton()
{
}

void CraftButton::onAttach()
{
	m_highlightMaterial = ResourceManager::getResource<Material>("craftButtonHMaterial");
	m_unhighlightMaterial = ResourceManager::getResource<Material>("craftButtonUMaterial");
}

void CraftButton::onMouseRelease(MouseReleasedEvent & e, const float timestep, const float totalTime)
{
	glm::vec2 mousePosition = InputPoller::getMousePosition();
	bool containsMouse = m_render->containsPoint(mousePosition);

	if (e.getButton() == Mouse::LEFTBUTTON && containsMouse)
	{

	}
}

void CraftButton::onPreUpdate(const float timestep, const float totalTime)
{
}

void CraftButton::onMouseMoved(MouseMovedEvent & e, const float timestep, const float totalTime)
{
	if (!m_render) m_render = getParent()->getComponent<MeshRender2D>();
	bool containsMouse = m_render->containsPoint(e.getPos());

	if (m_render)
	{
		if (containsMouse && !InputPoller::isMousePressed(Mouse::LEFTBUTTON))
			m_hovered = true;
		else if (!containsMouse && !InputPoller::isMousePressed(Mouse::LEFTBUTTON))
			m_hovered = false;
	}
}

void CraftButton::onSubmit(const Renderers renderer, const std::string & renderState)
{
	if (getParent()->containsComponent<MeshRender2D>())
	{
		if (m_hovered)
			getParent()->getComponent<MeshRender2D>()->setMaterial(m_highlightMaterial);
		else
			getParent()->getComponent<MeshRender2D>()->setMaterial(m_unhighlightMaterial);
	}
}
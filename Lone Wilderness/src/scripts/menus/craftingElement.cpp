/*! \file craftingElement.cpp
*
* \brief A script to handle the crafting items
*
* \author: Daniel Bullin
*
*/
#include "scripts/menus/craftingElement.h"
#include "independent/entities/entity.h"
#include "independent/systems/components/scene.h"
#include "independent/rendering/renderers/renderer2D.h"
#include "scripts/gameObjects/player.h"

CraftingElement::CraftingElement()
{
	m_render = nullptr;
	m_selectedTint = { 1.f, 1.f, 1.f, 1.f };
	m_inventorySlot = nullptr;
	m_inventoryCurrent = nullptr;
	m_itemIndex = -1;
}

CraftingElement::CraftingElement(nlohmann::json scriptData)
{
	m_render = nullptr;
	m_selectedTint = { 1.f, 1.f, 1.f, 1.f };
	m_inventorySlot = nullptr;
	m_inventoryCurrent = nullptr;
	m_itemIndex = scriptData["index"].get<int>();
}

CraftingElement::~CraftingElement()
{
}

void CraftingElement::onAttach()
{
}

void CraftingElement::onMouseRelease(MouseReleasedEvent & e, const float timestep, const float totalTime)
{
	glm::vec2 mousePosition = InputPoller::getMousePosition();
	bool containsMouse = m_render->containsPoint(mousePosition);

	if (e.getButton() == Mouse::LEFTBUTTON && containsMouse)
	{
		
	}
}

void CraftingElement::onMouseMoved(MouseMovedEvent & e, const float timestep, const float totalTime)
{
	if (!m_render) m_render = getParent()->getComponent<MeshRender2D>();
	bool containsMouse = m_render->containsPoint(e.getPos());

	if (m_render)
	{
		if (containsMouse && !InputPoller::isMousePressed(Mouse::LEFTBUTTON))
			m_selectedTint = { 1.f, 0.5f, 0.85f, 1.f };
		else if (!containsMouse && !InputPoller::isMousePressed(Mouse::LEFTBUTTON))
			m_selectedTint = { 1.f, 1.0f, 1.f, 1.f };
	}

	if(m_itemIndex == -1)
		m_selectedTint = { 1.f, 1.0f, 1.f, 1.f };
}

void CraftingElement::onPreUpdate(const float timestep, const float totalTime)
{
}

void CraftingElement::onSubmit(const Renderers renderer, const std::string & renderState)
{
	if (getParent()->containsComponent<MeshRender2D>())
	{
		getParent()->getComponent<MeshRender2D>()->getMaterial()->setTint(m_selectedTint);
	}
}

Entity * CraftingElement::getSlot()
{
	return m_inventorySlot;
}

void CraftingElement::resetSlot()
{
	m_inventorySlot = 0;
	m_inventoryCurrent->setDisplay(false);
	m_selectedTint = { 1.f, 1.f, 1.f, 1.f };
	getParent()->getComponent<MeshRender2D>()->getMaterial()->setTint(m_selectedTint);
}

uint32_t CraftingElement::getInvenIndex()
{
	return m_itemIndex;
}

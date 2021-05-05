/*! \file inventoryElement.cpp
*
* \brief A script to handle the inventory items
*
* \author: Daniel Bullin
*
*/
#include "scripts/menus/inventoryElement.h"
#include "independent/entities/entity.h"
#include "independent/systems/components/scene.h"
#include "independent/rendering/renderers/renderer2D.h"
#include "scripts/gameObjects/player.h"

InventoryElement::InventoryElement()
{
	m_render = nullptr;
	m_selectedTint = { 1.f, 1.f, 1.f, 1.f };
	m_inventorySlot = nullptr;
	m_inventoryCurrent = nullptr;
	m_inventoryIndex = 0;
}

InventoryElement::InventoryElement(nlohmann::json scriptData)
{
	m_render = nullptr;
	m_selectedTint = { 1.f, 1.f, 1.f, 1.f };
	m_inventorySlot = nullptr;
	m_inventoryCurrent = nullptr;
	m_inventoryIndex = scriptData["index"].get<uint32_t>();
}

InventoryElement::~InventoryElement()
{
}

void InventoryElement::onAttach()
{
}

void InventoryElement::onMouseRelease(MouseReleasedEvent & e, const float timestep, const float totalTime)
{
	glm::vec2 mousePosition = InputPoller::getMousePosition();
	bool containsMouse = m_render->containsPoint(mousePosition);

	if (e.getButton() == Mouse::LEFTBUTTON && containsMouse)
	{
		Player* player = static_cast<Player*>(getParent()->getParentScene()->getEntity("Player1")->getComponent<NativeScript>());
		if (player->getInventory()->getItem(m_inventoryIndex))
		{
			if (!m_inventorySlot)
			{
				m_inventoryCurrent->setDisplay(true);

				if (m_inventoryCurrent)
					m_inventoryCurrent->getComponent<Transform>()->setLocalPosition(getParent()->getComponent<Transform>()->getWorldPosition());

				m_inventorySlot = getParent();
			}
			else
			{
				m_inventoryCurrent->setDisplay(false);
				m_inventorySlot = nullptr;
			}
		}
	}
}

void InventoryElement::onMouseMoved(MouseMovedEvent & e, const float timestep, const float totalTime)
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
}

void InventoryElement::onPreUpdate(const float timestep, const float totalTime)
{
	if (!m_inventoryCurrent)
		m_inventoryCurrent = getParent()->getParentScene()->getEntity("inventoryCurrent");
}

void InventoryElement::onSubmit(const Renderers renderer, const std::string & renderState)
{
	if (getParent()->containsComponent<MeshRender2D>())
	{
		getParent()->getComponent<MeshRender2D>()->getMaterial()->setTint(m_selectedTint);
	}
}

Entity * InventoryElement::getSlot()
{
	return m_inventorySlot;
}

void InventoryElement::resetSlot()
{
	m_inventorySlot = 0;
	m_inventoryCurrent->setDisplay(false);
	m_selectedTint = { 1.f, 1.f, 1.f, 1.f };
	getParent()->getComponent<MeshRender2D>()->getMaterial()->setTint(m_selectedTint);
}

uint32_t InventoryElement::getInvenIndex()
{
	return m_inventoryIndex;
}

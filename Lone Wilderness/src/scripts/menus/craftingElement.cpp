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
#include "scripts/menus/craftingMenu.h"

CraftingElement::CraftingElement()
{
	m_render = nullptr;
	m_selectedTint = { 1.f, 1.f, 1.f, 1.f };
	m_itemIndex = -1;
}

CraftingElement::CraftingElement(nlohmann::json scriptData)
{
	m_render = nullptr;
	m_selectedTint = { 1.f, 1.f, 1.f, 1.f };
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

	if (e.getButton() == Mouse::LEFTBUTTON && containsMouse && m_itemIndex != -1)
	{
		auto crafting = static_cast<CraftingMenu*>(getParent()->getParentScene()->getEntity("CraftingEnt")->getComponent<NativeScript>());
		crafting->selectItem(static_cast<Items::Items>(m_itemIndex));
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
	if (getParent()->getName() == "C_ItemInfoImage")
	{
		auto crafting = static_cast<CraftingMenu*>(getParent()->getParentScene()->getEntity("CraftingEnt")->getComponent<NativeScript>());
		if (crafting->getSelectedItem() != Items::None)
		{
			getParent()->setDisplay(true);
			m_render->setMaterial(Items::getMaterial(crafting->getSelectedItem()));
		}
		else
			getParent()->setDisplay(false);
	}
}

void CraftingElement::onSubmit(const Renderers renderer, const std::string & renderState)
{
	if (getParent()->containsComponent<MeshRender2D>())
	{
		getParent()->getComponent<MeshRender2D>()->getMaterial()->setTint(m_selectedTint);
	}
}

uint32_t CraftingElement::getItemIndex()
{
	return m_itemIndex;
}

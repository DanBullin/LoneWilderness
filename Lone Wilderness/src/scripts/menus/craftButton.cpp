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
#include "scripts/menus/craftingMenu.h"

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
		auto crafting = static_cast<CraftingMenu*>(getParent()->getParentScene()->getEntity("CraftingEnt")->getComponent<NativeScript>());
		
		if (crafting->getSelectedItem() != Items::None)
		{
			Player* player = static_cast<Player*>(getParent()->getParentScene()->getEntity("Player1")->getComponent<NativeScript>());
			auto cost = Items::getCost(static_cast<Items::Items>(crafting->getSelectedItem()));

			bool canMake = false;
			for (int i = 0; i < cost.size(); i++)
			{
				if (player->getInventory()->getItemCount(cost.at(i).first, 0) < cost.at(i).second)
				{
					break;
				}

				if (i == cost.size() - 1)
					canMake = true;
			}

			if(canMake) player->getInventory()->giveItem(crafting->getSelectedItem(), 0, 1);
			for (auto& item : cost)
			{
				player->getInventory()->takeItem(item.first, 0, item.second);
			}
		}
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
/*! \file inventoryElement.cpp
*
* \brief A script to handle the inventory items
*
* \author: Daniel Bullin
*
*/
#include "scripts/menus/inventoryElement.h"
#include "independent/entities/entity.h"
#include "independent/rendering/renderers/renderer2D.h"

InventoryElement::InventoryElement()
{
	m_render = nullptr;
	m_selectedTint = { 1.f, 1.f, 1.f, 1.f };
}

InventoryElement::~InventoryElement()
{
}

void InventoryElement::onAttach()
{
}

void InventoryElement::onMouseRelease(MouseReleasedEvent & e, const float timestep, const float totalTime)
{
}

void InventoryElement::onMouseMoved(MouseMovedEvent & e, const float timestep, const float totalTime)
{
	
}

void InventoryElement::onPreUpdate(const float timestep, const float totalTime)
{
}

void InventoryElement::onSubmit(const Renderers renderer, const std::string & renderState)
{
	if (getParent()->containsComponent<MeshRender2D>())
	{
		getParent()->getComponent<MeshRender2D>()->getMaterial()->setTint(m_selectedTint);
	}
}
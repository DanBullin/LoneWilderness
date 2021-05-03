/*! \file UIElement.cpp
*
* \brief A UI element
*
* \author Daniel Bullin
*
*/
#include "independent/entities/components/UIElement.h"
#include "independent/entities/components/Transform.h"
#include "independent/entities/entity.h"
#include "independent/systems/systems/windowManager.h"

namespace Engine
{
	UIElement::UIElement(glm::vec2 anchor, glm::vec2 offset, glm::vec2 scale, bool absolute) : EntityComponent(ComponentType::UIElement)
	{
		m_anchor = anchor;
		m_offset = offset;
		m_scaleSize = scale;
		m_useAbsoluteSize = absolute;
	}

	UIElement::~UIElement()
	{
	}

	void UIElement::onAttach()
	{
	}

	void UIElement::onDetach()
	{
	}

	void UIElement::onUpdate(const float timestep, const float totalTime)
	{
		auto size = WindowManager::getFocusedWindow()->getProperties().getSizef();
		auto trans = getParent()->getComponent<Transform>();

		if (m_anchor != glm::vec2(-1, -1))
		{
			trans->setLocalPosition({ ceil((size.x * m_anchor.x) + m_offset.x), ceil((size.y * m_anchor.y) + m_offset.y),  trans->getLocalPosition().z });
		}

		if (!m_useAbsoluteSize)
		{
			trans->setScale({ size.x * m_scaleSize.x, size.y * m_scaleSize.y, trans->getScale().z });
		}
	}

	void UIElement::printComponentDetails()
	{
	}

	glm::vec2 UIElement::getAnchor()
	{
		return m_anchor;
	}

	glm::vec2 UIElement::getOffset()
	{
		return m_offset;
	}

	glm::vec2 UIElement::getScaleSize()
	{
		return m_scaleSize;
	}

	bool UIElement::getUseAbsoluteSize()
	{
		return m_useAbsoluteSize;
	}

	void UIElement::setAnchor(const glm::vec2 & anchor)
	{
		m_anchor = anchor;
	}

	void UIElement::setOffset(const glm::vec2 & offset)
	{
		m_offset = offset;
	}

	void UIElement::setScaleSize(const glm::vec2 & scale)
	{
		m_scaleSize = scale;
	}

	void UIElement::useAbsoluteSize(const bool use)
	{
		m_useAbsoluteSize = use;
	}
}
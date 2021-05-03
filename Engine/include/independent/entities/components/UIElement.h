/*! \file UIElement.h
*
* \brief A UI element
*
* \author Daniel Bullin
*
*/
#ifndef UIELEMENT_H
#define UIELEMENT_H

#include "independent/entities/entityComponent.h"

namespace Engine
{
	/*! \class UIElement
	* \brief A UI element
	*/
	class UIElement : public EntityComponent
	{
	private:
		glm::vec2 m_anchor;
		glm::vec2 m_offset;
		glm::vec2 m_scaleSize;
		bool m_useAbsoluteSize;
	public:
		UIElement(glm::vec2 anchor, glm::vec2 offset, glm::vec2 scale, bool absolute);
		~UIElement();

		void onAttach() override; //!< Occurs when component is attached to an entity
		void onDetach() override; //!< Occurs when component is detached from an entity
		void onUpdate(const float timestep, const float totalTime) override; //!< Update the component
		void printComponentDetails() override; //!< Print component details

		glm::vec2 getAnchor();
		glm::vec2 getOffset();
		glm::vec2 getScaleSize();
		bool getUseAbsoluteSize();
		void setAnchor(const glm::vec2& anchor);
		void setOffset(const glm::vec2& offset);
		void setScaleSize(const glm::vec2& scale);
		void useAbsoluteSize(const bool use);
	};
}
#endif
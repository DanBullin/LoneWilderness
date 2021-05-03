/*! \file hotbar.h
*
* \brief The player's hotbar
*
* \author: Daniel Bullin
*
*/
#ifndef HOTBAR_H
#define HOTBAR_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/UIElement.h"

using namespace Engine;

/*! \class Hotbar
* \brief The player's hotbar
*/
class Hotbar : public NativeScript
{
private:
	static const uint32_t s_maxCount = 5;
	std::vector<uint32_t> m_hotbarItems;
	uint32_t m_currentItem = 0;
	UIElement* m_currentElement;
public:
	Hotbar(); //!< Constructor
	~Hotbar(); //!< Destructor

	void onAttach() override; //!< Called when attached to the entity
	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call before game update
	void onKeyPress(KeyPressedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key press
	void onMouseScrolled(MouseScrolledEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse scroll

	void setHotbarItem(const uint32_t hotbarIndex, const uint32_t item);
	void scrollDown();
	void scrollUp();

};
#endif
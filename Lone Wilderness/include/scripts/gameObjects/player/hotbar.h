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
#include "independent/entities/components/meshRender2D.h"
#include "independent/entities/components/text.h"

using namespace Engine;

class Player;

/*! \class Hotbar
* \brief The player's hotbar
*/
class Hotbar : public NativeScript
{
private:
	static const uint32_t s_maxCount = 5;
	uint32_t m_currentItem = 0;
	UIElement* m_currentElement;
	bool m_updated; //!< Has the hotbar been updated
	std::vector<std::pair<MeshRender2D*, Text*>> m_hotbarElements; //!< The images for the different hotbar items and text
	Player* m_player; //!< A pointer to the player script
public:
	Hotbar(); //!< Constructor
	~Hotbar(); //!< Destructor

	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call before game update
	void onKeyPress(KeyPressedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key press
	void onMouseScrolled(MouseScrolledEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse scroll

	void scrollDown();
	void scrollUp();
	void selectHotbar(const uint32_t index);

	uint32_t getSelectedItem(); //!< Get the selected item index
	static uint32_t getMaxCount(); //!< Get the total number of items on the hotbar

};
#endif
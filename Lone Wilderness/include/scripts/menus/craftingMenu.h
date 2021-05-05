/*! \file craftingMenu.h
*
* \brief A script to show the crafting menu
*
* \author: Daniel Bullin
*
*/
#ifndef CRAFTINGMENU_H
#define CRAFTINGMENU_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/meshRender2D.h"
#include "independent/entities/components/text.h"

using namespace Engine;

class Player;

/*! \class CraftingMenu
* \brief A script to show the crafting menu
*/
class CraftingMenu : public NativeScript
{
private:
	Player* m_player; //!< A pointer to the player script
public:
	CraftingMenu(); //!< Constructor
	~CraftingMenu(); //!< Destructor
	void onAttach() override; //!< On attach
	void onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key release
	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call before game update
};
#endif
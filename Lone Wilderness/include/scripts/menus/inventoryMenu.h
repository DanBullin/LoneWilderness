/*! \file inventoryMenu.h
*
* \brief A script to show the inventory menu
*
* \author: Daniel Bullin
*
*/
#ifndef INVENTORYMENU_H
#define INVENTORYMENU_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/meshRender2D.h"
#include "independent/entities/components/text.h"

using namespace Engine;

class Player;

/*! \class InventoryMenu
* \brief A script to show the inventory menu
*/
class InventoryMenu : public NativeScript
{
private:
	bool m_updated; //!< Has the inventory been updated
	std::vector<std::pair<MeshRender2D*, Text*>> m_inventoryElements; //!< The images for the different inventory items
	Player* m_player; //!< A pointer to the player script
public:
	InventoryMenu(); //!< Constructor
	~InventoryMenu(); //!< Destructor
	void onAttach() override; //!< On attach
	void onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key release
	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call before game update
};
#endif
/*! \file inventory.h
*
* \brief An inventory script
*
* \author: Daniel Bullin
*
*/
#ifndef INVENTORY_H
#define INVENTORY_H

#include "independent/entities/components/nativeScript.h"
#include "scripts/gameObjects/player/item.h"

#define INVENLIMIT 15
#define AMOUNTLIMIT 50

using namespace Engine;

/*! \class Inventory
* \brief A script to handle the player's inventory
*/
class Inventory : public NativeScript
{
private:
	std::vector<std::pair<Item*, int>> m_items; //!< The inventory item slots (Items, Amount)
public:
	Inventory(); //!< Constructor
	~Inventory(); //!< Destructor

	void onAttach() override; //!< Called when attached to the entity
	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call before game update

	void giveItem(Items::Items item, const uint32_t state, const int amount); //!< The hotbar index to place the item
	void takeItem(Items::Items itemType, const uint32_t state, const int amount); //!< Remove an item
	Item* getItem(Items::Items itemType, const uint32_t state); //!< Get the index by type
	Item* getItem(const uint32_t hotbarIndex); //!< Get the item by hotbar index
	bool containsItem(Items::Items itemType, const uint32_t state); //!< Contains item
	bool containsItem(Items::Items itemType, const uint32_t state, const int amount); //!< Contains item

	void print();
};
#endif
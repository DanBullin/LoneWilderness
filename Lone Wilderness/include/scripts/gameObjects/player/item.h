/*! \file item.h
*
* \brief An item object
*
* \author: Daniel Bullin
*
*/
#ifndef ITEM_H
#define ITEM_H

#include "independent/entities/components/nativeScript.h"
#include "independent/systems/systems/resourceManager.h"

using namespace Engine;

namespace Items
{
	/*! \enum Items
	* \brief The different items
	*/
	enum Items
	{
		Log, Stone, Axe, Sword, Pickaxe, WoodenFloor, None
	};

	static bool isPlaceable(Items item)
	{
		switch (item)
		{
			case Items::Log: return false;
			case Items::Stone: return true;
			case Items::Axe: return false;
			case Items::Sword: return false;
			case Items::Pickaxe: return false;
			case Items::WoodenFloor: return true;
		}
	}

	static Model3D* getModel(Items item)
	{
		switch (item)
		{
		case Items::WoodenFloor: return ResourceManager::getResource<Model3D>("cube");
		case Items::Stone: return ResourceManager::getResource<Model3D>("cube");
		}
	}

	static Material* getWorldMaterial(Items item)
	{
		switch (item)
		{
		case Items::WoodenFloor: return ResourceManager::getResource<Material>("woodWMaterial");
		case Items::Stone: return ResourceManager::getResource<Material>("stoneWMaterial");
		}
	}

	static Material* getMaterial(Items item)
	{
		switch (item)
		{
		case Items::Log: return ResourceManager::getResource<Material>("logMaterial");
		case Items::Stone: return ResourceManager::getResource<Material>("stoneMaterial");
		case Items::Axe: return ResourceManager::getResource<Material>("axeMaterial");
		case Items::Sword: return ResourceManager::getResource<Material>("swordMaterial");
		case Items::Pickaxe: return ResourceManager::getResource<Material>("pickaxeMaterial");
		case Items::WoodenFloor: return ResourceManager::getResource<Material>("woodenPlatformMaterial");
		}
	}
}

/*! \class Item
* \brief An item object
*/
class Item : public NativeScript
{
protected:
	Items::Items m_type; //!< Item type
	uint32_t m_state; //!< The item's state
	bool m_placeable; //!< Is the item placeable in the world
public:
	Item(Items::Items type, uint32_t state); //!< Constructor
	~Item(); //!< Destructor

	inline Items::Items getType() { return m_type; } //!< Get the item type
	inline uint32_t getState() { return m_state; } //!< Get the items state
	void setState(const uint32_t state); //!< Set the items state
};
#endif
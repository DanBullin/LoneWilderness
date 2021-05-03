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

using namespace Engine;

/*! \class Inventory
* \brief A script to handle the player's inventory
*/
class Inventory : public NativeScript
{
private:
	
public:
	Inventory(); //!< Constructor
	~Inventory(); //!< Destructor

	void onAttach() override; //!< Called when attached to the entity
	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call before game update
	void onPostUpdate(const float timestep, const float totalTime) override; //!< Call after game update
	void onKeyPress(KeyPressedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key press
	void onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key release
};
#endif
/*! \file player.h
*
* \brief A player dummy class
*
* \author: Daniel Bullin
*
*/
#ifndef PLAYER_H
#define PLAYER_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/text.h"
#include "independent/entities/components/characterController.h"
#include "scripts/gameObjects/player/hotbar.h"
#include "scripts/gameObjects/player/inventory.h"


using namespace Engine;

/*! \class Player
* \brief A player script
*/
class Player : public NativeScript
{
private:
	CharacterController* m_controller; //!< A character controller
	Hotbar* m_hotbar;
	Inventory* m_inventory;

	uint32_t m_health;
	uint32_t m_atkDmg;
	Text* m_healthText;
	Text* m_dmgText;
public:
	Player(); //!< Constructor
	~Player(); //!< Destructor

	void onAttach() override; //!< Called when attached to the entity
	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call before game update
	void onPostUpdate(const float timestep, const float totalTime) override; //!< Call after game update
	void onKeyPress(KeyPressedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key press
	void onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key release
	void onMouseScrolled(MouseScrolledEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse scroll
	void onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse move
	void onRender(const Renderers renderer, const std::string& renderState) override; //!< Call upon render

	uint32_t getHealth();
	uint32_t getAttackDamage();
	Hotbar* getHotbar();
	Inventory* getInventory();
};
#endif
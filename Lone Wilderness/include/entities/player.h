/*! \file player.h
*
* \brief A player dummy class
*
* \author: Daniel Bullin
*
*/
#ifndef PLAYER_H
#define PLAYER_H

#include "independent/entities/entity.h"

using namespace Engine;

/*! \class Player
* \brief A player dummy class
*/
class Player : public Entity
{
public:
	Player(); //!< Constructor
	~Player(); //!< Destructor
	
	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call before game update
	void onPostUpdate(const float timestep, const float totalTime) override; //!< Call after game update
	void onKeyPress(KeyPressedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key press
	void onMouseScrolled(MouseScrolledEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse scroll
	void onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse move
};
#endif
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
private:
	int j;
public:
	Player(); //!< Constructor
	~Player(); //!< Destructor
	
	virtual void onPreUpdate(const float timestep, const float totalTime) override; //!< Call before game update
	virtual void onPostUpdate(const float timestep, const float totalTime) override; //!< Call after game update
	virtual void onWindowResize(WindowResizeEvent& e, const float timestep, const float totalTime) override; //!< Called when a window is resized
	virtual void onWindowFocus(WindowFocusEvent& e, const float timestep, const float totalTime) override; //!< Called when a window gains focus
	virtual void onWindowLostFocus(WindowLostFocusEvent& e, const float timestep, const float totalTime) override; //!< Called when a window loses focus
	virtual void onWindowMoved(WindowMovedEvent& e, const float timestep, const float totalTime) override; //!< Called when a window is moved
	virtual void onKeyPress(KeyPressedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key press
	virtual void onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key release
	virtual void onMousePress(MousePressedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse press
	virtual void onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse release
	virtual void onMouseScrolled(MouseScrolledEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse scroll
	virtual void onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse move
	virtual void onRender(const Renderers::Renderers renderer) override; //!< Call upon render
};
#endif
/*! \file UILayer.h
*
* \brief A UI layer dummy class
*
* \author: Daniel Bullin
*
*/
#ifndef UILAYER_H
#define UILAYER_H

#include "independent/layers/layer.h"

using namespace Engine;

/*! \class UILayer
* \brief A UI layer dummy class
*/
class UILayer : public Layer
{
private:

public:
	UILayer(); //!< Constructor
	~UILayer(); //!< Destructor

	virtual void onAttach() override; //!< Called when attaching to layer manager
	virtual void onDetach() override; //!< Called when detaching from layer manager
	virtual void onUpdate(const float timestep, const float totalTime) override; //!< Called one per frame
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
};
#endif
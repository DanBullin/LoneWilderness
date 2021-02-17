/*! \file defaultLayer.h
*
* \brief A default layer dummy class
*
* \author: Daniel Bullin
*
*/
#ifndef DEFAULTLAYER_H
#define DEFAULTLAYER_H

#include "independent/layers/layer.h"

using namespace Engine;

/*! \class DefaultLayer
* \brief A default layer dummy class
*/
class DefaultLayer : public Layer
{
public:
	DefaultLayer(); //!< Constructor
	~DefaultLayer(); //!< Destructor

	void onAttach() override; //!< Called when attaching to layer manager
	void onDetach() override; //!< Called when detaching from layer manager
	void onUpdate(const float timestep, const float totalTime) override; //!< Called one per frame
	void onWindowResize(WindowResizeEvent& e, const float timestep, const float totalTime) override; //!< Called when a window is resized
	void onWindowFocus(WindowFocusEvent& e, const float timestep, const float totalTime) override; //!< Called when a window gains focus
	void onWindowLostFocus(WindowLostFocusEvent& e, const float timestep, const float totalTime) override; //!< Called when a window loses focus
	void onWindowMoved(WindowMovedEvent& e, const float timestep, const float totalTime) override; //!< Called when a window is moved
	void onKeyPress(KeyPressedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key press
	void onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key release
	void onMousePress(MousePressedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse press
	void onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse release
	void onMouseScrolled(MouseScrolledEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse scroll
	void onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse move
};
#endif
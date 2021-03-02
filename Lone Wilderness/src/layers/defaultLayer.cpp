/*! \file defaultLayer.cpp
*
* \brief A default layer dummy class
*
* \author: Daniel Bullin
*
*/
#include "layers/defaultLayer.h"

//! DefaultLayer()
DefaultLayer::DefaultLayer() : Layer("Default")
{
}

//! ~DefaultLayer()
DefaultLayer::~DefaultLayer()
{
}

//! onAttach()
void DefaultLayer::onAttach()
{	
}

//! onDetach()
void DefaultLayer::onDetach()
{
}

//! onUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void DefaultLayer::onUpdate(const float timestep, const float totalTime)
{
}

//! onWindowResize()
/*!
\param e a WindowResizeEvent& - A window resize event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void DefaultLayer::onWindowResize(WindowResizeEvent& e, const float timestep, const float totalTime)
{
}

//! onWindowFocus()
/*!
\param e a WindowFocusEvent& - A window focus event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void DefaultLayer::onWindowFocus(WindowFocusEvent& e, const float timestep, const float totalTime)
{
}

//! onWindowLostFocus()
/*!
\param e a WindowLostFocusEvent& - A window lost focus event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void DefaultLayer::onWindowLostFocus(WindowLostFocusEvent& e, const float timestep, const float totalTime)
{
}

//! onWindowMoved()
/*!
\param e a WindowMovedEvent& - A window moved event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void DefaultLayer::onWindowMoved(WindowMovedEvent& e, const float timestep, const float totalTime)
{
}

//! onKeyPress()
/*!
\param e a KeyPressedEvent& - A key press event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void DefaultLayer::onKeyPress(KeyPressedEvent& e, const float timestep, const float totalTime)
{
}

//! onKeyRelease()
/*!
\param e a KeyReleasedEvent& - A key release event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void DefaultLayer::onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime)
{
}

//! onMousePress()
/*!
\param e a MousePressedEvent& - A mouse pressed event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void DefaultLayer::onMousePress(MousePressedEvent& e, const float timestep, const float totalTime)
{
}

//! onMouseRelease()
/*!
\param e a MouseReleasedEvent& - A mouse released event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void DefaultLayer::onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime)
{
}

//! onMouseScrolled()
/*!
\param e a MouseScrolledEvent& - A mouse scrolled event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void DefaultLayer::onMouseScrolled(MouseScrolledEvent& e, const float timestep, const float totalTime)
{
}

//! onMouseMoved()
/*!
\param e a MouseMovedEvent& - A mouse moved event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void DefaultLayer::onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime)
{
}
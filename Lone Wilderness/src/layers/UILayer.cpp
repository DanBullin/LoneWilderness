/*! \file UILayer.cpp
*
* \brief A UI layer dummy class
*
* \author: Daniel Bullin
*
*/
#include "layers/UILayer.h"
#include "independent/systems/systemManager.h"

//! UILayer()
UILayer::UILayer() : Layer("UI")
{
}

//! ~UILayer()
UILayer::~UILayer()
{

}

//! onAttach()
void UILayer::onAttach()
{

}

//! onDetach()
void UILayer::onDetach()
{

}

//! onUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void UILayer::onUpdate(const float timestep, const float totalTime)
{

}

//! onWindowResize()
/*!
\param e a WindowResizeEvent& - A window resize event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void UILayer::onWindowResize(WindowResizeEvent& e, const float timestep, const float totalTime)
{
}

//! onWindowFocus()
/*!
\param e a WindowFocusEvent& - A window focus event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void UILayer::onWindowFocus(WindowFocusEvent& e, const float timestep, const float totalTime)
{
}

//! onWindowLostFocus()
/*!
\param e a WindowLostFocusEvent& - A window lost focus event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void UILayer::onWindowLostFocus(WindowLostFocusEvent& e, const float timestep, const float totalTime)
{
}

//! onWindowMoved()
/*!
\param e a WindowMovedEvent& - A window moved event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void UILayer::onWindowMoved(WindowMovedEvent& e, const float timestep, const float totalTime)
{
}

//! onKeyPress()
/*!
\param e a KeyPressedEvent& - A key press event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void UILayer::onKeyPress(KeyPressedEvent& e, const float timestep, const float totalTime)
{
}

//! onKeyRelease()
/*!
\param e a KeyReleasedEvent& - A key release event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void UILayer::onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime)
{
}

//! onMousePress()
/*!
\param e a MousePressedEvent& - A mouse pressed event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void UILayer::onMousePress(MousePressedEvent& e, const float timestep, const float totalTime)
{
}

//! onMouseRelease()
/*!
\param e a MouseReleasedEvent& - A mouse released event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void UILayer::onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime)
{
}

//! onMouseScrolled()
/*!
\param e a MouseScrolledEvent& - A mouse scrolled event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void UILayer::onMouseScrolled(MouseScrolledEvent& e, const float timestep, const float totalTime)
{
}

//! onMouseMoved()
/*!
\param e a MouseMovedEvent& - A mouse moved event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void UILayer::onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime)
{
}
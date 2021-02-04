/*! \file defaultLayer.cpp
*
* \brief A default layer dummy class
*
* \author: Daniel Bullin
*
*/
#include "layers/defaultLayer.h"
#include "independent/systems/systemManager.h"

DefaultLayer::DefaultLayer() : Layer("Default")
{
}

DefaultLayer::~DefaultLayer()
{
	
}

void DefaultLayer::onAttach()
{
	
}

void DefaultLayer::onDetach()
{
	
}

void DefaultLayer::onUpdate(const float timestep, const float totalTime)
{
	
}

void DefaultLayer::onWindowResize(WindowResizeEvent & e, const float timestep, const float totalTime)
{
}

void DefaultLayer::onWindowFocus(WindowFocusEvent & e, const float timestep, const float totalTime)
{
}

void DefaultLayer::onWindowLostFocus(WindowLostFocusEvent & e, const float timestep, const float totalTime)
{
}

void DefaultLayer::onWindowMoved(WindowMovedEvent & e, const float timestep, const float totalTime)
{
}

void DefaultLayer::onKeyPress(KeyPressedEvent & e, const float timestep, const float totalTime)
{
}

void DefaultLayer::onKeyRelease(KeyReleasedEvent & e, const float timestep, const float totalTime)
{
}

void DefaultLayer::onMousePress(MousePressedEvent & e, const float timestep, const float totalTime)
{
}

void DefaultLayer::onMouseRelease(MouseReleasedEvent & e, const float timestep, const float totalTime)
{
}

void DefaultLayer::onMouseScrolled(MouseScrolledEvent & e, const float timestep, const float totalTime)
{
}

void DefaultLayer::onMouseMoved(MouseMovedEvent & e, const float timestep, const float totalTime)
{
}
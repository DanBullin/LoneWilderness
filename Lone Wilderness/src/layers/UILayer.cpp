/*! \file UILayer.cpp
*
* \brief A UI layer dummy class
*
* \author: Daniel Bullin
*
*/
#include "layers/UILayer.h"
#include "independent/systems/systemManager.h"

UILayer::UILayer() : Layer("UI")
{
}

UILayer::~UILayer()
{

}

void UILayer::onAttach()
{

}

void UILayer::onDetach()
{

}

void UILayer::onUpdate(const float timestep, const float totalTime)
{

}

void UILayer::onWindowResize(WindowResizeEvent & e, const float timestep, const float totalTime)
{
}

void UILayer::onWindowFocus(WindowFocusEvent & e, const float timestep, const float totalTime)
{
}

void UILayer::onWindowLostFocus(WindowLostFocusEvent & e, const float timestep, const float totalTime)
{
}

void UILayer::onWindowMoved(WindowMovedEvent & e, const float timestep, const float totalTime)
{
}

void UILayer::onKeyPress(KeyPressedEvent & e, const float timestep, const float totalTime)
{
}

void UILayer::onKeyRelease(KeyReleasedEvent & e, const float timestep, const float totalTime)
{
}

void UILayer::onMousePress(MousePressedEvent & e, const float timestep, const float totalTime)
{
}

void UILayer::onMouseRelease(MouseReleasedEvent & e, const float timestep, const float totalTime)
{
}

void UILayer::onMouseScrolled(MouseScrolledEvent & e, const float timestep, const float totalTime)
{
}

void UILayer::onMouseMoved(MouseMovedEvent & e, const float timestep, const float totalTime)
{
}
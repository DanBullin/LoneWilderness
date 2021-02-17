/*! \file menuText.cpp
*
* \brief A UI Text entity class for the main menu
*
* \author: Daniel Bullin
*
*/
#include "entities/mainMenu/menuText.h"
#include "independent/systems/systemManager.h"

//! MenuText()
MenuText::MenuText()
{
}

//! ~MenuText()
MenuText::~MenuText()
{
}

//! onMouseRelease()
/*!
\param e a MouseReleasedEvent& - A mouse released event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void MenuText::onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime)
{
	if (getName() == "MenuText" && getComponent<Text>("Play")->containsPoint(InputPoller::getMousePosition()))
	{
		if (SceneManager::getScene("scene1"))
		{
			SceneManager::setActiveScene("scene1", true);
			WindowManager::getFocusedWindow()->setCursorInputMode(CursorInputMode::Disabled);
		}
	}
	else if (getName() == "MenuText" && getComponent<Text>("Quit")->containsPoint(InputPoller::getMousePosition()))
	{
		WindowManager::getFocusedWindow()->close();
	}
}

//! onMouseMoved()
/*!
\param e a MouseMovedEvent& - A mouse moved event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void MenuText::onMouseMoved(MouseMovedEvent & e, const float timestep, const float totalTime)
{
	for (auto& trans : getComponents<Text>())
	{
		if (trans->containsPoint(e.getPos()))
			trans->setColour({ 1.f, 0.f, 1.f, 1.f });
		else
			trans->setColour({ 1.f, 1.f, 1.f, 1.f });
	}
}
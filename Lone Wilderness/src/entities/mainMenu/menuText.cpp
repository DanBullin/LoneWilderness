/*! \file menuText.cpp
*
* \brief A UI Text entity class for the main menu
*
* \author: Daniel Bullin
*
*/
#include "entities/mainMenu/menuText.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/windowManager.h"

//! MenuText()
MenuText::MenuText()
{
}

//! ~MenuText()
MenuText::~MenuText()
{
}

//! onMousePress()
/*!
\param e a MousePressedEvent& - A mouse released event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void MenuText::onMousePress(MousePressedEvent& e, const float timestep, const float totalTime)
{
	Text* text = getComponent<Text>();
	bool containsMouse = containsPoint(InputPoller::getMousePosition());

	if (text)
	{
		if (containsMouse && e.getButton() == Mouse::LEFTBUTTON)
			text->setColour({ 1.f, 1.f, 0.f, 1.f });
	}
}

//! onMouseRelease()
/*!
\param e a MouseReleasedEvent& - A mouse released event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void MenuText::onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime)
{
	glm::vec2 mousePosition = InputPoller::getMousePosition();

	if (getName() == "PlayGame" && containsPoint(mousePosition) && e.getButton() == Mouse::LEFTBUTTON)
	{
		if (SceneManager::getScene("gameScene"))
		{
			SceneManager::setActiveScene("gameScene", false);
			WindowManager::getFocusedWindow()->setCursorInputMode(CursorInputMode::Disabled);
		}
		else
			ENGINE_ERROR("[MenuText::onMouseRelease] There is no scene called: danScene.");
	}
	else if (getName() == "QuitText" && containsPoint(mousePosition) && e.getButton() == Mouse::LEFTBUTTON)
	{
		if (WindowManager::getFocusedWindow())
			WindowManager::getFocusedWindow()->close();
	}

	Text* text = getComponent<Text>();
	bool containsMouse = containsPoint(mousePosition);

	if (text)
	{
		if (!containsMouse && e.getButton() == Mouse::LEFTBUTTON)
			text->setColour({ 1.f, 1.f, 1.f, 1.f });
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
	Text* text = getComponent<Text>();
	bool containsMouse = containsPoint(e.getPos());

	if (text)
	{
		if (containsMouse && !InputPoller::isMousePressed(Mouse::LEFTBUTTON))
			text->setColour({ 1.f, 0.f, 1.f, 1.f });
		else if(!containsMouse && !InputPoller::isMousePressed(Mouse::LEFTBUTTON))
			text->setColour({ 1.f, 1.f, 1.f, 1.f });
	}
}
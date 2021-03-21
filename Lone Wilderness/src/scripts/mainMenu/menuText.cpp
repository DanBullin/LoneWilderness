/*! \file menuText.cpp
*
* \brief A UI Text script class for the main menu
*
* \author: Daniel Bullin
*
*/
#include "scripts/mainMenu/menuText.h"
#include "loaders/sceneLoader.h"
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
	Text* text = getParent()->getComponent<Text>();
	bool containsMouse = getParent()->containsPoint(InputPoller::getMousePosition());

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

	if (getParent()->getName() == "PlayGame" && getParent()->containsPoint(mousePosition) && e.getButton() == Mouse::LEFTBUTTON)
	{
		if (SceneManager::getScene("gameScene"))
		{
			SceneManager::setActiveScene("gameScene", false);
			WindowManager::getFocusedWindow()->setCursorInputMode(CursorInputMode::Disabled);
		}
		else
		{
			SceneLoader::load("gameScene", "assets/scenes/gameScene/");
			SceneManager::setActiveScene("danScene", false);
			WindowManager::getFocusedWindow()->setCursorInputMode(CursorInputMode::Disabled);
		}
	}
	else if (getParent()->getName() == "QuitText" && getParent()->containsPoint(mousePosition) && e.getButton() == Mouse::LEFTBUTTON)
	{
		if (WindowManager::getFocusedWindow())
			WindowManager::getFocusedWindow()->close();
	}

	Text* text = getParent()->getComponent<Text>();
	bool containsMouse = getParent()->containsPoint(mousePosition);

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
	Text* text = getParent()->getComponent<Text>();
	bool containsMouse = getParent()->containsPoint(e.getPos());

	if (text)
	{
		if (containsMouse && !InputPoller::isMousePressed(Mouse::LEFTBUTTON))
			text->setColour({ 1.f, 0.f, 1.f, 1.f });
		else if (!containsMouse && !InputPoller::isMousePressed(Mouse::LEFTBUTTON))
			text->setColour({ 1.f, 1.f, 1.f, 1.f });
	}
}
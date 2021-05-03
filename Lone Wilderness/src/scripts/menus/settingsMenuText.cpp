/*! \file settingsMenuText.cpp
*
* \brief A script for all the interactable text in the settings menu
*
* \author: Daniel Bullin
*
*/
#include "scripts/menus/settingsMenuText.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/windowManager.h"
#include "scripts/layerControl.h"

//! SettingsMenuText()
SettingsMenuText::SettingsMenuText()
{
	m_text = nullptr;
}

//! ~SettingsMenuText()
SettingsMenuText::~SettingsMenuText()
{
}

//! onMousePress()
/*!
\param e a MousePressedEvent& - A mouse released event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void SettingsMenuText::onMousePress(MousePressedEvent& e, const float timestep, const float totalTime)
{
	bool containsMouse = getParent()->containsPoint(InputPoller::getMousePosition());

	if (m_text)
	{
		if (containsMouse && e.getButton() == Mouse::LEFTBUTTON)
			m_text->setColour({ 1.f, 0.7f, 0.f, 1.f });
	}
}

//! onMouseRelease()
/*!
\param e a MouseReleasedEvent& - A mouse released event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void SettingsMenuText::onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime)
{
	glm::vec2 mousePosition = InputPoller::getMousePosition();
	std::string entityName = getParent()->getName();
	bool containsMouse = getParent()->containsPoint(mousePosition);
	std::string sceneName = getParent()->getParentScene()->getName();
	Scene* scene = getParent()->getParentScene();

	if (e.getButton() == Mouse::LEFTBUTTON && containsMouse)
	{
		if (entityName == "Settings_Resume")
		{
			ENGINE_INFO("TEST");
		}
		else if (entityName == "Settings_Quit")
		{
			if (getParent()->getParentScene()->getName() != "mainMenu")
			{
				LayerControl::hide("Settings", scene);
				LayerControl::deactivate("Settings", scene);
				LayerControl::show("Pause", scene);
				LayerControl::activate("Pause", scene);
			}
			else
			{
				LayerControl::hide("Settings", scene);
				LayerControl::deactivate("Settings", scene);
				LayerControl::show("UI", scene);
				LayerControl::activate("UI", scene);
			}
		}
	}

	if (m_text)
	{
		if (!containsMouse && e.getButton() == Mouse::LEFTBUTTON)
			m_text->setColour({ 1.f, 1.f, 1.f, 1.f });
	}
}

//! onMouseMoved()
/*!
\param e a MouseMovedEvent& - A mouse moved event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void SettingsMenuText::onMouseMoved(MouseMovedEvent & e, const float timestep, const float totalTime)
{
	if (!m_text) m_text = getParent()->getComponent<Text>();
	bool containsMouse = getParent()->containsPoint(e.getPos());

	if (m_text)
	{
		if (containsMouse && !InputPoller::isMousePressed(Mouse::LEFTBUTTON))
			m_text->setColour({ 1.f, 0.5f, 0.85f, 1.f });
		else if (!containsMouse && !InputPoller::isMousePressed(Mouse::LEFTBUTTON))
			m_text->setColour({ 1.f, 1.0f, 1.f, 1.f });
	}
}
/*! \file pauseMenuText.cpp
*
* \brief A script for all the interactable text in the pause menu
*
* \author: Daniel Bullin
*
*/
#include "scripts/menus/pauseMenuText.h"
#include "loaders/sceneLoader.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/windowManager.h"
#include "scripts/layerControl.h"

//! loadScene()
/*!
\param sceneName a const std::string& - The scene name
*/
void PauseMenuText::loadScene(const std::string& sceneName)
{
	if (SceneManager::getScene(sceneName))
	{
		SceneManager::setActiveScene(sceneName, true);
	}
	else
	{
		SceneLoader::load(sceneName, "assets/scenes/" + sceneName + "/");
		SceneManager::setActiveScene(sceneName, true);
	}
}

//! PauseMenuText()
PauseMenuText::PauseMenuText()
{
	m_text = nullptr;
}

//! ~PauseMenuText()
PauseMenuText::~PauseMenuText()
{
}

//! onMousePress()
/*!
\param e a MousePressedEvent& - A mouse released event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void PauseMenuText::onMousePress(MousePressedEvent& e, const float timestep, const float totalTime)
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
void PauseMenuText::onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime)
{
	glm::vec2 mousePosition = InputPoller::getMousePosition();
	std::string entityName = getParent()->getName();
	bool containsMouse = getParent()->containsPoint(mousePosition);
	std::string sceneName = getParent()->getParentScene()->getName();
	Scene* scene = getParent()->getParentScene();

	if (e.getButton() == Mouse::LEFTBUTTON && containsMouse)
	{
		if (entityName == "Pause_Resume")
		{
			LayerControl::hide("Pause", scene);
			LayerControl::deactivate("Pause", scene);
			WindowManager::getFocusedWindow()->setCursorInputMode(CursorInputMode::Disabled);
			SceneManager::getActiveScene()->getEntity("Player1")->getComponent<CharacterController>()->setFrozen(false);
			LayerControl::show("UI", scene);
		}
		else if (entityName == "Pause_Settings")
		{
			LayerControl::hide("Pause", scene);
			LayerControl::deactivate("Pause", scene);
			LayerControl::show("Settings", scene);
			LayerControl::activate("Settings", scene);
		}
		else if (entityName == "Pause_Quit")
		{
			LayerControl::hide("Pause", scene);
			LayerControl::deactivate("Pause", scene);
			loadScene("mainMenu");
			WindowManager::getFocusedWindow()->setCursorInputMode(CursorInputMode::Visible);
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
void PauseMenuText::onMouseMoved(MouseMovedEvent & e, const float timestep, const float totalTime)
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
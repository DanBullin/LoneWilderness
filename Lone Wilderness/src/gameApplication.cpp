/*! \file gameApplication.cpp
*
* \brief The game application
*
* \author Daniel Bullin
*
*/
#include "gameApplication.h"
#include "independent/systems/systems/windowManager.h"
#include "independent/systems/systems/renderSystem.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/fontManager.h"
#include "loaders/sceneLoader.h"

//! GameApplication()
GameApplication::GameApplication()
{
	WindowProperties props("Lone Wilderness", 800, 600, glm::ivec2(500, 300), true, false, false, 1.f, true, false, CursorInputMode::Visible);
	WindowManager::registerWindow("Main", props);
	auto Window = WindowManager::getFocusedWindow();
	Window->setIcon("assets/textures/icon.png");

	FontManager::loadFont("Arial", "assets/fonts/arial/arial.ttf", 48);

	SceneLoader::load("mainMenu", "assets/scenes/mainMenu/");
	SceneLoader::load("scene1", "assets/scenes/scene1/");

	RenderSystem::initialise();
}

//! ~GameApplication()
GameApplication::~GameApplication()
{
}

//! startApplication()
/*!
\return a Application* - A pointer to the engine instance
*/
Application* Engine::startApplication()
{
	return new GameApplication();
}
/*! \file gameApplication.cpp
*
* \brief The game application
*
* \author Daniel Bullin
*
*/
#include "gameApplication.h"
#include "loaders/sceneLoader.h"

//! GameApplication()
GameApplication::GameApplication()
{
	SceneLoader::load("mainMenu", "assets/scenes/mainMenu/");
	SceneLoader::load("gameScene", "assets/scenes/gameScene/");
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
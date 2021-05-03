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
	SceneLoader::load("loading", "assets/scenes/loading/");
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
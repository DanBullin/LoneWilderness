/*! \file gameApplication.cpp
*
* \brief The game application
*
* \author Daniel Bullin
*
*/
#include "gameApplication.h"

#include "loaders/sceneLoader.h"
#include "layers/defaultLayer.h"
#include "layers/UILayer.h"
#include "independent/systems/systemManager.h"
#include "independent/utils/nameUtils.h"
#include "independent/rendering/renderPasses/passes/defaultPass.h"
#include "independent/rendering/renderPasses/passes/secondPass.h"

//! GameApplication()
GameApplication::GameApplication()
{
	WindowProperties props("Lone Wilderness", 800, 600, glm::ivec2(500, 300), true, false, false, 1.f, true, false, CursorInputMode::Visible);
	WindowManager::registerWindow("Main", props);
	auto Window = WindowManager::getFocusedWindow();

	///// 
	// Default loading procedure
	/////

	ResourceManager::loadInternalResources();

	RenderSystem::initialise();

	FontManager::loadFont("Arial", "assets/fonts/arial/arial.ttf", 48);

	////
	// Custom loading stuff
	////

	auto Scene1 = SceneManager::createScene("scene1");
	auto MainMenu = SceneManager::createScene("mainMenu");

	Scene1->setClearColour({ 0.1f, 0.2f, 0.3f, 1.f });
	Scene1->getLayerManager()->attachLayer(new DefaultLayer);
	Scene1->getLayerManager()->attachLayer(new UILayer);

	MainMenu->setClearColour({ 0.5f, 0.5f, 0.5f, 1.f });
	MainMenu->getLayerManager()->attachLayer(new DefaultLayer);
	MainMenu->getLayerManager()->attachLayer(new UILayer);

	SceneLoader::load(Scene1, "assets/scenes/scene1/");
	SceneLoader::load(MainMenu, "assets/scenes/mainMenu/");

	Scene1->addRenderPass(new DefaultPass);
	Scene1->addRenderPass(new SecondPass);

	MainMenu->addRenderPass(new SecondPass);

	SceneManager::setActiveScene("mainMenu", false);

	Window->setIcon("assets/textures/icon.png");
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
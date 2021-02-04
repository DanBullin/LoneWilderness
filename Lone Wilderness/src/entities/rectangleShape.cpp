/*! \file rectangleShape.cpp
*
* \brief A RectangleShape dummy class
*
* \author: Daniel Bullin
*
*/
#include "entities/rectangleShape.h"
#include "independent/systems/systemManager.h"

//! RectangleShape()
RectangleShape::RectangleShape()
{

}

//! ~RectangleShape()
RectangleShape::~RectangleShape()
{

}

void RectangleShape::onMouseMoved(MouseMovedEvent & e, const float timestep, const float totalTime)
{
	for (auto& trans : getComponents<Text>())
	{
		if (getName() == "MenuOptions" && trans->containsPoint(e.getPos()))
		{
			trans->setColour({ 1.f, 1.f, 1.f, 1.f });
		}
		else if (getName() == "MenuOptions" && !trans->containsPoint(e.getPos()))
		{
			trans->setColour({ 1.f, 0.f, 1.f, 1.f });
		}
	}
}

void RectangleShape::onKeyRelease(KeyReleasedEvent & e, const float timestep, const float totalTime)
{
	if (e.getKeyCode() == Keys::K)
	{
		if(containsComponent<Text>())
			getComponent<Text>()->setFont("Arial");
	}
	if (e.getKeyCode() == Keys::L)
	{
		SceneManager::setActiveScene("mainMenu", false);
	}
	if (e.getKeyCode() == Keys::J)
	{
		if (containsComponent<Text>())
			getComponent<Text>()->setColour({ 1.f, 1.f, 0.f, 0.5f});
	}
}

void RectangleShape::onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime)
{
	auto trans = getComponent<Text>("Continue");

	if (trans)
	{
		if (getName() == "MenuOptions" && trans->containsPoint(InputPoller::getMousePosition()))
		{
			SceneManager::setActiveScene("scene1", false);
			WindowManager::getFocusedWindow()->setCursorInputMode(CursorInputMode::Disabled);
		}
	}
}

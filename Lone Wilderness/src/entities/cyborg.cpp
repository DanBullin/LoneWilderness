/*! \file cyborg.cpp
*
* \brief A Cyborg dummy class
*
* \author: Daniel Bullin
*
*/
#include "entities/cyborg.h"
#include "independent/systems/systemManager.h"

//! Cyborg()
Cyborg::Cyborg()
{
	m_hide = false;
}

//! ~Cyborg()
Cyborg::~Cyborg()
{

}

//! onPreUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Cyborg::onPreUpdate(const float timestep, const float totalTime)
{
	if (getComponent<MeshRender3D>()->getMaterial()->getShader() == ResourceManager::getShader("blinnPhong2").get())
	{
		getComponent<Transform3D>()->setOrientation(getComponent<Transform3D>()->getOrientation() + glm::vec3(0.f, 1.f, 0.f));
	}
}

//! onPostUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Cyborg::onPostUpdate(const float timestep, const float totalTime)
{

}

void Cyborg::onWindowResize(WindowResizeEvent& e, const float timestep, const float totalTime)
{
}

void Cyborg::onWindowFocus(WindowFocusEvent& e, const float timestep, const float totalTime)
{
}

void Cyborg::onWindowLostFocus(WindowLostFocusEvent& e, const float timestep, const float totalTime)
{
}

void Cyborg::onWindowMoved(WindowMovedEvent& e, const float timestep, const float totalTime)
{
}

void Cyborg::onKeyPress(KeyPressedEvent& e, const float timestep, const float totalTime)
{
	
}

void Cyborg::onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime)
{
	if (e.getKeyCode() == Keys::P)
		m_hide = true;
	else if (e.getKeyCode() == Keys::O)
		m_hide = false;
	else if (e.getKeyCode() == Keys::U)
	{
		if (getName() == "Player3")
			setDelete(true);
	}
}

void Cyborg::onMousePress(MousePressedEvent& e, const float timestep, const float totalTime)
{
}

void Cyborg::onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime)
{

}

void Cyborg::onMouseScrolled(MouseScrolledEvent& e, const float timestep, const float totalTime)
{
}

void Cyborg::onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime)
{
	
}

//! onRender()
void Cyborg::onRender(const Renderers::Renderers renderer)
{
	if (!m_hide)
	{
		if (renderer == Renderers::Renderer3D)
		{
			for (auto& meshRender : getComponents<MeshRender3D>())
			{
				for (auto& mesh : meshRender->getModel()->getMeshes())
					Renderer3D::submit(mesh.getGeometry(), meshRender->getMaterial(), meshRender->getModelMatrix());
			}
		}
	}
}

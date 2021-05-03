/*! \file cameraRotationScript.cpp
*
* \brief A camera rotation script
*
* \author: Daniel Bullin
*
*/
#include "scripts/mainMenu/cameraRotationScript.h"
#include "loaders/sceneLoader.h"
#include "independent/systems/systems/sceneManager.h"

//! CameraRotationScript()
CameraRotationScript::CameraRotationScript()
{
}

//! ~CameraRotationScript()
CameraRotationScript::~CameraRotationScript()
{
}

//! onAttach()
void CameraRotationScript::onAttach()
{
	m_camera = getParent()->getParentScene()->getEntity("Player1")->getComponent<Camera>();
}

//! onPostUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void CameraRotationScript::onPostUpdate(const float timestep, const float totalTime)
{
	if (m_camera)
	{
		m_camera->getCameraData().Yaw += (10.f * timestep);
		m_camera->updateCameraVectors();
	}
}

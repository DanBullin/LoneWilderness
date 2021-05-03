/*! \file cameraRotationScript.h
*
* \brief A camera rotation script
*
* \author: Daniel Bullin
*
*/
#ifndef CAMERAROTATIONSCRIPT_H
#define CAMERAROTATIONSCRIPT_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/camera.h"

using namespace Engine;

/*! \class CameraRotationScript
* \brief A CameraRotationScript class
*/
class CameraRotationScript : public NativeScript
{
private:
	Camera* m_camera; //!< The main camera
public:
	CameraRotationScript(); //!< Constructor
	~CameraRotationScript(); //!< Destructor
	void onAttach() override; //!< Called when attached to the entity
	void onPostUpdate(const float timestep, const float totalTime) override; //!< Call after game update
};
#endif
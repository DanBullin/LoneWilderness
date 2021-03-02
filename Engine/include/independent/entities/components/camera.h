/*! \file camera.h
*
* \brief A camera object in the game world
*
* \author Daniel Bullin and www.learnOpenGL.com
*
*/
#ifndef CAMERA_H
#define CAMERA_H

#include "independent/entities/entityComponent.h"
#include "independent/entities/components/components/skybox.h"

namespace Engine
{
	/*! \struct Projection
	* \brief The data needed for the projection matrix
	*/
	struct Projection
	{
		float NearPlane; //!< The near clipping plane
		float FarPlane; //!< The far clipping plane
		float AspectRatio; //!< The aspect ratio
		float Left; //!< The left ortho bound
		float Right; //!< The right ortho bound
		float Top; //!< The top ortho bound
		float Bottom; //!< The bottom ortho bound

		Projection(const float left = 0.f, const float right = 800.f, const float top = 0.f, const float bottom = 600.f, const float ratio = 1.33f, const float nearPlane = 0.1f, const float farPlane = 1000.f)
			: Left(left), Right(right), Top(top), Bottom(bottom), AspectRatio(ratio), NearPlane(nearPlane), FarPlane(farPlane) {} //!< Ortho constructor
			/*!< \param left a const float - The left value
				 \param right a const float - The right value
				 \param top a const float - The top value
				 \param bottom a const float - The bottom value
				 \param ratio a const float - The aspect ratio
				 \param nearPlane a const float - The distance of the near plane
				 \param farPlane a const float - The distance of the far plane */
	};

	/*! \struct CameraData
	* \brief Camera data about its relationship with the world
	*/
	struct CameraData
	{
		glm::vec3 Front; //!< The front direction vector
		glm::vec3 Up; //!< The up direction vector
		glm::vec3 Right; //!< The right direction vector
		glm::vec3 WorldUp; //!< The world's up vector
		float Yaw; //!< Yaw rotation
		float Pitch; //!< Pitch rotation
		float Zoom; //!< The amount of zoom

		CameraData(const glm::vec3 front, const glm::vec3 up, const glm::vec3 worldUp, const float yaw, const float pitch,
			const float zoom)
			: Front(front), Up(up), WorldUp(worldUp), Yaw(yaw), Pitch(pitch), Zoom(zoom) {} //!< Constructor
			/*!< \param front a const glm::vec3 - The front direction of the camera
				 \param up a const glm::vec3 - The up direction of the camera
				 \param worldUp a const glm::vec3 - The up direction of the world
				 \param yaw a const float - The yaw angle of the camera
				 \param pitch a const float - The pitch angle of the camera
				 \param zoom a const float - The zoom on the camera */
	};

	/*! \class Camera
	* \brief A camera object in the game world
	*/
	class Camera : public EntityComponent
	{
	private:
		CameraData m_cameraData; //!< The camera data
		Projection m_projection; //!< The projection data
		bool m_mainCamera; //!< Is this camera the main camera in the scene
		glm::vec4 m_clearColour; //!< The clear colour when rendering
		Skybox* m_skybox; //!< The skybox attatched to the camera
	public:
		Camera(const CameraData& cameraData); //!< Constructor
		~Camera(); //!< Destructor

		void onAttach() override; //!< Occurs when component is attached to an entity
		void onDetach() override; //!< Occurs when component is detached from an entity
		void onUpdate(const float timestep, const float totalTime) override; //!< Update the component
		void printComponentDetails() override; //!< Print component details

		const glm::vec4& getClearColour(); //!< Get the clear colour
		void setClearColour(const glm::vec4& colour); //!< Set the clear colour

		glm::mat4 getViewMatrix(const bool perspective); //!< Get the view matrix
		glm::mat4 getProjectionMatrix(const bool perspective); //!< Get the projection matrix
		glm::vec3 getWorldPosition(); //!< Get the world position of the camera

		void setProjection(const Projection& projection); //!< Set the projection data
		Projection& getProjection(); //!< Get the projection data
		void setCameraData(const CameraData& data); //!< Set the camera data
		CameraData& getCameraData(); //!< Get the camera data

		void setMainCamera(const bool mainCamera); //!< Set if this camera is the main camera
		const bool getMainCamera(); //!< Get if this camera is the main camera

		void setSkybox(Skybox* skybox); //!< Set the attatched skybox
		Skybox* getSkybox(); //!< Get the attatched skybox

		void updateCameraVectors(); //!< Update all camera vectors
	};
}
#endif
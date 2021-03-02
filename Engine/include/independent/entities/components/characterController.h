/*! \file characterController.h
*
* \brief A component which controls the entity's transform
*
* \author Daniel Bullin
*
*/
#ifndef CHARACTERCONTROLLER_H
#define CHARACTERCONTROLLER_H

#include "independent/entities/entityComponent.h"
#include "independent/entities/components/camera.h"

namespace Engine
{
	/*! \enum Movement
	* \brief Enum for possible movements
	*/
	enum Movement
	{
		FORWARD, //!< The forward direction of the camera
		BACKWARD, //!< The backward direction of the camera
		LEFT, //!< The left direction of the camera
		RIGHT, //!< The right direction of the camera
		UP, //!< The upward direction of the camera
		DOWN //!< The downward direction of the camera
	};

	/*! \class CharacterController
	* \brief A component which controls the entity's transform
	*/
	class CharacterController : public EntityComponent
	{
	private:
		float m_movementSpeed; //!< The movement speed
		float m_mouseSensitivity; //!< The mouse sensitivity
	public:
		CharacterController(const float speed, const float sensitivity); //!< Default constructor
		~CharacterController(); //!< Destructor

		void onAttach() override; //!< Occurs when component is attached to an entity
		void onDetach() override; //!< Occurs when component is detached from an entity
		void onUpdate(const float timestep, const float totalTime) override; //!< Update the character controller
		void printComponentDetails() override; //!< Print component details

		void setSpeed(const float speed); //!< Set the speed
		void setSensitivity(const float sensitivity); //!< Set the sensitivity
		float getSpeed() const; //!< Get the speed
		float getSensitivity() const; //!< Get the sensitivity

		void move(const Movement direction, const float deltaTime); //!< Move the entity
		void rotate(float xoffset, float yoffset, bool constrainPitch = true); //!< Rotate the entity
		void zoom(float yoffset); //!< Zoom in/out
	};
}
#endif
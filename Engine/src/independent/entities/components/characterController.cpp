/*! \file characterController.cpp
*
* \brief A component which controls the entity's transform
*
* \author Daniel Bullin
*
*/
#include "independent/entities/components/characterController.h"
#include "independent/entities/entity.h"
#include "independent/systems/components/scene.h"
#include "independent/entities/components/transform.h"
#include "independent/systems/systems/log.h"

namespace Engine
{
	//! CharacterController
	/*
	\param speed a const float - The speed of the character
	\param sensitivity a const float - The mouse sensitivity of the character
	*/
	CharacterController::CharacterController(const float speed, const float sensitivity) : EntityComponent(ComponentType::CharacterController)
	{
		m_mouseSensitivity = 0.f;
		m_movementSpeed = 0.f;
		setSpeed(speed);
		setSensitivity(sensitivity);
	}

	//! ~CharacterController()
	CharacterController::~CharacterController()
	{
	}

	//! onAttach()
	void CharacterController::onAttach()
	{
	}

	//! onDetach()
	void CharacterController::onDetach()
	{
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The time step
	\param totalTime a const float - The total time of the application
	*/
	void CharacterController::onUpdate(const float timestep, const float totalTime)
	{
	}

	//! printComponentDetails()
	void CharacterController::printComponentDetails()
	{
		Entity* parent = getParent();

		if (!parent)
		{
			ENGINE_ERROR("[CharacterController::printComponentDetails] This component does not have a valid parent entity. Component Name: {0}.", m_name);
			return;
		}

		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("CharacterController Details for Entity: {0}", parent->getName());
		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("Speed: {0}.", m_movementSpeed);
		ENGINE_TRACE("Sensitivity: {0}.", m_mouseSensitivity);
		ENGINE_TRACE("==========================================");
	}

	//! setSpeed()
	/*
	\param speed a const float - The speed of the entity
	*/
	void CharacterController::setSpeed(const float speed)
	{
		if (speed >= 0.f && speed <= 100.f)
			m_movementSpeed = speed;
		else
		{
			if (getParent())
				ENGINE_ERROR("[CharacterController::setSpeed] An invalid speed value was provided. Entity Name: {0}.", getParent()->getName());
			else
				ENGINE_ERROR("[CharacterController::setSpeed] An invalid speed value was provided.");
		}
	}

	//! setSensitivity()
	/*
	\param sensitivity a const float - The sensitivity of the entity
	*/
	void CharacterController::setSensitivity(const float sensitivity)
	{
		if (sensitivity >= 0.f && sensitivity <= 1.f)
			m_mouseSensitivity = sensitivity;
		else
		{
			if (m_parentEntity)
				ENGINE_ERROR("[CharacterController::setSensitivity] An invalid sensitivity value was provided. Entity Name: {0}.", m_parentEntity->getName());
			else
				ENGINE_ERROR("[CharacterController::setSensitivity] An invalid sensitivity value was provided.");
		}
	}

	//! getSpeed()
	/*
	\return a float - The speed of the entity
	*/
	float CharacterController::getSpeed() const
	{
		return m_movementSpeed;
	}

	//! getSensitivity()
	/*
	\return a float - The sensitivity of the entity
	*/
	float CharacterController::getSensitivity() const
	{
		return m_mouseSensitivity;
	}

	//! move()
	/*!
	\param direction a const Movement - The direction to move
	\param deltaTime a const float - Delta time which is a measure of how long it took to render last frame
	*/
	void CharacterController::move(const Movement direction, const float deltaTime)
	{
		Entity* parent = getParent();

		if (!parent)
		{
			ENGINE_ERROR("[CharacterController::move] This component does not have a valid parent entity. Component Name: {0}.", m_name);
			return;
		}

		// Translate the entity
		Transform* trans = parent->getComponent<Transform>();

		if (trans)
		{
			float velocity = m_movementSpeed * deltaTime;

			if (direction == Movement::FORWARD)
				trans->setPosition(trans->getPosition() += getParent()->getParentScene()->getMainCamera()->getCameraData().Front * velocity);
			if (direction == Movement::BACKWARD)
				trans->setPosition(trans->getPosition() -= getParent()->getParentScene()->getMainCamera()->getCameraData().Front * velocity);
			if (direction == Movement::LEFT)
				trans->setPosition(trans->getPosition() -= getParent()->getParentScene()->getMainCamera()->getCameraData().Right * velocity);
			if (direction == Movement::RIGHT)
				trans->setPosition(trans->getPosition() += getParent()->getParentScene()->getMainCamera()->getCameraData().Right * velocity);
		}
		else
			ENGINE_ERROR("[CharacterController::move] This character controller cannot detect a valid transform. Entity Name: {0}.", parent->getName());
	}

	//! rotate()
	/*!
	\param xoffset a float - The yaw rotation offset
	\param yoffset a float - The pitch rotation offset
	\param constrainPitch a bool - Constrain the pitch?
	*/
	void CharacterController::rotate(float xoffset, float yoffset, bool constrainPitch)
	{
		Entity* parent = getParent();

		if (!parent)
		{
			ENGINE_ERROR("[CharacterController::rotate] This component does not have a valid parent entity. Component Name: {0}.", m_name);
			return;
		}

		Scene* scene = parent->getParentScene();

		if (!scene)
		{
			ENGINE_ERROR("[CharacterController::rotate] The entity this component is attached to does not belong to a valid scene. Entity Name: {0}.", parent->getName());
			return;
		}

		// Rotate the camera
		xoffset *= m_mouseSensitivity;
		yoffset *= m_mouseSensitivity;

		Camera* cam = scene->getMainCamera();
		if (cam)
		{
			cam->getCameraData().Yaw += xoffset;
			cam->getCameraData().Pitch += yoffset;

			// Makes sure that when pitch is out of bounds, screen doesn't get flipped
			if (constrainPitch)
			{
				if (cam->getCameraData().Pitch > 89.0f)
					cam->getCameraData().Pitch = 89.0f;
				if (cam->getCameraData().Pitch < -89.0f)
					cam->getCameraData().Pitch = -89.0f;
			}

			// update Front, Right and Up Vectors using the updated Euler angles
			cam->updateCameraVectors();
		}
		else
			ENGINE_ERROR("[CharacterController::rotate] The scene does not have a valid main camera. Scene Name: {0}.", scene->getName());
	}

	//! zoom()
	/*!
	\param yoffset a float - The pitch rotation offset
	*/
	void CharacterController::zoom(float yoffset)
	{
		Entity* parent = getParent();

		if (!parent)
		{
			ENGINE_ERROR("[CharacterController::zoom] This component does not have a valid parent entity. Component Name: {0}.", m_name);
			return;
		}

		Scene* scene = parent->getParentScene();

		if (!scene)
		{
			ENGINE_ERROR("[CharacterController::zoom] The entity this component is attached to does not belong to a valid scene. Entity Name: {0}.", parent->getName());
			return;
		}

		Camera* cam = scene->getMainCamera();
		// Affect the zoom
		if (cam)
		{
			cam->getCameraData().Zoom -= (float)yoffset;
			if (cam->getCameraData().Zoom < 1.0f)
				cam->getCameraData().Zoom = 1.0f;
			if (cam->getCameraData().Zoom > 45.0f)
				cam->getCameraData().Zoom = 45.0f;
		}
		else
			ENGINE_ERROR("[CharacterController::zoom] The scene does not have a valid main camera. Scene Name: {0}.", scene->getName());
	}
}
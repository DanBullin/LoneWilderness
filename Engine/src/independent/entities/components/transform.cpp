/*! \file transform.cpp
*
* \brief The position, scale and rotation of an 3D entity
*
* \author Daniel Bullin
*
*/
#include "independent/entities/components/transform.h"
#include "independent/entities/entity.h"
#include "independent/systems/systems/log.h"

namespace Engine
{
	//! Transform()
	/*!
	\param xPos a const float - The x position of the entity in the game world
	\param yPos a const float - The y position of the entity in the game world
	\param zPos a const float - The z position of the entity in the game world
	\param xRotation a const float - The rotation around the x axis of the entity in the game world
	\param yRotation a const float - The rotation around the y axis of the entity in the game world
	\param zRotation a const float - The rotation around the z axis of the entity in the game world
	\param sX a const float - The scale in the x axis of the entity in the game world
	\param sY a const float - The scale in the y axis of the entity in the game world
	\param sZ a const float - The scale in the z axis of the entity in the game world
	*/
	Transform::Transform(const float xPos, const float yPos, const float zPos, const float xRotation, const float yRotation, const float zRotation, const float sX, const float sY, const float sZ)
		: EntityComponent(ComponentType::Transform)
	{
		setPosition(xPos, yPos, zPos);
		setOrientation(xRotation, yRotation, xRotation);
		setScale(sX, sY, sZ);
	}

	//! ~Transform()
	Transform::~Transform()
	{
	}

	//! onAttach()
	void Transform::onAttach()
	{
	}

	//! onDetach
	void Transform::onDetach()
	{
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The time step
	\param totalTime a const float - The total time of the application
	*/
	void Transform::onUpdate(const float timestep, const float totalTime)
	{
	}

	//! printComponentDetails()
	void Transform::printComponentDetails()
	{
		Entity* parent = getParent();

		if (!parent)
		{
			ENGINE_ERROR("[Transform::printComponentDetails] This component does not have a valid parent entity. Component Name: {0}.", m_name);
			return;
		}

		ENGINE_TRACE("====================================================");
		ENGINE_TRACE("Transform Details for Entity: {0}", parent->getName());
		ENGINE_TRACE("====================================================");
		ENGINE_TRACE("Position: {0}, {1}, {2}", m_position.x, m_position.y, m_position.z);
		ENGINE_TRACE("Orientation: {0}, {1}, {2}", m_orientation.x, m_orientation.y, m_orientation.z);
		ENGINE_TRACE("Scale: {0}, {1}, {2}", m_scale.x, m_scale.y, m_scale.z);
		ENGINE_TRACE("====================================================");
	}

	//! getPosition()
	/*!
	\return a glm::vec3 - The position of the entity in the game world
	*/
	glm::vec3 Transform::getPosition()
	{
		if (getParent()->getParentEntity())
			return getParent()->getParentEntity()->getComponent<Transform>()->getPosition() + m_position;
		else
			return m_position;
	}

	//! setPosition()
	/*!
	\param xPos a const float - The x position of the entity
	\param yPos a const float - The y position of the entity
	\param zPos a const float - The z position of the entity
	*/
	void Transform::setPosition(const float xPos, const float yPos, const float zPos)
	{
		setPosition({ xPos, yPos, zPos });
	}

	//! setPosition()
	/*!
	\param newPos a const glm::vec3& - The position of the entity
	*/
	void Transform::setPosition(const glm::vec3& newPos)
	{
		m_position = newPos;
	}

	//! getOrientation()
	/*!
	\return a glm::vec3 - The orientation of the transform in euler angles
	*/
	glm::vec3 Transform::getOrientation()
	{
		if (getParent()->getParentEntity())
			return getParent()->getParentEntity()->getComponent<Transform>()->getOrientation() + m_orientation;
		else
			return m_orientation;
	}

	//! setOrientation()
	/*!
	\param xRotation a const float - The rotation around the x axis of the entity
	\param yRotation a const float - The rotation around the y axis of the entity
	\param zRotation a const float - The rotation around the z axis of the entity
	*/
	void Transform::setOrientation(const float xRotation, const float yRotation, const float zRotation)
	{
		setOrientation({ xRotation, yRotation, zRotation });
	}

	//! setOrientation()
	/*!
	\param newOrientation a const glm::vec3& - The orientation of the entity
	*/
	void Transform::setOrientation(const glm::vec3& newOrientation)
	{
		m_orientation = newOrientation;
	}

	//! getScale()
	/*!
	\return a glm::vec3& - The scale of the transform
	*/
	glm::vec3 Transform::getScale()
	{
		if (getParent()->getParentEntity())
			return getParent()->getParentEntity()->getComponent<Transform>()->getScale() * m_scale;
		else
			return m_scale;
	}

	//! setScale()
	/*!
	\param sX a const float - The scale of the transform in the x axis
	\param sY a const float - The scale of the transform in the y axis
	\param sZ a const float - The scale of the transform in the z axis
	*/
	void Transform::setScale(const float sX, const float sY, const float sZ)
	{
		setScale({ sX, sY, sZ });
	}

	//! setScale()
	/*!
	\param newScale a const glm::vec3& - The scale of the entity
	*/
	void Transform::setScale(const glm::vec3& newScale)
	{
		if (newScale.x >= 0.f && newScale.y >= 0.f && newScale.z >= 0.f)
			m_scale = newScale;
		else
			ENGINE_ERROR("[Transform::setScale] An invalid scale value was provided. Scale: {0}, {1}, {2}.", newScale.x, newScale.y, newScale.z);
	}

	//! getModelMatrix()
	/*!
	\return a glm::mat4 - The model transformation matrix
	*/
	glm::mat4 Transform::getModelMatrix()
	{
		// Order: Translate then Rotation then Scale
		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, getPosition());
		model = glm::rotate(model, glm::radians(getOrientation().x), glm::vec3(1.f, 0.f, 0.f));
		model = glm::rotate(model, glm::radians(getOrientation().y), glm::vec3(0.f, 1.f, 0.f));
		model = glm::rotate(model, glm::radians(getOrientation().z), glm::vec3(0.f, 0.f, 1.f));
		model = glm::scale(model, getScale());
		return model;
	}
}
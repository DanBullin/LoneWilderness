/*! \file transform3D.cpp
*
* \brief The position, scale and rotation of an 3D entity
*
* \author Daniel Bullin
*
*/

#include "independent/entities/components/transform3D.h"
#include "independent/entities/entity.h"
#include "independent/systems/systems/log.h"
namespace Engine
{
	//! Transform3D()
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
	Transform3D::Transform3D(const float xPos, const float yPos, const float zPos, const float xRotation, const float yRotation, const float zRotation, const float sX, const float sY, const float sZ)
		: EntityComponent(ComponentType::Transform3D), m_position(xPos, yPos, zPos), m_orientation(xRotation, yRotation, zRotation), m_scale(sX, sY, sZ)
	{
	}

	//! ~Transform3D()
	Transform3D::~Transform3D()
	{

	}

	//! onAttach()
	void Transform3D::onAttach()
	{
	}

	//! onDetach
	void Transform3D::onDetach()
	{
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The time step
	\param totalTime a const float - The total time of the application
	*/
	void Transform3D::onUpdate(const float timestep, const float totalTime)
	{
	}

	//! printComponentDetails()
	void Transform3D::printComponentDetails()
	{
		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("Transform3D Details for Entity: {0}", getParent()->getName());
		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("Position: {0}, {1}, {2}", m_position.x, m_position.y, m_position.z);
		ENGINE_TRACE("Orientation: {0}, {1}, {2}", m_orientation.x, m_orientation.y, m_orientation.z);
		ENGINE_TRACE("Scale: {0}, {1}, {2}", m_scale.x, m_scale.y, m_scale.z);
		ENGINE_TRACE("==========================================");
	}

	//! getPosition()
	/*!
	\return a glm::vec3 - The position of the entity in the game world
	*/
	glm::vec3 Transform3D::getPosition() const
	{
		return m_position;
	}

	//! setPosition()
	/*!
	\param xPos a const float - The x position of the entity
	\param yPos a const float - The y position of the entity
	\param zPos a const float - The z position of the entity
	*/
	void Transform3D::setPosition(const float xPos, const float yPos, const float zPos)
	{
		setPosition({ xPos, yPos, zPos });
	}

	//! setPosition()
	/*!
	\param newPos a const glm::vec3& - The position of the entity
	*/
	void Transform3D::setPosition(const glm::vec3& newPos)
	{
		m_position = newPos;
	}

	//! getOrientation()
	/*!
	\return a glm::vec3 - The orientation of the transform
	*/
	glm::vec3 Transform3D::getOrientation() const
	{
		return m_orientation;
	}

	//! setOrientation()
	/*!
	\param xRotation a const float - The rotation around the x axis of the entity
	\param yRotation a const float - The rotation around the y axis of the entity
	\param zRotation a const float - The rotation around the z axis of the entity
	*/
	void Transform3D::setOrientation(const float xRotation, const float yRotation, const float zRotation)
	{
		setOrientation({ xRotation, yRotation, zRotation });
	}

	//! setOrientation()
	/*!
	\param newOrientation a const glm::vec3& - The orientation of the entity
	*/
	void Transform3D::setOrientation(const glm::vec3& newOrientation)
	{
		m_orientation = newOrientation;
	}

	//! getScale()
	/*!
	\return a glm::vec3& - The scale of the transform
	*/
	glm::vec3 Transform3D::getScale() const
	{
		return m_scale;
	}

	//! setScale()
	/*!
	\param sX a const float - The scale of the transform in the x axis
	\param sY a const float - The scale of the transform in the y axis
	\param sZ a const float - The scale of the transform in the z axis
	*/
	void Transform3D::setScale(const float sX, const float sY, const float sZ)
	{
		setScale({ sX, sY, sZ });
	}

	//! setScale()
	/*!
	\param newScale a const glm::vec3& - The scale of the entity
	*/
	void Transform3D::setScale(const glm::vec3& newScale)
	{
		m_scale = newScale;
	}
}
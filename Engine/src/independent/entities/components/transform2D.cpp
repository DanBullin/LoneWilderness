/*! \file transform2D.cpp
*
* \brief The position, scale and rotation of an 2D entity
*
* \author Daniel Bullin
*
*/
#include "independent/entities/components/transform2D.h"
#include "independent/entities/entity.h"
#include "independent/systems/systems/log.h"

namespace Engine
{
	//! Transform2D()
	/*!
	\param xPos a const float - The x position of the entity in the game world
	\param yPos a const float - The y position of the entity in the game world
	\param rotation a const float - The rotation of the entity in the game world
	\param sX a const float - The scale in the x axis of the entity in the game world
	\param sY a const float - The scale in the y axis of the entity in the game world
	*/
	Transform2D::Transform2D(const float xPos, const float yPos, const float rotation, const float sX, const float sY)
		: EntityComponent(ComponentType::Transform2D), m_position(xPos, yPos), m_orientation(rotation), m_scale(sX, sY)
	{
	}

	//! ~Transform2D()
	Transform2D::~Transform2D()
	{
	}

	//! onAttach()
	void Transform2D::onAttach()
	{
	}

	//! onDetach
	void Transform2D::onDetach()
	{
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The time step
	\param totalTime a const float - The total time of the application
	*/
	void Transform2D::onUpdate(const float timestep, const float totalTime)
	{
	}

	//! printComponentDetails()
	void Transform2D::printComponentDetails()
	{
		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("Transform2D Details for Entity: {0}", getParent()->getName());
		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("Position: {0}, {1}", m_position.x, m_position.y);
		ENGINE_TRACE("Orientation: {0}", m_orientation);
		ENGINE_TRACE("Scale: {0}, {1}", m_scale.x, m_scale.y);
		ENGINE_TRACE("==========================================");
	}

	//! getPosition()
	/*!
	\return a glm::vec2 - The position of the entity in the game world
	*/
	glm::vec2 Transform2D::getPosition() const
	{
		return m_position;
	}

	//! setPosition()
	/*!
	\param xPos a const float - The x position of the entity
	\param yPos a const float - The y position of the entity
	*/
	void Transform2D::setPosition(const float xPos, const float yPos)
	{
		setPosition({ xPos, yPos });
	}

	//! setPosition()
	/*!
	\param newPos a const glm::vec2& - The position of the entity
	*/
	void Transform2D::setPosition(const glm::vec2& newPos)
	{
		m_position = newPos;
	}

	//! getOrientation()
	/*!
	\return a float - The rotation of the entity
	*/
	float Transform2D::getOrientation() const
	{
		return m_orientation;
	}

	//! setOrientation()
	/*!
	\param rotation a const float - The rotation of the entity
	*/
	void Transform2D::setOrientation(const float rotation)
	{
		m_orientation = rotation;
	}

	//! getScale()
	/*!
	\return a glm::vec2 - The scale of the transform
	*/
	glm::vec2 Transform2D::getScale() const
	{
		return m_scale;
	}

	//! setScale()
	/*!
	\param sX a const float - The scale of the transform in the x axis
	\param sY a const float - The scale of the transform in the y axis
	*/
	void Transform2D::setScale(const float sX, const float sY)
	{
		m_scale = { sX, sY };
	}
}
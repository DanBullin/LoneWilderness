/*! \file meshRender2D.cpp
*
* \brief A 2D mesh render which contains the material to apply to a 2D quad
*
* \author Daniel Bullin
*
*/

#include "independent/entities/components/meshRender2D.h"
#include "independent/entities/entity.h"
#include "independent/systems/systems/log.h"

namespace Engine
{
	//! MeshRender2D()
	/*!
	\param localPos a const glm::vec2& - The local position
	\param localOrientation a const float - The local orientation
	\param localScale a const glm::vec2& - The local scale
	\param material a const Shared<Material>& - A pointer to the material
	*/
	MeshRender2D::MeshRender2D(const glm::vec2& localPos, const float localOrientation, const glm::vec2& localScale, const Shared<Material>& material) 
		: EntityComponent(ComponentType::MeshRender2D), m_material(material.get()), m_localPosition(localPos), m_localOrientation(localOrientation), m_localScale(localScale)
	{
	}

	//! ~MeshRender2D()
	MeshRender2D::~MeshRender2D()
	{
		m_material = nullptr;
	}

	//! onAttach()
	void MeshRender2D::onAttach()
	{
	}

	//! onDetach
	void MeshRender2D::onDetach()
	{
	}

	//! containsPoint()
	/*!
	\param coordinate a const glm::vec2& - The coordinate to check
	\return a bool - Is this coordinate within this bounding box
	*/
	bool MeshRender2D::containsPoint(const glm::vec2& coordinate)
	{
		glm::vec2 topLeft = (getParent()->getComponent<Transform2D>()->getPosition() + m_localPosition) - (m_localScale / 2.f);
		glm::vec2 bottomRight = (getParent()->getComponent<Transform2D>()->getPosition() + m_localPosition) + (m_localScale / 2.f);

		if (coordinate.x >= topLeft.x && coordinate.x <= bottomRight.x)
			if (coordinate.y >= topLeft.y && coordinate.y <= bottomRight.y)
				return true;

		return false;
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The time step
	\param totalTime a const float - The total time of the application
	*/
	void MeshRender2D::onUpdate(const float timestep, const float totalTime)
	{
	}

	//! printComponentDetails()
	void MeshRender2D::printComponentDetails()
	{
		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("MeshRender2D Details for Entity: {0}", getParent()->getName());
		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("Material Address: {0}.", (void*)m_material);
		ENGINE_TRACE("Local Position: {0}, {1}.", m_localPosition.x, m_localPosition.y);
		ENGINE_TRACE("Local Orientation: {0}.", m_localOrientation);
		ENGINE_TRACE("Local Scale: {0}, {1}.", m_localScale.x, m_localScale.y);
		ENGINE_TRACE("==========================================");
	}

	//!	setMaterial()
	/*!
	\param material a const std::shared_ptr<Material>& - A pointer to the material
	*/
	void MeshRender2D::setMaterial(const Shared<Material>& material)
	{
		if (material.get())
			m_material = material.get();
		else
			ENGINE_ERROR("[MeshRender2D::setMaterial] An invalid material was provided.");
	}

	//!	getMaterial()
	/*!
	\return a Material* - A pointer to the material
	*/
	Material* MeshRender2D::getMaterial()
	{
		return m_material;
	}

	//! setLocalPosition()
	/*!
	\param newPos a const glm::vec2& - The new position
	*/
	void MeshRender2D::setLocalPosition(const glm::vec2& newPos)
	{
		m_localPosition = newPos;
	}

	//!	getLocalPosition()
	/*!
	\return a glm::vec2 - The local position of the mesh render
	*/
	glm::vec2 MeshRender2D::getLocalPosition()
	{
		return m_localPosition;
	}

	//! setLocalOrientation()
	/*!
	\param newOrientation a const float - The new local orientation of the mesh render
	*/
	void MeshRender2D::setLocalOrientation(const float newOrientation)
	{
		m_localOrientation = newOrientation;
	}

	//!	getLocalOrientation()
	/*!
	\return a float - The local orientation of the mesh render
	*/
	float MeshRender2D::getLocalOrientation()
	{
		return m_localOrientation;
	}

	//! setLocalScale()
	/*!
	\param newScale a const glm::vec2& - The new scale
	*/
	void MeshRender2D::setLocalScale(const glm::vec2& newScale)
	{
		m_localScale = newScale;
	}

	//!	getLocalScale()
	/*!
	\return a glm::vec2 - The local scale of the mesh render
	*/
	glm::vec2 MeshRender2D::getLocalScale()
	{
		return m_localScale;
	}

	//! getModelMatrix()
	/*!
	\return a glm::mat4 - The model transformation matrix
	*/
	glm::mat4 MeshRender2D::getModelMatrix()
	{
		Transform2D* trans = getParent()->getComponent<Transform2D>();
		// Order: Translate then Rotation then Scale
		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(trans->getPosition(), 0.f));
		model = glm::translate(model, glm::vec3(m_localPosition, 0.f));
		model = glm::rotate(model, glm::radians(trans->getOrientation()), glm::vec3(0.f, 0.f, 1.f));
		model = glm::rotate(model, glm::radians(m_localOrientation), glm::vec3(0.f, 0.f, 1.f));
		model = glm::scale(model, glm::vec3(trans->getScale(), 0.f));
		model = glm::scale(model, glm::vec3(m_localScale, 0.f));
		return model;
	}
}
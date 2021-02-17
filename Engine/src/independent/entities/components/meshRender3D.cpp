/*! \file meshRender3D.cpp
*
* \brief A 3D mesh render which contains the 3D geometry to render and the material to apply to the geometry.
*
* \author Daniel Bullin
*
*/
#include "independent/entities/components/meshRender3D.h"
#include "independent/entities/entity.h"
#include "independent/systems/systems/log.h"

namespace Engine
{
	//! MeshRender3D()
	/*!
	\param localPos a const glm::vec3& - The local position
	\param localOrientation a const glm::vec3& - The local orientation
	\param localScale a const glm::vec3& - The local scale
	\param model a Model3D* - A pointer to the 3D model
	\param material a Material* - A pointer to the material
	*/
	MeshRender3D::MeshRender3D(const glm::vec3& localPos, const glm::vec3& localOrientation, const glm::vec3& localScale, Model3D* model, Material* material) 
		: EntityComponent(ComponentType::MeshRender3D), m_model(model), m_material(material), m_localPosition(localPos), m_localOrientation(localOrientation), m_localScale(localScale)
	{
	}

	//! ~MeshRender3D()
	MeshRender3D::~MeshRender3D()
	{
		m_material = nullptr;
		m_model = nullptr;
	}

	//! onAttach()
	void MeshRender3D::onAttach()
	{
	}

	//! onDetach
	void MeshRender3D::onDetach()
	{
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The time step
	\param totalTime a const float - The total time of the application
	*/
	void MeshRender3D::onUpdate(const float timestep, const float totalTime)
	{
	}

	//! printComponentDetails()
	void MeshRender3D::printComponentDetails()
	{
		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("MeshRender3D Details for Entity: {0}", getParent()->getName());
		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("3D Model Address: {0}", (void*)m_model);
		ENGINE_TRACE("Material Address: {0}", (void*)m_material);
		ENGINE_TRACE("Local Position: {0}, {1}, {2}.", m_localPosition.x, m_localPosition.y, m_localPosition.z);
		ENGINE_TRACE("Local Orientation: {0}, {1}, {2}.", m_localOrientation.x, m_localOrientation.y, m_localOrientation.z);
		ENGINE_TRACE("Local Scale: {0}, {1}, {2}.", m_localScale.x, m_localScale.y, m_localScale.z);
		ENGINE_TRACE("==========================================");
	}

	//!	setModel()
	/*!
	\param model a Model3D* - A pointer to the 3D model
	*/
	void MeshRender3D::setModel(Model3D* model)
	{
		if (model)
			m_model = model;
		else
			ENGINE_ERROR("[MeshRender3D::setModel] An invalid model was provided.");
	}

	//!	getModel()
	/*!
	\return a Model3D* - A pointer to the 3D model
	*/
	Model3D* MeshRender3D::getModel()
	{
		return m_model;
	}

	//!	setMaterial()
	/*!
	\param material a Material* - A pointer to the material
	*/
	void MeshRender3D::setMaterial(Material* material)
	{
		if (material)
			m_material = material;
		else
			ENGINE_ERROR("[MeshRender3D::setMaterial] An invalid material was provided.");
	}

	//!	getMaterial()
	/*!
	\return a Material* - A pointer to the material
	*/
	Material* MeshRender3D::getMaterial()
	{
		return m_material;
	}

	//! setLocalPosition()
	/*!
	\param newPos a const glm::vec3& - The new position of the mesh render
	*/
	void MeshRender3D::setLocalPosition(const glm::vec3& newPos)
	{
		m_localPosition = newPos;
	}

	//!	getLocalPosition()
	/*!
	\return a glm::vec3 - The local position of the mesh render
	*/
	glm::vec3 MeshRender3D::getLocalPosition()
	{
		return m_localPosition;
	}

	//! setLocalOrientation()
	/*!
	\param newOrientation a const glm::vec3& - The new oritentation of the mesh render
	*/
	void MeshRender3D::setLocalOrientation(const glm::vec3& newOrientation)
	{
		m_localOrientation = newOrientation;
	}

	//!	getLocalOrientation()
	/*!
	\return a glm::vec3 - The local orientation of the mesh render
	*/
	glm::vec3 MeshRender3D::getLocalOrientation()
	{
		return m_localOrientation;
	}

	//! setLocalScale()
	/*!
	\param newScale a const glm::vec3& - The new scale of the mesh render
	*/
	void MeshRender3D::setLocalScale(const glm::vec3& newScale)
	{
		m_localScale = newScale;
	}

	//!	getLocalScale()
	/*!
	\return a glm::vec3 - The local scale of the mesh render
	*/
	glm::vec3 MeshRender3D::getLocalScale()
	{
		return m_localScale;
	}

	//! getModelMatrix()
	/*!
	\return a glm::mat4 - The model transformation matrix
	*/
	glm::mat4 MeshRender3D::getModelMatrix()
	{
		Transform3D* trans = getParent()->getComponent<Transform3D>();
		// Order: Translate then Rotation then Scale
		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, trans->getPosition());
		model = glm::translate(model, m_localPosition);
		model = glm::rotate(model, glm::radians(trans->getOrientation().x), glm::vec3(1.f, 0.f, 0.f));
		model = glm::rotate(model, glm::radians(trans->getOrientation().y), glm::vec3(0.f, 1.f, 0.f));
		model = glm::rotate(model, glm::radians(trans->getOrientation().z), glm::vec3(0.f, 0.f, 1.f));
		model = glm::rotate(model, glm::radians(m_localOrientation.x), glm::vec3(1.f, 0.f, 0.f));
		model = glm::rotate(model, glm::radians(m_localOrientation.y), glm::vec3(0.f, 1.f, 0.f));
		model = glm::rotate(model, glm::radians(m_localOrientation.z), glm::vec3(0.f, 0.f, 1.f));
		model = glm::scale(model, trans->getScale());
		model = glm::scale(model, m_localScale);
		return model;
	}
}
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
	\param material a Material* - A pointer to the material
	*/
	MeshRender2D::MeshRender2D(Material* material) : EntityComponent(ComponentType::MeshRender2D)
	{
		setMaterial(material);
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
		Entity* parent = getParent();

		if (!parent)
		{
			ENGINE_ERROR("[MeshRender2D::printComponentDetails] This component does not have a valid parent entity. Component Name: {0}.", m_name);
			return;
		}

		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("MeshRender2D Details for Entity: {0}", parent->getName());
		ENGINE_TRACE("==========================================");
		if(m_material) ENGINE_TRACE("Material Name: {0}.", m_material->getName());
		ENGINE_TRACE("==========================================");
	}

	//!	setMaterial()
	/*!
	\param material a Material* - A pointer to the material
	*/
	void MeshRender2D::setMaterial(Material* material)
	{
		if (material)
			m_material = material;
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
}
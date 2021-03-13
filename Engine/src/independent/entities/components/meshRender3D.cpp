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
#include "independent/rendering/renderers/renderer3D.h"

namespace Engine
{
	//! MeshRender3D()
	/*!
	\param model a Model3D* - A pointer to the 3D model
	\param material a Material* - A pointer to the material
	*/
	MeshRender3D::MeshRender3D(Model3D* model, Material* material) 
		: EntityComponent(ComponentType::MeshRender3D), m_model(model), m_material(material)
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
		Entity* parent = getParent();

		if (!parent)
		{
			ENGINE_ERROR("[MeshRender3D::printComponentDetails] This component does not have a valid parent entity. Component Name: {0}.", m_name);
			return;
		}

		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("MeshRender3D Details for Entity: {0}", parent->getName());
		ENGINE_TRACE("==========================================");
		if(m_model) ENGINE_TRACE("Model Name: {0}", m_model->getName());
		if(m_material) ENGINE_TRACE("Material Name: {0}", m_material->getName());
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

	//! onRender()
	void MeshRender3D::onRender()
	{
		if (getParent()->containsComponent<Transform>())
		{
			if (m_material)
			{
				if (getParent()->containsComponent<NativeScript>())
				{
					getParent()->getComponent<NativeScript>()->onSubmit(Renderers::Renderer3D);
				}

				for (auto& mesh : m_model->getMeshes())
					Renderer3D::submit(getParent()->getName(), mesh.getGeometry(), m_material, getParent()->getComponent<Transform>()->getModelMatrix());
			}
			else
			{
				if (getParent()->containsComponent<NativeScript>())
				{
					getParent()->getComponent<NativeScript>()->onSubmit(Renderers::Renderer3D);
				}

				for (auto& mesh : m_model->getMeshes())
					Renderer3D::submit(getParent()->getName(), mesh.getGeometry(), mesh.getMaterial(), getParent()->getComponent<Transform>()->getModelMatrix());
			}
		}
		else
			ENGINE_ERROR("[MeshRender3D::onRender] The entity this mesh render is attached to does not have a valid transform.");
	}
}
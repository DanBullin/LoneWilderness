/*! \file skybox.cpp
*
* \brief A Skybox class
*
* \author: Daniel Bullin
*
*/
#include "independent/entities/components/components/skybox.h"
#include "independent/systems/systems/log.h"
#include "independent/rendering/renderers/renderer3D.h"

namespace Engine
{
	//! Skybox()
	/*!
	\param model a Model3D* - A pointer to the 3D model
	\param material a Material* - A pointer to the material
	*/
	Skybox::Skybox(Model3D* model, Material* material)
	{
		setModel(model);
		setMaterial(material);
	}

	//! ~Skybox()
	Skybox::~Skybox()
	{
		ENGINE_INFO("[Skybox::~Skybox] Deleting skybox.");
		if (m_material) m_material->decreaseCount();
		if (m_model) m_model->decreaseCount();
		m_material = nullptr;
		m_model = nullptr;
	}

	//!	setModel()
	/*!
	\param model a Model3D* - A pointer to the 3D model
	*/
	void Skybox::setModel(Model3D* model)
	{
		if (model)
			m_model = model;
		else
			ENGINE_ERROR("[Skybox::setModel] An invalid model was provided.");
	}

	//!	getModel()
	/*!
	\return a Model3D* - A pointer to the 3D model
	*/
	Model3D* Skybox::getModel()
	{
		return m_model;
	}

	//!	setMaterial()
	/*!
	\param material a Material* - A pointer to the material
	*/
	void Skybox::setMaterial(Material* material)
	{
		if (material)
			m_material = material;
		else
			ENGINE_ERROR("[Skybox::setMaterial] An invalid material was provided.");
	}

	//!	getMaterial()
	/*!
	\return a Material* - A pointer to the material
	*/
	Material* Skybox::getMaterial()
	{
		return m_material;
	}

	//! onRender()
	void Skybox::onRender()
	{
		// Submit the model and material, model matrix is irrelevant as it isnt used
		Renderer3D::submit(m_model->getMeshes().at(0).getGeometry(), m_material, glm::mat4(1.f));
	}
}
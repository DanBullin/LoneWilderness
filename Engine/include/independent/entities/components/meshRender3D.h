/*! \file meshRender3D.h
*
* \brief A 3D mesh render which contains the 3D geometry to render and the material to apply to the geometry.
*
* \author Daniel Bullin
*
*/
#ifndef MESHRENDER3D_H
#define MESHRENDER3D_H

#include "independent/entities/entityComponent.h"
#include "independent/rendering/geometry/model3D.h"
#include "independent/rendering/materials/material.h"

namespace Engine
{
	/*! \class MeshRender3D
	* \brief A 3D mesh render which contains the 3D model and material of the entity
	*/
	class MeshRender3D : public EntityComponent
	{
	private:
		Model3D* m_model; //!< A pointer to the 3D geometry
		Material* m_material; //!< A pointer to the material
	public:
		MeshRender3D(Model3D* model, Material* material); //!< Constructor
		~MeshRender3D(); //!< Destructor

		void onAttach() override; //!< Occurs when component is attached to an entity
		void onDetach() override; //!< Occurs when component is detached from an entity
		void onUpdate(const float timestep, const float totalTime) override; //!< Update the 3D mesh render
		void printComponentDetails() override; //!< Print component details

		void setModel(Model3D* model); //!< Set the 3D model
		Model3D* getModel(); //!< Get the 3D model

		void setMaterial(Material* material); //!< Set the material
		Material* getMaterial(); //!< Get the material
	};
}
#endif
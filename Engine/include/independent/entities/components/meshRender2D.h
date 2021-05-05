/*! \file meshRender2D.h
*
* \brief A 2D mesh render which contains the material to apply to a 2D quad
*
* \author Daniel Bullin
*
*/
#ifndef MESHRENDER2D_H
#define MESHRENDER2D_H

#include "independent/entities/entityComponent.h"
#include "independent/rendering/materials/material.h"

namespace Engine
{
	/*! \class MeshRender2D
	* \brief A 2D mesh render which contains the material of the quad
	*/
	class MeshRender2D : public EntityComponent
	{
	private:
		Material* m_material; //!< A pointer to the material
	public:
		MeshRender2D(Material* material); //!< Constructor
		~MeshRender2D(); //!< Destructor

		void onAttach() override; //!< Occurs when component is attached to an entity
		void onDetach() override; //!< Occurs when component is detached from an entity
		void onUpdate(const float timestep, const float totalTime) override; //!< Update the component
		void printComponentDetails() override; //!< Print component details

		void setMaterial(Material* material); //!< Set the material
		Material* getMaterial(); //!< Get the material

		void onRender(); //!< A render submission command

		bool containsPoint(const glm::vec2 point); //!< Contains a point in the geometry
	};
}
#endif
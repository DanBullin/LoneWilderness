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
		glm::vec2 m_localPosition; //!< The local position of the geometry to the entity
		float m_localOrientation; //!< The local rotation of the geometry to the entity
		glm::vec2 m_localScale; //!< The local scale of the geometry to the entity
	public:
		MeshRender2D(const glm::vec2& localPos, const float localOrientation, const glm::vec2& localScale, const Shared<Material>& material); //!< Constructor
		~MeshRender2D(); //!< Destructor

		void onAttach() override; //!< Occurs when component is attached to an entity
		void onDetach() override; //!< Occurs when component is detached from an entity
		void onUpdate(const float timestep, const float totalTime) override; //!< Update the component
		void printComponentDetails() override; //!< Print component details

		bool containsPoint(const glm::vec2& coordinate); //!< Returns whether the point is inside this mesh render's bounding box

		void setMaterial(const Shared<Material>& material); //!< Set the material
		Material* getMaterial(); //!< Get the material

		void setLocalPosition(const glm::vec2& newPos); //!< Set the local position
		glm::vec2 getLocalPosition(); //!< Get the local position
		void setLocalOrientation(const float newOrientation); //!< Set the local orientation
		float getLocalOrientation(); //!< Get the local orientation
		void setLocalScale(const glm::vec2& newScale); //!< Set the local scale
		glm::vec2 getLocalScale(); //!< Get the local scale

		glm::mat4 getModelMatrix(); //!< Get the model matrix of the geometry
	};
}
#endif
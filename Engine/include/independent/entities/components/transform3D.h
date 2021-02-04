/*! \file transform3D.h
*
* \brief The position, scale and rotation of an 3D entity
*
* \author Daniel Bullin
*
*/
#ifndef TRANSFORM3D_H
#define TRANSFORM3D_H

#include "independent/entities/entityComponent.h"

namespace Engine
{
	/*! \class Transform3D
	* \brief The position, scale and rotation of an 3D object in the world
	*/
	class Transform3D : public EntityComponent
	{
	private:
		glm::vec3 m_position; //!< Position of the entity
		glm::vec3 m_orientation; //!< Orientation of the entity
		glm::vec3 m_scale; //!< Scale of the entity
	public:
		Transform3D(const float xPos, const float yPos, const float zPos, const float xRotation, const float yRotation, const float zRotation, const float sX, const float sY, const float sZ); //!< Constructor requiring all transform types
		~Transform3D(); //!< Destructor

		void onAttach() override; //!< Occurs when component is attached to an entity
		void onDetach() override; //!< Occurs when component is detached from an entity
		void onUpdate(const float timestep, const float totalTime) override; //!< Update the component
		void printComponentDetails() override; //!< Print component details

		glm::vec3 getPosition() const; //!< Get the position of the entity in the world
		void setPosition(const float xPos, const float yPos, const float zPos); //!< Set the position of the entity in the world
		void setPosition(const glm::vec3& newPos); //!< Set the position of the entity in the world

		glm::vec3 getOrientation() const; //!< Get the orientation of the entity in the world
		void setOrientation(const float xRotation, const float yRotation, const float zRotation); //!< Set the orientation of the entity in the world
		void setOrientation(const glm::vec3& newOrientation); //!< Set the orientation of the entity in the world

		glm::vec3 getScale() const; //!< Get the scale of the entity in the world
		void setScale(const float sX, const float sY, const float sZ); //!< Set the scale of the entity in the world
		void setScale(const glm::vec3& newScale); //!< Set the scale of the entity in the world
	};
}
#endif
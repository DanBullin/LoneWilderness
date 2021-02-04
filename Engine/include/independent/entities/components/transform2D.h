/*! \file transform2D.h
*
* \brief The position, scale and rotation of a 2D entity
*
* \author Daniel Bullin
*
*/
#ifndef TRANSFORM2D_H
#define TRANSFORM2D_H

#include "independent/core/common.h"
#include "independent/entities/entityComponent.h"

namespace Engine
{
	/*! \class Transform2D
	* \brief The position, scale and rotation of an 2D object in the world
	*/
	class Transform2D : public EntityComponent
	{
	private:
		glm::vec2 m_position; //!< Position of the entity
		float m_orientation; //!< Rotation of the entity
		glm::vec2 m_scale; //!< Scale of the entity
	public:
		Transform2D(const float xPos, const float yPos, const float rotation, const float sX, const float sY); //!< Constructor requiring all transform types
		~Transform2D(); //!< Destructor

		void onAttach() override; //!< Occurs when component is attached to an entity
		void onDetach() override; //!< Occurs when component is detached from an entity
		void onUpdate(const float timestep, const float totalTime) override; //!< Update the component
		void printComponentDetails() override; //!< Print component details

		bool containsPoint(const glm::vec2& coordinate); //!< Returns whether the point is inside this transform's bounding box

		glm::vec2 getPosition() const; //!< Get the position of the entity in the world
		void setPosition(const float xPos, const float yPos); //!< Set the position of the entity in the world
		void setPosition(const glm::vec2& newPos); //!< Set the position of the entity in the world

		float getOrientation() const; //!< Get the orientation of the entity in the world
		void setOrientation(const float rotation); //!< Set the orientation of the entity in the world

		glm::vec2 getScale() const; //!< Get the scale of the entity in the world
		void setScale(const float sX, const float sY); //!< Set the scale of the entity in the world
	};
}

#endif
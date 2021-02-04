/*! \file entityComponent.h
*
* \brief An entity component which describes one particular feature of an entity. Follows the ECS architectural pattern
*
* \author Daniel Bullin
*
*/
#ifndef ENTITYCOMPONENT_H
#define ENTITYCOMPONENT_H

#include "independent/core/common.h"

#define COMPONENTLIMIT 100

namespace Engine
{
	class Entity; //!< Forward declare entity

	/*! \enum ComponentType
	* \brief The different types of entity components
	*/
	enum class ComponentType
	{
		None,		   //!< No type
		Camera,		   //!< A camera in the world
		Transform3D,   //!< A 3D transform of an entity
		Transform2D,   //!< A 2D transform of an entity
		MeshRender3D,  //!< The render details for a 3D entity
		MeshRender2D,  //!< The render details for a 2D entity
		EventListener, //!< An event listener to respond to inputs and updates
		Text		   //!< A text render component
	};

	namespace Components
	{
		//! getComponentLimit()
		/*!
		\param componentClassName a const std::string& - The name of the component class
		\return an uint32_t - The maximum number of component type on an entity
		*/
		static uint32_t getComponentLimit(const std::string& componentClassName)
		{
			// Class types should be in the format [class Engine::{CLASS_NAME}]
			// Identify unique cases otherwise it returns arbitrary limit
			if (componentClassName == "class Engine::Transform3D")
				return 1;
			else if (componentClassName == "class Engine::Transform2D")
				return 1;
			else if (componentClassName == "class Engine::EventListener")
				return 1;
			else
				return COMPONENTLIMIT;
		}
	}

	/*! \class EntityComponent
	* \brief An entity component which are attached to entities
	*/
	class EntityComponent
	{
	private:
		Entity* m_parentEntity; //!< The entity this component is attached to
		std::string m_name; //!< The name of the component
		ComponentType m_componentType; //!< The component type

		EntityComponent(const EntityComponent& other) {} //!< The copy constructor disabled
		void operator=(const EntityComponent& other) {} //!< The assignment operator disabled
	public:
		EntityComponent(const ComponentType type); //!< Constructor requiring type
		virtual ~EntityComponent(); //!< Destructor

		void setParent(Entity* parent); //!< Set the component's parent
		Entity* getParent(); //!< Get the entity this component is attached to

		void setName(const char* name); //!< Set the name of the component
		const std::string& getName() const; //!< Get the name of the component

		const ComponentType& getComponentType() const; //!< Get the component type

		virtual void onAttach() = 0; //!< Occurs when component is attached to an entity
		virtual void onDetach() = 0; //!< Occurs when component is detached from an entity
		virtual void onUpdate(const float timestep, const float totalTime) = 0; //!< Update function for components
			/*!< \param timestep a const float - The timestep
				 \param totalTime a const float - The total runtime of the application */

		virtual void printComponentDetails() = 0; //!< Print component details
	};
}
#endif
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
#include "independent/systems/systems/resourceManager.h"

namespace Engine
{
	class Entity; //!< Forward declare entity

	/*! \enum ComponentType
	* \brief The different types of entity components
	*/
	enum class ComponentType
	{
		None = 0,				 //!< No type
		Camera = 1,				 //!< A camera in the world
		Transform = 2,			 //!< A 3D transform of an entity
		MeshRender3D = 3,		 //!< The render details for a 3D entity
		MeshRender2D = 4,		 //!< The render details for a 2D entity
		Text = 5,				 //!< A text render component
		CharacterController = 6, //!< A character controller
		DirectionalLight = 7,	 //!< A directional light source
		PointLight = 8,		     //!< A point light source
		SpotLight = 9,			 //!< A spot light source
		NativeScript = 10		 //!< A native script
	};

	/*! \class EntityComponent
	* \brief An entity component which are attached to entities
	*/
	class EntityComponent
	{
	private:
		EntityComponent(const EntityComponent& other) {} //!< The copy constructor disabled
		void operator=(const EntityComponent& other) {} //!< The assignment operator disabled
	protected:
		std::string m_name; //!< The name of the component
		ComponentType m_componentType; //!< The component type
		Entity* m_parentEntity; //!< The entity this component is attached to
	public:
		EntityComponent(const ComponentType type); //!< Constructor requiring component type
		virtual ~EntityComponent(); //!< Destructor

		void detach(); //!< Detach this component from its parent

		void setParent(Entity* parent); //!< Set the component's parent
		Entity* getParent(); //!< Get the entity this component is attached to

		void setName(const std::string& name); //!< Set the name of the component
		const std::string& getName() const; //!< Get the name of the component

		const ComponentType& getComponentType() const; //!< Get the component type

		virtual void onAttach() = 0; //!< Occurs when component is attached to an entity
		virtual void onDetach() = 0; //!< Occurs when component is detached from an entity
		virtual void onUpdate(const float timestep, const float totalTime) = 0; //!< Update function for components
			/*!< \param timestep a const float - The timestep
				 \param totalTime a const float - The total runtime of the application */

		virtual void printComponentDetails() = 0; //!< Print component details
	};

	namespace Components
	{
		//! toString()
		/*
		\param type a ComponentType - The component type
		\return a const std::string - The component type as a string
		*/
		static const std::string toString(ComponentType type)
		{
			switch (type)
			{
			case ComponentType::None: return "None";
			case ComponentType::Camera: return "Camera";
			case ComponentType::Transform: return "Transform";
			case ComponentType::MeshRender3D: return "MeshRender3D";
			case ComponentType::MeshRender2D: return "MeshRender2D";
			case ComponentType::Text: return "Text";
			case ComponentType::CharacterController: return "CharacterController";
			case ComponentType::DirectionalLight: return "DirectionalLight";
			case ComponentType::PointLight: return "PointLight";
			case ComponentType::SpotLight: return "SpotLight";
			case ComponentType::NativeScript: return "NativeScript";
			default: return "Not a valid component type.";
			}
		}

		//! toType()
		/*!
		\param classType a const std::string& - The class type
		\return a const ComponentType - The type of component
		*/
		static const ComponentType toType(const std::string& classType)
		{
			// Class types should be in the format [class Engine::{CLASS_NAME}]
			if (classType == "class Engine::Camera" || classType == "Camera")
				return ComponentType::Camera;
			else if (classType == "class Engine::Transform" || classType == "Transform")
				return ComponentType::Transform;
			else if (classType == "class Engine::MeshRender3D" || classType == "MeshRender3D")
				return ComponentType::MeshRender3D;
			else if (classType == "class Engine::MeshRender2D" || classType == "MeshRender2D")
				return ComponentType::MeshRender2D;
			else if (classType == "class Engine::Text" || classType == "Text")
				return ComponentType::Text;
			else if (classType == "class Engine::CharacterController" || classType == "CharacterController")
				return ComponentType::CharacterController;
			else if (classType == "class Engine::PointLight" || classType == "PointLight")
				return ComponentType::PointLight;
			else if (classType == "class Engine::SpotLight" || classType == "SpotLight")
				return ComponentType::SpotLight;
			else if (classType == "class Engine::DirectionalLight" || classType == "DirectionalLight")
				return ComponentType::DirectionalLight;
			else if (classType == "class Engine::NativeScript" || classType == "NativeScript")
				return ComponentType::NativeScript;
			else
				return ComponentType::None;
		}
	}
}
#endif
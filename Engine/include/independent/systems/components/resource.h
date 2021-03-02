/*! \file resource.h
*
* \brief A resource that holds some data
*
* \author Daniel Bullin
*
*/
#ifndef RESOURCE_H
#define RESOURCE_H

#include <inttypes.h>
#include <string>

namespace Engine
{
	/*! \enum ResourceType
	* \brief The different types of resources
	*/
	enum class ResourceType
	{
		// When resources are deleted as a whole, it will be done in the reverse order because some resources rely on another
		VertexBuffer = 0,
		IndexBuffer = 1,
		VertexArray = 2,
		IndirectBuffer = 3,
		UniformBuffer = 4,
		FrameBuffer = 5,
		ShaderProgram = 6,
		Texture = 7,
		SubTexture = 8,
		Material = 9,
		Model3D = 10
	};

	//! toString()
	/*
	\param type a const ResourceType - The type of resource
	\return a std::string - The type as a string literal
	*/
	static std::string toString(const ResourceType type)
	{
		switch (type)
		{
			case ResourceType::VertexBuffer:
				return "VertexBuffer";
			case ResourceType::IndexBuffer:
				return "IndexBuffer";
			case ResourceType::VertexArray:
				return "VertexArray";
			case ResourceType::IndirectBuffer:
				return "IndirectBuffer";
			case ResourceType::UniformBuffer:
				return "UniformBuffer";
			case ResourceType::FrameBuffer:
				return "FrameBuffer";
			case ResourceType::ShaderProgram:
				return "ShaderProgram";
			case ResourceType::Texture:
				return "Texture";
			case ResourceType::SubTexture:
				return "SubTexture";
			case ResourceType::Material:
				return "Material";
			case ResourceType::Model3D:
				return "Model3D";
			default:
				return "NONE";
		}
	}

	/*! \class Resource
	* \brief A base class defining a resource
	*/
	class Resource 
	{
	protected:
		std::string m_name; //!< The name of the resource
		ResourceType m_type; //!< The type of resource
	public:
		Resource(const std::string& resourceName, const ResourceType type); //!< Constructor
		virtual ~Resource(); //!< Destructor
		const std::string& getName() const; //!< Get the name of the resource
		ResourceType getType() const; //!< Get the resource type
		virtual void printDetails() = 0; //!< Print the details of the resource
	};
}

#endif
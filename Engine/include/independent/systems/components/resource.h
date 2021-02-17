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
		Texture, SubTexture, UniformBuffer, FrameBuffer, ShaderProgram, VertexArray, VertexBuffer, IndexBuffer, IndirectBuffer,
		Model3D, Material
	};

	/*! \class Resource
	* \brief A base class defining a resource
	*/
	class Resource 
	{
	protected:
		std::string m_name; //!< The name of the resource
		ResourceType m_type; //!< The type of resource
		uint32_t m_referenceCounter = 0; //!< How many external references are there to this resource
	public:
		Resource(const std::string& resourceName, const ResourceType type); //!< Constructor
		virtual ~Resource() = default; //!< Destructor
		void increaseCount(); //!< Increase the reference counter by 1
		void decreaseCount(); //!< Decrease the reference counter by 1
		const std::string& getName() const; //!< Get the name of the resource
		const uint32_t getReferenceCount() const; //!< Get the reference count of this resource
		ResourceType getType() const; //!< Get the resource type
	};
}

#endif
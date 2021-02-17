/*! \file resourceLoader.h
*
* \brief A resource loader which loads resources from JSON config files
*
* \author Daniel Bullin
*
*/
#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#include "independent/core/common.h"
#include "independent/systems/systems/resourceManager.h"

namespace Engine
{
	/*! \class ResourceLoader
	* \brief A utility class to help load resources from file
	*/
	class ResourceLoader
	{
	private:
		static uint32_t getSize(const std::string& className); //!< Calculate the size in bytes
		static uint32_t getCapacity(const std::string& capacityLocation); //!< Get the capacity
	public:
		static void loadVertexBuffers(const std::string& filePath); //!< Load the vertex buffers needed in this scene
		static void loadIndexBuffers(const std::string& filePath); //!< Load the index buffers needed in this scene
		static void loadVertexArrays(const std::string& filePath); //!< Load the vertex arrays needed in this scene
		static void loadIndirectBuffers(const std::string& filePath); //!< Load the indirect buffers needed in this scene
		static void loadUniformBuffers(const std::string& filePath); //!< Load the uniform buffers needed in this scene
		static void loadFrameBuffers(const std::string& filePath); //!< Load the frame buffers needed in this scene
		static void loadShaderPrograms(const std::string& filePath); //!< Load the shader programs needed in this scene
		static void loadTextures(const std::string& filePath); //!< Load the textures needed in this scene
		static void loadSubTextures(const std::string& filePath); //!< Load the subtextures needed in this scene
		static void load3DModels(const std::string& filePath); //!< Load the 3D Models needed in this scene
		static void loadMaterials(const std::string& filePath); //!< Load the Materials needed in this scene
	};
}
#endif
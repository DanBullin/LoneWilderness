/*! \file assimpLoader.h
*
* \brief A utility class to load 3D models from file into assimp's data structures and then into our own mesh classes.
*
* \author Daniel Bullin
*
*/
#ifndef ASSIMPLOADER_H
#define ASSIMPLOADER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "independent/rendering/geometry/model3D.h"

namespace Engine
{
	/*! \class AssimpLoader
	* \brief A utility class to load 3D models from file into assimp's data structures and into our own mesh classes
	*/
	class AssimpLoader
	{
	private:
		static void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh3D>& meshes); //!< Process a node from Assimps' scene data structure
		static Mesh3D processMesh(aiMesh* mesh, const aiScene* scene); //!< Process mesh from Assimps' node's data structure
	public:
		static void loadModel(const std::string& filePath, std::vector<Mesh3D>& meshes); //!< Load a model's data into the meshes reference passed
	};
}
#endif
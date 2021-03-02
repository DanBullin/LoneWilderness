/*! \file assimpLoader.cpp
*
* \brief A utility class to load 3D models from file into assimp's data structures and then into our own mesh classes.
*
* \author Daniel Bullin
*
*/
#include "independent/utils/assimpLoader.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/rendering/renderers/renderer3D.h"
#include "independent/utils/memoryUtils.h"

namespace Engine
{
	//! processNode()
	/*!
	\param node a aiNode* - A node containing a number of meshes
	\param scene a const aiScene* - A scene containing all data loaded from model into assimp
	\param meshes a std::vector<Mesh3D>& - A vector of meshes referenced from the model we're currently loading
	*/
	void AssimpLoader::processNode(aiNode* node, const aiScene* scene, std::vector<Mesh3D>& meshes)
	{
		// Create meshes for all aiMeshes in the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// Go through all the meshes in the node and assign it to mesh
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			// Use aiMesh to create our custom Mesh object
			// Populate the vector of meshes we were given by reference
			meshes.push_back(processMesh(mesh, scene));
		}

		// After we've processed all meshes for the current node, go through all the child nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
			processNode(node->mChildren[i], scene, meshes);
	}

	//! processMesh()
	/*!
	\param mesh a aiMesh* - A mesh filled with a number of vertices and materials
	\param scene a const aiScene* - A scene containing all data loaded from model into assimp
	\return a Mesh3D - A 3D mesh
	*/
	Mesh3D AssimpLoader::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		// The local mesh information to fill
		std::vector<Vertex3D> vertices;
		std::vector<unsigned int> indices;

		vertices.reserve(mesh->mNumVertices);

		// Go through each vertices in the mesh
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			// Get all vertex information
			const aiVector3D* aPos = &(mesh->mVertices[i]);
			const aiVector3D* aNormal = &(mesh->mNormals[i]);
			const aiVector3D* aTexCoord = &(mesh->mTextureCoords[0][i]);
			const aiVector3D* aTangent = &(mesh->mTangents[i]);
			const aiVector3D* aBitangent = &(mesh->mBitangents[i]);

			// Create a vertex and store all vertices information based on the current vertex we're processing
			Vertex3D vertex({ aPos->x, aPos->y, aPos->z }, { aTexCoord->x, aTexCoord->y },
				MemoryUtils::normalise({ aNormal->x, aNormal->y, aNormal->z }), MemoryUtils::normalise({ aTangent->x, aTangent->y, aTangent->z }), MemoryUtils::normalise({ aBitangent->x, aBitangent->y, aBitangent->z }));

			// Add new vertex to the list
			vertices.push_back(vertex);
		}

		// Go through all the faces for the mesh and store the indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// Create a piece of geometry using local vertices and indices information
		Geometry3D geometry;
		geometry.VertexBuffer = ResourceManager::getResource<VertexBuffer>("Vertex3DBuffer");
		Renderer3D::addGeometry(vertices, indices, geometry);
		return Mesh3D(geometry);
	}

	//! loadModel()
	/*!
	\param filePath a const std::string& - The file path of the model
	\param meshes a std::vector<Mesh3D>& - The vector of meshes in the model
	*/
	void AssimpLoader::loadModel(const std::string& filePath, std::vector<Mesh3D>& meshes)
	{
		// Read model file into assimp
		Assimp::Importer importer;

		// Calculate tangents, etc
		const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		// Check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			ENGINE_ERROR("[AssimpLoader::loadModel] Cannot load model {0} into assimp.", filePath);
			return;
		}

		// Clear any previous meshes in the models' meshes
		meshes.clear();

		// Recursively go through each node and create meshes
		processNode(scene->mRootNode, scene, meshes);
	}
}
/*! \file model3D.cpp
*
* \brief This is a 3D model class which contains a list of all the meshes the model has.
*
* \author Daniel Bullin
*
*/
#include "independent/rendering/geometry/model3D.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/resourceManager.h"

namespace Engine
{
	//! Model3D()
	/*
	\param modelName a const std::string& - The name of the 3D model
	*/
	Model3D::Model3D(const std::string& modelName) : Resource(modelName, ResourceType::Model3D)
	{
	}

	//! ~Model3D()
	Model3D::~Model3D()
	{
		if (ResourceManager::getConfigValue(Config::PrintResourcesInDestructor))
			ENGINE_INFO("[Model3D::~Model3D] Deleting 3D model with name: {0}.", m_name);
		// Clear the mesh list
		destroy();
	}

	//! getMeshes()
	/*!
	\return a std::vector<Mesh3D>& - The reference to a vector of 3D meshes
	*/
	std::vector<Mesh3D>& Model3D::getMeshes()
	{
		return m_meshes;
	}

	//! destroy()
	void Model3D::destroy()
	{
		// Clear the mesh list
		if (m_meshes.size() != 0)
		{
			m_meshes.clear();
		}
	}

	//! printDetails()
	void Model3D::printDetails()
	{
		for (int i = 0; i < m_meshes.size(); i++)
		{
			auto geom = m_meshes[i].getGeometry();
			ENGINE_TRACE("Mesh{0}: Geometry ID: {1}, Vertex Count: {2}, Index Count: {3}, First Vertex: {4}, First Index: {5}, Vertex Buffer: {6}.",
				i, geom.ID, geom.VertexCount, geom.IndexCount, geom.FirstVertex, geom.FirstIndex, geom.VertexBuffer->getName());
		}
	}
}
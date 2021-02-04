/*! \file model3D.cpp
*
* \brief This is a 3D model class which contains a list of all the meshes the model has.
*
* \author Daniel Bullin
*
*/

#include "independent/systems/systems/log.h"
#include "independent/rendering/geometry/model3D.h"
#include "independent/utils/memoryUtils.h"

namespace Engine
{
	//! Model3D()
	Model3D::Model3D()
	{
	}

	//! ~Model3D()
	Model3D::~Model3D()
	{
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
			ENGINE_INFO("[Model3D::destroy] Deleting model.");
			m_meshes.clear();
		}
	}
}
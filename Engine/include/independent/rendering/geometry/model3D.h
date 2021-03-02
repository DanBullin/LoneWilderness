/*! \file model3D.h
*
* \brief This is a 3D model class which contains a list of all the meshes the model has.
*
* \author Daniel Bullin
*
*/
#ifndef MODEL3D_H
#define MODEL3D_H

#include "independent/rendering/geometry/mesh3D.h"
#include "independent/systems/components/resource.h"

namespace Engine
{
	/*! \class Model3D
	* \brief A 3D model class that contains all the meshes of the model
	*/
	class Model3D : public Resource
	{
	private:
		std::vector<Mesh3D> m_meshes; //!< List of all the meshes associated with the model
	public:
		Model3D(const std::string& modelName); //!< Constructor
		~Model3D(); //!< Destructor
		std::vector<Mesh3D>& getMeshes(); //!< Get the list of meshes as a reference
		void destroy(); //!< Destory the meshes
		void printDetails() override; //!< Print the resource details
	};
}
#endif
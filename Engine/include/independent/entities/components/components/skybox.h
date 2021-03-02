/*! \file skybox.h
*
* \brief A skybox component that is attached to the main camera
*
* \author Daniel Bullin
*
*/
#ifndef SKYBOX_H
#define SKYBOX_H

#include "independent/entities/entityComponent.h"

namespace Engine
{
	/*! \class Skybox
	* \brief A Skybox class
	*/
	class Skybox
	{
	private:
		Material* m_material; //!< The material containing the cubemap texture, shader and tint
		Model3D* m_model; //!< The cube model
	public:
		Skybox(Model3D* model, Material* material); //!< Constructor
		~Skybox(); //!< Destructor

		void setModel(Model3D* model); //!< Set the 3D model
		Model3D* getModel(); //!< Get the 3D model

		void setMaterial(Material* material); //!< Set the material
		Material* getMaterial(); //!< Get the material

		void onRender(); //!< Render the skybox
	};
}
#endif
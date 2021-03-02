/*! \file destroyable.h
*
* \brief A class which defines whether the object which inherits it is to be deleted
*
* \author Daniel Bullin
*
*/
#ifndef DESTROYABLE_H
#define DESTROYABLE_H

namespace Engine
{
	/*! \class Destroyable
	* \brief A destroyable object
	*/
	class Destroyable
	{
	private:
		bool m_destroy = false; //!< Should this object be destroyed
	public:
		void destroy(); //!< Destroy this object
		void undestroy(); //!< Undestroys an object that has not yet been deleted but is waiting to
		const bool getDestroyed() const; //!< Get whether this object is to be destroyed
	};
}
#endif
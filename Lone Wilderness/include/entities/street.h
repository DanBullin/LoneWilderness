/*! \file street.h
*
* \brief A Street dummy class
*
* \author: Aaron Taleghani
*
*/
#ifndef STREET_H
#define STREET_H

#include "independent/entities/entity.h"

using namespace Engine;

/*! \class Street
* \brief A street dummy class
*/
class Street : public Entity
{
public:
	Street(); //!< Constructor
	~Street(); //!< Destructor
};
#endif
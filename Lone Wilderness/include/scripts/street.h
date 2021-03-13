/*! \file street.h
*
* \brief A Street dummy class
*
* \author: Aaron Taleghani
*
*/
#ifndef STREET_H
#define STREET_H

#include "independent/entities/components/nativeScript.h"

using namespace Engine;

/*! \class Street
* \brief A street dummy class
*/
class Street : public NativeScript
{
public:
	Street(); //!< Constructor
	~Street(); //!< Destructor
};
#endif
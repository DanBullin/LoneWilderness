/*! \file item.h
*
* \brief An item object
*
* \author: Daniel Bullin
*
*/
#include "scripts/gameObjects/player/item.h"

//! Item()
/*!
\param type an Items - The item type
\param state a const uint32_t - The item's state
*/
Item::Item(Items::Items type, uint32_t state) : m_type(type), m_state(state)
{
}

//! ~Item()
Item::~Item()
{
}

//! setState
/*!
\param state a const uint32_t - The item's state
*/
void Item::setState(const uint32_t state)
{
	m_state = state;
}
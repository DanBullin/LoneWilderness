/*! \file textureUnitManager.cpp
*
* \brief A texture unit manager which manages all the texture units
*
* \author DMU Course material
*
*/
#include "independent/rendering/textures/textureUnitManager.h"
#include "independent/systems/systems/log.h"

namespace Engine
{
	//! TextureUnitManager()
	/*!
	\param capacity a const uint32_t - The total number of texture units
	\param reservationCount a const uint32_t - The number of reserved slots to reserve
	*/
	TextureUnitManager::TextureUnitManager(const uint32_t capacity, const uint32_t reservationCount)
		: m_buffer(capacity, 0xFFFFFFFF), m_capacity(capacity), m_reservedSlots(reservationCount), m_head(reservationCount), m_tail(0)
	{
	}

	//! ~TextureUnitManager()
	TextureUnitManager::~TextureUnitManager()
	{
		ENGINE_INFO("[TextureUnitManager::~TextureUnitManager] Deleting the texture unit manager.");
	}

	//! full()
	/*!
	\return a bool - Is the texture unit buffer full
	*/
	bool TextureUnitManager::full() const
	{
		return m_full;
	}

	//! clear()
	/*!
	\param deleteReservations a const bool - Delete the reserved slots too
	*/
	void TextureUnitManager::clear(const bool deleteReservations)
	{
		m_head = m_reservedSlots;
		m_tail = 0;
		m_full = false;

		// Either delete all or only the unreserved units
		if (deleteReservations) std::fill(m_buffer.begin(), m_buffer.end(), 0xFFFFFFFF);
		else std::fill(m_buffer.begin() + m_reservedSlots, m_buffer.end(), 0xFFFFFFFF);
	}

	//! getUnit()
	/*!
	\param textureID a const uint32_t - The texture ID
	\param textureUnit an int32_t& - A reference to the texture unit held in the renderer
	*/
	bool TextureUnitManager::getUnit(const uint32_t textureID, int32_t& textureUnit)
	{
		// Is the texture already bound
		// Go through all unserved slots looking for the texture ID
		for (uint32_t i = m_tail; i < m_head; i++)
		{
			if (m_buffer.at(i) == textureID)
			{
				// Texture found, no need to bind, just return it
				textureUnit = i;
				return false;
			}
		}

		// Texture unit is not bound, check if list is full
		// If it is, simply set head back to position of the tail + the number of reserved slots
		if (m_full)
		{
			m_head = m_tail + m_reservedSlots;
			m_full = false;
		}

		// Update the current head's unit with the texture ID
		// Give the unit argument the new unit index
		m_buffer.at(m_head) = textureID;
		textureUnit = m_head;

		// Checking if over capacity
		m_head = (++m_head) % m_capacity; // If it equals 0, then buffer is full

		if (m_head == m_tail) m_full = true;

		return true;
	}

	//! setReservedUnit()
	/*!
	\param reservedSlot a const uint32_t - The reserved slot index
	\param textureID a const uint32_t - The texture ID to put in the reserved slot
	*/
	void TextureUnitManager::setReservedUnit(const uint32_t reservedSlot, const uint32_t textureID)
	{
		// Check if the slot is a reserved slot
		// All reserved slots are from the beginning and held sequentially so check if argument is less than the number of reserved slots
		if (reservedSlot < m_reservedSlots)
			m_buffer.at(reservedSlot) = textureID;
		else
			ENGINE_ERROR("[TextureUnitManager::setReservedUnit] An invalid reserved slot was provided. Slot: {0}", reservedSlot);
	}

	//! getRemainingUnitCount()
	/*!
	\return an uint32_t - The number of remaining units
	*/
	uint32_t TextureUnitManager::getRemainingUnitCount()
	{
		return m_capacity - m_head;
	}

	//! bindToUnit()
	/*!
	\param texture a Texture* - The texture to bind
	*/
	void TextureUnitManager::bindToUnit(Texture* texture)
	{
		if (!texture)
		{
			ENGINE_ERROR("[TextureUnitManager::bindToUnit] This is an invalid texture.");
			return;
		}

		// Find the texture unit the texture is bound to, and bind it
		for (int i = 0; i < m_buffer.size(); i++)
			if (m_buffer[i] == texture->getID())
				texture->bind(i);
	}
}
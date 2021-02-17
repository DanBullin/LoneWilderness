/*! \file textureUnitManager.h
*
* \brief A texture unit manager which manages all the texture units
*
* \author DMU Course material
*
*/
#ifndef TEXTUREUNITMANAGER_H
#define TEXTUREUNITMANAGER_H

#include "independent/rendering/textures/texture.h"

namespace Engine
{
	/*! \class TextureUnitManager
	* \brief A manager for the texture units
	*/
	class TextureUnitManager
	{
	private:
		std::vector<uint32_t> m_buffer; //!< The ring buffer

		uint32_t m_capacity; //!< Capacity of the ring buffer
		uint32_t m_reservedSlots; //!< The number of reserved slots

		uint32_t m_head; //!< The head of the buffer
		uint32_t m_tail; //!< The tail of the buffer
		bool m_full = false; //!< Is the ring buffer full
	public:
		TextureUnitManager(const uint32_t capacity, const uint32_t reservationCount); //!< Constructor
		~TextureUnitManager(); //!< Destructor

		bool full() const; //!< Is the texture unit list full
		void clear(const bool deleteReservations); //!< Clear the buffer
		bool getUnit(const uint32_t textureID, int32_t& textureUnit); //!< Returns whether or not the texture needs binding to the unit passed, the unit is returned
		void setReservedUnit(const uint32_t reservedSlot, const uint32_t textureID); //!< Set the unit of a reserved slot
		uint32_t getRemainingUnitCount(); //!< Get the count of free units
		void bindToUnit(Texture* texture); //!< Bind texture to unit
	};
}
#endif
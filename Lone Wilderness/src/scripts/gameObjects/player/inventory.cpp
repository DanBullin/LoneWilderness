/*! \file inventory.cpp
*
* \brief An inventory script
*
* \author: Daniel Bullin
*
*/
#include "scripts/gameObjects/player/inventory.h"
#include "independent/systems/systems/log.h"

Inventory::Inventory()
{
}

Inventory::~Inventory()
{
	for (auto& item : m_items)
	{
		if (item.first)
		{
			delete item.first;
		}
	}
	m_items.clear();
}

void Inventory::onAttach()
{
}

void Inventory::onPreUpdate(const float timestep, const float totalTime)
{
}

void Inventory::giveItem(Items::Items item, const uint32_t state, const int amount)
{
	bool foundItem = false;
	for (auto& items : m_items)
	{
		if (items.first)
		{
			if (items.first->getType() == item && items.first->getState() == state)
			{
				foundItem = true;
				items.second += amount;
				if (items.second > AMOUNTLIMIT)
					items.second = 50;
			}
		}
	}

	if (!foundItem)
	{
		if(m_items.size() < INVENLIMIT-1)
			m_items.push_back({ new Item(item, state), amount});
	}
}

void Inventory::takeItem(Items::Items itemType, const uint32_t state, const int amount)
{
	for (int i = 0; i < INVENLIMIT; i++)
	{
		if (m_items.size() > i)
		{
			if (m_items[i].first)
			{
				if (m_items[i].first->getType() == itemType && m_items[i].first->getState() == state)
				{
					m_items[i].second -= amount;

					if (m_items[i].second <= 0)
					{
						delete m_items[i].first;
						m_items.erase(m_items.begin() + i);
						return;
					}
				}
			}
		}
	}
}

Item * Inventory::getItem(Items::Items itemType, const uint32_t state)
{
	for (auto& items : m_items)
	{
		if (items.first)
		{
			if (items.first->getType() == itemType && items.first->getState() == state)
			{
				return items.first;
			}
		}
	}
	return nullptr;
}

Item * Inventory::getItem(const uint32_t hotbarIndex)
{
	if (m_items.size() > hotbarIndex)
	{
		if (m_items.at(hotbarIndex).first)
		{
			return m_items.at(hotbarIndex).first;
		}
	}
	return nullptr;
}

int Inventory::getItemCount(const uint32_t itemIndex)
{
	return m_items[itemIndex].second;
}

bool Inventory::containsItem(Items::Items itemType, const uint32_t state)
{
	for (auto& items : m_items)
	{
		if (items.first)
		{
			if (items.first->getType() == itemType && items.first->getState() == state)
			{
				return true;
			}
		}
	}
	return false;
}

bool Inventory::containsItem(Items::Items itemType, const uint32_t state, const int amount)
{
	for (auto& items : m_items)
	{
		if (items.first)
		{
			if (items.first->getType() == itemType && items.first->getState() == state && items.second >= amount)
			{
				return true;
			}
		}
	}
	return false;
}

void Inventory::swapItems(const uint32_t index1, const uint32_t index2)
{
	auto item1 = m_items.at(index1);
	auto item2 = m_items.at(index2);
	m_items.at(index1) = item2;
	m_items.at(index2) = item1;
}

void Inventory::print()
{
	for (int i = 0; i < INVENLIMIT; i++)
	{
		if (m_items.size() > i)
		{
			if (m_items[i].first)
			{
				ENGINE_INFO("Item {0}: {1} - Amount: {2}.", i, m_items[i].first->getType(), m_items[i].second);
			}
		}
	}
}

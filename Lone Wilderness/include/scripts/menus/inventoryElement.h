/*! \file inventoryElement.h
*
* \brief A script to handle the inventory items
*
* \author: Daniel Bullin
*
*/
#ifndef INVENTORYELEMENT_H
#define INVENTORYELEMENT_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/meshRender2D.h"

using namespace Engine;

class Player;

/*! \class InventoryElement
* \brief A script to handle the inventory items
*/
class InventoryElement : public NativeScript
{
private:
	MeshRender2D* m_render;
	glm::vec4 m_selectedTint;
	Player* m_player; //!< A pointer to the player script
	Entity* m_inventorySlot;
	Entity* m_inventoryCurrent;
	uint32_t m_inventoryIndex;
public:
	InventoryElement(); //!< Constructor
	InventoryElement(nlohmann::json scriptData); //!< Constructor
	~InventoryElement(); //!< Destructor
	void onAttach() override; //!< On attach
	void onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime) override;
	void onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse move
	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call before game update
	void onSubmit(const Renderers renderer, const std::string& renderState) override; //!< Call upon render if mesh render component attached

	Entity* getSlot();
	void resetSlot();
	uint32_t getInvenIndex();
};
#endif
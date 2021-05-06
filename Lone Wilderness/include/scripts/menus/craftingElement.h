/*! \file craftingElement.h
*
* \brief A script to handle the crafting items
*
* \author: Daniel Bullin
*
*/
#ifndef CRAFTINGELEMENT_H
#define CRAFTINGELEMENT_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/meshRender2D.h"

using namespace Engine;

class Player;

/*! \class CraftingElement
* \brief A script to handle the crafting items
*/
class CraftingElement : public NativeScript
{
private:
	MeshRender2D* m_render;
	glm::vec4 m_selectedTint;
	Player* m_player; //!< A pointer to the player script
	int m_itemIndex;
public:
	CraftingElement(); //!< Constructor
	CraftingElement(nlohmann::json scriptData); //!< Constructor
	~CraftingElement(); //!< Destructor
	void onAttach() override; //!< On attach
	void onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime) override;
	void onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse move
	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call before game update
	void onSubmit(const Renderers renderer, const std::string& renderState) override; //!< Call upon render if mesh render component attached

	uint32_t getItemIndex();
};
#endif
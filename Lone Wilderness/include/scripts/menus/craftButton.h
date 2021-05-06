/*! \file craftButton.h
*
* \brief A script to handle the craft button
*
* \author: Daniel Bullin
*
*/
#ifndef CRAFTBUTTON_H
#define CRAFTBUTTON_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/meshRender2D.h"

using namespace Engine;

class Player;

/*! \class CraftButton
* \brief A script to handle the craft button
*/
class CraftButton : public NativeScript
{
private:
	MeshRender2D* m_render;
	Material* m_highlightMaterial;
	Material* m_unhighlightMaterial;
	bool m_hovered;
	Player* m_player; //!< A pointer to the player script
public:
	CraftButton(); //!< Constructor
	~CraftButton(); //!< Destructor
	void onAttach() override; //!< On attach
	void onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime) override;
	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call before game update
	void onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse move
	void onSubmit(const Renderers renderer, const std::string& renderState) override; //!< Call upon render if mesh render component attached
};
#endif
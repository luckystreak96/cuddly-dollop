#ifndef MENU_BACKGROUND_BOX_H__
#define MENU_BACKGROUND_BOX_H__

#include "fontGraphicsComponent.h"
#include "mesh.h"

class MenuBackgroundBox : public FontGraphicsComponent
{
public:
	// Width and Height in tile sizes
	// If position is -1 for x y and z, itll center the menu
	MenuBackgroundBox(Vector3f position = Vector3f(0.2f, 0.1f, 0.0f), int width = 11, int height = 7);
	void Update();
protected:
	Mesh m_mesh;
	std::vector<GraphComp_ptr> m_boxParts;
};

#endif
#ifndef MENU_BACKGROUND_BOX_H__
#define MENU_BACKGROUND_BOX_H__

#include "fontGraphicsComponent.h"
#include "mesh.h"

class MenuBackgroundBox : public FontGraphicsComponent
{
public:
	MenuBackgroundBox(Vector3f position = Vector3f(), Vector3f size = Vector3f(10, 10, 1));
	void Update();
protected:
	Mesh m_mesh;
	std::vector<GraphComp_ptr> m_boxParts;
};

#endif
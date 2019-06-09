#ifndef MENU_COMPONENT_H__
#define MENU_COMPONENT_H__

#include "hudComponent.h"

class MenuComponent : public HudComponent
{
public:
	// active determines whether to handle input etc
	virtual std::shared_ptr<MenuComponent> Update(bool active);

protected:
	// should input be handled?
	bool m_active;
};

typedef std::shared_ptr<MenuComponent> MenuComp_ptr;

#endif // !MENU_COMPONENT_H__

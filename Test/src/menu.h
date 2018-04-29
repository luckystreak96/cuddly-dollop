#ifndef MENU_H__
#define MENU_H__

#include "menuComponent.h"
#include <deque>
#include <vector>

class Menu
{
public:
	// Load stuff and setup base active component
	Menu();

	// Update menu components, only active handles input
	virtual void Update();

	// Destroy components and close menu
	virtual void Close();

	// Set a new component as active
	virtual void PushActive(MenuComp_ptr menuComp);

	// Return to previous active component
	virtual void PopActive();

public:
	// Menu is done, gotta close
	bool _done;

protected:
	std::vector<MenuComp_ptr> m_components;
	std::deque<MenuComp_ptr> m_activeStack;
};

#endif // !MENU_H__
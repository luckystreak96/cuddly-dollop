#ifndef MENU_H__
#define MENU_H__

#include "menuComponent.h"
#include "graphics_component.h"
#include <deque>
#include <vector>

class Menu
{
public:
	// Load stuff and setup base active component
	Menu();

	// Update menu components, only active handles input
	virtual void Update();

	virtual void SetRender();

	// Load default page and open menu
	virtual void Open();

	// Load page and open menu
	virtual void Open(MenuComp_ptr page);

	// Destroy components and close menu
	virtual void Close();

	// Set a new component as active
	virtual void PushActive(MenuComp_ptr menuComp);

	// Return to previous active component
	virtual void PopActive();

	// Add a new component to know how to kickstart the menu
	virtual void AddComponent(MenuComp_ptr menuComp);

public:
	// Menu is done, gotta close
	bool _done;

protected:
	std::vector<MenuComp_ptr> m_components;
	std::deque<MenuComp_ptr> m_activeStack;

	std::vector<std::shared_ptr<GraphicsComponent>> _hudBG;
};

#endif // !MENU_H__

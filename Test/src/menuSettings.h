#ifndef MENU_SETTINGS_H__
#define MENU_SETTINGS_H__

#include "menuComponent.h"
#include "fontGraphicsComponent.h"
#include "menuOptions.h"

class MenuSettings : public MenuComponent
{
public:
	MenuSettings();

	// Handles observer updates
	virtual std::shared_ptr<MenuComponent> Update(bool active);

	virtual void Destroy();
	virtual void SetRender();
private:
	std::vector<GraphComp_ptr> m_backgrounds;
	MenuOptions m_options;
};

#endif // !MENU_PARTY_H__

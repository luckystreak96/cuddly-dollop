#ifndef MENU_SETTINGS_H__
#define MENU_SETTINGS_H__

#include "menuComponent.h"
#include "fontGraphicsComponent.h"
#include "menuOptions.h"

enum SettingsMenuOptions { SMO_Resume, SMO_Exit };

class MenuSettings : public MenuComponent
{
public:
	MenuSettings();

	// Handles observer updates
	virtual std::shared_ptr<MenuComponent> Update(bool active);
	virtual void HandleEvents(int choice);

	virtual void Destroy();
	virtual void SetRender();
private:
	std::vector<GraphComp_ptr> m_backgrounds;
	MenuOptions m_options;

	static std::map<SettingsMenuOptions, std::string> _SettingsMenuOptionsMap;
};

#endif // !MENU_PARTY_H__

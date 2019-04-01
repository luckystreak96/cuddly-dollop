#ifndef MENU_PARTY_H__
#define MENU_PARTY_H__

#include "menuComponent.h"
#include "fontGraphicsComponent.h"

class MenuParty : public MenuComponent
{
public:
	MenuParty();

	// Handles observer updates
	virtual void Update();

	virtual void Destroy();
	virtual void SetRender();
private:
	std::vector<std::shared_ptr<GraphicsComponent>> m_backgrounds;
};

#endif // !MENU_PARTY_H__

#include "menuParty.h"

#include "renderer.h"
#include "menuBackgroundBox.h"

MenuParty::MenuParty()
{
	m_backgrounds.push_back(std::shared_ptr<MenuBackgroundBox>(new MenuBackgroundBox(Vector3f(-1), 11, 7)));
}

void MenuParty::Update()
{
}

void MenuParty::Destroy()
{
	MenuComponent::Destroy();
}

void MenuParty::SetRender()
{
	for (auto& x : m_backgrounds)
		Renderer::GetInstance().Add(x);
}

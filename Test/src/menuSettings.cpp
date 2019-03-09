#include "menuSettings.h"

#include "renderer.h"
#include "menuBackgroundBox.h"
#include "command_factory.h"

MenuSettings::MenuSettings()
{
	std::shared_ptr<MenuBackgroundBox> bg = std::shared_ptr<MenuBackgroundBox>(new MenuBackgroundBox(Vector3f(-1), 4, 6));
	m_backgrounds.push_back(bg);

	// Build options
	std::vector<std::shared_ptr<MenuOption>> options;
	std::shared_ptr<MenuOption> mo;

	mo = std::shared_ptr<MenuOption>(new MenuOption("Resume"));
	mo->store_command(CommandFactory::create_exit_menu_command(this));
	options.push_back(mo);

	mo = std::shared_ptr<MenuOption>(new MenuOption("Mute/Unmute BGM"));
	mo->store_command(CommandFactory::create_mute_bgm_command());
	options.push_back(mo);

	mo = std::shared_ptr<MenuOption>(new MenuOption("Quit"));
	mo->store_command(CommandFactory::create_exit_game_command());
	options.push_back(mo);

	// Create buttons
	Vector3f pos;
	pos.x = ((bg->_bottomRight - bg->_topLeft).x / 2) + bg->_topLeft.x;
	pos.y = bg->_topLeft.y - 64;
	pos.z = bg->_topLeft.z - 1;
	m_options.init(options, pos, true);
	//m_options = MenuOptions(options, pos);
}

std::shared_ptr<MenuComponent> MenuSettings::Update(bool active)
{
	if (!active)
		return NULL;

	// A choice was selected
	if (m_options.Update())
	{
		int choice = m_options._selectedOption;
	}

	return NULL;
}

void MenuSettings::Destroy()
{
	MenuComponent::Destroy();
}

void MenuSettings::SetRender()
{
	for (auto& x : m_backgrounds)
		Renderer::GetInstance().Add(x);

	m_options.SetRender();
}

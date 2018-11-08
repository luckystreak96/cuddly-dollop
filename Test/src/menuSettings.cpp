#include "menuSettings.h"

#include "renderer.h"
#include "menuBackgroundBox.h"
#include "glfwBackend.h"

std::map<SettingsMenuOptions, std::string> MenuSettings::_SettingsMenuOptionsMap = 
{
	{SMO_Exit, "Exit"},
	{SMO_Resume, "Resume"}
};

MenuSettings::MenuSettings()
{
	std::shared_ptr<MenuBackgroundBox> bg = std::shared_ptr<MenuBackgroundBox>(new MenuBackgroundBox(Vector3f(-1), 4, 6));
	m_backgrounds.push_back(bg);

	std::vector<std::string> options;
	for (auto& x : _SettingsMenuOptionsMap)
		options.push_back(x.second);

	Vector3f pos;
	pos.x = ((bg->_bottomRight - bg->_topLeft).x / 2) + bg->_topLeft.x;
	pos.y = bg->_topLeft.y - 0.5f;
	pos.z = bg->_topLeft.z - 1;
	m_options.init(options, pos);
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
		HandleEvents(choice);
	}

	return NULL;
}

void MenuSettings::HandleEvents(int choice)
{
	SettingsMenuOptions smo = (SettingsMenuOptions)choice;
	switch (smo)
	{
	case SMO_Resume:
		_done = true;
		break;
	case SMO_Exit:
		GLFWManager::Exit();
		break;
	default:
		break;
	}
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

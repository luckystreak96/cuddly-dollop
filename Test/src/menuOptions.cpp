#include "menuOptions.h"

#include "input_manager.h"
#include "menuFactory.h"
#include "renderer.h"
#include "colors.h"

MenuOptions::MenuOptions() : m_enabled(false), _selectedOption(-1) {}

MenuOptions::~MenuOptions()
{
}

void MenuOptions::init(std::vector<std::shared_ptr<MenuOption>> options, Vector3f pos, bool centered)
{
	// If the new strings are the same as the old ones, then dont bother changing anything
	if (options.size() != 0 && options.size() == m_options.size() && pos == m_position)
	{
		for (int i = 0; i < options.size(); i++)
		{
			if (options[i]->_title == m_options[i]->_title)
			{
				m_enabled = true;
				m_firstUpdate = true;
				return;
			}
		}
	}

	m_position = pos;
	m_options = std::vector<std::shared_ptr<MenuOption>>(options);

	auto strings = std::vector<std::string>();
	for (auto x : m_options)
		strings.push_back(x->_title);

	if (m_fonts.size() >= options.size())
	{
		while (m_fonts.size() > options.size())
			m_fonts.pop_back();
		MenuFactory::UpdateOptions(strings, pos, centered, &m_fonts);
	}
	else
	{
		m_fonts.clear();
		m_fonts = MenuFactory::BuildOptions(OT_Text, strings, pos, centered);
	}

	//if(_selectedOption != -1 && _selectedOption < m_fonts.size())
	_selectedOption = 0;
	m_firstUpdate = true;
	m_enabled = true;
}

void MenuOptions::init(std::vector<std::string> options, Vector3f pos, bool centered)
{
	std::vector<std::shared_ptr<MenuOption>> opts;

	for(std::string s : options)
		opts.push_back(std::shared_ptr<MenuOption>(new MenuOption(s)));

	init(opts, pos, centered);
}

void MenuOptions::disable() { m_enabled = false; }
void MenuOptions::enable() { if (m_fonts.size()) m_enabled = true; }

bool MenuOptions::Update()
{
	if (!m_enabled)
		return false;

	bool result = false;
	bool mustUpdate = false;

	if (InputManager::GetInstance().FrameKeyStatus(std::vector<InputAction> { A_Up, A_Down, A_Left, A_Right, A_Accept, A_Cancel}, AnyPress, 2))
		mustUpdate = true;

	if (InputManager::GetInstance().FrameKeyStatus(A_Up, KeyPressed, 2))
		if (_selectedOption > 0)
			_selectedOption--;

	if (InputManager::GetInstance().FrameKeyStatus(A_Down, KeyStatus::KeyPressed, 2))
		if (_selectedOption < m_options.size() - 1)
			_selectedOption++;

	if (InputManager::GetInstance().FrameKeyStatus(A_Accept, KeyPressed, 2))
	{
	    m_options[_selectedOption]->execute_command();
		result = true;
	}

	if (mustUpdate || m_firstUpdate)
	{
		assert(m_fonts.size() == m_options.size());
		for (int i = 0; i < m_fonts.size(); i++)
		{
			if (i == _selectedOption)
				m_fonts[i].GetGraphics()->SetColorAll(colors::mysterious_red);
			else
				m_fonts[i].GetGraphics()->SetColorAll(colors::davy_grey);
		}

		m_firstUpdate = false;
	}

	return result;
}

void MenuOptions::SetRender()
{
	if (!m_enabled)
		return;

	for (auto& x : m_fonts)
		Renderer::GetInstance().Add(x.GetGraphics());
}


MenuOption::MenuOption(std::string title)
{
	_title = title;
}

//std::string MenuOptions::GetSelectedOption()
//{
//	assert(m_options.size() > 0);
//
//	if(m_options.size() > 0)
//		return m_options.at(m_selectedOption);
//
//	return "ERROR DIDN'T INIT MENUOPTIONS CLASS PROPERLY";
//}
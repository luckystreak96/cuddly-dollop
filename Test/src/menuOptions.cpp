#include "menuOptions.h"

#include "input_manager.h"
#include "menuFactory.h"
#include "renderer.h"

MenuOptions::MenuOptions()
{
}

MenuOptions::MenuOptions(std::vector<std::string> options, Vector3f pos) : m_position(pos), m_options(std::vector<std::string>(options))
{
	m_graphics = MenuFactory::BuildOptions(OT_Text, options, pos, true);
	_selectedOption = 0;
	m_firstUpdate = true;
}

bool MenuOptions::Update()
{
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
		result = true;

	if (mustUpdate || m_firstUpdate)
	{
		assert(m_graphics.size() == m_options.size());
		for (int i = 0; i < m_graphics.size(); i++)
		{
			if (i == _selectedOption)
				m_graphics[i]->SetColorAll(Vector3f(1, 0, 0));
			else
				m_graphics[i]->SetColorAll();
		}

		m_firstUpdate = false;
	}

	return result;
}

void MenuOptions::SetRender()
{
	for (auto& x : m_graphics)
		Renderer::GetInstance().Add(x);
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
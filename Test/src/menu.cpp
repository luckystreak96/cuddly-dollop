#include "menu.h"

#include "input_manager.h"

Menu::Menu()
{
	_done = true;
}

void Menu::Update()
{
	if (_done)
		return;

	for (int i = 0; i < m_components.size(); i++)
		PushActive(m_components.at(i)->Update(i == m_components.size() - 1));
}

void Menu::Close()
{
	for (auto& x : m_components)
		x->Destroy();

	_done = true;
}

void Menu::PushActive(MenuComp_ptr menuComp)
{
	// Only push back if there's something
	if (menuComp)
		m_activeStack.push_back(menuComp);
}

void Menu::PopActive()
{
	if (m_activeStack.size() == 1)
		Close();
	else
		m_activeStack.pop_back();
}

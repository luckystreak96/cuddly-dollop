#include "menu.h"

#include "menuParty.h"
#include "menuSettings.h"
#include "input_manager.h"

Menu::Menu()
{
	_done = true;
}

void Menu::AddComponent(MenuComp_ptr menuComp)
{
	m_components.push_back(menuComp);
	if (m_activeStack.size() == 0)
		PushActive(menuComp);
}

void Menu::Update()
{
	if (_done)
		return;

	for (int i = 0; i < m_components.size(); i++)
		PushActive(m_components.at(i)->Update(i == m_components.size() - 1));

	if (InputManager::GetInstance().FrameKeyStatus(InputAction::A_Cancel, AnyPress, 2) || m_activeStack.back()->_done)
		PopActive();
}

void Menu::SetRender()
{
	for (int i = 0; i < m_components.size(); i++)
		m_components.at(i)->SetRender();
}

void Menu::Close()
{
	// We dont want to unload everything, we just want to put it away

	for (auto& x : m_components)
		x->Destroy();

	m_components.clear();
	m_activeStack.clear();

	_done = true;
	InputManager::GetInstance().PopLockLevel();

	if (Camera::_currentCam)
		Camera::_currentCam->_paused = false;
}

void Menu::Open()
{
	InputManager::GetInstance().SetLockLevel(2);
	_done = false;
}

void Menu::Open(MenuComp_ptr page)
{
	AddComponent(page);
	InputManager::GetInstance().SetLockLevel(2);
	_done = false;
}

void Menu::PushActive(MenuComp_ptr menuComp)
{
	// Only push back if there's something
	if (menuComp)
		m_activeStack.push_back(menuComp);
}

void Menu::PopActive()
{
	if (m_activeStack.size() <= 1)
		Close();
	else
		m_activeStack.pop_back();
}

#include "input_manager.h"

InputManager::InputManager() : m_inputHold(new std::map<unsigned int, bool>()), m_inputQueue(new std::list<std::pair<unsigned int, KeyStatus>>())
{
}

InputManager::~InputManager()
{
	delete m_inputHold;
	delete m_inputQueue;
}

void InputManager::SetupFrameKeys()
{
	std::list<std::pair<unsigned int, KeyStatus>>* keys = InputManager::GetInstance().GetKeys();

	m_keyMap = std::map<std::pair<unsigned int, KeyStatus>, bool>();
	for (auto a : *keys)
		m_keyMap.emplace(a, false);
}

bool InputManager::FrameKeyStatus(unsigned int key, KeyStatus status)
{
	if (status == AnyPress)
		return m_inputHold->count(key) || m_keyMap.count(std::pair<unsigned int, KeyStatus>(key, KeyPressed)) || m_keyMap.count(std::pair<unsigned int, KeyStatus>(key, HoldDownPress));
	else if (status == AnyRelease)
		return m_keyMap.count(std::pair<unsigned int, KeyStatus>(key, Release)) || m_keyMap.count(std::pair<unsigned int, KeyStatus>(key, ReleaseLong)) || m_keyMap.count(std::pair<unsigned int, KeyStatus>(key, ReleaseQuick));
	return m_keyMap.count(std::pair<unsigned int, KeyStatus>(key, status));
}

std::list<std::pair<unsigned int, KeyStatus>>* InputManager::GetKeys()
{
	std::list<std::pair<unsigned int, KeyStatus>>* temp = m_inputQueue;
	m_inputQueue = new std::list<std::pair<unsigned int, KeyStatus>>();
	return temp;
}

std::map<unsigned int, bool>* InputManager::GetHeldKeys()
{
	return m_inputHold;
}

void InputManager::Input(unsigned int key, bool keydown)
{
	if (keydown)
	{
		if (m_inputHold->count(key) != 0)
			m_inputQueue->push_back(std::pair<unsigned int, KeyStatus>(key, HoldDownPress));
		else
			m_inputQueue->push_back(std::pair<unsigned int, KeyStatus>(key, KeyPressed));

		m_inputHold->emplace(key, true);
	}
	else
	{
		//implement ReleaseQuick and ReleaseLong
		m_inputQueue->push_back(std::pair<unsigned int, KeyStatus>(key, Release));
		m_inputHold->erase(key);
	}
}

void InputManager::SpecialInput(unsigned int key, bool keydown)
{
	key += SpecialKeyValue;
	Input(key, keydown);
}

int InputManager::FindKey(std::list<std::pair<unsigned int, KeyStatus>>* list, unsigned int key)
{
	int counter = 0;
	for (auto a : *list)
	{
		if (a.first == key)
			return counter;
		counter++;
	}

	return -1;
}

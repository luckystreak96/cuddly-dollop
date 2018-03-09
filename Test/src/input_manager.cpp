#include "input_manager.h"
#include <cassert>
#include "gameData.h"

InputManager::InputManager() : m_inputHold(std::map<unsigned int, bool>()), m_inputQueue(std::list<std::pair<unsigned int, KeyStatus>>()), m_lockLevel(0)
{
}

InputManager::~InputManager()
{
}

void InputManager::SetupFrameKeys()
{
	std::list<std::pair<unsigned int, KeyStatus>> keys = InputManager::GetInstance().GetKeys();

	m_keyMap = std::map<std::pair<unsigned int, KeyStatus>, bool>();
	for (auto a : keys)
		m_keyMap.emplace(a, false);
}

bool InputManager::FrameKeyStatus(Action action, KeyStatus status, unsigned int accessLevel)
{
	assert(GameData::KeyMap.count(action));
	unsigned int actionNum = GameData::KeyMap.count(action) ? GameData::KeyMap.at(action) : 0;
	return FrameKeyStatus(actionNum, status, accessLevel);
}

bool InputManager::FrameKeyStatus(unsigned int key, KeyStatus status, unsigned int accessLevel)
{
	//The key is never considered pressed to a method that doesnt currently have access
	if (accessLevel < m_lockLevel)
		return false;

	if (status == AnyPress)
		return m_inputHold.count(key) || m_keyMap.count(std::pair<unsigned int, KeyStatus>(key, KeyPressed)) || m_keyMap.count(std::pair<unsigned int, KeyStatus>(key, HoldDownPress));
	else if (status == AnyRelease)
		return m_keyMap.count(std::pair<unsigned int, KeyStatus>(key, Release)) || m_keyMap.count(std::pair<unsigned int, KeyStatus>(key, ReleaseLong)) || m_keyMap.count(std::pair<unsigned int, KeyStatus>(key, ReleaseQuick));
	return m_keyMap.count(std::pair<unsigned int, KeyStatus>(key, status));
}

std::list<std::pair<unsigned int, KeyStatus>> InputManager::GetKeys()
{
	std::list<std::pair<unsigned int, KeyStatus>> temp = std::list<std::pair<unsigned int, KeyStatus>>(m_inputQueue);
	m_inputQueue = std::list<std::pair<unsigned int, KeyStatus>>();
	return temp;
}

std::list<std::pair<unsigned int, KeyStatus>> InputManager::GetKeysNoReset()
{
	std::list<std::pair<unsigned int, KeyStatus>> temp = std::list<std::pair<unsigned int, KeyStatus>>(m_inputQueue);
	return temp;
}

std::map<unsigned int, bool> InputManager::GetHeldKeys()
{
	return m_inputHold;
}

void InputManager::Input(unsigned int key, bool keydown)
{
	if (keydown)
	{
		if (m_inputHold.count(key) != 0)
			m_inputQueue.push_back(std::pair<unsigned int, KeyStatus>(key, HoldDownPress));
		else
			m_inputQueue.push_back(std::pair<unsigned int, KeyStatus>(key, KeyPressed));

		m_inputHold.emplace(key, true);
	}
	else
	{
		//implement ReleaseQuick and ReleaseLong
		m_inputQueue.push_back(std::pair<unsigned int, KeyStatus>(key, Release));
		m_inputHold.erase(key);
	}
}

void InputManager::Input(unsigned int key, KeyStatus status)
{
	if (status == KeyStatus::HoldDownPress || status == KeyStatus::KeyPressed)
	{
		if (m_inputHold.count(key) != 0)
			m_inputQueue.push_back(std::pair<unsigned int, KeyStatus>(key, HoldDownPress));
		else
			m_inputQueue.push_back(std::pair<unsigned int, KeyStatus>(key, KeyPressed));

		m_inputHold.emplace(key, true);
	}
	else
	{
		//implement ReleaseQuick and ReleaseLong
		m_inputQueue.push_back(std::pair<unsigned int, KeyStatus>(key, Release));
		m_inputHold.erase(key);
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

void InputManager::SetLockLevel(unsigned int level)
{
	m_lockLevel = level;
}

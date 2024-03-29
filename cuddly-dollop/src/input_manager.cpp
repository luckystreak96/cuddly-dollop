#include "input_manager.h"
#include <cassert>
#include <algorithm>
#include <set>
#include "gameData.h"

// Make sure to add a default value in GameData
std::map<InputAction, std::string> InputManager::InputActionStrings = {
	std::make_pair(A_Accept, "Accept"),
	std::make_pair(A_Cancel, "Cancel"),
	std::make_pair(A_Mute, "Mute"),
	std::make_pair(A_Exit, "Exit"),
	std::make_pair(A_Pause, "Pause"),
	std::make_pair(A_Right, "Right"),
	std::make_pair(A_Left, "Left"),
	std::make_pair(A_Down, "Down"),
	std::make_pair(A_Up, "Up"),
	std::make_pair(A_Menu, "Menu"),
	std::make_pair(A_AltR, "AltR")
};

InputManager::InputManager() : m_inputQueue(std::list<std::pair<unsigned int, KeyStatus>>()), m_lockLevel(0)
{
	m_lockLevel.push_front(0);
}

InputManager::~InputManager()
{
}

void InputManager::SetupFrameKeys()
{
	m_textKeys = std::move(m_textKeysBuffer);
	m_textKeysBuffer.clear();
	std::list<std::pair<unsigned int, KeyStatus>> keys = InputManager::GetInstance().GetKeys();

	std::vector<unsigned int> toDelete = std::vector<unsigned int>();
	std::vector<unsigned int> toRelease = std::vector<unsigned int>();
	std::set<unsigned int> buttons;
	//m_keyMap = std::map<std::pair<unsigned int, KeyStatus>, bool>();

	for (auto& x : m_keyMap)
	{
		// What was released needs to be removed now
		if (x.second.first == KeyStatus::Release)
			toDelete.push_back(x.first);
	}

	// Handle release logic
	for (auto& o : keys)
	{
		if (m_keyMap.count(o.first) && o.second == Release)
			toRelease.push_back(o.first);
	}

	// If the button wasnt pressed before, lets make it be pressed
	for (auto& x : keys)
	{
		// != release because we don't want to receive release inputs if there was no press beforehand
		if (!m_keyMap.count(x.first) && x.second != Release)
		{
			m_keyMap.emplace(x.first, std::pair<KeyStatus, float>(x.second, 1));
			buttons.emplace(x.first);
		}
	}

	for (auto& x : toRelease)
	{
		//m_keyMap.erase(x);
		if (m_keyMap.count(x))
			m_keyMap[x].first = Release;
	}

	// Delete the ToDelete list here
	for (auto x : toDelete)
		m_keyMap.erase(x);

	// make all the keypress holdpress if its been chilling for at least a frame
	for (auto& a : m_keyMap)
	{
		if (buttons.count(a.first))
			continue;
		if (a.second.first == KeyStatus::KeyPressed)
			a.second.first = HoldDownPress;
	}

	UpdateObservers();
}

unsigned int InputManager::GetKeyCount()
{
	return m_keyMap.size();
}

bool InputManager::FrameKeyStatus(InputAction action, KeyStatus status, unsigned int accessLevel)
{
	if (!GameData::KeyMap[action])
		return false;
	//unsigned int actionNum = GameData::KeyMap.count(action) ? GameData::KeyMap.at(action) : 0;
	return FrameKeyStatus(GameData::KeyMap[action], status, accessLevel);
}

bool InputManager::FrameKeyStatus(std::vector<InputAction> action, KeyStatus status, unsigned int accessLevel)
{
	for (auto& x : action)
	{
		if (!GameData::KeyMap[x])
			continue;
		//assert(GameData::KeyMap.count(x));
		//unsigned int actionNum = GameData::KeyMap.count(x) ? GameData::KeyMap.at(x) : 0;
		if (FrameKeyStatus(GameData::KeyMap[x], status, accessLevel))
			return true;
	}

	return false;
}

bool InputManager::FrameKeyStatus(unsigned int key, KeyStatus status, unsigned int accessLevel)
{
	//auto it = m_keyMap.find(key);
	if (!m_keyMap.count(key))
		return false;

	//The key is never considered pressed to a method that doesnt currently have access
	if (accessLevel < m_lockLevel.front())
		return false;

	if (status == AnyPress)
		return (m_keyMap[key].first == KeyPressed || m_keyMap[key].first == HoldDownPress);

	return m_keyMap[key].first == status;
}

std::list<std::pair<unsigned int, KeyStatus>> InputManager::GetKeys()
{
	if (m_inputQueue.size() == 0)
		return m_inputQueue;
	std::list<std::pair<unsigned int, KeyStatus>> temp = std::move(m_inputQueue);
	m_inputQueue.clear();
	return temp;
}

std::list<std::pair<unsigned int, KeyStatus>> InputManager::GetKeysNoReset()
{
	std::list<std::pair<unsigned int, KeyStatus>> temp = std::list<std::pair<unsigned int, KeyStatus>>(m_inputQueue);
	return temp;
}

void InputManager::Input(unsigned int key, bool keydown)
{
	if (keydown)
		m_inputQueue.push_back(std::pair<unsigned int, KeyStatus>(key, KeyPressed));
	else
		m_inputQueue.push_back(std::pair<unsigned int, KeyStatus>(key, Release));
}

void InputManager::Input(unsigned int key, KeyStatus status)
{
	m_inputQueue.push_back(std::pair<unsigned int, KeyStatus>(key, status));
}


void InputManager::SetKeyPercent(InputAction action, float percent)
{
	unsigned int actionNum = GameData::KeyMap[action];
	if (actionNum == 0)
		return;
	if (!m_powers.count(actionNum))
		m_powers.emplace(actionNum, percent);
	else
		m_powers[actionNum] = percent;
}

float InputManager::GetKeyPercent(InputAction action)
{
	unsigned int actionNum = GameData::KeyMap[action];
	if (actionNum == 0)
		return 1;
	if (m_powers.count(actionNum))
		return m_powers[actionNum];
	else
		return 1;
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
	m_lockLevel.push_front(level);
}

void InputManager::PopLockLevel()
{
	if (m_lockLevel.size() > 0)
		m_lockLevel.pop_front();

	if (m_lockLevel.size() == 0)
		m_lockLevel.push_back(0);
}


// Returns the action associated with its string value
InputAction InputManager::StringToInputAction(std::string str)
{
	for (auto x : InputActionStrings)
		// If we find the matching result...
		if (x.second == str)
			return x.first;

	return A_Last;
}

std::vector<unsigned int> InputManager::FrameTypingKeys()
{
	std::vector<unsigned int> result;

    for(auto x : m_textKeys)
		result.push_back(x);

    //for(auto x : result)
    //	std::cout << x << std::endl;

    return result;
}

void InputManager::InputText(unsigned int codepoint)
{
	m_textKeysBuffer.push_back(codepoint);
	//std::cout<< codepoint << std::endl;
}


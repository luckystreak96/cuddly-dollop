#include "input_manager.h"
#include <cassert>
#include <set>
#include "gameData.h"

std::map<InputAction, std::string> InputManager::InputActionStrings = {
	std::make_pair(A_Accept, "Accept"),
	std::make_pair(A_Cancel, "Cancel"),
	std::make_pair(A_Mute, "Mute"),
	std::make_pair(A_Exit, "Exit"),
	std::make_pair(A_Pause, "Pause"),
	std::make_pair(A_Right, "Right"),
	std::make_pair(A_Left, "Left"),
	std::make_pair(A_Down, "Down"),
	std::make_pair(A_Up, "Up")
};

InputManager::InputManager() : m_inputQueue(std::list<std::pair<unsigned int, KeyStatus>>()), m_lockLevel(0)
{
}

InputManager::~InputManager()
{
}

void InputManager::SetupFrameKeys()
{
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
		if (!m_keyMap.count(x.first))
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
}

bool InputManager::FrameKeyStatus(InputAction action, KeyStatus status, unsigned int accessLevel)
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
		return m_keyMap.count(key) && (m_keyMap[key].first == KeyPressed || m_keyMap[key].first == HoldDownPress);

	return m_keyMap.count(key) && m_keyMap[key].first == status;
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
	unsigned int actionNum = GameData::KeyMap.count(action) ? GameData::KeyMap.at(action) : 0;
	if (!m_powers.count(actionNum))
		m_powers.emplace(actionNum, percent);
	else
		m_powers[actionNum] = percent;
}

float InputManager::GetKeyPercent(InputAction action)
{
	unsigned int actionNum = GameData::KeyMap.count(action) ? GameData::KeyMap.at(action) : 0;
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
	m_lockLevel = level;
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


#ifndef INPUT_MANAGER_H__
#define INPUT_MANAGER_H__

#include <list>
#include <map>
#include <vector>
#include <deque>
#include <utility>
#include <string>
//#include <iostream>
#include "key_status.h"
#include "observable.h"

// Actions to assign the buttons to
enum InputAction { A_Accept, A_Cancel, A_Mute, A_Exit, A_Pause, A_Right, A_Left, A_Down, A_Up, A_Menu, A_AltR, A_Last };

// Holds all inputs in one place to be easily accessed across the whole app
class InputManager : public Observable
{
public:
	// The class is a singleton instead of being completely static
	static InputManager& GetInstance()
	{
		static InputManager instance;
		return instance;
	}

	// Set lock level
	void SetLockLevel(unsigned int level);

	// Pop lock level
	void PopLockLevel();

	// Clears list of keys
	void SetupFrameKeys();

	// Gets UTF32 codepoint character
	void InputText(unsigned int codepoint);

	// Adds an input to the list, called when you press the key
	void Input(unsigned int key, bool keydown);

	// Adds an input to the list, you can specify the status
	void Input(unsigned int key, KeyStatus status);

	// Adds an input to the list, called when you press a special key
	void SpecialInput(unsigned int key, bool keydown);

	// Returns the list of keys, and resets it
	std::list<std::pair<unsigned int, KeyStatus>> GetKeys();

	// Returns the list of keys, and doesnt reset it
	std::list<std::pair<unsigned int, KeyStatus>> GetKeysNoReset();

	// Does something, but its never used
	static int FindKey(std::list<std::pair<unsigned int, KeyStatus>>* list, unsigned int key);

	// Returns the status of the key that frame, to be called after SetupFrameKeys
	// If the accessLevel is lower than the lockLevel, the result is always nothing
	bool FrameKeyStatus(unsigned int key, KeyStatus status = AnyPress, unsigned int accessLevel = 0);
	bool FrameKeyStatus(InputAction action, KeyStatus status = AnyPress, unsigned int accessLevel = 0);
	// This version checks if any of the keys are down
	bool FrameKeyStatus(std::vector<InputAction> action, KeyStatus status = AnyPress, unsigned int accessLevel = 0);

	std::vector<unsigned int> FrameTypingKeys();

	// percent goes from -1 to 1
	void SetKeyPercent(InputAction action, float percent);

	float GetKeyPercent(InputAction action);

	unsigned int GetKeyCount();

	static std::map<InputAction, std::string> InputActionStrings;
	static InputAction StringToInputAction(std::string str);

public:
	// Adds a value to the pressed characters so theres no conflicts with the other letters
	static const unsigned int SpecialKeyValue = 700;

private:
	InputManager();
	~InputManager();

	// When locked is true, you need to access the input with a certain priority for it to work
	std::deque<unsigned int> m_lockLevel;
	std::map<unsigned int, float> m_powers;
	std::map<unsigned int, std::pair<KeyStatus, float>> m_keyMap;
	std::vector<unsigned int> m_textKeys;
	std::vector<unsigned int> m_textKeysBuffer;
	std::list<std::pair<unsigned int, KeyStatus>> m_inputQueue;
};

#endif // !INPUT_MANAGER_H__

#ifndef INPUT_MANAGER_H__
#define INPUT_MANAGER_H__

#include "array2d.h"
#include <list>
#include <map>
#include <vector>
#include <utility>
#include "key_status.h"


// Holds all inputs in one place to be easily accessed across the whole app
class InputManager
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

	// Clears list of keys
	void SetupFrameKeys();

	// Adds an input to the list, called when you press the key
	void Input(unsigned int key, bool keydown);

	// Adds an input to the list, you can specify the status
	void Input(unsigned int key, KeyStatus status);

	// Adds an input to the list, called when you press a special key
	void SpecialInput(unsigned int key, bool keydown);

	// Returns the list of keys, and resets it
	std::list<std::pair<unsigned int, KeyStatus>> GetKeys();

	// Returns the list of keys, and resets it
	std::list<std::pair<unsigned int, KeyStatus>> GetKeysNoReset();

	// Gets a list of currently held keys
	std::map<unsigned int, bool> GetHeldKeys();

	// Does something, but its never used
	static int FindKey(std::list<std::pair<unsigned int, KeyStatus>>* list, unsigned int key);

	// Returns the status of the key that frame, to be called after SetupFrameKeys
	// If the accessLevel is lower than the lockLevel, the result is always nothing
	bool FrameKeyStatus(unsigned int key, KeyStatus status = AnyPress, unsigned int accessLevel = 0);

public:
	// Adds a value to the pressed characters so theres no conflicts with the other letters
	static const unsigned int SpecialKeyValue = 700;

private:
	InputManager();
	~InputManager();

	// When locked is true, you need to access the input with a certain priority for it to work
	unsigned int m_lockLevel;
	std::map<unsigned int, bool> m_inputHold;
	std::map<std::pair<unsigned int, KeyStatus>, bool> m_keyMap;
	std::list<std::pair<unsigned int, KeyStatus>> m_inputQueue;
};

#endif // !INPUT_MANAGER_H__

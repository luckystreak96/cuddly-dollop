#ifndef INPUT_MANAGER_H__
#define INPUT_MANAGER_H__

#include "array2d.h"
#include <list>
#include <map>
#include <vector>
#include <utility>
#include "key_status.h"

class InputManager
{
public:
	static InputManager& GetInstance()
	{
		static InputManager instance;
		return instance;
	}

	void SetupFrameKeys();
	void Input(unsigned int key, bool keydown);
	void SpecialInput(unsigned int key, bool keydown);
	std::list<std::pair<unsigned int, KeyStatus>> GetKeys();
	std::map<unsigned int, bool> GetHeldKeys();
	static int FindKey(std::list<std::pair<unsigned int, KeyStatus>>* list, unsigned int key);
	bool FrameKeyStatus(unsigned int key, KeyStatus status = AnyPress);

public:
	static const unsigned int SpecialKeyValue = 700;

private:
	InputManager();
	~InputManager();
	std::map<unsigned int, bool> m_inputHold;
	std::map<std::pair<unsigned int, KeyStatus>, bool> m_keyMap;
	//std::map<unsigned int, bool> m_heldKeyMap;
	std::list<std::pair<unsigned int, KeyStatus>> m_inputQueue;
};

#endif // !INPUT_MANAGER_H__

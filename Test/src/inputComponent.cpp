#include "inputComponent.h"

InputComponent::InputComponent() : m_pos(Vector3f())
{

}


std::vector<std::string> InputComponent::Interact()
{
	//unsigned int font = FontManager::GetInstance().AddFont(false, true, "res/fonts/spooky.png");
	unsigned int font = FontManager::GetInstance().AddDialogueBox();
	Vector3f pos = m_pos;
	pos.y += 1.0f;
	pos.z = 0;
	pos.x += 0.5f;
	FontManager::GetInstance().SetScale(font, 0.5f, 0.5f);
	FontManager::GetInstance().SetTextSpeed(font, 0.03f);
	FontManager::GetInstance().SetText(font, "Hello!", pos, true);
	//InputManager::GetInstance().SetLockLevel(1);
	return std::vector<std::string>()/* = { "TELEPORT", "6", "6", "4" }*/; 
}

void InputComponent::ReceiveMessage(std::vector<std::string> msg)
{
	if (msg.size() == 0)
		return;
	if (msg.at(0) == "SET_POSITION")
	{
		if (msg.size() >= 4)
		{
			m_pos = Vector3f((float)::atof(msg.at(1).c_str()), (float)::atof(msg.at(2).c_str()), (float)::atof(msg.at(3).c_str()));
		}
	}
}


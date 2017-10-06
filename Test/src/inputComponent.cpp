#include "inputComponent.h"

InputComponent::InputComponent() : m_pos(Vector3f()), m_eventQueue(EventQueue())
{

}

void InputComponent::Update()
{
	m_eventQueue.Update(ElapsedTime::GetInstance().GetElapsedTime());
}


std::vector<std::string> InputComponent::Interact()
{
	DialogueBox* db = new DialogueBox();
	db->SetScale(0.5f, 0.5f);
	db->SetTextSpeed(0.03f);
	db->SetText("Hello! \nHaha!\nCoolCats.\n");

	DialogueBox* db2 = new DialogueBox();
	db2->SetScale(0.7f, 0.7f);
	db2->SetTextSpeed(0.2f);
	db2->SetText("I'm talking to you right now! Isn't this great? We should hang out sometime! Have a blast! You'll see! It'll be so great, we'll eat everyone's apples and they'll all be so mad! Haha! Apples really are great! Andyways dude, I gotta go. My wife is waiting for me. You have a nice day and take care of yourself, okay!?");

	m_eventQueue.PushBack(db);
	m_eventQueue.PushBack(db2);

	//unsigned int font = FontManager::GetInstance().AddDialogueBox();
	//Vector3f pos = m_pos;
	//pos.y += 1.0f;
	//pos.z = 0;
	//pos.x += 0.5f;
	//FontManager::GetInstance().SetScale(font, 0.5f, 0.5f);
	//FontManager::GetInstance().SetTextSpeed(font, 0.03f);
	//FontManager::GetInstance().SetText(font, "Hello!", pos, true);

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


#include "inputComponent.h"

InputComponent::InputComponent() : m_pos(Vector3f())/*, m_eventQueue(EventQueue())*/
{

}

void InputComponent::Update()
{
	//m_eventQueue.Update(ElapsedTime::GetInstance().GetElapsedTime());
}


//std::vector<std::string> InputComponent::Interact(EventQueue* eq, unsigned int parent_id)
//{
//	// Load the file (could be made to load only once at the start?)
//	rapidjson::Document doc = JsonHandler::LoadJsonFromFile(DATA_FILE);
//
//	// If the json file has entities and the entities is an array
//	if (doc.HasMember("entities") && doc["entities"].IsArray())
//	{
//		// Shortcut notation
//		const auto& arr = doc["entities"].GetArray();
//
//		// Go through the entities
//		for (unsigned int i = 0; i < arr.Size(); i++)
//		{
//			// If the entity is you...
//			if (arr[i]["id"] == parent_id)
//			{
//				// Add your events to the event queue
//				const auto& evts = arr[i]["events"].GetArray();
//				for (auto& x : evts)
//				{
//					// Find a way to add args to the event
//					IEvent* ev = EventFactory::BuildEvent((EventTypes)EventFactory::TypeDict.at(x["type"].GetString()), std::vector<std::string>());
//					eq->PushBack(ev);
//				}
//			}
//		}
//	}
//
//	return std::vector<std::string>()/* = { "TELEPORT", "6", "6", "4" }*/;
//}

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


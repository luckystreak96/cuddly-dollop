#include "eventFactory.h"

std::map<std::string, unsigned int> EventFactory::TypeDict =
{
	{ "teleport", ET_Teleport },
	{ "dialogue", ET_DialogueBox },
	{ "move_right", ET_MoveRight },
	{ "move_down", ET_MoveDown },
	{ "move_up", ET_MoveUp}
};

std::map<std::string, unsigned int> EventFactory::EEMDict =
{
	{ "blocking", BLOCKING },
	{ "async", ASYNC }
};

IEvent* EventFactory::BuildEvent(EventTypes et, std::vector<std::string> s)
{
	//if (s.size() <= 0)
	//	return NULL;

	IEvent* result = NULL;

	switch (et)
	{
	case EventTypes::ET_Teleport:
		break;
	case EventTypes::ET_MoveRight:
		result = new EventMove(2, 3.0f, 1);
		break;
	case EventTypes::ET_MoveUp:
		result = new EventMove(2, 3.0f, 0);
		break;
	case EventTypes::ET_MoveDown:
		result = new EventMove(2, 3.0f, 2);
		break;
	case EventTypes::ET_DialogueBox:
		//No args = default text
		if (s.size() <= 0)
		{
			result = new DialogueBox(new DialogueGraph());
			break;
		}
		break;
	default:
		break;
	}

	return result;
}

std::vector<EventQueue> EventFactory::LoadEvent(unsigned int entity_id, std::string DATA_FILE)
{
	std::vector<EventQueue> result = std::vector<EventQueue>();

	// Load the file (could be made to load only once at the start?)
	rapidjson::Document doc = JsonHandler::LoadJsonFromFile(DATA_FILE);

	// If the json file has entities and the entities is an array
	if (doc.HasMember("entities") && doc["entities"].IsArray())
	{
		// Shortcut notation
		const auto& arr = doc["entities"].GetArray();

		// Go through the entities
		for (unsigned int i = 0; i < arr.Size(); i++)
		{
			// If the entity is you...
			if (arr[i]["id"] == entity_id)
			{
				// Check out all the queues
				const auto& ques = arr[i]["queues"].GetArray();
				for (auto& x : ques)
				{
					//Make sure that the event is flagged as valid
					if (!x.HasMember("flag") || x["flag"].GetInt() == 1)
					{
						EventQueue queue = EventQueue();

						// Set repeating if necessary
						if (x.HasMember("repeating") && x["repeating"].GetBool() == true)
							queue.SetRepeating(true);

						// Add your events to the event queue
						const auto& evts = x["events"].GetArray();
						for (auto& e : evts)
						{
							// Make sure the event exists
							if (TypeDict.find(e["type"].GetString()) != TypeDict.end())
							{
								// Find a way to add args to the event
								IEvent* ev = EventFactory::BuildEvent((EventTypes)TypeDict.at(e["type"].GetString()), std::vector<std::string>());

								// Set execution mode if necessary
								if (e.HasMember("execution_type") && EEMDict.find(e["execution_type"].GetString()) != EEMDict.end())
									ev->SetExecutionMode((EventExecutionMode)EEMDict.at(e["execution_type"].GetString()));

								queue.PushBack(ev);
							}
						}
						if (queue.Count() > 0)
							result.push_back(queue);
					}
				}
			}
		}
	}

	return result;
}


void EventFactory::FlagEvent(unsigned int entity_id, unsigned int queue_id, int flag, std::string DATA_FILE)
{

	// Load the file (could be made to load only once at the start?)
	rapidjson::Document doc = JsonHandler::LoadJsonFromFile(DATA_FILE);

	// If the json file has entities and the entities is an array
	if (doc.HasMember("entities") && doc["entities"].IsArray())
	{
		// Shortcut notation
		const auto& arr = doc["entities"].GetArray();

		// Go through the entities
		for (unsigned int i = 0; i < arr.Size(); i++)
		{
			// If the entity is you...
			if (arr[i]["id"] == entity_id)
			{
				// Check out all the queues
				const auto& ques = arr[i]["queues"].GetArray();
				for (auto& x : ques)
				{
					if (x.HasMember("id") && x["id"].GetInt() == queue_id)
					{
						//Make sure that the event has a flag property
						if (x.HasMember("flag"))
						{
							x["flag"].SetInt(flag);
						}
					}
				}
			}
		}
	}
}
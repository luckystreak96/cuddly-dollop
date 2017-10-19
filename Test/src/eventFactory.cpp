#include "eventFactory.h"

std::map<std::string, unsigned int> EventFactory::TypeDict =
{
	{ "teleport", ET_Teleport },
	{ "dialogue", ET_DialogueBox },
	{ "move_right", ET_MoveRight },
	{ "move_down", ET_MoveDown },
	{ "move_up", ET_MoveUp },
	{ "move_left", ET_MoveLeft}
};

std::map<std::string, unsigned int> EventFactory::EEMDict =
{
	{ "blocking", BLOCKING },
	{ "async", ASYNC }
};

IEvent* EventFactory::BuildEvent(EventTypes et, std::map<std::string, EventArgType> args)
{
	//if (s.size() <= 0)
	//	return NULL;

	IEvent* result = NULL;

	switch (et)
	{
	case EventTypes::ET_Teleport:
		break;
	case EventTypes::ET_MoveRight:
		result = new EventMove(3, 3.0f, 1);
		break;
	case EventTypes::ET_MoveUp:
		result = new EventMove(
			3, 
			args.count("distance") ? std::get<float>(args.at("distance")) : 3.0f,
			0);
		break;
	case EventTypes::ET_MoveDown:
		result = new EventMove(3,
			args.count("distance") ? std::get<float>(args.at("distance")) : 3.0f,
			2);
		break;
	case EventTypes::ET_MoveLeft:
		result = new EventMove(3, 3.0f, 3);
		break;
	case EventTypes::ET_DialogueBox:
		result = new DialogueBox(2, new DialogueGraph());
		break;
	default:
		break;
	}

	return result;
}

std::vector<EventQueue> EventFactory::LoadEvent(int map_id, unsigned int entity_id)
{
	std::vector<EventQueue> result = std::vector<EventQueue>();

	auto& ques = JsonHandler::LoadQueues(map_id, entity_id).GetArray();
	for (auto& x : ques)
	{
		//Make sure that the event is flagged as valid
		if (!x.HasMember("flag") || x["flag"].GetInt() == 1)
		{
			int id = -1;
			if (x.HasMember("id"))
				id = x["id"].GetInt();
			EventQueue queue = EventQueue(id);

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
					std::map<std::string, EventArgType> args = std::map<std::string, EventArgType>();

					// If there are args
					if (e.HasMember("args"))
						for (rapidjson::Value::ConstMemberIterator iter = e["args"].MemberBegin(); iter != e["args"].MemberEnd(); ++iter)
						{
							EventArgType eat;
							rapidjson::Type t;
							t = iter->value.GetType();
							switch (t)
							{
							case rapidjson::Type::kNumberType:
								if (iter->value.IsFloat())
									eat = iter->value.GetFloat();
								if (iter->value.IsInt())
									eat = iter->value.GetInt();
								break;
							case rapidjson::Type::kStringType:
								eat = iter->value.GetString();
								break;
							case rapidjson::Type::kTrueType:
								eat = iter->value.GetBool();
								break;
							case rapidjson::Type::kFalseType:
								eat = iter->value.GetBool();
								break;
							}

							args.emplace(std::string(iter->name.GetString()), eat);
						}

					// If the event has a queue in it
					if (e.HasMember("queues"))
					{
						EventArgType eat;
						rapidjson::Value val = e["queues"].GetArray();
						args.emplace("queue", LoadEvent(val));
					}

					IEvent* ev = EventFactory::BuildEvent((EventTypes)TypeDict.at(e["type"].GetString()), args);

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
	return result;
}

// Recursive AS FUCK
std::vector<EventQueue> EventFactory::LoadEvent(rapidjson::Value& v)
{
	std::vector<EventQueue> result = std::vector<EventQueue>();

	for (auto& p : v.GetArray())
	{
		EventQueue queue = EventQueue();
		// Set repeating if necessary
		if (p.HasMember("repeating") && p["repeating"].GetBool() == true)
			queue.SetRepeating(true);

		// Add your events to the event queue
		const auto& evts = p["events"].GetArray();
		for (auto& e : evts)
		{
			// Make sure the event exists
			if (TypeDict.find(e["type"].GetString()) != TypeDict.end())
			{
				std::map<std::string, EventArgType> args = std::map<std::string, EventArgType>();

				// If there are args
				if (e.HasMember("args"))
					for (rapidjson::Value::ConstMemberIterator iter = e["args"].MemberBegin(); iter != e["args"].MemberEnd(); ++iter)
					{
						EventArgType eat;
						rapidjson::Type t;
						t = iter->value.GetType();
						switch (t)
						{
						case rapidjson::Type::kNumberType:
							if (iter->value.IsFloat())
								eat = iter->value.GetFloat();
							if (iter->value.IsInt())
								eat = iter->value.GetInt();
							break;
						case rapidjson::Type::kStringType:
							eat = iter->value.GetString();
							break;
						case rapidjson::Type::kTrueType:
							eat = iter->value.GetBool();
							break;
						case rapidjson::Type::kFalseType:
							eat = iter->value.GetBool();
							break;
						}

						args.emplace(std::string(iter->name.GetString()), eat);
					}


				// If the event has a queue in it
				if (e.HasMember("queues"))
				{
					EventArgType eat;
					rapidjson::Value val = e["queues"].GetArray();
					args.emplace("queue", LoadEvent(val));
				}

				IEvent* ev = EventFactory::BuildEvent((EventTypes)TypeDict.at(e["type"].GetString()), args);

				// Set execution mode if necessary
				if (e.HasMember("execution_type") && EEMDict.find(e["execution_type"].GetString()) != EEMDict.end())
					ev->SetExecutionMode((EventExecutionMode)EEMDict.at(e["execution_type"].GetString()));

				queue.PushBack(ev);
			}
		}
		if (queue.Count() > 0)
			result.push_back(queue);
	}
	return result;
}



void EventFactory::FlagEvent(int map_id, unsigned int entity_id, unsigned int queue_id, int flag, std::string DATA_FILE)
{
	// Check out all the queues
	const auto& ques = JsonHandler::LoadQueues(map_id, entity_id).GetArray();
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

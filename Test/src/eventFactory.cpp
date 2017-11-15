#include "eventFactory.h"

std::map<std::string, unsigned int> EventFactory::TypeDict =
{
	{ "teleport", ET_Teleport },
	{ "dialogue", ET_DialogueBox },
	{ "move_right", ET_MoveRight },
	{ "move_down", ET_MoveDown },
	{ "move_up", ET_MoveUp },
	{ "move_left", ET_MoveLeft },
	{ "call_queue", ET_CallQueue },
	{ "map_change", ET_MapChange }
};

std::map<std::string, unsigned int> EventFactory::EEMDict =
{
	{ "blocking", BLOCKING },
	{ "async", ASYNC }
};

std::shared_ptr<IEvent> EventFactory::BuildEvent(EventTypes et, std::map<std::string, EventArgType> args, unsigned int entity_id)
{
	//if (s.size() <= 0)
	//	return NULL;

	std::shared_ptr<IEvent> result = NULL;

	// Target id setup
	unsigned int id = args.count("id") ? std::get<int>(args.at("id")) : 0;
	if (id == 0) id = entity_id;

	switch (et)
	{
	case EventTypes::ET_MapChange:
		result = std::shared_ptr<IEvent>(new EventMapChange(id));
		break;
	case EventTypes::ET_Teleport:
		result = std::shared_ptr<IEvent>(new EventTeleport(id,
			args.count("x") ? std::get<float>(args.at("x")) : 0.0f,
			args.count("y") ? std::get<float>(args.at("y")) : 0.0f,
			args.count("z") ? std::get<float>(args.at("z")) : 0.0f));
		break;
	case EventTypes::ET_MoveRight:
		result = std::shared_ptr<IEvent>(new EventMove(id,
			args.count("distance") ? std::get<float>(args.at("distance")) : 3.0f,
			1));
		break;
	case EventTypes::ET_MoveUp:
		result = std::shared_ptr<IEvent>(new EventMove(id,
			args.count("distance") ? std::get<float>(args.at("distance")) : 3.0f,
			0));
		break;
	case EventTypes::ET_MoveDown:
		result = std::shared_ptr<IEvent>(new EventMove(id,
			args.count("distance") ? std::get<float>(args.at("distance")) : 3.0f,
			2));
		break;
	case EventTypes::ET_MoveLeft:
		result = std::shared_ptr<IEvent>(new EventMove(id,
			args.count("distance") ? std::get<float>(args.at("distance")) : 3.0f,
			3));
		break;
	case EventTypes::ET_CallQueue:
		result = std::shared_ptr<IEvent>(new EventCaller(id, args.count("queue_id") ? std::get<int>(args.at("queue_id")) : 0));
		break;
	case EventTypes::ET_DialogueBox:
	{
		std::vector<DialogueChoice> choices;
		std::vector<Dialogue> dialogues;

		// Add the choices and dialogues to the vectors
		for (auto x : args)
		{
			std::vector<std::string> vs = Utils::Split(x.first, ' ');
			for (auto s : vs)
			{
				if (s == "choice")
				{
					DialogueChoice dc;

					// For each param in the choice
					for (auto t : std::get<std::map<std::string, std::variant<bool, float, int, std::string, std::vector<std::shared_ptr<EventQueue>>>>>(x.second))
					{
						// Dialogue id
						if (t.first == "d")
							dc.DialogueId = std::get<int>(t.second);
						else if (t.first == "text")
							dc.Text = std::get<std::string>(t.second);
						else if (t.first == "next")
							dc.NextTextId = std::get<int>(t.second);
						else if (t.first == "queues")
							dc.Queue = (std::get<std::vector<std::shared_ptr<EventQueue>>>(t.second)).at(0);
					}

					choices.push_back(dc);
				}
				else if (s == "dialogue")
				{
					Dialogue d;

					// For each param in the dialogue
					for (auto t : (std::get<std::map<std::string, std::variant<bool, float, int, std::string, std::vector<std::shared_ptr<EventQueue>>>>>(x.second)))
					{
						// Dialogue id
						if (t.first == "id")
							d.Id = std::get<int>(t.second);
						else if (t.first == "text")
							d.Text = std::get<std::string>(t.second);
						else if (t.first == "next")
							d.NextTextId = std::get<int>(t.second);
						else if (t.first == "queues")
							d.Queue = (std::get<std::vector<std::shared_ptr<EventQueue>>>(t.second)).at(0);
						else if (t.first == "type")
							d.Type = DialogueGraph::StringToDialogueType(std::get<std::string>(t.second));
					}

					dialogues.push_back(d);
				}
			}
		}

		result = std::shared_ptr<IEvent>(new DialogueBox(entity_id, dialogues, choices));
		break;
	}
	default:
		break;
	}

	return result;
}

void EventFactory::SetActivationType(std::shared_ptr<EventQueue> eq, std::string s)
{
	switch (tolower(s[0]))
	{
	case 'a':
		eq->SetActivationType(AT_Autorun);
		break;
	case 't':
		eq->SetActivationType(AT_Touch);
		break;
	case 'i':
		eq->SetActivationType(AT_Interact);
		break;
	}
}

std::vector<std::shared_ptr<EventQueue>> EventFactory::LoadEvent(int map_id, unsigned int entity_id, std::shared_ptr<JsonHandler> jh)
{
	std::vector<std::shared_ptr<EventQueue>> result = std::vector<std::shared_ptr<EventQueue>>();

	auto& ques = jh->LoadQueues(map_id, entity_id).GetArray();
	for (auto& x : ques)
	{
		//Make sure that the event is flagged as valid
		if (!x.HasMember("flag") || x["flag"].GetInt() == 1)
		{
			int id = -1;
			if (x.HasMember("id"))
				id = x["id"].GetInt();
			std::shared_ptr<EventQueue> queue = std::shared_ptr<EventQueue>(new EventQueue(id));

			// Set repeating if necessary
			if (x.HasMember("repeating") && x["repeating"].GetBool() == true)
				queue->SetRepeating(true);

			// Find activation type
			if (x.HasMember("activation") && x["activation"].GetType() == rapidjson::Type::kStringType)
				SetActivationType(queue, std::string(x["activation"].GetString()));

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
						for (rapidjson::Value::MemberIterator iter = e["args"].MemberBegin(); iter != e["args"].MemberEnd(); ++iter)
						{
							EventArgType eat = AddArg(iter, false);
							args.emplace(std::string(iter->name.GetString()), eat);
						}

					// If the event has a queue in it
					if (e.HasMember("queues"))
					{
						EventArgType eat;
						rapidjson::Value val = e["queues"].GetArray();
						args.emplace("queue", LoadEvent(val));
					}

					std::shared_ptr<IEvent> ev = EventFactory::BuildEvent((EventTypes)TypeDict.at(e["type"].GetString()), args, entity_id);

					// Set execution mode if necessary
					if (e.HasMember("execution_type") && EEMDict.find(e["execution_type"].GetString()) != EEMDict.end())
						ev->SetExecutionMode((EventExecutionMode)EEMDict.at(e["execution_type"].GetString()));


					queue->PushBack(ev);
				}
			}
			if (queue->Count() > 0)
				result.push_back(queue);
		}
	}
	return result;
}

std::shared_ptr<EventQueue> EventFactory::LoadEvent(int map_id, unsigned int entity_id, unsigned int queue_id, std::shared_ptr<JsonHandler> jh)
{
	std::shared_ptr<EventQueue> result = std::shared_ptr<EventQueue>(new EventQueue(-1));

	auto& ques = jh->LoadQueues(map_id, entity_id).GetArray();
	for (auto& x : ques)
	{
		//Make sure that the event is flagged as valid
		if (!x.HasMember("flag") || x["flag"].GetInt() == 1)
		{
			int id = -1;
			if (x.HasMember("id"))
				id = x["id"].GetInt();

			if (id != queue_id)
				continue;

			result = std::shared_ptr<EventQueue>(new EventQueue(id));

			// Set repeating if necessary
			if (x.HasMember("repeating") && x["repeating"].GetBool() == true)
				result->SetRepeating(true);

			// Find activation type
			if (x.HasMember("activation") && x["activation"].GetType() == rapidjson::Type::kStringType)
				SetActivationType(result, std::string(x["activation"].GetString()));

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
						for (rapidjson::Value::MemberIterator iter = e["args"].MemberBegin(); iter != e["args"].MemberEnd(); ++iter)
						{
							EventArgType eat = AddArg(iter, false);
							args.emplace(std::string(iter->name.GetString()), eat);
						}

					// If the event has a queue in it
					if (e.HasMember("queues"))
					{
						EventArgType eat;
						rapidjson::Value val = e["queues"].GetArray();
						args.emplace("queue", LoadEvent(val));
					}

					std::shared_ptr<IEvent> ev = EventFactory::BuildEvent((EventTypes)TypeDict.at(e["type"].GetString()), args, entity_id);

					// Set execution mode if necessary
					if (e.HasMember("execution_type") && EEMDict.find(e["execution_type"].GetString()) != EEMDict.end())
						ev->SetExecutionMode((EventExecutionMode)EEMDict.at(e["execution_type"].GetString()));


					result->PushBack(ev);
				}
			}
			if (!result->Count())
				result = std::shared_ptr<EventQueue>(new EventQueue(-1));
			return result;
		}
	}
	return result;
}

EventArgType EventFactory::AddArg(rapidjson::Value::MemberIterator iter, bool secondIteration)
{
	std::map<std::string, std::variant<bool, float, int, std::string, std::vector<std::shared_ptr<EventQueue>>>> map = std::map<std::string, std::variant<bool, float, int, std::string, std::vector<std::shared_ptr<EventQueue>>>>();
	EventArgType eat = false;
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
		eat = std::string(iter->value.GetString());
		break;
	case rapidjson::Type::kTrueType:
		eat = iter->value.GetBool();
		break;
	case rapidjson::Type::kFalseType:
		eat = iter->value.GetBool();
		break;
	case rapidjson::Type::kObjectType:
		if (secondIteration)
			break;
		for (rapidjson::Value::MemberIterator it = iter->value.MemberBegin(); it != iter->value.MemberEnd(); ++it)
			map.emplace(std::string(it->name.GetString()), AddArg(it));
		eat = map;
		break;
	case rapidjson::Type::kArrayType:
		rapidjson::Value val = iter->value.GetArray();
		eat = LoadEvent(val);
		break;
	}

	return eat;
}

std::variant<bool, float, int, std::string, std::vector<std::shared_ptr<EventQueue>>> EventFactory::AddArg(rapidjson::Value::MemberIterator iter)
{
	std::map<std::string, std::variant<bool, float, int, std::string, std::vector<std::shared_ptr<EventQueue>>>> map = std::map<std::string, std::variant<bool, float, int, std::string, std::vector<std::shared_ptr<EventQueue>>>>();
	std::variant<bool, float, int, std::string, std::vector<std::shared_ptr<EventQueue>>> eat = false;
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
		eat = std::string(iter->value.GetString());
		break;
	case rapidjson::Type::kTrueType:
		eat = iter->value.GetBool();
		break;
	case rapidjson::Type::kFalseType:
		eat = iter->value.GetBool();
		break;
	case rapidjson::Type::kArrayType:
		rapidjson::Value val = iter->value.GetArray();
		eat = LoadEvent(val);
		break;
	}

	return eat;
}


// Recursive AS FUCK
std::vector<std::shared_ptr<EventQueue>> EventFactory::LoadEvent(rapidjson::Value& v)
{
	std::vector<std::shared_ptr<EventQueue>> result = std::vector<std::shared_ptr<EventQueue>>();

	for (auto& p : v.GetArray())
	{
		std::shared_ptr<EventQueue> queue = std::shared_ptr<EventQueue>(new EventQueue());
		// Set repeating if necessary
		if (p.HasMember("repeating") && p["repeating"].GetBool() == true)
			queue->SetRepeating(true);

		// Find activation type
		if (p.HasMember("activation"))
			SetActivationType(queue, p.GetString());

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

				std::shared_ptr<IEvent> ev = EventFactory::BuildEvent((EventTypes)TypeDict.at(e["type"].GetString()), args);

				// Set execution mode if necessary
				if (e.HasMember("execution_type") && EEMDict.find(e["execution_type"].GetString()) != EEMDict.end())
					ev->SetExecutionMode((EventExecutionMode)EEMDict.at(e["execution_type"].GetString()));

				queue->PushBack(ev);
			}
		}
		if (queue->Count() > 0)
			result.push_back(queue);
	}
	return result;
}



void EventFactory::FlagEvent(int map_id, unsigned int entity_id, unsigned int queue_id, int flag, std::string DATA_FILE)
{
	// Check out all the queues
	//const auto& ques = JsonHandler::LoadQueues(map_id, entity_id).GetArray();
	//for (auto& x : ques)
	//{
	//	if (x.HasMember("id") && x["id"].GetInt() == queue_id)
	//	{
	//		//Make sure that the event has a flag property
	//		if (x.HasMember("flag"))
	//		{
	//			x["flag"].SetInt(flag);
	//		}
	//	}
	//}
}

// Defined here to avoid circular dependancies
EventUpdateResponse EventCaller::UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents)
{
	EventUpdateResponse eur = EventUpdateResponse();
	eur.IsDone = false;

	if (m_firstTime)
	{
		// Possible that this steals the instance of and reks other events - be wary
		for (auto x : *ents->at(m_target)->GetQueues())
			if (x->GetID() == m_targetQueue)
				m_queue = x;
		/* EventFactory::LoadEvent(-1, m_target, m_targetQueue);*/
		if (m_queue->GetID() == -1)
		{
			m_completed = true;
			return eur;
		}

		eur.Queue = m_queue;
		m_firstTime = false;
	}
	else
	{
		if (m_queue->IsDone())
		{
			m_completed = true;
			eur.IsDone = true;
		}
	}

	return eur;
}
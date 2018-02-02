#include "eventFactory.h"
#include "eventBattle.h"
#include "map_handler.h"
#include "eventParticle.h"
#include "eventBGM.h"
#include "eventSetFlag.h"
#include "eventSpriteChange.h"
#include "eventAddToFlag.h"
#include "eventToggleFlag.h"

int EventFactory::m_entity_id = 0;

std::map<std::string, unsigned int> EventFactory::TypeDict =
{
	{ "teleport", ET_Teleport },
	{ "dialogue", ET_DialogueBox },
	{ "move_right", ET_MoveRight },
	{ "move_down", ET_MoveDown },
	{ "move_up", ET_MoveUp },
	{ "move_left", ET_MoveLeft },
	{ "call_queue", ET_CallQueue },
	{ "map_change", ET_MapChange },
	{ "weather", ET_Weather },
	{ "particle", ET_Particle },
	{ "play_sound", ET_PlaySound },
	{ "play_bgm", ET_PlayBGM },
	{ "set_flag", ET_SetFlag },
	{ "toggle_flag", ET_ToggleFlag },
	{ "add_to_flag", ET_AddToFlag },
	{ "sprite_change", ET_SpriteChange },
	{ "battle", ET_Battle },
};

std::map<std::string, unsigned int> EventFactory::EEMDict =
{
	{ "blocking", BLOCKING },
	{ "async", ASYNC }
};

float EventArgType::getFloat()
{
	if (std::holds_alternative<float>(inner))
		return std::get<float>(inner);
	if (std::holds_alternative<int>(inner))
		return (float)std::get<int>(inner);

	// If we get here then its not even an int so wtf -_-
	return std::get<float>(inner);
}

EventArgType::EventArgType() : inner(true) {}
EventArgType::EventArgType(EventArgInner eai) : inner(eai)
{
}


std::shared_ptr<IEvent> EventFactory::BuildEvent(EventTypes et, std::map<std::string, EventArgType> args, MapHandler* map, unsigned int entity_id)
{
	//if (s.size() <= 0)
	//	return NULL;

	std::shared_ptr<IEvent> result = NULL;

	// Target id setup
	unsigned int id = args.count("id") ? args.at("id").get<int>() : 0;
	if (id == 0) id = entity_id;

	std::string str;
	Vector3f vec;
	ParticleType type;

	switch (et)
	{
	case EventTypes::ET_Battle:
		result = std::shared_ptr<IEvent>(new EventBattle());
		break;
	case EventTypes::ET_MapChange:
		result = std::shared_ptr<IEvent>(new EventMapChange(id));
		break;
	case EventTypes::ET_Teleport:
		result = std::shared_ptr<IEvent>(new EventTeleport(id,
			args.count("x") ? args.at("x").getFloat() : 0.0f,
			args.count("y") ? args.at("y").getFloat() : 0.0f,
			args.count("z") ? args.at("z").getFloat() : 0.0f));
		break;
	case EventTypes::ET_SetFlag:
		result = std::shared_ptr<IEvent>(new EventSetFlag(
			args.count("name") ? args.at("name").get<std::string>() : "",
			args.count("value") ? args.at("value").get<int>() : 1));
		break;
	case EventTypes::ET_ToggleFlag:
		result = std::shared_ptr<IEvent>(new EventToggleFlag(
			args.count("name") ? args.at("name").get<std::string>() : ""));
		break;
	case EventTypes::ET_AddToFlag:
		result = std::shared_ptr<IEvent>(new EventAddToFlag(
			args.count("name") ? args.at("name").get<std::string>() : "",
			args.count("value") ? args.at("value").get<int>() : 1));
		break;
	case EventTypes::ET_SpriteChange:
		result = std::shared_ptr<IEvent>(new EventSpriteChange(
			args.count("sprite") ? args.at("sprite").get<std::string>() : "",
			args.count("id") ? args.at("id").get<int>() : 1));
		break;
	case EventTypes::ET_PlaySound:
		result = std::shared_ptr<IEvent>(new EventSound(
			args.count("sound_file") ? args.at("sound_file").get<std::string>() : "res/audio/fx/swish_2.wav"));
		break;
	case EventTypes::ET_PlayBGM:
		result = std::shared_ptr<IEvent>(new EventBGM(
			args.count("sound_file") ? args.at("sound_file").get<std::string>() : "res/audio/fx/washeslow.wav"));
		break;
	case EventTypes::ET_MoveRight:
		result = std::shared_ptr<IEvent>(new EventMove(id,
			args.count("distance") ? args.at("distance").getFloat() : 3.0f,
			1));
		break;
	case EventTypes::ET_MoveUp:
		result = std::shared_ptr<IEvent>(new EventMove(id,
			args.count("distance") ? args.at("distance").getFloat() : 3.0f,
			0));
		break;
	case EventTypes::ET_MoveDown:
		result = std::shared_ptr<IEvent>(new EventMove(id,
			args.count("distance") ? args.at("distance").getFloat() : 3.0f,
			2));
		break;
	case EventTypes::ET_MoveLeft:
		result = std::shared_ptr<IEvent>(new EventMove(id,
			args.count("distance") ? args.at("distance").getFloat() : 3.0f,
			3));
		break;
	case EventTypes::ET_Weather:
		str = args.count("type") ? args.at("type").get<std::string>() : "snow";
		type = ParticleFromString.count(str) ? ParticleFromString.at(str) : PT_Snow;
		result = std::shared_ptr<IEvent>(new EventWeather(args.count("count") ? args.at("count").get<int>() : 50,
			ParticleFromString.count(str) ? ParticleFromString.at(str) : PT_Snow,
			map->GetMapSize(),
			args.count("smooth") ? args.at("smooth").get<bool>() : false,
			args.count("sprite") ? args.at("sprite").get<std::string>() : "snowflake.png"));
		break;
	case EventTypes::ET_Particle:
		str = args.count("type") ? args.at("type").get<std::string>() : "music";
		result = std::shared_ptr<IEvent>(new EventParticle(args.count("count") ? args.at("count").get<int>() : 50,
			ParticleFromString.count(str) ? ParticleFromString.at(str) : PT_Music,
			id,
			args.count("sprite") ? args.at("sprite").get<std::string>() : ""));
		if (args.count("power"))
			dynamic_cast<EventParticle*>(result.get())->SetPower(args.at("power").getFloat());
		break;
	case EventTypes::ET_CallQueue:
		result = std::shared_ptr<IEvent>(new EventCaller(id, args.count("queue_id") ? args.at("queue_id").get<int>() : 0));
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
					for (auto t : x.second.get<std::map<std::string, EventArgType>>())
					{
						// Dialogue id
						if (t.first == "d")
							dc.DialogueId = t.second.get<int>();
						else if (t.first == "text")
							dc.Text = t.second.get<std::string>();
						else if (t.first == "next")
							dc.NextTextId = t.second.get<int>();
						else if (t.first == "queues")
							dc.Queue = t.second.get<std::vector<std::shared_ptr<EventQueue>>>().at(0);
					}

					choices.push_back(dc);
				}
				else if (s == "dialogue")
				{
					Dialogue d;

					// For each param in the dialogue
					for (auto t : x.second.get<std::map<std::string, EventArgType>>())
					{
						// Dialogue id
						if (t.first == "id")
							d.Id = t.second.get<int>();
						else if (t.first == "text")
							d.Text = t.second.get<std::string>();
						else if (t.first == "next")
							d.NextTextId = t.second.get<int>();
						else if (t.first == "queues")
							d.Queue = t.second.get<std::vector<std::shared_ptr<EventQueue>>>().at(0);
						else if (t.first == "type")
							d.Type = DialogueGraph::StringToDialogueType(t.second.get<std::string>());
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

std::vector<std::shared_ptr<EventQueue>> EventFactory::LoadEvent(int map_id, unsigned int entity_id, std::shared_ptr<JsonHandler> jh, MapHandler* map)
{
	std::vector<std::shared_ptr<EventQueue>> result = std::vector<std::shared_ptr<EventQueue>>();

	m_entity_id = entity_id;

	auto ques = jh->LoadQueues(map_id, entity_id).GetArray();
	for (auto& x : ques)
	{
		// Get flag name
		std::string flag = "";
		if (x.HasMember("flag") && x["flag"].IsString())
			flag = x["flag"].GetString();

		// Get flag expected value
		int flag_value = 1;
		if (x.HasMember("flag_value") && x["flag_value"].IsInt())
			flag_value = x["flag_value"].GetInt();

		// Get flag condition
		FlagCondition flag_condition = FC_Value;
		if (x.HasMember("flag_condition") && x["flag_condition"].IsInt())
			flag_condition = (FlagCondition)x["flag_condition"].GetInt();

		// Get queue id
		int id = -1;
		if (x.HasMember("id"))
			id = x["id"].GetInt();

		// Create queue object
		std::shared_ptr<EventQueue> queue = std::shared_ptr<EventQueue>(new EventQueue(id));

		// Set attributes
		queue->Flag = flag;
		queue->FlagValue = flag_value;
		queue->Condition = flag_condition;

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
						EventArgType eat = AddArg(iter, map);
						args.emplace(std::string(iter->name.GetString()), eat);
					}

				// If the event has a queue in it
				if (e.HasMember("queues"))
				{
					EventArgType eat;
					rapidjson::Value val = e["queues"].GetArray();
					args.emplace("queue", LoadEvent(val, map));
				}

				std::shared_ptr<IEvent> ev = EventFactory::BuildEvent((EventTypes)TypeDict.at(e["type"].GetString()), args, map, entity_id);

				// Set execution mode if necessary
				if (e.HasMember("execution_type") && EEMDict.find(e["execution_type"].GetString()) != EEMDict.end())
					ev->SetExecutionMode((EventExecutionMode)EEMDict.at(e["execution_type"].GetString()));


				queue->PushBack(ev);
			}
		}
		if (queue->Count() > 0)
			result.push_back(queue);
		//}
	}
	return result;
}

std::shared_ptr<EventQueue> EventFactory::LoadEvent(int map_id, unsigned int entity_id, unsigned int queue_id, std::shared_ptr<JsonHandler> jh, MapHandler* map)
{
	std::shared_ptr<EventQueue> result = std::shared_ptr<EventQueue>(new EventQueue(-1));

	auto ques = jh->LoadQueues(map_id, entity_id).GetArray();
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
							EventArgType eat = AddArg(iter, map);
							args.emplace(std::string(iter->name.GetString()), eat);
						}

					// If the event has a queue in it
					if (e.HasMember("queues"))
					{
						EventArgType eat;
						rapidjson::Value val = e["queues"].GetArray();
						args.emplace("queue", LoadEvent(val, map));
					}

					std::shared_ptr<IEvent> ev = EventFactory::BuildEvent((EventTypes)TypeDict.at(e["type"].GetString()), args, map, entity_id);

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

EventArgType EventFactory::AddArg(rapidjson::Value::MemberIterator iter, MapHandler* maphandler)
{
	std::map<std::string, EventArgType> map = std::map<std::string, EventArgType>();
	EventArgType eat(false);
	rapidjson::Type t;
	t = iter->value.GetType();
	switch (t)
	{
	case rapidjson::Type::kNumberType:
		if (iter->value.IsFloat())
			eat.inner = iter->value.GetFloat();
		if (iter->value.IsInt())
			eat.inner = iter->value.GetInt();
		break;
	case rapidjson::Type::kStringType:
		eat.inner = std::string(iter->value.GetString());
		break;
	case rapidjson::Type::kTrueType:
		eat.inner = iter->value.GetBool();
		break;
	case rapidjson::Type::kFalseType:
		eat.inner = iter->value.GetBool();
		break;
	case rapidjson::Type::kObjectType:
		for (rapidjson::Value::MemberIterator it = iter->value.MemberBegin(); it != iter->value.MemberEnd(); ++it)
			map.emplace(std::string(it->name.GetString()), AddArg(it, maphandler));
		eat.inner = map;
		break;
	case rapidjson::Type::kArrayType:
		rapidjson::Value val = iter->value.GetArray();
		eat.inner = LoadEvent(val, maphandler);
		break;
	}

	return eat;
}


// Recursive AS FUCK
std::vector<std::shared_ptr<EventQueue>> EventFactory::LoadEvent(rapidjson::Value& v, MapHandler* map)
{
	std::vector<std::shared_ptr<EventQueue>> result = std::vector<std::shared_ptr<EventQueue>>();

	for (auto& p : v.GetArray())
	{
		int id = -1;
		if (p.HasMember("id"))
			id = p["id"].GetInt();

		std::shared_ptr<EventQueue> queue = std::shared_ptr<EventQueue>(new EventQueue(id));

		// Set repeating if necessary
		if (p.HasMember("repeating") && p["repeating"].GetBool() == true)
			queue->SetRepeating(true);

		// Find activation type
		if (p.HasMember("activation"))
			SetActivationType(queue, p["activation"].GetString());

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
					for (rapidjson::Value::MemberIterator iter = e["args"].MemberBegin(); iter != e["args"].MemberEnd(); ++iter)
					{
						EventArgType eat = AddArg(iter, map);
						args.emplace(std::string(iter->name.GetString()), eat);
					}


				// If the event has a queue in it
				if (e.HasMember("queues"))
				{
					EventArgType eat;
					rapidjson::Value val = e["queues"].GetArray();
					args.emplace("queue", LoadEvent(val, map));
				}

				std::shared_ptr<IEvent> ev = EventFactory::BuildEvent((EventTypes)TypeDict.at(e["type"].GetString()), args, map, m_entity_id);

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

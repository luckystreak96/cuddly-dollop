#include "eventFactory.h"
#include "eventBattle.h"
#include "map_handler.h"
#include "eventParticle.h"
#include "eventBGM.h"
#include "eventSetFlag.h"
#include "eventSpriteChange.h"
#include "eventAddToFlag.h"
#include "eventToggleFlag.h"

#include <variant>

int EventFactory::m_entity_id = 0;

std::map<std::string, EventTypeInfo> EventFactory::TypeDict =
{
	{ "teleport", EventTypeInfo(ET_Teleport,
	       {{"id", BT_int}, {"x", BT_int},{"y", BT_int},{"z", BT_int},},
	       "Teleports an *entity_id* to the desired *x* *y* *z* location.") },

	{ "dialogue", EventTypeInfo(ET_DialogueBox,
			{{"[strings]", BT_string}},
			"Opens a series of text boxes with the specified text.") },

	{ "move_right", EventTypeInfo(ET_MoveRight,
			{{"id", BT_int}, {"distance", BT_int},},
			"Makes an entity move right until they've moved *amount* distance.") },

	{ "move_up", EventTypeInfo(ET_MoveUp,
			{{"id", BT_int}, {"distance", BT_int},},
			"Makes an entity move up until they've moved *amount* distance.") },

	{ "move_left", EventTypeInfo(ET_MoveLeft,
			{{"id", BT_int}, {"distance", BT_int},},
			"Makes an entity move left until they've moved *amount* distance.") },

	{ "move_down", EventTypeInfo(ET_MoveDown,
			{{"id", BT_int}, {"distance", BT_int},},
			"Makes an entity move down until they've moved *amount* distance.") },

	{ "call_queue", EventTypeInfo(ET_CallQueue,
			{{"id", BT_int}, {"queue_id", BT_int},},
			"Calls the queue with the specified *queue_id*. Id is necessary but useless?") },

	{ "map_change", EventTypeInfo(ET_MapChange,
			{{"id", BT_int},},
			"Changes to the map with the specified *id*.") },

	{ "weather", EventTypeInfo(ET_Weather,
			{{"type", BT_string},{"count", BT_int},{"smooth", BT_int},{"sprite", BT_string},},
			"Starts weather effect. Ex: weather \"snow\" 50 1 \"snowflake.png\"") },

	{ "particle", EventTypeInfo(ET_Particle,
			{{"type", BT_string},{"count", BT_int},{"sprite", BT_string},{"power", BT_int},},
			"Starts particle effect. Ex: particle \"music\" 10 \"snowflake.png\" 50") },

	{ "play_sound", EventTypeInfo(ET_PlaySound,
			{{"sound_file", BT_string}},
			"Plays sound effect. Ex: play_sound \"res/audio/fx/swish_2.wav\"") },

	{ "play_bgm", EventTypeInfo(ET_PlayBGM,
			{{"sound_file", BT_string}},
			"Plays bgm. Ex: play_bgm \"res/audio/bgm/washeslow.wav\"") },

	{ "set_flag", EventTypeInfo(ET_SetFlag,
			{{"name", BT_string}, {"value", BT_int}},
			"Sets flag named *name* to *value*.") },

	{ "toggle_flag", EventTypeInfo(ET_ToggleFlag,
			{{"name", BT_string},},
			"Toggles flag named *name*.") },

	{ "add_to_flag", EventTypeInfo(ET_AddToFlag,
			{{"name", BT_string}, {"value", BT_int}},
			"Adds *value* to flag named *name*.") },

	{ "sprite_change", EventTypeInfo(ET_SpriteChange,
			{{"id", BT_int}, {"sprite", BT_string}},
			"Sets entity of *id* to the sprite *sprite*.") },

	{ "battle", EventTypeInfo(ET_Battle,
			{},
			"No args. Starts the generic battle.") },
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

EventArgInner EventArgType::str_to_eai(std::string str) {
    BaseType bt = Utils::interpret_type(str);
    if(bt == BT_int)
    	return std::atoi(str.c_str());
	else if(bt == BT_float)
		return (float)std::atof(str.c_str());
	else
		return str;
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
			args.count("sound_file") ? args.at("sound_file").get<std::string>() : "res/audio/bgm/washeslow.wav"));
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
		std::map<std::string, std::shared_ptr<DialogueGraph>> graphs;

		// If the dialogue is only a simple sequence of strings,
		// Create it simply
		// Does not support choices for now
		if(std::holds_alternative<std::string>((*args.begin()).second.inner))
		{
			std::map<std::string, EventArgType> dialogues;
			int counter = 0;
			for(auto x : args)
			{
			    counter++;
				dialogues.emplace("dialogue " + std::to_string(counter), x.second);
			}

			return std::shared_ptr<IEvent>(new DialogueBox(entity_id, CreateDialogueGraph(dialogues)));
		}

		std::string first = (*args.begin()).first;

		// Ensure that languages are enabled
		std::vector<std::string> langCheck = Utils::Split(first, ' ');

		// If theres no languages, it just creates the one dialogue and returns it
		if (langCheck[0] == "dialogue" || langCheck[0] == "choice")
			return std::shared_ptr<IEvent>(new DialogueBox(entity_id, CreateDialogueGraph(args)));

		// Otherwise iterate through the localized dialogues
		for (auto langDialogue : args)
			graphs.emplace(langDialogue.first, CreateDialogueGraph(langDialogue.second.get<std::map<std::string, EventArgType>>()));

		return std::shared_ptr<IEvent>(new DialogueBox(entity_id, graphs));
	}
	default:
		break;
	}

	return result;
}

std::shared_ptr<DialogueGraph> EventFactory::CreateDialogueGraph(std::map<std::string, EventArgType> args)
{
	std::vector<DialogueChoice> choices;
	std::vector<Dialogue> dialogues;

	int linear_dialogue_id_counter = 0;

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

				// If the dialogue is linear, just add the stuff
				if(std::holds_alternative<std::string>(x.second.inner))
				{
					d.Id = linear_dialogue_id_counter++;
					d.Text = x.second.get<std::string>();
					if (linear_dialogue_id_counter >= args.size())
					{
						d.Type = DialogueType::End;
						d.NextTextId = -1;
					}
					else {
						d.NextTextId = linear_dialogue_id_counter;
						d.Type = DialogueType::Simple;
					}
				}
				else
				{
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
				}

				dialogues.push_back(d);
			}
		}
	}

	return std::shared_ptr<DialogueGraph>(new DialogueGraph(dialogues, choices));
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

				std::shared_ptr<IEvent> ev = EventFactory::BuildEvent((EventTypes)TypeDict.at(e["type"].GetString())._eventType, args, map, entity_id);

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

					std::shared_ptr<IEvent> ev = EventFactory::BuildEvent((EventTypes)TypeDict.at(e["type"].GetString())._eventType, args, map, entity_id);

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

				std::shared_ptr<IEvent> ev = EventFactory::BuildEvent((EventTypes)TypeDict.at(e["type"].GetString())._eventType, args, map, m_entity_id);

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

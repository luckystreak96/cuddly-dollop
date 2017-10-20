#ifndef EVENT_FACTORY_H__
#define EVENT_FACTORY_H__

#include <string>
#include <vector>
#include <variant>
#include <map>
#include "iEvent.h"
#include "dialogueBox.h"
#include "eventMove.h"
#include "eventQueue.h"
#include "utils.h"

typedef std::variant<bool, float, int, std::string, std::vector<EventQueue>, std::map<std::string, std::variant<bool, float, int, std::string, std::vector<EventQueue>>>> EventArgType;

enum EventTypes { ET_Teleport, ET_DialogueBox, ET_MoveRight, ET_MoveUp, ET_MoveDown, ET_MoveLeft };

class EventFactory
{
public:
	static IEvent* BuildEvent(EventTypes et, std::map<std::string, EventArgType> args, unsigned int entity_id = 0);
	static std::map<std::string, unsigned int> TypeDict;
	static std::map<std::string, unsigned int> EEMDict;
	static std::vector<EventQueue> LoadEvent(int map_id, unsigned int entity_id);
	static std::vector<EventQueue> LoadEvent(rapidjson::Value& v);
	static void FlagEvent(int map_id, unsigned int entity_id, unsigned int queue_id, int flag, std::string DATA_FILE = "res/data/data.json");
	static EventArgType AddArg(rapidjson::Value::MemberIterator iter, bool secondIteration);
	static std::variant<bool, float, int, std::string, std::vector<EventQueue>> AddArg(rapidjson::Value::MemberIterator iter);
};

#endif // !DIALOGUE_GRAPH_H__


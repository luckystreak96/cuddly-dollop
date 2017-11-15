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
#include "eventCaller.h"
#include "eventMapChange.h"
#include "eventTeleport.h"

typedef std::variant<bool, float, int, std::string, std::vector<std::shared_ptr<EventQueue>>, std::map<std::string, std::variant<bool, float, int, std::string, std::vector<std::shared_ptr<EventQueue>>>>> EventArgType;

enum EventTypes { ET_Teleport, ET_DialogueBox, ET_MoveRight, ET_MoveUp, ET_MoveDown, ET_MoveLeft, ET_CallQueue, ET_MapChange};

class EventFactory
{
public:
	static std::shared_ptr<IEvent> BuildEvent(EventTypes et, std::map<std::string, EventArgType> args, unsigned int entity_id = 0);
	static std::map<std::string, unsigned int> TypeDict;
	static std::map<std::string, unsigned int> EEMDict;
	static std::vector<std::shared_ptr<EventQueue>> LoadEvent(int map_id, unsigned int entity_id, std::shared_ptr<JsonHandler> jh);
	static std::shared_ptr<EventQueue> LoadEvent(int map_id, unsigned int entity_id, unsigned int queue_id, std::shared_ptr<JsonHandler> jh);
	static std::vector<std::shared_ptr<EventQueue>> LoadEvent(rapidjson::Value& v);
	static void FlagEvent(int map_id, unsigned int entity_id, unsigned int queue_id, int flag, std::string DATA_FILE = "res/data/data.json");
	static EventArgType AddArg(rapidjson::Value::MemberIterator iter, bool secondIteration);
	static std::variant<bool, float, int, std::string, std::vector<std::shared_ptr<EventQueue>>> AddArg(rapidjson::Value::MemberIterator iter);
	static void SetActivationType(std::shared_ptr<EventQueue> eq, std::string s);
	static float GetFloat(EventArgType eat);
};

#endif // !DIALOGUE_GRAPH_H__


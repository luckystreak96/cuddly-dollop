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
#include "eventWeather.h"
#include "eventSound.h"

class EventTest;
using EventTestInner = std::variant<bool, std::map<std::string, EventTest>>;
class EventTest
{
public:
	EventTestInner inner;
	template<typename T>
	T get();
};

template<typename T>
T EventTest::get()
{
	return std::get<T>(inner);
}

typedef std::variant<bool, float, int, std::string, std::vector<std::shared_ptr<EventQueue>>, 
	std::map<std::string, std::variant<bool, float, int, std::string, std::vector<std::shared_ptr<EventQueue>>>>> EventArgType;

class MapHandler;

class EventFactory
{
public:
	static std::shared_ptr<IEvent> BuildEvent(EventTypes et, std::map<std::string, EventArgType> args, MapHandler* map, unsigned int entity_id = 0);
	static std::map<std::string, unsigned int> TypeDict;
	static std::map<std::string, unsigned int> EEMDict;
	static std::vector<std::shared_ptr<EventQueue>> LoadEvent(int map_id, unsigned int entity_id, std::shared_ptr<JsonHandler> jh, MapHandler* map);
	static std::shared_ptr<EventQueue> LoadEvent(int map_id, unsigned int entity_id, unsigned int queue_id, std::shared_ptr<JsonHandler> jh, MapHandler* map);
	static std::vector<std::shared_ptr<EventQueue>> LoadEvent(rapidjson::Value& v, MapHandler* map);
	static void FlagEvent(int map_id, unsigned int entity_id, unsigned int queue_id, int flag, std::string DATA_FILE = "res/data/data.json");
	static EventArgType AddArg(rapidjson::Value::MemberIterator iter, bool secondIteration, MapHandler* map);
	static std::variant<bool, float, int, std::string, std::vector<std::shared_ptr<EventQueue>>> AddArg(rapidjson::Value::MemberIterator iter, MapHandler* maphandler);
	static void SetActivationType(std::shared_ptr<EventQueue> eq, std::string s);
	static float GetFloat(EventArgType eat);
};

#endif // !DIALOGUE_GRAPH_H__


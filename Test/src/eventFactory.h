#ifndef EVENT_FACTORY_H__
#define EVENT_FACTORY_H__

#include <string>
#include <vector>
#include <map>
#include "iEvent.h"
#include "dialogueBox.h"
#include "eventMove.h"
#include "eventQueue.h"

enum EventTypes { ET_Teleport, ET_DialogueBox, ET_MoveRight, ET_MoveUp, ET_MoveDown };

class EventFactory
{
public:
	static IEvent* BuildEvent(EventTypes et, std::vector<std::string> obj);
	static std::map<std::string, unsigned int> TypeDict;
	static std::map<std::string, unsigned int> EEMDict;
	static std::vector<EventQueue> LoadEvent(unsigned int entity_id, std::string dataFile = "res/data/data.json");
	static void FlagEvent(unsigned int entity_id, unsigned int queue_id, int flag, std::string DATA_FILE = "res/data/data.json");
};

#endif // !DIALOGUE_GRAPH_H__


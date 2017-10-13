#ifndef EVENT_MANAGER_H__
#define EVENT_MANAGER_H__

#include <deque>
#include <map>
#include <algorithm>
#include "iEvent.h"
#include "input_manager.h"
#include "entity.h"
#include "eventQueue.h"
#include "eventFactory.h"

class EventManager
{
public:
	EventManager();
	void PushBack(EventQueue ev);
	void Update(double elapsedTime);
	void SetEntitiesMap(std::map<unsigned int, Entity*>* ents);
	void SetFlag(unsigned int eventID);
protected:
	void Erase(unsigned int index, unsigned int queueIndex);
	void UpdateLockLevel();
	void AddToLockVector(IEvent* ev);
protected:
	std::vector<EventQueue> m_queues;
	static std::vector<IEvent*> m_locks;
	std::map<unsigned int, Entity*>* m_entities;
};

#endif // !DIALOGUE_GRAPH_H__


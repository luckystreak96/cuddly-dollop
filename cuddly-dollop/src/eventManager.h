#ifndef EVENT_MANAGER_H__
#define EVENT_MANAGER_H__

#include <deque>
#include <map>
#include <algorithm>
#include "iEvent.h"
#include "input_manager.h"
#include "entity.h"
#include "eventQueue.h"

class EventManager
{
public:
	EventManager();
	~EventManager();
	void PushBack(std::shared_ptr<EventQueue> ev);
	void force_push_front(std::shared_ptr<EventQueue> ev);
	void Update(double elapsedTime);
	void SetEntitiesMap(std::map<unsigned int, std::shared_ptr<Entity>>* ents);
	void SetFlag(unsigned int eventID);
protected:
	bool IsAllDone(unsigned int index);
	void Erase(unsigned int queueIndex);
	void RemoveLock(std::shared_ptr<IEvent> ev);
	void UpdateLockLevel();
	void AddToLockVector(std::shared_ptr<IEvent> ev);
protected:
	std::vector<std::shared_ptr<EventQueue>> m_queues;
	static std::vector<std::shared_ptr<IEvent>> m_locks;
	std::map<unsigned int, std::shared_ptr<Entity>>* m_entities;
};

#endif // !DIALOGUE_GRAPH_H__


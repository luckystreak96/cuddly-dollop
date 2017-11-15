#ifndef IEVENT_H__
#define IEVENT_H__

#include <map>
#include "eventQueue.h"
#include <rapidjson\document.h>
#include <memory>

class Entity;

struct EventUpdateResponse
{
	bool IsDone;
	std::shared_ptr<EventQueue> Queue;
};

enum EventExecutionMode
{
	BLOCKING, ASYNC
};

class IEvent
{
public:
	IEvent();
	virtual ~IEvent() = default;

	// Returns true if the event is done updating
	virtual EventUpdateResponse UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents);
	virtual void ResetEvent();
	unsigned int GetLockLevel();
	bool IsCompleted();
	EventExecutionMode GetExecutionMode();
	void SetExecutionMode(EventExecutionMode eem);
protected:
	// Stops from updating if its done but not destroyed
	//std::map<std::string, rapidjson::Value> m_args;
	bool m_completed;
	EventExecutionMode m_mode;
	unsigned int m_lockLevel;
	unsigned int m_target;
};

#endif // !DIALOGUE_GRAPH_H__


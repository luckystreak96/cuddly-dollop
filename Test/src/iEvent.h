#ifndef IEVENT_H__
#define IEVENT_H__

#include <map>
#include "eventQueue.h"

class Entity;

struct EventUpdateResponse
{
	bool IsDone;
	EventQueue Queue;
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
	virtual EventUpdateResponse UpdateEvent(double elapsedTime, std::map<unsigned int, Entity*>* ents);
	virtual void ResetEvent();
	unsigned int GetLockLevel();
	EventExecutionMode GetExecutionMode();
	void SetExecutionMode(EventExecutionMode eem);
protected:
	// Stops from updating if its done but not destroyed
	bool m_completed;
	EventExecutionMode m_mode;
	unsigned int m_lockLevel;
};

#endif // !DIALOGUE_GRAPH_H__


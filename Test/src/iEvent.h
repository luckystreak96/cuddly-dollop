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

enum EventTypes { ET_Teleport, ET_DialogueBox, ET_MoveRight, ET_MoveUp, ET_MoveDown, ET_MoveLeft, 
	ET_CallQueue, ET_MapChange, ET_Weather, ET_Particle, ET_PlaySound, ET_PlayBGM, ET_SetFlag, ET_ToggleFlag, ET_AddToFlag,
	ET_SpriteChange};

class IEvent
{
public:
	IEvent();
	virtual ~IEvent() = default;

	// Returns true if the event is done updating
	virtual EventUpdateResponse UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents);
	virtual std::shared_ptr<IEvent> Clone();
	virtual void ResetEvent();
	virtual EventTypes GetEventType();
	void SetCloneBaseAttributes(std::shared_ptr<IEvent>);
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
	EventTypes m_eventType;
};

#endif // !DIALOGUE_GRAPH_H__


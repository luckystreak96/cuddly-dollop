#ifndef IEVENT_H__
#define IEVENT_H__

enum EventExecutionMode
{
	BLOCKING, ASYNC
};

class IEvent
{
public:
	IEvent();

	// Returns true if the event is done updating
	virtual bool UpdateEvent(double elapsedTime);
	unsigned int GetLockLevel();
	EventExecutionMode GetExecutionMode();
protected:
	//bool m_completed;
	EventExecutionMode m_mode;
	unsigned int m_lockLevel;
};

#endif // !DIALOGUE_GRAPH_H__


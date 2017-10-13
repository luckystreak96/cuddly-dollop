#ifndef EVENT_QUEUE_H__
#define EVENT_QUEUE_H__

#include <deque>
#include "iEvent.h"
#include "jsonHandler.h"

class EventQueue
{
public:
	EventQueue();
	~EventQueue();
	void PushBack(IEvent* ev);
	unsigned int GetID();
	unsigned int Count();
	IEvent* Get(unsigned int index);
	void Remove(unsigned int index);
	bool IsRepeating();
	void SendToBack();
	void SetRepeating(bool rep);
private:
	unsigned int m_id;
	bool m_repeat;
	std::deque<IEvent*> m_queue;
};

#endif // !EVENT_LOOP_H__


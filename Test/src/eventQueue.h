#ifndef EVENT_QUEUE_H__
#define EVENT_QUEUE_H__

#include <deque>
//#include "iEvent.h"
#include "jsonHandler.h"

class IEvent;

class EventQueue
{
public:
	EventQueue(int id = -1);
	~EventQueue();
	void PushBack(IEvent* ev);
	int GetID();
	unsigned int Count();
	IEvent* Get(unsigned int index);
	void Remove(unsigned int index);
	bool IsRepeating();
	void SendToBack();
	void SetRepeating(bool rep);
private:
	int m_id;
	bool m_repeat;
	std::deque<IEvent*> m_queue;
};

#endif // !EVENT_LOOP_H__


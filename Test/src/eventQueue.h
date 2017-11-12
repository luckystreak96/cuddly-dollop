#ifndef EVENT_QUEUE_H__
#define EVENT_QUEUE_H__

#include <deque>
//#include "iEvent.h"
#include "jsonHandler.h"

class IEvent;

enum ActivationType { AT_Autorun, AT_Interact, AT_Touch };

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
	ActivationType GetActivationType();
	void SetActivationType(ActivationType at);
private:
	int m_id;
	ActivationType m_activation;
	bool m_repeat;

	std::deque<IEvent*> m_queue;
};

#endif // !EVENT_LOOP_H__


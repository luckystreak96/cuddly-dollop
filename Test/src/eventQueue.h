#ifndef EVENT_QUEUE_H__
#define EVENT_QUEUE_H__

#include <deque>
#include <memory>
//#include "iEvent.h"
#include "jsonHandler.h"

class IEvent;

enum ActivationType { AT_Autorun, AT_Interact, AT_Touch };

class EventQueue
{
public:
	EventQueue(int id = -1);
	~EventQueue();
	void PushBack(std::shared_ptr<IEvent> ev);
	int GetID();
	unsigned int Count();
	std::shared_ptr<IEvent> Get(unsigned int index);
	void Remove(unsigned int index);
	bool IsRepeating();
	void SendToBack();
	void SetRepeating(bool rep);
	ActivationType GetActivationType();
	void SetActivationType(ActivationType at);
	void ClearEvents();
private:
	int m_id;
	ActivationType m_activation;
	bool m_repeat;

	std::deque<std::shared_ptr<IEvent>> m_queue;
};

#endif // !EVENT_LOOP_H__


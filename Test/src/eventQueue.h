#ifndef EVENT_QUEUE_H__
#define EVENT_QUEUE_H__

#include <deque>
#include "iEvent.h"
#include "input_manager.h"

class EventQueue
{
public:
	EventQueue();
	void PushBack(IEvent* ev);
	void Update(double elapsedTime);
	bool Complete();
protected:
	void Erase(unsigned int index);
	void UnlockInput();
protected:
	std::deque<IEvent*> m_queue;
	static std::vector<IEvent*> m_locks;
};

#endif // !DIALOGUE_GRAPH_H__


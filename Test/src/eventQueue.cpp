#include "eventQueue.h"


EventQueue::EventQueue() : m_repeat(false), m_id(0)
{

}

EventQueue::~EventQueue()
{
	//for (auto x : m_queue)
	//	if (x)
	//		delete x;
}

void EventQueue::PushBack(IEvent* ev)
{
	m_queue.push_back(ev);
}


unsigned int EventQueue::GetID()
{
	return m_id;
}

unsigned int EventQueue::Count()
{
	return m_queue.size();
}

IEvent* EventQueue::Get(unsigned int index)
{
	if (Count() > index)
		return m_queue.at(index);

	return NULL;
}

void EventQueue::Remove(unsigned int index)
{
	delete m_queue.at(index);
	m_queue.erase(m_queue.begin() + index);
}

bool EventQueue::IsRepeating()
{
	return m_repeat;
}


void EventQueue::SendToBack()
{
	// Make sure that you dont go on forever lol
	for (int i = 0; i < Count(); i++)
	{
		IEvent* x = m_queue.front();
		m_queue.pop_front();
		x->ResetEvent();
		m_queue.push_back(x);
		if (x->GetExecutionMode() == EventExecutionMode::BLOCKING)
			break;
	}
}

void EventQueue::SetRepeating(bool rep)
{
	m_repeat = rep;
}

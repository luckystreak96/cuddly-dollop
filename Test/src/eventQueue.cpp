#include "eventQueue.h"


EventQueue::EventQueue(int id) : m_repeat(false), m_id(id), m_activation(AT_Interact)
{

}

EventQueue::~EventQueue()
{
}

void EventQueue::ClearEvents()
{
	m_queue.clear();
}

void EventQueue::PushBack(std::shared_ptr<IEvent> ev)
{
	m_queue.push_back(ev);
}


int EventQueue::GetID()
{
	return m_id;
}

unsigned int EventQueue::Count()
{
	return m_queue.size();
}

std::shared_ptr<IEvent> EventQueue::Get(unsigned int index)
{
	if (Count() > index)
		return m_queue.at(index);

	return NULL;
}

void EventQueue::Remove(unsigned int index)
{
	m_queue.erase(m_queue.begin() + index);
}

bool EventQueue::IsRepeating()
{
	return m_repeat;
}


void EventQueue::SendToBack()
{
	// Make sure that you dont go on forever lol
	std::shared_ptr<IEvent> x = m_queue.front();
	m_queue.pop_front();
	m_queue.push_back(x);
}

void EventQueue::SetRepeating(bool rep)
{
	m_repeat = rep;
}

ActivationType EventQueue::GetActivationType()
{
	return m_activation;
}

void EventQueue::SetActivationType(ActivationType at)
{
	m_activation = at;
}


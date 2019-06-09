#include "eventQueue.h"
#include "iEvent.h"


EventQueue::EventQueue(int id) : m_repeat(false), m_id(id), m_activation(AT_Interact)
{

}

EventQueue::~EventQueue()
{
}

bool EventQueue::IsDone()
{
	for (auto x : m_queue)
		if (!x->IsCompleted())
			return false;
	return true;
}

std::shared_ptr<EventQueue> EventQueue::Clone()
{
	std::shared_ptr<EventQueue> result = std::shared_ptr<EventQueue>(new EventQueue(m_id));
	result->Condition = Condition;
	result->Flag = Flag;
	result->FlagValue = FlagValue;
	result->m_activation = m_activation;
	result->m_repeat = m_repeat;

	// Deque
	std::deque<std::shared_ptr<IEvent>> deque;
	for (auto &x : m_queue)
		deque.emplace_back(x->Clone());
	result->m_queue = deque;

	return result;
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


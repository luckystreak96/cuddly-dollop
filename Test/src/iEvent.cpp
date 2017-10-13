#include "iEvent.h"

IEvent::IEvent() : m_lockLevel(0), m_mode(ASYNC), m_completed(false) {}

//IEvent::~IEvent() {}

unsigned int IEvent::GetLockLevel()
{
	return m_lockLevel;
}

bool IEvent::UpdateEvent(double elapsedTime, std::map<unsigned int, Entity*>* ents)
{
	return m_completed;
}

EventExecutionMode IEvent::GetExecutionMode()
{
	return m_mode;
}

void IEvent::SetExecutionMode(EventExecutionMode eem)
{
	m_mode = eem;
}

void IEvent::ResetEvent()
{
	m_completed = false;
}
#include "iEvent.h"

IEvent::IEvent() : m_lockLevel(0), m_mode(ASYNC), m_completed(false) {}

//IEvent::~IEvent() {}

unsigned int IEvent::GetLockLevel()
{
	return m_lockLevel;
}

EventUpdateResponse IEvent::UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents)
{
	EventUpdateResponse eur = EventUpdateResponse();
	eur.IsDone = m_completed;

	return eur;
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

bool IEvent::IsCompleted()
{
	return m_completed;
}

EventTypes IEvent::GetEventType()
{
	return m_eventType;
}
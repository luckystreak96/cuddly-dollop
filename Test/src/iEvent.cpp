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

std::shared_ptr<IEvent> IEvent::Clone()
{
	std::shared_ptr<IEvent> result = std::shared_ptr<IEvent>(new IEvent());
	return result;
}

void IEvent::SetCloneBaseAttributes(std::shared_ptr<IEvent> ev)
{
	ev->m_completed = m_completed;
	ev->m_eventType = m_eventType;
	ev->m_lockLevel = m_lockLevel;
	ev->m_mode = m_mode;
	ev->m_target = m_target;
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
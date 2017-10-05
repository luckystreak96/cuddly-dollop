#include "iEvent.h"

IEvent::IEvent() : m_lockLevel(0), m_mode(ASYNC), m_completed(false) {}

//IEvent::~IEvent() {}

unsigned int IEvent::GetLockLevel()
{
	return m_lockLevel;
}

bool IEvent::UpdateEvent(double elapsedTime)
{
	return m_completed;
}

EventExecutionMode IEvent::GetExecutionMode()
{
	return m_mode;
}

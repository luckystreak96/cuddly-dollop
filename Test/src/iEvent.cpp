#include "iEvent.h"

//bool IEvent::IsDone()
//{
//	return m_completed;
//}

IEvent::IEvent() : m_lockLevel(0), m_mode(ASYNC) {}

unsigned int IEvent::GetLockLevel()
{
	return m_lockLevel;
}

bool IEvent::UpdateEvent(double elapsedTime)
{
	return false;
}

EventExecutionMode IEvent::GetExecutionMode()
{
	return m_mode;
}

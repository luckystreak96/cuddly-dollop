#include "eventCaller.h"

EventCaller::EventCaller(unsigned int entity_id, unsigned int queue_id) : m_targetQueue(queue_id)
{
	m_target = entity_id;
	m_lockLevel = 0;
	m_mode = EventExecutionMode::BLOCKING;
}

// Update method is in EventFactory

void EventCaller::ResetEvent()
{
	IEvent::ResetEvent();
}

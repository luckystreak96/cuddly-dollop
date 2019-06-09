#include "eventCaller.h"

EventCaller::EventCaller(unsigned int entity_id, unsigned int queue_id) : m_targetQueue(queue_id)
{
	m_target = entity_id;
	m_lockLevel = 0;
	m_mode = EventExecutionMode::BLOCKING;
}

std::shared_ptr<IEvent> EventCaller::Clone()
{
	std::shared_ptr<IEvent> result = std::shared_ptr<IEvent>(new EventCaller(m_target, m_targetQueue));
	SetCloneBaseAttributes(result);
	return result;
}

// Update method is in EventFactory

void EventCaller::ResetEvent()
{
	m_firstTime = true;
	IEvent::ResetEvent();
}

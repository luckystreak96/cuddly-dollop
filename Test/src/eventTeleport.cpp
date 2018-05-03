#include "eventTeleport.h"

#include "mathutils.h"

EventTeleport::EventTeleport(unsigned int entity_id, float x, float y, float z) : m_x(x), m_y(y), m_z(z)
{
	m_target = entity_id;
	m_lockLevel = 0;
	m_mode = EventExecutionMode::ASYNC;
}

EventUpdateResponse EventTeleport::UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents)
{
	EventUpdateResponse eur = EventUpdateResponse();
	eur.IsDone = true;

	ents->at(m_target)->Physics()->AbsolutePosition(Vector3f(m_x, m_y, m_z));

	m_completed = true;
	return eur;
}

std::shared_ptr<IEvent> EventTeleport::Clone()
{
	std::shared_ptr<IEvent> result = std::shared_ptr<IEvent>(new EventTeleport(m_target, m_x, m_y, m_z));
	SetCloneBaseAttributes(result);
	return result;
}

void EventTeleport::ResetEvent()
{
	IEvent::ResetEvent();
}

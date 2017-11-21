#include "eventMove.h"

EventMove::EventMove(unsigned int entity_id, float distance, unsigned int direction) : m_firstSetup(true), m_distance(distance), m_direction(direction)
{
	m_target = entity_id;
	m_moveAxis = (direction == 0 || direction == 2) ? 1 : 0;
	m_lockLevel = 0;
	m_mode = EventExecutionMode::ASYNC;
}

EventUpdateResponse EventMove::UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents)
{
	EventUpdateResponse eur = EventUpdateResponse();
	eur.IsDone = true;
	if (m_completed)
		return eur;

	// If pos hasnt been set yet
	if (m_firstSetup)
	{
		m_startPos = ents->at(m_target)->Physics()->Position();
		m_firstSetup = false;
	}
	if (m_target == 1)
		int lol = 69;
	ents->at(m_target)->Physics()->ActionMove(m_direction == 0, m_direction == 2, m_direction == 3, m_direction == 1);

	if (abs(ents->at(m_target)->Physics()->Position()[m_moveAxis] - m_startPos[m_moveAxis]) >= m_distance * 0.97f)//The character will slide a little after reaching destination
	{
		//ents->at(m_target)->Physics()->AbsolutePosition(m_startPos + ((m_direction == 2 || m_direction == 3) ? -m_distance : m_distance), Vector3f(m_moveAxis == 0 ? 1 : 0, m_moveAxis == 1 ? 1 : 0, m_moveAxis == 2 ? 1 : 0));
		ents->at(m_target)->Physics()->RemoveVelocity();
		ents->at(m_target)->Physics()->SetConversationLock(false);
		m_completed = true;
		return eur;
	}

	eur.IsDone = false;
	return eur;
}

void EventMove::ResetEvent()
{
	m_firstSetup = true;
	IEvent::ResetEvent();
}

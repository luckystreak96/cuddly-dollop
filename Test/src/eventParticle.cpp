#include "eventParticle.h"

EventParticle::EventParticle(int particleCount, ParticleType type, unsigned int entity_id) : m_firstSetup(true), m_particleType(type), m_count(particleCount)
{
	m_target = entity_id;
	m_lockLevel = 0;
	m_mode = EventExecutionMode::ASYNC;
}

EventUpdateResponse EventParticle::UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents)
{
	EventUpdateResponse eur = EventUpdateResponse();
	eur.IsDone = true;
	if (m_completed)
		return eur;

	Vector3f pos;

	if (ents->count(m_target))
		pos = ents->at(m_target)->Physics()->Position() + Vector3f(ents->at(m_target)->Physics()->GetSize().x / 1.2f, ents->at(m_target)->Physics()->GetSize().y * 2.0f + 0.1f, 0.6f);

	// If pos hasnt been set yet
	if (m_firstSetup)
	{
		m_particles.Init(m_particleType, m_count, pos);
		m_firstSetup = false;
	}

	m_particles.Update(pos);
	m_particles.SetRender();

	eur.IsDone = false;
	return eur;
}

void EventParticle::SetMapSize(Vector3f mapsize)
{
	m_mapSize = mapsize;
}


void EventParticle::ResetEvent()
{
	m_firstSetup = true;
	IEvent::ResetEvent();
}

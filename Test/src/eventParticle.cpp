#include "eventParticle.h"

EventParticle::EventParticle(int particleCount, ParticleType type, unsigned int entity_id, std::string sprite_override)
	: m_firstSetup(true), m_particleType(type), m_count(particleCount), m_sprite(sprite_override)
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
	{
		switch (m_particleType)
		{
		case PT_Explosion:
			pos = ents->at(m_target)->Physics()->Position() + Vector3f(ents->at(m_target)->Physics()->GetSize().x / 2.0f, 0.3f, 0.6f);
			break;
		default:
			pos = ents->at(m_target)->Physics()->Position() + Vector3f(ents->at(m_target)->Physics()->GetSize().x * 0.9f, ents->at(m_target)->Physics()->GetSize().y * 1.25f, 0.6f);
		}
	}

	// If pos hasnt been set yet
	if (m_firstSetup)
	{
		m_particles.Init(m_particleType, m_count, pos, false, m_sprite);
		m_firstSetup = false;
	}

	m_particles.Update(pos);

	if (m_particles.completed)
	{
		m_completed = true;
		return eur;
	}

	m_particles.SetRender();

	eur.IsDone = false;
	return eur;
}

std::shared_ptr<IEvent> EventParticle::Clone()
{
	std::shared_ptr<IEvent> result = std::shared_ptr<IEvent>(new EventParticle(m_count, m_particleType, m_target, m_sprite));
	dynamic_cast<EventParticle*>(result.get())->SetPower(m_power);
	SetCloneBaseAttributes(result);
	return result;
}


void EventParticle::SetMapSize(Vector3f mapsize)
{
	m_mapSize = mapsize;
}

void EventParticle::SetPower(float power)
{
	m_particles.SetPowerLevel(power);
	m_power = power;
}


void EventParticle::ResetEvent()
{
	m_firstSetup = true;
	m_particles = ParticleGenerator();
	m_particles.SetPowerLevel(m_power);
	IEvent::ResetEvent();
}

#include "eventWeather.h"

EventWeather::EventWeather(int particleCount, ParticleType type, Vector3f mapsize, bool smooth, std::string sprite) 
	: m_firstSetup(true), m_particleType(type), m_mapSize(mapsize), m_count(particleCount), m_smooth(smooth), m_sprite(sprite)
{
	m_target = 0;
	m_lockLevel = 0;
	m_mode = EventExecutionMode::ASYNC;
}

EventUpdateResponse EventWeather::UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents)
{
	EventUpdateResponse eur = EventUpdateResponse();
	eur.IsDone = true;
	if (m_completed)
		return eur;

	// If pos hasnt been set yet
	if (m_firstSetup)
	{
		m_particles.Init(m_particleType, m_count, m_mapSize, m_smooth, m_sprite);
		m_firstSetup = false;
	}

	m_particles.Update();
	m_particles.SetRender();

	eur.IsDone = false;
	return eur;
}

std::shared_ptr<IEvent> EventWeather::Clone()
{
	std::shared_ptr<IEvent> result = std::shared_ptr<IEvent>(new EventWeather(m_count, m_particleType, m_mapSize, m_smooth));
	SetCloneBaseAttributes(result);
	return result;
}

void EventWeather::SetMapSize(Vector3f mapsize)
{
	m_mapSize = mapsize;
}


void EventWeather::ResetEvent()
{
	m_firstSetup = true;
	IEvent::ResetEvent();
}

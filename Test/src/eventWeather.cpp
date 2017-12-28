#include "eventWeather.h"

EventWeather::EventWeather(int particleCount, ParticleType type, Vector3f mapsize, bool smooth) 
	: m_firstSetup(true), m_particleType(type), m_mapSize(mapsize), m_count(particleCount), m_smooth(smooth)
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
		m_particles.Init(m_particleType, m_count, m_mapSize, m_smooth);
		m_firstSetup = false;
	}

	m_particles.Update();
	m_particles.SetRender();

	eur.IsDone = false;
	return eur;
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

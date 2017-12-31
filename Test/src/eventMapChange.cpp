#include "eventMapChange.h"
#include "scene_world.h"

EventMapChange::EventMapChange(unsigned int map) : m_map(map), m_firstSetup(true)
{
	m_lockLevel = 5;
	m_mode = EventExecutionMode::BLOCKING;
}

EventUpdateResponse EventMapChange::UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents)
{
	EventUpdateResponse eur = EventUpdateResponse();
	eur.IsDone = true;

	if (m_firstSetup)
	{
		// Start a fadeout?
		m_firstSetup = false;
		SceneGenData sgd;
		sgd.id = m_map;
		sgd.sceneType = ST_World;
		SceneWorld::SetNextScene(sgd);
	}

	return eur;
}

std::shared_ptr<IEvent> EventMapChange::Clone()
{
	std::shared_ptr<IEvent> result = std::shared_ptr<IEvent>(new EventMapChange(m_map));
	SetCloneBaseAttributes(result);
	return result;
}

void EventMapChange::ResetEvent()
{
	IEvent::ResetEvent();
}

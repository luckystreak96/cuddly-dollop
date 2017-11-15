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
	}
	else if(1/*fadeout is done*/)
	{
		SceneWorld::SetNextScene(std::shared_ptr<Scene>(new SceneWorld(m_map)));
		return eur;
	}

	eur.IsDone = false;
	return eur;
}


void EventMapChange::ResetEvent()
{
	IEvent::ResetEvent();
}

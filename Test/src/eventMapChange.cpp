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
	//eur.IsDone = true;

	if (m_firstSetup)
	{
		// Start a fadeout?
		m_firstSetup = false;
		SceneGenData sgd;
		sgd.id = m_map;
		sgd.sceneType = ST_World;
		SceneWorld::SetNextScene(sgd);
	}
	//else if(1/*fadeout is done*/)
	//{
		//m_completed = true;
		//eur.IsDone = false;
		//return eur;
	//}

	eur.IsDone = false;
	return eur;
}


void EventMapChange::ResetEvent()
{
	IEvent::ResetEvent();
}

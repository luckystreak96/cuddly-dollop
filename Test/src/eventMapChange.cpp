#include "eventMapChange.h"
#include "scene_world.h"

EventMapChange::EventMapChange(unsigned int map) : m_map(map)
{
	m_lockLevel = 0;
	m_mode = EventExecutionMode::BLOCKING;
}

EventUpdateResponse EventMapChange::UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents)
{
	EventUpdateResponse eur = EventUpdateResponse();
	eur.IsDone = true;
	std::shared_ptr<Scene> s = std::shared_ptr<Scene>(new SceneWorld(m_map));
	SceneWorld::SetNextScene(s);
	return eur;
}

void EventMapChange::ResetEvent()
{
	IEvent::ResetEvent();
}

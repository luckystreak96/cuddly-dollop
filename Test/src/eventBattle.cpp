#include "eventBattle.h"
#include "scene_world.h"

EventBattle::EventBattle() : m_firstSetup(true)
{
	m_lockLevel = 5;
	m_mode = EventExecutionMode::BLOCKING;
}

EventUpdateResponse EventBattle::UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents)
{
	EventUpdateResponse eur = EventUpdateResponse();
	eur.IsDone = true;

	if (m_firstSetup)
	{
		// Start a fadeout?
		m_firstSetup = false;
		SceneGenData sgd;
		sgd.id = 1;
		sgd.sceneType = ST_Battle;
		SceneWorld::SetNextScene(sgd);
	}

	if (SceneWorld::NextScene.id != 0)
	{
		eur.IsDone = false;
		return eur;
	}

	m_completed = true;
	return eur;
}

std::shared_ptr<IEvent> EventBattle::Clone()
{
	std::shared_ptr<IEvent> result = std::shared_ptr<IEvent>(new EventBattle());
	SetCloneBaseAttributes(result);
	return result;
}

void EventBattle::ResetEvent()
{
	IEvent::ResetEvent();
}

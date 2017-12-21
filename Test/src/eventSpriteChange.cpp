#include "eventSpriteChange.h"
#include "gameData.h"

EventSpriteChange::EventSpriteChange(std::string newSprite, int target) : m_newSprite(newSprite)
{
	m_target = target;
	m_lockLevel = 0;
	m_mode = EventExecutionMode::ASYNC;
}

EventUpdateResponse EventSpriteChange::UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents)
{
	EventUpdateResponse eur = EventUpdateResponse();
	eur.IsDone = true;

	if (m_target == 1)
		GameData::PlayerSprite = m_newSprite;

	if (ents->count(m_target))
		ents->at(m_target)->Graphics()->SetTexture(m_newSprite);

	m_completed = true;
	return eur;
}

void EventSpriteChange::ResetEvent()
{
	IEvent::ResetEvent();
}

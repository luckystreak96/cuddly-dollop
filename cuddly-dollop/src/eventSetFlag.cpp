#include "eventSetFlag.h"
#include "gameData.h"

EventSetFlag::EventSetFlag(std::string name, int value) : m_flag(name), m_flagValue(value)
{
	m_lockLevel = 0;
	m_mode = EventExecutionMode::ASYNC;
}

EventUpdateResponse EventSetFlag::UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents)
{
	EventUpdateResponse eur = EventUpdateResponse();
	eur.IsDone = true;

	GameData::EmplaceFlag(m_flag, m_flagValue);

	m_completed = true;
	return eur;
}

std::shared_ptr<IEvent> EventSetFlag::Clone()
{
	std::shared_ptr<IEvent> result = std::shared_ptr<IEvent>(new EventSetFlag(m_flag, m_flagValue));
	SetCloneBaseAttributes(result);
	return result;
}

void EventSetFlag::ResetEvent()
{
	IEvent::ResetEvent();
}

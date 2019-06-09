#include "eventAddToFlag.h"
#include "gameData.h"

EventAddToFlag::EventAddToFlag(std::string name, int value) : m_flag(name), m_value(value)
{
	m_lockLevel = 0;
	m_mode = EventExecutionMode::ASYNC;
}

EventUpdateResponse EventAddToFlag::UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents)
{
	EventUpdateResponse eur = EventUpdateResponse();
	eur.IsDone = true;

	if (GameData::Flags.count(m_flag))
		GameData::EmplaceFlag(m_flag, GameData::Flags.at(m_flag) + m_value);
	else
		GameData::EmplaceFlag(m_flag, m_value);

	m_completed = true;
	return eur;
}

std::shared_ptr<IEvent> EventAddToFlag::Clone()
{
	std::shared_ptr<IEvent> result = std::shared_ptr<IEvent>(new EventAddToFlag(m_flag, m_value));
	SetCloneBaseAttributes(result);
	return result;
}

void EventAddToFlag::ResetEvent()
{
	IEvent::ResetEvent();
}

#include "eventToggleFlag.h"
#include "gameData.h"

EventToggleFlag::EventToggleFlag(std::string name) : m_flag(name)
{
	m_lockLevel = 0;
	m_mode = EventExecutionMode::ASYNC;
}

EventUpdateResponse EventToggleFlag::UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents)
{
	EventUpdateResponse eur = EventUpdateResponse();
	eur.IsDone = true;

	if (GameData::Flags.count(m_flag))
	{
		switch (GameData::Flags.at(m_flag))
		{
		case 0:
			GameData::EmplaceFlag(m_flag, 1);
			break;
		default:
			GameData::EmplaceFlag(m_flag, 0);
			break;
		}
	}
	else
		GameData::EmplaceFlag(m_flag, 1);

	m_completed = true;
	return eur;
}

void EventToggleFlag::ResetEvent()
{
	IEvent::ResetEvent();
}

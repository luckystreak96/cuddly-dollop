#include "eventBGM.h"
#include "soundManager.h"

EventBGM::EventBGM(std::string soundFile) : m_soundFile(soundFile), m_firstTime(true)
{
	//m_target = entity_id;
	m_lockLevel = 0;
	m_mode = EventExecutionMode::ASYNC;
}

EventBGM::~EventBGM()
{
}

EventUpdateResponse EventBGM::UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents)
{
	EventUpdateResponse eur = EventUpdateResponse();
	eur.IsDone = true;

	if (m_firstTime)
	{
		SoundManager::GetInstance().SetBGM(m_soundFile);
		m_firstTime = false;
	}

	m_completed = true;
	return eur;
}

void EventBGM::ResetEvent()
{
	IEvent::ResetEvent();
	m_firstTime = true;
}

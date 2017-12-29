#include "eventSound.h"
#include "soundManager.h"

EventSound::EventSound(std::string soundFile) : m_soundFile(soundFile), m_firstTime(true)
{
	//m_target = entity_id;
	m_lockLevel = 0;
	m_mode = EventExecutionMode::ASYNC;
}

EventSound::~EventSound()
{
	SoundManager::GetInstance().Stop(m_source);
	SoundManager::GetInstance().DeleteSource(m_source);
}

EventUpdateResponse EventSound::UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents)
{
	EventUpdateResponse eur = EventUpdateResponse();
	eur.IsDone = true;

	if (m_firstTime)
	{
		m_source = SoundManager::GetInstance().CreateSource();
		SoundManager::GetInstance().CreateBuffer(m_soundFile);
		SoundManager::GetInstance().Play(m_source, m_soundFile);
		m_firstTime = false;
	}

	if (SoundManager::GetInstance().IsPlaying(m_source))
	{
		eur.IsDone = false;
		return eur;
	}

	SoundManager::GetInstance().Stop(m_source);
	SoundManager::GetInstance().DeleteSource(m_source);

	m_completed = true;
	return eur;
}

std::shared_ptr<IEvent> EventSound::Clone()
{
	std::shared_ptr<IEvent> result = std::shared_ptr<IEvent>(new EventSound(m_soundFile));
	SetCloneBaseAttributes(result);
	return result;
}

void EventSound::ResetEvent()
{
	IEvent::ResetEvent();
	m_firstTime = true;
}

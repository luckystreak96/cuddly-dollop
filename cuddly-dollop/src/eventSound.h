#ifndef EVENT_SOUND_H__
#define EVENT_SOUND_H__

#include "iEvent.h"
#include "entity.h"

class EventSound : public IEvent
{
public:
	EventSound(std::string file);
	~EventSound();
	EventUpdateResponse UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents);
	std::shared_ptr<IEvent> Clone();
	void ResetEvent();

private:
	std::string m_soundFile;
	unsigned int m_source;
	bool m_firstTime;
};

#endif // !EVENT_SOUND_H__

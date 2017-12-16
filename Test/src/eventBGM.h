#ifndef EVENT_BGM_H__
#define EVENT_BGM_H__

#include "iEvent.h"
#include "entity.h"

class EventBGM : public IEvent
{
public:
	EventBGM(std::string file);
	~EventBGM();
	EventUpdateResponse UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents);
	void ResetEvent();

private:
	std::string m_soundFile;
	bool m_firstTime;
};

#endif // !EVENT_SOUND_H__

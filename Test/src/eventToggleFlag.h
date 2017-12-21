#ifndef EVENT_TOGGLE_FLAG_H__
#define EVENT_TOGGLE_FLAG_H__

#include "iEvent.h"
#include "entity.h"

class EventToggleFlag : public IEvent
{
public:
	EventToggleFlag(std::string name);
	~EventToggleFlag() {}
	EventUpdateResponse UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents);
	void ResetEvent();

private:
	std::string m_flag;
};

#endif // !EVENT_SET_FLAG_H__

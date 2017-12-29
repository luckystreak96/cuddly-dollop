#ifndef EVENT_ADD_TO_FLAG_H__
#define EVENT_ADD_TO_FLAG_H__

#include "iEvent.h"
#include "entity.h"

class EventAddToFlag : public IEvent
{
public:
	EventAddToFlag(std::string name, int value);
	~EventAddToFlag() {}
	EventUpdateResponse UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents);
	std::shared_ptr<IEvent> Clone();
	void ResetEvent();

private:
	std::string m_flag;
	int m_value;
};

#endif // !EVENT_SET_FLAG_H__

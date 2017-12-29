#ifndef EVENT_SET_FLAG_H__
#define EVENT_SET_FLAG_H__

#include "iEvent.h"
#include "entity.h"

class EventSetFlag : public IEvent
{
public:
	EventSetFlag(std::string name, int value);
	~EventSetFlag() {}
	EventUpdateResponse UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents);
	std::shared_ptr<IEvent> Clone();
	void ResetEvent();

private:
	std::string m_flag;
	int m_flagValue;
};

#endif // !EVENT_SET_FLAG_H__

#ifndef EVENT_CALLER_H__
#define EVENT_CALLER_H__

#include "iEvent.h"
#include "entity.h"

class EventCaller : public IEvent
{
public:
	EventCaller(unsigned int entity_id, unsigned int queue_id);
	~EventCaller() {}
	EventUpdateResponse UpdateEvent(double elapsedTime, std::map<unsigned int, Entity*>* ents);
	void ResetEvent();

private:
	unsigned int m_targetQueue;
	std::shared_ptr<EventQueue> m_queue;
	bool m_firstTime;
};

#endif // !DIALOGUE_BOX_H__

#ifndef EVENT_TELEPORT_H__
#define EVENT_TELEPORT_H__

#include "iEvent.h"
#include "entity.h"

class EventTeleport : public IEvent
{
public:
	EventTeleport(unsigned int entity_id, float x, float y, float z);
	~EventTeleport() {}
	EventUpdateResponse UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents);
	std::shared_ptr<IEvent> Clone();
	void ResetEvent();

private:
	float m_x, m_y, m_z;
};

#endif // !DIALOGUE_BOX_H__

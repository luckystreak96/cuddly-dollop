#ifndef EVENT_MAP_CHANGE_H__
#define EVENT_MAP_CHANGE_H__

#include "iEvent.h"
#include "entity.h"
#include <thread>
#include <mutex>
#include "scene.h"

class EventMapChange : public IEvent
{
public:
	EventMapChange(unsigned int map);
	~EventMapChange() {}
	EventUpdateResponse UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents);
	std::shared_ptr<IEvent> Clone();
	void ResetEvent();

private:
	unsigned int m_map;
	bool m_firstSetup;
};

#endif // !DIALOGUE_BOX_H__

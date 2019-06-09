#ifndef EVENT_BATTLE_H__
#define EVENT_BATTLE_H__

#include "iEvent.h"
#include "entity.h"
#include <thread>
#include <mutex>
#include "scene.h"

class EventBattle : public IEvent
{
public:
	EventBattle();
	~EventBattle() {}
	EventUpdateResponse UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents);
	std::shared_ptr<IEvent> Clone();
	void ResetEvent();

private:
	//unsigned int m_map;
	bool m_firstSetup;
};

#endif // !DIALOGUE_BOX_H__

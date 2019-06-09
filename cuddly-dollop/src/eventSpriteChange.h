#ifndef EVENT_SPRITE_CHANGE_H__
#define EVENT_SPRITE_CHANGE_H__

#include "iEvent.h"
#include "entity.h"

class EventSpriteChange : public IEvent
{
public:
	EventSpriteChange(std::string newSprite, int entityID);
	~EventSpriteChange() {}
	EventUpdateResponse UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents);
	std::shared_ptr<IEvent> Clone();
	void ResetEvent();

private:
	std::string m_newSprite;
};

#endif // !EVENT_SET_FLAG_H__
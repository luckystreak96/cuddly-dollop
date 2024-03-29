#ifndef EVENT_MOVE_H__
#define EVENT_MOVE_H__

#include "iEvent.h"
#include "entity.h"

class EventMove : public IEvent
{
public:
	EventMove(unsigned int entity_id, float distance = 3.0f, unsigned int direction = 2);
	~EventMove() {}
	EventUpdateResponse UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents);
	std::shared_ptr<IEvent> Clone();
	void ResetEvent();

private:
	//unsigned int m_target;
	bool m_firstSetup;
	int m_moveAxis;
	float m_distance;
	unsigned int m_direction;
	Vector3f m_startPos;
};

#endif // !DIALOGUE_BOX_H__

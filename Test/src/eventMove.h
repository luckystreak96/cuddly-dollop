#ifndef EVENT_MOVE_H__
#define EVENT_MOVE_H__

#include "iEvent.h"
#include "entity.h"

class EventMove : public IEvent
{
public:
	EventMove(unsigned int entity_id, float distance = 3.0f, unsigned int direction = 2);
	~EventMove() {}
	bool UpdateEvent(double elapsedTime, std::map<unsigned int, Entity*>* ents);
	void ResetEvent();

private:
	unsigned int m_target;
	bool m_firstSetup;
	int m_moveAxis;
	float m_distance;
	unsigned int m_direction;
	Vector3f m_startPos;
};

#endif // !DIALOGUE_BOX_H__

#ifndef PHYSICS_RPG_H__
#define PHYSICS_RPG_H__

#include "physics_type.h"

class physics_rpg : public physics_type
{
public:
	physics_rpg();
	void calculate_collision();
	void calculate_position_adjustments();

	void notify_ptr_added(PhysicsComponent* ptr);
	void notify_ptr_removed(PhysicsComponent* ptr);

private:
	bool m_initialized;
};

#endif
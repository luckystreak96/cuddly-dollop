#ifndef PHYSICS_TYPE_H__
#define PHYSICS_TYPE_H__

#include "physicsComponent.h"
#include <set>

class physics_type
{
public:
	physics_type() {}

	virtual void calculate_collision() = 0;
	virtual void calculate_position_adjustments() = 0;

	virtual void notify_ptr_added(PhysicsComponent* ptr) {}
	virtual void notify_ptr_removed(PhysicsComponent* ptr) {}

	void set_objects(std::set<PhysicsComponent*>* comp) { m_components = comp; }

protected:
	std::set<PhysicsComponent*>* m_components;
};

#endif
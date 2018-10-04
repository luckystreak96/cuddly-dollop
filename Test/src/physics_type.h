#ifndef PHYSICS_TYPE_H__
#define PHYSICS_TYPE_H__

#include "physicsComponent.h"
#include <vector>

class physics_type
{
public:
	physics_type() {}

	virtual void calculate_collision() = 0;
	virtual void calculate_position_adjustments() = 0;

	void set_objects(std::vector<PhysicsComponent*>* comp) { m_components = comp; }

protected:
	std::vector<PhysicsComponent*>* m_components;
};

#endif
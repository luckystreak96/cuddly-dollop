#ifndef PHYSICS_MANAGER_H__
#define PHYSICS_MANAGER_H__

#include "physics_type.h"
#include "physicsComponent.h"
#include "observer.h"

class physics_manager : public Observer
{
public:
	physics_manager(physics_type* engine);
	~physics_manager();

	void calculate_frame();

private:

private:
	std::vector<PhysicsComponent*> m_components;
	physics_type* m_engine;
};


#endif